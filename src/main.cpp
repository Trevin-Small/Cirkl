// Library Includes
#include <string>
#include <Arduino.h>
#include <Arduino_JSON.h>
#include <WifiLocation.h>
#include "time.h"
#include "HTTPClient.h"
#include "SD_MMC.h"
#include "WiFi.h"
#include "Wire.h"
#include "SPI.h"
#include "lvgl.h"
#include "TouchLib.h"

// Local Includes
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "./hardware_drivers/XL9535_driver.h"
#include "ui.h"
#include "pin_config.h"
#include "secrets.h"

typedef struct {
  uint8_t cmd;
  uint8_t data[16];
  uint8_t databytes; // No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

esp_lcd_panel_handle_t panel_handle = NULL;

DRAM_ATTR static const lcd_init_cmd_t st_init_cmds[] = {
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}, 0x05},
    {0xC0, {0x3b, 0x00}, 0x02},
    {0xC1, {0x0b, 0x02}, 0x02},
    {0xC2, {0x07, 0x02}, 0x02},
    {0xCC, {0x10}, 0x01},
    {0xCD, {0x08}, 0x01}, // 用565时屏蔽    666打开
    {0xb0, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x06, 0x05, 0x09, 0x08, 0x21, 0x06, 0x13, 0x10, 0x29, 0x31, 0x18}, 0x10},
    {0xb1, {0x00, 0x11, 0x16, 0x0e, 0x11, 0x07, 0x05, 0x09, 0x09, 0x21, 0x05, 0x13, 0x11, 0x2a, 0x31, 0x18}, 0x10},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x11}, 0x05},
    {0xb0, {0x6d}, 0x01},
    {0xb1, {0x37}, 0x01},
    {0xb2, {0x81}, 0x01},
    {0xb3, {0x80}, 0x01},
    {0xb5, {0x43}, 0x01},
    {0xb7, {0x85}, 0x01},
    {0xb8, {0x20}, 0x01},
    {0xc1, {0x78}, 0x01},
    {0xc2, {0x78}, 0x01},
    {0xc3, {0x8c}, 0x01},
    {0xd0, {0x88}, 0x01},
    {0xe0, {0x00, 0x00, 0x02}, 0x03},
    {0xe1, {0x03, 0xa0, 0x00, 0x00, 0x04, 0xa0, 0x00, 0x00, 0x00, 0x20, 0x20}, 0x0b},
    {0xe2, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x0d},
    {0xe3, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe4, {0x22, 0x00}, 0x02},
    {0xe5, {0x05, 0xec, 0xa0, 0xa0, 0x07, 0xee, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xe6, {0x00, 0x00, 0x11, 0x00}, 0x04},
    {0xe7, {0x22, 0x00}, 0x02},
    {0xe8, {0x06, 0xed, 0xa0, 0xa0, 0x08, 0xef, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x10},
    {0xeb, {0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00}, 0x07},
    {0xed, {0xff, 0xff, 0xff, 0xba, 0x0a, 0xbf, 0x45, 0xff, 0xff, 0x54, 0xfb, 0xa0, 0xab, 0xff, 0xff, 0xff}, 0x10},
    {0xef, {0x10, 0x0d, 0x04, 0x08, 0x3f, 0x1f}, 0x06},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x13}, 0x05},
    {0xef, {0x08}, 0x01},
    {0xFF, {0x77, 0x01, 0x00, 0x00, 0x00}, 0x05},
    {0x36, {0x08}, 0x01},
    {0x3a, {0x66}, 0x01},
    {0x11, {0x00}, 0x80},
    // {0xFF, {0x77, 0x01, 0x00, 0x00, 0x12}, 0x05},
    // {0xd1, {0x81}, 0x01},
    // {0xd2, {0x06}, 0x01},
    {0x29, {0x00}, 0x80},
    {0, {0}, 0xff}};

XL9535 xl;
TouchLib touch(Wire, IIC_SDA_PIN, IIC_SCL_PIN, CTS820_SLAVE_ADDRESS);

bool touch_pin_get_int = false;
void deep_sleep(void);
void SD_init(void);
void tft_init(void);
void lcd_cmd(const uint8_t cmd);
void lcd_data(const uint8_t *data, int len);
void wifi_task(void *param);

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
    touch_pin_get_int = false;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
  static lv_disp_drv_t disp_drv;      // contains callback functions
  static lv_indev_drv_t indev_drv;
  // put your setup code here, to run once:
  pinMode(BAT_VOLT_PIN, ANALOG);

  Wire.begin(IIC_SDA_PIN, IIC_SCL_PIN, (uint32_t)400000);
  Serial.begin(115200);
  xl.begin();
  uint8_t pin = (1 << PWR_EN_PIN) | (1 << LCD_CS_PIN) | (1 << TP_RES_PIN) | (1 << LCD_SDA_PIN) | (1 << LCD_CLK_PIN) |
                (1 << LCD_RST_PIN) | (1 << SD_CS_PIN);

  xl.pinMode8(0, pin, OUTPUT);
  xl.digitalWrite(PWR_EN_PIN, 1);
  pinMode(EXAMPLE_PIN_NUM_BK_LIGHT, OUTPUT);
  digitalWrite(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);

  SD_init();

  xl.digitalWrite(TP_RES_PIN, 0);
  delay(100);
  xl.digitalWrite(TP_RES_PIN, 1);
  //scan_iic();
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
              // EXAMPLE_PIN_NUM_DATA0,
              EXAMPLE_PIN_NUM_DATA13,
              EXAMPLE_PIN_NUM_DATA14,
              EXAMPLE_PIN_NUM_DATA15,
              EXAMPLE_PIN_NUM_DATA16,
              EXAMPLE_PIN_NUM_DATA17,

              EXAMPLE_PIN_NUM_DATA6,
              EXAMPLE_PIN_NUM_DATA7,
              EXAMPLE_PIN_NUM_DATA8,
              EXAMPLE_PIN_NUM_DATA9,
              EXAMPLE_PIN_NUM_DATA10,
              EXAMPLE_PIN_NUM_DATA11,
              // EXAMPLE_PIN_NUM_DATA12,

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
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = lv_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // Touchscreen interrupt pin
  pinMode(TP_INT_PIN, INPUT_PULLUP);
  attachInterrupt(
      TP_INT_PIN, [] { touch_pin_get_int = true; }, FALLING);


  System.is_asleep = false;
  ui_init();
  xTaskCreatePinnedToCore(wifi_task, "wifi_task", 1024 * 6, NULL, 1, NULL, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t Millis;
  delay(2);

  if (!System.is_asleep) {
    lv_timer_handler();
  }

  if (millis() - Millis > 500) {
    Millis = millis();
  }

}

