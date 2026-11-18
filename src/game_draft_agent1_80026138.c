#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gt_draft_80026138(uint32 alternate)
{
    uint32 base, records, total, position = 0, flags, first_flag, second_flag;
    sint16 inserted, laps;
    FUNCTION_MARKER(0x80026138, "GTMAIN.EXE");
    base = alternate ? 0x800CB51C : 0x800CAF7C;
    base += r_u8(0x800B63BD) * 180;
    records = base + 20;
    total = r_u32(0x800CB8E0);
    while (position < 8) {
        uint32 previous = r_u32(records + position * 20 + 4);
        if (!previous || total < previous)
            break;
        ++position;
    }
    if (position < 7) {
        uint32 row = 7;
        do {
            uint32 destination = records + row * 20;
            uint32 a = r_u32(destination - 20);
            uint32 b = r_u32(destination - 16);
            uint32 c = r_u32(destination - 12);
            uint32 d = r_u32(destination - 8);
            w_u32(destination, a);
            w_u32(destination + 4, b);
            w_u32(destination + 8, c);
            w_u32(destination + 12, d);
            w_u32(destination + 16, r_u32(destination - 4));
            --row;
        } while (position < row);
    }
    if (position < 8) {
        uint32 entry = records + position * 20;
        w_u32(entry + 4, total);
        w_u16(entry + 8, r_u16(0x800B6670));
        inserted = position;
    } else {
        inserted = -1;
    }
    first_flag = r_s8(0x800BDD01);
    second_flag = r_s8(0x800BDD00);
    flags = first_flag ? 2 : 0;
    if (second_flag) {
        w_u32(base + 4, r_u32(0x800BDB48));
        flags |= 1;
        w_u16(base + 8, r_u16(0x800B6670));
    }
    gtmain_race_record_update(total);
    laps = r_s16(0x800B634C);
    if (alternate)
        gtmain_menu_result_mode6_run(flags, inserted, base, records, 0x800BDB70, laps, total);
    else
        gtmain_menu_result_mode1_run(flags, inserted, base, records, 0x800BDB70, laps, total);
    return 0;
}
