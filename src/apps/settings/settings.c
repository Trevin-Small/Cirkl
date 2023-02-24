#include "ui.h"
#include "ui_lib.h"
#include "apps.h"

lv_obj_t * settings_init(lv_obj_t * parent) {
  lv_obj_t * settings_content = new_ui_flex_container(parent, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  lv_obj_t * settings_label = new_ui_label(settings_content, &sf_bold_22, "Settings", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  lv_obj_set_style_text_color(settings_label, System.font_accent_color, LV_STATE_DEFAULT);

  System.font_accent_color = lv_color_hex(0x00ff00);

  return settings_content;
}