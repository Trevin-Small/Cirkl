// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project
#pragma once

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#define MSG_BAT_VOLT_UPDATE 1
#define MSG_TOUCH_UPDATE    2
#define MSG_WIFI_UPDATE     3
#define SCREEN_SIZE       480
#define WEATHER_ICON_SIZE  64
#define APP_ICON_SIZE      96

#include "stdint.h"

typedef struct {
  uint16_t x;
  uint16_t y;
} touch_point_t;

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

extern lv_color_t system_theme;
extern lv_color_t system_font_main;
extern lv_color_t system_font_accent;
extern bool is_asleep;

LV_IMG_DECLARE( ui_img_mail_png);   // assets/mail.png
LV_IMG_DECLARE( ui_img_gallery_png);   // assets/gallery.png
LV_IMG_DECLARE( ui_img_settings_png);   // assets/settings.png
LV_IMG_DECLARE( ui_img_calendar_png);   // assets/calendar.png
LV_IMG_DECLARE( ui_img_shutdown_png);   // assets/shutdown.png
LV_IMG_DECLARE( ui_img_weather_sun_png);   // assets/shutdown.png
LV_IMG_DECLARE( ui_img_weather_cloud_png);   // assets/shutdown.png
LV_IMG_DECLARE( ui_img_weather_rain_png);   // assets/shutdown.png
LV_IMG_DECLARE( ui_img_weather_snow_png);   // assets/shutdown.png
LV_IMG_DECLARE( ui_img_weather_wind_png);   // assets/shutdown.png

void deep_sleep(void);
void device_sleep();
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif