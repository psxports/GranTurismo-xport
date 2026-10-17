#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002AE40(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    sint32 v1 = 0;
    sint32 i = 0;
    sint32 v3 = 0;
    sint32 v4 = 0;
    sint32 v5 = 0;
    sint32 v6 = 0;
    sint8 v7 = 0;
  result = r_u32(0x800B630Cu);
  if ( !r_u32(0x800B630Cu) )
  {
    w_u32(0x800CB8E0u, 0);
    gt_draft_unresolved_call(0x8007BD58u, -2146649884, 0, 400, 0, 0, 0, 0, 0);
    w_u16(0x800CBA74u, r_u8(0x800B63BCu));
    gtmain_memory_copy(-2146649482, -2146736560, 10);
    w_u16(0x800CBA80u, r_u16(0x800B63C4u));
    w_u16(0x800CBA82u, r_u16(0x800B63C6u));
    gtmain_memory_copy(-2146649468, -2146715760, 96);
    gtmain_memory_copy(-2146649372, -2146715664, 96);
    v1 = -2146649276;
    for ( i = -2146650120; i != -2146649928; i += 16 )
    {
      v3 = r_u32(i + 4);
      v4 = r_u32(i + 8);
      v5 = r_u32(i + 12);
    w_u32(v1, r_u32(i));
    w_u32(v1 + 4, v3);
    w_u32(v1 + 8, v4);
    w_u32(v1 + 12, v5);
      v1 += 16;
    }
    w_u16(v1, r_u32(0x800CB8B8u));
    if ( r_u8(0x800CB7F4u) == 1 )
    {
      v6 = r_u32(0x800CB770u);
      v7 = r_u32(0x800CB774u);
    w_u32(0x800CBC06u, r_u32(0x800CB76Cu));
    }
    else
    {
      v6 = r_u32(0x800CB77Cu);
      v7 = r_u32(0x800CB780u);
    w_u32(0x800CBC06u, r_u32(0x800CB778u));
    }
    w_u32(0x800CBC0Au, v6);
    w_u32(0x800CBC0Eu, v7);
    result = r_u8(0x800B6318u);
    w_u32(0x800CBC0Fu, r_u8(0x800B6318u));
    w_u32(0x800CBC10u, r_u8(0x800CB7F4u));
    w_u32(0x800CBC11u, r_u8(0x800CB7F5u));
    w_u32(0x800CBC12u, r_u8(0x800CB7F6u));
    w_u32(0x800CBC13u, r_u8(0x800CB7F9u));
    w_u32(0x800CBC14u, r_u8(0x800B63D0u));
    w_u32(0x800CBC16u, r_u8(0x800B6394u));
    w_u32(0x800CBC17u, r_u8(0x800B634Cu));
  }
  return result;
}
