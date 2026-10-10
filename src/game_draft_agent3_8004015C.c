#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 8004015C */
uint32 gtmain_menu_record_labels_prepare(void)
{
    sint32 loading, kind, replay;
    uint32 label = 0u, caption = 5u, text;
    w_u8(0x800B5300u, 0u);
    (void)gtmain_menu_record_header_prepare();
    loading = (sint8)r_u8(0x801F1FE5u);
    text = loading ? 0x8008D2F8u : 0x8008D274u;
    /* Reviewed literal messages contain no format conversions */
    fputs((const char *)psx_addr(text, 1u), stdout);
    kind = (sint8)r_u8(0x801F1FE4u);
    if (kind == 1) {
        if (!loading) {
            fputs((const char *)psx_addr(0x8008D2D4u, 1u), stdout);
            replay = (sint8)r_u8(0x801F2034u);
            if (replay == 0) w_u32(0x80092274u, 0x800BC30Au);
            else if (replay == 1) w_u32(0x80092274u, 0x800BC31Eu);
            else if (replay == 2) w_u32(0x80092274u, 0x800BC330u);
        } else {
            replay = (sint8)r_u8(0x801F2034u);
            fputs((const char *)psx_addr(0x8008D2D4u, 1u), stdout);
            w_u32(0x80092274u, replay == 4 ? 0x800BC359u : 0x800BC349u);
        }
        w_u8(0x80092271u, 2u);
        caption = 15u;
    } else if (kind == 2) {
        fputs((const char *)psx_addr(0x8008D2C0u, 1u), stdout);
        label = loading ? 0x800BE140u : 0x800BE128u;
        w_u32(0x80092274u, label);
        w_u8(0x80092271u, 0u);
    } else {
        fputs((const char *)psx_addr(0x8008D2E8u, 1u), stdout);
        label = loading ? 0x800BD45Cu : 0x800BD449u;
        w_u32(0x80092274u, label);
        w_u8(0x80092271u, 2u);
    }
    w_u8(0x80092288u, caption);
    (void)gtmain_menu_mode_dispatch(0);
    return 0u;
}
