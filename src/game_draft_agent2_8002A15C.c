#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002A15C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 v0 = 0;
    sint32 v1 = 0;
    sint32 v2 = 0;
    sint8 v4[32] = {0};
  v0 = 0x80091290u;
  if ( r_u8(0x8008F893u) != 1 )
    v0 = 0x80091298u;
  v1 = r_u32(0x800B6368u);
  gt_draft_unresolved_call(0x8002955Cu, r_u32(0x800B6368u), 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x800295A8u, v1, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C874u, 8421504, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, v1, r_u16(0x800911B8u), 64, r_u32(0x800911BCu), 0, 0, 0, 0);
  if ( r_u32(0x800BF52Cu) <= 0x36EE7Fu )
  {
    gt_draft_unresolved_call(0x8006CA50u, gt_draft_buffer_address(v4, sizeof(v4)) /* TODO Adapter lifecycle */, r_u32(0x800BF52Cu), 0, 0, 0, 0, 0, 0);
    v2 = gt_draft_unresolved_call(0x8006C900u, 2, gt_draft_buffer_address(v4, sizeof(v4)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006CA10u, v1, (320 - v2) >> 1, 144, gt_draft_buffer_address(v4, sizeof(v4)) /* TODO Adapter lifecycle */, 0, 0, 0, 0);
  }
  gt_draft_unresolved_call(0x8006CA10u, v1, r_u16(0x800911C0u), 209, r_u32(0x800911C4u), 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006C790u, 3, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006CA10u, v1, (sint16)r_u16(v0), 96, (sint32)r_u32(v0 + 4u * (1)), 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x80029614u, v1, 0, 0, 0, 0, 0, 0, 0);
}
