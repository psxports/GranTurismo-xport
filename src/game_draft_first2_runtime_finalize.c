#include "game_first2_registry.h"

static uint32 runtime_destructor(uint32 token)
{
    /* TODO Dispatch the real native destructor and preserve its result */
    gt_draft_failure("Native runtime destructor token adapter", token);
}
uint32 gtmain_runtime_destructors(void)
{
    uint32 count = r_u32(0x801FB710u), result, cursor;
    if (count == 0xFFFFFFFFu) {
        count = 0;
        if (r_u32(0x801FB714u)) {
            cursor = 0x801FB714u;
            do { cursor += 4u; ++count; } while (r_u32(cursor));
        }
    }
    result = count * 4u;
    if (count) {
        cursor = 0x801FB710u + result;
        do {
            uint32 token = r_u32(cursor); cursor -= 4u; --count;
            result = runtime_destructor(token);
        } while (count);
    }
    return result;
}
uint32 gtmain_runtime_finalize(void)
{
    uint32 result = r_u32(0x800B5F3Cu);
    if (!result) { w_u32(0x800B5F3Cu, 1); return gtmain_runtime_destructors(); }
    return result;
}
