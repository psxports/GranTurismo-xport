#include "game_first2_registry.h"
#include "psx.h"

/* Original: GTMAIN.EXE:sub_8002BA60. */ uint8 *gtmain_race_frame_draw(void)
{
    sint32 mode = r_s8(0x800B633Cu);
    uint8 submitted = 0, *context;
    if (mode == 0) {
        w_u32(0x800B6638u, gtmain_driver_leader());
        gtmain_race_view_draw(0);
        gtmain_race_overlay_draw(0);
    } else if (mode == 1) {
        w_u32(0x800B6638u, 0x800B6524u);
        gtmain_race_view_draw(1); gtmain_race_overlay_draw(1);
        DrawSync(0);
        gtmain_ordering_submit(0);
        gtmain_ordering_clear(1); gtmain_packet_buffer_select(1);
        w_u32(0x800B6638u, 0x800B6410u);
        gtmain_race_view_draw(2);
        submitted = 1;
    } else if (mode == 2) {
        DrawSync(0);
        gtmain_ordering_clear(1); gtmain_packet_buffer_select(1);
        submitted = 1;
        w_u32(0x800B6638u, gtmain_driver_leader());
        gtmain_race_view_draw(0); gtmain_race_overlay_draw(0);
    }
    context = gtmain_race_context_get();
    context[38] = submitted;
    return context;
}
