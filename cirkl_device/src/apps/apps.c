#include "system.h"
#include "ui.h"
#include "ui_lib.h"
#include "apps.h"

#define X_BTN_BG_WIDTH                 80
#define X_BTN_BG_HEIGHT               160
#define X_BTN_SIZE                     50
#define X_BTN_X                       350
#define X_BTN_Y_HIDE                 -130
#define X_BTN_Y_SHOW                   10
#define X_BTN_ANIM_DURATION           250
#define X_BTN_ANIM_DELAY             1750
#define X_BTN_ANIM_START_DELAY        250

struct x_out_btn {
  lv_obj_t * bg;
  lv_obj_t * btn;
  lv_anim_t anim;
} x_out_btn;

void anim_x_btn(lv_event_t * e) {

  if (lv_obj_get_y(x_out_btn.bg) != X_BTN_Y_HIDE) {
    return;
  }

  lv_anim_init(&x_out_btn.anim);
  lv_anim_set_var(&x_out_btn.anim, x_out_btn.bg);
  lv_anim_set_values(&x_out_btn.anim, X_BTN_Y_HIDE, X_BTN_Y_SHOW);
  lv_anim_set_exec_cb(&x_out_btn.anim, (lv_anim_exec_xcb_t) lv_obj_set_y);
  lv_anim_set_path_cb(&x_out_btn.anim, lv_anim_path_ease_in_out);
  lv_anim_set_early_apply(&x_out_btn.anim, true);
  lv_anim_set_time(&x_out_btn.anim, X_BTN_ANIM_DURATION);
  lv_anim_set_playback_delay(&x_out_btn.anim, X_BTN_ANIM_DELAY + X_BTN_ANIM_DURATION);
  lv_anim_set_playback_time(&x_out_btn.anim, X_BTN_ANIM_DURATION);

  lv_anim_start(&x_out_btn.anim);

}

void app_tile_init() {
  x_out_btn.bg = new_ui_flex_container(lv_layer_top(), LV_ALIGN_DEFAULT, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, X_BTN_BG_WIDTH, X_BTN_BG_HEIGHT, X_BTN_X, X_BTN_Y_HIDE);

  lv_obj_set_style_bg_color(x_out_btn.bg, lv_color_hex(0x535353), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(x_out_btn.bg, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(x_out_btn.bg, 15, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(x_out_btn.bg, 12, LV_STATE_DEFAULT);

  x_out_btn.btn = new_ui_flex_container(x_out_btn.bg, LV_ALIGN_DEFAULT, LV_FLEX_FLOW_ROW, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, X_BTN_SIZE, X_BTN_SIZE, 0, 0);

  lv_obj_set_style_bg_color(x_out_btn.btn, lv_color_hex(0xff0000), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(x_out_btn.btn, LV_OPA_COVER, LV_STATE_DEFAULT);
  lv_obj_set_style_radius(x_out_btn.btn, 10, LV_STATE_DEFAULT);

  lv_obj_t * x_btn_label = new_ui_label(x_out_btn.btn, &sf_bold_22, "#ffffff x#", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  // Add a callback to close the app when the "x" button is clicked
  lv_obj_add_event_cb(x_out_btn.btn, close_app_event_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_add_event_cb(System.app_screen_tile, anim_x_btn, LV_EVENT_CLICKED, NULL);
}

void open_app_event_cb(lv_event_t * e) {
  // Get the app_init function pointer passed to open_app
  lv_obj_t * (*app_init_func)(lv_obj_t *) = lv_event_get_user_data(e);

  // Call the initialization function of the app
  System.app_content = app_init_func(System.app_screen_tile);

  lv_obj_set_parent(x_out_btn.bg, lv_layer_top());

  // Slide to the app screen tile
  lv_obj_set_tile(System.main_tile_view, System.app_screen_tile, LV_ANIM_ON);

  System.app_is_open = true;
}

void open_gallery_event_cb(lv_event_t * e) {
  // Get the app_init function pointer passed to open_app
  lv_obj_t * (*app_init_func)(lv_obj_t *) = lv_event_get_user_data(e);

  // Slide to the app screen tile
  lv_obj_set_tile(System.main_tile_view, System.app_screen_tile, LV_ANIM_OFF);

  // Call the initialization function of the app
  System.app_content = app_init_func(System.app_screen_tile);

  lv_obj_set_parent(x_out_btn.bg, lv_layer_top());

  System.app_is_open = true;
}

void close_app_event_cb(lv_event_t * e) {

    if (System.photo_timer != NULL) {
      lv_timer_del(System.photo_timer);
    }

    lv_obj_set_parent(x_out_btn.bg, System.app_screen_tile);
    lv_obj_set_tile(System.main_tile_view, System.app_list_tile, LV_ANIM_ON);
    System.app_is_open = false;
    lv_obj_clean(System.app_content);
    lv_anim_del(x_out_btn.bg, (lv_anim_exec_xcb_t) lv_obj_set_y);
    lv_obj_set_y(x_out_btn.bg, X_BTN_Y_HIDE);
}