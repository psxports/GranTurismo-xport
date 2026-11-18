#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include "psx_press.h"

void gtmenu_used_sell_event(uint32 event)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 kind, page, previous, target;
    sint32 count = 0;
    FUNCTION_MARKER(0x80026874, "GTMENU.EXE");
    if (r_u32(0x80070898)) count = gtmenu_model_info_count();
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_scene_draw(r_u32(event + 4));
        gtmenu_sell_price_draw(r_u32(event + 4));
        if (!r_u32(0x80070898)) return;
        frame = gtmenu_local_stack_push(8, 0x80026874);
        w_u32(frame.address, 0x00410096);
        w_u32(frame.address + 4, 0x00FC01CA);
        gtmenu_model_info_angle_add(r_s16(0x800708A2));
        gtmenu_color_picker_bounds_set(frame.address);
        gtmenu_model_preview_frame_draw();
        gtmenu_model_preview_draw(r_u32(event + 4), 192, 64);
        gtmenu_local_stack_pop(frame);
    } else if (kind == 24) {
        if ((r_u32(event + 12) & 0x9000) && gtmenu_page_back_target()) {
            gtmenu_sound_effect(3);
            gtmenu_garage_callback_reset();
            page = r_u16(0x800A77C4);
            previous = r_u16(0x8007089C);
            w_u16(0x800A77C0, page);
            w_u16(0x800A77C4, previous);
            target = gtmenu_page_back_target();
            gtmenu_page_open(target);
        } else if (r_u32(event + 12) & 0x6000) {
            gtmenu_used_sell_activate();
        }
    }
    if (r_u32(0x80070898) && gtmenu_model_info_selection() >= count)
        gtmenu_model_info_selection_set((uint32)count - 1);
}

