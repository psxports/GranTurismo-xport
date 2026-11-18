#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
/* Original: GTMAIN.EXE:sub_800406F4. */ uint32 gt_draft_800406F4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
    w_u32(0x801F1FDCu, -2145570852);
    w_u8(0x801F1FE4u, 1);
    w_u8(0x801F1FE5u, 1);
    w_u32(0x801F2488u, a1);
    w_u32(0x801F1FE8u, 0x8008D3A0u);
    w_u32(0x801F1FECu, r_u32(0x8008D3B0u));
    w_u32(0x801F1FF0u, r_u32(0x8008D3B4u));
    w_u32(0x801F1FF4u, r_u32(0x8008D3B8u));
    w_u32(0x801F1FF8u, r_u32(0x8008D3BCu));
    w_u32(0x801F1FFCu, r_u32(0x8008D3C0u));
    w_u32(0x801F2000u, r_u32(0x8008D3C4u));
    w_u32(0x801F2004u, r_u32(0x8008D3C8u));
    w_u8(0x801F2008u, r_u8(0x8008D3CCu));
  result = 3;
    w_u32(0x800C0A80u, a0);
    w_u32(0x801F1FE0u, 0);
    w_u8(0x801F2034u, 3);
    w_u16(0x801F202Eu, 0);
  return result;
}
