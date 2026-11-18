#include "game_local_stack.h"

static uint32 menu_local_stack_top;

GT_LOCAL_STACK_FRAME gtmenu_local_stack_push(uint32 bytes, uint32 owner)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 launch, alternate, baseline, reserve, low, top, heap, heap_bytes;
    launch = r_u32(0x800566AC);
    alternate = r_u32(0x8007B624);
    reserve = r_u32(0x8007B634);
    baseline = alternate ? alternate : launch;
    heap = r_u32(0x8007B62C);
    heap_bytes = r_u32(0x8007B628);
    if (launch < 0x800ACA00 || launch > 0x80200000 ||
        reserve > launch - 0x800ACA00 || bytes > 0x200000 - 7)
        gt_draft_failure("GTMENU local stack reservation", owner);
    low = (launch - reserve) & ~15;
    if (low < 0x800ACA00 || baseline < low || baseline > launch || (baseline & 7) ||
        heap < 0x800ACA00 || heap > low || heap_bytes > low - heap)
        gt_draft_failure("GTMENU local stack memory layout", owner);
    top = menu_local_stack_top ? menu_local_stack_top : baseline;
    bytes = (bytes + 7) & ~7;
    if (top < low || top > baseline || bytes > top - low)
        gt_draft_failure("GTMENU local stack capacity", owner);
    frame.previous = menu_local_stack_top;
    frame.address = top - bytes;
    if (frame.address < heap + heap_bytes)
        gt_draft_failure("GTMENU local stack heap overlap", owner);
    menu_local_stack_top = frame.address;
    return frame;
}

void gtmenu_local_stack_pop(GT_LOCAL_STACK_FRAME frame)
{
    if (menu_local_stack_top != frame.address)
        gt_draft_failure("GTMENU local stack allocation order", frame.address);
    menu_local_stack_top = frame.previous;
}