uint32 gtmenu_used_sell_begin(uint32 previous_page)
{
    uint32 archive, category, record, entry, reference, strings, first, second;
    sint32 index;
    FUNCTION_MARKER(0x80026A2C, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    category = r_u16(0x800A77C2);
    w_u32(0x80070898, 0);
    w_u32(0x8007089C, previous_page);
    w_u32(0x800A77B4, 0x80026874);
    w_u16(0x800708A0, category);
    index = gtmenu_current_record_index();
    if (index < 0) return index;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    entry = archive + 24 + (r_u16(record) & 0xFFF) * 424;
    reference = r_u32(entry + 392);
    strings = r_u32(archive + 16);
    first = gtmenu_packed_text_entry(r_u32(strings + (reference >> 16) * 4 + 4), reference & 0xFFFF) + 1;
    entry = archive + 24 + (r_u16(record) & 0xFFF) * 424;
    reference = r_u32(entry + 396);
    strings = r_u32(archive + 16);
    second = gtmenu_packed_text_entry(r_u32(strings + (reference >> 16) * 4 + 4), reference & 0xFFFF) + 1;
    gtmenu_format_strings2(0x8009ABAC, 0x80054F50, first, second);
    w_u32(0x80070898, 1);
    return 1;
}

uint32 gtmenu_used_sell_commit(void)
{
    uint32 record, base, bonus = 0, sale;
    sint32 index, make;
    sint64 credits;
    FUNCTION_MARKER(0x80026B80, "GTMENU.EXE");
    gtmenu_resource_group(13);
    credits = (sint64)(sint32)gtmenu_credits_get() * 100;
    if (gtmenu_current_record_index() < 0) return 0;
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    index = gtmenu_current_record_index();
    base = gtmenu_garage_price(index);
    if (!base) return 0;
    index = gtmenu_record_pointer_index(record);
    make = r_s16(0x800708A0);
    if (gtmenu_record_matches_make(record, make)) bonus = base / 10;
    sale = (base >> 1) + bonus;
    credits += sale;
    if (credits > 200000000000LL) credits = 200000000000LL;
    credits = gtmenu_signed_divide64(credits, 100, 0x80026C94);
    gtmenu_credits_set(credits);
    gtmenu_garage_remove(index);
    gtmenu_current_record_set(-1);
    w_u32(0x80070898, 0);
    return 1;
}

uint32 gtmenu_map_return(void)
{
    uint32 callback, page;
    FUNCTION_MARKER(0x80026CD8, "GTMENU.EXE");
    callback = r_u32(0x8009AC2C);
    w_u8(0x800708AC, 0);
    w_u32(0x800A77B4, callback);
    gtmenu_music_stop();
    gtmenu_music_select(1);
    gtmenu_music_start();
    page = r_u16(0x800A77C4);
    w_u16(0x800A77C4, 1);
    w_u16(0x800A77C0, page);
    return gtmenu_page_open(0x80054F58);
}

uint32 gtmenu_car_wash_activate(void)
{
    uint32 action, record, page;
    sint32 index;
    sint64 credits;
    FUNCTION_MARKER(0x80026D4C, "GTMENU.EXE");
    action = r_u32(0x800A7A5C);
    credits = (sint64)(sint32)gtmenu_credits_get() * 100;
    if (gtmenu_string_compare(action, 0x80054F60)) {
        if (gtmenu_string_compare(action, 0x80054F58)) return 0;
        gtmenu_sound_effect(5);
        gtmenu_map_return();
        return 1;
    }
    if (gtmenu_current_record_index() < 0) {
        gtmenu_sound_effect(6);
        return 0;
    }
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    if (!r_u16(record + 90)) {
        gtmenu_sound_effect(6);
        return 0;
    }
    if (credits < 5000) {
        gtmenu_sound_effect(6);
        page = r_u16(0x800A77C4);
        gtmenu_confirmation_open(15, 0, page, 2);
        return 0;
    }
    gtmenu_sound_effect(7);
    credits -= 5000;
    w_u16(0x800708A4, 400);
    w_u16(0x800708A6, 225);
    credits = gtmenu_signed_divide64(credits, 100, 0x80026E2C);
    w_u16(record + 90, 0);
    gtmenu_credits_set(credits);
    return 1;
}

uint32 gtmenu_car_wash_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 record, result;
    sint32 index;
    FUNCTION_MARKER(0x80026EC0, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x80026EC0);
    w_u32(frame.address, 0x00460032);
    w_u32(frame.address + 4, 0x016E01A5);
    w_u32(frame.address + 8, 0x00E201E6);
    w_u32(frame.address + 12, 0x01640268);
    if (r_s8(0x800708AC)) {
        gtmenu_color_picker_bounds_set(frame.address);
        gtmenu_model_preview_frame_draw();
    }
    if (gtmenu_current_record_index() >= 0) {
        index = gtmenu_current_record_index();
        record = gtmenu_record_address(index);
        if (r_u16(record + 90)) {
            result = gtmenu_rectangle_pulse(ordering, frame.address + 8, 0);
            gtmenu_local_stack_pop(frame);
            return result;
        }
    }
    result = gtmenu_rectangle_shade(ordering, frame.address + 8, 0x60);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_car_wash_event(uint32 event)
{
    uint32 kind, remaining, value;
    sint32 timer;
    FUNCTION_MARKER(0x80026FB0, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_model_info_angle_add(r_s16(0x800708A4));
        timer = r_s16(0x800708A6);
        if (timer > 0) {
            if (!(timer % 22)) gtmenu_sound_effect(7);
            value = r_u16(0x800708A8);
            value -= r_u16(0x800708AA);
            w_u16(0x800708A8, value);
            gtmenu_preview_control_set((sint16)value);
            remaining = r_u16(0x800708A6);
            w_u16(0x800708A6, remaining - 1);
        } else {
            w_u16(0x800708A4, 5);
        }
        gtmenu_car_wash_draw(r_u32(event + 4));
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind == 24) {
        if (r_u32(event + 12) & 0x9000) {
            gtmenu_sound_effect(3);
            return gtmenu_map_return();
        }
        if (r_u32(event + 12) & 0x6000) return gtmenu_car_wash_activate();
        return 0;
    }
    return 24;
}

void gtmenu_car_wash_begin(void)
{
    uint32 callback, record, initial, current;
    sint32 index, step;
    FUNCTION_MARKER(0x8002712C, "GTMENU.EXE");
    w_u8(0x800708AC, gtmenu_current_record_index() >= 0);
    callback = r_u32(0x800A77B4);
    w_u16(0x800708A4, 5);
    w_u16(0x800708A6, 0);
    w_u32(0x800A77B4, 0x80026FB0);
    w_u32(0x8009AC2C, callback);
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    initial = r_u16(record + 90);
    step = (sint16)initial / 225;
    current = r_u16(record + 90);
    w_u16(0x800708A8, initial);
    w_u16(0x800708AA, step);
    gtmenu_preview_control_set(current);
}

uint32 gtmenu_text_draw_double(uint32 ordering, uint32 x, sint16 y, uint32 color, uint32 small, uint32 text)
{
    uint32 state, byte, code, encoded, position, top, bottom;
    FUNCTION_MARKER(0x800271F0, "GTMENU.EXE");
    state = gtmenu_text_surface_address();
    w_u32(state + 36, color);
    if (!r_u8(text)) return 0;
    top = (uint32)y << 16;
    bottom = ((uint32)y + 1) << 16;
    do {
        byte = r_u8(text);
        if (byte < 0x80) {
            code = byte | 0xA380;
            ++text;
        } else {
            byte = r_u8(text);
            code = byte << 8;
            code |= r_u8(text + 1);
            text += 2;
        }
        code &= 0x7F7F;
        encoded = gtmenu_font_code_encode(code);
        position = x & 0xFFFF;
        if (!small) {
            gtmenu_font_quad16_draw(state, encoded, ordering, position | top, 0x00100010);
            encoded = gtmenu_font_code_encode(code);
            gtmenu_font_quad16_draw(state, encoded, ordering, position | bottom, 0x00100010);
            x += 16;
        } else {
            gtmenu_font_quad12_draw(state, encoded, ordering, position | top, 0x000C000C);
            encoded = gtmenu_font_code_encode(code);
            gtmenu_font_quad12_draw(state, encoded, ordering, position | bottom, 0x000C000C);
            x += 12;
        }
    } while (r_u8(text));
    return 0;
}

uint16 gtmenu_font_code_encode(uint16 code)
{
    uint32 high, low, base;
    FUNCTION_MARKER(0x80034D28, "GTMENU.EXE");
    if (code == 0x2D7F) return 0x877F;
    high = code >> 8;
    low = code & 0xFF;
    base = ((high + 33) << 7) ^ 0xA000;
    if (high & 1) base += 32 - (low < 0x60);
    else base -= 2;
    return base + low;
}

uint32 gtmenu_race_info_return(void)
{
    uint32 category, page, callback, name;
    FUNCTION_MARKER(0x800282B0, "GTMENU.EXE");
    category = r_u16(0x80070A0C);
    page = r_u16(0x800A77C4);
    callback = r_u32(0x8009AC68);
    w_u16(0x800A77C0, page);
    w_u16(0x800A77C4, category);
    w_u32(0x800A77B4, callback);
    name = gtmenu_category_name((sint16)category);
    return gtmenu_page_open(name);
}

uint32 gtmenu_race_info_callback_restore(void)
{
    uint32 callback;
    FUNCTION_MARKER(0x8002830C, "GTMENU.EXE");
    callback = r_u32(0x8009AC68);
    w_u32(0x800A77B4, callback);
    return callback;
}

uint32 gtmenu_race_info_activate(void)
{
    FUNCTION_MARKER(0x80028328, "GTMENU.EXE");
    if (!gtmenu_string_compare(r_u32(0x800A7A5C), 0x8005518C)) {
        gtmenu_sound_effect(5);
        gtmenu_race_info_return();
        return 1;
    }
    if (gtmenu_ui_default_activate()) {
        gtmenu_race_info_callback_restore();
        return 1;
    }
    gtmenu_sound_effect(6);
    return 0;
}

uint32 gtmenu_race_info_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 offset, first, second, third, fourth, category, sequence = 0, prizes = 0;
    uint32 index, y, text, price, result;
    sint32 width, centered, label;
    FUNCTION_MARKER(0x800283A8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(256, 0x800283A8);
    for (offset = 0; offset < 80; offset += 16) {
        first = r_u32(0x80055370 + offset);
        second = r_u32(0x80055374 + offset);
        third = r_u32(0x80055378 + offset);
        fourth = r_u32(0x8005537C + offset);
        w_u32(frame.address + offset, first);
        w_u32(frame.address + offset + 4, second);
        w_u32(frame.address + offset + 8, third);
        w_u32(frame.address + offset + 12, fourth);
    }
    first = r_u32(0x800553C0);
    second = r_u32(0x800553C4);
    w_u32(frame.address + 80, first);
    w_u32(frame.address + 84, second);
    w_u32(frame.address + 88, r_u32(0x800553C8));
    w_u32(frame.address + 96, r_u32(0x800553CC));
    first = r_u32(0x800553D0);
    second = r_s8(0x800553D4);
    w_u32(frame.address + 104, first);
    w_u8(frame.address + 108, second);
    first = r_u32(0x800553D8);
    second = r_s8(0x800553DC);
    third = r_s8(0x800553DD);
    w_u32(frame.address + 112, first);
    w_u8(frame.address + 116, second);
    w_u8(frame.address + 117, third);
    w_u8(frame.address + 118, r_s8(0x800553DE));
    first = r_u32(0x800553E0);
    second = r_s8(0x800553E4);
    third = r_s8(0x800553E5);
    w_u32(frame.address + 120, first);
    w_u8(frame.address + 124, second);
    w_u8(frame.address + 125, third);
    first = r_u32(0x800553E8);
    second = r_s8(0x800553EC);
    third = r_s8(0x800553ED);
    w_u32(frame.address + 128, first);
    w_u8(frame.address + 132, second);
    w_u8(frame.address + 133, third);
    w_u32(frame.address + 136, r_u32(0x800553F0));
    w_u32(frame.address + 144, r_u32(0x800553F4));
    w_u32(frame.address + 152, r_u32(0x800553F8));
    w_u32(frame.address + 160, r_u32(0x800553FC));
    first = r_u32(0x80055400);
    second = r_s8(0x80055404);
    third = r_s8(0x80055405);
    w_u32(frame.address + 168, first);
    w_u8(frame.address + 172, second);
    w_u8(frame.address + 173, third);
    category = r_u16(0x80070A0C);
    w_u32(frame.address + 176, r_u32(0x80055408));
    switch ((sint16)(category - 14)) {
    case 0: prizes = 0x80070A2C; sequence = frame.address + 88; break;
    case 1: prizes = 0x80070A4C; sequence = frame.address + 96; break;
    case 2: prizes = 0x80070A6C; sequence = frame.address + 104; break;
    case 3: prizes = 0x80070A8C; sequence = frame.address + 112; break;
    case 6: prizes = 0x80070B2C; sequence = frame.address + 120; break;
    case 7: prizes = 0x80070B0C; sequence = frame.address + 128; break;
    case 8: prizes = 0x80070B4C; sequence = frame.address + 136; break;
    case 9: prizes = 0x80070AAC; sequence = frame.address + 152; break;
    case 10: prizes = 0x80070ACC; sequence = frame.address + 160; break;
    case 11: prizes = 0x80070AEC; sequence = frame.address + 144; break;
    case 12: prizes = 0x80070B8C; sequence = frame.address + 176; break;
    case 13: case 14: case 15:
        prizes = 0x80070B6C;
        sequence = frame.address + 168;
        break;
    }
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(1);
    width = gtmenu_text_width(1, 0x800A6CBE);
    centered = (sint32)(640 - (uint32)width);
    gtmenu_text_draw(ordering, centered / 2, 86, 0x800A6CBE);
    gtmenu_text_color_set(0x6580);
    gtmenu_text_font_set(0);
    gtmenu_text_draw(ordering, 82, 150, 0x800A6CDA);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    index = 0;
    y = 170;
    if (r_s8(sequence) != -1) {
        do {
            gtmenu_format_integer(frame.address + 248, 0x8005540C, index + 1);
            label = r_s8(sequence + index);
            text = r_u32(frame.address + (uint32)label * 4);
            ++index;
            gtmenu_format_string(frame.address + 184, 0x80055410, text);
            gtmenu_text_draw(ordering, 82, y, frame.address + 248);
            gtmenu_text_draw(ordering, 112, y, frame.address + 184);
            label = r_s8(sequence + index);
            y += 20;
        } while (label != -1);
    }
    gtmenu_text_color_set(0x6580);
    gtmenu_text_font_set(0);
    gtmenu_text_draw(ordering, 402, 150, 0x800A6CEB);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    price = r_u32(prizes);
    gtmenu_price_draw(ordering, 602, 170, 1, price);
    gtmenu_text_color_set(0x6580);
    gtmenu_text_font_set(0);
    gtmenu_text_draw(ordering, 402, 190, 0x800A6D01);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    price = r_u32(prizes + 28);
    gtmenu_price_draw(ordering, 602, 210, 1, price);
    gtmenu_text_color_set(0x6580);
    gtmenu_text_font_set(0);
    gtmenu_text_draw(ordering, 402, 230, 0x800A6D17);
    y = 250;
    for (index = 1; index < 7; ++index) {
        gtmenu_text_color_set(0x808080);
        gtmenu_text_font_set(0);
        text = r_u32(0x80070A10 + index * 4);
        gtmenu_text_draw(ordering, 402, y, text);
        gtmenu_text_color_set(0x808080);
        gtmenu_text_font_set(0);
        price = r_u32(prizes + index * 4);
        gtmenu_price_draw(ordering, 602, y, 1, price);
        y += 20;
    }
    result = gtmenu_text_draw(ordering, 0, 0, 0x80055414);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_race_info_event(uint32 event)
{
    uint32 kind, buttons;
    FUNCTION_MARKER(0x800289A4, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_race_info_draw(r_u32(event + 4));
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind == 24) {
        if (r_u32(event + 12) & 0x9000) {
            gtmenu_sound_effect(3);
            return gtmenu_race_info_return();
        }
        buttons = r_u32(event + 12) & 0x6000;
        if (buttons) return gtmenu_race_info_activate();
        return buttons;
    }
    return 24;
}

uint32 gtmenu_race_info_open(sint16 category)
{
    uint32 callback, page;
    FUNCTION_MARKER(0x80028A4C, "GTMENU.EXE");
    callback = r_u32(0x800A77B4);
    w_u16(0x80070A0C, category);
    w_u32(0x8009AC68, callback);
    gtmenu_car_spec_filter_set(1);
    page = r_u16(0x800A77C4);
    w_u32(0x800A77B4, 0x800289A4);
    w_u16(0x800A77C4, 663);
    w_u16(0x800A77C0, page);
    return gtmenu_page_open(0x8005545C);
}

sint32 gtmenu_ui_selection_active(void)
{
    FUNCTION_MARKER(0x8001A8A8, "GTMENU.EXE");
    return r_s8(0x80058449);
}

uint32 gtmenu_controller_event_dispatch(uint32 input, uint32 mode)
{
    sint32 delay;
    uint32 x, y, held, pressed, moving, direction, buttons, callback, active = 0;
    FUNCTION_MARKER(0x80028B18, "GTMENU.EXE");
    delay = r_s8(0x80070BAC);
    if (delay) {
        w_u8(0x80070BAC, delay - 1);
        return 0;
    }
    if (r_u32(0x800566F4)) gtmenu_cursor_step();
    x = r_u16(0x800A7560);
    y = r_u16(0x800A7568);
    w_u32(0x8009AC70, mode);
    w_u16(0x8009AC7C, x);
    w_u16(0x8009AC7E, y);
    held = r_u16(input + 2);
    pressed = r_u16(input + 4);
    moving = r_u32(0x800566F4);
    buttons = (held & 0xF0) | (pressed & 0xF00);
    if (!moving && buttons) {
        if (!gtmenu_ui_selection_active() && !mode) {
            direction = gtmenu_input_direction(buttons);
            if (gtmenu_cursor_move(direction) >= 0) gtmenu_sound_effect(0);
        }
        active = 1;
        w_u32(0x8009AC6C, 27);
        w_u32(0x8009AC78, held);
    }
    buttons = r_u16(input + 4);
    if (buttons) {
        active = 1;
        w_u32(0x8009AC6C, 24);
        w_u32(0x8009AC78, buttons);
    } else {
        buttons = r_u16(input + 6);
        if (buttons) {
            active = 1;
            w_u32(0x8009AC6C, 25);
            w_u32(0x8009AC78, buttons);
        } else {
            buttons = r_u16(input + 4);
            buttons |= r_u16(input + 8);
            if (buttons) {
                active = 1;
                w_u32(0x8009AC6C, 26);
                w_u32(0x8009AC78, buttons);
            }
        }
    }
    if (active) {
        callback = r_u32(0x800A77B4);
        gtmenu_event_callback_invoke(callback, 0x8009AC6C);
    }
    return active;
}

uint32 gtmenu_pointer_history_store(uint32 x, uint32 y, uint32 by_field)
{
    uint32 offset;
    FUNCTION_MARKER(0x80028F94, "GTMENU.EXE");
    if (!by_field) {
        w_u16(0x800A7B80, x);
        w_u16(0x800A7B84, y);
        return 0x800A7B80;
    }
    offset = ((xport_gpu_status_read_missing(0x1F801814) >> 31) ^ 1) * 2;
    w_u16(0x800A7B80 + offset, x);
    w_u16(0x800A7B84 + offset, y);
    return offset;
}

uint32 gtmenu_pointer_frame_draw(uint32 ordering, sint16 x, sint16 y, uint32 pointer, uint32 enabled)
{
    sint32 field = -1;
    uint32 page, current, offset, result;
    sint16 saved_x, saved_y;
    FUNCTION_MARKER(0x80028D40, "GTMENU.EXE");
    if (enabled) field = (xport_gpu_status_read_missing(0x1F801814) >> 31) ^ 1;
    if (!r_u8(0x800A7B89) && pointer) {
        gtmenu_pointer_background_capture(ordering, x, y, field);
        w_u16(0x80070BB2, x);
        w_u16(0x80070BB0, x);
        w_u16(0x80070BB6, y);
        w_u16(0x80070BB4, y);
        return 0x80070BB4;
    }
    if (r_u8(0x800A7B88)) {
        page = r_u16(0x800A77C4);
        if (page != 655 && page != 659 && !gtmenu_ui_selection_active()) {
            page = r_u16(0x800A77C4);
            if (page != 46) {
                current = r_u16(0x800A77C4);
                if (current - 4 >= 2 && page != 6)
                    gtmenu_pointer_pulse_draw(ordering, x, y);
            }
        }
    }
    if (pointer) gtmenu_pointer_background_capture(ordering, x, y, field);
    gtmenu_page_widgets_draw(ordering);
    if (r_s8(0x800A7B61)) gtmenu_background_draw(0);
    if (r_u8(0x800A7B88) && pointer) {
        offset = field < 0 ? 0 : (uint32)field * 2;
        saved_x = r_s16(0x80070BB0 + offset);
        saved_y = r_s16(0x80070BB4 + offset);
        result = gtmenu_pointer_background_restore(ordering, saved_x, saved_y, field);
    } else {
        result = 1;
        w_u8(0x800A7B88, 1);
    }
    if (pointer) {
        result = (uint32)field * 2;
        offset = field < 0 ? 0 : result;
        w_u16(0x80070BB0 + offset, x);
        w_u16(0x80070BB4 + offset, y);
    }
    return result;
}

uint32 gtmenu_layout_text(uint32 ordering, uint32 node_handle)
{
    uint32 node, product, font_y, index = 0, text, color, flags, value, byte, code;
    uint32 current_x, current_y, left, position, size, reference, saved_y, saved_left;
    uint16 line_height, start_x, start_y, width, height;
    sint32 font_width, half_width, half_extent, limit;
    FUNCTION_MARKER(0x80028FEC, "GTMENU.EXE");
    node = r_u32(node_handle);
    font_width = r_s16(0x800A7A1C);
    product = (uint32)font_width * r_u32(node + 16);
    font_y = r_u16(0x800A7A1E);
    line_height = r_u16(0x800A7A20) + font_y;
    r_u32(0x8005546C);
    r_u32(0x80055470);
    node = r_u32(node_handle);
    color = r_u32(0x800A7A00);
    half_extent = (sint32)product / 2;
    value = r_u32(node + 4);
    if ((sint32)value < 0) return value;
    w_u32(node + 4, value | 0x80000000);
    reference = r_u32(0x800A7A3C);
    if (r_u32(reference) == 0x8001) gtmenu_layout_vertical_align();
    value = gtmenu_layout_horizontal_align(half_extent);
    current_y = r_u16(0x800A79D6);
    flags = r_u32(0x800A79DC);
    w_u16(0x800A79D4, value);
    start_x = value;
    start_y = current_y;
    if (flags & 0x100) {
        value = r_u32(0x800A79F4);
        w_u32(0x800A7A00, value);
    }
    node = r_u32(node_handle);
    text = r_u32(node + 12);
    if (r_u8(text)) {
        half_width = font_width / 2;
        do {
            text = r_u32(node + 12) + index;
            byte = r_u8(text);
            if (byte & 0x80) {
                byte = r_u8(text);
                code = r_u8(text + 1) | (byte << 8);
                code = gtmenu_font_code_encode(code & 0x7F7F);
                size = (font_width & 0xFFFF) | (font_y << 16);
                gtmenu_layout_glyph_outline(code, ordering, size, 1);
                current_x = r_u16(0x800A79D4);
                index += 2;
                value = (uint32)font_width + current_x;
            } else {
                current_x = r_u16(0x800A79D4);
                ++index;
                value = current_x + (uint32)half_width;
            }
            w_u16(0x800A79D4, value);
            current_x = r_u16(0x800A79D4);
            limit = r_s16(0x800A7A26);
            if ((sint32)current_x + font_width >= limit) {
                current_y = r_u16(0x800A79D6);
                current_x = r_u16(0x800A79D4);
                value = current_y + line_height;
                w_u16(0x800A79D6, value);
                height = value - start_y;
                width = current_x - start_x;
                gtmenu_layout_float_expire();
                flags = r_u32(0x800A79DC);
                left = r_u16(0x800A7A24);
                w_u16(0x800A79D4, left);
                position = start_x | ((uint32)start_y << 16);
                size = width | ((uint32)height << 16);
                if (flags & 0x100) {
                    gtmenu_layout_underline_draw(ordering, position, size);
                    reference = r_u32(0x800A7A38);
                    gtmenu_layout_link_region_add(position, size, reference);
                }
                if (r_u32(0x800A79DC) & 0x40000)
                    gtmenu_layout_underline_draw(ordering, position, size);
                product = index * (uint32)font_width;
                value = gtmenu_layout_horizontal_align((sint32)((uint32)half_extent - product));
                flags = r_u32(0x800A79DC);
                w_u16(0x800A79D4, value);
                start_x = value;
                if (flags & 0xC0) {
                    start_y = r_u16(0x800A79D6);
                } else {
                    flags = r_u32(0x800A79DC);
                    if (flags & 0x38) {
                        saved_y = r_u16(0x800A7A46);
                        saved_left = r_u16(0x800A7598);
                        w_u32(0x800A79DC, flags & 0xFFFFFFC7);
                        w_u16(0x800A79D6, saved_y);
                        start_y = saved_y;
                        w_u16(0x800A7A24, saved_left);
                        w_u16(0x800A79D4, saved_left);
                        start_x = saved_left;
                    }
                }
                current_y = r_u16(0x800A79D6);
                limit = r_s16(0x800A7A2A);
                if ((sint32)current_y >= limit) break;
            }
            node = r_u32(node_handle);
            text = r_u32(node + 12);
        } while (r_u8(text + index));
    }
    current_x = r_u16(0x800A79D4);
    current_y = r_u16(0x800A79D6);
    width = current_x - start_x;
    height = current_y - start_y;
    if (!height) height = line_height;
    position = start_x | ((uint32)start_y << 16);
    size = width | ((uint32)height << 16);
    if (r_u32(0x800A79DC) & 0x100) {
        gtmenu_layout_underline_draw(ordering, position, size);
        reference = r_u32(0x800A7A38);
        w_u32(0x800A7A00, color);
        gtmenu_layout_link_region_add(position, size, reference);
    }
    if (r_u32(0x800A79DC) & 0x40000)
        gtmenu_layout_underline_draw(ordering, position, size);
    reference = r_u32(0x800A7A3C);
    if (r_u32(reference) == 0x8001) {
        reference = r_u32(0x800A7A40);
        if (r_u32(reference + 4) & 0x40) {
            value = r_u16(0x800A759A);
            w_u16(0x800A7A26, value);
        }
    }
    flags = r_u32(0x800A79DC);
    w_u32(0x800A79DC, flags & 0xFFFFFFDF);
    return 0x800A79DC;
}

uint32 gtmenu_layout_vertical_align(void)
{
    uint32 owner, flags, extent, font_y, current_y, value;
    FUNCTION_MARKER(0x8002B1AC, "GTMENU.EXE");
    owner = r_u32(0x800A7A40);
    flags = r_u32(owner + 4);
    if (flags & 0x20) {
        extent = r_u16(0x800A7A6A);
        font_y = r_u16(0x800A7A1E);
        current_y = r_u16(0x800A79D6);
        value = extent - font_y;
        w_u16(0x800A79D6, current_y + value);
        return value;
    }
    flags = r_u32(owner + 4);
    if (flags & 8) {
        extent = r_u16(0x800A7A6A);
        font_y = r_u16(0x800A7A1E);
        current_y = r_u16(0x800A79D6);
        value = current_y + (extent >> 1) - font_y;
        w_u16(0x800A79D6, value);
        return value;
    }
    flags = r_u32(owner + 4);
    if (!(flags & 0x80)) return 0;
    value = r_u16(0x800A7A44);
    w_u16(0x800A79D4, value);
    w_u16(0x800A7A24, value);
    return 0x800A7A24;
}

uint32 gtmenu_layout_horizontal_align(sint32 extent)
{
    uint32 flags, current;
    sint32 left, right, centered;
    FUNCTION_MARKER(0x8002B25C, "GTMENU.EXE");
    flags = r_u32(0x800A79DC);
    if (flags & 1) {
        right = r_s16(0x800A7A26);
        left = r_s16(0x800A7A24);
        if (extent < right - left) {
            centered = (sint32)((uint32)(left + right) - (uint32)extent);
            return centered / 2;
        }
    } else {
        current = r_u16(0x800A79D4);
        right = r_s16(0x800A7A26);
        if ((sint32)current < right) return current;
    }
    return r_s16(0x800A7A24);
}

uint32 gtmenu_layout_glyph_outline(uint16 code, uint32 ordering, uint32 size, uint32 shade)
{
    uint32 current_x, current_y, flags, packed_x, packed_y, position;
    sint32 x, y;
    FUNCTION_MARKER(0x8002B2D8, "GTMENU.EXE");
    current_x = r_u16(0x800A79D4);
    current_y = r_u16(0x800A79D6);
    flags = r_u32(0x800A79DC);
    if (flags & 0x10000) shade = 0;
    x = (sint16)(current_x + 1);
    y = (sint16)(current_y + 1);
    packed_x = (uint16)x;
    packed_y = (uint32)y << 16;
    gtmenu_layout_glyph_draw(code, ordering, packed_x | packed_y, size, 0);
    position = packed_x | ((uint32)(y - 1) << 16);
    gtmenu_layout_glyph_draw(code, ordering, position, size, shade);
    position = packed_x | ((uint32)(y + 1) << 16);
    gtmenu_layout_glyph_draw(code, ordering, position, size, shade);
    position = (uint16)(x - 1) | packed_y;
    gtmenu_layout_glyph_draw(code, ordering, position, size, shade);
    position = (uint16)(x + 1) | packed_y;
    return gtmenu_layout_glyph_draw(code, ordering, position, size, shade);
}

uint32 gtmenu_layout_float_expire(void)
{
    uint32 y, bottom, flags, left, right;
    FUNCTION_MARKER(0x8002B41C, "GTMENU.EXE");
    y = r_u16(0x800A79D6);
    bottom = r_u16(0x800A7A46);
    if (y < bottom) return 1;
    flags = r_u32(0x800A79DC);
    if (!(flags & 0xC0)) return 0xFFFFFF3F;
    left = r_u16(0x800A7598);
    right = r_u16(0x800A759A);
    flags &= 0xFFFFFF3F;
    w_u32(0x800A79DC, flags);
    w_u16(0x800A7A24, left);
    w_u16(0x800A7A26, right);
    return flags;
}

uint32 gtmenu_layout_underline_draw(uint32 ordering, uint32 position, uint32 size)
{
    uint32 line, strip, color, link;
    FUNCTION_MARKER(0x8002B478, "GTMENU.EXE");
    line = r_u32(0x800A8434);
    color = r_u32(0x800A7A00);
    w_u32(0x800A8434, line + 16);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, line);
    w_u8(ordering + 1, line >> 8);
    w_u8(ordering + 2, line >> 16);
    w_u8(line, link);
    w_u8(line + 1, link >> 8);
    w_u8(line + 2, link >> 16);
    w_u8(line + 3, 3);
    w_u32(line + 4, color | 0x40000000);
    w_u16(line + 8, (position & 0xFFFF) + 1);
    w_u16(line + 10, (position >> 16) + (size >> 16) - 2);
    strip = r_u32(0x800A8434);
    w_u16(line + 12, (position & 0xFFFF) + (size & 0xFFFF) - 2);
    w_u32(0x800A8434, strip + 16);
    w_u16(line + 14, (position >> 16) + (size >> 16) - 2);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, strip);
    w_u8(ordering + 1, strip >> 8);
    w_u8(ordering + 2, strip >> 16);
    w_u8(strip, link);
    w_u8(strip + 1, link >> 8);
    w_u8(strip + 2, link >> 16);
    w_u8(strip + 3, 3);
    w_u32(strip + 4, color | 0x62000000);
    w_u16(strip + 8, position);
    w_u16(strip + 10, (position >> 16) + (size >> 16) - 3);
    w_u16(strip + 14, 3);
    w_u16(strip + 12, size);
    return 3;
}

