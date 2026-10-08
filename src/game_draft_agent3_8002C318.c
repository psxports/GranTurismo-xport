#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8002C264 has additional inferred arguments requiring integration review */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Original BREAK exception handler is an unresolved adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
/* TODO Unresolved string buffer marshalling retains the literal */
uint32 gt_draft_8002C318(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v5;
  sint32 v6;
  sint32 v7;
  uint32 v11;
  uint32 v12;
  sint8 v13;
  sint8 v14;
  sint8 v15;
  sint32 v16;
  uint32 v17;
  sint32 v19;
  sint32 v20;
  sint32 v21;
  sint32 vars0;
  v5 = r_u32((uint32)((a3 + 4u * (uint32)(1174))));
  v6 = (sint32)((uint32)(2298) * (uint32)(r_u32((uint32)((a3 + 4u * (uint32)(39))))));
  v7 = gt3_sdiv(v6, v5);
  if (!r_u32((uint32)((a3 + 4u * (uint32)(1174)))))
    gt_draft_exception(7u);
  if (((v5 == (sint32)(0u - (uint32)(1))) && (v6 == 0x80000000)))
    gt_draft_exception(6u);
  v11 = r_u32((uint32)(0x800BFE4C));
  v12 = (uint32)(((uint32)(r_u32((uint32)(0x800BFE4C))) + (uint32)(8)));
  w_u32((uint32)((uint32)(((uint32)(r_u32((uint32)(0x800BFE4C))) + (uint32)(4)))), (uint32)(947945727));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(8)))), (uint32)(601));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(16)))), (uint32)(601));
  w_u32((uint32)((uint32)(((uint32)(v11) + (uint32)(12)))), (uint32)(128));
  w_u32((uint32)((uint32)(((uint32)(v11) + (uint32)(20)))), (uint32)(8421631));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(24)))), (uint32)((sint32)(0u - (uint32)(156))));
  w_u32((uint32)((uint32)(((uint32)(v11) + (uint32)(28)))), (uint32)(128));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(32)))), (uint32)((sint32)(0u - (uint32)(156))));
  w_u32((uint32)((uint32)(v11)), (uint32)(0x8000000));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(10)))), (uint32)(9));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(18)))), (uint32)((sint32)(0u - (uint32)(9))));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(26)))), (uint32)(24));
  w_u16((uint32)((uint32)(((uint32)(v11) + (uint32)(34)))), (uint32)((sint32)(0u - (uint32)(24))));
  gt_draft_unresolved_call(0x8002BF84u, v12, (sint32)((uint32)(v7) + (uint32)(1399)), (sint32)a1, (sint32)a2, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8002BF84u, (uint32)(((uint32)(v11) + (uint32)(16))), (sint32)((uint32)(v7) + (uint32)(1399)), (sint32)a1, (sint32)a2, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8002BF84u, (uint32)(((uint32)(v11) + (uint32)(24))), (sint32)((uint32)(v7) + (uint32)(1399)), (sint32)a1, (sint32)a2, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8002BF84u, (uint32)(((uint32)(v11) + (uint32)(32))), (sint32)((uint32)(v7) + (uint32)(1399)), (sint32)a1, (sint32)a2, 0u, 0u, 0u, 0u);
  v13 = (sint8)r_u8((uint32)(a0));
  v14 = (sint8)r_u8((uint32)((a0 + 1u * (uint32)(1))));
  v15 = (sint8)r_u8((uint32)((a0 + 1u * (uint32)(2))));
  w_u8((uint32)(a0), (uint32)(v11));
  w_u8((uint32)((a0 + 1u * (uint32)(1))), (uint32)((((uint32)(v11) >> 8u) & 0xFFu)));
  w_u8((uint32)((a0 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)(v11) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
  w_u8((uint32)((uint32)(v11)), (uint32)(v13));
  w_u8((uint32)((uint32)(((uint32)(v11) + (uint32)(1)))), (uint32)(v14));
  w_u8((uint32)((uint32)(((uint32)(v11) + (uint32)(2)))), (uint32)(v15));
  w_u32((uint32)(0x800BFE4C), (uint32)(((uint32)(v11) + (uint32)(36))));
  gt_draft_8002C028(a0, (sint32)a1, (sint32)a2, (sint32)(a3), 0u, 0u, 0u, 0u);
  v16 = r_u32((uint32)((a3 + 4u * (uint32)(38))));
  if ((v16 < 0))
    v16 = (sint32)(0u - (uint32)(v16));
  gt_draft_unresolved_call(0x8006C874u, 16512, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C6C8u, a0, (sint32)((uint32)((sint32)a1) + (uint32)(26)), (sint32)((uint32)((sint32)a2) + (uint32)(48)), gt_draft_buffer_address((void *)"%3d", sizeof("%3d")), v16, 0u, 0u, 0u);
  gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v17 = 0x8008C03Cu;
  if (!r_u8((uint32)(0x800CB763)))
    v17 = 0x8008C034u;
  gt_draft_unresolved_call(0x8006CA10u, a0, (sint32)((uint32)((sint32)a1) + (uint32)(42)), (sint32)((uint32)((sint32)a2) + (uint32)(66)), v17, 0u, 0u, 0u, 0u);
  v20 = (((uint32)(v20) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(47))) << 0u) & 0x0000FFFFu));
  v21 = (((uint32)(v21) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(63))) << 0u) & 0x0000FFFFu));
  v20 = (((uint32)(v20) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(28))) << 16u) & 0xFFFF0000u));
  v21 = (((uint32)(v21) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(44))) << 16u) & 0xFFFF0000u));
  return gt_draft_8002C264((sint32)(a0), (sint32)((uint32)((sint32)a1) + (uint32)(50)), (sint32)((uint32)((sint32)a2) + (uint32)(28)), (sint32)(gt_draft_buffer_address(&v20, sizeof(v20))), (sint32)(a3),0u,0u,0u);
}

