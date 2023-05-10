#include "TRGB.h"
#include "system.h"
#include "pin_config.h"
#include "./hardware_drivers/XL9535_driver.h"
#include "./system_utils/lv_fs_cb.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"

#include <string>
#include <Arduino.h>
#include "lvgl.h"
#include "TouchLib.h"
#include "SD_MMC.h"
#include "Wire.h"
#include "SPI.h"
#include "WiFi.h"

lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
lv_disp_drv_t disp_drv;      // lvgl display driver
lv_indev_drv_t indev_drv;    // lvgl touch panel driver
lv_fs_drv_t fs_drv;          // lvgl file system driver

/*
 * -----------------------------------------------------------------------------
 * Utility functions
 * -----------------------------------------------------------------------------
*/

esp_lcd_panel_handle_t panel_handle = NULL;
TouchLib touch(Wire, IIC_SDA_PIN, IIC_SCL_PIN, CTS820_SLAVE_ADDRESS);

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map) {
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
  lv_disp_flush_ready(drv);
}

static void lv_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {

  touch_point_t p = {0};
  if(touch.read()){
    TP_Point t = touch.getPoint(0);
    data->point.x = p.x = t.x;
    data->point.y = p.y = t.y;
    data->state = LV_INDEV_STATE_PR;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void interacted() {
  System.is_asleep = false;
  System.last_interact_time = millis();
}

/*
 * -----------------------------------------------------------------------------
 * TRGB Private members
 * -----------------------------------------------------------------------------
*/

void TRGB::tft_init(void) {
  xl.digitalWrite(LCD_CS_PIN, 1);
  xl.digitalWrite(LCD_SDA_PIN, 1);
  xl.digitalWrite(LCD_CLK_PIN, 1);

  // Reset the display
  xl.digitalWrite(LCD_RST_PIN, 1);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  xl.digitalWrite(LCD_RST_PIN, 0);
  vTaskDelay(200 / portTICK_PERIOD_MS);
  xl.digitalWrite(LCD_RST_PIN, 1);
  vTaskDelay(200 / portTICK_PERIOD_MS);

  // Setup register for driving tft display
  int cmd = 0;
  while (st_init_cmds[cmd].databytes != 0xff) {
    lcd_cmd(st_init_cmds[cmd].cmd);
    lcd_data(st_init_cmds[cmd].data, st_init_cmds[cmd].databytes & 0x1F);
    if (st_init_cmds[cmd].databytes & 0x80) {
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    cmd++;
  }
}

void TRGB::lcd_send_data(uint8_t data) {
  uint8_t n;
  for (n = 0; n < 8; n++) {
    if (data & 0x80)
      xl.digitalWrite(LCD_SDA_PIN, 1);
    else
      xl.digitalWrite(LCD_SDA_PIN, 0);

    data <<= 1;
    xl.digitalWrite(LCD_CLK_PIN, 0);
    xl.digitalWrite(LCD_CLK_PIN, 1);
  }
}

void TRGB::lcd_cmd(const uint8_t cmd) {
  xl.digitalWrite(LCD_CS_PIN, 0);
  xl.digitalWrite(LCD_SDA_PIN, 0);
  xl.digitalWrite(LCD_CLK_PIN, 0);
  xl.digitalWrite(LCD_CLK_PIN, 1);
  lcd_send_data(cmd);
  xl.digitalWrite(LCD_CS_PIN, 1);
}

void TRGB::lcd_data(const uint8_t *data, int len) {
  uint32_t i = 0;
  if (len == 0)
    return; // no need to send anything
  do {
    xl.digitalWrite(LCD_CS_PIN, 0);
    xl.digitalWrite(LCD_SDA_PIN, 1);
    xl.digitalWrite(LCD_CLK_PIN, 0);
    xl.digitalWrite(LCD_CLK_PIN, 1);
    lcd_send_data(*(data + i));
    xl.digitalWrite(LCD_CS_PIN, 1);
    i++;
  } while (len--);
}

void TRGB::invoke_wifi_task(void * task_params) {
  // Call the wifi task function
  ((wifi_task_t *) task_params)->wifi_task_func();
}

/*
 * -----------------------------------------------------------------------------
 * TRGB Public members
 * -----------------------------------------------------------------------------
*/

TRGB::TRGB(){ return; }

void TRGB::enable_backlight() {
  pinMode(EXAMPLE_PIN_NUM_BK_LIGHT, OUTPUT);
  analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, System.brightness);
}

void TRGB::SD_init() {

  // put your setup code here, to run once:
  Wire.begin(IIC_SDA_PIN, IIC_SCL_PIN, (uint32_t)400000);
  xl.begin();
  uint8_t pin = (1 << PWR_EN_PIN) | (1 << LCD_CS_PIN) | (1 << TP_RES_PIN) | (1 << LCD_SDA_PIN) | (1 << LCD_CLK_PIN) |
                (1 << LCD_RST_PIN) | (1 << SD_CS_PIN);

  xl.pinMode8(0, pin, OUTPUT);
  xl.digitalWrite(PWR_EN_PIN, 1);
  xl.digitalWrite(SD_CS_PIN, 1); // To use SDIO one-line mode, you need to pull the CS pin high
  SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN);

  if (!SD_MMC.begin("/sdcard", true, true)) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD card size: ");
  Serial.println(SD_MMC.cardSize());

}

void TRGB::lvgl_init() {

  xl.digitalWrite(TP_RES_PIN, 0);
  delay(200);
  xl.digitalWrite(TP_RES_PIN, 1);
  delay(200);
  touch.init();
  tft_init();

  esp_lcd_rgb_panel_config_t panel_config = {
      .clk_src = LCD_CLK_SRC_PLL160M,
      .timings =
          {
              .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
              .h_res = EXAMPLE_LCD_H_RES,
              .v_res = EXAMPLE_LCD_V_RES,
              // The following parameters should refer to LCD spec
              .hsync_pulse_width = 1,
              .hsync_back_porch = 30,
              .hsync_front_porch = 50,
              .vsync_pulse_width = 1,
              .vsync_back_porch = 30,
              .vsync_front_porch = 20,
              .flags =
                  {
                      .pclk_active_neg = 1,
                  },
          },
      .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
      .psram_trans_align = 64,
      .hsync_gpio_num = EXAMPLE_PIN_NUM_HSYNC,
      .vsync_gpio_num = EXAMPLE_PIN_NUM_VSYNC,
      .de_gpio_num = EXAMPLE_PIN_NUM_DE,
      .pclk_gpio_num = EXAMPLE_PIN_NUM_PCLK,
      .data_gpio_nums =
          {
              // RGB565 being driven in parallel

              // 5 pins for red channel
              // EXAMPLE_PIN_NUM_DATA0,
              EXAMPLE_PIN_NUM_DATA13,
              EXAMPLE_PIN_NUM_DATA14,
              EXAMPLE_PIN_NUM_DATA15,
              EXAMPLE_PIN_NUM_DATA16,
              EXAMPLE_PIN_NUM_DATA17,

              // 6 pins for green channel
              EXAMPLE_PIN_NUM_DATA6,
              EXAMPLE_PIN_NUM_DATA7,
              EXAMPLE_PIN_NUM_DATA8,
              EXAMPLE_PIN_NUM_DATA9,
              EXAMPLE_PIN_NUM_DATA10,
              EXAMPLE_PIN_NUM_DATA11,
              // EXAMPLE_PIN_NUM_DATA12,

              // 5 pins for blue channel
              EXAMPLE_PIN_NUM_DATA1,
              EXAMPLE_PIN_NUM_DATA2,
              EXAMPLE_PIN_NUM_DATA3,
              EXAMPLE_PIN_NUM_DATA4,
              EXAMPLE_PIN_NUM_DATA5,
          },
      .disp_gpio_num = EXAMPLE_PIN_NUM_DISP_EN,
      .on_frame_trans_done = NULL,
      .user_ctx = NULL,
      .flags =
          {
              .fb_in_psram = 1, // allocate frame buffer in PSRAM
          },
  };
  ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

  // Draw logo on device startup
  //esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 480, 480, logo_img);
  //delay(2000);

  lv_init();
  lv_png_init();

  // alloc draw buffers used by LVGL from PSRAM
  lv_color_t *buf1 =
      (lv_color_t *)heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  assert(buf1);
  lv_color_t *buf2 =
      (lv_color_t *)heap_caps_malloc(EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  assert(buf2);
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES);

  // Register display driver to LVGL
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = example_lvgl_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  System.display = lv_disp_drv_register(&disp_drv);

  // Register touch panel driver to LVGL
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lv_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // Register file system driver to LVGL
  lv_fs_drv_init(&fs_drv);
  fs_drv.letter = 'S';
  fs_drv.open_cb = open_file;
  fs_drv.close_cb = close_file;
  fs_drv.read_cb = read_file;
  fs_drv.write_cb = write_file;
  fs_drv.seek_cb = seek_file;
  fs_drv.tell_cb = tell_file;

  /*
  fs_drv.dir_open_cb = SD_dir_open;
  fs_drv.dir_read_cb = SD_dir_read;
  fs_drv.dir_close_cb = SD_dir_close;
  */
  lv_fs_drv_register(&fs_drv);

  // Touchscreen interrupt pin
  pinMode(TP_INT_PIN, INPUT_PULLUP);
  attachInterrupt(TP_INT_PIN, interacted, FALLING);

}

void TRGB::wifi_task_init(void (* task_func)(), BaseType_t xCoreID) {
  wifi_task_t w = {task_func};
  // Pin wifi funcitonality to core 0 (core 1 is default core)
  xTaskCreatePinnedToCore(TRGB::invoke_wifi_task, "wifi_task", 1024 * 6,
                          (void *) &w, 1, &wifi_task_handle, 0);
}

void TRGB::sleep() {

  // disable "wakeup" on touch
  detachInterrupt(TP_INT_PIN);

  // Mark device as asleep
  System.is_asleep = true;
  //delay(100);

  // Turn off backlight
  analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, BRIGHTNESS_OFF);

  // Reset LCD
  /*
  xl.digitalWrite(LCD_RST_PIN, 0);
  delay(50);
  xl.digitalWrite(LCD_RST_PIN, 1);
  */

  // If the app list is open, switch the active screen to the main home screen
  if (System.app_list_tile == lv_tileview_get_tile_act(System.main_tile_view)) {
    lv_obj_set_tile(System.main_tile_view, System.info_tile, LV_ANIM_OFF);
  }

  // Clear the LCD (Hide all LVGL elements)
  lv_obj_add_flag(System.active_screen, LV_OBJ_FLAG_HIDDEN);
  lv_refr_now(System.display);

  // Enable touch "wakeup" after 500 milliseconds
  delay(500);
  attachInterrupt(TP_INT_PIN, interacted, FALLING);

  // Block thread until device is awake
  while (System.is_asleep) {
    delay(100);
  }

  // Unhide LVGL elements
  lv_obj_clear_flag(System.active_screen, LV_OBJ_FLAG_HIDDEN);
  lv_refr_now(System.display);

  // Turn the backlight on
  analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, System.brightness);

  // Update the interaction time
  System.last_interact_time = millis();

}

void TRGB::deep_sleep() {

  // End wifi task
  if (wifi_task_handle) {
    vTaskDelete(wifi_task_handle);
  }

  System.is_asleep = true;
  System.wifi_active = false;

  WiFi.disconnect();

  // Temporarily disable wakeup on touch
  detachInterrupt(TP_INT_PIN);

  // Turn off backlight
  analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, BRIGHTNESS_OFF);

  // If the SD card is initialized, it needs to be unmounted.
  if (SD_MMC.cardSize())
    SD_MMC.end();

  // Delay temporarily to prevent waking up from the shutdown button touch
  delay(1000);

  // Enable deep sleep wakeup on touch
  esp_sleep_enable_ext0_wakeup((gpio_num_t)TP_INT_PIN, 0);

  // Enter deep sleep mode
  esp_deep_sleep_start();

}

void shutdown() {
  //trgb.deep_sleep();
  trgb.sleep();
}