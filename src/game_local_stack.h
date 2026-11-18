#ifndef GT_GAME_LOCAL_STACK_H
#define GT_GAME_LOCAL_STACK_H

#include "game_first2_registry.h"

typedef struct {
    uint32 address;
    uint32 previous;
} GT_LOCAL_STACK_FRAME;

uint32 gt_local_stack_top(void);
GT_LOCAL_STACK_FRAME gt_local_stack_push(uint32 bytes, uint32 owner);
GT_LOCAL_STACK_FRAME gtmain_cd_decode_stack_push(void);
void gt_local_stack_pop(GT_LOCAL_STACK_FRAME frame);
GT_LOCAL_STACK_FRAME gtmenu_local_stack_push(uint32 bytes, uint32 owner);
void gtmenu_local_stack_pop(GT_LOCAL_STACK_FRAME frame);

static uint32 gt_local_stack_workspace(uint32 offset, uint32 size, uint32 owner)
{
    uint32 launch = r_u32(0x8008F1D8u), alternate = r_u32(0x800B1AA4u);
    uint32 baseline = alternate ? alternate : launch;
    uint32 top = gt_local_stack_top();
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 heap = r_u32(0x800B1AACu), bytes = r_u32(0x800B1AA8u);
    uint32 extent = bytes < 16u ? 16u : bytes, address;
    if (top < 0x80010000u + offset || top > 0x80200000u || (top & 7u) || size > offset)
        gt_draft_failure("GTMAIN local stack range", owner);
    address = top - offset;
    /* Keep nested temporaries above the resident session context */
    if (top != baseline && address < baseline - 0x41D8u)
        gt_draft_failure("GTMAIN nested local stack overlap", owner);
    if (address < 0x801FB724u || heap < 0x80010000u || heap > address || extent > address - heap ||
        (!alternate && (reserve < offset || address < launch - reserve)))
        gt_draft_failure("GTMAIN local stack overlap", owner);
    return address;
}

#endif
