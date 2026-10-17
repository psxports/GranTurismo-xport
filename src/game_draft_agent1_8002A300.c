#include "game_draft_registry.h"

/* GTMAIN.EXE 8002A300 */
void gt_draft_8002A300(void)
{
    sint32 mode = r_s16(0x800B63D6u);
    if (mode == 2) (void)gt_draft_8002A0D4();
    else if (mode == 4) (void)gt_draft_8002A15C();
    else if (mode == 1) (void)gt_draft_8002A084();
    else gt_draft_8002A270();
}
