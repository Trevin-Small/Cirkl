#include "lvgl.h"

void * SD_open_file(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
void * SD_read_file(_lv_fs_drv_t *drv, void * fp, void * buf, uint32_t bytes_to_read, uint32_t * bytes_read);
void * SD_write_file(lv_fs_drv_t * drv, void * fp, const void * buf, uint32_t bytes_to_write, uint32_t * bytes_written);
void * SD_close_file(lv_fs_drv_t * drv, void * fp);
void * SD_seek_file();

void * SD_dir_open(_lv_fs_drv_t * drv, const char * path);
void * SD_dir_read(_lv_fs_drv_t * drv, void * rddir_p, char * fn);
void * SD_dir_close(_lv_fs_drv_t * drv, void * rddir_p);