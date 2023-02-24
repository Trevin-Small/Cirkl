#include "../ui.h"
#include "../ui_lib.h"
#include "apps.h"

void open_app_event_cb(lv_event_t * e) {
  // Get the app_init function pointer passed to open_app
  lv_obj_t * (*app_init_func)(lv_obj_t *) = lv_event_get_user_data(e);

  // Call the initialization function of the app
  lv_obj_t * content = app_init_func(System.app_screen_tile);

  lv_obj_t * close_app_bg = new_ui_flex_container(System.app_screen_tile, LV_ALIGN_DEFAULT, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 70, 110, 355, 20);

  lv_obj_set_style_bg_color(close_app_bg, lv_color_hex(0x535353), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(close_app_bg, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(close_app_bg, 15, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(close_app_bg, 15, LV_STATE_DEFAULT);

  lv_obj_t * close_app_btn = new_ui_flex_container(close_app_bg, LV_ALIGN_DEFAULT, LV_FLEX_FLOW_ROW, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 40, 40, 0, 0);

  lv_obj_set_style_bg_color(close_app_btn, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(close_app_btn, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(close_app_btn, 10, LV_STATE_DEFAULT);

  lv_obj_t * btn_label = new_ui_label(close_app_btn, &sf_bold_18, "#ffffff x#", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  // Add a callback to close the app when the "x" button is clicked
  lv_obj_add_event_cb(close_app_btn, close_app_event_cb, LV_EVENT_CLICKED, NULL);

  // Slide to the app screen tile
  lv_obj_set_tile(System.main_tile_view, System.app_screen_tile, LV_ANIM_ON);
  System.app_is_open = true;
}

void close_app_event_cb(lv_event_t * e) {
    lv_obj_clean(System.app_screen_tile);
    System.app_is_open = false;
    lv_obj_set_tile(System.main_tile_view, System.app_list_tile, LV_ANIM_ON);
}