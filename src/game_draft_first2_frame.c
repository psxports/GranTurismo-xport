#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 80027288 */
uint32 gtmain_race_frame_reset(uint32 font)
{
    return gtmain_font_load((sint32)font);
}

/* GTMAIN.EXE 8006F0C0 */
void gtmain_frame_enabled(uint32 enabled)
{
    w_u32(0x800BFF74u, enabled);
}

/* GTMAIN.EXE 8006EFB4 */
void gtmain_display_mask(uint32 enabled)
{
    w_u32(0x800BFF70u, enabled);
    SetDispMask(enabled != 0u);
}

/* GTMAIN.EXE 8006ED9C */
sint32 gtmain_display_mode_init(uint32 flags)
{
    uint32 mode = flags & 7u, table = 0x800AD898u + 20u * mode;
    uint32 value, index;
    w_u32(0x800BFE70u, mode);
    value = r_u16(table);
    w_u16(0x800BFE78u, 0u);
    w_u16(0x800BFE7Au, 0u);
    w_u32(0x800BFE74u, value);
    w_u16(0x800BFE7Cu, r_u16(table + 2u));
    w_u16(0x800BFE7Eu, r_u16(table + 4u));
    w_u16(0x800BFE80u, r_u16(table + 6u));
    w_u16(0x800BFE82u, r_u16(table + 8u));
    w_u16(0x800BFE84u, r_u16(table + 2u));
    w_u16(0x800BFE86u, r_u16(table + 4u));
    if (flags & 32u) SetVideoMode(1);
    for (index = 0u; index < 2u; ++index) {
        uint32 dimensions = 0x800BFE78u + 8u * index;
        uint32 display = 0x800BFE88u + 20u * index;
        uint32 draw = 0x800BFEB0u + 92u * index;
        sint32 x = (sint16)r_u16(dimensions);
        sint32 y = (sint16)r_u16(dimensions + 2u);
        sint32 width = (sint16)r_u16(dimensions + 4u);
        sint32 height = (sint16)r_u16(dimensions + 6u);
        SetDefDispEnv((DISPENV *)psx_addr(display, sizeof(DISPENV)), x, y, width, height);
        x = (sint16)r_u16(dimensions);
        y = (sint16)r_u16(dimensions + 2u);
        width = (sint16)r_u16(dimensions + 4u);
        height = (sint16)r_u16(dimensions + 6u);
        SetDefDrawEnv((DRAWENV *)psx_addr(draw, sizeof(DRAWENV)), x, y, width, height);
        if (flags & 16u) w_u8(display + 17u, 1u);
        if (flags & 8u) w_u8(display + 16u, 1u);
        if (flags & 32u) w_u16(display + 10u, mode == 7u || mode == 37u ? 16u : 24u);
    }
    if (!(flags & 64u)) {
        w_u32(0x800BFF68u, 0u);
        w_u32(0x800BFF6Cu, 1u);
    }
    w_u32(0x800BFF74u, 1u);
    return 1;
}