uint32 gtmenu_layout_link_region_add(uint32 position, uint32 size, uint32 reference)
{
    uint32 region, text;
    FUNCTION_MARKER(0x8002B57C, "GTMENU.EXE");
    position -= 0x10000;
    region = gtmenu_link_region_create();
    w_u32(region + 40, position);
    w_u32(region + 44, size);
    text = r_u32(reference);
    gtmenu_string_copy(region + 8, text + 12);
    w_u32(region + 4, 0);
    return gtmenu_link_region_append(region);
}

uint32 gtmenu_link_region_create(void)
{
    uint32 region;
    FUNCTION_MARKER(0x8002CB58, "GTMENU.EXE");
    region = gtmenu_heap_alloc(48);
    gtmenu_memory_fill(region, 0, 48);
    return region;
}

uint32 gtmenu_link_region_append(uint32 region)
{
    uint32 tail;
    FUNCTION_MARKER(0x8002CB94, "GTMENU.EXE");
    tail = r_u32(0x800A7A0C);
    if (tail) {
        tail = r_u32(0x800A7A0C);
        w_u32(tail + 4, region);
        w_u32(0x800A7A0C, region);
        return tail;
    }
    w_u32(0x800A7A0C, region);
    w_u32(0x800A7A08, region);
    return 0x800A7A08;
}

