#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003DC70(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    w_u8(0x801F2480u, a0);
  switch ( a0 )
  {
    case 0:
      gt_draft_80046AC4(0x8009228Cu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x8009229Cu);
    w_u32(0x801F2484u, r_u32(0x8009229Cu));
      return result;
    case 1:
      result = 8;
    w_u32(0x801F2481u, 8);
    w_u32(0x801F2483u, 0);
      return result;
    case 2:
    case 6:
      gt_draft_80046AC4(0x800922A4u, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922B4u);
    w_u32(0x801F2484u, r_u32(0x800922B4u));
      return result;
    case 3:
    case 9:
    case 22:
      goto LABEL_5;
    case 4:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_80046AC4(0x800922A4u, 0, 0, 0, 0, 0, 0, 0);
    w_u32(0x801F2484u, r_u32(0x800922B4u));
LABEL_5:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_80046AC4(0x800922A4u, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922B4u);
    w_u32(0x801F2484u, r_u32(0x800922B4u));
      return result;
    case 5:
      gt_draft_80046AD4(0x800922ECu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922FCu);
    w_u32(0x801F2484u, r_u32(0x800922FCu));
      return result;
    case 10:
      goto LABEL_22;
    case 13:
    case 19:
    case 31:
    case 48:
    w_u8(0x800B5300u, 1);
      gt_draft_80046AD4(0x800922A4u, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922D4u);
    w_u32(0x801F2484u, r_u32(0x800922D4u));
      return result;
    case 14:
    case 32:
    case 47:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_80046AC4(0x800922BCu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922DCu);
    w_u32(0x801F2484u, r_u32(0x800922DCu));
      return result;
    case 15:
      gt_draft_80046AD4(0x800922BCu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922E4u);
    w_u32(0x801F2484u, r_u32(0x800922E4u));
      return result;
    case 20:
    case 21:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_80046AC4(0x800922BCu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x80092304u);
    w_u32(0x801F2484u, r_u32(0x80092304u));
      return result;
    case 25:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
      gt_draft_80046AC4(0x800922BCu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x8009230Cu);
    w_u32(0x801F2484u, r_u32(0x8009230Cu));
      return result;
    case 26:
    case 44:
      gt_draft_unresolved_call(0x80035778u, 6, 0, 0, 0, 0, 0, 0, 0);
LABEL_22:
      gt_draft_80046AD4(0x800922BCu, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922CCu);
    w_u32(0x801F2484u, r_u32(0x800922CCu));
      break;
    case 27:
      result = gt_draft_unresolved_call(0x80047CA0u, r_u32(0x801F1FDCu), 0, 0, 0, 0, 0, 0, 0);
      break;
    case 28:
      result = gt_draft_80048448(r_u32(0x801F1FDCu), r_u32(0x801F202Eu), 0, 0, 0, 0, 0, 0);
      break;
    case 35:
      result = gt_draft_unresolved_call(0x800477E0u, r_u32(0x801F1FDCu), 0, 0, 0, 0, 0, 0, 0);
      break;
    case 36:
      gt_draft_80046AD4(0x800922A4u, 0, 0, 0, 0, 0, 0, 0);
      result = (sint32)r_u32(0x800922D4u);
    w_u32(0x801F2484u, r_u32(0x800922D4u));
      break;
    case 37:
    w_u32(0x801F202Cu, 1);
      result = gt_draft_unresolved_call(0x80049898u, r_u32(0x801F1FDCu), r_u32(0x801F229Eu), 0, 0, 0, 0, 0, 0);
      break;
    case 38:
      result = gt_draft_unresolved_call(0x800492ECu, r_u32(0x801F1FDCu), r_u32(0x801F202Eu), r_u32(0x801F2034u), r_u32(0x801F2035u), 0, 0, 0, 0);
      break;
    case 39:
      result = gt_draft_8004A148(0, 0, 0, 0, 0, 0, 0, 0);
      break;
    case 40:
    case 41:
    case 42:
    case 43:
    case 45:
    case 46:
      result = -1;
    w_u32(0x801F2030u, -1);
      break;
    default:
      return result;
  }
  return result;
}
