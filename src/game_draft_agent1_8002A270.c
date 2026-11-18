#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* GTMAIN.EXE 8002A270 */
/* Original: GTMAIN.EXE:sub_8002A270. */ void gt_draft_8002A270(void)
{
    uint32 ordering = r_u32(0x800B6368u);
    (void)gtmain_race_countdown_block_draw(ordering, 68, -1);
    gtmain_race_current_time_draw(ordering, 16, 16);
    (void)gtmain_race_lap_times_draw(ordering, 16, 38);
    (void)gtmain_race_record_label_draw();
    (void)gtmain_race_instruments_draw(ordering, 238, 150, 0x800B6850u);
    gtmain_race_status_draw(ordering, 68, 0x800B6850u);
}
