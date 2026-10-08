#include "game_draft_registry.h"

static sint32 gt_agent1_ratio(sint32 numerator,sint32 denominator)
{
    if(!denominator) {
        gt_draft_exception(7);
        /* TODO Exception continuation policy */
        return numerator<0 ? 1 : -1;
    }
    if(numerator==(sint32)0x80000000u && denominator==-1) {
        gt_draft_exception(6);
        /* TODO Exception continuation policy */
        return numerator;
    }
    return numerator/denominator;
}

uint32 gt_draft_800409A4(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    uint32 rect[2],guest_rect,value,offset=0,color,x=(uint32)(sint32)(sint16)a3;
    sint32 ratio,y=(sint16)a4,height=30;
    value=a1;
    if((sint32)a1<0) {value=a2;offset=24;}
    color=gt_draft_unresolved_call(0x80035398u,6,a5,0,0,0,0,0,0);
    rect[0]=((x+offset)&65535u)|(((uint32)(y+30)&65535u)<<16);
    rect[1]=((x+243u)&65535u)|(((uint32)(y+30)&65535u)<<16);
    guest_rect=gt_draft_buffer_address(rect,sizeof(rect));
    gt_draft_unresolved_call(0x8006F4F0u,a0,guest_rect,color,0,0,0,0,0);
    gt_draft_buffer_release(guest_rect);
    ratio=gt_agent1_ratio((sint32)(255u*value),(sint32)a2);
    rect[0]=((x+31u)&65535u)|(((uint32)(y+28)&65535u)<<16);
    rect[1]=((x+248u)&65535u)|(((uint32)(y+28)&65535u)<<16);
    color=(uint8)((sint32)((255u-(uint32)ratio)*a5)>>7)|(16u*a5&0xFF00u)|((uint32)(uint8)((sint32)((uint32)ratio*a5)>>7)<<16);
    guest_rect=gt_draft_buffer_address(rect,sizeof(rect));
    gt_draft_unresolved_call(0x8006F4F0u,a0,guest_rect,color,0,0,0,0,0);
    gt_draft_buffer_release(guest_rect);
    if(!a6) {y=(sint16)(a4+6u);height=24;}
    rect[0]=((x+offset)&65535u)|(((uint32)y&65535u)<<16);
    rect[1]=((x+247u)&65535u)|(((uint32)(y+height-1)&65535u)<<16);
    color=(uint8)((sint32)a5>>3)|((a5<<6)&0xFF00u)|((uint32)(uint8)((sint32)a5>>1)<<16)|0x02000000u;
    guest_rect=gt_draft_buffer_address(rect,sizeof(rect));
    gt_draft_unresolved_call(0x8006F4F0u,a0,guest_rect,color,0,0,0,0,0);
    gt_draft_buffer_release(guest_rect);
    /* TODO Retained argument carriers of the external adapters */
    return gt_draft_unresolved_call(0x8006F590u,a0,544,0,0,0,0,0,0);
}
