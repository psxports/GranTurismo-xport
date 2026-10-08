#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002FAA0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
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
      gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8006C874u, v5, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8006CA10u, a0, r_u16(0x800911A8u + 2u * (4 * v6)), a1, r_u32(0x800911ACu + 4u * (2 * v6)), 0, 0, 0, 0);
      gt_draft_unresolved_call(0x8002E208u, 0, 0, 0, 0, 0, 0, 0, 0);
    }
  }
}
