#include "../ui.h"
#include "../ui_lib.h"
#include "apps.h"

#define X_BTN_WIDTH                    70
#define X_BTN_HEIGHT                  130
#define X_BTN_X                       350
#define X_BTN_Y_HIDE                 -100
#define X_BTN_Y_SHOW                    0
#define X_BTN_ANIM_DURATION           300
#define X_BTN_ANIM_DELAY             1000
#define X_BTN_ANIM_START_DELAY        300

void anim_x_btn(lv_event_t * e) {

  lv_obj_t * btn = lv_event_get_user_data(e);

  lv_anim_t show_btn;
  lv_anim_init(&show_btn);
  lv_anim_set_var(&show_btn, btn);
  lv_anim_set_values(&show_btn, X_BTN_Y_HIDE, X_BTN_Y_SHOW);
  lv_anim_set_exec_cb(&show_btn, (lv_anim_exec_xcb_t) lv_obj_set_y);
  lv_anim_set_path_cb(&show_btn, lv_anim_path_ease_in_out);
  lv_anim_set_early_apply(&show_btn, true);
  lv_anim_set_time(&show_btn, X_BTN_ANIM_DURATION);
  lv_anim_set_playback_delay(&show_btn, X_BTN_ANIM_DELAY + X_BTN_ANIM_DURATION);
  lv_anim_set_playback_time(&show_btn, X_BTN_ANIM_DURATION);

  lv_anim_start(&show_btn);

}

void app_tile_init() {
  lv_obj_t * x_btn_bg = new_ui_flex_container(System.app_screen_tile, LV_ALIGN_DEFAULT, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, X_BTN_WIDTH, X_BTN_HEIGHT, X_BTN_X, X_BTN_Y_HIDE);

  lv_obj_set_style_bg_color(x_btn_bg, lv_color_hex(0x535353), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(x_btn_bg, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(x_btn_bg, 15, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(x_btn_bg, 15, LV_STATE_DEFAULT);

  lv_obj_t * x_btn = new_ui_flex_container(x_btn_bg, LV_ALIGN_DEFAULT, LV_FLEX_FLOW_ROW, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, 40, 40, 0, 0);

  lv_obj_set_style_bg_color(x_btn, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(x_btn, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(x_btn, 10, LV_STATE_DEFAULT);

  lv_obj_t * x_btn_label = new_ui_label(x_btn, &sf_bold_18, "#ffffff x#", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  // Add a callback to close the app when the "x" button is clicked
  lv_obj_add_event_cb(x_btn, close_app_event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_add_event_cb(System.app_screen_tile, anim_x_btn, LV_EVENT_CLICKED, x_btn_bg);
}

void open_app_event_cb(lv_event_t * e) {
  // Get the app_init function pointer passed to open_app
  lv_obj_t * (*app_init_func)(lv_obj_t *) = lv_event_get_user_data(e);

  // Call the initialization function of the app
  System.app_content = app_init_func(System.app_screen_tile);

  // Slide to the app screen tile
  lv_obj_set_tile(System.main_tile_view, System.app_screen_tile, LV_ANIM_ON);
  System.app_is_open = true;
}

void close_app_event_cb(lv_event_t * e) {
    lv_obj_set_tile(System.main_tile_view, System.app_list_tile, LV_ANIM_ON);
    System.app_is_open = false;
    lv_obj_clean(System.app_content);
}