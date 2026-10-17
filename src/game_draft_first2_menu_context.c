#include "game_first2_registry.h"
#include <string.h>
#include <stdio.h>

static uint32 menu_scale(uint32 product)
{
    sint64 wide = (sint64)(sint32)product * (sint64)(sint32)0x80808081u;
    uint32 high = (uint32)((uint64)wide >> 32);
    return (uint32)((sint32)(high + product) >> 7) - (uint32)((sint32)product >> 31);
}
static uint32 menu_callback_dispatch(uint32 token)
{
    switch (token) {
    case 0x80035D88u: return (uint32)gtmain_menu_default_callback();
    case 0x8003A3B4u: return (uint32)gtmain_menu_effect_reset();
    case 0x8003A424u: return (uint32)gtmain_menu_effect_restart();
    case 0x8003A3F0u: return (uint32)gtmain_menu_effect_tick();
    case 0x8003A344u: return gtmain_menu_effect_begin();
    case 0x800335ECu: return gtmain_menu_result_prepare();
    case 0x80032D1Cu: return gtmain_menu_lap_results_prepare();
    case 0x800379C0u: return gtmain_menu_quick_end_initialize();
    case 0x80036350u: return (uint32)gtmain_menu_race_selection_tick();
    default:
        /* TODO Bind remaining callback tokens with their actual native ABI */
        gt_draft_failure("Native menu callback token adapter", token);
    }
}
/* GTMAIN.EXE 8003527C */
uint32 gtmain_menu_callback(uint32 token)
{
    return menu_callback_dispatch(token);
}

uint32 gtmain_menu_name(uint32 guest_name)
{
    if (!strncmp((const char *)psx_addr(guest_name, 7), (const char *)psx_addr(0x8008CBC4u, 7), 7)) return guest_name + 7u;
    return guest_name;
}
sint32 gtmain_menu_push_tail(uint32 descriptor, uint32 context, sint32 previous_index)
{
    uint32 next = (uint32)previous_index + 1u;
    w_u8(context + 256u, next);
    w_u32(context + 192u + (uint32)(sint32)(sint8)(uint8)next * 4u, descriptor);
    return 0;
}
sint32 gtmain_menu_push(uint32 descriptor)
{
    uint32 context = r_u32(0x800BDD08u);
    sint32 previous = (sint8)r_u8(context + 256u);
    if (previous >= 15) return -1;
    return gtmain_menu_push_tail(descriptor, context, previous);
}
uint32 gtmain_menu_flags(uint8 flags)
{
    uint32 context = r_u32(0x800BDD08u);
    w_u8(context + 258u, flags); return context;
}
uint32 gtmain_menu_pulse_color(sint32 intensity)
{
    uint32 address = 0x80091AF8u + (uint32)(sint32)(sint8)r_u8(0x80091B38u) * 3u;
    uint32 first = (uint32)intensity * r_u8(address);
    uint32 second = (uint32)intensity * r_u8(address + 1u);
    uint32 third = (uint32)intensity * r_u8(address + 2u);
    return (menu_scale(first) & 255u) | ((menu_scale(second) & 255u) << 8) | ((menu_scale(third) & 255u) << 16);
}
sint32 gtmain_menu_pulse_component(sint32 channel, sint32 intensity)
{
    uint32 phase = (uint32)(sint32)(sint8)r_u8(0x80091B38u);
    uint32 offset = channel == 0 ? 0u : channel == 1 ? 1u : 2u;
    uint32 product = (uint32)intensity * r_u8(0x80091AF8u + phase * 3u + offset);
    return (sint32)menu_scale(product);
}
uint32 gtmain_menu_sound(sint32 effect)
{
    return (uint32)gtmain_race_sound_effect(effect, 0x4000, 0x40, 0);
}
sint32 gtmain_menu_selection(uint16 buttons, sint32 selection, sint32 count, uint32 decrement_mask, uint32 increment_mask, sint32 sound, uint32 wrap)
{
    if (buttons & decrement_mask) {
        if (sound >= 0) (void)gtmain_menu_sound(sound);
        selection = (sint32)((uint32)selection - 1u);
        if (selection < 0) { selection = 0; if (wrap) selection = (sint32)((uint32)count - 1u); }
    }
    if (buttons & increment_mask) {
        if (sound >= 0) (void)gtmain_menu_sound(sound);
        selection = (sint32)((uint32)selection + 1u);
        if (selection >= count) { selection = 0; if (!wrap) selection = (sint32)((uint32)count - 1u); }
    }
    return selection;
}
uint32 gtmain_menu_input(void)
{
    uint32 context = r_u32(0x800BDD08u), packet;
    (void)gtmain_input_poll();
    packet = gtmain_input_packet(0); (void)gtmain_input_decode(context + 2u, packet);
    w_u32(context + 152u, gtmain_input_packet(0));
    packet = gtmain_input_packet(4); (void)gtmain_input_decode(context + 76u, packet);
    w_u32(context + 156u, gtmain_input_packet(4));
    return 1;
}
static void menu_draw_prepare_missing(void)
{
    /* TODO Bind the unselected no-argument menu draw preparation */
    gt_draft_failure("Native menu draw preparation", 0x80035DC0u);
}

