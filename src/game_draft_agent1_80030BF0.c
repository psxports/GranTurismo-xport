#include "game_draft_registry.h"

/* Original: GTMAIN.EXE:sub_80030BF0. */ uint32 gt_draft_80030BF0(void)
{
    return (r_u16(0x800BC1C8) & 0x6000) != 0;
}
