#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003BA30(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v14 = 0;
    sint8 v15 = 0;
    sint32 v16 = 0;
    sint8 v17 = 0;
    sint32 v18 = 0;
    sint32 v19 = 0;
    sint32 v20 = 0;
    sint32 v21 = 0;
    sint32 v22 = 0;
    sint8 v23 = 0;
    sint32 v24 = 0;
    sint8 v25 = 0;
    sint32 v26 = 0;
    uint32 v27 = 0;
    sint32 v28 = 0;
    sint32 v29 = 0;
    sint32 v30 = 0;
    sint16 v31 = 0;
    uint32 v32 = 0;
    sint32 v33 = 0;
    sint32 v34 = 0;
    uint32 v35 = 0;
    sint32 v36 = 0;
    uint32 v37 = 0;
    sint16 v38 = 0;
    sint16 v39 = 0;
    sint32 v40 = 0;
    sint32 v41 = 0;
    sint32 v42 = 0;
    sint32 v43 = 0;
    uint32 v44 = 0;
    sint32 v45 = 0;
    sint32 v46 = 0;
    sint32 v47 = 0;
    uint32 v48 = 0;
    uint32 v49 = 0;
    uint32 v50 = 0;
    sint32 v51 = 0;
    sint32 v52 = 0;
    sint32 result = 0;
    sint32 rectangle[2] = {0};
    sint32 v56 = 0;
    sint32 v57 = 0;
    uint32 v58 = 0;
    sint32 v59 = 0;
    sint32 v60 = 0;
    sint32 v61 = 0;
    sint32 v62 = 0;
    sint32 v63 = 0;
    sint32 v64 = 0;
    uint32 v65 = 0;
    sint32 v66 = 0;
    sint32 v67 = 0;
    sint32 v68 = 0;
    sint32 v69 = 0;
    sint32 v70 = 0;
    uint32 v71 = 0;
    uint32 v72 = 0;
    uint32 v73 = 0;
  v58 = a0;
  v59 = a3;
  v68 = 0;
  v69 = 0;
  gt_draft_unresolved_call(0x8006C860u, 0, 0, 0, 0, 0, 0, 0, 0);
  v14 = (uint16)v59;
  v15 = (sint8)r_u8(a1 + 1u * (2));
  v60 = v15;
  v16 = v15 / 2;
  v62 = (sint8)r_u8(a1 + 1u * (1));
  v17 = (sint8)r_u8(a1 + 1u * (8));
  v18 = v17 & 0x1F;
  v66 = (sint8)r_u8(a1 + 1u * (9)) + (sint8)r_u8(a1 + 1u * (10));
  v67 = v17 & 0x20;
  v19 = v62 - v16;
  v64 = (sint8)r_u8(a1 + 1u * (3));
  if ( v62 - v16 < 0 )
    v19 = 0;
  v20 = (sint8)r_u8(a1);
  if ( v20 < v19 + v60 )
    v19 = v20 - v60;
  v21 = (sint8)r_u8(a1 + 1u * (12)) - v16;
  v61 = v19 + v60;
  v63 = v21;
  if ( v21 < 0 )
    v63 = 0;
  v22 = v63;
  if ( v20 < v63 + v60 )
  {
    v63 = v20 - v60;
    v22 = v20 - v60;
  }
  if ( v19 != v22 )
  {
    v23 = 3;
    if ( v22 >= v19 )
      v23 = 4;
    w_u8(a1 + 1u * (13), v23);
    w_u8(a1 + 1u * (14), 8);
  }
  v24 = (sint8)r_u8(a1 + 1u * (13));
  v60 = 0;
  if ( v24 )
  {
    v25 = (sint8)r_u8(a1 + 1u * (14)) - 1;
    w_u8(a1 + 1u * (14), v25);
    if ( v25 > 0 )
    {
      v26 = (sint8)r_u8(a1 + 1u * (13));
      if ( v26 == 3 )
      {
        --v19;
        v14 -= v66 * (8 - v25) / 8;
        v60 = v66 * (8 - v25) / 8;
        v68 = v19;
        v69 = v61 - 1;
      }
      else if ( v26 == 4 )
      {
        ++v61;
        v14 -= v66 * v25 / 8;
        v60 = v66 * v25 / 8;
        v69 = v19;
        v68 = v61 - 1;
      }
      v60 -= v66;
    }
    else
    {
    w_u8(a1 + 1u * (13), 0);
    w_u8(a1 + 1u * (14), 0);
    }
  }
  v63 = v19;
  v70 = 0;
  v65 = (uint32)(r_u32(a1 + 4u * (1)) + 8 * v19);
  if ( v19 < v61 )
  {
    v71 = gt_draft_buffer_address(rectangle, sizeof(rectangle)) /* TODO Adapter lifecycle */;
    v72 = v58 + 4;
    v73 = a7;
    v27 = (uint32)(v65 + 4);
    while ( 2 )
    {
      if ( a6 )
    w_u32(v73 + 4u * (4), v19);
      v28 = a5;
      if ( (r_u8(v65) & 0x40) != 0 )
        v28 = a5 >> 1;
      if ( (sint8)r_u8(a1 + 1u * (13)) )
      {
        if ( v19 == v68 )
        {
          v29 = v28 * (sint8)r_u8(a1 + 1u * (14));
          v28 = v29 >> 3;
          if ( v29 < 0 )
            v28 = (v29 + 7) >> 3;
        }
        else if ( v19 == v69 )
        {
          v28 -= v28 * (sint8)r_u8(a1 + 1u * (14)) / 8;
        }
      }
      if ( (r_u8(v65) & 0x3Fu) < 2 || (r_u8(v65) & 0x3Fu) >= 4 && ((r_u8(v65) & 0x3Fu) >= 0xC || (r_u8(v65) & 0x3Fu) < 0xA) )
      {
        if ( v19 == v62 && v67 == 0 )
          v30 = gt_draft_unresolved_call(0x8003553Cu, v28 >> 1, 0, 0, 0, 0, 0, 0, 0);
        else
          v30 = gt_draft_unresolved_call(0x80035458u, v28 >> 1, 0, 0, 0, 0, 0, 0, 0);
        gt_draft_unresolved_call(0x8006C874u, v30, 0, 0, 0, 0, 0, 0, 0);
      }
      else if ( v19 == v62 && v67 == 0 )
      {
        v28 = gt_draft_unresolved_call(0x80035608u, 0, v28, 0, 0, 0, 0, 0, 0);
      }
      v31 = a2;
      switch ( r_u8(v65) & 0x3F )
      {
        case 1:
          goto LABEL_71;
        case 2:
          goto LABEL_56;
        case 3:
          goto LABEL_48;
        case 4:
          v14 += v64;
          goto LABEL_79;
        case 5:
          v14 += v64;
LABEL_71:
          v44 = (sint16)r_u16(v27);
          gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
          if ( v18 )
          {
            if ( v18 != 2 )
              goto LABEL_77;
    v45 = (v45 & 0xFFFF0000u) | (((uint32)(gt_draft_unresolved_call(0x8006C900u, 2, v44, 0, 0, 0, 0, 0, 0)) & 0xFFFFu) << 0);
          }
          else
          {
            v45 = gt_draft_unresolved_call(0x8006C900u, 2, v44, 0, 0, 0, 0, 0, 0) >> 1;
          }
          v31 = a2 - v45;
LABEL_77:
          v46 = v31;
          v47 = (sint16)v14;
          v48 = v44;
          v49 = v58 + 4;
          goto LABEL_86;
        case 6:
          v14 += v64;
LABEL_56:
          v35 = (uint32)(sint16)r_u16(v27);
          if ( v18 )
          {
            if ( v18 != 2 )
              goto LABEL_62;
    v36 = (v36 & 0xFFFF0000u) | (((uint32)(r_u16(v35 + 2u * (4))) & 0xFFFFu) << 0);
          }
          else
          {
            v36 = r_u16(v35 + 2u * (4)) >> 1;
          }
          v31 = a2 - v36;
LABEL_62:
          gt_draft_8003AD48(v35, (sint32)(v58 + 4), v31, (sint16)v14, 0, 0, 0, 0);
          goto LABEL_87;
        case 7:
          v14 += v64;
LABEL_48:
          v32 = (sint16)r_u16(v27);
          if ( v18 )
          {
            v33 = v28 >> 1;
            if ( v18 != 2 )
              goto LABEL_54;
            v34 = ((uint16)(sint16)r_u16(v32 + 2u * (4)) * r_u32(v27 - 4u * (6))) >> 7;
          }
          else
          {
            v34 = ((uint16)(sint16)r_u16(v32 + 2u * (4)) * r_u32(v27 - 4u * (6))) >> 8;
          }
          v31 = a2 - v34;
          v33 = v28 >> 1;
LABEL_54:
          gt_draft_8003ADC0((sint16)r_u16(v27), (sint32)(v58 + 4), v31, v14, v33, r_u32(v27 - 4u * (6)), rectangle[0], rectangle[1]);
          goto LABEL_87;
        case 8:
        case 0xA:
          goto LABEL_46;
        case 9:
        case 0xB:
          v14 += v64;
LABEL_46:
          v31 = a2;
          goto LABEL_87;
        case 0xC:
          goto LABEL_65;
        case 0xD:
          v14 += v64;
LABEL_65:
          v38 = 320;
          v37 = (sint16)r_u16(v27);
          v39 = v14 + 17;
          goto LABEL_66;
        case 0xE:
          v37 = (sint16)r_u16(v27);
          v38 = gt_draft_unresolved_call(0x8006C900u, 2, (sint16)r_u16(v27), 0, 0, 0, 0, 0, 0);
          v39 = v14 + 17;
LABEL_66:
    v56 = (v56 & 0xFFFF0000u) | (((uint32)(a2) & 0xFFFFu) << 0);
    v56 = (v56 & 0x0000FFFFu) | (((uint32)(v39) & 0xFFFFu) << 16);
    v57 = (v57 & 0xFFFF0000u) | (((uint32)(a2 + v38 - 1) & 0xFFFFu) << 0);
    v57 = (v57 & 0x0000FFFFu) | (((uint32)(v39) & 0xFFFFu) << 16);
          rectangle[0] = v56;
          rectangle[1] = v57;
          v40 = gt_draft_unresolved_call(0x80035398u, r_u32(v27 - 4u * (6)), v28, 0, 0, 0, 0, 0, 0);
          gt_draft_unresolved_call(0x8006F4F0u, v72, v71, v40 | 0x2000000, 0, 0, 0, 0, 0);
    v56 = (v56 & 0xFFFF0000u) | (((uint32)(a2 - 20) & 0xFFFFu) << 0);
    v56 = (v56 & 0x0000FFFFu) | (((uint32)(v14 + 3) & 0xFFFFu) << 16);
    v57 = (v57 & 0xFFFF0000u) | (((uint32)(a2 - 10) & 0xFFFFu) << 0);
    v57 = (v57 & 0x0000FFFFu) | (((uint32)(v14 + 13) & 0xFFFFu) << 16);
          rectangle[0] = v56;
          rectangle[1] = v57;
          v41 = gt_draft_unresolved_call(0x80035398u, r_u32(v27 - 4u * (6)), v28, 0, 0, 0, 0, 0, 0);
          gt_draft_unresolved_call(0x8006F4F0u, v72, v71, v41, 0, 0, 0, 0, 0);
          gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
          if ( !v18 )
          {
            v43 = gt_draft_unresolved_call(0x8006C900u, 2, v37, 0, 0, 0, 0, 0, 0) >> 1;
            goto LABEL_84;
          }
          v42 = 2;
          if ( v18 == 2 )
            goto LABEL_83;
          goto LABEL_85;
        default:
LABEL_79:
          v37 = (sint16)r_u16(v27);
          gt_draft_unresolved_call(0x8006C790u, 3, 0, 0, 0, 0, 0, 0, 0);
          if ( v18 )
          {
            v42 = 3;
            if ( v18 != 2 )
              goto LABEL_85;
LABEL_83:
    v43 = (v43 & 0xFFFF0000u) | (((uint32)(gt_draft_unresolved_call(0x8006C900u, v42, v37, 0, 0, 0, 0, 0, 0)) & 0xFFFFu) << 0);
          }
          else
          {
            v43 = gt_draft_unresolved_call(0x8006C900u, 3, v37, 0, 0, 0, 0, 0, 0) >> 1;
          }
LABEL_84:
          v31 = a2 - v43;
LABEL_85:
          v46 = v31;
          v47 = (sint16)v14;
          v48 = v37;
          v49 = v58 + 4;
LABEL_86:
          gt_draft_unresolved_call(0x8006CA10u, v49, v46, v47, v48, 0, 0, 0, 0);
LABEL_87:
          if ( a6 )
          {
    w_u32(v73 + 4u * (52), v28 >> 1);
            v50 = (a7 + 2u * (v70));
    w_u16(v50 + 2u * (25), v31);
    w_u16(v50 + 2u * (49), v14);
          }
          v27 += 4;
          ++v19;
          v14 += v66;
          v65 += 8;
          v73 = (uint32)((uint32)v73 + 2);
          ++v70;
          if ( v19 >= v61 )
            break;
          continue;
      }
      break;
    }
  }
  if ( r_u8(0x80091B41u) )
  {
    v51 = gt_draft_unresolved_call(0x80035398u, 0, a5, 0, 0, 0, 0, 0, 0);
    v52 = v51;
    if ( v63 )
      gt_draft_unresolved_call(0x8003B5B8u, v58, a2, v59 - 8, v51, 0, 0, 0, 0);
    if ( v61 != (sint8)r_u8(a1) )
      gt_draft_unresolved_call(0x8003B600u, v58, a2, v14 + v60 + 9, v52, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006F590u, v58, 544, 0, 0, 0, 0, 0, 0);
  }
  result = -1;
  if ( a6 )
  {
    w_u16(a7, v60);
    w_u32(a7 + 4u * (v70 + 2), -1);
  }
  return result;
}
