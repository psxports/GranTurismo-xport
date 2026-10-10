#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8002B5D8 */
uint32 gtmain_race_frame_finish(void)
{
    if ((sint8)r_u8(0x800B633Cu)) gtmain_ordering_submit(1u);
    /* TODO Mode4 caption callee retains its unrecorded 8006F798 dependency */
    if (r_u8(0x800CB7F5u) == 4u)
        return gt_draft_8002A124(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    return 4u;
}
