#include "game_first2_registry.h"
#include <string.h>

/* GTMAIN.EXE 8003A540 */
uint32 gtmain_menu_frame_context_initialize(uint8 *context)
{
    /* TODO Translate unselected original menu setup after an observed native blocker */
    gt_draft_failure("GTMAIN menu initial setup", 0x8003A438u);
    gtmain_display_mode_init(1u);
    w_u8(0x800B1A8Cu, 30u);
    w_u8(0x800B1A8Du, 10u);
    gtmain_frame_enabled(1u);
    gtmain_draw_background(1u, 0u, 0u, 0u);
    gtmain_race_frame_reset(5u);
    memset(context, 0, 40u);
    xport_store_le32(context + 16u, 0x80035810u);
    xport_store_le32(context + 8u, 0x80035AF0u);
    xport_store_le32(context + 12u, 0x80035978u);
    xport_store_le32(context + 32u, 0x80035D90u);
    return 0x80035D90u;
}

/* GTMAIN.EXE 8003A5E8 */
uint32 gtmain_menu_input_context_initialize(uint32 context)
{
    uint32 result;
    w_u32(context + 168u, 0u);
    w_u32(context + 172u, 4097u);
    gtmain_ordering_node_insert(context + 160u);
    gtmain_input_reset(context + 2u);
    result = gtmain_input_reset(context + 76u);
    w_u8(context + 257u, 0u);
    w_u8(context + 259u, 0u);
    w_u32(0x800BDD08u, context);
    return result;
}

/* GTMAIN.EXE 8003A720 */
sint32 gtmain_menu_frame_run(void)
{
    uint8 context[40];
    w_u8(0x800B52FAu, 0u);
    gtmain_menu_frame_context_initialize(context);
    gtmain_menu_input_context_initialize(0x800BDD10u);
    /* TODO Complete setup and callback dependencies of the actual native frame context */
    return gtmain_frame_run(context) == 2 ? -1 : 0;
}

