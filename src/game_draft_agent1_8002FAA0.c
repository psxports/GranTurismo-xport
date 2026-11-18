#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* GTMAIN.EXE 8002FAA0 */
/* Original: GTMAIN.EXE:sub_8002FAA0. */ void gtmain_race_status_draw(uint32 a0, sint32 a1, uint32 a2)
{
    sint32 v5 = 0;
    sint32 v6 = 0;
  if ( !(sint16)r_u16(0x800B63D6u) )
  {
    v5 = 8421504;
    v6 = -1;
    switch ( r_u8(a2 + 2004) )
    {
      case 1:
        v5 = 128;
        v6 = 4;
        break;
      case 4:
        v5 = 32896;
        v6 = 6;
        break;
      case 5:
        v5 = 0x8000;
        v6 = 7;
        break;
      case 6:
        v5 = 0x8000;
        v6 = 8;
        break;
      case 0xB:
        v5 = 8421504;
        v6 = 5;
        break;
      default:
        break;
    }
    if ( v6 >= 0 )
    {
      gtmain_text_font(2);
      gtmain_text_color((uint32)v5);
      gtmain_font_text_draw(a0, r_s16(0x800911A8u + 8u * (uint32)v6), a1, r_u32(0x800911ACu + 8u * (uint32)v6));
      gtmain_race_timing_flag_clear();
    }
  }
}

/* TODO Remove the legacy undefined return contract at remaining draft consumers */
uint32 gt_draft_8002FAA0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gtmain_race_status_draw(a0, (sint32)a1, a2);
}
