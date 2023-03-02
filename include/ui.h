#pragma once

#define MSG_TIME_UPDATE          1
#define MSG_DATE_UPDATE          2
#define MSG_TEMPERATURE_UPDATE   3
#define MSG_WEATHER_ICON_UPDATE  4

#define SCREEN_SIZE       480
#define WEATHER_ICON_SIZE  64
#define APP_ICON_SIZE      96
#define APP_GRID_SIZE     128

#include "stdint.h"

typedef struct {
  uint16_t x;
  uint16_t y;
} touch_point_t;

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

LV_IMG_DECLARE( ui_img_mail_png);
LV_IMG_DECLARE( ui_img_gallery_png);
LV_IMG_DECLARE( ui_img_settings_png);
LV_IMG_DECLARE( ui_img_calendar_png);
LV_IMG_DECLARE( ui_img_shutdown_png);
LV_IMG_DECLARE( ui_img_weather_sun_png);
LV_IMG_DECLARE( ui_img_weather_moon_png);
LV_IMG_DECLARE( ui_img_weather_cloud_d_png);
LV_IMG_DECLARE( ui_img_weather_cloud_n_png);
LV_IMG_DECLARE( ui_img_weather_rain_png);
LV_IMG_DECLARE( ui_img_weather_snow_png);
LV_IMG_DECLARE( ui_img_weather_wind_png);
LV_IMG_DECLARE( ui_img_weather_fog_png);

void deep_sleep(void);
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif