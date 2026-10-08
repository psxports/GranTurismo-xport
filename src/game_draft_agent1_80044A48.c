#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80044A48(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v12 = 0;
    sint32 v13 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
  uint32 vars0=0; /* TODO Pseudocode phantom stack local */ // [sp+18h] [+0h]
  uint32 vars4=0; /* TODO Pseudocode phantom stack local */ // [sp+1Ch] [+4h]
  uint32 vars8=0; /* TODO Pseudocode phantom stack local */ // [sp+20h] [+8h]
  uint32 varsC=0; /* TODO Pseudocode phantom stack local */ // [sp+24h] [+Ch]
  v12 = 0;
  v13 = 48;
  do
  {
    v14 = v12;
    if ( (sint8)r_u8(0x800BEA24u) )
    {
      v14 = 0;
      v15 = -2146736528;
      do
      {
        if ( r_u8(v15 + 3) == v12 )
          break;
        ++v14;
        v15 += 76;
      }
      while ( v14 < 6 );
    }
    gt_draft_800446DC(a0, a1 + 32, a2 + v13, a4 >> 1, v14, v12++, 0, 0 /* TODO Retained callee carriers */);
    v13 += 26;
  }
  while ( v12 < 6 );
  return 0;
}
