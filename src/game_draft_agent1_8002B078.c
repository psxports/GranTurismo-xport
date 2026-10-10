#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002B078(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 result = 0;
  result = r_u32(0x800B630Cu);
  if ( !r_u32(0x800B630Cu) )
  {
    w_u32(0x800CB8E0u, r_u32(0x800B6870u));
    gtmain_memory_copy(-2146649884, -2146706576, 400);
    result = r_u32(0x800B7028u);
    w_u32(0x800B6348u, r_u32(0x800B7028u));
  }
  return result;
}