uint32 gtmain_menu_draw(void)
{
    sint32 prepare = r_s8(0x80091B39u);
    uint32 context = r_u32(0x800BDD08u), draw_context, start, end;
    sint32 mode;
    if (prepare) menu_draw_prepare_missing();
    mode = r_s8(context + 259u);
    switch (mode) {
    case 0: return gtmain_menu_panel_draw(r_u32(0x800BDD08u), 0, 0, 0);
    case 1:
        start = (uint32)r_s16(context + 262u);
        draw_context = r_u32(0x800BDD08u);
        gtmain_menu_panel_draw(draw_context, 0u - start, 0, 0);
        end = (uint32)r_s16(context + 266u);
        start = (uint32)r_s16(context + 262u);
        draw_context = r_u32(0x800BDD08u);
        return gtmain_menu_panel_draw(draw_context, end - start, 0, 1);
    case 2:
        start = (uint32)r_s16(context + 262u);
        end = (uint32)r_s16(context + 266u);
        draw_context = r_u32(0x800BDD08u);
        gtmain_menu_panel_draw(draw_context, start - end, 0, 1);
        draw_context = r_u32(0x800BDD08u);
        start = (uint32)r_s16(context + 262u);
        return gtmain_menu_panel_draw(draw_context, start, 0, 0);
    case 3:
        start = (uint32)r_s16(context + 264u);
        draw_context = r_u32(0x800BDD08u);
        gtmain_menu_panel_draw(draw_context, 0, 0u - start, 0);
        end = (uint32)r_s16(context + 268u);
        start = (uint32)r_s16(context + 264u);
        draw_context = r_u32(0x800BDD08u);
        return gtmain_menu_panel_draw(draw_context, 0, end - start, 1);
    case 4:
        start = (uint32)r_s16(context + 264u);
        end = (uint32)r_s16(context + 268u);
        draw_context = r_u32(0x800BDD08u);
        gtmain_menu_panel_draw(draw_context, 0, start - end, 1);
        draw_context = r_u32(0x800BDD08u);
        start = (uint32)r_s16(context + 264u);
        return gtmain_menu_panel_draw(draw_context, 0, start, 0);
    default: return 0u;
    }
}
uint32 gtmain_menu_exit_requested(void)
{
    uint32 context = r_u32(0x800BDD08u);
    if (context == 0xFFFFFFFEu) return 0;
    return (r_u8(context + 257u) != 0) * 2u;
}
uint32 gtmain_menu_transition(sint8 mode)
{
    uint32 context = r_u32(0x800BDD08u), result = 4;
    sint32 previous = (sint8)r_u8(context + 259u);
    if (previous == 0 || (previous == 1 && mode == 1) || (previous == 2 && mode == 2))
        w_u16(context + 262u, r_u16(context + 266u));
    else if ((previous == 1 && mode == 2) || (previous == 2 && mode == 1))
        w_u16(context + 262u, 320u - r_u16(context + 262u));
    if (mode == 3 || mode == 4) { result = r_u16(context + 268u); w_u16(context + 264u, result); }
    w_u8(context + 259u, (uint8)mode);
    return result;
}
uint32 gtmain_menu_transition_tick(void)
{
    uint32 context = r_u32(0x800BDD08u), result, value;
    sint32 mode = (sint8)r_u8(context + 259u);
    result = mode < 3;
    if (mode > 0) {
        result = mode < 5;
        if (mode >= 5) return result;
        if (mode < 3) { value = r_u16(context + 262u); value -= r_u16(context + 270u); w_u16(context + 262u, value); }
        else { value = r_u16(context + 264u); value -= r_u16(context + 272u); w_u16(context + 264u, value); }
        result = value << 16;
        if ((sint32)result < 0) w_u8(context + 259u, 0);
    }
    return result;
}
static uint32 menu_current_descriptor(uint32 context)
{
    sint32 index = (sint8)r_u8(context + 256u);
    return r_u32(context + 192u + (uint32)index * 4u);
}
static sint32 menu_pop_missing(void)
{
    /* TODO Bind the unselected no-argument signed-result menu pop */
    gt_draft_failure("Native menu pop service", 0x8003532Cu);
}

