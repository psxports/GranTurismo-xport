#include "game_first2_registry.h"

static uint32 results_competitor_status(sint32 competitor)
{
    /* TODO Bind unselected competitor status service */
    gt_draft_failure("GTMAIN result competitor status", 0x80035138u);
}

static uint32 results_status_two(void)
{
    /* TODO Bind unselected result status service */
    gt_draft_failure("GTMAIN result status two", 0x800351D4u);
}

static uint32 results_status_four(void)
{
    /* TODO Bind unselected result status service */
    gt_draft_failure("GTMAIN result status four", 0x80035228u);
}

/* GTMAIN.EXE:80025C5C */
uint32 gtmain_race_results_commit(void)
{
    uint32 competitor = r_u8(0x800B63BDu);
    uint32 record = 0x800CAE3Cu + competitor * 40u;
    uint32 second_record = record + 20u;
    uint32 before_two = 0u, after_two = 0u;
    uint8 changed = 0u, flags = 0u;
    if (!(sint8)r_u8(0x800B63D2u)) {
        uint32 before_competitor, before_four, after_competitor, after_four;
        uint32 current, column, shift, flag_address;
        uint8 prior;
        before_competitor = results_competitor_status((sint32)competitor);
        before_two = results_status_two();
        before_four = results_status_four();
        current = r_u8(0x800B63BDu);
        column = r_u8(0x800B631Cu);
        shift = r_u8(0x800CB7F9u);
        flag_address = 0x800CAE3Cu + current * 4u + 2300u + column;
        prior = r_u8(flag_address);
        w_u8(flag_address, (uint8)(prior | (1u << (shift & 31u))));
        current = r_u8(0x800B63BDu);
        after_competitor = results_competitor_status((sint32)current);
        after_two = results_status_two();
        after_four = results_status_four();
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
