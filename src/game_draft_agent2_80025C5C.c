#include "game_first2_registry.h"
#include "xport_trace.h"

/* GTMAIN.EXE:80025C5C */
/* Original: GTMAIN.EXE:sub_80025C5C. */ uint32 gtmain_race_results_commit(void)
{
    uint32 competitor, record, second_record;
    uint32 before_two = 0u, after_two = 0u;
    uint8 changed = 0u, flags = 0u;
    FUNCTION_MARKER(0x80025C5C, "GTMAIN.EXE");
    competitor = r_u8(0x800B63BDu);
    record = 0x800CAE3Cu + competitor * 40u;
    second_record = record + 20u;
    if (!(sint8)r_u8(0x800B63D2u)) {
        uint32 before_competitor, before_four, after_competitor, after_four;
        uint32 current, column, shift, flag_address;
        uint8 prior;
        before_competitor = gtmain_event_competitor_complete(competitor);
        before_two = gtmain_event_all_medals_two();
        before_four = gtmain_event_all_medals_four();
        current = r_u8(0x800B63BDu);
        column = r_u8(0x800B631Cu);
        shift = r_u8(0x800CB7F9u);
        flag_address = 0x800CAE3Cu + current * 4u + 2300u + column;
        prior = r_u8(flag_address);
        w_u8(flag_address, (uint8)(prior | (1u << (shift & 31u))));
        current = r_u8(0x800B63BDu);
        after_competitor = gtmain_event_competitor_complete(current);
        after_two = gtmain_event_all_medals_two();
        after_four = gtmain_event_all_medals_four();
        if (before_competitor != after_competitor || before_two != after_two ||
            before_four != after_four) changed = 1u;
    }
    if ((sint8)r_u8(0x800BDD01u)) {
        w_u32(second_record + 4u, r_u32(0x800BDB4Cu));
        flags = 2u;
        w_u16(second_record + 8u, r_u16(0x800B6670u));
    }
    if ((sint8)r_u8(0x800BDD00u)) {
        w_u32(record + 4u, r_u32(0x800BDB48u));
        flags |= 1u;
        w_u16(record + 8u, r_u16(0x800B6670u));
    }
    {
        sint16 index = (sint8)r_u8(0x800B63D2u);
        sint16 threshold = (sint16)r_u16(0x800B634Cu);
        uint32 extra = r_u32(0x800CB8E0u);
        gtmain_menu_result_mode0_run(flags, index, record, second_record,
            0x800BDB70u, threshold, extra, changed);
    }
    if (after_two && before_two != after_two) {
        w_u32(0x800B6390u, 0x8008BBF4u);
        return 1u;
    }
    return 0u;
}
