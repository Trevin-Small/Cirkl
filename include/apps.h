#pragma once

#include "ui.h"

//------------------------ SYSTEM FUNCTIONS ---------------------------//
lv_obj_t * home_screen();
void app_tile_init();
void open_app_event_cb(lv_event_t * e);
void close_app_event_cb(lv_event_t * e);


//---------------------- APP SPECIFIC FUNCTIONS -----------------------//
lv_obj_t * gallery_init(lv_obj_t * parent);
lv_obj_t * calendar_init(lv_obj_t * parent);
lv_obj_t * settings_init(lv_obj_t * parent);