#include "system.h"
#include "ui.h"
#include "ui_lib.h"
#include "apps.h"
#include "gallery.h"

int curr_img_num = 0;
int num_images = NUM_IMAGES;

unsigned long photo_swap_interval = SWAP_INTERVAL;

lv_obj_t * img;
lv_timer_t * photo_timer;

void set_picture_num(int num) {
  char img_format_str[9];
  sprintf(img_format_str, "S:/%d.png", num);
  lv_img_set_src(img, img_format_str);
}

void next_picture(lv_timer_t * timer) {

  ++curr_img_num;

  if (curr_img_num >= num_images) {
    curr_img_num = 0;
  }

  set_picture_num(curr_img_num);

}

lv_obj_t * gallery_init(lv_obj_t * parent) {

  lv_obj_t * gallery_content = new_ui_flex_container(parent, LV_ALIGN_CENTER, LV_FLEX_FLOW_COLUMN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_SIZE_CONTENT, LV_SIZE_CONTENT, 0, 0);
  lv_obj_add_flag(gallery_content, LV_OBJ_FLAG_EVENT_BUBBLE);
  lv_obj_clear_flag(gallery_content, LV_OBJ_FLAG_SCROLLABLE);

  curr_img_num = 0;
  num_images = NUM_IMAGES;
  photo_swap_interval = SWAP_INTERVAL;

  img = lv_img_create(gallery_content);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_flag(img, LV_OBJ_FLAG_EVENT_BUBBLE);
  set_picture_num(curr_img_num);

  photo_timer = lv_timer_create(next_picture, photo_swap_interval, NULL);

  return gallery_content;
}