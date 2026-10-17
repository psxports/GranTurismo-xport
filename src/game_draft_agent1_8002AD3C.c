#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8002AD3C */
void gt_draft_8002AD3C(void)
{
    sint32 entry, parameter;
    gtmain_stream_mode_set(0);
    entry = (sint8)r_u8(0x800B63C1u);
    parameter = (sint8)r_u8(0x800B6314u);
    gtmain_race_stream_start(entry, parameter);
    gtmain_race_sound_callbacks_active();
}
