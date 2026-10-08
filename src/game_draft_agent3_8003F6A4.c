#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Native buffer marshalling and callback output depend on the unresolved buffer adapter */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8003F6A4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 draft_buffer_v40[2];
  sint32 v12;
  sint32 v13;
  sint32 v14;
  sint32 v15;
  sint32 v16;
  sint32 v17;
  sint32 v18;
  sint32 v19;
  sint32 v20;
  sint32 v21;
  sint32 v22;
  sint32 v23;
  sint32 v24;
  sint32 v25;
  sint32 v26;
  sint32 v27;
  sint32 v28;
  sint16 v29;
  sint32 v30;
  sint16 v31;
  uint8 v32;
  uint8 v33;
  sint32 v35;
  sint32 v36;
  sint32 v37;
  sint32 v38;
  sint32 v39;
  uint8 v42[256];
  sint32 v43;
  sint32 v44;
  sint32 v45;
  sint32 v46;
  v12 = 0;
  v13 = 74;
  v45 = 160;
  v14 = (sint32)(0u - (uint32)(1));
  v46 = 1;
  switch ((sint8)r_u8((uint32)(0x801F2480)))
  {
    case 0:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712618)), (sint32)a4, 0u, 0u, 0u);
      v35 = 1;
      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC((sint32)a0, 0x8009228Cu, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v14 = (sint8)r_u8((uint32)(0x80092298u));
      v12 = ((uint32)((uint8)(gt_draft_unresolved_call(0x80035608u, 0, (sint32)a4, 0u, 0u, 0u, 0u, 0u, 0u))) << ((uint32)(16) & 31u));
      v46 = 0;
      goto LABEL_70;

    case 1:
      if ((r_u8((uint32)(0x801F2481)) <= 0))
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712517)), (sint32)a4, 0u, 0u, 0u);
    else
      (w_u8(0x801F2481, (uint32)(r_u8((uint32)(0x801F2481))) - 1u), r_u8((uint32)(0x801F2481)));
      goto LABEL_70;

    case 2:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712495)), (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 3:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712461)), (sint32)a4, 0u, 0u, 0u);
      v35 = 0;
      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC((sint32)a0, 0x800922A4u, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v14 = (sint8)r_u8((uint32)(0x800922B0u));
      v15 = (sint32)a4;
      goto LABEL_67;

    case 4:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712304)), (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 5:
      gt_draft_unresolved_call(0x8003D9D8u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712432)), (sint32)(0u - (uint32)(2146712395)), (sint32)a4, 0u, 0u);
      v35 = 0;
      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC((sint32)a0, 0x800922ECu, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v14 = (sint8)r_u8((uint32)(0x800922F8u));
      v15 = (sint32)a4;
      goto LABEL_67;

    case 6:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712432)), (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 7:

    case 8:
      v18 = (sint32)(0u - (uint32)(2146712367));
      goto LABEL_69;

    case 9:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712344)), (sint32)a4, 0u, 0u, 0u);
      v35 = 0;
      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC((sint32)a0, 0x800922A4u, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v14 = (sint8)r_u8((uint32)(0x800922B0u));
      v15 = (sint32)a4;
      goto LABEL_67;

    case 0xA:
      v16 = (sint32)(0u - (uint32)(2146712200));
      v17 = (sint32)(0u - (uint32)(2146712169));
      goto LABEL_65;

    case 0xB:

    case 0xC:
      v18 = (sint32)(0u - (uint32)(2146712145));
      goto LABEL_69;

    case 0xD:

    case 0x1F:
      goto LABEL_62;

    case 0xE:

    case 0x20:

    case 0x2F:
      v16 = (sint32)(0u - (uint32)(2146712265));
      v17 = (sint32)(0u - (uint32)(2146712229));
      goto LABEL_65;

    case 0xF:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712027)), (sint32)a4, 0u, 0u, 0u);
      v20 = (sint32)a0;
      goto LABEL_66;

    case 0x10:

    case 0x11:

    case 0x12:
      if ((sint8)r_u8((uint32)(0x801F1FE4)))
      v18 = (sint32)(0u - (uint32)(2146711672));
    else
      v18 = (sint32)(0u - (uint32)(2146711750));
      goto LABEL_69;

    case 0x13:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146711727)), (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 0x14:
      v16 = (sint32)(0u - (uint32)(2146711906));
      v17 = (sint32)(0u - (uint32)(2146711775));
      goto LABEL_65;

    case 0x15:
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 1))
    {
      v16 = (sint32)(0u - (uint32)(2146711874));
    }
    else
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 2))
    {
      v16 = (sint32)(0u - (uint32)(2146711838));
    }
    else
    {
      v16 = (sint32)(0u - (uint32)(2146711803));
    }
      v17 = (sint32)(0u - (uint32)(2146711775));
      goto LABEL_65;

    case 0x16:
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 1))
    {
      v19 = (sint32)(0u - (uint32)(2146711997));
    }
    else
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 2))
    {
      v19 = (sint32)(0u - (uint32)(2146711967));
    }
    else
    {
      v19 = (sint32)(0u - (uint32)(2146711933));
    }
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, v19, (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 0x17:

    case 0x18:
      v18 = (sint32)(0u - (uint32)(2146711599));
      goto LABEL_69;

    case 0x19:
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 1))
    {
      v16 = (sint32)(0u - (uint32)(2146711576));
    }
    else
      if (((sint8)r_u8((uint32)(0x801F1FE4)) == 2))
    {
      v16 = (sint32)(0u - (uint32)(2146711549));
    }
    else
    {
      v16 = (sint32)(0u - (uint32)(2146711514));
    }
      v17 = (sint32)(0u - (uint32)(2146711486));
      goto LABEL_65;

    case 0x1A:
      v16 = (sint32)(0u - (uint32)(2146711438));
      v17 = (sint32)(0u - (uint32)(2146711376));
      goto LABEL_65;

    case 0x1B:
      v13 = (sint32)(0u - (uint32)(32));
      v45 = 160;
      v35 = (sint32)a4;
      gt_draft_80048134(r_u32((uint32)(0x801F1FDC)), (sint32)a0, (sint32)a1, (sint32)a2, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x1C:
      v13 = (sint32)(0u - (uint32)(32));
      v45 = 160;
      gt_draft_unresolved_call(0x800484C0u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)a3, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x1D:

    case 0x1E:

    case 0x21:

    case 0x22:
      v18 = (sint32)(0u - (uint32)(2146712145));
      goto LABEL_69;

    case 0x23:
      v13 = 28;
      v45 = 256;
      gt_draft_unresolved_call(0x80047B24u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)a3, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x24:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146711684)), (sint32)a4, 0u, 0u, 0u);
      goto LABEL_63;

    case 0x25:
      gt_draft_unresolved_call(0x800499C0u, (sint32)((uint32)((sint32)a0) + (uint32)(4)), (sint32)a1, (sint32)a2, (sint32)a3, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x26:
      v13 = (sint32)(0u - (uint32)(32));
      v45 = 160;
      v36 = 1;
      v35 = (sint8)r_u8((uint32)(0x801F2034));
      gt_draft_unresolved_call(0x8004965Cu, (sint32)((uint32)((sint32)a0) + (uint32)(4)), (sint32)a1, (sint32)a2, (sint32)a3, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x27:
      v13 = (sint32)(0u - (uint32)(32));
      v45 = 160;
      gt_draft_unresolved_call(0x8004A1C0u, (sint32)((uint32)((sint32)a0) + (uint32)(4)), (sint32)a1, (sint32)a2, (sint32)a3, 0u, 0u, 0u, 0u);
      goto LABEL_70;

    case 0x28:

    case 0x29:
      if (((sint8)r_u8((uint32)(0x801F2034)) == 1))
    {
      v18 = (sint32)(0u - (uint32)(2146711283));
      goto LABEL_69;
    }
      if (((sint8)r_u8((uint32)(0x801F2034)) == 2))
    {
      v18 = (sint32)(0u - (uint32)(2146711259));
      LABEL_69:
      gt_draft_unresolved_call(0x8003D9D8u, (sint32)a0, (sint32)a1, (sint32)a2, v18, (sint32)(0u - (uint32)(2146711640)), (sint32)a4, 0u, 0u);

      goto LABEL_70;
    }
      gt_draft_unresolved_call(0x8003D9D8u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712145)), (sint32)(0u - (uint32)(2146711640)), (sint32)a4, 0u, 0u);
      if (((sint16)r_u16((uint32)(0x801F2030)) >= 0))
    {
      v21 = gt_draft_unresolved_call(0x80035458u, ((sint32)((sint32)a4) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, v21, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C790u, 1, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C860u, 0, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8007C014u, gt_draft_buffer_address(v42, sizeof(v42)), 0x8008D194u, (sint32)((uint32)((sint16)r_u16((uint32)(0x801F2030))) + (uint32)(1)), 0u, 0u, 0u, 0u, 0u);
      v22 = gt_draft_8003A8F4(1, gt_draft_buffer_address(v42, sizeof(v42)), 0, 0u, 0u, 0u, 0u, 0u);
      gt_draft_8003A8AC((sint32)a0, (sint32)((uint32)((sint32)a1) - (uint32)((sint32)((uint32)(((sint32)(v22) >> ((uint32)(1) & 31u))) - (uint32)(160)))), (sint32)((uint32)((sint32)a2) + (uint32)(194)), gt_draft_buffer_address(v42, sizeof(v42)), 0u, 0u, 0u, 0u);
    }
      LABEL_70:
    if (v46)
    {
      v23 = (sint32)(0u - (uint32)(2146712639));
      if (!(sint8)r_u8((uint32)(0x801F2482)))
        v23 = (sint32)(0u - (uint32)(2146712660));
      gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v24 = gt_draft_unresolved_call(0x80035458u, ((sint32)((sint32)a4) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, v24, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v25 = gt_draft_unresolved_call(0x80035F7Cu, v23, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x80035E40u, (sint32)a0, (sint32)((uint32)((sint32)((uint32)((sint32)a1) + (uint32)(v45))) - (uint32)(((sint32)(v25) >> ((uint32)(1) & 31u)))), (sint32)((uint32)((sint32)a2) + (uint32)(v13)), v23, 0u, 0u, 0u, 0u);
    }

      if ((v14 >= 0))
    {
      gt_draft_unresolved_call(0x8006C790u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C860u, 2, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v26 = gt_draft_unresolved_call(0x80035458u, ((sint32)((sint32)a4) >> ((uint32)(1) & 31u)), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006C874u, v26, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v27 = (sint32)((uint32)(4) * (uint32)(v14));
      v28 = gt_draft_unresolved_call(0x80035F7Cu, r_u32((uint32)((uint32)(((uint32)(v27) + (uint32)(r_u32((uint32)(0x801F2484))))))), 0u, 0u, 0u, 0u, 0u, 0u, 0u);
      v29 = v28;
      v30 = (sint32)((uint32)((sint32)a1) - (uint32)((sint32)((uint32)(((sint32)(v28) >> ((uint32)(1) & 31u))) - (uint32)(160))));
      gt_draft_unresolved_call(0x80035E40u, (sint32)a0, v30, (sint32)((uint32)((sint32)a2) + (uint32)(171)), r_u32((uint32)((uint32)(((uint32)(v27) + (uint32)(r_u32((uint32)(0x801F2484))))))), 0u, 0u, 0u, 0u);
      v43 = (((uint32)(v43) & 0xFFFF0000u) | (((uint32)(v30) << 0u) & 0x0000FFFFu));
      v43 = (((uint32)(v43) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(175))) << 16u) & 0xFFFF0000u));
      v44 = (((uint32)(v44) & 0xFFFF0000u) | (((uint32)((sint32)((uint32)((sint32)((uint32)(v30) + (uint32)(v29))) - (uint32)(1))) << 0u) & 0x0000FFFFu));
      v44 = (((uint32)(v44) & 0x0000FFFFu) | (((uint32)((sint32)((uint32)((sint32)a2) + (uint32)(187))) << 16u) & 0xFFFF0000u));
      draft_buffer_v40[0] = v43;
      draft_buffer_v40[1] = v44;
      gt_draft_unresolved_call(0x8006F4F0u, (sint32)a0, gt_draft_buffer_address(&draft_buffer_v40[0], sizeof(draft_buffer_v40)), (v12 | 0x2000000), 0u, 0u, 0u, 0u, 0u);
      gt_draft_unresolved_call(0x8006F590u, (sint32)a0, 512, 0u, 0u, 0u, 0u, 0u, 0u);
    }
      gt_draft_unresolved_call(0x8003D8F4u, r_u32((uint32)(0x801F2488)), (sint32)((uint32)((sint32)a0) + (uint32)(16)), (sint32)a1, (sint32)a2, (sint32)a4,0u,0u,0u);
      v31 = 63;
      v32 = 12;
      v33 = 0;
      if (((sint8)r_u8((uint32)(0x801F1FE5)) == 1))
    {
      v31 = 0;
      v32 = 36;
      v33 = 95;
    }
      gt_draft_unresolved_call12(0x8003B784u, (uint32)((sint32)((uint32)((sint32)a0) + (uint32)(16000))), (sint32)a1, (sint32)a2, 320, 240, (sint32)a4, 0, 0, 0, v31, v32, v33);
      return 0;

    case 0x2A:

    case 0x2B:
      v18 = (sint32)(0u - (uint32)(2146711599));
      goto LABEL_69;

    case 0x2C:
      v16 = (sint32)(0u - (uint32)(2146711403));
      v17 = (sint32)(0u - (uint32)(2146711376));
      LABEL_65:
    gt_draft_unresolved_call(0x8003D9D8u, (sint32)a0, (sint32)a1, (sint32)a2, v16, v17, (sint32)a4, 0u, 0u);

      v20 = (sint32)a0;
      LABEL_66:
    v35 = 0;

      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC(v20, 0x800922BCu, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v15 = (sint32)a4;
      v14 = (sint8)r_u8((uint32)(0x800922C8u));
      LABEL_67:
    v12 = (uint8)(gt_draft_unresolved_call(0x80035608u, 0, v15, 0u, 0u, 0u, 0u, 0u, 0u));

      goto LABEL_70;

    case 0x2D:

    case 0x2E:
      v18 = (sint32)(0u - (uint32)(2146711319));
      goto LABEL_69;

    case 0x30:
      if (((sint8)r_u8((uint32)(0x801F2034)) == 1))
    {
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712069)), (sint32)a4, 0u, 0u, 0u);
    }
    else
      if (((sint8)r_u8((uint32)(0x801F2034)) == 2))
    {
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712094)), (sint32)a4, 0u, 0u, 0u);
    }
    else
    {
      LABEL_62:
      gt_draft_unresolved_call(0x8003D968u, (sint32)a0, (sint32)a1, (sint32)a2, (sint32)(0u - (uint32)(2146712118)), (sint32)a4, 0u, 0u, 0u);

    }
      LABEL_63:
    v35 = 1;

      v36 = 0;
      v37 = 0;
      gt_draft_80046BBC((sint32)a0, 0x800922A4u, (sint32)((uint32)((sint32)a1) + (uint32)(160)), (sint32)((uint32)((sint32)a2) + (uint32)(190)), 0u, 0u, 0u, 0u);
      v14 = (sint8)r_u8((uint32)(0x800922B0u));
      v12 = ((uint32)((uint8)(gt_draft_unresolved_call(0x80035608u, 0, (sint32)a4, 0u, 0u, 0u, 0u, 0u, 0u))) << ((uint32)(16) & 31u));
      goto LABEL_70;

    case 0x31:

    case 0x32:
      v18 = (sint32)(0u - (uint32)(2146711750));
      goto LABEL_69;

    default:
      goto LABEL_70;

  }

}

