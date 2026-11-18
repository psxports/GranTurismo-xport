#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gtmenu_model_info_open_id(uint16 id, sint32 category)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 models, archive, model, descriptor, unavailable, available;
    uint32 count, color_table, index, name, previous, first, second, result;
    sint32 model_index;
    sint8 code;
    FUNCTION_MARKER(0x80016C44, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80016C44);
    models = gtmenu_resource_group(13) + 24;
    archive = gtmenu_resource_archive();
    model = models + (uint32)(id & 0xFFF) * 424;
    w_u8(0x800806F0, 0);
    w_u32(0x800806F4, model);
    descriptor = r_u32(0x800806F4);
    unavailable = !r_u32(descriptor + 388);
    w_u8(0x800806FD, unavailable);
    w_u16(0x800806F2, id);
    w_u32(0x80058164, 0x800806E8);
    w_u16(0x80058160, category);
    gtmenu_memory_fill(0x80080718, 0, 128);
    w_u8(0x8005815D, 1);
    w_u8(0x8005815E, 0);
    gtmenu_memory_fill(0x80080B98, 0, 96);
    w_u16(0x80080B98, (id & 0xFFF) + 1);
    available = gtmenu_model_info_change_available(model);
    w_u8(0x8005815C, available);
    model_index = gtmenu_model_name_index(model);
    count = gtmenu_model_color_count(model_index);
    color_table = gtmenu_model_color_table(model_index);
    for (index = 0; index < count; ++index) {
        code = (sint8)gtmenu_model_color_code(index, color_table);
        name = gtmenu_model_color_name((id & 0xFFF) + 1, code, archive);
        gtmenu_string_copy(0x80080798 + index * 64, name);
    }
    gtmenu_model_info_selection_set(id >> 12);
    gtmenu_settings_from_record(0x800A7840, 0x80080B98, archive);
    gtmenu_model_stats_build(0x800A7840, frame.address, 0x800A77D0);
    previous = r_u16(0x800A77C4);
    w_u32(0x800A77B4, 0x800167DC);
    w_u16(0x800A77C4, 661);
    w_u16(0x800A77C0, previous);
    gtmenu_model_info_page_open(category);
    descriptor = r_u32(0x80058164);
    first = r_u32(0x80080BF8);
    second = r_u32(0x80080BFC);
    gtmenu_model_info_bind(descriptor, first, second, 0, 0);
    gtmenu_preview_control_set(0);
    result = gtmenu_model_logo_load(model);
    gtmenu_local_stack_pop(frame);
    return result;
}

void gtmenu_model_info_selection_set(sint16 selection)
{
    FUNCTION_MARKER(0x8001DBA4, "GTMENU.EXE");
    w_u16(0x80058638, selection);
}

uint32 gtmenu_model_color_table(uint32 index)
{
    FUNCTION_MARKER(0x800320A8, "GTMENU.EXE");
    return gtmenu_color_table_entry(r_u32(0x800A8878), index);
}

sint32 gtmenu_model_color_index(uint32 code, uint32 table)
{
    uint32 count, index = 0, entry;
    FUNCTION_MARKER(0x800320D4, "GTMENU.EXE");
    if (!r_u8(table)) return -1;
    code |= 0x20;
    count = r_u8(table);
    do {
        entry = table + index * 2;
        ++index;
        if (code == (uint32)r_s8(entry + 2)) return r_u8(entry + 1);
    } while (index < count);
    return -1;
}

sint32 gtmenu_model_color_code(uint32 index, uint32 table)
{
    uint32 count, current = 0, entry;
    FUNCTION_MARKER(0x8003212C, "GTMENU.EXE");
    if (!r_u8(table)) return 0;
    count = r_u8(table);
    do {
        entry = table + current * 2;
        ++current;
        if (index == r_u8(entry + 1)) return r_s8(entry + 2);
    } while (current < count);
    return 0;
}

uint32 gtmenu_model_info_bind(uint32 node, uint32 first, uint32 second, uint32 mode, uint32 extra)
{
    uint32 result;
    FUNCTION_MARKER(0x8001DCC8, "GTMENU.EXE");
    if (first & 1) first = (first & 0xFFFF0000) | ((first + 1) & 0xFFFF);
    w_u32(0x8008E8EC, first);
    w_u32(0x8008E8F0, second);
    result = gtmenu_model_preview_bind(node, mode, extra);
    w_u8(0x80058640, 1);
    return result;
}

void gtmenu_preview_control_set(uint32 mode)
{
    FUNCTION_MARKER(0x8001E3B8, "GTMENU.EXE");
    w_u32(0x80058644, mode);
}

uint32 gtmenu_model_info_open_garage(sint32 index, sint16 category)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 record, models, archive, model, id, available, count, color_table;
    uint32 current, name, previous, descriptor, first, second, mode, result;
    sint32 model_index;
    sint8 code;
    FUNCTION_MARKER(0x80016EB8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80016EB8);
    record = gtmenu_record_address(index);
    models = gtmenu_resource_group(13) + 24;
    archive = gtmenu_resource_archive();
    id = r_u16(record);
    w_u8(0x80080708, 0);
    w_u32(0x80058164, 0x80080700);
    w_u8(0x8005815D, 1);
    w_u16(0x80058160, category);
    w_u16(0x8008070A, index);
    w_u8(0x8005815E, 2);
    model = models + (id & 0xFFF) * 424;
    w_u32(0x8008070C, model);
    available = gtmenu_model_info_change_available(model);
    id = r_u16(record);
    w_u8(0x8005815C, available);
    gtmenu_model_info_selection_set(id >> 12);
    model_index = gtmenu_model_name_index(model);
    count = gtmenu_model_color_count(model_index);
    color_table = gtmenu_model_color_table(model_index);
    for (current = 0; current < count; ++current) {
        code = (sint8)gtmenu_model_color_code(current, color_table);
        name = gtmenu_model_color_name((r_u16(record) & 0xFFF) + 1, code, archive);
        gtmenu_string_copy(0x80080798 + current * 64, name);
    }
    gtmenu_memory_copy(0x80080B98, record, 96);
    w_u16(0x80080B98, r_u16(0x80080B98) + 1);
    gtmenu_settings_from_record(0x800A7840, 0x80080B98, archive);
    gtmenu_model_stats_build(0x800A7840, frame.address, 0x800A77D0);
    previous = r_u16(0x800A77C4);
    w_u32(0x800A77B4, 0x800167DC);
    w_u16(0x800A77C4, 661);
    w_u16(0x800A77C0, previous);
    gtmenu_page_open(0x80053828);
    descriptor = r_u32(0x80058164);
    first = r_u32(0x80080BF8);
    second = r_u32(0x80080BFC);
    mode = r_u16(record + 34) != 0;
    gtmenu_model_info_bind(descriptor, first, second, mode, 0);
    gtmenu_preview_control_set(r_u16(record + 90));
    result = gtmenu_model_logo_load(model);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_purchase_commit(void)
{
    uint32 descriptor, model, value;
    sint32 selection, count;
    sint64 credits, price, remaining;
    FUNCTION_MARKER(0x800170F8, "GTMENU.EXE");
    descriptor = r_u32(0x80058164);
    credits = (sint64)(sint32)gtmenu_credits_get() * 100;
    value = r_u32(descriptor + 16);
    if (value) price = (sint32)value;
    else {
        model = r_u32(descriptor + 12);
        price = r_u32(model + 388);
    }
    if (credits < price) return 2;
    selection = gtmenu_model_info_selection();
    if (gtmenu_garage_add(descriptor, (uint8)selection)) return 3;
    value = r_u32(descriptor + 16);
    if (value) price = (sint32)value;
    else {
        model = r_u32(descriptor + 12);
        price = r_u32(model + 388);
    }
    remaining = credits - price;
    gtmenu_credits_set((uint32)gtmenu_signed_divide64(remaining, 100, 0x800171FC));
    if (gtmenu_current_record_index() >= 0) return 0;
    count = gtmenu_record_count();
    gtmenu_current_record_set((sint16)(count - 1));
    return 1;
}

uint32 gtmenu_model_change_prepare(void)
{
    uint32 descriptor, record;
    FUNCTION_MARKER(0x80017244, "GTMENU.EXE");
    descriptor = r_u32(0x80058164);
    gtmenu_current_record_set((sint16)r_u16(descriptor + 10));
    record = gtmenu_record_address(gtmenu_current_record_index());
    return gtmenu_record_special_parts_mark(record);
}

uint32 gtmenu_garage_widget_create(uint32 output, uint32 rectangle)
{
    uint32 list, widget, entries;
    FUNCTION_MARKER(0x800172C4, "GTMENU.EXE");
    list = gtmenu_primary_list();
    widget = gtmenu_ui_list_create(rectangle, list, 1);
    if (!widget) {
        w_u32(output, 0);
        return output;
    }
    gtmenu_ui_active_list_set(widget, 0);
    entries = r_u32(widget + 16);
    if (r_u32(entries)) {
        gtmenu_ui_selection_mode_set(2);
        if (r_u16(0x800A77C0) == 661) gtmenu_ui_selection_restore();
    }
    w_u32(output, widget);
    return output;
}

uint32 gtmenu_ui_list_create(uint32 rectangle, uint32 list, uint8 kind)
{
    uint32 node, first, second, y;
    FUNCTION_MARKER(0x8001A808, "GTMENU.EXE");
    node = gtmenu_list_node_allocate();
    if (!node) return 0;
    w_u8(0x80058448, kind);
    w_u32(node, 3);
    first = r_u32(rectangle);
    second = r_u32(rectangle + 4);
    w_u32(node + 4, first);
    w_u32(node + 8, second);
    w_u32(node + 16, list);
    y = r_u16(rectangle + 2);
    w_u32(node + 12, 0);
    w_u16(node + 22, y);
    return node;
}

