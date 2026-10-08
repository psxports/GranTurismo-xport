#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Continued from the existing camera or packet translation without the guest save frame */
/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001DCC8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  uint32 v2;
  sint16 v3;
  sint16 v4;
  sint32 v5;
  sint32 v6;
  v2 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))));
  v3 = r_u16((uint32)((v2 + 2u * (uint32)(40))));
  v4 = r_u16((uint32)((v2 + 2u * (uint32)(39))));
  if ((r_u32((uint32)(0x800B630C)) == 1))
    gt_draft_8001C7F4((sint32)a0, r_u16((uint32)((v2 + 2u * (uint32)(4)))), 0u, 0u, 0u, 0u, 0u, 0u);
  if (((v3 & 0x100) != 0))
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(255)))), (uint32)(1));
  v5 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(258)))));
  v6 = (v5 < 2);
  if (((v4 & 0x200) != 0))
  {
    (v5 += 1u);
    if (!v6)
      v5 = 0;
  }
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(258)))), (uint32)(v5));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(260)))), (uint32)(v5));
  if ((v5 <= 0))
    w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(254)))), (uint32)(1));
  w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(262)))), (uint32)(r_u16((uint32)(0x800CB758))));
  return gt_draft_8001D8A0((sint32)a0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