uint32 gtmenu_layout_image(uint32 ordering, uint32 widget)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 first, second, flags, link = 0, result = 0x80000000;
    uint32 x, y, width, height, clut_x, clut_y, pixel_x, pixel_y, descriptor, current;
    sint32 right, left, available;
    FUNCTION_MARKER(0x800294F8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(40, 0x800294F8);
    right = r_s16(0x800A7A26);
    left = r_s16(0x800A7A24);
    first = r_u32(0x80055474);
    second = r_u32(0x80055478);
    w_u32(frame.address, first);
    w_u32(frame.address + 4, second);
    flags = r_u32(widget + 4);
    available = right - left;
    if ((sint32)flags < 0) goto done;
    flags |= 0x80000000;
    w_u32(widget + 4, flags);
    if (flags & 0x1000000) {
        if (flags & 0x100000) {
            w_u16(frame.address + 8, 0);
            w_u16(frame.address + 10, 0);
            width = r_u16(0x800A7590);
            w_u16(frame.address + 14, 480);
            w_u16(frame.address + 12, width);
            result = gtmenu_bs_image_upload(widget + 12, frame.address + 8);
            if (result) goto done;
        } else if (!(r_u32(widget + 4) & 0x10000)) {
            flags = r_u32(widget + 4);
            result = 655;
            if (flags & 0x2000000) {
                y = r_u16(0x800A77C4) == 655 ? 0 : r_u16(0x800A7596);
                w_u16(frame.address + 2, y);
                x = r_u16(frame.address);
                width = r_u16(0x800A7590);
                w_u16(frame.address + 4, width);
                height = r_u16(0x800A7592);
                height = (height - y) & 0xFFFF;
                w_u16(frame.address + 6, height);
                current = r_u32(widget + 76);
                result = gtmenu_layout_color_fill(ordering, current, x | (y << 16), width | (height << 16));
            }
            goto done;
        }
        result = gtmenu_named_action_find(0x800A7644);
        if (result) {
            w_u16(result + 4, 0);
            w_u16(result + 6, 0);
        }
        goto done;
    }
    if (widget + 44) link = gtmenu_named_action_find(widget + 44);
    if (r_u32(widget + 4) & 0x10000) {
        clut_x = r_u16(0x800A75F4);
        clut_y = r_u16(0x800A75F6);
        pixel_x = r_u16(0x800A75F0);
        pixel_y = r_u16(0x800A75F2);
        descriptor = frame.address + 16;
        gtmenu_layout_tim_prepare(descriptor, widget, clut_x | (clut_y << 16), pixel_x | (pixel_y << 16));
        width = r_u32(descriptor) & 0xFFFF;
        if ((sint32)width < available) w_u16(0x800A79D4, (available - (sint32)width) / 2);
        else {
            x = r_u16(0x800A7A24);
            w_u16(0x800A79D4, x);
        }
        current = gtmenu_current_ordering();
        gtmenu_layout_image_sprite_draw(descriptor, current, widget);
        result = r_u32(descriptor);
        if (link) w_u32(link + 4, result);
    } else {
        result = r_u32(widget + 4) & 0x100000;
        if (result) {
            x = r_u16(0x800A79D4);
            y = r_u16(0x800A79D6);
            w_u16(frame.address + 8, x);
            w_u16(frame.address + 10, y);
            if (link) {
                w_u16(link + 4, x);
                y = r_u16(frame.address + 10);
                w_u16(link + 6, y);
            }
            width = r_u16(0x800A7590);
            w_u16(frame.address + 14, 480);
            w_u16(frame.address + 12, width);
            result = gtmenu_bs_image_upload(widget + 12, frame.address + 8);
        }
    }
done:
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_named_action_find(uint32 name)
{
    uint32 action;
    FUNCTION_MARKER(0x8002B8A4, "GTMENU.EXE");
    action = r_u32(0x800A7A10);
    while (action) {
        if (!gtmenu_string_compare(action + 8, name)) break;
        action = r_u32(action);
    }
    return action;
}

uint32 gtmenu_layout_color_fill(uint32 ordering, uint32 color, uint32 position, uint32 size)
{
    uint32 packet, link;
    FUNCTION_MARKER(0x8002B900, "GTMENU.EXE");
    packet = r_u32(0x800A8434);
    w_u32(0x800A8434, packet + 16);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u8(packet + 3, 3);
    w_u32(packet + 4, color | 0x60000000);
    w_u16(packet + 8, position);
    w_u16(packet + 10, position >> 16);
    w_u16(packet + 12, size);
    w_u16(packet + 14, size >> 16);
    return size >> 16;
}

uint32 gtmenu_layout_table(uint32 ordering, uint32 node_handle)
{
    uint32 table, padding, spacing, border, saved_left, flags, packed, value;
    uint32 cell, next, column = 0, row = 0, x, y, saved_x, saved_y, cell_height;
    uint32 row_height, column_width, color, font_y, extra, caption_height;
    sint32 font_width, count, delta;
    FUNCTION_MARKER(0x80029810, "GTMENU.EXE");
    table = r_u32(node_handle);
    padding = r_u16(table + 20);
    spacing = r_u16(table + 22);
    border = r_u16(table + 18);
    value = r_u32(table + 4);
    saved_left = r_u16(0x800A7A24);
    if ((sint32)value < 0) return value;
    w_u32(table + 4, value | 0x80000000);
    flags = r_u32(0x800A79DC);
    packed = r_u32(0x800A79D4);
    w_u32(0x800A79DC, flags | 0x200);
    w_u32(0x800A79D8, packed);
    gtmenu_layout_table_measure(node_handle);
    table = r_u32(node_handle);
    if (r_u32(table + 4) & 0x800) gtmenu_layout_table_border_draw(ordering, node_handle);
    table = r_u32(node_handle);
    if (r_u32(table + 12)) {
        font_width = r_s16(0x800A7A1C);
        count = r_s16(table + 16);
        value = r_u16(table + 8);
        delta = ((sint32)value - count * (font_width / 2)) / 2;
        x = r_u16(0x800A79D4);
        w_u16(0x800A79D4, x + (uint32)delta);
        table = r_u32(node_handle);
        if (r_u32(table + 4) & 0x20) {
            value = r_u16(table + 10);
            y = r_u16(0x800A79D6);
            delta = (sint16)padding * 2;
            w_u16(0x800A79D6, y + value + (uint32)delta);
        }
        gtmenu_layout_text(ordering, node_handle);
        table = r_u32(node_handle);
        if (r_u32(table + 4) & 0x20) {
            y = r_u16(0x800A79DA);
            w_u16(0x800A79D6, y);
        }
    }
    value = padding + border + spacing;
    x = r_u16(0x800A7A24);
    y = r_u16(0x800A79D6);
    w_u16(0x800A79D4, x + value);
    w_u16(0x800A79D6, y + value);
    packed = r_u32(0x800A79D4);
    w_u32(0x800A79D8, packed);
    table = r_u32(node_handle);
    cell = r_u32(table + 112);
    next = r_u32(cell + 32);
    while (cell) {
        saved_x = r_u16(0x800A79D4);
        saved_y = r_u16(0x800A79D6);
        flags = r_u32(cell + 4);
        packed = r_u32(0x800A79D4);
        if (flags & 0x8000) {
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags | 0x10000);
        } else {
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags & 0xFFFEFFFF);
        }
        x = r_u16(0x800A79D4);
        w_u16(0x800A79D4, x + padding);
        w_u16(0x800A7A24, x + padding);
        table = r_u32(node_handle);
        cell_height = r_u16(cell + 10);
        row_height = r_u16(table + row * 4 + 34);
        delta = ((sint32)row_height - (sint32)cell_height) / 2;
        y = r_u16(0x800A79D6);
        w_u16(0x800A79D6, y + (uint32)delta);
        gtmenu_layout_nodes_process(ordering, cell + 24);
        if (r_u32(cell + 4) & 0x10000) {
            table = r_u32(node_handle);
            column_width = r_u16(table + column * 4 + 32);
            table = r_u32(node_handle);
            row_height = r_u16(table + row * 4 + 34);
            color = r_u32(cell + 20);
            gtmenu_layout_color_fill(ordering, color, saved_x | (saved_y << 16), column_width | (row_height << 16));
        }
        table = r_u32(node_handle);
        column_width = r_u16(table + column * 4 + 32);
        y = packed >> 16;
        w_u16(0x800A79D6, y);
        w_u16(0x800A79D4, packed + column_width + spacing);
        table = r_u32(node_handle);
        count = r_s16(table + 26);
        ++column;
        cell = next;
        if ((sint32)column >= count) {
            x = r_u16(0x800A79D8);
            column = 0;
            w_u16(0x800A79D4, x);
            table = r_u32(node_handle);
            row_height = r_u16(table + row * 4 + 34);
            ++row;
            w_u16(0x800A79D6, y + row_height + spacing);
        }
        next = r_u32(cell + 32);
    }
    table = r_u32(node_handle);
    value = r_u32(table + 12);
    if (value) {
        value = r_u32(cell + 4) & 0x20;
        if (value) {
            y = r_u16(0x800A79DA);
            caption_height = r_u16(table + 10);
            font_y = r_u16(0x800A7A1E);
            extra = r_u16(0x800A7A20);
            value = font_y + extra;
            w_u16(0x800A79D6, y + caption_height + value);
        }
    }
    w_u16(0x800A7A24, saved_left);
    return value;
}

