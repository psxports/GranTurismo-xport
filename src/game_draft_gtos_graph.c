#include "game_draft_registry.h"
#include <stdio.h>
#include <string.h>

uint32 gtos_graph_reset(uint32 mode)
{
    uint32 selected = mode & 7u, type;
    if (selected == 0u || selected == 3u || selected == 5u) {
        if (selected != 5u)
            printf("GTOS ResetGraph:jtb=%08X,env=%08X\n", 0x8007423Cu, 0x80074284u);
        memset(psx_addr(0x80074284u, 128u), 0, 128u);
        ResetCallback();
        if (ResetGraph(0) < 0)
            return gt_draft_unresolved_call(0x80046210u, mode, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        /* Modes three and five return zero without the original hardware probe */
        type = selected == 0u ? (uint32)GetGraphType() : 0u;
        w_u8(0x80074284u, type);
        w_u8(0x80074285u, 1u);
        type = r_u8(0x80074284u);
        w_u16(0x80074288u, r_u16(0x80074304u + type * 4u));
        w_u16(0x8007428Au, r_u16(0x80074310u + type * 4u));
        memset(psx_addr(0x80074294u, 92u), 0xFF, 92u);
        memset(psx_addr(0x800742F0u, 20u), 0xFF, 20u);
        return type;
    }
    if (r_u8(0x80074286u) >= 2u) {
        uint32 callback = r_u32(0x80074280u);
        if (callback == 0x8004B270u) printf("GTOS ResetGraph(%d)...\n", (sint32)mode);
        else gt_draft_unresolved_call(callback, 0x8004D668u, mode, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    w_u32(0x80074380u, 0u);
    w_u32(0x8007437Cu, 0u);
    w_u32(0x1F801814u, 0x02000000u);
    w_u32(0x1F801814u, 0x01000000u);
    return 0u;
}
