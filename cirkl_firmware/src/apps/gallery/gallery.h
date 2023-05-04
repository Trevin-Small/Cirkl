#include "lvgl.h"

#define NUM_IMAGES       3

typedef struct gallery {

  int curr_img_num;
  int num_images;

  unsigned long photo_swap_interval;

  lv_obj_t * img;
  lv_timer_t * photo_timer;

} gallery_t;

extern gallery_t * Gallery;