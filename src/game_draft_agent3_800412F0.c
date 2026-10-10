#include "game_first2_registry.h"

/* GTMAIN.EXE 800412F0 */
uint32 gtmain_menu_item_transition_draw(uint32 output, uint32 ordering,
    uint32 selector, sint32 x, sint32 y, sint32 intensity, uint32 flags)
{
    sint16 previous_x = (sint16)x, current_x = (sint16)x;
    sint16 previous_y = (sint16)y, current_y = (sint16)y;
    sint32 previous_intensity = 0, current_intensity = intensity;
    uint32 color = gtmain_menu_palette_color(0, intensity);
    uint32 descriptor, result, coordinate;
    sint32 mode, stride, step, offset;
    w_u8(output + 1u, 255u);
    mode = (sint8)r_u8(selector + 14u);
    if (mode) {
        w_u8(output + 1u, r_u8(selector + 13u));
        mode = (sint8)r_u8(selector + 14u);
        if (mode >= 1 && mode <= 4) {
            stride = (sint16)r_u16(selector + ((mode <= 2) ? 2u : 4u));
            step = (sint8)r_u8(selector + 15u);
            offset = (sint32)((uint32)stride * (uint32)step) / 12;
            previous_intensity = (sint32)((uint32)intensity * (12u - (uint32)step)) / 12;
            if (mode == 1) {
                coordinate = (uint32)x + (uint32)offset;
                previous_x = (sint16)coordinate;
                current_x = (sint16)(coordinate - (uint32)stride);
            } else if (mode == 2) {
                coordinate = (uint32)x - (uint32)offset;
                previous_x = (sint16)coordinate;
                current_x = (sint16)(coordinate + (uint32)stride);
            } else if (mode == 3) {
                coordinate = (uint32)y + (uint32)offset;
                previous_y = (sint16)coordinate;
                current_y = (sint16)(coordinate - (uint32)stride);
            } else {
                coordinate = (uint32)y - (uint32)offset;
                previous_y = (sint16)coordinate;
                current_y = (sint16)(coordinate + (uint32)stride);
            }
        }
        step = (sint8)r_u8(selector + 13u);
        descriptor = r_u32(selector + 8u) + (uint32)step * 12u;
        (void)gtmain_menu_item_center_draw(ordering, descriptor,
            previous_x, previous_y, previous_intensity >> 1);
        step = (sint8)r_u8(selector + 15u);
        current_intensity = (255 * step) / 12;
    } else {
        uint32 orientation = r_u8(selector + 12u);
        if (!(orientation & 32u) && (sint8)r_u8(0x80091B41u) && (flags & 1u)) {
            orientation &= 31u;
            /* TODO Bind the four unrecorded original arrow bodies */
            if (orientation == 1u) {
                offset = (sint16)r_u16(selector + 4u) >> 1;
                (void)gt_draft_unresolved_call(0x8003B5B8u, ordering, (sint16)x,
                    (uint32)(sint32)(sint16)y - (uint32)offset - 12u, color, 0, 0, 0, 0);
                offset = (sint16)r_u16(selector + 4u) >> 1;
                (void)gt_draft_unresolved_call(0x8003B600u, ordering, (sint16)x,
                    (uint32)(sint32)(sint16)y + (uint32)offset + 12u, color, 0, 0, 0, 0);
            } else if (!orientation) {
                offset = (sint16)r_u16(selector + 2u) >> 1;
                (void)gt_draft_unresolved_call(0x8003B690u, ordering,
                    (uint32)(sint32)(sint16)x - (uint32)offset - 12u, (sint16)y, color, 0, 0, 0, 0);
                offset = (sint16)r_u16(selector + 2u) >> 1;
                (void)gt_draft_unresolved_call(0x8003B648u, ordering,
                    (uint32)(sint32)(sint16)x + (uint32)offset + 12u, (sint16)y, color, 0, 0, 0, 0);
            }
            (void)gtmain_packet_page(ordering, 544);
            step = (sint8)r_u8(selector + 15u);
            current_intensity = (255 * step) / 12;
        }
    }
    step = (sint8)r_u8(selector + 1u);
    descriptor = r_u32(selector + 8u) + (uint32)step * 12u;
    (void)gtmain_menu_item_center_draw(ordering, descriptor,
        current_x, current_y, current_intensity >> 1);
    result = r_u8(selector + 1u);
    w_u16(output + 2u, (uint16)current_x);
    w_u16(output + 4u, (uint16)current_y);
    w_u8(output + 10u, (uint32)current_intensity);
    w_u16(output + 6u, (uint16)previous_x);
    w_u16(output + 8u, (uint16)previous_y);
    w_u8(output + 11u, (uint32)previous_intensity);
    w_u8(output, result);
    return result;
}