uint32 gtmenu_layout_rule(uint32 ordering, uint32 widget)
{
    uint32 left, right, flags, color, current_x, y, font_y, width, height, packet, link;
    sint32 centered;
    uint16 x, draw_y;
    FUNCTION_MARKER(0x80029C28, "GTMENU.EXE");
    left = r_u16(0x800A7A24);
    flags = r_u32(widget + 4);
    right = r_u16(0x800A7A26);
    if ((sint32)flags < 0) return 0x80000000;
    flags |= 0x80000000;
    color = flags & 0x1000000 ? 0 : 0x808080;
    w_u32(widget + 4, flags);
    current_x = r_u16(0x800A79D4);
    if ((sint32)current_x != (sint16)left) {
        y = r_u16(0x800A79D6);
        font_y = r_u16(0x800A7A1E);
        w_u16(0x800A79D4, left);
        w_u16(0x800A79D6, y + font_y);
    }
    width = r_u16(widget + 8);
    centered = ((sint16)right - (sint16)left - (sint16)width) / 2;
    height = r_u16(widget + 10);
    y = r_u16(0x800A79D6);
    x = left + (uint32)centered;
    packet = r_u32(0x800A8434);
    y += 4;
    w_u16(0x800A79D6, y);
    draw_y = y;
    w_u32(0x800A8434, packet + 16);
    w_u16(0x800A79D6, y + height + 4);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u8(packet + 3, 3);
    w_u32(packet + 4, color | 0x62000000);
    w_u16(packet + 8, x);
    w_u16(packet + 10, draw_y);
    w_u16(packet + 12, width);
    w_u16(packet + 14, height);
    return height;
}

void gtmenu_navigation_owner_release(uint32 owner)
{
    uint32 node, next, kind;
    FUNCTION_MARKER(0x8002A39C, "GTMENU.EXE");
    node = r_u32(owner + 40);
    while (node) {
        kind = r_u32(node + 36);
        next = r_u32(node);
        if (kind == 3) gtmenu_heap_free(r_u32(node + 44));
        gtmenu_heap_free(node);
        node = next;
    }
}

