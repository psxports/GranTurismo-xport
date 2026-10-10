#include "game_first2_registry.h"
#include <stdio.h>

static void vblank_timeout_counter(uint32 value)
{
    /* TODO Bind original timeout service 8008B2C8 */
    gt_draft_failure("Native VBlank timeout counter adapter", 0x8008B2C8u);
}
static uint32 vblank_timeout_mode(uint32 mode, uint32 value)
{
    /* TODO Bind original timeout service with its actual result */
    (void)mode; (void)value;
    gt_draft_failure("Native VBlank timeout mode adapter", 0x8008B2B8u);
}
uint32 gtmain_vblank_wait(sint32 target, uint32 budget)
{
    uint32 remaining = budget << 15;
    uint32 result = (sint32)r_u32(0x800B38A4u) < target;
    if (result) {
        for (;;) {
            --remaining;
            if (remaining == 0xFFFFFFFFu) {
                puts("VSync: timeout\n");
                vblank_timeout_counter(0);
                return vblank_timeout_mode(3, 0);
            }
            result = (sint32)r_u32(0x800B38A4u) < target;
            if (!result) return result;
        }
    }
    return result;
}
