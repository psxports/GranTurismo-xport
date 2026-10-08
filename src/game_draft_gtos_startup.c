#include "game_draft_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32 gtos_native_main(void)
{
    uint32 count, cursor;
    if (!r_u32(0x80076CCCu)) {
        w_u32(0x80076CCCu, 1u);
        count = r_u32(0x801B16B0u);
        cursor = 0x801B16B0u;
        if (count == 0xFFFFFFFFu) {
            count = 0u;
            if (r_u32(cursor + 4u)) {
                cursor += 4u;
                do { cursor += 4u; ++count; } while (r_u32(cursor));
            }
        }
        cursor = 0x801B16B0u + count * 4u;
        while (count) {
            uint32 callback = r_u32(cursor);
            cursor -= 4u;
            --count;
            /* TODO Dispatch any nonempty GTOS constructor list */
            gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
    }
    {
        uint32 start = r_u32(0x80073ABCu), size = r_u32(0x80073AB8u);
        printf("GTOS heap: area = %08X, size = %08X, end = %08X\n", start, size, start + size);
    }
    ResetCallback();
    if (!CdInit())
        return gt_draft_unresolved_call(0x80042B90u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gtos_sound_init();
    /* Guest BIOS card driver setup is skipped for the native build */
    memset(psx_addr(0x8007AA30u, 96u), 0, 96u);
    if (!InitPAD(0x8007AA90u, 64u, 0x8007AAD0u, 64u) || !StartPAD())
        return gt_draft_unresolved_call(0x8003A6ACu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    /* TODO Adapt SDK port state processing when GTOS begins polling controls */
    gtos_graph_reset(3u);
    return gt_draft_unresolved_call(0x80042500u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

uint32 gtos_native_transition(uint32 executable)
{
    uint32 bound = r_u32(0x8008F1D8u);
    uint32 cursor, start, end, size;
    uint8 *payload;
    FILE *file = NULL;
    size_t received;
    int extra, failed;
    /* Select the translated image from the actual loaded PS-X EXE */
    if (memcmp(psx_addr(executable, 8u), "PS-X EXE", 8u) ||
        r_u32(executable + 16u) != 0x80045E80u || r_u32(executable + 24u) != 0x80010000u)
        return gt_draft_unresolved_call(0x8006A334u, executable, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (fopen_s(&file, "DATA/GTOS.payload.bin", "rb") || !file) goto failed;
    payload = (uint8 *)malloc(415744u);
    if (!payload) { fclose(file); goto failed; }
    received = fread(payload, 1u, 415744u, file);
    extra = fgetc(file);
    failed = ferror(file);
    fclose(file);
    if (received != 415744u || extra != EOF || failed) { free(payload); goto failed; }
    memcpy(psx_addr(0x80010000u, 415744u), payload, 415744u);
    free(payload);
    /* The native entry loads decoded data without executing the PSLZ or BIOS loader */
    w_u32(0x8004D924u, bound);
    w_u32(0x8004D950u, 0x8004B2E0u - 0x80010000u);
    w_u32(0x8004D958u, 0x800756C4u - 0x8004D930u);
    w_u32(0x8004D960u, 0x801B56C4u - 0x800756C4u);
    w_u32(0x8004D964u, 1u);
    w_u32(0x8004D968u, 0u);
    cursor = 0x801B56C4u;
    do { cursor -= 4u; w_u32(cursor, 0u); } while (cursor >= 0x800756C4u);
    start = 0x801B56D0u;
    end = ((bound - r_u32(0x80073B5Cu)) & ~15u) | 0x80000000u;
    size = end - start;
    if ((sint32)size < 0) size = 0u;
    w_u32(0x80073ABCu, start);
    w_u32(0x80073AB8u, size);
    memset(psx_addr(start, size), 0, size);
    w_u32(start, 0u);
    w_u32(start + 4u, 0u);
    w_u32(start + 8u, size < 16u ? 0u : size - 16u);
    w_u32(start + 12u, r_u32(start + 12u) & ~1u);
    w_u8(0x801B16C0u, 0u);
    w_u32(0x80073B60u, 1u);
    return gtos_native_main();
failed:
    return gt_draft_unresolved_call(0x8006A334u, executable, bound, 0u, 0u, 0u, 0u, 0u, 0u);
}
