#include "system.h"
#include "ui.h"
#include "ui_lib.h"
#include "apps.h"

lv_obj_t * calendar_init(lv_obj_t * parent) {
  lv_obj_t * calendar_content = new_ui_flex_container(parent, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);
  lv_obj_set_scroll_dir(calendar_content, LV_DIR_VER);

  lv_obj_t * calendar_label = new_ui_label(calendar_content, &sf_bold_22, "Calendar", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  lv_obj_set_style_text_color(calendar_label, System.font_accent_color, LV_STATE_DEFAULT);

  return calendar_content;
}