uint32 gtmenu_ui_child_list_event(uint32 event, uint32 owner)
{
    /* Original occurrences
     * GTMENU.EXE:0x8001736C
     * GTMENU.EXE:0x800174A8
     * GTMENU.EXE:0x8001C158
     */
    uint32 child;
    child = r_u32(owner + 8);
    if (!child) return 3;
    if (r_u32(child) != 3) return 3;
    return gtmenu_ui_list_event_dispatch(event, child);
}

uint32 gtmenu_current_car_widget_create(uint32 output, uint32 rectangle)
{
    sint32 selected;
    uint32 list, widget, entries;
    FUNCTION_MARKER(0x800173D8, "GTMENU.EXE");
    selected = gtmenu_current_record_index();
    list = gtmenu_primary_list();
    widget = gtmenu_ui_list_create(rectangle, list, 0);
    if (!widget) {
        w_u32(output, 0);
        return output;
    }
    gtmenu_ui_active_list_set(widget, 0);
    entries = r_u32(widget + 16);
    if (r_u32(entries)) {
        gtmenu_ui_selection_mode_set(2);
        if (r_u16(0x800A77C0) == 661) {
            gtmenu_ui_selection_restore();
            w_u32(output, widget);
            return output;
        }
        if (selected >= 0) gtmenu_ui_selection_set(0, (sint16)selected);
    }
    w_u32(output, widget);
    return output;
}

uint32 gtmenu_ui_selection_mode_set(uint32 mode)
{
    uint32 previous;
    FUNCTION_MARKER(0x8001A78C, "GTMENU.EXE");
    previous = r_u32(0x800A77B4);
    w_u32(0x800A77B4, 0x8001A488);
    w_u8(0x8005844A, mode);
    w_u8(0x80058449, 1);
    w_u32(0x80081F7C, previous);
    return 1;
}

uint32 gtmenu_ui_selection_set(uint32 secondary, sint16 selection)
{
    uint32 widget;
    FUNCTION_MARKER(0x8001A948, "GTMENU.EXE");
    if (secondary) widget = r_u32(0x80058444);
    else widget = r_u32(0x80058440);
    w_u16(widget + 20, selection);
    return widget;
}

uint32 gtmenu_ui_selection_restore(void)
{
    uint32 widget, selection;
    FUNCTION_MARKER(0x8001A970, "GTMENU.EXE");
    widget = r_u32(0x80058440);
    selection = r_u16(0x8005844C);
    w_u16(widget + 20, selection);
    return selection;
}

uint32 gtmenu_menu_texture_load(uint32 name, uint32 clut_position, uint32 clut_size,
    uint32 pixel_position, uint32 pixel_size, uint32 pixels_first)
{
    uint32 image;
    FUNCTION_MARKER(0x800175E4, "GTMENU.EXE");
    image = gtmenu_menu_asset_load(name);
    gtmenu_tim_upload(image, clut_position, clut_size, pixel_position, pixel_size, pixels_first);
    return gtmenu_heap_free(image);
}

uint32 gtmenu_music_resources_initialize(void)
{
    uint32 archive, offset, result;
    FUNCTION_MARKER(0x800176B0, "GTMENU.EXE");
    archive = r_u32(0x800A7AB4);
    offset = r_u32(archive + 40);
    gtmenu_resource_names_index(0x80081EE0, archive + offset);
    archive = r_u32(0x800A7AB4);
    offset = r_u32(archive + 52);
    result = archive + offset;
    w_u32(0x80081ED8, result);
    return result;
}

sint32 gtmenu_resource_names_index(uint32 index, uint32 text)
{
    uint32 cursor = text, count = 0;
    sint32 value;
    FUNCTION_MARKER(0x800230D0, "GTMENU.EXE");
    value = (sint8)r_u8(cursor);
    while (value) {
        if (value == 10) {
            w_u8(cursor, 0);
            if (cursor == text) break;
            w_u32(index + count * 4 + 8, text);
            ++count;
            text = cursor + 1;
        }
        ++cursor;
        value = r_s8(cursor);
    }
    w_u32(index, count);
    return value;
}

uint32 gtmenu_menu_initialize(void)
{
    uint32 state = 4, flags;
    FUNCTION_MARKER(0x80017704, "GTMENU.EXE");
    gtmenu_memory_fill(0x800A77A0, 0, 32);
    gtmenu_resources_initialize();
    gtmenu_render_reset();
    gtmenu_menu_state_reset();
    gtmenu_input_initialize();
    w_u32(0x800A77B4, 0x8001FDA4);
    gtmenu_navigation_initialize();
    gtmenu_lists_initialize();
    gtmenu_links_initialize();
    flags = r_u32(0x800A2408);
    if (flags & 0x10C40037) state = r_u16(0x800A240C);
    else w_u32(0x800A2408, r_u32(0x800A2408) | 0x10C40037);
    w_u32(0x800A7A18, 0);
    w_u32(0x800A7564, 0);
    w_u32(0x800A7570, 0);
    w_u16(0x800A77B2, 0);
    return gtmenu_initial_page_open(state);
}

uint32 gtmenu_ordering_draw_current(void)
{
    uint32 ordering;
    FUNCTION_MARKER(0x800177F4, "GTMENU.EXE");
    ordering = gtmenu_text_surface_address();
    return gtmenu_ordering_draw(ordering);
}

uint32 gtmenu_menu_frame_loop(void)
{
    FUNCTION_MARKER(0x8001781C, "GTMENU.EXE");
    gtmenu_frame_prepare(1);
    gtmenu_display_buffer_apply(1);
    gtmenu_gpu_wait(0);
    VSync(0);
    gtmenu_ordering_draw_current();
    gtmenu_display_buffer_apply(1);
    gtmenu_gpu_wait(0);
    if (r_s32(0x80056C58) >= 0) {
        do {
            gtmenu_input_update();
            gtmenu_frame_prepare(0);
            VSync(0);
            gtmenu_display_buffers_swap(0);
            gtmenu_display_enable(1);
            gtmenu_ordering_draw_current();
            gtmenu_display_buffer_apply(1);
        } while (!(r_u32(0x80056C58) & 0x80000000));
    }
    gtmenu_frames_flush();
    gtmenu_gpu_wait(0);
    return 0;
}

uint32 gtmenu_menu_state_reset(void)
{
    uint32 x, y, width, height;
    FUNCTION_MARKER(0x800178EC, "GTMENU.EXE");
    gtmenu_memory_fill(0x800A79D0, 0, 228);
    x = r_u16(0x800A75A0);
    y = r_u16(0x800A75A2);
    width = r_u16(0x800A75A4);
    height = r_u16(0x800A75A6);
    w_u32(0x800A7570, 0);
    w_u32(0x800A7564, 0);
    w_u32(0x800A7A18, 0);
    w_u16(0x800A7A1C, x);
    w_u16(0x800A7A1E, y);
    w_u16(0x800A7A20, width);
    w_u32(0x800A7A2C, height);
    return 0x800A7A1C;
}

uint32 gtmenu_page_runtime_release(void)
{
    FUNCTION_MARKER(0x80017958, "GTMENU.EXE");
    gtmenu_layout_release();
    gtmenu_heap_free(r_u32(0x800A79D0));
    return gtmenu_layout_initialize();
}

void gtmenu_hitboxes_disable(void)
{
    FUNCTION_MARKER(0x80017B40, "GTMENU.EXE");
    w_u32(0x800A7A18, 0);
}

uint32 gtmenu_press_button_draw(uint32 ordering)
{
    sint32 available, x;
    uint32 color, counter, result;
    FUNCTION_MARKER(0x80017B50, "GTMENU.EXE");
    available = (sint32)(640u - (uint32)gtmenu_text_width(0, 0x80053848));
    x = available / 2;
    gtmenu_text_font_set(0);
    color = r_s32(0x80058188) < 14 ? 0 : 0xFFFFFF;
    gtmenu_text_color_set(color);
    gtmenu_text_draw(ordering, x, 360, 0x80053848);
    counter = r_u32(0x80058188) + 1;
    w_u32(0x80058188, counter);
    result = (sint32)counter < 28;
    if (!result) w_u32(0x80058188, 0);
    return result;
}

uint32 gtmenu_letterbox_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80017C1C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80017C1C);
    w_u16(frame.address, 0);
    w_u16(frame.address + 2, 0);
    w_u16(frame.address + 4, 639);
    w_u16(frame.address + 6, 15);
    gtmenu_rectangle_draw(ordering, frame.address, 0);
    w_u16(frame.address, 0);
    w_u16(frame.address + 2, 496);
    w_u16(frame.address + 4, 639);
    w_u16(frame.address + 6, 511);
    result = gtmenu_rectangle_draw(ordering, frame.address, 0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_frames_flush(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8001BCFC, "GTMENU.EXE");
    for (index = 0; index < 2; ++index) {
        gtmenu_gpu_wait(0);
        gtmenu_ordering_draw_current();
        VSync(0);
        gtmenu_display_buffer_apply(1);
    }
    return 0;
}

