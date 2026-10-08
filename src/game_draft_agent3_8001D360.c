#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Continued from the existing camera or packet translation without the guest save frame */
/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001D360(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 draft_buffer_v8[3];
  sint32 v2;
  uint32 v3;
  uint32 v4;
  sint32 v5;
  sint32 v6;
  v2 = (sint32)((uint32)((sint32)a0) + (uint32)(120));
  v3 = (uint32)(r_u32((uint32)(0x800B6400)));
  v4 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))));
  gt_draft_8001D308((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(120))), (uint32)(((uint32)(r_u32((uint32)(0x800B6400))) + (uint32)(24))), 0u, 0u, 0u, 0u, 0u, 0u);
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(244)))), (uint32)(gt_draft_80023DCC(r_u16((uint32)((v3 + 2u * (uint32)(3)))), v2, 0u, 0u, 0u, 0u, 0u, 0u)));
  draft_buffer_v8[0] = ((uint32)(r_u32((uint32)((v4 + 4u * (uint32)(557))))) - (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(120)))))));
  draft_buffer_v8[1] = ((uint32)(r_u32((uint32)((v4 + 4u * (uint32)(558))))) - (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(124)))))));
  v5 = r_u32((uint32)((v4 + 4u * (uint32)(559))));
  v6 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(128)))));
  draft_buffer_v8[1] += 52428;
  draft_buffer_v8[2] = (sint32)((uint32)(v5) - (uint32)(v6));
  gt_draft_8001C870((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(140))), gt_draft_buffer_address(&draft_buffer_v8[0], sizeof(draft_buffer_v8)), 0u, 0u, 0u, 0u, 0u, 0u);
  /* Wrapped product is shifted arithmetically in the existing translation */
  {
      uint32 lower = r_u16(v3 + 16u);
      uint32 upper = r_u16(v3 + 18u);
      uint32 product = (upper - lower) * r_u32(0x800B3910u);
      uint32 distance = (uint32)((sint32)product >> 12) + lower;
      return gt_draft_8001DF30(a0, distance, 0u, 0u, 0u, 0u, 0u, 0u);
  }
}
