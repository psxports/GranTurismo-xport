#include "game_first2_registry.h"
#include "xport_trace.h"

/* GTMAIN.EXE 800461A0 */
/* Original: GTMAIN.EXE:sub_800461A0. */ uint32 gtmain_menu_selection_snapshot_initialize(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x800461A0, "GTMAIN.EXE");
    mode = r_u8(0x800B5374);
    if (mode == 27 || mode == 35) {
        sint32 count = r_s8(0x80093530), i;
        w_u16(0x801D2FDE, (uint16)count);
        for (i = 0; i < count; ++i) {
            uint32 rows;
            w_u8(0x801D2FE0 + (uint32)i, 0);
            rows = r_u32(0x80093534);
            if (!(r_u8(rows + 8 * (uint32)i) & 0x40))
                w_u8(0x801D2FE0 + (uint32)i, 1);
        }
        w_u16(0x801D2FDC, 0);
        gtmain_menu_selection_peak_reset(0x801D2FE8);
        gtmain_race_specification_summary(0x800BB850, 0x801D3644, 0x801D35D8);
        gtmain_menu_selection_peak_update(0x801D2FE8, 0x801D3644, 0x801D35D8);
    } else {
        w_u16(0x801D2FDC, 0xFFFF);
    }
    gtmain_menu_selector_reset(0x80093530);
    return 0;
}
