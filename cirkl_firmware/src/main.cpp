#include "system.h"
#include "location.h"
#include "ui.h"
#include "TRGB.h"
#include "./system_utils/wifi_utils.h"
#include "./system_utils/SD_utils.h"

system_t System;
TRGB trgb;
loc_t location;
u_int8_t current_brightness = BRIGHTNESS_DEFAULT;

void setup() {
  System.sleep_after_time = SLEEP_AFTER_TIME;
  System.brightness = BRIGHTNESS_DEFAULT;
  System.gallery_update_interval = GALLERY_UPDATE_INTERVAL;
  System.is_asleep = false;
  System.wifi_active = true;
  System.color_palette = false;
  location.is_known = false;

  // Initialize SD card hardware
  trgb.SD_init();

  // Get coordinates from SD card
  read_coordniates();

  // Get UI color palette from SD card
  read_color_palette();

  // Initialize display drivers for LVGL
  trgb.display_init();

  // Pin wifi funcitonality to core 0 (core 1 is default core)
  xTaskCreatePinnedToCore(wifi_task, "wifi_task", 1024 * 6, NULL, 1, NULL, 0);
}

void loop() {

  delay(2);

  if (!System.is_asleep) {

    // Allow LVGL to update the display
    lv_timer_handler();

    // Check if system properties should be updated
    if (millis() - System.last_check_millis > SYS_UPDATE_INTERVAL) {

      System.last_check_millis = millis();

      if (System.brightness != current_brightness) {
        analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, System.brightness);
        current_brightness = System.brightness;
      }

      // Put device to sleep if period of inactivity
      if (millis() - System.last_interact_time >= System.sleep_after_time) {
        trgb.sleep();
      }

    }

  }

}