uint32 gtmenu_layout_tag(uint32 widget)
{
    uint32 tag, code, left, font_width, line_height, font_y, spacing, flags;
    uint32 x, y, bottom, right, reference, result, source, new_width, new_height;
    sint32 delta;
    FUNCTION_MARKER(0x80029D74, "GTMENU.EXE");
    tag = r_u8(widget);
    left = r_u16(0x800A7A24);
    font_y = r_u16(0x800A7A1E);
    spacing = r_u16(0x800A7A20);
    font_width = r_u16(0x800A7A1C);
    tag -= 8;
    line_height = font_y + spacing;
    flags = r_u32(widget + 4);
    if ((sint32)flags < 0) return 0x800A7A1C;
    w_u32(widget + 4, flags | 0x80000000);
    code = tag & 0x7F;
    if (tag & 0x80) {
        if (code - 1 >= 74) return (code - 1) * 4;
        switch (code) {
        case 1:
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags & 0xFFFFFEFF);
            return 0x800A79DC;
        case 2: case 3: case 4: case 6: case 40: case 54: case 74:
            flags = r_u32(0x800A79DC) & 0xFFFDFFFF;
            w_u32(0x800A79DC, flags);
            return flags;
        case 7: case 17: case 61:
            flags = r_u32(0x800A79DC) & 0xFFFEFFFF;
            w_u32(0x800A79DC, flags);
            return flags;
        case 16:
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags & 0xFFFFFFFE);
            x = r_u16(0x800A79D4);
            result = (sint16)left;
            if ((sint32)x != (sint16)left) {
                reference = r_u32(0x800A7A3C);
                if (r_u32(reference) == 0x8001) {
                    result = r_u16(0x800A7A46);
                    w_u16(0x800A79D6, result);
                } else {
                    y = r_u16(0x800A79D6);
                    result = y + line_height;
                    w_u16(0x800A79D6, result);
                }
            }
            w_u16(0x800A79D4, left);
            return result;
        case 31: case 32: case 33: case 34: case 35: case 36:
            new_width = r_u16(0x8009AC84);
            new_height = r_u16(0x8009AC86);
            w_u16(0x800A7A1C, new_width);
            w_u16(0x800A7A1E, new_height);
            return 0x800A7A1C;
        case 47:
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags & 0xFFFFEFFF);
            return 0x800A79DC;
        case 64:
            flags = r_u32(0x800A79DC);
            w_u32(0x800A79DC, flags & 0xFFFFFDFF);
            return 0x800A79DC;
        case 72:
            flags = r_u32(0x800A79DC) & 0xFFFBFFFF;
            w_u32(0x800A79DC, flags);
            return flags;
        default:
            return 0x8002A38C;
        }
    }
    switch (code) {
    case 2: case 3: case 4: case 6: case 40: case 54: case 74:
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags | 0x20000);
        return 0x800A79DC;
    case 7: case 17: case 61:
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags | 0x10000);
        return 0x800A79DC;
    case 16:
        flags = r_u32(0x800A79DC) | 1;
        w_u32(0x800A79DC, flags);
        return flags;
    case 72:
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags | 0x40000);
        return 0x800A79DC;
    case 14: case 53:
        reference = r_u32(0x800A7A3C);
        if (r_u32(reference) == 0x8001) {
            y = r_u16(0x800A7A46);
            w_u16(0x800A79D6, y);
        } else {
            if (code == 53) {
                x = r_u16(0x800A79D4);
                if ((sint32)x != (sint16)left) delta = (sint16)line_height * 2;
                else delta = line_height;
            } else delta = line_height;
            y = r_u16(0x800A79D6);
            w_u16(0x800A79D6, y + (uint32)delta);
        }
        result = gtmenu_layout_float_expire();
        w_u16(0x800A79D4, left);
        return result;
    case 25: case 20:
        y = r_u16(0x800A79D6);
        bottom = r_u16(0x800A7A46);
        if (bottom < y) {
            x = r_u16(0x800A79D4);
            result = (sint16)left;
            if ((sint32)x != (sint16)left) {
                y = r_u16(0x800A79D6);
                result = y + line_height;
                w_u16(0x800A79D6, result);
            }
        } else {
            bottom = r_u16(0x800A7A46);
            left = r_u16(0x800A7598);
            right = r_u16(0x800A759A);
            result = right;
            w_u16(0x800A79D6, bottom);
            w_u16(0x800A7A24, left);
            w_u16(0x800A7A26, right);
        }
        if (code == 20) result = left + (uint32)((sint16)font_width * 6);
        w_u16(0x800A79D4, code == 20 ? result : left);
        return result;
    case 31: source = 0x800A75A8; break;
    case 32: source = 0x800A75AC; break;
    case 33: source = 0x800A75B0; break;
    case 34: source = 0x800A75B4; break;
    case 35: source = 0x800A75B8; break;
    case 36: source = 0x800A75A0; break;
    case 70:
        reference = r_u32(0x800A7A40);
        result = r_u16(reference + 8);
        w_u16(0x800A7A24, result);
        return result;
    default:
        if (code < 7) return code < 2;
        if (code < 17) return 14;
        if (code < 26) return 20;
        if (code < 34) return 32;
        if (code < 55) return 53;
        if (code < 73) return 70;
        return 0x8006;
    }
    font_width = r_u16(0x800A7A1C);
    font_y = r_u16(0x800A7A1E);
    new_width = r_u16(source);
    new_height = r_u16(source + 2);
    w_u16(0x8009AC84, font_width);
    w_u16(0x8009AC86, font_y);
    w_u16(0x800A7A1C, new_width);
    w_u16(0x800A7A1E, new_height);
    return 0x8009AC84;
}

