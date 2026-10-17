#include "game_draft_registry.h"
#include "psx_mdec.h"

static MdecState gtos_decoder;

void gtos_mdec_reset(uint32 mode)
{
    uint32 tables[2] = { 0x80073BE4u, 0x80073C68u };
    uint32 i;
    if (mode > 1u) {
        gt_draft_unresolved_call(0x80042500u, mode, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    if (!mode) ResetCallback();
    if (!mode) mdec_initialize(&gtos_decoder);
    else mdec_control(&gtos_decoder, 0x80000000u);
    mdec_control(&gtos_decoder, 0x60000000u);
    if (mode) return;
    /* Load the image's quantization and inverse transform tables */
    for (i = 0u; i < 2u; ++i) {
        if (mdec_write_data(&gtos_decoder, r_u32(tables[i])) != 2u ||
            mdec_dma_write(&gtos_decoder, (const uint32 *)psx_addr(tables[i] + 4u, 128u), 32u) != 64u) {
            gt_draft_unresolved_call(0x800428B0u, tables[i], 32u, 0u, 0u, 0u, 0u, 0u, 0u);
            return;
        }
    }

}

void gtos_mdec_image(uint32 input, uint32 width, uint32 height, sint32 x, sint32 y)
{
    uint32 column, columns = (width + 15u) >> 4u;
    uint32 rows = (height + 15u) & ~15u;
    uint32 words = r_u16(input), fed = 0u, command = r_u32(input) & ~0x0A000000u;
    uint32 strip[3072];
    PSX_RECT rect;
    if (rows > 256u || !columns)
        gt_draft_unresolved_call(0x8002F7FCu, width, height, 0u, 0u, 0u, 0u, 0u, 0u);
    gtos_mdec_reset(1u);
    w_u32(input, command);
    mdec_write_data(&gtos_decoder, command);
    rect.x = (sint16)x;
    rect.y = (sint16)y;
    rect.w = 24;
    rect.h = (sint16)rows;
    /* Drain each original vertical macroblock column before uploading it */
    for (column = 0u; column < columns; ++column) {
        uint32 received = 0u, required = 12u * rows;
        while (received < required) {
            uint32 before_fed = fed, before_received = received;
            while (fed < words) {
                if (!mdec_write_data(&gtos_decoder, r_u32(input + 4u + 4u * fed))) break;
                ++fed;
            }
            if (gtos_decoder.copy_ticks) mdec_advance(&gtos_decoder, gtos_decoder.copy_ticks);
            received += mdec_dma_read(&gtos_decoder, strip + received, required - received);
            if (fed == before_fed && received == before_received && !gtos_decoder.copy_ticks)
                gt_draft_unresolved_call(0x800426E0u, fed, words, received, required, column, 0u, 0u, 0u);
        }
        if (LoadImagePSX(&rect, strip) < 0)
            gt_draft_unresolved_call(0x80046838u, column, rows, 0u, 0u, 0u, 0u, 0u, 0u);
        DrawSync(0);
        rect.x += 24;
    }

}

void gtos_mdec_frame(uint32 input, uint32 width, uint32 height, sint32 x, sint32 y)
{
    gtos_mdec_image(input, width, height, (3 * x) >> 1, y);
    w_u16(0x8004D9BCu, r_u16(0x8004D9BCu) + 1u);
}