uint32 gtmenu_hitboxes_build(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 output, base, count, index = 0, parent, child, kind, rectangle;
    uint32 first, second, value, high, low, radius;
    FUNCTION_MARKER(0x80017990, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80017990);
    output = gtmenu_hitbox_buffer();
    base = gtmain_menu_default_callback();
    count = gtmenu_layout_rect_count();
    w_u32(output, count + base);
    gtmain_menu_default_callback();
    parent = r_u32(0x800A7A10);
    while (parent) {
        child = r_u32(parent + 40);
        while (child) {
            kind = r_u32(child + 36);
            if (kind == 1) {
                rectangle = output + (base + index) * 8;
                first = r_u16(parent + 4);
                second = r_u16(child + 40);
                w_u16(rectangle + 4, first + second);
                first = r_u16(parent + 6);
                second = r_u16(child + 42);
                w_u16(rectangle + 6, first + second);
                first = r_u16(parent + 4);
                second = r_u16(child + 44);
                value = r_u16(rectangle + 4);
                w_u16(rectangle + 8, first + second - value);
                first = r_u16(parent + 6);
                second = r_u16(child + 46);
                value = r_u16(rectangle + 6);
                w_u16(rectangle + 10, first + second - value);
            } else if (kind == 2) {
                high = r_u16(child + 46);
                low = r_u16(child + 44);
                radius = r_u16(child + 40);
                gtmenu_circle_bounds(frame.address, low | (high << 16), radius);
                rectangle = output + (base + index) * 8;
                first = r_u16(parent + 4);
                second = r_u16(frame.address);
                w_u16(rectangle + 4, first + second);
                first = r_u16(parent + 6);
                second = r_u16(frame.address + 2);
                w_u16(rectangle + 6, first + second);
                w_u16(rectangle + 8, r_u16(frame.address + 4));
                w_u16(rectangle + 10, r_u16(frame.address + 6));
            }
            child = r_u32(child);
            ++index;
        }
        parent = r_u32(parent);
    }
    w_u32(0x800A7A18, 1);
    gtmenu_hitbox_mode_set(0);
    gtmenu_local_stack_pop(frame);
    return 1;
}

uint32 gtmenu_navigation_node_create(uint32 kind, uint32 rectangle)
{
    uint32 node, first, second;
    FUNCTION_MARKER(0x80017CC0, "GTMENU.EXE");
    node = gtmenu_heap_alloc(20);
    first = r_u32(rectangle);
    second = r_u32(rectangle + 4);
    w_u32(node, first);
    w_u32(node + 4, second);
    w_u32(node + 8, 0);
    w_u32(node + 12, kind);
    w_u32(node + 16, 0);
    return node;
}

uint32 gtmenu_page_action_code(uint32 page)
{
    uint32 index = 0, name;
    sint8 first, second;
    FUNCTION_MARKER(0x80017D18, "GTMENU.EXE");
    if (!page) return 0;
    if (r_s8(0x80058184) && !gtmenu_string_compare_folded(page, 0x80053A20)) return 0;
    if (r_s8(0x80058185) && !gtmenu_string_compare_folded(page, 0x80053A2C)) return 0;
    if (!r_u8(0x8009B85D)) {
        first = (sint8)gtmenu_ascii_lower(r_s8(page));
        if (first == 'c') {
            second = (sint8)gtmenu_ascii_lower(r_s8(page + 1));
            if (second == first) {
                if (!gtmenu_string_compare_folded(page + 2, 0x80053A3C)) return 0;
                return 56;
            }
        }
    }
    name = r_u32(0x8005818C);
    while (name) {
        if (!gtmenu_string_compare_folded(page, name)) break;
        ++index;
        name = r_u32(0x8005818C + index * 8);
    }
    return r_u32(0x80058190 + index * 8);
}

void gtmenu_page_widgets_draw(uint32 ordering)
{
    uint32 node, kind, category, current;
    FUNCTION_MARKER(0x80017E78, "GTMENU.EXE");
    node = r_u32(0x80081F60);
    while (node) {
        kind = r_u32(node + 12);
        if ((kind - 1 < 10) || kind == 11) gtmenu_ui_child_list_event(ordering, node);
        else {
            kind = r_u32(node + 12);
            if (kind == 54) gtmenu_catalog_widget_draw(ordering, node);
            else if (kind == 14) gtmenu_ui_child_list_event(ordering, node);
        }
        node = r_u32(node + 16);
    }
    if (r_u16(0x800A77C4) == 655) gtmenu_press_button_draw(ordering);
    gtmenu_license_panel_pulse_draw(ordering);
    category = r_u16(0x800A77C4);
    if (category == 19) {
        gtmenu_page_hint_draw(ordering, !r_s8(0x80058185));
        category = r_u16(0x800A77C4);
    }
    if (category == 39) gtmenu_page_hint_draw(ordering, 2);
    category = r_u16(0x800A77C4);
    if (category - 655 >= 2) {
        current = r_u16(0x800A77C4);
        if (current != 1 && category - 658 >= 2 && current != 48 && current != 46 &&
            current != 662 && category - 14 >= 2 && category - 16 >= 2 &&
            category - 34 >= 2 && category - 36 >= 2 && current != 38 &&
            category - 20 >= 2 && category - 22 >= 2 && category - 24 >= 2 &&
            category - 26 >= 2 && category - 28 >= 2 && category - 30 >= 2 &&
            current != 32 && category - 41 >= 2 && current != 43 && current != 6) {
            gtmenu_hud_current_car_draw(ordering);
            gtmenu_hud_credits_draw(ordering);
            gtmenu_hud_days_draw(ordering);
        }
    }
    gtmenu_page_footer_draw(ordering);
    gtmenu_page_cursor_draw(ordering);
}

uint32 gtmenu_page_widget_create(uint32 output, uint32 description)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 kind, result = 0;
    FUNCTION_MARKER(0x80018120, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80018120);
    kind = r_u32(description + 12);
    if (kind == 1) {
        gtmenu_garage_widget_create(frame.address, description);
        result = r_u32(frame.address);
    } else {
        kind = r_u32(description + 12);
        if (kind - 2 < 8) {
            gtmenu_garage_widget_create(frame.address, description);
            result = r_u32(frame.address);
        } else if (kind == 10) {
            gtmenu_garage_widget_create(frame.address, description);
        } else {
            kind = r_u32(description + 12);
            if (kind == 11) {
                gtmenu_current_car_widget_create(frame.address + 4, description);
                result = r_u32(frame.address + 4);
            } else if (kind == 54) {
                gtmenu_catalog_list_create(frame.address + 4, description, 0);
                result = r_u32(frame.address + 4);
            } else if (kind != 52 && kind != 53) {
                kind = r_u32(description + 12);
                if (kind == 12) gtmenu_model_info_layout_set(description);
                else if (kind == 13) gtmenu_model_info_position_set(description);
                else if (r_u32(description + 12) == 14) {
                    gtmenu_simple_list_widget_create(frame.address + 4, description);
                    result = r_u32(frame.address + 4);
                }
            }
        }
    }
    w_u32(output, result);
    gtmenu_local_stack_pop(frame);
    return output;
}

uint32 gtmenu_navigation_initialize(void)
{
    FUNCTION_MARKER(0x80018278, "GTMENU.EXE");
    w_u32(0x80081F60, 0);
    w_u32(0x80081F64, 0);
    w_u32(0x80081F68, 0);
    return 0x80081F60;
}

uint32 gtmenu_simple_list_widget_create(uint32 output, uint32 rectangle)
{
    uint32 list, widget;
    FUNCTION_MARKER(0x8001C0EC, "GTMENU.EXE");
    list = gtmenu_primary_list();
    widget = gtmenu_ui_list_create(rectangle, list, 1);
    if (widget) gtmenu_ui_active_list_set(widget, 0);
    w_u32(output, widget);
    return output;
}

uint32 gtmenu_circle_bounds(uint32 output, uint32 packed_center, uint32 radius)
{
    uint32 x, y, size;
    FUNCTION_MARKER(0x80023CC0, "GTMENU.EXE");
    x = packed_center - radius;
    y = (uint32)((sint32)packed_center >> 16) - radius;
    size = (radius * 2) & 0xFFFF;
    w_u32(output, (x & 0xFFFF) | (y << 16));
    w_u32(output + 4, size | (size << 16));
    return output;
}

uint32 gtmenu_navigation_append(uint32 kind, uint32 rectangle)
{
    uint32 node, head, tail;
    FUNCTION_MARKER(0x80018290, "GTMENU.EXE");
    node = gtmenu_navigation_node_create(kind, rectangle);
    head = r_u32(0x80081F60);
    if (head) {
        tail = r_u32(0x80081F64);
        if (tail) {
            tail = r_u32(0x80081F64);
            w_u32(tail + 16, node);
            w_u32(0x80081F64, node);
            return tail;
        }
    }
    w_u32(0x80081F60, node);
    w_u32(0x80081F64, node);
    return 0x80081F60;
}

void gtmenu_page_widgets_initialize(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 node, widget;
    FUNCTION_MARKER(0x80018300, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80018300);
    node = r_u32(0x80081F60);
    while (node) {
        gtmenu_page_widget_create(frame.address, node);
        widget = r_u32(frame.address);
        w_u32(node + 8, widget);
        node = r_u32(node + 16);
    }
    gtmenu_local_stack_pop(frame);
}

uint32 gtmenu_navigation_bind_pair(uint32 first, uint32 second, uint32 first_rectangle, uint32 second_rectangle)
{
    uint32 kind, node, index;
    FUNCTION_MARKER(0x80018354, "GTMENU.EXE");
    kind = gtmenu_page_action_code(0x800539C8);
    gtmenu_navigation_append(kind, first_rectangle);
    kind = gtmenu_page_action_code(0x800539BC);
    gtmenu_navigation_append(kind, second_rectangle);
    node = r_u32(0x80081F60);
    for (index = 0; index < 2; ++index) {
        w_u32(node + 8, index ? second : first);
        node = r_u32(node + 16);
    }
    return 0;
}

