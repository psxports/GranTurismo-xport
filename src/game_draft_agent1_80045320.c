#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
/* Original: GTMAIN.EXE:sub_80045320. */ uint32 gt_draft_80045320(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{

  return (sint32)r_u32(0x80092D14u + 4u * a0);
}
