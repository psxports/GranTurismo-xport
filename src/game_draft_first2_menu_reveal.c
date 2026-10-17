#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 80033A1C */
uint32 gtmain_menu_result_name_request_initialize(void)
{
    sint32 mode = r_s8(0x801D2FECu);
    if ((mode >= 3 && mode < 6) || (mode >= 7 && mode < 9)) {
        gtmain_menu_resource_request_initialize(0x8009196Cu, 11, 128, 0x801D300Cu);
        gtmain_menu_name_text_set(0x8009196Cu, 0x800CFF88u);
    } else {
        gtmain_menu_resource_request_initialize(0x8009196Cu, 3, 0, 0x801D300Cu);
    }
    return 0u;
}

static void end_panel_initialize_missing(uint32 context)
{
    /* TODO Bind the unselected one-argument panel initializer */
    gt_draft_failure("End panel initialization at801F3018", 0x8003D1A8u);
}

/* GTMAIN.EXE 80038660 */
uint32 gtmain_menu_end_panel_initialize(void)
{
    sint32 mode;
    uint32 title, asset;
    w_u16(0x801F3002u, 0u);
    w_u8(0x801F2FE5u, 0u);
    end_panel_initialize_missing(0x801F3018u);
    mode = r_s8(0x801F2FE0u);
    switch (mode) {
    case 1: title = 0x800BCBE3u; asset = 19u; break;
    case 2: title = 0x800BCBC9u; asset = 19u; break;
    case 3: title = 0x800BCBB1u; asset = 19u; break;
    default: title = 0x800BCBFEu; asset = 16u; break;
    }
    w_u32(0x801F2FDCu, title);
    gtmain_menu_asset_activate(asset);
    return 0u;
}

/* GTMAIN.EXE 80037CC8 */
sint32 gtmain_menu_quick_end_run(uint32 selector, uint8 disabled, uint32 first,
    uint32 second, uint8 style)
{
    uint8 course = r_u8(0x800B63BDu);
    sint32 status;
    w_u32(0x801F2FE0u, first);
    w_u32(0x801F2FE4u, second);
    w_u8(0x801F2FDCu, disabled);
    w_u16(0x801F2FDEu, 3584u);
    w_u8(0x801F2FE8u, 0u);
    w_u8(0x801F2FEBu, style);
    w_u8(0x801F2FECu, course);
    gtmain_menu_draw_callback_set(selector);
    gtmain_menu_panel_special_bind(0x800BDD10u, 0x80092154u);
    gtmain_music_sector_init();
    gtmain_stream_stop_wait(1u);
    gtmain_menu_frame_run();
    gtmain_stream_stop_wait(1u);
    status = r_s8(0x801F2FDDu);
    printf((const char *)psx_addr(0x8008CC3Cu, 1u), status);
    return r_s8(0x801F2FDDu);
}

/* GTMAIN.EXE 80037E00 */
sint32 gtmain_menu_quick_end_mode3(uint8 disabled)
{
    return gtmain_menu_quick_end_run(0x80091B8Cu, disabled, 0x80091B7Cu,
        0x80091B74u, 3u);
}

/* GTMAIN.EXE 800379C0 */
uint32 gtmain_menu_quick_end_initialize(void)
{
    uint32 first, second, disabled;
    gtmain_menu_asset_activate(22u);
    first = r_u8(0x80091B4Cu) & 0x3Fu;
    w_u8(0x80091B4Cu, first);
    second = r_u8(0x80091B64u) & 0x3Fu;
    disabled = r_u8(0x801F2FDCu);
    w_u8(0x80091B64u, second);
    if (!disabled) {
        w_u8(0x80091B4Cu, first | 0x40u);
        w_u8(0x80091B64u, second | 0x40u);
    }
    gtmain_menu_selector_reset(0x80091B7Cu);
    return 0u;
}

static void reveal_panel_create(uint32 owner, uint32 index,
    uint32 descriptor, uint32 count, uint32 flags)
{
    gt_draft_8003CC24(owner, index, descriptor, count, flags);
}

