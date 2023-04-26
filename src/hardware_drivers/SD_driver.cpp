#include "lvgl.h"
#include "SD_driver.h"
#include "system.h"
#include "SD_MMC.h"
#include "../pin_config.h"
#include "../hardware_drivers/XL9535_driver.h"

void * SD_open_file(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode) {

  File file;

  Serial.print("Opening ");
  Serial.println(path);

  if (mode == LV_FS_MODE_RD) {
    return SD_MMC.open(path, "r");
  } else if (mode == LV_FS_MODE_WR) {
    return SD_MMC.open(path, "w");
  } else if (mode == LV_FS_MODE_RD | LV_FS_MODE_WR) {
    return SD_MMC.open(path, "rw");
  }

  Serial.println("Bad open mode specified.");
  return NULL;

}

lv_fs_res_t SD_close_file(lv_fs_drv_t * drv, void * fp) {
  lv_fs_res_t res = LV_FS_RES_OK;

  Serial.println("Closing file.");

  ((File *) (fp))->close();

  delete ((File *) fp);

  return res;
}

lv_fs_res_t SD_read_file(_lv_fs_drv_t *drv, void * fp, void * buf, uint32_t btr, uint32_t * br) {

  lv_fs_res_t res = LV_FS_RES_OK;

  File file = *((File *) (fp));

  *br = file.read((uint8_t *) buf, btr);

  return res;

}

lv_fs_res_t SD_write_file(lv_fs_drv_t * drv, void * fp, const void * buf, uint32_t btw, uint32_t * bw) {
  lv_fs_res_t res = LV_FS_RES_OK;

  File file = *((File *) (fp));

  *bw = file.write((uint8_t *) buf, btw);

  return res;
}

lv_fs_res_t SD_seek_file(lv_fs_drv_t *drv, void *fp, uint32_t pos, lv_fs_whence_t whence) {
  lv_fs_res_t res = LV_FS_RES_OK;

  File file = *((File *) (fp));

  if (whence == LV_FS_SEEK_SET) {
    file.seek(pos, SeekSet);
  } else if (whence == LV_FS_SEEK_CUR) {
    file.seek(pos, SeekCur);
  } else {
    file.seek(pos, SeekEnd);
  }

  return res;
}

lv_fs_res_t SD_tell_file(_lv_fs_drv_t *drv, void *fp, uint32_t *pos_p) {
  lv_fs_res_t res = LV_FS_RES_OK;

  File file = *((File *) (fp));

  *pos_p = file.position();

  return res;
}

/*
lv_fs_res_t SD_dir_open(_lv_fs_drv_t * drv, const char * path) {

}

lv_fs_res_t SD_dir_read(_lv_fs_drv_t * drv, void * rddir_p, char * fn) {

}

lv_fs_res_t SD_dir_close(_lv_fs_drv_t * drv, void * rddir_p) {

}
*/