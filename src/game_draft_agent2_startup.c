#include "game_draft_registry.h"
/* Unverified draft adapted from the existing startup translation */
uint32 gt_draft_80010000(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    /* TODO Missing loader context adapter preserves native return boundary */
    gt_draft_loader_context_capture(0x8008F1D8u);
    /* TODO Honor optional alternate loader stack at 0x800B1AA4 without emulating a guest frame */
    uint32 stack_override = r_u32(0x800B1AA4u);
    (void)stack_override;
    w_u32(0x8008F204u, 0x8008B848u - 0x80010000u);
    w_u32(0x8008F20Cu, 0x800B38F4u - 0x8008F1E0u);
    w_u32(0x8008F214u, 0x801FB724u - 0x800B38F4u);
    w_u32(0x8008F218u, a0);
    w_u32(0x8008F21Cu, a1);
    uint32 clear = 0x801FB724u;
    if (clear >= 0x800B38F4u) {
        do { clear -= 4u; w_u32(clear, 0); } while (clear >= 0x800B38F4u);
    }
    gt_draft_unresolved_call(0x8007BC84u, 0, 0, 0, 0, 0, 0, 0, 0);
    uint32 result = gt_draft_unresolved_call(0x80019EC0u, a0, a1, 0, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x80082928u, result, 0, 0, 0, 0, 0, 0, 0);
}