uint32 gtmenu_navigation_bind_trade(uint32 child, uint32 rectangle)
{
    uint32 kind, node;
    FUNCTION_MARKER(0x800183FC, "GTMENU.EXE");
    kind = gtmenu_page_action_code(0x800539F4);
    gtmenu_navigation_append(kind, rectangle);
    node = r_u32(0x80081F60);
    w_u32(node + 8, child);
    return node;
}

uint32 gtmenu_navigation_nodes_clear(void)
{
    uint32 node, child, next;
    FUNCTION_MARKER(0x80018450, "GTMENU.EXE");
    node = r_u32(0x80081F60);
    while (node) {
        child = r_u32(node + 8);
        next = r_u32(node + 16);
        if (child && r_u32(child) == 3) gtmenu_heap_free(child);
        gtmenu_heap_free(node);
        node = next;
    }
    return gtmenu_navigation_initialize();
}

uint32 gtmenu_navigation_select_point(uint16 x, uint32 y)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 node, found = 0;
    FUNCTION_MARKER(0x800184FC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x800184FC);
    node = r_u32(0x80081F60);
    w_u32(frame.address, x | (y << 16));
    while (node) {
        if (gtmenu_point_in_rectangle(node, frame.address)) {
            gtmenu_navigation_selection_set(node);
            found = 1;
        }
        node = r_u32(node + 16);
        if (found) break;
    }
    if (!found) gtmenu_navigation_selection_set(0);
    gtmenu_local_stack_pop(frame);
    return found;
}

void gtmenu_navigation_selection_set(uint32 node)
{
    FUNCTION_MARKER(0x8001858C, "GTMENU.EXE");
    w_u32(0x80081F68, node);
}

uint32 gtmenu_navigation_selection(void)
{
    FUNCTION_MARKER(0x8001859C, "GTMENU.EXE");
    return r_u32(0x80081F68);
}

uint32 gtmenu_point_in_rectangle(uint32 rectangle, uint32 point)
{
    sint32 coordinate, origin, extent;
    FUNCTION_MARKER(0x80023D0C, "GTMENU.EXE");
    coordinate = r_s16(point);
    origin = r_s16(rectangle);
    extent = r_s16(rectangle + 4);
    if ((uint32)(coordinate - origin) >= (uint32)extent) return 0;
    coordinate = r_s16(point + 2);
    origin = r_s16(rectangle + 2);
    extent = r_s16(rectangle + 6);
    return (uint32)(coordinate - origin) < (uint32)extent;
}

uint32 gtmenu_navigation_input(uint32 event)
{
    FUNCTION_MARKER(0x800185AC, "GTMENU.EXE");
    if (!(r_u32(event + 12) & 0xF0)) gtmenu_navigation_selection();
    return 0;
}

void gtmenu_car_spec_filter_set(uint8 enabled)
{
    FUNCTION_MARKER(0x80018600, "GTMENU.EXE");
    w_u8(0x80058184, enabled);
}

void gtmenu_hifi_filter_set(uint8 enabled)
{
    FUNCTION_MARKER(0x8001860C, "GTMENU.EXE");
    w_u8(0x80058185, enabled);
}

uint32 gtmenu_triangle_wave(uint32 phase)
{
    sint32 value;
    FUNCTION_MARKER(0x800186BC, "GTMENU.EXE");
    value = 255 - (sint32)(phase & 0x1FF);
    if (value < 0) value = -value;
    return value;
}

uint32 gtmenu_text_centered_at_y(uint32 ordering, uint32 rectangle, sint16 y, uint32 color, uint32 text)
{
    sint32 first, last, width, offset, x;
    FUNCTION_MARKER(0x8001878C, "GTMENU.EXE");
    last = r_s16(rectangle + 4);
    first = r_s16(rectangle);
    gtmenu_text_font_set(0);
    gtmenu_text_color_set(color);
    width = gtmenu_text_width(0, text);
    offset = (sint32)((uint32)(last - first) - (uint32)width) >> 1;
    x = (sint32)((uint32)first + (uint32)offset);
    return gtmenu_text_draw(ordering, x, y, text);
}

uint32 gtmenu_two_choices_draw(uint32 selected, uint32 ordering, uint32 first_rectangle,
    uint32 second_rectangle, uint32 first_text, uint32 second_text, uint32 phase)
{
    uint32 intensity, first_color, second_color = 0;
    FUNCTION_MARKER(0x80018824, "GTMENU.EXE");
    gtmenu_text_centered(ordering, first_rectangle, 0x808080, first_text);
    gtmenu_text_centered(ordering, second_rectangle, 0x808080, second_text);
    gtmenu_rectangle_outline(ordering, first_rectangle, 0x808080);
    gtmenu_rectangle_outline(ordering, second_rectangle, 0x808080);
    intensity = gtmenu_triangle_wave(phase * 8) & 0xFF;
    first_color = intensity | (intensity << 8);
    if (selected) {
        second_color = first_color;
        first_color = 0;
    }
    gtmenu_rectangle_draw(ordering, first_rectangle, first_color);
    return gtmenu_rectangle_draw(ordering, second_rectangle, second_color);
}

uint32 gtmenu_record_display_name(uint32 record, uint32 destination)
{
    uint32 models, records, packed, table, first, second;
    FUNCTION_MARKER(0x80018AA0, "GTMENU.EXE");
    models = gtmenu_resource_group(13);
    records = models + 24;
    packed = r_u32(records + 424 * (r_u16(record) & 0xFFF) + 392);
    table = r_u32(models + 16);
    table = r_u32(table + 4 * (packed >> 16) + 4);
    first = gtmenu_packed_text_entry(table, packed & 0xFFFF) + 1;
    packed = r_u32(records + 424 * (r_u16(record) & 0xFFF) + 396);
    table = r_u32(models + 16);
    table = r_u32(table + 4 * (packed >> 16) + 4);
    second = gtmenu_packed_text_entry(table, packed & 0xFFFF);
    gtmenu_string_copy(destination, first);
    if (r_s8(second + 1) != '-') {
        gtmenu_string_append(destination, 0x80053C7C);
        gtmenu_string_append(destination, second + 1);
    }
    return destination;
}

uint32 gtmenu_text_font_set(sint32 font)
{
    uint32 clut, state, glyphs, strings, spacing, result;
    FUNCTION_MARKER(0x8001AC3C, "GTMENU.EXE");
    clut = r_u16(0x800A7AE8);
    state = 0x800A7AE0;
    result = font < 2;
    switch (font) {
    case 0:
        glyphs = 0x80058C90;
        strings = 0x8005E190;
        spacing = 16;
        break;
    case 1:
        clut += 64;
        glyphs = 0x8005E590;
        strings = 0x80064410;
        spacing = 24;
        break;
    case 2:
    case 3:
        state = 0x800A7AC0;
        if (font == 2) {
            glyphs = 0x80068F1C;
            strings = 0x8006AADC;
        } else {
            glyphs = 0x8006ACDC;
            strings = 0x8006C65C;
        }
        result = r_u32(0x800A7AEC);
        w_u32(0x800A7ACC, result);
        clut = r_u16(state + 8);
        if (font == 3) clut += 64;
        spacing = 20;
        break;
    default:
        return font < 2 ? result : 3;
    }
    w_u16(state + 26, font);
    w_u32(state, glyphs);
    w_u32(state + 4, strings);
    w_u16(state + 30, spacing);
    w_u16(state + 24, clut);
    return result;
}

uint32 gtmenu_text_color_set(uint32 rgb)
{
    uint32 scale, color = 0, shift, channel;
    sint32 product;
    FUNCTION_MARKER(0x8001AD54, "GTMENU.EXE");
    scale = r_u32(0x80058450);
    for (shift = 0; shift < 24; shift += 8) {
        channel = (rgb >> shift) & 0xFF;
        product = (sint32)(channel * 10 * scale);
        color |= ((uint32)(product / 100) & 0xFF) << shift;
    }
    w_u32(0x800A7AEC, (r_u32(0x800A7AEC) & 0xFF000000) | color);
    w_u32(0x800A7ACC, (r_u32(0x800A7ACC) & 0xFF000000) | color);
    return 0x800A7ACE;
}

uint32 gtmenu_font_line_height(uint32 font)
{
    FUNCTION_MARKER(0x8001AE38, "GTMENU.EXE");
    if (font == 0) return 16;
    if (font == 1) return 24;
    if (font == 2 || font == 3) return 18;
    return 0;
}

uint32 gtmenu_text_fixed_width(sint32 font, uint32 text)
{
    uint32 spacing;
    FUNCTION_MARKER(0x8001AE78, "GTMENU.EXE");
    if (font == 0) spacing = 16;
    else if (font == 1) spacing = 24;
    else if (font == 2 || font == 3) spacing = 18;
    else return 0;
    return (uint32)gtmenu_string_length(text) * spacing;
}

uint32 gtmenu_text_comma_width(sint32 font, uint32 text)
{
    uint32 spacing, width = 0;
    sint32 length, index;
    FUNCTION_MARKER(0x8001AEE0, "GTMENU.EXE");
    if (!text) return 0;
    if (font == 0) spacing = 14;
    else if (font == 1) spacing = 24;
    else if (font == 2 || font == 3) spacing = 18;
    else return 0;
    length = gtmenu_string_length(text);
    for (index = 0; index < length; ++index) {
        width += r_s8(text + (uint32)index) == ',' ? 4 : spacing;
    }
    return width;
}

