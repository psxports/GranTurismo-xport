#include "game_first2_registry.h"

/* GTMAIN.EXE:80022C4C */
/* Original: GTMAIN.EXE:sub_80022C4C. */ uint32 gt_draft_80022C4C(uint32 alternate)
{
    uint32 object;
    FUNCTION_MARKER(0x80022C4C, "GTMAIN.EXE");
    object = gtmain_event_result_record();
    uint32 second = object + (alternate ? 56u : 28u);
    uint8 flags = 0u;
    sint16 threshold, index;
    uint32 extra;
    if ((sint8)r_u8(0x800BDD01u)) {
        w_u32(second + 12u, r_u32(0x800BDB4Cu));
        flags = 2u;
        w_u16(second + 16u, r_u16(0x800B6670u));
    }
    if ((sint8)r_u8(0x800BDD00u)) {
        w_u32(object + 12u, r_u32(0x800BDB48u));
        flags |= 1u;
        w_u16(object + 16u, r_u16(0x800B6670u));
    }
    threshold = (sint16)r_u16(0x800B634Cu);
    extra = r_u32(0x800CB8E0u);
    index = (sint8)r_u8(0x800B63D2u);
    return gtmain_menu_result_mode5_run(flags, index, 0u, alternate, object,
        0x800BDB70u, threshold, extra, 0u);
}
