#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 80033ABC */
/* Original: GTMAIN.EXE:sub_80033ABC. */ uint32 gtmain_menu_result_panel_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra, uint8 mode, uint8 style, uint8 panel)
{
    uint32 cursor = 0x801D3AE4u;
    printf((const char *)psx_addr(0x8008CAF4u, 1u));
    w_u8(0x801D2FEDu, flags);
    w_u16(0x801D2FEEu, (uint16)index);
    w_u32(0x801D2FF0u, first);
    w_u32(0x801D2FF4u, second);
    w_u8(0x801D2FEAu, 0u);
    w_u32(0x801D2FFCu, extra);
    w_u32(0x801D2FF8u, metric);
    w_u16(0x801D3000u, (uint16)threshold);
    w_u8(0x801D2FEBu, panel);
    for (sint32 count = 99; count >= 0; --count) {
        w_u8(cursor, 8u);
        cursor -= 8u;
    }
    w_u32(0x801D3AF0u, 0x801D37CCu);
    w_u8(0x801D3AF5u, 13u);
    w_u8(0x801D3AF4u, 0u);
    w_u8(0x801D3AF6u, 2u);
    gtmain_menu_panel_special_bind(0x800BDD10u, 0x80092154u);
    w_u16(0x801D2FE8u, 3584u);
    w_u16(0x801D2FE2u, 0u);
    w_u16(0x801D2FDEu, 0xFFFFu);
    w_u16(0x801D2FDCu, 0xFFFFu);
    w_u16(0x801D2FE0u, 0xFFFFu);
    w_u16(0x801D2FE4u, 0u);
    w_u8(0x801D2FECu, mode);
    w_u8(0x800919E8u, style);
    w_u8(0x80091994u, style);
    gtmain_music_sector_init();
    gtmain_stream_stop_wait(1u);
    gtmain_menu_frame_run();
    gtmain_stream_stop_wait(1u);
    return 0u;
}

/* GTMAIN.EXE 80033E44 */
/* Original: GTMAIN.EXE:sub_80033E44. */ uint32 gtmain_menu_result_mode7_run(uint8 flags, uint32 first, uint32 second, uint32 metric,
    sint16 threshold, uint32 extra)
{
    gtmain_menu_draw_callback_set(0x8009197Cu);
    return gtmain_menu_result_panel_run(flags, 0, first, second, metric, threshold, extra, 7u, 4u, 0u);
}

/* GTMAIN.EXE 80033EE0 */
/* Original: GTMAIN.EXE:sub_80033EE0. */ uint32 gtmain_menu_result_mode8_run(uint8 flags, uint32 first, uint32 second, uint32 metric,
    sint16 threshold, uint32 extra)
{
    gtmain_menu_draw_callback_set(0x8009197Cu);
    return gtmain_menu_result_panel_run(flags, 0, first, second, metric, threshold, extra, 8u, 24u, 0u);
}

/* GTMAIN.EXE 800340D4 */
/* Original: GTMAIN.EXE:sub_800340D4. */ uint32 gtmain_menu_result_mode4_run(uint32 extra, sint16 index, uint32 first)
{
    gtmain_menu_draw_callback_set(0x800919B4u);
    return gtmain_menu_result_panel_run(0u, index, first, 0u, 0u, 1, extra, 4u, 22u, 0u);
}

/* GTMAIN.EXE 80034150 */
/* Original: GTMAIN.EXE:sub_80034150. */ uint32 gtmain_menu_result_mode4_alternate_run(uint32 extra, sint16 index, uint32 first)
{
    gtmain_menu_draw_callback_set(0x800919B4u);
    return gtmain_menu_result_panel_run(0u, index, first, 0u, 0u, 1, extra, 4u, 22u, 0u);
}