uint32 gtmain_menu_tick(void)
{
    uint32 context = r_u32(0x800BDD08u), old, descriptor, command, enabled;
    gtmain_menu_frame_begin();
    (void)gtmain_menu_flags(4);
    old = r_u8(0x80091B38u); w_u8(0x80091B38u, old + 1u);
    if ((sint8)old >= 16) w_u8(0x80091B38u, 0);
    old = r_u8(0x80091B40u); w_u8(0x80091B40u, old + 1u);
    if ((sint8)old >= 32) w_u8(0x80091B40u, 0);
    old = (uint32)(sint32)(sint8)r_u8(0x80091B40u);
    w_u8(0x80091B41u, (r_u32(0x80091B3Cu) >> (old & 31u)) & 1u);
    if ((sint8)r_u8(context + 259u)) (void)gtmain_menu_transition_tick();
    if (!gtmain_menu_input()) return 0;
    enabled = r_u8(context + 259u) - 3u >= 2u;
    w_u8(context, enabled); w_u8(context + 1u, enabled);
    descriptor = menu_current_descriptor(context);
    command = gtmain_menu_callback(r_u32(descriptor + 8u));
    if (!enabled) return 1;
    switch (command) {
    case 0:
        { sint32 kind = (sint8)r_u8(descriptor); uint32 buttons = r_u16(context + 6u); if (kind == 1 && (buttons & 8u)) w_u8(context + 257u, 1); }
        return 1;
    case 1:
        if (menu_pop_missing() < 0) w_u8(context + 257u, 1);
        else (void)gtmain_menu_callback(r_u32(menu_current_descriptor(context) + 20u));
        w_u32(context + 188u, descriptor); (void)gtmain_menu_transition(3); return 1;
    case 2:
        if (menu_pop_missing() < 0) w_u8(context + 257u, 1);
        if (menu_pop_missing() < 0) w_u8(context + 257u, 1);
        else (void)gtmain_menu_callback(r_u32(menu_current_descriptor(context) + 20u));
        w_u32(context + 188u, descriptor); (void)gtmain_menu_transition(3); return 1;
    case 3:
        old = (uint32)(sint32)(sint8)r_u8(context + 256u);
        w_u32(context + 188u, descriptor);
        (void)gtmain_menu_callback(r_u32(r_u32(context + 192u + old * 4u) + 16u));
        (void)gtmain_menu_transition(4); return 1;
    case 5: w_u32(context + 188u, descriptor); (void)gtmain_menu_transition(1); return 1;
    case 6: w_u32(context + 188u, descriptor); (void)gtmain_menu_transition(2); return 1;
    case 7: w_u8(context + 257u, 1); printf((const char *)psx_addr(0x8008CBE0u, 1u)); return 1;
    default: return 1;
    }
}
