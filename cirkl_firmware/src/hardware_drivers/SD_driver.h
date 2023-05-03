#include "lvgl.h"

void * SD_open_file(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
lv_fs_res_t SD_close_file(lv_fs_drv_t * drv, void * fp);
lv_fs_res_t SD_read_file(_lv_fs_drv_t *drv, void * fp, void * buf,
                                  uint32_t btr, uint32_t * br);
lv_fs_res_t SD_write_file(lv_fs_drv_t * drv, void * fp, const void * buf,
                                  uint32_t btw, uint32_t * bw);
lv_fs_res_t SD_seek_file(lv_fs_drv_t *drv, void *fp, uint32_t pos, lv_fs_whence_t whence);
lv_fs_res_t SD_tell_file(_lv_fs_drv_t *drv, void *fp, uint32_t *pos_p);

/*
lv_fs_res_t SD_seek_file();
lv_fs_res_t SD_dir_open(_lv_fs_drv_t * drv, const char * path);
lv_fs_res_t SD_dir_read(_lv_fs_drv_t * drv, void * rddir_p, char * fn);
lv_fs_res_t SD_dir_close(_lv_fs_drv_t * drv, void * rddir_p);
*/