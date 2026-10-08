#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
/* TODO Unresolved string buffer marshalling retains the literal */
uint32 gt_draft_8002C028(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 v7;
  sint32 v8;
  sint32 v9;
  sint32 v10;
  sint32 v11;
  sint32 v12;
  sint32 v13;
  sint32 v14;
  sint32 i;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  sint32 result;
  sint32 v20;
  sint32 v21;
  sint32 v22;
  sint32 v23;
  v7 = ((sint32)((uint32)((sint32)a2) << ((uint32)(16) & 31u)) | (sint32)a1);
  v8 = 0;
  v9 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a3) + (uint32)(2350)))));
  v10 = r_u32((uint32)(0x800BFE4C));
  v11 = (sint32)((uint32)((sint32)a3) + (uint32)(2344));
  if ((v9 > 0))
  {
    do
    {
      w_u32((uint32)((uint32)(v10)), (uint32)(50331648));
      if (((v8 & 3) != 0))
        w_u32((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(4)))), (uint32)(1086374080));
      else
        w_u32((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(4)))), (uint32)(1090519039));
      v12 = (sint32)((uint32)(v11) + (uint32)((sint32)((uint32)(16) * (uint32)(v8))));
      w_u32((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(8)))), (uint32)(((uint32)(v7) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v12) + (uint32)(16)))))))));
      w_u32((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(12)))), (uint32)(((uint32)(v7) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v12) + (uint32)(20)))))))));
      v12 = (((uint32)(v12) & 0xFFFF00FFu) | (((uint32)(r_u8((uint32)(a0))) << 8u) & 0x0000FF00u));
      v12 = (((uint32)(v12) & 0xFF00FFFFu) | (((uint32)(r_u8((uint32)((a0 + 1u * (uint32)(1))))) << 16u) & 0x00FF0000u));
      v12 = (((uint32)(v12) & 0x00FFFFFFu) | (((uint32)(r_u8((uint32)((a0 + 1u * (uint32)(2))))) << 24u) & 0xFF000000u));
      w_u16((uint32)((uint32)(a0)), (uint32)(v10));
      w_u8((uint32)((a0 + 1u * (uint32)(2))), (uint32)(((uint32)((uint32)((sint32)((uint32)(v10) << ((uint32)(8) & 31u)))) >> ((uint32)(24) & 31u))));
      w_u8((uint32)((uint32)(v10)), (uint32)((((uint32)(v12) >> 8u) & 0xFFu)));
      w_u8((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(1)))), (uint32)((((uint32)(v12) >> 16u) & 0xFFu)));
      w_u8((uint32)((uint32)((sint32)((uint32)(v10) + (uint32)(2)))), (uint32)((((uint32)(v12) >> 24u) & 0xFFu)));
      (v8 += 1u);
      v10 += 16;
    }
    while ((v8 < v9));
  }
  v13 = v10;
  v14 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a3) + (uint32)(2352)))));
  for (i = 0; (i < (sint32)((uint32)(v9) - (uint32)(1))); v13 += 24)
  {
    w_u32((uint32)((uint32)(v13)), (uint32)(83886080));
    if ((i >= v14))
      w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(4)))), (uint32)(671088768));
    else
      w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(4)))), (uint32)(671088640));
    v16 = (sint32)((uint32)(v11) + (uint32)((sint32)((uint32)(16) * (uint32)(i))));
    w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(8)))), (uint32)(((uint32)(v7) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v16) + (uint32)(20)))))))));
    v17 = r_u32((uint32)((uint32)((sint32)((uint32)(v16) + (uint32)(24)))));
    v18 = (sint32)((uint32)(v11) + (uint32)((sint32)((uint32)(16) * (uint32)((sint32)((uint32)(i) + (uint32)(1))))));
    w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(12)))), (uint32)((sint32)((uint32)(v7) + (uint32)(v17))));
    w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(16)))), (uint32)(((uint32)(v7) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v18) + (uint32)(20)))))))));
    w_u32((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(20)))), (uint32)(((uint32)(v7) + (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v18) + (uint32)(24)))))))));
    v18 = (((uint32)(v18) & 0xFFFF00FFu) | (((uint32)(r_u8((uint32)(a0))) << 8u) & 0x0000FF00u));
    v18 = (((uint32)(v18) & 0xFF00FFFFu) | (((uint32)(r_u8((uint32)((a0 + 1u * (uint32)(1))))) << 16u) & 0x00FF0000u));
    v18 = (((uint32)(v18) & 0x00FFFFFFu) | (((uint32)(r_u8((uint32)((a0 + 1u * (uint32)(2))))) << 24u) & 0xFF000000u));
    w_u16((uint32)((uint32)(a0)), (uint32)(v13));
    w_u8((uint32)((a0 + 1u * (uint32)(2))), (uint32)(((uint32)((uint32)((sint32)((uint32)(v13) << ((uint32)(8) & 31u)))) >> ((uint32)(24) & 31u))));
    w_u8((uint32)((uint32)(v13)), (uint32)((((uint32)(v18) >> 8u) & 0xFFu)));
    w_u8((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(1)))), (uint32)((((uint32)(v18) >> 16u) & 0xFFu)));
    w_u8((uint32)((uint32)((sint32)((uint32)(v13) + (uint32)(2)))), (uint32)((((uint32)(v18) >> 24u) & 0xFFu)));
    (i += 1u);
  }

  w_u32((uint32)(0x800BFE4C), (uint32)(v13));
  gt_draft_unresolved_call(0x8006C790u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  result = gt_draft_unresolved_call(0x8006C874u, 8421504, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
  v20 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)(v11) + (uint32)(6)))));
  v21 = 0;
  if ((v20 > 0))
  {
    result = 0;
    do
    {
      v22 = (sint32)((uint32)((sint32)a1) + (uint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)(v11) + (uint32)(result))) + (uint32)(12)))))));
      if ((v21 >= 40))
        v23 = (sint32)((uint32)(v22) - (uint32)(7));
      else
        v23 = (sint32)((uint32)(v22) - (uint32)(3));
      gt_draft_unresolved_call(0x8006C6C8u, a0, v23, (sint32)((uint32)((sint32)((uint32)((sint32)a2) + (uint32)((sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)(v11) + (uint32)((sint32)((uint32)(16) * (uint32)(v21))))) + (uint32)(14)))))))) - (uint32)(3)), gt_draft_buffer_address((void *)"%d", sizeof("%d")), gt3_sdiv(v21, 4), 0u, 0u, 0u);
      v21 += 4;
      result = (sint32)((uint32)(16) * (uint32)(v21));
    }
    while ((v21 < v20));
  }
  return result;
}

