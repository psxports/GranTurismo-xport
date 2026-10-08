#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80030828(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v3 = 0;
    sint32 v6 = 0;
    sint32 v7 = 0;
  v3 = a0;
  if ( a2 >= 0 && (sint16)r_u16(0x800B63DEu) )
  {
    gt_draft_unresolved_call(0x8006C790u, 3, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006C874u, 8421504, 0, 0, 0, 0, 0, 0, 0);
    v6 = a1;
    v7 = a1 + 44;
    gt_draft_unresolved_call(0x8006CA10u, v3, r_u16(0x800911F8u + 2u * (4 * a2)), v6, r_u32(0x800911FCu + 4u * (2 * a2)), 0, 0, 0, 0);
    a0 = v3;
  }
  else
  {
    v7 = a1 + 28;
  }
  return gt_draft_unresolved_call(0x8003061Cu, a0, v7, 0, 0, 0, 0, 0, 0);
}
