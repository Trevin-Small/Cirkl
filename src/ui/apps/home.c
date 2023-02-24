#include "../ui.h"
#include "../ui_lib.h"
#include "apps.h"

void device_sleep_cb() {
    System.is_asleep = true;
    deep_sleep();
}

lv_obj_t * create_info_tile(lv_obj_t * parent, u_int8_t col, u_int8_t row, lv_dir_t direction) {

  lv_obj_t *ui_info_tile;
  lv_obj_t *ui_col_container;
  lv_obj_t *ui_clock_label;
  lv_obj_t *ui_clock_suffix_label;
  lv_obj_t *ui_date_label;
  lv_obj_t *ui_weather_row;
  lv_obj_t *ui_weather_img;
  lv_obj_t *ui_weather_label;
  lv_obj_t *ui_reminder_row;
  lv_obj_t *ui_reminder_bubble;
  lv_obj_t *ui_reminder_label;

  ui_info_tile = lv_tileview_add_tile(parent, col, row, direction);

  ui_col_container = new_ui_flex_container(ui_info_tile, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START,
    LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  ui_clock_label = new_ui_label(ui_col_container, &sf_bold_84, "1:10", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  ui_date_label = new_ui_label(ui_col_container, &sf_bold_22, "Thursday, Feb 23", LV_ALIGN_CENTER,
    LV_SIZE_CONTENT, 0, 0);

  ui_weather_row = new_ui_flex_container(ui_col_container, LV_ALIGN_CENTER, LV_FLEX_FLOW_ROW, LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  ui_weather_img = new_ui_img(ui_weather_row, &ui_img_weather_rain_png, false, LV_ALIGN_CENTER, WEATHER_ICON_SIZE,
    WEATHER_ICON_SIZE, 0, 0);

  ui_weather_label = new_ui_label(ui_weather_row, &sf_bold_30, "56F", LV_ALIGN_OUT_BOTTOM_MID, LV_SIZE_CONTENT,
    0, 0);

  ui_reminder_row = new_ui_flex_container(ui_col_container, LV_ALIGN_CENTER, LV_FLEX_FLOW_ROW, LV_FLEX_ALIGN_CENTER,
    LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  ui_reminder_bubble = lv_obj_create(ui_reminder_row);
  lv_obj_set_style_bg_color(ui_reminder_bubble, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_reminder_bubble, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
  lv_obj_set_size(ui_reminder_bubble, 14, 14);
  lv_obj_set_style_radius(ui_reminder_bubble, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
  lv_obj_align(ui_reminder_bubble, LV_ALIGN_CENTER, 0, 0);

  ui_reminder_label = new_ui_label(ui_reminder_row, &sf_bold_14, "2 New Reminders", LV_ALIGN_OUT_BOTTOM_MID, LV_SIZE_CONTENT,
    0, 0);

  lv_obj_set_style_text_color(ui_clock_label, System.font_accent_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_date_label, System.font_main_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_weather_label, System.font_main_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_reminder_label, System.font_main_color, LV_STATE_DEFAULT);

  return ui_info_tile;
}

lv_obj_t * create_app_list_tile(lv_obj_t * parent, u_int8_t col, u_int8_t row, lv_dir_t direction) {

  static lv_coord_t col_dsc[] = {APP_ICON_SIZE, APP_ICON_SIZE, LV_GRID_TEMPLATE_LAST};
  static lv_coord_t row_dsc[] = {APP_GRID_SIZE, APP_GRID_SIZE, LV_GRID_TEMPLATE_LAST};

  lv_obj_t *ui_app_tile;
  lv_obj_t *ui_col_container;
  lv_obj_t *ui_app_label;
  lv_obj_t *ui_grid;
  lv_obj_t *ui_gallery_btn;
  lv_obj_t *ui_gallery_label;
  lv_obj_t *ui_calendar_btn;
  lv_obj_t *ui_calendar_label;
  lv_obj_t *ui_settings_btn;
  lv_obj_t *ui_settings_label;
  lv_obj_t *ui_shutdown_btn;
  lv_obj_t *ui_shutdown_label;

  ui_app_tile = lv_tileview_add_tile(parent, col, row, direction);

  ui_col_container = new_ui_flex_container(ui_app_tile, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START,
    LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, -20);

  ui_app_label = new_ui_label(ui_col_container, &sf_bold_22, "Apps", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  ui_grid = lv_obj_create(ui_col_container);
  lv_obj_set_style_grid_column_dsc_array(ui_grid, col_dsc, LV_STATE_DEFAULT);
  lv_obj_set_style_grid_row_dsc_array(ui_grid, row_dsc, LV_STATE_DEFAULT);
  lv_obj_set_size(ui_grid, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_layout(ui_grid, LV_LAYOUT_GRID);
  lv_obj_align(ui_grid, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_opa( ui_grid, 0, LV_STATE_DEFAULT );
  lv_obj_set_style_border_opa( ui_grid, 0, LV_STATE_DEFAULT );


  ui_gallery_btn = new_ui_img_btn(ui_grid, &ui_img_gallery_png, LV_ALIGN_CENTER, APP_ICON_SIZE, APP_ICON_SIZE, 0, 0);
  ui_gallery_label = new_ui_label(ui_gallery_btn, &sf_bold_14, "Gallery", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);
  lv_obj_align_to(ui_gallery_label, ui_gallery_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_add_event_cb(ui_gallery_btn, open_app_event_cb, LV_EVENT_CLICKED, &gallery_init);


  ui_calendar_btn = new_ui_img_btn(ui_grid, &ui_img_calendar_png, LV_ALIGN_CENTER, APP_ICON_SIZE, APP_ICON_SIZE, 0, 0);
  ui_calendar_label = new_ui_label(ui_calendar_btn, &sf_bold_14, "Calendar", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);
  lv_obj_align_to(ui_calendar_label, ui_calendar_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_add_event_cb(ui_calendar_btn, open_app_event_cb, LV_EVENT_CLICKED, &calendar_init);


  ui_settings_btn = new_ui_img_btn(ui_grid, &ui_img_settings_png, LV_ALIGN_CENTER, APP_ICON_SIZE, APP_ICON_SIZE, 0, 0);
  ui_settings_label = new_ui_label(ui_settings_btn, &sf_bold_14, "Settings", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);
  lv_obj_align_to(ui_settings_label, ui_settings_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_add_event_cb(ui_settings_btn, open_app_event_cb, LV_EVENT_CLICKED, &settings_init);


  ui_shutdown_btn = new_ui_img_btn(ui_grid, &ui_img_shutdown_png, LV_ALIGN_CENTER, APP_ICON_SIZE, APP_ICON_SIZE, 0, 0);
  ui_shutdown_label = new_ui_label(ui_shutdown_btn, &sf_bold_14, "Shutdown", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);
  lv_obj_align_to(ui_shutdown_label, ui_shutdown_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_add_event_cb(ui_shutdown_btn, device_sleep_cb, LV_EVENT_CLICKED, NULL);


  lv_obj_set_grid_cell(ui_gallery_btn, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_grid_cell(ui_calendar_btn, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_grid_cell(ui_settings_btn, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
  lv_obj_set_grid_cell(ui_shutdown_btn, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

  lv_obj_set_style_text_color(ui_app_label, System.font_accent_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_gallery_label, System.font_main_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_calendar_label, System.font_main_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_settings_label, System.font_main_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_shutdown_label, System.font_main_color, LV_STATE_DEFAULT);

  return ui_app_tile;
}

lv_obj_t * create_app_tile(lv_obj_t * parent, u_int8_t col, u_int8_t row, lv_dir_t direction) {

  lv_obj_t *ui_app_tile;
  ui_app_tile = lv_tileview_add_tile(parent, col, row, direction);

  return ui_app_tile;
}

lv_obj_t * home_screen(void) {
  lv_obj_t* ui_screen_home = lv_obj_create(NULL);
  lv_obj_clear_flag( ui_screen_home, LV_OBJ_FLAG_SCROLLABLE );

  System.main_tile_view = lv_tileview_create(ui_screen_home);
  lv_obj_set_style_bg_color(System.main_tile_view, System.theme_main_color, LV_STATE_DEFAULT);
  lv_obj_set_size(System.main_tile_view, SCREEN_SIZE, SCREEN_SIZE);

  int can_swipe_right = System.app_is_open == true ? LV_DIR_RIGHT : LV_DIR_LEFT;

  System.info_tile = create_info_tile(System.main_tile_view, 0, 0, LV_DIR_RIGHT);
  System.app_list_tile = create_app_list_tile(System.main_tile_view, 1, 0, LV_DIR_LEFT | can_swipe_right);
  System.app_screen_tile = create_app_tile(System.main_tile_view, 2, 0, LV_DIR_NONE);

  app_tile_init();

  return ui_screen_home;
}