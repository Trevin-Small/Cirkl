#pragma once

#define SLEEP_AFTER_TIME         120000
#define SYS_UPDATE_INTERVAL         100
#define GALLERY_UPDATE_INTERVAL   10000
#define WEATHER_UPDATE_INTERVAL 1200000

#define BRIGHTNESS_MAX              254
#define BRIGHTNESS_MIN               50
#define BRIGHTNESS_DEFAULT          200
#define BRIGHTNESS_OFF                0

#include "stdint.h"

typedef struct {
  uint16_t x;
  uint16_t y;
} touch_point_t;

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct system {
    int32_t brightness;
    unsigned long last_interact_time;
    unsigned long last_check_millis;
    unsigned long sleep_after_time;
    unsigned long gallery_update_interval;
    bool is_asleep;
    bool wifi_active;
    bool app_is_open;

    lv_disp_t * display;
    lv_obj_t * active_screen;
    lv_obj_t * main_tile_view;
    lv_obj_t * info_tile;
    lv_obj_t * app_list_tile;
    lv_obj_t * app_screen_tile;
    lv_obj_t * app_content;
    lv_coord_t app_tile_coord_x;
    lv_coord_t app_tile_coord_y;
    lv_timer_t * photo_timer;

    bool color_palette;
    lv_color_t theme_main_color;
    lv_color_t theme_accent_color;
    lv_color_t font_main_color;
    lv_color_t font_accent_color;
} system_t;

extern system_t System;

void shutdown();

#ifdef __cplusplus
} /*extern "C"*/
#endif