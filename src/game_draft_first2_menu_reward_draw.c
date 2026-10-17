#include "game_first2_registry.h"

static uint32 reward_amount_draw(uint32 ordering, sint32 x, sint32 y, sint32 palette, sint32 intensity, uint32 amount, sint32 span, sint32 row_y, sint32 elapsed)
{
    /* TODO Bind nine-argument amount drawing without expanding unselected game code */
    gt_draft_failure("Native reward amount draw 346F0", 0x800346F0u);
}
static uint32 reward_title_draw(uint32 ordering, sint32 x, sint32 y, uint32 text, sint32 palette, sint32 intensity, sint32 elapsed)
{
    /* TODO Bind seven-argument animated title drawing */
    gt_draft_failure("Native reward title draw 34248", 0x80034248u);
}
static sint32 reward_center(sint32 x, sint32 width)
{
    return (sint32)((uint32)x - ((uint32)(width >> 1) - 160u));
}

/* GTMAIN.EXE:8003480C */
uint32 gtmain_menu_reward_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity)
{
    sint32 place, palette, style = 3, phase, elapsed, width;
    uint32 animation, amount, total, flags, title;
    gtmain_text_flags(0);
    place = r_s8(0x801D2FDEu);
    animation = r_u8(0x801D3024u);
    palette = r_s8(0x80091A08u + animation);
    if (place >= 0 && place < 3)
        style = r_s8(0x80091B28u + 4u * (uint32)place + animation);
    phase = r_s16(0x801D3022u);
    elapsed = phase - 66;
    if (elapsed >= 0) {
        amount = r_u32(0x801D2FE0u);
        place = r_s16(0x801D2FECu);
        (void)reward_amount_draw(ordering, x, y, palette, intensity, amount, place, 92, elapsed);
        phase = r_s16(0x801D3022u);
    }
    elapsed = phase - 90;
    if (elapsed >= 0) {
        total = r_u32(0x801D2FE4u);
        amount = r_u32(0x801D2FE0u);
        place = r_s16(0x801D2FEEu);
        (void)reward_amount_draw(ordering, x, y, palette, intensity, total - amount, place, 158, elapsed);
    }
    phase = r_s16(0x801D3022u);
    elapsed = phase - 54;
    if (elapsed >= 0) {
        place = r_s8(0x801D2FDEu);
        flags = r_u32(0x801D2FDCu);
        title = r_u32(0x800919ECu + 4u * (uint32)place);
        if (!(flags & 0x00FFFF00u)) title = 0x800BCD10u;
        width = gtmain_text_width(3, title);
        (void)reward_title_draw(ordering, reward_center(x, width), (sint32)((uint32)y + 64u), title, style, intensity, elapsed);
    }
    (void)gtmain_text_font(2);
    phase = r_s16(0x801D3022u);
    elapsed = phase - 78;
    if (elapsed >= 0) {
        width = gtmain_text_width(2, 0x800BCD01u);
        (void)reward_title_draw(ordering, reward_center(x, width), (sint32)((uint32)y + 136u), 0x800BCD01u, 3, intensity, elapsed);
        phase = r_s16(0x801D3022u);
    }
    if (phase >= 170) {
        (void)gtmain_text_color(gtmain_menu_pulse_color(intensity >> 1));
        width = gtmain_text_width(2, 0x800BCC0Cu);
        (void)gtmain_font_text_draw(ordering, reward_center(x, width), (sint32)((uint32)y + 204u), 0x800BCC0Cu);
    }
    return 0;
}
