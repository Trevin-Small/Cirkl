#include "system.h"
#include "ui.h"
#include "apps.h"

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match the display requirements"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match the display requirements"
#endif

//////////////////////////// MAIN ///////////////////////////

void ui_init( void ) {
    /*
    System.theme_main_color = lv_color_hex(colors[0]);
    System.font_main_color = lv_color_hex(colors[1]);
    System.font_accent_color = lv_color_hex(colors[2]);
    System.theme_accent_color = lv_color_hex(colors[3]);
    */

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
        true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    System.active_screen = home_screen();
    lv_disp_load_scr( System.active_screen );
}
