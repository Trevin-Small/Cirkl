// Local Includes
#include "system.h"
#include "location.h"
#include "ui.h"
#include "TRGB.h"
#include "./wifi/wifi_utils.h"

system_t System;
TRGB trgb;
loc_t location;
u_int8_t current_brightness = BRIGHTNESS_DEFAULT;

void wifi_task(void * param);

void setup() {
  System.sleep_delay = SYS_SLEEP_DELAY;
  System.brightness = BRIGHTNESS_DEFAULT;
  System.photo_interval = SYS_PHOTO_INTERVAL;
  System.is_asleep = false;
  System.wifi_active = true;

  trgb.init();

  xTaskCreatePinnedToCore(wifi_task, "wifi_task", 1024 * 6, NULL, 1, NULL, 0);
}

void loop() {

  delay(2);

  if (!System.is_asleep) {
    lv_timer_handler();

    if (millis() - System.last_check_millis > SYS_CHECK_INTERVAL) {

      System.last_check_millis = millis();

      if (System.brightness != current_brightness) {
        analogWrite(EXAMPLE_PIN_NUM_BK_LIGHT, System.brightness);
        current_brightness = System.brightness;
      }

      if (millis() - System.last_interact_time >= System.sleep_delay) {
        trgb.sleep();
      }

    }

  }

}