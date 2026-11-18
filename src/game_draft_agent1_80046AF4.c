#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
/* Original: GTMAIN.EXE:sub_80046AF4. */ uint32 gt_draft_80046AF4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v4 = 0;
    sint16 v5 = 0;
    sint32 result = 0;
    uint32 pressed;
  v4 = -1;
  if ( (sint8)r_u8(a0 + 13) > 0 )
    return v4;
  v5 = r_u16(a1 + 4) | r_u16(a1 + 8);
  if ( (v5 & 0x80) != 0 )
  {
    gtmain_race_sound_effect(0, 0x4000, 0x40, 0);
    w_u8(a0 + 12, 0);
  }
  if ( (v5 & 0x20) != 0 )
  {
    gtmain_race_sound_effect(0, 0x4000, 0x40, 0);
    w_u8(a0 + 12, 1);
  }
  pressed = r_u16(a1 + 4);
  if ( (pressed & 0x6000u) != 0 )
    v4 = (sint8)r_u8(a0 + 12);
  result = v4;
  if ( (pressed & 0x9000u) != 0 )
    return -2;
  return result;
}
