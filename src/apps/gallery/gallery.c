#include "system.h"
#include "ui.h"
#include "ui_lib.h"
#include "apps.h"

const char * img_filename = "img.png";

lv_obj_t * gallery_init(lv_obj_t * parent) {

  lv_obj_t * gallery_content = new_ui_flex_container(parent, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  lv_obj_t * img = lv_img_create(gallery_content);
  lv_img_set_src(img, "S:/img.png");
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * gallery_label = new_ui_label(gallery_content, &sf_bold_22, "Gallery", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  lv_obj_set_style_text_color(gallery_label, System.font_accent_color, LV_STATE_DEFAULT);

  return gallery_content;
}