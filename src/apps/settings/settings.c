#include "ui.h"
#include "ui_lib.h"
#include "apps.h"

lv_obj_t * brightness_label;

static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    System.brightness = lv_slider_get_value(slider);
    char buf[17];
    lv_snprintf(buf, sizeof(buf), "Brightness: %d%%", (int) ( ( ( (double)lv_slider_get_value(slider) - BRIGHTNESS_MIN ) / (BRIGHTNESS_MAX - BRIGHTNESS_MIN) ) * 100) );
    lv_label_set_text(brightness_label, buf);
}

lv_obj_t * settings_init(lv_obj_t * parent) {
  lv_obj_t * settings_content = new_ui_flex_container(parent, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);

  lv_obj_t * settings_label = new_ui_label(settings_content, &sf_bold_30, "Settings", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  brightness_label = new_ui_label(settings_content, &sf_bold_22, "", LV_ALIGN_CENTER, LV_SIZE_CONTENT, 0, 0);

  lv_obj_t * brightness_slider = lv_slider_create(settings_content);

  lv_obj_set_width(brightness_slider, 250);
  lv_obj_set_style_pad_all(brightness_slider, 18, LV_STATE_DEFAULT);
  lv_obj_set_style_height(brightness_slider, 25, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(brightness_slider, System.font_accent_color, LV_PART_KNOB);
  lv_obj_align(brightness_slider, LV_ALIGN_CENTER, 0, 0);
  lv_slider_set_range(brightness_slider, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
  lv_slider_set_value(brightness_slider, System.brightness, false);
  lv_obj_add_event_cb(brightness_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  lv_obj_set_style_text_color(settings_label, System.font_accent_color, LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(brightness_label, System.font_main_color, LV_STATE_DEFAULT);

  lv_event_send(brightness_slider, LV_EVENT_VALUE_CHANGED, NULL);

  return settings_content;
}