#include "game_draft_registry.h"

/* GTMAIN.EXE 8002CD40 */
/* Original: GTMAIN.EXE:sub_8002CD40. */ uint32 gt_draft_8002CD40(void)
{
    uint8 count;
    if (r_u32(0x800B630Cu) == 1u) {
        w_u8(0x8008FBEFu, 0u);
        w_u8(0x8008FBF0u, 0u);
        return 1u;
    }
    if (gt_draft_8002CD1C(0u)) w_u8(0x8008FBEFu, 0u);
    else {
        count = (uint8)(r_u8(0x8008FBEFu) + 1u);
        w_u8(0x8008FBEFu, count);
        if ((sint8)count >= 3) {
            w_u8(0x8008FBEFu, 3u);
            w_u8(0x800B63D8u, 1u);
            return 0u;
        }
    }
    if (r_u8(0x800CB7F5u) != 2u) return 1u;
    if (gt_draft_8002CD1C(1u)) {
        w_u8(0x8008FBF0u, 0u);
        return 1u;
    }
    count = (uint8)(r_u8(0x8008FBF0u) + 1u);
    w_u8(0x8008FBF0u, count);
    if ((sint8)count < 3) return 1u;
    w_u8(0x8008FBF0u, 3u);
    w_u8(0x800B63D8u, 1u);
    return 0u;
}
