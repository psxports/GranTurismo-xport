#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

static void gtmain_loading_packets_bind(uint32 *first, uint32 *second, uint32 count)
{
    gtmain_frame_tables_bind(first, second, count);
}

static void gtmain_loading_primitives_bind(uint32 first, uint32 second, uint32 bytes)
{
    gtmain_packet_buffers(first, second, bytes);
}

static uint32 loading_stack_locals(void)
{
    uint32 launch = r_u32(0x8008F1D8u), alternate = r_u32(0x800B1AA4u);
    uint32 top = alternate ? alternate : launch;
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 heap = r_u32(0x800B1AACu), bytes = r_u32(0x800B1AA8u);
    uint32 extent = bytes < 16u ? 16u : bytes, base;
    /* Live callers are 10000, 19EC0, 1A068, 2881C and 27330 */
    const uint32 callers = 0x20u + 0x18u + 0x18u + 0x18u + 0x18u;
    if (top < 0x80010000u + callers + 0x4140u || top > 0x80200000u || (top & 7u))
        gt_draft_failure("GTMAIN loading stack range", 0x80028994u);
    base = top - callers - 0x4140u;
    if (base < 0x801FB724u || heap < 0x80010000u || heap > base || extent > base - heap ||
        (!alternate && (reserve < callers + 0x4140u || base < launch - reserve)))
        gt_draft_failure("GTMAIN loading stack overlap", 0x80028994u);
    return base;
}

static sint32 gtmain_loading_context_run(const uint32 *context)
{
    return gtmain_frame_run((uint8 *)context);
}

/* GTMAIN.EXE 80028968 */
/* Original: GTMAIN.EXE:sub_80028968. */ sint32 gtmain_race_loading_tick(void)
{
    uint32 value = r_u32(0x8008F880u) - 1u;
    w_u32(0x8008F880u, value);
    return (sint32)value;
}

/* GTMAIN.EXE 80028984 */
/* Original: GTMAIN.EXE:sub_80028984. */ sint32 gtmain_race_loading_done(void)
{
    return r_u32(0x8008F880u) == 0u;
}

/* GTMAIN.EXE 80028A60 */
/* Original: GTMAIN.EXE:sub_80028A60. */ sint32 gtmain_race_pad_axes_reset(uint32 state)
{
    w_u8(state + 82u, 0u);
    w_u8(state + 83u, 128u);
    w_u8(state + 84u, 0u);
    w_u8(state + 85u, 0u);
    return -128;
}

/* GTMAIN.EXE 80028A78 */
/* Original: GTMAIN.EXE:sub_80028A78. */ sint32 gtmain_race_pad_reset(uint32 state)
{
    w_u8(state + 2u, 0u);
    gtmain_input_reset(state + 4u);
    w_u16(state + 78u, 0u);
    w_u16(state + 80u, 0u);
    return gtmain_race_pad_axes_reset(state);
}

/* GTMAIN.EXE 800288F4 */
/* Original: GTMAIN.EXE:sub_800288F4. */ sint32 gtmain_race_loading_draw(void)
{
    uint32 width, ordering_link;
    sint32 x;
    width = (uint32)gtmain_text_width(3, 0x800BD944u);
    gtmain_text_font(3);
    gtmain_text_color(0x808080u);
    ordering_link = gtmain_frame_ordering(0u);
    x = (sint32)(320u - width) >> 1;
    return gtmain_font_text_draw(ordering_link, (sint16)x, 108, 0x800BD944u);
}

/* GTMAIN.EXE 80028994 */
/* Original: GTMAIN.EXE:sub_80028994. */ sint32 gtmain_race_loading_screen(void)
{
    uint32 workspace = loading_stack_locals();
    uint32 *context;
    sint32 result;
    context = (uint32 *)psx_addr(workspace + 0x10u, 40u);
    gtmain_loading_packets_bind((uint32 *)psx_addr(workspace + 0x38u, 128u),
        (uint32 *)psx_addr(workspace + 0xB8u, 128u), 32u);
    gtmain_loading_primitives_bind(workspace + 0x138u, workspace + 0x2138u, 8192u);
    gtmain_display_mode_init(1u);
    memset(context, 0, 40u);
    context[4] = 0x800288F4u;
    context[2] = 0x80028968u;
    context[3] = 0x80028984u;
    gtmain_frame_enabled(1u);
    gtmain_draw_background(1u, 0u, 0u, 0u);
    VSync(0);
    gtmain_display_mask(0u);
    VSync(0);
    gtmain_race_frame_reset(5u);
    w_u32(0x8008F880u, 4u);
    result = gtmain_loading_context_run(context);
    return result;
}

