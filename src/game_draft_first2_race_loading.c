#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

static void gtmain_loading_packets_bind(uint32 *first, uint32 *second, uint32 count)
{
    gtmain_frame_tables_bind(first, second, count);
}

static void gtmain_loading_primitives_bind(uint8 *first, uint8 *second, uint32 bytes)
{
    /* TODO Bind the two native primitive buffers */
    (void)first; (void)second; (void)bytes;
    gt_draft_failure("GTMAIN native loading primitives", 0x8006F0A0u);
}

static sint32 gtmain_loading_context_run(const uint32 *context)
{
    return gtmain_frame_run((uint8 *)context);
}

/* GTMAIN.EXE 80028968 */
sint32 gtmain_race_loading_tick(void)
{
    uint32 value = r_u32(0x8008F880u) - 1u;
    w_u32(0x8008F880u, value);
    return (sint32)value;
}

/* GTMAIN.EXE 80028984 */
sint32 gtmain_race_loading_done(void)
{
    return r_u32(0x8008F880u) == 0u;
}

/* GTMAIN.EXE 80028A60 */
sint32 gtmain_race_pad_axes_reset(uint32 state)
{
    w_u8(state + 82u, 0u);
    w_u8(state + 83u, 128u);
    w_u8(state + 84u, 0u);
    w_u8(state + 85u, 0u);
    return -128;
}

/* GTMAIN.EXE 80028A78 */
sint32 gtmain_race_pad_reset(uint32 state)
{
    w_u8(state + 2u, 0u);
    gtmain_input_reset(state + 4u);
    w_u16(state + 78u, 0u);
    w_u16(state + 80u, 0u);
    return gtmain_race_pad_axes_reset(state);
}

/* GTMAIN.EXE 800288F4 */
void gtmain_race_loading_draw(void)
{
    uint32 width, ordering_link;
    sint32 x;
    width = (uint32)gtmain_text_width(3, 0x800BD944u);
    gtmain_text_font(3);
    gtmain_text_color(0x808080u);
    ordering_link = gtmain_frame_ordering(0u);
    x = (sint32)(320u - width) >> 1;
    gtmain_font_text_draw(ordering_link, (sint16)x, 108, 0x800BD944u);
}

/* GTMAIN.EXE 80028994 */
sint32 gtmain_race_loading_screen(void)
{
    uint32 ordering_first[32], ordering_second[32], context[10];
    uint8 primitive_first[8192], primitive_second[8192];
    gtmain_loading_packets_bind(ordering_first, ordering_second, 32u);
    gtmain_loading_primitives_bind(primitive_first, primitive_second, 8192u);
    /* TODO Resolve the remaining selected game frame-service bindings */
    gtmain_display_mode_init(1u);
    memset(context, 0, sizeof(context));
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
    return gtmain_loading_context_run(context);
}

