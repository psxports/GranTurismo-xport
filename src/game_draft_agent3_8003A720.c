#include "game_first2_registry.h"
#include "game_local_stack.h"
#include <string.h>
#include <stdio.h>

/* GTMAIN.EXE 8003A438 */
/* Original: GTMAIN.EXE:sub_8003A438. */ static sint32 menu_frame_buffers_initialize(void)
{
    gtmain_frame_tables_bind((uint32 *)psx_addr(0x801A8FDCu, 0x5000u),
        (uint32 *)psx_addr(0x801ADFDCu, 0x5000u), 0x1400u);
    gtmain_packet_buffers(0x801B2FDCu, 0x801C2FDCu, 0x10000u);
    printf("otag 1 : %x\n", 0x801A8FDCu);
    printf("otag 2 : %x\n", 0x801ADFDCu);
    printf("otag size : %x\n", 0x1400u);
    printf("packet 1 : %x\n", 0x801B2FDCu);
    printf("packet 2 : %x\n", 0x801C2FDCu);
    printf("packet size : %x\n", 0x10000u);
    printf("save data area : %x (size=%x)\n", 0x801D2FDCu, 0x1F000u);
    return printf("kanji cache area : %x (size=%x)\n", 0x801F3FDCu, 0x801F480Cu);
}

/* GTMAIN.EXE 8003A540 */
/* Original: GTMAIN.EXE:sub_8003A540. */ uint32 gtmain_menu_frame_context_initialize(uint8 *context)
{
    menu_frame_buffers_initialize();
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
/* Original: GTMAIN.EXE:sub_8003A5E8. */ uint32 gtmain_menu_input_context_initialize(uint32 context)
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
/* Original: GTMAIN.EXE:sub_8003A720. */ sint32 gtmain_menu_frame_run(void)
{
    /* The completed race frame has released this stack-region slot */
    uint8 *context = (uint8 *)psx_addr(gt_local_stack_workspace(0x4200u, 40u, 0x8003A720u), 40u);
    w_u8(0x800B52FAu, 0u);
    gtmain_menu_frame_context_initialize(context);
    gtmain_menu_input_context_initialize(0x800BDD10u);
    /* The frame loop consumes the initialized native context */
    return gtmain_frame_run(context) == 2 ? -1 : 0;
}