sint32 gtmenu_glyph_run_width(uint32 state, uint32 text)
{
    uint32 glyphs, tables, glyph, table, next, kern, width = 0, advance;
    sint32 character;
    FUNCTION_MARKER(0x80032604, "GTMENU.EXE");
    character = r_s8(text);
    glyphs = r_u32(state);
    tables = r_u32(state + 4);
    while (character) {
        ++text;
        glyph = glyphs + 8 * (uint32)character;
        next = r_u8(text);
        table = r_u32(tables + 4 * (uint32)character);
        kern = r_u32(table + 4 * (next >> 3));
        advance = r_u8(glyph + 6);
        kern = (kern >> (4 * (next & 7))) & 0xF;
        width += 1 + advance - kern;
        character = next;
    }
    return (sint32)width;
}

sint32 gtmenu_text_width(uint32 unused_font, uint32 text)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 width = 0, special = 0, count = 0, code, half_height;
    sint32 font, next_character;
    FUNCTION_MARKER(0x8001B074, "GTMENU.EXE");
    font = r_s16(0x800A7AFA);
    half_height = gtmenu_font_line_height((uint32)font) / 2;
    if (!text) return 0;
    frame = gtmenu_local_stack_push(2056, 0x8001B074);
    code = r_u8(text);
    if (code) {
        for (;;) {
            if (code - 0x80 < 0x18) {
                if (!special) {
                    w_u8(frame.address + count, 0);
                    width += (uint32)gtmenu_glyph_run_width(0x800A7AE0, frame.address);
                    special = 1;
                    count = 0;
                }
                code = r_u8(text);
                ++text;
                w_u8(frame.address + count, code);
                ++count;
            } else if (special) {
                w_u8(frame.address + count, 0);
                width += (uint32)gtmenu_string_length(frame.address) * half_height;
                special = 0;
                count = 0;
            }
            code = r_u8(text);
            w_u8(frame.address + count, code);
            ++text;
            next_character = r_s8(text);
            code = r_u8(text);
            ++count;
            if (!next_character) break;
        }
    }
    w_u8(frame.address + count, 0);
    if (special) width += (uint32)gtmenu_string_length(frame.address) * half_height;
    else width += (uint32)gtmenu_glyph_run_width(0x800A7AE0, frame.address);
    gtmenu_local_stack_pop(frame);
    return (sint32)width;
}

uint32 gtmenu_rectangle_outline(uint32 ordering, uint32 rectangle, uint32 color)
{
    uint32 first_word, last_word, first, packet, end, old_link, edge, count;
    sint32 left, right, top, bottom, swap, x0, y0, x1, y1;
    FUNCTION_MARKER(0x80032440, "GTMENU.EXE");
    first_word = r_u32(rectangle);
    last_word = r_u32(rectangle + 4);
    left = (sint16)first_word;
    right = (sint16)last_word;
    top = (sint32)first_word >> 16;
    bottom = (sint32)last_word >> 16;
    if (right < left) { swap = left; left = right; right = swap; }
    if (bottom < top) { swap = top; top = bottom; bottom = swap; }
    first = r_u32(0x800A8434);
    packet = first;
    color ^= 0x41000000;
    if (left == right || top == bottom) {
        w_u32(packet, 0x03000000);
        w_u32(packet + 4, color);
        w_u16(packet + 8, left);
        w_u16(packet + 10, top);
        w_u16(packet + 12, right);
        w_u16(packet + 14, bottom);
        end = packet + 16;
    } else {
        count = bottom - top < 2 ? 2 : 4;
        for (edge = 0; edge < count; ++edge) {
            w_u32(packet, (r_u32(packet) & 0xFF000000) | ((packet + 16) & 0xFFFFFF));
            x0 = left;
            x1 = right;
            if (edge < 2) y0 = y1 = edge ? bottom : top;
            else {
                y0 = top + 1;
                y1 = bottom - 1;
                x0 = x1 = edge == 2 ? left : right;
            }
            w_u8(packet + 3, 3);
            w_u32(packet + 4, color);
            w_u16(packet + 8, x0);
            w_u16(packet + 10, y0);
            w_u16(packet + 12, x1);
            w_u16(packet + 14, y1);
            end = packet + 16;
            if (edge + 1 < count) packet = end;
        }
    }
    old_link = r_u32(ordering) & 0xFFFFFF;
    w_u32(ordering, (r_u32(ordering) & 0xFF000000) | (first & 0xFFFFFF));
    w_u32(packet, (r_u32(packet) & 0xFF000000) | old_link);
    w_u32(0x800A8434, end);
    return packet;
}

uint32 gtmenu_text_draw(uint32 ordering, sint32 x, sint16 y, uint32 text)
{
    uint32 code, glyphs, tables, color, clut, glyph, geometry, metrics, height;
    uint32 packet, old_link, next, table, kern, cursor = (uint32)x;
    FUNCTION_MARKER(0x8001B280, "GTMENU.EXE");
    code = r_u8(text);
    while (code) {
        glyphs = r_u32(0x800A7AE0);
        tables = r_u32(0x800A7AE4);
        color = r_u32(0x800A7AEC);
        clut = r_u16(0x800A7AF8);
        glyph = glyphs + 8 * code;
        geometry = r_u32(glyph);
        metrics = r_u32(glyph + 4);
        height = geometry >> 24;
        if (height) {
            packet = r_u32(0x800A8434);
            w_u32(0x800A8434, packet + 20);
            old_link = r_u32(ordering) & 0xFFFFFF;
            w_u32(ordering, (r_u32(ordering) & 0xFF000000) | (packet & 0xFFFFFF));
            w_u32(packet, (r_u32(packet) & 0xFF000000) | old_link);
            w_u8(packet + 3, 4);
            w_u32(packet + 4, color);
            w_u16(packet + 8, cursor);
            w_u16(packet + 10, (sint32)y + ((metrics >> 8) & 0xFF) - 4);
            w_u16(packet + 16, (geometry >> 16) & 0xFF);
            w_u16(packet + 18, height);
            w_u16(packet + 12, geometry);
            w_u16(packet + 14, clut);
        }
        ++text;
        next = r_u8(text);
        if (next - 0x80 < 0x18 || code >= 0x80) {
            cursor += 1 + ((metrics >> 16) & 0xFF);
        } else {
            table = r_u32(tables + 4 * code);
            kern = r_u32(table + 4 * (next >> 3));
            kern = (kern >> (4 * (next & 7))) & 0xF;
            cursor += 1 + ((metrics >> 16) & 0xFF) - kern;
            if ((code | 0x20) == 'p' && r_s8(text) == '.') cursor += 6;
        }
        code = next;
    }
    return gtmenu_text_draw_mode(0x800A7AE0, ordering);
}

uint32 gtmenu_text_cursor_draw_mode(uint32 ordering)
{
    uint32 status, command, packet, link;
    FUNCTION_MARKER(0x8001A988, "GTMENU.EXE");
    status = r_u32(0x1F801814);
    command = r_u16(0x800A7AEA);
    command = (command | (status & 0x200) | 0xE1000000) & ~0x400u;
    packet = r_u32(0x800A8434);
    w_u32(packet + 4, command);
    w_u32(packet, 0x01000000);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, packet, 7);
    w_masked_u32(packet, link, 7);
    w_u32(0x800A8434, packet + 8);
    return packet;
}

uint32 gtmenu_text_cursor_begin(uint32 ordering, sint16 x, sint16 y)
{
    uint32 packet;
    FUNCTION_MARKER(0x8001A9EC, "GTMENU.EXE");
    packet = gtmenu_text_cursor_draw_mode(ordering);
    w_u32(0x800A7AF4, packet);
    w_u16(0x800A7AF0, x);
    w_u16(0x800A7AF2, y);
    return packet;
}

uint32 gtmenu_glyph_draw_fixed(uint8 character)
{
    uint32 position, glyph, geometry, metrics, height, packet, ordering, color, clut, link;
    sint32 cursor, spacing, x, y;
    FUNCTION_MARKER(0x8001AA34, "GTMENU.EXE");
    position = r_u32(0x800A7AF0);
    glyph = r_u32(0x800A7AE0);
    cursor = r_s16(0x800A7AFC);
    spacing = r_s16(0x800A7AFE);
    glyph += 8 * character;
    geometry = r_u32(glyph);
    metrics = r_u32(glyph + 4);
    height = geometry >> 24;
    if (height) {
        x = (sint16)position + (sint32)(metrics & 0xFF) + cursor;
        y = ((sint32)position >> 16) + (sint32)((metrics >> 8) & 0xFF) - 4;
        packet = r_u32(0x800A8434);
        ordering = r_u32(0x800A7AF4);
        color = r_u32(0x800A7AEC);
        clut = r_u16(0x800A7AF8);
        w_u16(packet + 10, y);
        w_u32(packet, 0x04000000);
        w_u32(packet + 4, color);
        w_u16(packet + 8, x);
        w_u16(packet + 16, (geometry >> 16) & 0xFF);
        w_u16(packet + 18, height);
        w_u16(packet + 12, geometry);
        w_u16(packet + 14, clut);
        link = r_masked_u32(ordering, 7);
        w_masked_u32(ordering, packet, 7);
        w_masked_u32(packet, link, 7);
        w_u32(0x800A7AF4, packet);
        w_u32(0x800A8434, packet + 20);
    }
    w_u16(0x800A7AFC, cursor + spacing);
    return 0;
}

