#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Original BREAK exception handler is an unresolved adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001EE20(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v2;
  uint32 v3;
  uint32 v4;
  sint32 v5;
  sint32 v6;
  sint32 result;
  sint32 v8;
  sint32 v9;
  uint32 v10;
  sint32 v11;
  v11 = gt_draft_unresolved_call(0x8006C2B4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v2 = gt_draft_unresolved_call(0x8006B828u, a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v3 = gt_draft_unresolved_call(0x8006C2ECu, v2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v4 = gt_draft_unresolved_call(0x8006C31Cu, gt_draft_buffer_address(&v11, sizeof(v11)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v5 = r_u16((uint32)(0x800C4C5A));
  if (!v3)
    gt_draft_exception(7u);
  v6 = gt3_umod(v4, v3);
  result = 0;
  if ((r_u16((uint32)(0x800C4C5A)) < 100))
  {
    gt_draft_unresolved_call(0x8006C144u, (sint32)(0u - (uint32)(2146603044)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gt_draft_unresolved_call(0x80024D38u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    v8 = gt_draft_unresolved_call(0x8006B798u, a0, (sint32)(0u - (uint32)(2146603044)), 0, 0u, 0u, 0u, 0u, 0u);
    v9 = (sint32)((uint32)(8) * (uint32)(v8));
    if ((v8 < 0))
    {
      gt_draft_unresolved_call(0x8008B7D8u, 0x8008B8E4u, a0, 0u, 0u, 0u, 0u, 0u, 0u);
      v8 = 0;
      v9 = 0;
    }
    v10 = (uint32)(((uint32)(((uint32)(((uint32)(r_u32((uint32)(0x800D6FEC))) - (uint32)(2146603044))) + (uint32)(24))) + (uint32)((sint32)((uint32)(4) * (uint32)((sint32)((uint32)((sint32)((uint32)(4) * (uint32)((sint32)((uint32)(v9) - (uint32)(v8))))) - (uint32)(v8)))))));
    gtmain_memory_copy((sint32)((uint32)((sint32)((uint32)(96) * (uint32)(v5))) - (uint32)(2146677268)), v10, 96);
    w_u16((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(96) * (uint32)(v5))) - (uint32)(2146677684))) + (uint32)(416)))), (uint32)(((uint32)(r_u16((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(96) * (uint32)(v5))) - (uint32)(2146677684))) + (uint32)(416)))))) | (uint32)(((uint32)((uint16)(v6)) << ((uint32)(12) & 31u))))));
    w_u32((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(4) * (uint32)(v5))) - (uint32)(2146677684))) + (uint32)(16)))), (uint32)(r_u32((uint32)((uint32)(((uint32)(((uint32)(((uint32)(r_u32((uint32)(0x800D7088))) - (uint32)(2146603044))) + (uint32)(((uint32)(424) * (uint32)((r_u16((uint32)(v10)) & 0xFFF)))))) - (uint32)(12)))))));
    result = 1;
    (w_u16(0x800C4C5A, (uint32)(r_u16((uint32)(0x800C4C5A))) + 1u), r_u16((uint32)(0x800C4C5A)));
  }
  return result;
}

