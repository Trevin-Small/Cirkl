#include "ui.h"

lv_obj_t * new_ui_screen(lv_color_t bg_color, lv_obj_flag_t f);

lv_obj_t * new_ui_container(lv_obj_t * parent, lv_align_t align, lv_coord_t width, lv_coord_t height, lv_coord_t x,
  lv_coord_t y);

lv_obj_t * new_ui_flex_container(lv_obj_t * parent, lv_align_t align, lv_flex_flow_t flex, lv_flex_align_t justify_content,
  lv_flex_align_t align_items, lv_flex_align_t align_content, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y);

lv_obj_t * new_ui_img_btn(lv_obj_t * parent, const lv_img_dsc_t * img, lv_align_t align, lv_coord_t width, lv_coord_t
  height, lv_coord_t x, lv_coord_t y);

lv_obj_t * new_ui_img(lv_obj_t * parent, const lv_img_dsc_t * img, bool scrollable, lv_align_t align, lv_coord_t width,
  lv_coord_t height, lv_coord_t x, lv_coord_t y);

lv_obj_t * new_ui_label(lv_obj_t * parent, const lv_font_t * font, const char * text, lv_align_t align, lv_coord_t width, lv_coord_t x, lv_coord_t y);

lv_obj_t * new_ui_container_label(lv_obj_t * parent, const lv_font_t * font, const char * text, lv_align_t align, lv_coord_t width,
  lv_coord_t height, lv_coord_t x, lv_coord_t y);