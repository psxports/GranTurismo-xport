#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gt_draft_80025EAC(void)
{
    sint32 timer, state;
    uint32 step, result;
    FUNCTION_MARKER(0x80025EAC, "GTMAIN.EXE");
    timer = r_s16(0x800B63DA);
    state = r_s16(0x800B63D6);
    step = r_u32(0x800BF518);
    if (state == 8) {
        ++timer;
        result = 9;
        if (timer >= 56)
            w_u16(0x800B63D6, 9);
    } else if (state < 9) {
        result = state < 6;
        if (state >= 6) {
            --timer;
            result = 1;
            if (!timer) {
                if (r_u32(0x800B630C) == 1) {
                    result = 3;
                    w_u8(0x800B63C0, result);
                } else {
                    sint32 stream = r_u16(0x800B6874) ? 16 : 18;
                    gtmain_race_stream_start(stream, 1);
                    w_u16(0x800B63D6, 8);
                    w_u8(0x800B63C8, 0);
                    result = gtmain_race_recorders_stop();
                }
            }
        }
    } else {
        result = step << 4;
        if (state == 9) {
            sint32 threshold = (sint32)(result - step + 56);
            ++timer;
            result = 2;
            if (timer >= threshold || gt_draft_80030B20() >= 0)
                w_u8(0x800B63C0, result);
        }
    }
    w_u16(0x800B63DA, timer);
    return result;
}
