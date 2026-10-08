#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80042158(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v0;
  sint32 v1;
  uint32 v2;
  v0 = gt_draft_unresolved_call(0x8001EB94u, (sint16)r_u16((uint32)(0x800BEA20)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v1 = r_u16((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)((uint8)(r_u32((uint32)((0x800941BDu + 1u * (uint32)((sint16)r_u16((uint32)(0x800BEA20))))))))))));
  if (((sint32)((uint32)(v1) - (uint32)(1)) < 0))
    return (sint32)(0u - (uint32)(1));
  v2 = (uint32)((sint32)((uint32)((sint32)((uint32)(72) * (uint32)((sint32)((uint32)(v1) - (uint32)(1))))) + (uint32)(v0)));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(69)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(12))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(82)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(19))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(68)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(11))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(81)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(18))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(67)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(10))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(80)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(17))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(76)))), (uint32)(gt_draft_unresolved_call(0x800417ACu, (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(30))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(31))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(13))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(38))))), 0u, 0u, 0u, 0u)));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(89)))), (uint32)(gt_draft_unresolved_call(0x800417ACu, (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(45))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(46))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(20))))), (uint8)(r_u8((uint32)((v2 + 1u * (uint32)(53))))), 0u, 0u, 0u, 0u)));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(70)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(13))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(72)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(15))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(71)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(14))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(73)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(16))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(83)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(20))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(85)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(22))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(84)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(21))))));
  w_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(86)))), (uint32)(r_u8((uint32)((v2 + 1u * (uint32)(23))))));
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D520u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D55Cu, (sint16)r_u16((uint32)(0x800BEA20)), v1, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D594u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(67))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5ACu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(80))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5C4u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(68))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5ACu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(81))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5DCu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(69))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5ACu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(82))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D5F4u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(70))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D610u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(72))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D62Cu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(71))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D648u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(73))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D664u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(83))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D680u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(85))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D69Cu, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(84))))), 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8008B7D8u, 0x8008D6B8u, r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BEA64))) + (uint32)(86))))), 0u, 0u, 0u, 0u, 0u, 0u);
  return 0;
}

