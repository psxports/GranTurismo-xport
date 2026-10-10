#include "game_draft_registry.h"
#include "game_first2_registry.h"

uint32 gt_draft_800380B4(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    /* Legacy a4 represents the reviewed incoming countdown */
    return gtmain_menu_countdown_toggle_tail((sint32)a4);
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
    return gtmain_camera_projection_set(a0, (sint32)a1);
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
