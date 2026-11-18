#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
/* Original: GTMAIN.EXE:sub_800245A0. */ uint32 gt_draft_800245A0(uint32 model)
{
    uint32 cursor = model + 68u;
    uint32 count = r_u32(model + 44u), index, first, second;
    w_u32(model, cursor);
    cursor += count * 8u;
    for (index = 0u; index < 8u; ++index) {
        w_u32(model + 4u + index * 4u, cursor);
        count = r_u16(model + 48u + index * 2u);
        cursor += count * r_u8(0x8008F720u + index);
    }
    first = r_u16(model + 64u);
    second = r_u16(model + 66u);
    w_u32(model + 36u, cursor);
    cursor += first * 16u;
    w_u32(model + 40u, cursor);
    return cursor + second * 20u;
}
