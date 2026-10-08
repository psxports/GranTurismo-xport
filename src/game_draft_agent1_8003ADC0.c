#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8003ADC0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint16 v14 = 0;
    sint32 v15 = 0;
    sint32 v16 = 0;
    sint16 v17 = 0;
    sint32 result = 0;
  v10 = r_u32(a1 + 4u);
  v11 = a5 * (uint16)(sint16)r_u16(a1 + 2u * (4));
  v12 = (uint16)(sint16)r_u16(a1 + 2u * (5));
  v13 = r_u32(0x800BFE4Cu);
  w_u32(r_u32(0x800BFE4Cu), 150994944u);
    w_u32(v13 + 4, a4 | (a4 << 8) | (a4 << 16) | 0x2C000000);
    w_u16(v13 + 8, a2);
    w_u16(v13 + 10, a3);
    w_u16(v13 + 18, a3);
    w_u16(v13 + 24, a2);
    w_u32(v13 + 12, v10);
  v14 = a2 + (v11 >> 7);
    w_u16(v13 + 16, v14);
    w_u16(v13 + 32, v14);
  v15 = (uint8)v10;
  v16 = (((uint32)(v10) >> 8) & 0xFFu);
  v17 = a3 + ((a5 * v12) >> 7);
    w_u16(v13 + 26, v17);
    w_u16(v13 + 34, v17);
    w_u32(v13 + 20, (uint8)(v15 + (sint16)r_u16(a1 + 2u * (4)) - 1) | (v16 << 8) | ((sint16)r_u16(a1) << 16));
    w_u32(v13 + 28, v15 | ((uint8)(v16 + (sint16)r_u16(a1 + 2u * (5)) - 1) << 8));
  result = a1 + 2;
    w_u32(v13 + 36, (uint8)(v15 + (sint16)r_u16(a1 + 2u * (4)) - 1) | ((uint8)(v16 + (sint16)r_u16(a1 + 2u * (5)) - 1) << 8));
    result = (result & 0x00FFFFFFu) | (((uint32)(r_u8(a1 + 2)) & 0xFFu) << 24);
    w_u16(a1, v13);
    w_u8(a1 + 2, (uint32)(v13 << 8) >> 24);
    w_u8(v13, (uint16)(a1 + 2) >> 8);
    w_u8(v13 + 1, (uint32)(a1 + 2) >> 16);
    w_u8(v13 + 2, (((uint32)(result) >> 24) & 0xFFu));
    w_u32(0x800BFE4Cu, v13 + 40);
  return result;
}
