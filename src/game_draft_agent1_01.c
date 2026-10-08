#include "game_draft_registry.h"

uint32 gt_draft_800380B4(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    /* TODO Incoming v0 is a nonstandard carrier supplied through a4 */
    uint32 countdown = a4;
    uint32 toggle;
    if ((sint32)countdown > 0) w_u16(0x801F300Au, countdown - 1u);
    toggle = (r_u8(0x801F3008u) + 1u) & 1u;
    w_u8(0x801F3008u, toggle);
    return toggle;
}

uint32 gt_draft_8001A068(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    /* TODO Arguments absent from the pseudocode remain unresolved */
    gt_draft_80019F10(a0,a1,a2,a3,a4,a5,a6,a7);
    gtmain_mode_init();
    return gt_draft_8001A098(0,0,0,0,0,0,0,0);
}

uint32 gt_draft_8001DF30(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    uint32 result;
    w_u16(a0 + 12u,a1);
    /* TODO External adapter may consume further retained arguments */
    gt_draft_unresolved_call(0x8006B3E0u,a0,a1,a2,a3,0,0,0,0);
    gt_draft_gte_write(24,1,r_u16(a0+8u)<<16);
    gt_draft_gte_write(25,1,r_u16(a0+10u)<<16);
    result=r_u16(a0+12u);
    gt_draft_gte_write(26,1,result);
    return result;
}

uint32 gt_draft_8001E218(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    uint32 mode=1,result,height=240,half_height=104;
    if ((sint16)r_u16(0x800B6506u)==1 && (sint16)r_u16(0x800B661Au)==1 && r_u16(0x800B6508u) && r_u16(0x800B661Cu)) mode=2;
    result=(uint32)(sint32)(sint8)r_u8(0x800B633Cu);
    w_u8(0x800B633Cu,mode);
    if(mode!=result) {
        if(mode==1) {height=120;half_height=52;}
        w_u16(0x800B6416u,height);w_u16(0x800B641Au,half_height);
        w_u16(0x800B652Au,height);w_u16(0x800B652Eu,half_height);
        /* TODO External adapter arguments beyond a0 are unclassified */
        gt_draft_unresolved_call(0x8006B3E0u,0x800B6410u,height,0x800B6410u,half_height,0,0,0,0);
        result=gt_draft_unresolved_call(0x8006B3E0u,0x800B6524u,0,0,0,0,0,0,0);
    }
    return result;
}