void lcd_send_data(uint8_t data) {
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

void lcd_cmd(const uint8_t cmd) {
  xl.digitalWrite(LCD_CS_PIN, 0);
  xl.digitalWrite(LCD_SDA_PIN, 0);
  xl.digitalWrite(LCD_CLK_PIN, 0);
  xl.digitalWrite(LCD_CLK_PIN, 1);
  lcd_send_data(cmd);
  xl.digitalWrite(LCD_CS_PIN, 1);
}

void lcd_data(const uint8_t *data, int len) {
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

void tft_init(void) {
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

void SD_init(void) {
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

  Serial.print("SD Card Type: ");

  if (cardType == CARD_MMC)
    Serial.println("MMC");
  else if (cardType == CARD_SD)
    Serial.println("SDSC");
  else if (cardType == CARD_SDHC)
    Serial.println("SDHC");
  else
    Serial.println("UNKNOWN");

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
// This task is used to test WIFI, http test
void wifi_task(void *param) {

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = -18000;
  const int   daylightOffset_sec = 3600;

  //WifiLocation location (GOOGLE_API_KEY);
  //location_t loc = location.getGeoFromWiFi();
  /*
    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson()+"\n");
    Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    //Serial.println("Longitude: " + String(loc.lon, 7));
    Serial.println ("Accuracy: " + String (loc.accuracy));
    Serial.println ("Result: " + location.wlStatusStr (location.getStatus ()));
  */
  struct tm timeinfo;

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  String str;
  HTTPClient http_client;
  WiFi.disconnect();

  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    vTaskDelay(100);

  }
  Serial.printf("\r\n-- wifi connect success! --\r\n");

  delay(100);
  String rsp;
  bool is_get_http = false;

  static u_int32_t time_millis = 0;
  static u_int32_t weather_millis = 0;
  char time_buf[6];
  char date_buf[24];
  int * icon_data[2];

  JSONVar weather_data;

  do {

    if (millis() - time_millis > 5000 || time_millis == 0) {
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
      } else {
        sprintf(time_buf, "%d:%02d", (timeinfo.tm_hour % 12), timeinfo.tm_min);
        lv_msg_send(MSG_TIME_UPDATE, time_buf);

        strftime(date_buf, sizeof(date_buf), "%A, %b %d", &timeinfo);
        lv_msg_send(MSG_DATE_UPDATE, date_buf);
      }

      time_millis = millis();
    }


    if (millis() - weather_millis > 1200000 || weather_millis == 0) {
      http_client.begin(WEATHER_API);
      int http_code = http_client.GET();
      Serial.printf("HTTP get code: %d\n", http_code);

      if (http_code > 0) {
        if (http_code == HTTP_CODE_OK) {
          rsp = http_client.getString();
          Serial.println(rsp);
          weather_data = JSON.parse(rsp);

          String temperature = JSON.stringify(weather_data["main"]["temp"]).substring(0, 2) + "F";
          lv_msg_send(MSG_TEMPERATURE_UPDATE, temperature.c_str());

          int weather_id = std::atoi(JSON.stringify(weather_data["weather"][0]["id"]).c_str());
          int weather_day = JSON.stringify(weather_data["weather"][0]["icon"]).c_str()[2] == 'd' ? 1 : 0;
          icon_data[0] = &weather_id;
          icon_data[1] = &weather_day;
          lv_msg_send(MSG_WEATHER_ICON_UPDATE, &icon_data);
        } else {
          Serial.printf("fail to get http client,code:%d\n", http_code);
          is_get_http = true;
        }
      } else {
        Serial.println("HTTP GET failed.");
        is_get_http = true;
      }

      weather_millis = millis();
    }

    delay(100);

  } while (!is_get_http);

  http_client.end();
  WiFi.disconnect();

  str = "#00ff00 WIFI detection function completed #";

  vTaskDelete(NULL);
}

void deep_sleep(void) {

  digitalWrite(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL);

  //WiFi.disconnect();
  detachInterrupt(TP_INT_PIN);
  xl.pinMode8(0, 0xff, INPUT);
  xl.pinMode8(1, 0xff, INPUT);
  xl.read_all_reg();

  // If the SD card is initialized, it needs to be unmounted.
  /*
  if (SD_MMC.cardSize())
    SD_MMC.end();
  */
  delay(500);

  esp_sleep_enable_ext0_wakeup((gpio_num_t)TP_INT_PIN, 0);
  esp_deep_sleep_start();

  System.is_asleep = false;
}