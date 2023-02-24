#include "ui.h"

lv_obj_t * new_ui_screen(lv_color_t bg_color, lv_obj_flag_t f) {
  lv_obj_t * screen = lv_obj_create(NULL);
  lv_obj_clear_flag( screen, f );
  lv_obj_set_style_bg_color( screen, bg_color, LV_STATE_DEFAULT );
}

lv_obj_t * new_ui_container(lv_obj_t * parent, lv_align_t align, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
  lv_obj_t * cont = lv_obj_create(parent);
  lv_obj_align( cont, align, x, y );
  lv_obj_set_width( cont, width);
  lv_obj_set_height( cont, height);
  lv_obj_set_style_pad_all( cont, 2, LV_STATE_DEFAULT );
  lv_obj_set_style_bg_opa( cont, 0, LV_STATE_DEFAULT );
  lv_obj_set_style_border_opa( cont, 0, LV_STATE_DEFAULT );
  return cont;
}

lv_obj_t * new_ui_flex_container(lv_obj_t * parent, lv_align_t align, lv_flex_flow_t flex, lv_flex_align_t justify_content, lv_flex_align_t align_items, lv_flex_align_t align_content, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
  lv_obj_t * cont = lv_obj_create(parent);
  lv_obj_align( cont, align, x, y );
  lv_obj_set_flex_flow( cont, flex );
  lv_obj_set_width( cont, width);
  lv_obj_set_height( cont, height);
  lv_obj_set_flex_align( cont, justify_content, align_items, align_content );
  lv_obj_set_style_pad_all( cont, 2, LV_STATE_DEFAULT );
  lv_obj_set_style_bg_opa( cont, 0, LV_STATE_DEFAULT );
  lv_obj_set_style_border_opa( cont, 0, LV_STATE_DEFAULT );
  return cont;
}

lv_obj_t * new_ui_img_btn(lv_obj_t * parent, const lv_img_dsc_t * img, lv_align_t align, int width, int height, lv_coord_t x, lv_coord_t y) {

  lv_obj_t * img_btn = lv_imgbtn_create(parent);
  lv_imgbtn_set_src( img_btn, LV_IMGBTN_STATE_RELEASED, NULL, img, NULL );
  lv_obj_align( img_btn, align, x, y );
  lv_obj_set_width( img_btn, width);
  lv_obj_set_height( img_btn, height);
  lv_obj_set_align( img_btn, LV_ALIGN_CENTER );
  return img_btn;
}

lv_obj_t * new_ui_img(lv_obj_t * parent, const lv_img_dsc_t * img, bool scrollable, lv_align_t align, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
  lv_obj_t * img_obj = lv_img_create(parent);
  lv_img_set_src( img_obj, img );
  lv_obj_align( img_obj, align, x, y );
  lv_obj_set_width( img_obj, width );
  lv_obj_set_height( img_obj, height );
  lv_obj_set_align( img_obj, LV_ALIGN_CENTER );
  lv_obj_add_flag( img_obj, LV_OBJ_FLAG_ADV_HITTEST );
  if (!scrollable) {
    lv_obj_clear_flag( img_obj, LV_OBJ_FLAG_SCROLLABLE );
  }
  return img_obj;
}

lv_obj_t * new_ui_label(lv_obj_t * parent, const lv_font_t * font, const char * text, lv_align_t align, lv_coord_t width, lv_coord_t x, lv_coord_t y) {
    lv_obj_t * label = lv_label_create(parent);
    //lv_obj_align( label, align, x, y);
    lv_obj_set_pos( label, x, y );
    lv_obj_set_width( label, width );
    lv_obj_set_style_text_font( label, font, LV_STATE_DEFAULT );
    lv_label_set_text( label, text );
    lv_label_set_recolor( label, true );
    lv_obj_set_style_pad_all( label, 0, LV_STATE_DEFAULT );
    return label;
}

lv_obj_t * new_ui_container_label(lv_obj_t * parent, const lv_font_t * font, const char * text, lv_align_t align, 
  lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
    lv_obj_t * container = new_ui_container(parent, align, width, height, x, y);
    lv_obj_t * label = new_ui_label(container, font, text, LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);
    return label;
}