uint32 gtmenu_glyph_draw_numeric(uint8 character)
{
    uint32 position, glyph, geometry, metrics, height, packet, ordering, color, clut, link;
    sint32 cursor, spacing, x, y;
    FUNCTION_MARKER(0x8001AB1C, "GTMENU.EXE");
    position = r_u32(0x800A7AF0);
    r_s16(0x800A7AFA);
    cursor = r_s16(0x800A7AFC);
    spacing = 10;
    if (character != ',') spacing = r_s16(0x800A7AFE) - 2;
    glyph = r_u32(0x800A7AE0) + 8 * character;
    geometry = r_u32(glyph);
    metrics = r_u32(glyph + 4);
    height = geometry >> 24;
    if (height) {
        x = (sint16)position + (sint32)(metrics & 0xFF) + cursor;
        y = ((sint32)position >> 16) + (sint32)((metrics >> 8) & 0xFF) - 4;
        packet = r_u32(0x800A8434);
        ordering = r_u32(0x800A7AF4);
        color = r_u32(0x800A7AEC);
        clut = r_u16(0x800A7AF8);
        w_u16(packet + 10, y);
        w_u32(packet, 0x04000000);
        w_u32(packet + 4, color);
        w_u16(packet + 8, x);
        w_u16(packet + 16, (geometry >> 16) & 0xFF);
        w_u16(packet + 18, height);
        w_u16(packet + 12, geometry);
        w_u16(packet + 14, clut);
        link = r_masked_u32(ordering, 7);
        w_masked_u32(ordering, packet, 7);
        w_masked_u32(packet, link, 7);
        w_u32(0x800A7AF4, packet);
        w_u32(0x800A8434, packet + 20);
    }
    w_u16(0x800A7AFC, cursor + spacing);
    return 0;
}

uint32 gtmenu_text_draw_mode(uint32 state, uint32 ordering)
{
    uint32 status, command, packet, link;
    FUNCTION_MARKER(0x8001B22C, "GTMENU.EXE");
    status = r_u32(0x1F801814);
    command = r_u16(state + 10);
    packet = r_u32(0x800A8434);
    command |= (status & 0x200) | 0xE1000000;
    w_u32(0x800A8434, packet + 8);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, packet, 7);
    w_masked_u32(packet, link, 7);
    w_u8(packet + 3, 1);
    w_u32(packet + 4, command);
    return 1;
}

uint32 gtmenu_fonts_initialize(uint32 texture_page)
{
    FUNCTION_MARKER(0x8001B48C, "GTMENU.EXE");
    gtmenu_font_texture_initialize(0x800A7AE0, texture_page, 0x80064810);
    gtmenu_text_font_set(0);
    gtmenu_font_texture_initialize(0x800A7AC0, texture_page + 1, 0x8006C85C);
    return gtmenu_text_font_set(2);
}

sint32 gtmenu_decimal_group(uint32 destination, uint32 source)
{
    sint32 length, digits, groups, remaining, limit;
    uint32 input, output, index;
    uint8 character;
    FUNCTION_MARKER(0x8001B6B4, "GTMENU.EXE");
    if (!source) return -1;
    length = gtmenu_string_length(source);
    digits = length;
    if (r_s8(source) == '-') digits = (sint32)((uint32)length - 1);
    if (digits < 4) {
        gtmenu_string_copy(destination, source);
        return 0;
    }
    groups = digits / 3;
    if (digits == 3 * groups) --groups;
    remaining = groups;
    output = (uint32)length + (uint32)groups;
    input = (uint32)length;
    w_u8(destination + output, 0);
    limit = (sint32)((uint32)length + 1);
    for (index = 0; (sint32)index < limit; ++index) {
        character = r_u8(source + input);
        --input;
        w_u8(destination + output, character);
        --output;
        if (index && remaining && index % 3 == 0) {
            w_u8(destination + output, ',');
            --output;
            --remaining;
        }
    }
    return groups;
}

uint32 gtmenu_scene_draw(uint32 ordering)
{
    uint32 x, y, scene;
    FUNCTION_MARKER(0x8001B93C, "GTMENU.EXE");
    x = r_u16(0x800A7A24);
    y = r_u16(0x800A7A28);
    scene = r_u32(0x800A79D0);
    w_u16(0x800A79D4, x);
    w_u16(0x800A79D6, y);
    gtmenu_scene_nodes_draw(ordering, scene);
    return gtmenu_fullscreen_clip(ordering);
}

uint32 gtmenu_fullscreen_present(void)
{
    uint32 ordering;
    FUNCTION_MARKER(0x8001B984, "GTMENU.EXE");
    ordering = gtmenu_current_ordering();
    gtmenu_fullscreen_clip(ordering);
    gtmenu_display_buffer_apply(0);
    return gtmenu_display_buffers_clear(0);
}

uint32 gtmenu_fullscreen_clip(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x8001B9BC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8001B9BC);
    w_u16(frame.address + 4, 640);
    w_u32(frame.address, 0);
    w_u16(frame.address + 6, 480);
    result = gtmenu_clip_rectangle(ordering, frame.address);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_clip_rectangle(uint32 ordering, uint32 rectangle)
{
    uint32 first, second, link, command, result;
    sint32 x, y, width, height;
    FUNCTION_MARKER(0x80024854, "GTMENU.EXE");
    first = r_u32(0x800A8434);
    w_u32(0x800A8434, first + 8);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, first, 7);
    w_masked_u32(first, link, 7);
    w_u8(first + 3, 1);
    w_u32(first + 4, 0xE3000000);
    x = r_s16(rectangle);
    second = r_u32(0x800A8434);
    command = 0xE3000000 | ((uint32)x & 0x3FF);
    w_u32(first + 4, command);
    y = r_s16(rectangle + 2);
    w_u32(0x800A8434, second + 8);
    command |= ((uint32)y & 0x3FF) << 10;
    w_u32(first + 4, command);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, second, 7);
    w_masked_u32(second, link, 7);
    w_u8(second + 3, 1);
    w_u32(second + 4, 0xE4000000);
    x = r_s16(rectangle);
    width = r_s16(rectangle + 4);
    command = 0xE4000000 | ((uint32)(x + width - 1) & 0x3FF);
    w_u32(second + 4, command);
    y = r_s16(rectangle + 2);
    height = r_s16(rectangle + 6);
    result = ((uint32)(y + height - 1) & 0x3FF) << 10;
    w_u32(second + 4, (command & 0xFFF003FF) | result);
    return result;
}

uint32 gtmenu_texture_page_draw(uint32 ordering, uint32 mode)
{
    uint32 packet, link;
    FUNCTION_MARKER(0x80032798, "GTMENU.EXE");
    packet = r_u32(0x800A8434);
    mode &= 0xFFFF;
    w_u32(packet, 0x01000000);
    w_u32(packet + 4, mode | 0xE1000000);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, packet, 7);
    w_masked_u32(packet, link, 7);
    w_u32(0x800A8434, packet + 8);
    return packet;
}

void gtmenu_car_info_color_toggle(void)
{
    uint8 selection;
    FUNCTION_MARKER(0x8001C198, "GTMENU.EXE");
    selection = r_u8(0x80058468) ^ 1;
    w_u8(0x80058468, selection);
    gtmenu_model_info_selection_set((sint8)selection);
}

sint32 gtmenu_current_car_preview_initialize(void)
{
    uint32 models, disabled, page, record, id;
    sint32 selected;
    FUNCTION_MARKER(0x8001C1E4, "GTMENU.EXE");
    models = gtmenu_resource_group(13) + 24;
    disabled = r_u8(0x80058461);
    page = r_u16(0x800A77C6);
    w_u8(0x80058464, 0);
    if (disabled) return disabled;
    if (page != 25) return 2;
    w_u8(0x80081F8D, 2);
    selected = gtmenu_current_record_index();
    if (selected < 0) return selected;
    record = gtmenu_record_address(gtmenu_current_record_index());
    id = r_u16(record) & 0xFFF;
    w_u32(0x80081F90, models + 424 * id);
    id = r_u16(record) & 0xFFF;
    w_u16(0x80081F8E, id);
    gtmenu_model_info_bind(0x80081F84, 0x003700BA, 0x00F201A5, 1, 0);
    gtmenu_preview_control_set(0);
    gtmenu_model_info_selection_set(0);
    w_u8(0x80058464, 1);
    w_u8(0x80058468, 0);
    return 1;
}

void gtmenu_parts_price_placeholders_draw(uint32 ordering, uint16 category, uint32 x, uint32 y)
{
    uint32 original_x, position;
    sint32 width;
    FUNCTION_MARKER(0x8001C5A8, "GTMENU.EXE");
    original_x = r_u16(x);
    if ((uint16)(category - 26) >= 16) return;
    gtmenu_text_color_set(0x808080);
    position = r_u16(y) + 30;
    w_u16(y, position);
    width = gtmenu_text_width(0, 0x80058458);
    position = r_u16(x) - (uint32)width;
    w_u16(x, position);
    gtmenu_text_draw(ordering, (sint16)position, r_s16(y), 0x80058458);
    position = r_u16(y) + 30;
    w_u16(y, position);
    width = gtmenu_text_width(0, 0x80058458);
    position = original_x - (uint32)width;
    w_u16(x, position);
    gtmenu_text_draw(ordering, (sint16)position, r_s16(y), 0x80058458);
}

uint32 gtmenu_parts_purchase_commit(void)
{
    sint64 credits, price;
    sint32 status;
    FUNCTION_MARKER(0x8001C320, "GTMENU.EXE");
    credits = (sint64)(sint32)gtmenu_credits_get() * 100;
    price = (sint32)gtmenu_part_adjustment_value(r_u32(0x800823A0));
    if (credits < price) return 2;
    status = gtmenu_part_install(r_u32(0x800823A0));
    if (status == 0 || status == 1) {
        gtmenu_credits_set((uint32)((credits - price) / 100));
        w_u8(0x80058460, 1);
        return status;
    }
    switch (status) {
    case -4: return 6;
    case -3: return 5;
    case -2: return 4;
    case -5: return 7;
    default: return 8;
    }
}

