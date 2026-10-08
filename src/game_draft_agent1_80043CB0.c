#include "game_draft_registry.h"

/* Unverified decimal formatting draft derived from the existing full pseudocode */
uint32 gt_draft_80043CB0(uint32 a0,uint32 a1,uint32 a2,uint32 a3,uint32 a4,uint32 a5,uint32 a6,uint32 a7)
{
    char buffer[40]={0};
    sint32 cursor=24,count=0,precision=(sint32)a4;
    sint32 value=(sint32)a3,quotient;
    uint32 guest_text,table_index=0,x,color,result;
    while(precision<0) {
        value=(sint32)((uint32)value*10u);
        ++precision;
    }
    quotient=value/10;
    do {
        char digit=(char)((uint32)value+48u);
        value/=10;
        --precision;
        buffer[cursor]=(char)(digit-10*quotient);
        quotient=value/10;
        if(!precision) {--cursor;++count;buffer[cursor]='.';}
        if(!value)break;
        ++count;--cursor;
    } while(count<16);
    while(precision>0) {
        ++count;--precision;--cursor;buffer[cursor]='0';
        if(!precision) {--cursor;++count;buffer[cursor]='.';}
    }
    if(!precision) {++count;buffer[24-count]='0';}
    guest_text=gt_draft_buffer_address(&buffer[24-count],(uint32)(count+2));
    x=a1;
    if(a7!=1) x-=gt_draft_unresolved_call(0x80035FC0u,a7,guest_text,0,0,0,0,0,0);
    /* TODO Callee inferred extra stack arguments are absent from this callsite */
    gt_draft_unresolved_call9(0x80036068u,a0,x,a2,guest_text,a7,0,0,0,0);
    gt_draft_buffer_release(guest_text);
    switch(a5) {
    case 1:x=a1-4u;table_index=12;break;
    case 2:x=a1-6u;table_index=13;break;
    case 3:x=a1-6u;table_index=14;break;
    case 4:x=a1-4u;table_index=15;break;
    case 5:x=a1-6u;table_index=16;break;
    default:break;
    }
    if(table_index) {
        color=(a6&255u);color=color|(color<<8)|(color<<16)|0x01000000u;
        gt_draft_unresolved_call(0x800706BCu,a0,(x&65535u)|(a2<<16),r_u32(0x80092334u+16u*table_index+12u),r_u32(0x80092334u+16u*table_index+8u),color,0,0,0);
    }
    result=gt_draft_unresolved_call(0x8006F590u,a0,552,0,0,0,0,0,0);
    return result;
}
