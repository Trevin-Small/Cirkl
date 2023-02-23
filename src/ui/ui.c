#include "ui.h"
#include "ui_helpers.h"
#include "./apps/apps.h"

///////////////////// VARIABLES ////////////////////
lv_color_t system_theme;
lv_color_t system_font_main;
lv_color_t system_font_accent;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

void ui_init( void ) {

    system_theme = lv_color_hex(0xF8F0E3);
    system_font_main = lv_color_hex(0x000000);
    system_font_accent = lv_color_hex(0x4F8FC0);

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    lv_obj_t * ui_screen_home = home_init();
    lv_disp_load_scr( ui_screen_home );

}