void gtmenu_selected_part_equip(uint32 record)
{
    uint32 node, stage, category, previous, data;
    FUNCTION_MARKER(0x8001C49C, "GTMENU.EXE");
    node = r_u32(0x800823A0);
    stage = r_u16(node + 10);
    category = r_u8(node + 9);
    ++stage;
    switch (category) {
    case 16: case 17: case 18: case 19: case 20:
    case 22: case 23: case 24: case 26: case 29:
    case 31: case 32: case 33: case 34:
        w_u16(record + 2 * (category - 13), stage);
        break;
    case 27:
        previous = r_u16(record + 44);
        w_u16(record + 28, stage);
        if (previous) w_u16(record + 44, 0);
        break;
    case 35:
        previous = r_u16(record + 28);
        w_u16(record + 44, stage);
        if (previous) w_u16(record + 28, 0);
        break;
    }
    node = r_u32(0x800823A0);
    data = r_u32(node + 12);
    gtmenu_part_settings_apply(record, category, data);
}

sint32 gtmenu_power_display_value(uint32 value)
{
    sint32 scaled;
    FUNCTION_MARKER(0x8001F868, "GTMENU.EXE");
    scaled = (sint32)(value * 1000);
    return scaled / 1014;
}

uint32 gtmenu_page_footer_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 page, screen, price = 0, node, text, first_buffer, grouped_buffer;
    uint32 shade, layout, x_address, y_address, pulse, position, input, index;
    sint32 width, power, delta;
    uint32 result;
    FUNCTION_MARKER(0x8001C68C, "GTMENU.EXE");
    page = r_u16(0x800A77C6);
    frame = gtmenu_local_stack_push(288, 0x8001C68C);
    first_buffer = frame.address;
    grouped_buffer = frame.address + 128;
    shade = frame.address + 256;
    layout = frame.address + 264;
    x_address = frame.address + 272;
    y_address = frame.address + 274;
    pulse = frame.address + 280;
    w_u16(x_address, 510);
    w_u16(y_address, 304);
    if (page - 42 < 8) w_u16(y_address, 332);
    else if (page - 1 < 21) w_u16(y_address, 354);
    else if (page - 22 < 4) w_u16(y_address, 365);
    screen = r_u16(0x800A77C4);
    if (screen - 165 >= 490) {
        gtmenu_local_stack_pop(frame);
        return 0;
    }
    node = r_u32(0x800823A0);
    if (node) price = gtmenu_part_adjustment_value(node);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    w_u32(layout, 0x012B0209);
    w_u32(layout + 4, 0x01860264);
    w_u32(shade, 0x012B0209);
    w_u32(shade + 4, 0x01860264);
    text = 0;
    if (r_u8(0x80058461)) text = 0x80058458;
    else if (r_u8(0x80058460)) text = 0x800A753E;
    if (text) {
        gtmenu_text_color_set(0x807040);
        width = gtmenu_text_width(0, text);
        position = 510u - (uint32)width;
        w_u16(x_address, position);
        gtmenu_text_draw(ordering, (sint16)position, r_s16(y_address), text);
        w_u16(x_address, 510);
        gtmenu_parts_price_placeholders_draw(ordering, page, x_address, y_address);
        gtmenu_rectangle_shade(ordering, shade, 96);
    } else {
        gtmenu_format_integer(first_buffer, 0x80053ECC, price / 100);
        gtmenu_decimal_group(grouped_buffer, first_buffer);
        width = gtmenu_text_width(0, grouped_buffer);
        position = 510u - (uint32)width;
        w_u16(x_address, position);
        gtmenu_text_draw(ordering, (sint16)position, r_s16(y_address), grouped_buffer);
        if (page - 26 < 16) {
            for (index = 0; index < 2; ++index) {
                position = r_u16(y_address) + 30;
                input = r_u16(index ? 0x80082334 : 0x800822C4);
                w_u16(y_address, position);
                power = gtmenu_power_display_value(input);
                gtmenu_format_integer_string(first_buffer, 0x80053ED0, power, 0x800A7556);
                width = gtmenu_text_width(0, first_buffer);
                position = 510u - (uint32)width;
                w_u16(x_address, position);
                gtmenu_text_draw(ordering, (sint16)position, r_s16(y_address), first_buffer);
            }
        }
        if ((sint16)page == 25) {
            w_u32(pulse, 0x00DB0062);
            w_u32(pulse + 4, 0x010A00B9);
            gtmenu_rectangle_pulse(ordering, pulse, 0);
        }
    }
    result = 160;
    if (r_u8(0x80058464)) {
        delta = r_s16(0x80058466);
        w_u32(layout, 0x003700A0);
        w_u32(layout + 4, 0x00F201CA);
        gtmenu_model_info_angle_add((uint32)delta);
        gtmenu_color_picker_bounds_set(layout);
        result = gtmenu_model_preview_frame_draw();
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_parts_purchase_open(void)
{
    uint32 disabled, page, mode, caption;
    FUNCTION_MARKER(0x8001CA90, "GTMENU.EXE");
    disabled = r_u8(0x80058461);
    page = r_u16(0x800A77C6);
    if (disabled || r_u8(0x80058462) || r_u8(0x80058460) || !r_u32(0x800823A0)) {
        gtmenu_sound_effect(6);
        return 0;
    }
    gtmenu_sound_effect(5);
    mode = page - 22 < 4 || page - 33 < 3;
    caption = gtmenu_part_caption(r_u32(0x800823A0), 0);
    gtmenu_model_purchase_open(mode, caption, (sint16)r_u16(0x800A77C4), 1);
    return 1;
}

uint32 gtmenu_available_parts_initialize(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 archive, page, make, record, current, proposed, model, category, variant = 0;
    uint32 resource, entries, entry, index, selected, stride, id_offset, kind_offset = 0;
    uint32 price_offset, kind_check = 0, field, special = 0, installed = 0, stage, data, owned;
    FUNCTION_MARKER(0x8001CB74, "GTMENU.EXE");
    archive = gtmenu_resource_archive();
    page = r_u16(0x800A77C6);
    make = r_u16(0x800A77C2);
    w_u32(0x800823A0, 0);
    w_u8(0x80058462, 0);
    if (gtmenu_current_record_index() < 0) {
        w_u8(0x80058461, 1);
        return 0;
    }
    record = gtmenu_record_address(gtmenu_current_record_index());
    gtmenu_resource_group(13);
    if (!gtmenu_record_matches_make(record, make)) {
        w_u8(0x80058461, 1);
        w_u8(0x80058463, 1);
        return 0;
    }
    w_u8(0x80058461, 0);
    w_u8(0x80058463, 0);
    frame = gtmenu_local_stack_push(328, 0x8001CB74);
    current = frame.address;
    proposed = frame.address + 96;
    gtmenu_memory_copy(current, record, 96);
    w_u16(current, r_u16(current) + 1);
    gtmenu_memory_copy(proposed, current, 96);
    model = r_u16(current) & 0xFFF;
    switch (page) {
    case 1: category = 16; break;
    case 2: category = 17; break;
    case 3: case 4: case 5: category = 33; variant = page - 3; break;
    case 6: case 7: case 8: category = 31; variant = page - 6; break;
    case 9: case 10: case 11: category = 32; variant = page - 9; break;
    case 12: case 13: case 14: category = 23; variant = page - 12; break;
    case 15: case 16: case 17: category = 18; variant = page - 15; break;
    case 18: case 19: case 20: category = 22; variant = page - 18; break;
    case 21: category = 29; break;
    case 22: case 23: case 24: category = 25; variant = page - 22; break;
    case 25: category = 30; break;
    case 26: case 27: case 28: category = 26; variant = page - 26; break;
    case 29: category = 20; break;
    case 30: case 31: case 32: category = 27; variant = page - 30; break;
    case 33: category = 28; break;
    case 34: category = 15; break;
    case 35: category = 21; break;
    case 36: case 37: category = 24; variant = page - 36; break;
    case 38: case 39: case 40: case 41: category = 35; variant = page - 38; break;
    case 42: category = 34; variant = 3; break;
    case 43: category = 34; variant = 1; break;
    case 44: category = 34; variant = 2; break;
    case 45: category = 34; break;
    case 46: category = 34; variant = 7; break;
    case 47: category = 34; variant = 5; break;
    case 48: category = 34; variant = 6; break;
    case 49: category = 34; variant = 4; break;
    default:
        gtmenu_local_stack_pop(frame);
        return 0;
    }
    resource = gtmenu_resource_group(category);
    entries = resource + 24;
    gtmenu_memory_fill(0x80081F9C, 0, 24);
    w_u8(0x80081FA4, 1);
    w_u8(0x80081FA5, category);
    field = 2 * (category - 13);
    switch (category) {
    case 15: case 21: case 28:
        stride = 32; id_offset = 16; price_offset = 20; break;
    case 16:
        stride = 24; id_offset = 4; price_offset = 8; break;
    case 17:
        stride = 28; id_offset = 10; kind_offset = 8; price_offset = 16; kind_check = 2; break;
    case 18:
        stride = 24; id_offset = 6; kind_offset = 8; price_offset = 12; kind_check = 1; break;
    case 20:
        stride = 32; id_offset = 16; kind_offset = 18; price_offset = 20; kind_check = 2; break;
    case 23:
        stride = 68; id_offset = 50; kind_offset = 1; price_offset = 56; kind_check = 1; break;
    case 24:
        stride = 32; id_offset = 16; kind_offset = 18; price_offset = 20; kind_check = 1; break;
    case 25:
        stride = 20; id_offset = 2; kind_offset = 1; price_offset = 4; kind_check = 1; break;
    case 26:
        stride = 36; id_offset = 16; kind_offset = 18; price_offset = 24; kind_check = 1; break;
    case 27:
        stride = 36; id_offset = 16; kind_offset = 19; price_offset = 20; kind_check = 1; break;
    case 30:
        stride = 32; id_offset = 14; price_offset = 16; break;
    case 33:
        stride = 72; id_offset = 54; kind_offset = 56; price_offset = 60; kind_check = 1; break;
    case 35:
        stride = 56; id_offset = 26; kind_offset = 43; price_offset = 44; kind_check = 1; break;
    default:
        stride = 20; id_offset = 4; kind_offset = 6; price_offset = 8; kind_check = 1; break;
    }
    selected = 0xFFFFFFFF;
    for (index = 0; index < r_u16(resource + 14); ++index) {
        entry = entries + stride * index;
        stage = r_u16(entry + id_offset);
        if (category == 15 || category == 21 || category == 25 || category == 28 || category == 30) special = 1;
        if (stage != model) continue;
        if (kind_check == 1 && r_u8(entry + kind_offset) != variant) continue;
        if (kind_check == 2 && r_u8(entry + kind_offset) == 255) continue;
        if (!r_u32(entry + price_offset)) continue;
        selected = index;
        w_u32(0x80081FA8, entry);
        w_u16(proposed + field, index + 1);
        if (category == 27) w_u16(proposed + 44, 0);
        if (category == 35) w_u16(proposed + 28, 0);
        if (special) {
            if (category == 25) {
                stage = r_u16(record + 24);
                if (stage) installed = r_u8(entries + 20 * stage - 19) >= variant;
                else installed = r_u16(record + 34) != 0;
            } else installed = r_u16(record + field) != 0;
        }
        break;
    }
    gtmenu_settings_from_record(0x80081FB4, current, archive);
    gtmenu_model_stats_build(0x80081FB4, frame.address + 192, 0x800822C4);
    if (selected == 0xFFFFFFFF) {
        w_u8(0x80058461, 1);
        gtmenu_local_stack_pop(frame);
        return 0;
    }
    data = r_u32(0x80081FA8);
    w_u16(0x80081FA6, selected);
    gtmenu_part_settings_apply(proposed, category, data);
    gtmenu_settings_from_record(0x8008213C, proposed, archive);
    gtmenu_model_stats_build(0x8008213C, frame.address + 256, 0x80082334);
    data = gtmenu_owned_parts_category(0x8009DF8C, category);
    owned = (r_u8(data + (selected >> 3)) >> (selected & 7)) & 1;
    w_u8(0x80058460, owned && (!special || installed));
    w_u32(0x800823A0, 0x80081F9C);
    gtmenu_local_stack_pop(frame);
    return 1;
}

uint32 gtmenu_current_power(void)
{
    FUNCTION_MARKER(0x8001D674, "GTMENU.EXE");
    return r_u16(0x800822C4);
}

uint32 gtmenu_parts_baseline_update(uint32 record)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 archive, result;
    FUNCTION_MARKER(0x8001D694, "GTMENU.EXE");
    archive = gtmenu_resource_archive();
    frame = gtmenu_local_stack_push(168, 0x8001D694);
    gtmenu_memory_copy(frame.address, record, 96);
    w_u16(frame.address, r_u16(frame.address) + 1);
    gtmenu_settings_from_record(0x80081FB4, frame.address, archive);
    result = gtmenu_model_stats_build(0x80081FB4, frame.address + 96, 0x800822C4);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_ordering_arrays_allocate(void)
{
    uint32 first, second, bytes;
    FUNCTION_MARKER(0x8001D790, "GTMENU.EXE");
    first = gtmenu_heap_alloc(r_u32(0x800823F8) * 4);
    bytes = r_u32(0x800823F8) * 4;
    w_u32(0x800823FC, first);
    second = gtmenu_heap_alloc(bytes);
    w_u32(0x80082400, second);
    return second;
}

uint32 gtmenu_primitive_arrays_allocate(void)
{
    uint32 first, second, bytes;
    FUNCTION_MARKER(0x8001D7D4, "GTMENU.EXE");
    first = gtmenu_heap_alloc(r_u32(0x80082404) * 4);
    bytes = r_u32(0x80082404) * 4;
    w_u32(0x80082408, first);
    second = gtmenu_heap_alloc(bytes);
    w_u32(0x8008240C, second);
    return second;
}

uint32 gtmenu_render_lists_initialize(uint32 ordering_count, uint32 primitive_count)
{
    uint32 first, second, count;
    FUNCTION_MARKER(0x8001D818, "GTMENU.EXE");
    w_u32(0x800823F8, ordering_count);
    gtmenu_ordering_arrays_allocate();
    first = r_u32(0x800823FC);
    second = r_u32(0x80082400);
    count = r_u32(0x800823F8);
    gtmenu_ordering_arrays_bind(first, second, count);
    w_u32(0x80082404, primitive_count);
    gtmenu_primitive_arrays_allocate();
    first = r_u32(0x80082408);
    second = r_u32(0x8008240C);
    count = r_u32(0x80082404);
    gtmenu_primitive_arrays_bind(first, second, count);
    w_u32(0x800823B0, 1);
    w_u32(0x800823AC, 0);
    gtmenu_render_list_insert(0x800823A4);
    w_u32(0x800823CC, 1024);
    w_u32(0x800823C8, 1);
    gtmenu_render_list_insert(0x800823C0);
    w_u32(0x800823E8, 128);
    w_u32(0x800823E4, 2);
    return gtmenu_render_list_insert(0x800823DC);
}

uint32 gtmenu_render_lists_release(void)
{
    FUNCTION_MARKER(0x8001D8BC, "GTMENU.EXE");
    gtmenu_render_list_remove(0x800823DC);
    gtmenu_render_list_remove(0x800823C0);
    gtmenu_render_list_remove(0x800823A4);
    gtmenu_heap_free(r_u32(0x8008240C));
    gtmenu_heap_free(r_u32(0x80082408));
    gtmenu_heap_free(r_u32(0x80082400));
    return gtmenu_heap_free(r_u32(0x800823FC));
}

uint32 gtmenu_secondary_ordering(void)
{
    FUNCTION_MARKER(0x8001D94C, "GTMENU.EXE");
    return r_u32(0x800823D8);
}

uint32 gtmenu_primitive_arrays_bind(uint32 first, uint32 second, uint32 count)
{
    FUNCTION_MARKER(0x800328A0, "GTMENU.EXE");
    w_u32(0x800A8440, first);
    w_u32(0x800A8444, second);
    w_u32(0x800A8430, count);
    return 0x800A8440;
}

uint32 gtmenu_render_pool_initialize(uint32 first, uint32 second, uint32 capacity)
{
    FUNCTION_MARKER(0x80036A6C, "GTMENU.EXE");
    w_u32(0x800A8590, 0);
    w_u32(0x800A8594, first);
    w_u32(0x800A8598, second);
    w_u32(0x800A859C, 0);
    w_u32(0x800A85A0, capacity);
    return 0x800A8590;
}

uint32 gtmenu_render_lists_partition(void)
{
    uint32 first, second, node, first_count, second_count, result = 0x800A8594;
    FUNCTION_MARKER(0x80036A8C, "GTMENU.EXE");
    first = r_u32(0x800A8594);
    node = r_u32(0x800A8590);
    second = r_u32(0x800A8598);
    while (node) {
        first_count = r_u32(node + 12);
        second_count = r_u32(node + 12);
        w_u32(node + 24, first);
        w_u32(node + 16, first);
        w_u32(node + 20, second);
        node = r_u32(node + 4);
        result = first_count * 4;
        first += result;
        second += second_count * 4;
    }
    return result;
}

uint32 gtmenu_ordering_arrays_bind(uint32 first, uint32 second, uint32 count)
{
    FUNCTION_MARKER(0x80032878, "GTMENU.EXE");
    gtmenu_render_pool_initialize(first, second, count);
    return gtmenu_display_buffers_clear(2);
}

uint32 gtmenu_render_list_insert(uint32 node)
{
    uint32 used, requested, capacity, previous = 0, next;
    sint32 priority;
    FUNCTION_MARKER(0x800328C0, "GTMENU.EXE");
    used = r_u32(0x800A859C);
    requested = r_u32(node + 12);
    capacity = r_u32(0x800A85A0);
    if (capacity < used + requested) return 0;
    used = r_u32(0x800A859C);
    requested = r_u32(node + 12);
    next = r_u32(0x800A8590);
    w_u32(0x800A859C, used + requested);
    if (next) {
        priority = r_s32(node + 8);
        do {
            if (priority < r_s32(next + 8)) break;
            previous = next;
            next = r_u32(next + 4);
        } while (next);
    }
    if (previous) w_u32(previous + 4, node);
    else w_u32(0x800A8590, node);
    if (next) w_u32(next, node);
    w_u32(node, previous);
    w_u32(node + 4, next);
    gtmenu_render_lists_partition();
    return 1;
}

uint32 gtmenu_render_list_remove(uint32 node)
{
    uint32 used, requested, previous, next;
    FUNCTION_MARKER(0x80032988, "GTMENU.EXE");
    used = r_u32(0x800A859C);
    requested = r_u32(node + 12);
    w_u32(0x800A859C, used - requested);
    previous = r_u32(node);
    next = r_u32(node + 4);
    if (previous) w_u32(previous + 4, next);
    else w_u32(0x800A8590, next);
    if (next) w_u32(next, previous);
    return gtmenu_render_lists_partition();
}

uint32 gtmenu_model_info_texture_load(uint32 name)
{
    FUNCTION_MARKER(0x8001DB00, "GTMENU.EXE");
    return gtmenu_menu_texture_load(name, 0x01E50280, -1, 0x010003C0, 0x00800020, 1);
}
