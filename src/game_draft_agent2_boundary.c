#include "game_draft_registry.h"
/* Unverified draft translations with MIPS recovered stack arguments */

/* Original: GTMAIN.EXE:sub_800407A8. */ uint32 gt_draft_800407A8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9)
{
    w_u32(0x801F1FDCu, 0x801D2FDCu);
    w_u8(0x801F1FE4u, 1); w_u8(0x801F1FE5u, 0);
    w_u32(0x801F1FE8u, 0x8008D3A0u); w_u32(0x801F2488u, a9);
    for (uint32 i = 0; i < 6; i += 2) {
        uint32 first = r_u32(0x8008D3B0u + 4u * i), second = r_u32(0x8008D3B0u + 4u * i + 4u);
        w_u32(0x801F1FECu + 4u * i, first); w_u32(0x801F1FECu + 4u * i + 4u, second);
    }
    w_u32(0x801F2004u, r_u32(0x8008D3C8u));
    w_u8(0x801F2008u, r_u8(0x8008D3CCu));
    w_u32(0x800C0A80u, a8); w_u32(0x801F1FE0u, 1);
    w_u16(0x801F202Eu, a0); w_u8(0x801F2034u, a1);
    w_u8(0x801F2035u, a5); w_u8(0x801F2036u, a6);
    w_u16(0x801F2038u, a2); w_u8(0x801F2037u, a3);
    w_u8(0x801F2040u, a4); w_u32(0x801F203Cu, a7);
    return 1;
}

uint32 gt_draft_8001F0DC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5)
{
    sint8 used_types[8]; uint16 choices[8];
    sint32 wanted = (sint32)a2, limit = wanted < 6 ? wanted : 5;
    sint32 excluded_type = (sint8)r_u8(gt_draft_unresolved_call(0x8006B668u, a4, 0, 0, 0, 0, 0, 0, 0));
    uint32 pool = a1, count = gt_draft_8001F080(pool, 0, 0, 0, 0, 0, 0, 0);
    sint32 selected = 0;
    while (selected < limit) {
        if (a5) { pool = r_u32(a5 + 4u * (uint32)selected); count = gt_draft_8001F080(pool, 0, 0, 0, 0, 0, 0, 0); }
        gt_draft_unresolved_call(0x8006C31Cu, a3, 0, 0, 0, 0, 0, 0, 0);
        uint32 candidate = 0, index = 0; sint32 kind = 0;
        for (uint32 attempts = 0; attempts < 256; ++attempts) {
            /* TODO Bind modulo by zero exception to the missing adapter */
            if (!count) gt_draft_exception(7);
            index = gt_draft_unresolved_call(0x8006C31Cu, a3, 0, 0, 0, 0, 0, 0, 0) % count;
            uint32 entry = r_u32(pool + index * 4u);
            kind = (sint8)r_u8(entry);
            sint32 parsed = (sint32)gt_draft_unresolved_call(0x8006B828u, entry, 0, 0, 0, 0, 0, 0, 0);
            candidate = parsed < 0 ? 0 : (uint32)parsed;
            if (parsed < 0 || kind == excluded_type || candidate == a4) continue;
            sint32 duplicate = 0;
            while (duplicate < selected) {
                if (used_types[duplicate] == kind || r_u32(a0 + 4u * (uint32)duplicate) == candidate) break;
                ++duplicate;
            }
            if (duplicate == selected) break;
        }
        choices[selected] = (uint16)index; used_types[selected] = (sint8)kind;
        w_u32(a0 + 4u * (uint32)selected, candidate); ++selected;
    }
    while (selected < wanted) {
        uint32 choice = gt_draft_unresolved_call(0x8006C31Cu, a3, 0, 0, 0, 0, 0, 0, 0) % 5u;
        w_u32(a0 + 4u * (uint32)selected, r_u32(a0 + 4u * choice)); ++selected;
    }
    uint32 result = (uint32)(limit > 0);
    for (sint32 i = 0; i < limit; ++i) {
        sint32 key = (sint16)choices[i]; uint32 value = r_u32(a0 + 4u * (uint32)i);
        for (sint32 j = i; j < limit; ++j) {
            sint32 other = (sint16)choices[j];
            if (other < key) {
                choices[j] = (uint16)key; key = other;
                uint32 temporary = r_u32(a0 + 4u * (uint32)j);
                w_u32(a0 + 4u * (uint32)j, value); value = temporary;
            }
        }
        choices[i] = (uint16)key; w_u32(a0 + 4u * (uint32)i, value);
        /* TODO Original result points into a caller-visible expired stack temporary */
        result = gt_draft_buffer_address(used_types, sizeof(used_types)) + 2u * (uint32)i;
    }
    return result;
}
