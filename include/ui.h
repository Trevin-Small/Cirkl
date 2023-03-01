// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project
#pragma once

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

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

typedef struct system {

    lv_color_t theme_main_color;
    lv_color_t theme_accent_color;
    lv_color_t font_main_color;
    lv_color_t font_accent_color;
    lv_obj_t * active_screen;
    lv_obj_t * main_tile_view;
    lv_obj_t * info_tile;
    lv_obj_t * app_list_tile;
    lv_obj_t * app_screen_tile;
    lv_obj_t * app_content;
    lv_coord_t app_tile_coord_x;
    lv_coord_t app_tile_coord_y;
    bool app_is_open;
    bool is_asleep;

} system_t;

extern system_t System;

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

#endif