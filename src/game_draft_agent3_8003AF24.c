#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Call 8003AEE8 has additional inferred arguments requiring integration review */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003AF24(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 draft_buffer_v32[2];
  sint32 v6;
  sint32 v7;
  uint32 v8;
  sint32 v9;
  sint32 v10;
  sint16 v11;
  sint32 v12;
  sint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 v16;
  sint16 v17;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  sint16 v21;
  sint16 v22;
  sint32 v23;
  sint32 v24;
  sint32 v25;
  sint32 v26;
  sint32 v27;
  sint32 v28;
  uint16 v29;
  sint32 v30;
  sint32 v34;
  sint32 v35;
  sint32 v36;
  sint32 v37;
  sint32 v38;
  sint32 v39;
  sint32 v40;
  sint32 v41;
  sint8 v42;
  v36 = (sint32)a0;
  v37 = (sint32)a3;
  v38 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))))) + (uint32)(4));
  v39 = ((r_u32((uint32)(0x800BFF6C)) == 0)) ? (0xF0) : (0);
  v6 = (sint32)a2;
  v42 = r_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(274)))));
  if (((sint32)a2 < 0))
    v6 = (sint32)(0u - (uint32)((sint32)a2));
  if ((v6 >= 181))
    v6 = 180;
  v40 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(4) * (uint32)((sint8)r_u8((uint32)((uint32)((sint32)((uint32)(v36) + (uint32)(256)))))))) + (uint32)(v36))) + (uint32)(192)))));
  v7 = gt3_sdiv((sint32)((uint32)(255) * (uint32)((sint32)((uint32)(180) - (uint32)(v6)))), 180);
  if ((v37 == 1))
    v40 = r_u32((uint32)((uint32)((sint32)((uint32)(v36) + (uint32)(188)))));
  v8 = r_u32((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(4)))));
  if (v8)
  {
    v41 = 0;
    if (v42)
    {
      gt_draft_unresolved_call(0x8006C790u, 3, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, (((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u))) | ((uint32)((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u)))) << ((uint32)(8) & 31u))) | ((uint32)((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u)))) << ((uint32)(16) & 31u))), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v16 = gt_draft_unresolved_call(0x8006C900u, 3, v8, 0u, 0u, 0u, 0u, 0u, 0u);
      v17 = v16;
      v12 = (sint32)((uint32)((sint32)a1) + (uint32)(((sint32)((sint32)((uint32)(320) - (uint32)(v16))) >> ((uint32)(1) & 31u))));
      if (r_u8((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(1))))))
      {
        v14 = (sint32)((uint32)((sint32)a2) + (uint32)(17));
        if ((r_u8((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(1))))) == 3))
        {
          v14 = (sint32)((uint32)((sint32)a2) + (uint32)(109));
          v41 = 1;
        }
      }
      else
      {
        v12 = (sint32)((uint32)((sint32)a1) + (uint32)(16));
        v14 = (sint32)((uint32)((sint32)a2) + (uint32)(17));
      }
      gt_draft_unresolved_call(0x8006CA10u, (sint32)((uint32)(v38) + (uint32)(16)), v12, v14, v8, 0u, 0u, 0u, 0u);
      v34 = (((uint32)(v34) & 0xFFFF0000u) | (((uint32)(v12) << 0u) & 0x0000FFFFu));
      v34 = (((uint32)(v34) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)(v14) + (uint32)(24))) << 16u) & 0xFFFF0000u));
      v35 = (((uint32)(v35) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)((uint32)(v12) + (uint32)(v17))) - (uint32)(1))) << 0u) & 0x0000FFFFu));
      v35 = (((uint32)(v35) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)(v14) + (uint32)(24))) << 16u) & 0xFFFF0000u));
      draft_buffer_v32[0] = v34;
      draft_buffer_v32[1] = v35;
      gt_draft_unresolved_call(0x8006F4F0u, (sint32)((uint32)(v38) + (uint32)(16)), gt_draft_buffer_address(&draft_buffer_v32[0], sizeof(draft_buffer_v32)), (uint8)(v7), 0u, 0u, 0u, 0u, 0u);
      goto LABEL_24;
    }
    v9 = gt_draft_8003A8F4(3, v8, 0, 0u, 0u, 0u, 0u, 0u);
    v10 = (sint8)r_u8((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(1)))));
    v11 = v9;
    if ((v10 == 1))
    {
      v13 = (sint32)((uint32)(v9) - (uint32)(304));
    }
    else
      if ((v10 >= 2))
    {
      if ((v10 == 2))
        v13 = (sint32)((uint32)(((sint32)(v9) >> ((uint32)(1) & 31u))) - (uint32)(160));
      else
        v13 = (sint32)((uint32)(v9) - (uint32)(304));
    }
    else
    {
      v12 = (sint32)((uint32)((sint32)a1) + (uint32)(16));
      if (!(r_u8((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(1)))))))
      {
        LABEL_18:
        v14 = (((uint32)(v14) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(19))) << 0u) & 0x0000FFFFu));

        v34 = (((uint32)(v34) & 0xFFFF0000u) | (((uint32)(v12) << 0u) & 0x0000FFFFu));
        v34 = (((uint32)(v34) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(38))) << 16u) & 0xFFFF0000u));
        v35 = (((uint32)(v35) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)((uint32)(v12) + (uint32)(v11))) - (uint32)(1))) << 0u) & 0x0000FFFFu));
        v35 = (((uint32)(v35) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(38))) << 16u) & 0xFFFF0000u));
        draft_buffer_v32[0] = v34;
        draft_buffer_v32[1] = v35;
        v15 = (sint32)((uint32)(v38) + (uint32)(16));
        gt_draft_unresolved_call(0x8006F4F0u, (sint32)((uint32)(v38) + (uint32)(16)), gt_draft_buffer_address(&draft_buffer_v32[0], sizeof(draft_buffer_v32)), (uint8)(v7), 0u, 0u, 0u, 0u, 0u);
        gt_draft_unresolved_call(0x8006C790u, 3, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        gt_draft_unresolved_call(0x8006C874u, (((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u))) | ((uint32)((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u)))) << ((uint32)(8) & 31u))) | ((uint32)((uint8)(((sint32)(v7) >> ((uint32)(1) & 31u)))) << ((uint32)(16) & 31u))), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        gt_draft_8003A8AC(v15, v12, (sint32)((uint32)((sint32)a2) + (uint32)(14)), v8, 0u, 0u, 0u, 0u);
        LABEL_24:
        v18 = gt_draft_unresolved_call(0x80035398u, r_u8((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(24))))), v7, 0u, 0u, 0u, 0u, 0u, 0u);

        if (v41)
        {
          v21 = (sint32)((uint32)(v14) + (uint32)(13));
          v22 = (sint32)((uint32)(v14) + (uint32)(14));
          v34 = (((uint32)(v34) & 0xFFFF0000u) | (((uint32)((sint32)a1) << 0u) & 0x0000FFFFu));
          v34 = (((uint32)(v34) & 0x0000FFFFu) | (((uint32)(v21) << 16u) & 0xFFFF0000u));
          v35 = (((uint32)(v35) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)(v12) - (uint32)(3))) << 0u) & 0x0000FFFFu));
          v35 = (((uint32)(v35) & 0x0000FFFFu) | (((uint32)(v22) << 16u) & 0xFFFF0000u));
          draft_buffer_v32[0] = v34;
          draft_buffer_v32[1] = v35;
          v23 = (sint32)((uint32)(v38) + (uint32)(16));
          gt_draft_unresolved_call(0x8006F4F0u, (sint32)((uint32)(v38) + (uint32)(16)), gt_draft_buffer_address(&draft_buffer_v32[0], sizeof(draft_buffer_v32)), v18, 0u, 0u, 0u, 0u, 0u);
          v34 = (((uint32)(v34) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a1) - (uint32)((sint32)((uint32)((sint32)((uint32)(v12) - (uint32)((sint32)a1))) - (uint32)(322))))) << 0u) & 0x0000FFFFu));
          v34 = (((uint32)(v34) & 0x0000FFFFu) | (((uint32)(v21) << 16u) & 0xFFFF0000u));
          v35 = (((uint32)(v35) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(319))) << 0u) & 0x0000FFFFu));
          v35 = (((uint32)(v35) & 0x0000FFFFu) | (((uint32)(v22) << 16u) & 0xFFFF0000u));
          draft_buffer_v32[0] = v34;
          draft_buffer_v32[1] = v35;
          v20 = v23;
          v19 = v18;
        }
        else
        {
          v34 = (((uint32)(v34) & 0xFFFF0000u) | (((uint32)((sint32)a1) << 0u) & 0x0000FFFFu));
          v34 = (((uint32)(v34) & 0x0000FFFFu) | (((uint32)((sint32)a2) << 16u) & 0xFFFF0000u));
          v35 = (((uint32)(v35) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(319))) << 0u) & 0x0000FFFFu));
          v35 = (((uint32)(v35) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(27))) << 16u) & 0xFFFF0000u));
          draft_buffer_v32[0] = v34;
          draft_buffer_v32[1] = v35;
          v19 = v18;
          v20 = (sint32)((uint32)(v38) + (uint32)(16));
        }
        gt_draft_unresolved_call(0x8006F4F0u, v20, gt_draft_buffer_address(&draft_buffer_v32[0], sizeof(draft_buffer_v32)), v19, 0u, 0u, 0u, 0u, 0u);
        gt_draft_8003AEE8(r_u32((uint32)((uint32)((sint32)((uint32)(v40) + (uint32)(12))))), (sint32)((uint32)(v38) + (uint32)(32)), (sint32)a1, (sint32)a2, v39, v7, draft_buffer_v32[0], draft_buffer_v32[1]);
        if (!v42)
        {
          gt_draft_unresolved_call(0x8006F498u, (sint32)((uint32)(v38) + (uint32)(64)), (sint32)((uint32)(v39) << ((uint32)(11) & 31u)), (sint32)((uint32)(v39) << ((uint32)(10) & 31u)), ((sint32)((uint32)((sint32)((uint32)(v39) + (uint32)(207))) << ((uint32)(10) & 31u)) | 0x13F), 0u, 0u, 0u, 0u);
          if (!v37)
          {
            v24 = v36;
            switch ((r_u8((uint32)((uint32)((sint32)((uint32)(v36) + (uint32)(258))))) & 0xF))
            {
              case 0:

              case 1:

              case 4:
                goto LABEL_33;

              case 2:
                v25 = v38;
                v26 = r_u32((uint32)(0x80092380u));
                v27 = r_u32((uint32)(0x8009237Cu));
                v28 = 13631600;
                goto LABEL_32;

              case 3:
                v25 = v38;
                v26 = r_u32((uint32)(0x80092380u));
                v27 = r_u32((uint32)(0x8009237Cu));
                v28 = 13631608;
                LABEL_32:
              gt_draft_unresolved_call(0x800706BCu, v25, v28, v26, v27, 0u, 0u, 0u, 0u);

                LABEL_33:
              v24 = v36;

                break;

              default:
                break;

            }

            v29 = 192;
            v30 = ((r_u8((uint32)((uint32)(((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)(v24) + (uint32)(152)))))) + (uint32)(1))))) & 0xF0) == 0x20);
            if (((r_u8((uint32)((uint32)((sint32)((uint32)(v24) + (uint32)(258))))) & 0x10) != 0))
            {
              v29 = 160;
              v30 += 2;
            }
            gt_draft_unresolved_call(0x800706BCu, v38, (v29 | 0xD00000), r_u32((uint32)((uint32)((0x80092334u + 1u * (uint32)((sint32)((uint32)((sint32)((uint32)(16) * (uint32)(v30))) + (uint32)(12))))))), r_u32((uint32)((uint32)((0x80092334u + 1u * (uint32)((sint32)((uint32)((sint32)((uint32)(16) * (uint32)(v30))) + (uint32)(8))))))), 0u, 0u, 0u, 0u);
          }
          v34 = 13631488;
          v35 = 13631807;
          draft_buffer_v32[0] = 13631488;
          draft_buffer_v32[1] = 13631807;
          gt_draft_unresolved_call(0x8006F4F0u, v38, gt_draft_buffer_address(&draft_buffer_v32[0], sizeof(draft_buffer_v32)), 3355443, 0u, 0u, 0u, 0u, 0u);
          gt_draft_unresolved_call(0x8006F590u, v38, ((uint16)((r_u8((uint32)(0x80092335u)) | 0x20)) | 0x200), 0u, 0u, 0u, 0u, 0u, 0u);
        }
        gt_draft_unresolved_call(0x8006F498u, (sint32)((uint32)(v38) + (uint32)(16)), (sint32)((uint32)(v39) << ((uint32)(11) & 31u)), (sint32)((uint32)(v39) << ((uint32)(10) & 31u)), ((sint32)((uint32)((sint32)((uint32)(v39) + (uint32)(239))) << ((uint32)(10) & 31u)) | 0x13F), 0u, 0u, 0u, 0u);
        return gt_draft_unresolved_call(0x8006F498u, (sint32)((uint32)(v38) + (uint32)(16360)), (sint32)((uint32)(v39) << ((uint32)(11) & 31u)), (sint32)((uint32)(v39) << ((uint32)(10) & 31u)), ((sint32)((uint32)((sint32)((uint32)(v39) + (uint32)(239))) << ((uint32)(10) & 31u)) | 0x13F), 0u, 0u, 0u, 0u);
      }
      v13 = (sint32)((uint32)(v9) - (uint32)(304));
    }
    v12 = (sint32)((uint32)((sint32)a1) - (uint32)(v13));
    goto LABEL_18;
  }
  return gt_draft_unresolved_call(0x8006F498u, (sint32)((uint32)(v38) + (uint32)(16360)), (sint32)((uint32)(v39) << ((uint32)(11) & 31u)), (sint32)((uint32)(v39) << ((uint32)(10) & 31u)), ((sint32)((uint32)((sint32)((uint32)(v39) + (uint32)(239))) << ((uint32)(10) & 31u)) | 0x13F), 0u, 0u, 0u, 0u);
}