uint32 gtmenu_page_link_create(uint32 list, uint32 color, uint32 target, uint32 name)
{
    uint32 node, widget;
    FUNCTION_MARKER(0x8002A408, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(80);
    w_u32(node, widget);
    w_u32(widget, 0x8003);
    widget = r_u32(node);
    w_u32(widget + 4, 0);
    widget = r_u32(node);
    gtmenu_string_copy(widget + 12, target);
    widget = r_u32(node);
    gtmenu_string_copy(widget + 44, name);
    widget = r_u32(node);
    w_u32(widget + 76, color);
    w_u32(node + 4, 0);
    if (r_u32(0x800A79DC) & 0x200) list = r_u32(0x800A7A70) + 24;
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_page_link_parse(uint32 list, uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 tag, flags, cursor;
    FUNCTION_MARKER(0x800254AC, "GTMENU.EXE");
    tag = r_u8(input);
    cursor = input + 1;
    if (tag & 0x80) {
        gtmenu_page_tag_append(list, tag);
        return cursor;
    }
    frame = gtmenu_local_stack_push(136, 0x800254AC);
    flags = r_u8(cursor);
    ++cursor;
    if (flags & 1) cursor = gtmenu_stream_string_read(cursor, frame.address + 64, frame.address + 128);
    if (flags & 2) cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 128);
    gtmenu_page_link_create(list, 0, frame.address, frame.address + 64);
    /* Original bug fix: Automatic string buffers are not heap allocations */
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_layout_mode_record(uint32 name, uint32 mode, uint32 bounds)
{
    uint32 record, first, second, owner;
    FUNCTION_MARKER(0x8002A504, "GTMENU.EXE");
    record = gtmenu_heap_alloc(48);
    w_u32(record, 0);
    gtmenu_string_copy(record + 4, name);
    w_u32(record + 36, mode);
    first = r_u32(bounds);
    second = r_u32(bounds + 4);
    w_u32(record + 40, first);
    w_u32(record + 44, second);
    owner = r_u32(0x800A7A14);
    return gtmenu_layout_mode_append(owner, record);
}

uint32 gtmenu_pointer_widget_at(sint16 x, sint16 y)
{
    uint32 region, owner, record, origin, found = 0;
    uint32 left, top, width, height;
    FUNCTION_MARKER(0x8002AE14, "GTMENU.EXE");
    region = r_u32(0x800A7A08);
    while (region) {
        left = r_u16(region + 40);
        top = r_u16(region + 42);
        width = r_u16(region + 44);
        height = r_u16(region + 46);
        if (gtmenu_packed_rectangle_contains(left | (top << 16), width | (height << 16), x, y))
            return region + 8;
        region = r_u32(region + 4);
    }
    owner = r_u32(0x800A7A10);
    while (owner && !found) {
        record = r_u32(owner + 40);
        while (record && !found) {
            origin = r_u32(owner + 4);
            if (gtmenu_layout_mode_contains(record, origin, x, y)) found = record;
            record = r_u32(record);
        }
        owner = r_u32(owner);
    }
    return found ? found + 4 : 0;
}

uint32 gtmenu_layout_mode_append(uint32 owner, uint32 record)
{
    uint32 tail;
    FUNCTION_MARKER(0x8002C688, "GTMENU.EXE");
    tail = r_u32(owner + 44);
    if (tail) {
        w_u32(tail, record);
        w_u32(owner + 44, record);
    } else {
        w_u32(owner + 44, record);
        w_u32(owner + 40, record);
    }
    return tail;
}

uint32 gtmenu_packed_rectangle_contains(uint32 position, uint32 size, sint16 x, sint16 y)
{
    sint32 left, top, width, height;
    FUNCTION_MARKER(0x8002C7FC, "GTMENU.EXE");
    left = (sint16)position;
    if (x < left) return 0;
    width = (sint16)size;
    if (x >= left + width) return 0;
    top = (sint16)(position >> 16);
    if (y < top) return 0;
    height = (sint16)(size >> 16);
    return y < top + height;
}

uint32 gtmenu_layout_mode_contains(uint32 record, uint32 origin, sint16 x, sint16 y)
{
    uint32 kind, offset, radius, radius_square, distance;
    sint32 base_x, base_y, edge, dx, dy;
    FUNCTION_MARKER(0x8002C860, "GTMENU.EXE");
    kind = r_u32(record + 36);
    if (kind == 1) {
        base_x = origin & 0xFFFF;
        edge = base_x + r_u16(record + 40);
        if (x < edge) return 0;
        edge = base_x + r_u16(record + 44);
        if (x >= edge) return 0;
        base_y = origin >> 16;
        edge = base_y + r_u16(record + 42);
        if (y < edge) return 0;
        edge = base_y + r_u16(record + 46);
        return y < edge;
    }
    if (kind == 2) {
        offset = r_u16(record + 44);
        dx = (sint16)((uint32)x - (origin + offset));
        offset = r_u16(record + 46);
        dy = (sint16)((uint32)y - ((origin >> 16) + offset));
        radius = r_u16(record + 40);
        distance = (uint32)(dx * dx) + (uint32)(dy * dy);
        radius_square = radius * radius;
        return (sint32)radius_square >= (sint32)distance;
    }
    return 0;
}

uint32 gtmenu_pointer_background_capture(uint32 ordering, sint16 x, sint16 y, sint32 field)
{
    uint32 packet, link, row = 0, offset, value, even_y;
    FUNCTION_MARKER(0x8002AF50, "GTMENU.EXE");
    offset = (uint32)field * 2;
    even_y = (uint32)y & 0xFFFFFFFE;
    do {
        packet = r_u32(0x800A8434);
        w_u32(0x800A8434, packet + 20);
        link = r_u8(ordering);
        link |= (uint32)r_u8(ordering + 1) << 8;
        link |= (uint32)r_u8(ordering + 2) << 16;
        w_u8(ordering, packet);
        w_u8(ordering + 1, packet >> 8);
        w_u8(ordering + 2, packet >> 16);
        w_u8(packet, link);
        w_u8(packet + 1, link >> 8);
        w_u8(packet + 2, link >> 16);
        w_u8(packet + 3, 4);
        w_u32(packet + 4, 0x80000000);
        w_u16(packet + 8, x);
        if (field < 0) {
            w_u16(packet + 10, y);
            value = r_u16(0x800A7B78);
            w_u16(packet + 12, value);
            value = r_u16(0x800A7B7C);
            w_u16(packet + 16, 32);
            w_u16(packet + 18, 32);
            w_u16(packet + 14, value);
            return 32;
        }
        w_u16(packet + 10, even_y + row + (uint32)field);
        value = r_u16(0x800A7B78 + offset);
        w_u16(packet + 12, value);
        value = r_u16(0x800A7B7C + offset);
        w_u16(packet + 16, 32);
        w_u16(packet + 18, 1);
        w_u16(packet + 14, value + row + (uint32)field);
        row += 2;
    } while (row < 34);
    return 0;
}

uint32 gtmenu_pointer_background_restore(uint32 ordering, sint16 x, sint16 y, sint32 field)
{
    uint32 packet, link, row = 0, offset, value, even_y;
    FUNCTION_MARKER(0x8002B080, "GTMENU.EXE");
    offset = (uint32)field * 2;
    even_y = (uint32)y & 0xFFFFFFFE;
    do {
        packet = r_u32(0x800A8434);
        w_u32(0x800A8434, packet + 20);
        link = r_u8(ordering);
        link |= (uint32)r_u8(ordering + 1) << 8;
        link |= (uint32)r_u8(ordering + 2) << 16;
        w_u8(ordering, packet);
        w_u8(ordering + 1, packet >> 8);
        w_u8(ordering + 2, packet >> 16);
        w_u8(packet, link);
        w_u8(packet + 1, link >> 8);
        w_u8(packet + 2, link >> 16);
        w_u8(packet + 3, 4);
        w_u32(packet + 4, 0x80000000);
        if (field < 0) {
            value = r_u16(0x800A7B78);
            w_u16(packet + 8, value);
            value = r_u16(0x800A7B7C);
            w_u16(packet + 12, x);
            w_u16(packet + 14, y);
            w_u16(packet + 16, 32);
            w_u16(packet + 18, 32);
            w_u16(packet + 10, value);
            return 32;
        }
        value = r_u16(0x800A7B78 + offset);
        w_u16(packet + 8, value);
        value = r_u16(0x800A7B7C + offset);
        w_u16(packet + 12, x);
        w_u16(packet + 14, even_y + row + (uint32)field);
        w_u16(packet + 16, 32);
        w_u16(packet + 18, 1);
        w_u16(packet + 10, value + row + (uint32)field);
        row += 2;
    } while (row < 34);
    return 0;
}

void gtmenu_bs_input_complete(void)
{
    FUNCTION_MARKER(0x8002B674, "GTMENU.EXE");
    DecDCTinCallback(0);
    DecDCToutSync(0);
}

uint32 gtmenu_layout_tim_prepare(uint32 descriptor, uint32 widget, uint32 clut_position, uint32 pixel_position)
{
    uint32 image, header, block, flags, length, shift, width, height, size, state, u;
    FUNCTION_MARKER(0x8002B970, "GTMENU.EXE");
    image = gtmenu_page_resource_load(widget + 12);
    if (!image) {
        w_u32(descriptor, 0);
        return descriptor;
    }
    header = image + 4;
    flags = r_u32(header);
    block = image + 8;
    if (flags & 8) {
        clut_position = gtmenu_tim_block_upload(block, clut_position, 0xFFFFFFFF);
        length = r_u32(header + 4);
        block += length;
    }
    flags = r_u32(header);
    width = r_u16(block + 8);
    shift = (2 - (flags & 7)) & 31;
    width <<= shift;
    w_u16(widget + 8, width);
    height = r_u16(block + 10);
    size = (width & 0xFFFF) | (height << 16);
    w_u16(widget + 10, height);
    pixel_position = gtmenu_tim_block_upload(block, pixel_position, 0xFFFFFFFF);
    w_u16(0x800A7A62, 0);
    w_u16(0x800A7A60, 0);
    state = r_u32(0x800A7A60);
    state = (state & 0xFFFFFFC0) | (clut_position & 0x3F);
    state = (state & 0xFFFF803F) | ((clut_position >> 10) & 0x7FC0);
    state = (state & 0xFFF0FFFF) | (((pixel_position >> 6) & 15) << 16);
    state = (state & 0xFFEFFFFF) | (((pixel_position >> 24) & 1) << 20);
    state = (state & 0xFF9FFFFF) | 0x200000;
    w_u32(0x800A7A60, state);
    flags = r_u32(header);
    state = (state & 0xFE7FFFFF) | ((flags & 3) << 23);
    w_u32(0x800A7A60, state);
    flags = r_u32(header);
    w_u16(0x800A7A66, (pixel_position >> 16) & 0xFF);
    shift = (2 - (flags & 7)) & 31;
    u = (pixel_position & 0x3F) << shift;
    w_u16(0x800A7A64, u);
    gtmenu_heap_free(image);
    w_u32(descriptor, size);
    return descriptor;
}

uint32 gtmenu_bs_image_upload(uint32 name, uint32 rectangle)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 image, work, pixels, work_bytes, pixel_bytes, data, result = 1;
    uint16 x, y, height;
    sint32 words, count, width, rows, end, upload_x;
    FUNCTION_MARKER(0x8002B69C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002B69C);
    x = r_u16(rectangle);
    y = r_u16(rectangle + 2);
    height = r_u16(rectangle + 6);
    w_u16(frame.address, x);
    w_u16(frame.address + 2, y);
    w_u16(frame.address + 4, 16);
    w_u16(frame.address + 6, height);
    if (r_s16(rectangle + 4) < 16) {
        w_u16(rectangle + 4, 16);
        w_u16(frame.address + 4, 16);
    }
    if (r_s16(rectangle + 6) < 256) {
        w_u16(rectangle + 6, 256);
        w_u16(frame.address + 6, 256);
    }
    image = gtmenu_page_resource_load(name);
    if (!image) goto done;
    words = DecDCTBufSize((uint32 *)psx_addr(image, 4));
    work_bytes = (uint32)words * 4 + 256;
    work = gtmenu_heap_alloc(work_bytes);
    if (!work) {
        gtmenu_heap_free(image);
        result = 2;
        goto done;
    }
    width = r_s16(rectangle + 4);
    rows = r_s16(rectangle + 6);
    count = width * rows / 2;
    pixel_bytes = (uint32)count * 4;
    pixels = gtmenu_heap_alloc(pixel_bytes);
    if (!pixels) {
        gtmenu_heap_free(work);
        gtmenu_heap_free(image);
        result = 3;
        goto done;
    }
    DecDCTinCallback(gtmenu_bs_input_complete);
    gtmenu_bs_words_unpack(image, work);
    DecDCTReset(1);
    DecDCTin((uint32 *)psx_addr(work, work_bytes), 0);
    gtmenu_heap_free(image);
    DecDCTout((uint32 *)psx_addr(pixels, pixel_bytes), count);
    DecDCToutSync(0);
    end = r_s16(rectangle);
    end += r_s16(rectangle + 4);
    upload_x = r_s16(frame.address);
    data = pixels;
    while (upload_x < end) {
        gtmenu_gpu_load_image(frame.address, data);
        x = r_u16(frame.address);
        x += 16;
        w_u16(frame.address, x);
        upload_x = (sint16)x;
        rows = r_s16(rectangle + 6);
        data += (uint32)rows << 5;
    }
    gtmenu_gpu_wait(0);
    gtmenu_heap_free(pixels);
    gtmenu_heap_free(work);
    result = 0;
done:
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_layout_image_sprite_draw(uint32 descriptor, uint32 ordering, uint32 widget)
{
    uint32 packet, sprite, link, value, position, state;
    uint16 saved_x, saved_y, width, height, cursor_x, cursor_y;
    FUNCTION_MARKER(0x8002BFB0, "GTMENU.EXE");
    if (r_u32(widget + 4) & 0x40) {
        value = r_u16(0x800A7A26);
        value -= r_u16(widget + 8);
        w_u16(0x800A7A26, value);
        w_u16(0x800A79D4, value);
    }
    packet = r_u32(0x800A8434);
    w_u32(0x800A8434, packet + 20);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u8(packet + 3, 4);
    w_u32(packet + 4, 0x64808080);
    saved_x = r_u16(0x800A79D4);
    cursor_x = r_u16(0x800A79D4);
    w_u16(packet + 8, saved_x);
    cursor_y = r_u16(0x800A79D6);
    saved_y = r_u16(0x800A79D6);
    w_u16(packet + 10, saved_y);
    width = r_u16(widget + 8);
    w_u16(packet + 16, width);
    height = r_u16(widget + 10);
    w_u16(packet + 18, height);
    value = r_u8(0x800A7A64);
    w_u8(packet + 12, value);
    value = r_u8(0x800A7A66);
    position = cursor_x | ((uint32)cursor_y << 16);
    w_u8(packet + 13, value);
    value = r_u16(0x800A7A60);
    sprite = packet;
    packet = r_u32(0x800A8434);
    w_u16(sprite + 14, value);
    w_u32(0x800A8434, packet + 8);
    link = r_u8(ordering);
    link |= (uint32)r_u8(ordering + 1) << 8;
    link |= (uint32)r_u8(ordering + 2) << 16;
    w_u8(ordering, packet);
    w_u8(ordering + 1, packet >> 8);
    w_u8(ordering + 2, packet >> 16);
    w_u8(packet, link);
    w_u8(packet + 1, link >> 8);
    w_u8(packet + 2, link >> 16);
    w_u8(packet + 3, 1);
    value = r_u16(0x800A7A62);
    w_u32(packet + 4, value | 0xE1000200);
    if (r_u32(0x800A79DC) & 0x100) {
        value = r_u32(0x800A7A38);
        gtmenu_layout_link_region_add(saved_x | ((uint32)saved_y << 16), width | ((uint32)height << 16), value);
    }
    value = r_u16(0x800A79D4);
    state = r_u32(0x800A79DC);
    value += width;
    link = r_u16(widget + 4);
    w_u16(0x800A79D4, value);
    w_u16(0x800A7A44, value);
    w_u16(0x800A7A46, saved_y + height);
    w_u32(0x800A79DC, link | state);
    w_u32(descriptor, position);
    return descriptor;
}

uint32 gtmenu_layout_table_measure(uint32 node)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 table, cell, size, padded, slot, value, column = 0, row = 0, sum, index;
    uint16 spacing, columns, rows;
    sint32 padding, count, border, result;
    FUNCTION_MARKER(0x8002C180, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002C180);
    table = r_u32(node);
    padding = r_s16(table + 20) * 2;
    cell = r_u32(table + 112);
    spacing = r_u16(table + 22);
    columns = r_u16(table + 26);
    rows = r_u16(table + 28);
    while (cell) {
        value = r_u32(cell + 24);
        gtmenu_layout_list_measure(frame.address, value);
        size = r_u32(frame.address);
        padded = (size + (uint32)padding) & 0xFFFF;
        padded |= ((size >> 16) + (uint32)padding) << 16;
        w_u32(cell + 8, size);
        table = r_u32(node);
        slot = table + row * 4;
        value = r_u16(slot + 34);
        if (value < (padded >> 16)) w_u16(slot + 34, padded >> 16);
        table = r_u32(node);
        slot = table + column * 4;
        value = r_u16(slot + 32);
        if (value < (padded & 0xFFFF)) w_u16(slot + 32, padded);
        ++column;
        if ((sint32)column >= (sint16)columns) {
            column = 0;
            ++row;
        }
        cell = r_u32(cell + 32);
    }
    sum = 0;
    count = (sint16)columns;
    if (count > 0) {
        table = r_u32(node);
        for (index = 0; index < (uint32)count; ++index) sum += r_u16(table + index * 4 + 32);
    }
    value = (uint32)((sint16)spacing * (count + 1));
    table = r_u32(node);
    border = r_s16(table + 18) * 2;
    w_u16(table + 8, sum + value + (uint32)border);
    sum = 0;
    count = (sint16)rows;
    if (count > 0) {
        table = r_u32(node);
        for (index = 0; index < (uint32)count; ++index) sum += r_u16(table + index * 4 + 34);
    }
    value = (uint32)((sint16)spacing * (count + 1));
    table = r_u32(node);
    result = r_s16(table + 18) * 2;
    w_u16(table + 10, sum + value + (uint32)result);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_layout_table_border_draw(uint32 ordering, uint32 node)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 table, x, y, padding, spacing, border, width, height, left, top, right;
    uint32 inner_left, inner_top, outer_right, bottom, outer_bottom, value, other, index, result;
    uint16 saved_left, saved_top, saved_right;
    sint32 count;
    FUNCTION_MARKER(0x8002C3BC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(24, 0x8002C3BC);
    table = r_u32(node);
    x = r_u16(0x800A79D8);
    y = r_u16(0x800A79DA);
    padding = r_u16(table + 20);
    spacing = r_u16(table + 22);
    border = r_u16(table + 18);
    width = r_u16(table + 8);
    height = r_u16(table + 10);
    left = padding + x;
    top = padding + y;
    right = left + width - 1;
    outer_right = right + border;
    inner_top = border + top;
    saved_left = left;
    w_u16(frame.address, left);
    inner_left = border + left;
    w_u16(frame.address + 10, inner_top);
    saved_top = top;
    w_u16(frame.address + 2, top);
    bottom = top + height - 1;
    w_u16(frame.address + 4, outer_right);
    saved_right = right;
    w_u16(frame.address + 8, right);
    w_u16(frame.address + 12, inner_left);
    w_u16(frame.address + 14, inner_top);
    w_u16(frame.address + 6, saved_top);
    gtmenu_polygon_draw(ordering, frame.address, 4, 0);
    value = r_u32(frame.address + 12);
    other = r_u16(frame.address + 12);
    x = r_u16(frame.address);
    outer_bottom = bottom + border;
    w_u16(frame.address + 10, bottom);
    w_u16(frame.address + 14, outer_bottom);
    w_u32(frame.address + 4, value);
    w_u16(frame.address + 8, other);
    w_u16(frame.address + 12, x);
    gtmenu_polygon_draw(ordering, frame.address, 4, 0);
    value = r_u32(frame.address + 8);
    w_u32(frame.address, value);
    other = r_u16(frame.address + 2);
    w_u16(frame.address + 8, outer_right);
    w_u16(frame.address + 10, outer_bottom);
    w_u16(frame.address + 4, saved_right);
    w_u16(frame.address + 6, other);
    gtmenu_polygon_draw(ordering, frame.address, 4, 0);
    value = r_u32(frame.address + 4);
    w_u16(frame.address, saved_right);
    w_u16(frame.address + 2, inner_top);
    w_u16(frame.address + 4, outer_right);
    w_u32(frame.address + 12, value);
    w_u16(frame.address + 6, saved_top);
    gtmenu_polygon_draw(ordering, frame.address, 4, 0);
    w_u16(frame.address + 16, inner_left);
    w_u16(frame.address + 18, inner_top);
    w_u16(frame.address + 20, spacing + inner_left);
    w_u16(frame.address + 22, bottom);
    table = r_u32(node);
    count = r_s16(table + 26);
    index = 0;
    while ((sint32)index < count) {
        gtmenu_rectangle_draw(ordering, frame.address + 16, 0);
        table = r_u32(node);
        value = r_u16(table + index * 4 + 32);
        other = r_u16(frame.address + 16);
        value += spacing;
        value += other;
        w_u16(frame.address + 16, value);
        w_u16(frame.address + 20, spacing + value);
        table = r_u32(node);
        count = r_s16(table + 26);
        ++index;
    }
    gtmenu_rectangle_draw(ordering, frame.address + 16, 0);
    w_u16(frame.address + 16, saved_left + border);
    value = saved_top + border;
    w_u16(frame.address + 18, value);
    w_u16(frame.address + 22, spacing + value);
    w_u16(frame.address + 20, saved_right);
    table = r_u32(node);
    count = r_s16(table + 28);
    index = 0;
    while ((sint32)index < count) {
        gtmenu_rectangle_draw(ordering, frame.address + 16, 0);
        table = r_u32(node);
        value = r_u16(table + index * 4 + 34);
        other = r_u16(frame.address + 18);
        value += spacing;
        value += other;
        w_u16(frame.address + 18, value);
        w_u16(frame.address + 22, spacing + value);
        table = r_u32(node);
        count = r_s16(table + 28);
        ++index;
    }
    result = gtmenu_rectangle_draw(ordering, frame.address + 16, 0);
    gtmenu_local_stack_pop(frame);
    return result;
}

