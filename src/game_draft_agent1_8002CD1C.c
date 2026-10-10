#include "game_first2_registry.h"

/* GTMAIN.EXE 8002CD1C */
uint32 gt_draft_8002CD1C(uint32 controller)
{
    return r_u8(gtmain_input_packet(controller << 2)) == 0u;
}