/* GTMAIN.EXE 80034B98 */
uint32 gtmain_menu_reveal_input_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), result = 0u;
    gtmain_menu_stream_check();
    gtmain_menu_model_pose_tick(0x801D3008u, 1u);
    if ((sint8)r_u8(input)) {
        sint32 phase = (sint16)r_u16(0x801D3004u);
        if (phase == 90) {
            if (r_u16(input + 6u) & 0xFF0Fu) {
                uint32 descriptor = 0x80092170u;
                if (r_u8(0x801D2FDCu) & 2u) {
                    reveal_panel_create(0x801D2FF8u, 0u, 0x80092170u, 17u, 0u);
                    descriptor = 0x80092248u;
                }
                result = 3u;
                gtmain_menu_push(descriptor);
                gtmain_menu_sound(4);
            }
        } else {
            if (phase == 30 || phase == 42) gtmain_menu_sound(1);
            w_u16(0x801D3004u, r_u16(0x801D3004u) + 1u);
        }
    }
    w_u16(0x801D3020u, (r_u16(0x801D3020u) + 1u) & 3u);
    return result;
}

/* GTMAIN.EXE 80038304 */
uint32 gtmain_menu_end_panel_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), phase;
    gtmain_menu_stream_check();
    gtmain_menu_model_pose_tick(0x801F3018u, 1u);
    if (!r_s8(input)) return 0u;
    gt_draft_800380AC();
    phase = r_u16(0x801F3002u);
    if (phase == 60u) {
        w_u16(0x801F3002u, phase + 1u);
    } else if (phase == 30u) {
        uint32 next;
        gtmain_menu_sound(1);
        next = r_u16(0x801F3002u);
        w_u16(0x801F300Au, 8u);
        w_u16(0x801F3002u, next + 1u);
    } else if (phase == 90u) {
        uint32 descriptor = 0x8009209Cu;
        if (!(r_u16(input + 6u) & 0xFF0Fu)) return 0u;
        if (r_s8(0x801F2FE1u) >= 0) {
            descriptor = 0x800920F0u;
        } else {
            uint32 flags = r_u8(0x801F2FE6u);
            if (flags & 1u) descriptor = 0x800920D4u;
            else if (flags & 4u) descriptor = 0x80092248u;
        }
        gtmain_menu_push(descriptor);
        gtmain_menu_sound(4);
        return 3u;
    } else {
        w_u16(0x801F3002u, r_u16(0x801F3002u) + 1u);
    }
    return 0u;
}

static void end_panel_title_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 text, sint32 style, sint32 intensity)
{
    /* TODO Bind unselected381A4 with the six established argument carriers */
    gt_draft_failure("End panel title transition", 0x800381A4u);
}

/* GTMAIN.EXE 80038484 */
uint32 gtmain_menu_end_panel_draw(uint32 ordering, sint32 x, sint32 y,
    sint32 intensity)
{
    gtmain_menu_machine_caption_draw(ordering, x, y, intensity);
    if (r_u16(0x801F3002u) >= 31u) {
        sint32 width, centered, style;
        uint32 mode, text, toggle;
        gtmain_text_font(3);
        gtmain_text_flags(0u);
        width = gtmain_text_width(3, r_u32(0x801F2FDCu));
        centered = (sint32)((uint32)x - ((uint32)(width >> 1) - 160u));
        mode = r_u8(0x801F2FE0u);
        text = r_u32(0x801F2FDCu);
        toggle = r_u8(0x801F3008u);
        style = r_s8(0x80091EDCu + 2u * (mode & 3u) + toggle);
        end_panel_title_draw(ordering, centered,
            (sint32)((uint32)y + 64u), text, style, intensity);
    }
    if (r_u16(0x801F3002u) >= 90u) {
        sint32 width, centered;
        gtmain_text_font(2);
        gtmain_text_color(gtmain_menu_pulse_color(intensity >> 1));
        width = gtmain_text_width(2, 0x800BCC0Cu);
        centered = (sint32)((uint32)x - ((uint32)(width >> 1) - 160u));
        gtmain_font_text_draw(ordering, centered,
            (sint32)((uint32)y + 204u), 0x800BCC0Cu);
    }
    if (!r_s8(0x801F2FE5u)) {
        sint32 mode = r_s8(0x801F2FE0u);
        if (mode > 0) {
            uint32 row = (uint32)mode << 6u;
            uint32 first = (((row + 256u) << 6u) | 20u) & 0xFFFFu;
            uint32 second = (((row + 257u) << 6u) | 21u) & 0xFFFFu;
            gtmain_menu_pose_model_draw(0x801F3018u, ordering,
                (sint32)((uint32)x + 160u), (sint32)((uint32)y + 105u),
                0x8000u, 0x8018AFDCu, 8u, first, 8u, second, 740u, 192u, 96u);
        }
    }
    return 0u;
}
