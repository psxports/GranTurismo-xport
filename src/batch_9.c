#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gtmenu_text_surface_state_clear(uint32 state)
{
    FUNCTION_MARKER(0x80034B5C, "GTMENU.EXE");
    gtmenu_memory_fill(state, 0, 48);
    w_u32(state + 36, 0x808080);
    w_u32(state + 40, 15);
    return 15;
}

uint32 gtmenu_text_surface_bounds_set(uint32 state, uint32 rectangle)
{
    uint32 x, y, width, height, flags, stride;
    FUNCTION_MARKER(0x80034B9C, "GTMENU.EXE");
    x = r_u16(rectangle);
    y = r_u16(rectangle + 2);
    width = r_u16(rectangle + 4);
    height = r_u16(rectangle + 6);
    flags = r_u32(state + 24);
    w_u16(state + 28, 0);
    w_u16(state + 30, 0);
    flags = (flags & 0xFFE0FFFF) | (((x >> 6) & 15) << 16) | (((y >> 8) & 1) << 20);
    width &= 0xFFFE;
    stride = width * 4;
    w_u16(state + 20, width);
    w_u16(state + 16, x);
    w_u16(state + 18, y);
    w_u16(state + 22, height);
    w_u16(state + 32, stride);
    w_u16(state + 34, height);
    w_u32(state + 24, flags);
    w_u16(state + 44, (sint16)width >> 1);
    return stride;
}

uint32 gtmenu_render_mode_set(uint8 mode)
{
    uint32 index, base, description, display, drawing, dimensions;
    sint32 x, y, width, height;
    FUNCTION_MARKER(0x8003491C, "GTMENU.EXE");
    base = mode & 7;
    description = 0x80074880 + base * 20;
    w_u32(0x800A8450, base);
    width = r_u16(description);
    w_u16(0x800A8458, 0);
    w_u16(0x800A845A, 0);
    w_u32(0x800A8454, width);
    w_u16(0x800A845C, r_u16(description + 2));
    w_u16(0x800A845E, r_u16(description + 4));
    w_u16(0x800A8460, r_u16(description + 6));
    w_u16(0x800A8462, r_u16(description + 8));
    w_u16(0x800A8464, r_u16(description + 2));
    w_u16(0x800A8466, r_u16(description + 4));
    if (mode & 0x20) SetVideoMode(1);
    for (index = 0; index < 2; ++index) {
        dimensions = 0x800A8458 + index * 8;
        display = 0x800A8468 + index * 20;
        drawing = 0x800A8490 + index * 92;
        x = r_s16(dimensions);
        y = r_s16(dimensions + 2);
        width = r_s16(dimensions + 4);
        height = r_s16(dimensions + 6);
        SetDefDispEnv((DISPENV *)psx_addr(display, sizeof(DISPENV)), x, y, width, height);
        x = r_s16(dimensions);
        y = r_s16(dimensions + 2);
        width = r_s16(dimensions + 4);
        height = r_s16(dimensions + 6);
        SetDefDrawEnv((DRAWENV *)psx_addr(drawing, sizeof(DRAWENV)), x, y, width, height);
        if (mode & 0x10) w_u8(0x800A8479 + index * 20, 1);
        if (mode & 8) w_u8(0x800A8478 + index * 20, 1);
        if (mode & 0x20) w_u16(0x800A8472 + index * 20, base == 7 ? 16 : 24);
    }
    if (!(mode & 0x40)) {
        w_u32(0x800A8548, 0);
        w_u32(0x800A854C, 1);
    }
    w_u32(0x800A8554, 1);
    return 1;
}

uint32 gtmenu_renderer_reset(void)
{
    FUNCTION_MARKER(0x80034B34, "GTMENU.EXE");
    gtmenu_gpu_graph_reset(1);
    return gtmenu_render_environments_bind();
}

uint32 gtmenu_render_display_apply(uint32 alternate)
{
    uint32 index, environment;
    FUNCTION_MARKER(0x8003EC90, "GTMENU.EXE");
    index = r_u32(alternate ? 0x800A854C : 0x800A8548);
    environment = 0x800A8468 + index * 20;
    PutDispEnv((DISPENV *)psx_addr(environment, sizeof(DISPENV)));
    return environment;
}

uint32 gtmenu_render_drawing_apply(uint32 alternate)
{
    uint32 index, environment;
    FUNCTION_MARKER(0x8003ECE4, "GTMENU.EXE");
    index = r_u32(alternate ? 0x800A854C : 0x800A8548);
    environment = 0x800A8490 + index * 92;
    PutDrawEnv((DRAWENV *)psx_addr(environment, sizeof(DRAWENV)));
    return environment;
}

uint32 gtmenu_render_environments_bind(void)
{
    FUNCTION_MARKER(0x800369C8, "GTMENU.EXE");
    gtmenu_render_display_apply(0);
    return gtmenu_render_drawing_apply(1);
}

uint32 gtmenu_license_panel_pulse_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 category, page, result = 19;
    FUNCTION_MARKER(0x80022F50, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x80022F50);
    w_u32(frame.address, 0x003A0222);
    w_u32(frame.address + 4, 0x005D0267);
    w_u32(frame.address + 8, 0x003A0222);
    w_u32(frame.address + 12, 0x00810267);
    category = r_u16(0x800A77C6);
    page = r_u16(0x800A77C4);
    if (category - 50 < 2 || page == 18) gtmenu_rectangle_pulse(ordering, frame.address, 0);
    if (page == 19) result = gtmenu_rectangle_pulse(ordering, frame.address + 8, 0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_page_hint_draw(uint32 ordering, uint32 mode)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result = 2;
    FUNCTION_MARKER(0x80023020, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x80023020);
    w_u32(frame.address, 0x005A0222);
    w_u32(frame.address + 4, 0x007A0263);
    w_u32(frame.address + 8, 0x003C0225);
    w_u32(frame.address + 12, 0x005A0262);
    if (!mode) result = gtmenu_rectangle_shade(ordering, frame.address, 255);
    else if (mode == 2) result = gtmenu_rectangle_pulse(ordering, frame.address + 8, 0);
    gtmenu_local_stack_pop(frame);
    return result;
}

sint32 gtmenu_resource_names_find(uint32 table, uint32 name)
{
    sint32 index = 0, count;
    FUNCTION_MARKER(0x80023134, "GTMENU.EXE");
    count = r_s32(table);
    while (index < count) {
        if (!gtmenu_string_compare_folded(name, r_u32(table + 8 + (uint32)index * 4))) return index;
        count = r_s32(table);
        ++index;
    }
    return -1;
}

uint32 gtmenu_input_state_address(uint32 secondary)
{
    FUNCTION_MARKER(0x80024410, "GTMENU.EXE");
    return secondary ? 0x8009AB1C : 0x8009AACC;
}

void gtmenu_input_prefix_clear(uint32 state)
{
    FUNCTION_MARKER(0x800419A8, "GTMENU.EXE");
    w_u16(state, 0);
    w_u16(state + 2, 0);
    w_u16(state + 4, 0);
    w_u16(state + 6, 0);
}

uint32 gtmenu_controller_repeat_initialize(uint32 state)
{
    uint32 index, value, destination, part;
    FUNCTION_MARKER(0x80041228, "GTMENU.EXE");
    gtmenu_input_prefix_clear(state);
    w_u16(state + 8, 0);
    for (index = 0; index < 16; ++index) {
        value = r_u32(0x8007B560);
        destination = state + 10 + index * 4;
        part = (destination + 3) & 3;
        w_masked_u32((destination + 3) & ~3, value >> ((3 - part) * 8), (1 << (part + 1)) - 1);
        part = destination & 3;
        w_masked_u32(destination & ~3, value << (part * 8), (15 << part) & 15);
    }
    return 64;
}

uint32 gtmenu_pointer_state_initialize(uint32 state)
{
    FUNCTION_MARKER(0x80041288, "GTMENU.EXE");
    gtmenu_memory_fill(state, 0, 32);
    gtmenu_input_prefix_clear(state);
    w_u16(state + 12, 15);
    w_u8(state + 28, 50);
    w_u8(state + 29, 50);
    w_u8(state + 30, 50);
    w_u8(state + 31, 1);
    return 1;
}

uint32 gtmenu_input_initialize(void)
{
    uint32 width, height, clip_width, clip_height, result;
    FUNCTION_MARKER(0x8002325C, "GTMENU.EXE");
    gtmenu_controller_repeat_initialize(gtmenu_input_state_address(0));
    gtmenu_controller_repeat_initialize(gtmenu_input_state_address(1));
    gtmenu_pointer_state_initialize(0x800A7B8C);
    w_u16(0x800A7B94, 320);
    w_u16(0x800A7B96, 154);
    width = r_u16(0x800A75C4);
    height = r_u16(0x800A75C6);
    width = (width - 32) & 0xFFFF;
    height = (height - 32) & 0xFFFF;
    clip_width = r_u16(0x800A7594);
    clip_height = r_u16(0x800A7596);
    w_u32(0x800A77A0, 0);
    w_u32(0x800A77A4, width | (height << 16));
    w_u32(0x800A77A8, 0);
    w_u32(0x800A77AC, clip_width | (clip_height << 16));
    w_u16(0x800A7BA0, 0);
    result = width - 1;
    w_u16(0x800A7BA2, 0);
    w_u16(0x800A7BA4, result);
    w_u16(0x800A7BA6, height - 1);
    return result;
}

uint32 gtmenu_link_store(uint32 index, uint32 name, uint32 value, uint32 upper)
{
    uint32 offset, result;
    FUNCTION_MARKER(0x8002337C, "GTMENU.EXE");
    offset = index * 68;
    result = gtmenu_string_copy(0x8008E994 + offset, name);
    w_u32(0x8008E9D4 + offset, value | (upper << 16));
    return result;
}

sint32 gtmenu_link_find(uint32 output, uint32 name)
{
    uint32 index, entry;
    FUNCTION_MARKER(0x800233E0, "GTMENU.EXE");
    for (index = 0; index < 32; ++index) {
        entry = 0x8008E994 + index * 68;
        if (!gtmenu_string_compare(entry, name)) {
            w_u32(output, r_u32(entry + 64));
            return index;
        }
    }
    return -1;
}

uint32 gtmenu_links_initialize(void)
{
    FUNCTION_MARKER(0x80023484, "GTMENU.EXE");
    w_u8(0x80058B04, 0);
    return gtmenu_memory_copy(0x8008E994, 0, 2176);
}

uint32 gtmenu_link_add(uint32 name, uint32 value, uint32 upper)
{
    uint8 next;
    uint32 result;
    FUNCTION_MARKER(0x800234B8, "GTMENU.EXE");
    gtmenu_link_store(r_s8(0x80058B04), name, value, upper);
    next = r_u8(0x80058B04) + 1;
    w_u8(0x80058B04, next);
    result = (sint8)next < 32;
    if (!result)
        w_u8(0x80058B04, 0);
    return result;
}

uint32 gtmenu_text_surface_address(void)
{
    FUNCTION_MARKER(0x8002393C, "GTMENU.EXE");
    return 0x8008F214;
}

void gtmenu_layout_release(void)
{
    uint32 list, node, next;
    FUNCTION_MARKER(0x80023974, "GTMENU.EXE");
    list = r_u32(0x800A79D0);
    node = r_u32(list);
    while (node) {
        next = r_u32(node + 4);
        gtmenu_widget_node_release(node);
        node = next;
    }
    w_u32(list + 4, 0);
    w_u32(list, 0);
}

uint32 gtmenu_layout_initialize(void)
{
    uint32 node, next;
    FUNCTION_MARKER(0x800239D0, "GTMENU.EXE");
    node = r_u32(0x800A7A08);
    while (node) {
        next = r_u32(node + 4);
        gtmenu_heap_free(node);
        node = next;
    }
    w_u32(0x800A7A0C, 0);
    w_u32(0x800A7A08, 0);
    return 0x800A7A08;
}

uint32 gtmenu_hitboxes_defaults_initialize(void)
{
    uint32 width, height;
    FUNCTION_MARKER(0x80023A20, "GTMENU.EXE");
    gtmenu_memory_fill(0x8009924C, 0, 400);
    width = r_u16(0x800A7560);
    height = r_u16(0x800A7568);
    w_u32(0x80099248, 0);
    w_u32(0x80099244, 0x80099248);
    w_u16(0x800993DC, width);
    w_u16(0x800993DE, height);
    return width;
}

uint32 gtmenu_hitboxes_bind(uint32 table)
{
    uint32 records, count;
    FUNCTION_MARKER(0x80023A70, "GTMENU.EXE");
    if (table) {
        records = table + 4;
        w_u32(0x80099244, table);
        count = r_u32(table);
    } else {
        w_u32(0x80099244, 0x80099248);
        count = r_u32(0x80099248);
        records = 0x8009924C;
    }
    w_u32(0x800A7570, records);
    w_u32(0x800A7564, count);
    return records;
}

uint32 gtmenu_input_direction(uint32 buttons)
{
    uint32 index, direction;
    FUNCTION_MARKER(0x80023ACC, "GTMENU.EXE");
    index = (buttons & 0x10) != 0;
    if (buttons & 0x40) index |= 2;
    if (buttons & 0x80) index |= 4;
    if (buttons & 0x20) index |= 8;
    direction = r_u8(0x80058B08 + index);
    if (direction != 5)
        return direction;
    index = (buttons & 0x400) != 0;
    if (buttons & 0x100) index |= 2;
    if (buttons & 0x800) index |= 4;
    if (buttons & 0x200) index |= 8;
    return r_u8(0x80058B18 + index);
}

sint32 gtmenu_pointer_center_hitbox(uint32 index)
{
    uint32 entry, extent, origin;
    sint16 x, y;
    FUNCTION_MARKER(0x80023B7C, "GTMENU.EXE");
    entry = r_u32(0x80099244) + index * 8;
    extent = r_u16(entry + 8);
    origin = r_u16(entry + 4);
    x = origin + (extent >> 1);
    w_u16(0x800A7B94, x);
    extent = r_u16(entry + 10);
    origin = r_u16(entry + 6);
    y = origin + (extent >> 1);
    w_u16(0x800A7B96, y);
    return gtmenu_cursor_nearest(x, y);
}

sint32 gtmenu_pointer_position_set(sint16 x, sint16 y)
{
    FUNCTION_MARKER(0x80023BE8, "GTMENU.EXE");
    w_u16(0x800A7B94, x);
    w_u16(0x800A7B96, y);
    return gtmenu_cursor_position(x, y);
}

uint32 gtmenu_widget_node_release(uint32 node)
{
    uint32 widget;
    FUNCTION_MARKER(0x80028AC4, "GTMENU.EXE");
    widget = r_u32(node);
    if (r_u32(widget) == 0x8004) {
        gtmenu_widget_groups_release(widget);
        widget = r_u32(node);
    }
    gtmenu_heap_free(widget);
    return gtmenu_heap_free(node);
}

void gtmenu_widget_groups_release(uint32 widget)
{
    uint32 group, next_group, node, next_node;
    FUNCTION_MARKER(0x8002ADA0, "GTMENU.EXE");
    group = r_u32(widget + 112);
    while (group) {
        node = r_u32(group + 24);
        next_group = r_u32(group + 32);
        while (node) {
            next_node = r_u32(node + 4);
            gtmenu_widget_node_release(node);
            node = next_node;
        }
        gtmenu_heap_free(group);
        group = next_group;
    }
}

uint32 gtmenu_layout_rect_count(void)
{
    uint32 owner, node, count = 0;
    FUNCTION_MARKER(0x80023C74, "GTMENU.EXE");
    owner = r_u32(0x800A7A10);
    while (owner) {
        node = r_u32(owner + 40);
        while (node) {
            node = r_u32(node);
            ++count;
        }
        owner = r_u32(owner);
    }
    return count;
}

uint32 gtmenu_configuration_defaults_initialize(void)
{
    FUNCTION_MARKER(0x80023D48, "GTMENU.EXE");
    w_u32(0x800A7580, 2);
    w_u16(0x800A7584, 2049);
    w_u32(0x800A7588, 0x4000);
    w_u16(0x800A7596, 56);
    w_u16(0x800A7598, 10);
    w_u16(0x800A759A, 630);
    w_u16(0x800A759C, 42);
    w_u16(0x800A759E, 470);
    w_u16(0x800A75A0, 16);
    w_u16(0x800A75A2, 16);
    w_u16(0x800A75A4, 3);
    w_u16(0x800A75A6, 5);
    w_u16(0x800A75A8, 32);
    w_u16(0x800A75AA, 32);
    w_u16(0x800A75AC, 28);
    w_u16(0x800A75AE, 28);
    w_u16(0x800A75B0, 24);
    w_u16(0x800A75B2, 24);
    w_u16(0x800A75B4, 20);
    w_u16(0x800A75B6, 20);
    w_u16(0x800A75B8, 18);
    w_u16(0x800A75BA, 18);
    w_u16(0x800A75BC, 4);
    w_u16(0x800A75BE, 2);
    w_u16(0x800A75C0, 2);
    w_u16(0x800A75C2, 2);
    w_u16(0x800A75C4, 624);
    w_u16(0x800A75C6, 448);
    w_u16(0x800A7590, 640);
    w_u16(0x800A7592, 480);
    w_u16(0x800A7594, 640);
    w_u16(0x800A75C8, 984);
    w_u16(0x800A75CA, 384);
    w_u16(0x800A75CC, 640);
    w_u16(0x800A75D0, 672);
    w_u16(0x800A75EE, 481);
    w_u16(0x800A75FC, 832);
    w_u16(0x800A75FE, 256);
    w_u16(0x800A7602, 511);
    w_u16(0x800A760E, 50);
    w_u16(0x800A7610, 400);
    w_u16(0x800A75CE, 480);
    w_u16(0x800A75D2, 480);
    w_u16(0x800A75E8, 640);
    w_u16(0x800A75EA, 0);
    w_u16(0x800A75EC, 640);
    w_u16(0x800A7600, 640);
    w_u16(0x800A760C, 0);
    w_u16(0x800A7612, 240);
    gtmenu_string_copy(0x800A7614, gtmenu_environment_value(0x80054DAC));
    gtmenu_string_copy(0x800A762C, gtmenu_environment_value(0x80054DB8));
    gtmenu_string_copy(0x800A7644, 0x80054DC8);
    gtmenu_string_copy(0x800A765C, 0x80054DD0);
    gtmenu_string_copy(0x800A7674, 0x80054DD8);
    gtmenu_string_copy(0x800A768C, 0x80054DE4);
    gtmenu_string_copy(0x800A76A4, 0x80054DD0);
    gtmenu_string_copy(0x800A76BC, 0x80054DF4);
    gtmenu_string_copy(0x800A76D4, 0x80054E00);
    gtmenu_string_copy(0x800A76EC, 0x80054E0C);
    gtmenu_string_copy(0x800A7704, 0x80054E14);
    gtmenu_string_copy(0x800A771C, 0x80054E24);
    gtmenu_string_copy(0x800A7734, 0x80054E2C);
    gtmenu_string_copy(0x800A774C, 0x80054E34);
    gtmenu_string_copy(0x800A7764, 0x80054E40);
    return gtmenu_string_copy(0x800A777C, 0x80054E4C);
}

uint32 gtmenu_page_resource_load(uint32 name)
{
    sint32 index;
    uint32 archive;
    FUNCTION_MARKER(0x80023FC0, "GTMENU.EXE");
    index = gtmenu_resource_names_find(0x800993EC, name);
    archive = r_u32(0x800993E4);
    w_u32(0x800A7B44, index);
    return gtmenu_archive_entry_unpack(archive, index, 0x8009A6B4);
}

uint32 gtmenu_page_resource_index_load(sint32 index)
{
    FUNCTION_MARKER(0x80024010, "GTMENU.EXE");
    w_u32(0x800A7B44, index);
    return gtmenu_archive_entry_unpack(r_u32(0x800993E4), index, 0x8009A6B4);
}

uint32 gtmenu_archive_entry_unpack(uint32 archive, sint32 index, uint32 output)
{
    uint32 entry, offset;
    sint32 length;
    FUNCTION_MARKER(0x80034C50, "GTMENU.EXE");
    entry = archive + (uint32)index * 12;
    offset = r_u32(entry + 16);
    length = r_s32(entry + 24);
    return gtmenu_lz_unpack(output, archive + offset, length);
}

uint32 gtmenu_page_resources_initialize(void)
{
    uint32 archive, offset, result;
    FUNCTION_MARKER(0x8002404C, "GTMENU.EXE");
    archive = r_u32(0x800A7AB4);
    offset = r_u32(archive + 16);
    gtmenu_resource_names_index(0x800993EC, archive + offset);
    archive = r_u32(0x800A7AB4);
    offset = r_u32(archive + 76);
    result = archive + offset;
    w_u32(0x800993E4, result);
    return result;
}

uint32 gtmenu_system_textures_load(void)
{
    uint32 allocation, image, next, first, second, clut_position, pixel_position, index;
    FUNCTION_MARKER(0x800240A0, "GTMENU.EXE");
    allocation = gtmenu_menu_asset_load(0x80054E58);
    image = allocation;
    for (index = 0; index < 27; ++index) {
        next = r_u32(image + 8);
        first = r_u32(0x80058B2C + index * 4);
        second = r_u32(0x80058B94 + index * 4);
        clut_position = (uint32)(sint32)(sint16)first | (first & 0xFFFF0000);
        pixel_position = (uint32)(sint32)(sint16)second | (second & 0xFFFF0000);
        next += image + 8;
        gtmenu_tim_upload(image, clut_position, 0xFFFFFFFF, pixel_position, 0xFFFFFFFF, 0);
        image = next + r_u32(next);
    }
    return gtmenu_heap_free(allocation);
}

uint32 gtmenu_pointer_pulse_draw(uint32 ordering, uint16 x, uint16 y)
{
    uint32 clut_x, clut_y, width, u, direction, v, height, intensity;
    uint32 uv_clut, size, color, step, value;
    FUNCTION_MARKER(0x800242DC, "GTMENU.EXE");
    clut_x = r_u16(0x80058B74);
    width = r_u16(0x80058C44);
    clut_y = r_u16(0x80058B76);
    u = r_s16(0x80058BDC);
    direction = r_u32(0x80058C64);
    v = r_u8(0x80058BDE);
    height = r_s16(0x80058C46);
    uv_clut = ((u - 960) * 4) & 255;
    uv_clut |= v << 8;
    uv_clut |= ((uint32)((sint16)clut_x >> 4) | (clut_y << 6)) << 16;
    size = width | (height << 16);
    intensity = r_u8(0x80058C68);
    color = 0x03000080 | (intensity << 8);
    gtmenu_textured_rectangle_packet(ordering, x | ((uint32)y << 16), uv_clut, size, color);
    gtmenu_texture_page_draw(ordering, 607);
    step = (sint16)(direction * 40);
    value = r_u32(0x80058C68) + step;
    w_u32(0x80058C68, value);
    if ((sint32)value >= 256) {
        w_u32(0x80058C64, 0xFFFFFFFF);
        w_u32(0x80058C68, 255);
    }
    if (r_s32(0x80058C68) < 0) {
        w_u32(0x80058C64, 1);
        w_u32(0x80058C68, 0);
    }
    return 1;
}

void gtmenu_secondary_input_enable(uint32 enabled)
{
    FUNCTION_MARKER(0x80024430, "GTMENU.EXE");
    w_u8(0x80058C78, enabled != 0);
}

uint32 gtmenu_controller_is_mouse(uint32 port)
{
    uint32 offset;
    FUNCTION_MARKER(0x80024450, "GTMENU.EXE");
    offset = (port >> 2) << 6;
    return (r_u8(0x800A8741 + offset) & 0xF0) == 0x10;
}

uint32 gtmenu_input_delay_reset(void)
{
    FUNCTION_MARKER(0x80024478, "GTMENU.EXE");
    w_u32(0x80058C7C, 4);
    return 4;
}

uint32 gtmenu_input_update(void)
{
    uint32 ordering, pointer, delay, enabled = 1, callback, result, x, y;
    sint32 timer;
    FUNCTION_MARKER(0x80024488, "GTMENU.EXE");
    ordering = gtmenu_current_ordering();
    delay = r_u32(0x80058C6C);
    pointer = r_u8(0x800A7B8A);
    if (delay) {
        w_u32(0x80058C6C, 0);
        gtmenu_controller_repeat_initialize(0x8009AACC);
    }
    if (r_u32(0x80058C70) != 4) {
        w_u32(0x80058C70, 4);
        gtmenu_controller_repeat_initialize(0x8009AB1C);
    }
    if (!gtmenu_controller_is_mouse(0))
        gtmenu_controller_state_update(0x8009AACC, gtmenu_controller_packet(0));
    if (!gtmenu_controller_is_mouse(4))
        gtmenu_controller_state_update(0x8009AB1C, gtmenu_controller_packet(4));
    delay = r_u32(0x80058C7C);
    if (delay) {
        w_u32(0x80058C7C, delay - 1);
    } else {
        if (gtmenu_controller_event_dispatch(0x8009AACC, 0))
            w_u32(0x80058C74, 0);
        if (r_s8(0x80058C78) && gtmenu_controller_event_dispatch(0x8009AB1C, 1))
            w_u32(0x80058C74, 0);
        if (gtmenu_pointer_hover_update())
            w_u32(0x80058C74, 0);
    }
    x = r_s16(0x800A7560);
    y = r_s16(0x800A7568);
    gtmenu_pointer_frame_draw(ordering, x, y, pointer, enabled);
    x = r_u16(0x800A7560);
    y = r_u16(0x800A7568);
    callback = r_u32(0x800A77B4);
    w_u32(0x8009AAB4, 3);
    w_u32(0x8009AAB8, ordering);
    w_u16(0x8009AAC0, x);
    w_u16(0x8009AAC2, y);
    gtmenu_event_callback_invoke(callback, 0x8009AAB4);
    x = r_u32(0x800A7560);
    y = r_u32(0x800A7568);
    result = gtmenu_pointer_history_store(x, y, 1);
    if (pointer) {
        timer = r_s16(0x800A77B0);
        if (timer > 0) {
            result = timer - 1;
            w_u16(0x800A77B0, result);
            w_u32(0x80058C80, 1);
        } else {
            result = r_u32(0x80058C80);
            if (result) {
                result = gtmenu_pointer_visibility_set(1, 0);
                w_u32(0x80058C80, 0);
            }
        }
    }
    return result;
}

uint32 gtmenu_controller_packet(uint32 port)
{
    uint32 packet;
    FUNCTION_MARKER(0x800412DC, "GTMENU.EXE");
    packet = 0x800A8740 + ((port >> 2) << 6);
    if ((r_u8(packet + 1) & 0xF0) == 0x80)
        packet += (port & 3) * 8 + 2;
    return packet;
}

uint32 gtmenu_pointer_hover_update(void)
{
    sint16 x, y;
    uint32 widget;
    FUNCTION_MARKER(0x80028CE0, "GTMENU.EXE");
    x = r_s16(0x800A7574);
    y = r_s16(0x800A755C);
    gtmenu_navigation_select_point(x, y);
    x = r_s16(0x800A7574);
    y = r_s16(0x800A755C);
    widget = gtmenu_pointer_widget_at(x, y);
    w_u32(0x800A7A5C, widget);
    return 0;
}

uint32 gtmenu_pointer_visibility_set(uint32 enabled, uint32 preserve)
{
    FUNCTION_MARKER(0x80026500, "GTMENU.EXE");
    w_u8(0x800A7B89, enabled != 0);
    w_u8(0x800A7B88, preserve != 0);
    return 0x800A7B89;
}

uint32 gtmenu_tim_upload(uint32 image, uint32 clut_position, uint32 clut_size,
    uint32 pixel_position, uint32 pixel_size, uint32 pixels_first)
{
    uint32 clut, pixels;
    FUNCTION_MARKER(0x80032244, "GTMENU.EXE");
    if (!(r_u32(image + 4) & 8))
        return gtmenu_tim_block_upload(image + 8, pixel_position, 0xFFFFFFFF);
    clut = image + 8;
    pixels = clut + r_u32(image + 8);
    if (pixels_first) {
        gtmenu_tim_block_upload(pixels, pixel_position, pixel_size);
        return gtmenu_tim_block_upload(clut, clut_position, clut_size);
    }
    gtmenu_tim_block_upload(clut, clut_position, clut_size);
    return gtmenu_tim_block_upload(pixels, pixel_position, pixel_size);
}

uint32 gtmenu_tim_block_upload(uint32 block, uint32 position, uint32 size)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 high, low;
    FUNCTION_MARKER(0x80034C90, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80034C90);
    if (position == 0xFFFFFFFF) {
        high = r_u16(block + 6);
        low = r_u16(block + 4);
        position = low | (high << 16);
    }
    if (size == 0xFFFFFFFF) {
        high = r_u16(block + 10);
        low = r_u16(block + 8);
        size = low | (high << 16);
    }
    w_u16(frame.address + 2, position >> 16);
    w_u16(frame.address, position);
    w_u16(frame.address + 4, size);
    w_u16(frame.address + 6, size >> 16);
    gtmenu_gpu_wait(0);
    gtmenu_gpu_load_image(frame.address, block + 12);
    gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return position;
}

uint32 gtmenu_race_request(uint32 kind, uint32 option, uint32 first_record, uint32 second_record, uint32 selection)
{
    uint32 result, first_count, second_count;
    FUNCTION_MARKER(0x800246CC, "GTMENU.EXE");
    gtmenu_debug_message(0x80054E64);
    w_u8(0x800A2415, kind);
    w_u8(0x800A2414, 2);
    switch (kind) {
    case 0xFFFFFFFF:
        result = gtmenu_debug_message(0x80054ED4);
        w_u8(0x800A2416, option);
        return result;
    case 0:
        return gtmenu_debug_message(0x80054E70);
    case 1:
        result = gtmenu_debug_integer(0x80054EB0, selection);
        w_u8(0x800A2416, selection);
        return result;
    case 2:
        gtmenu_debug_message(0x80054EA4);
        w_u8(0x800A2416, 0);
        w_u8(0x800A2418, selection);
        gtmenu_memory_copy(0x800A241A, first_record, 96);
        gtmenu_memory_copy(0x800A247A, second_record, 96);
        first_count = r_u16(0x800A241A);
        second_count = r_u16(0x800A247A);
        w_u16(0x800A241A, first_count + 1);
        w_u16(0x800A247A, second_count + 1);
        return gtmenu_days_add(1);
    case 3:
        result = gtmenu_debug_message(0x80054E84);
        w_u8(0x800A2418, option);
        return result;
    case 4:
        result = gtmenu_debug_message(0x80054E78);
        w_u8(0x800A2418, option);
        w_u8(0x800A2419, selection);
        return result;
    case 5:
        result = gtmenu_debug_message(0x80054E94);
        w_u8(0x800A2416, selection);
        return result;
    case 6:
        result = gtmenu_debug_integer(0x80054EC0, selection);
        w_u8(0x800A2416, selection);
        return result;
    default:
        return 0;
    }
}

uint32 gtmenu_page_command_set(uint32 command)
{
    FUNCTION_MARKER(0x80024A80, "GTMENU.EXE");
    gtmenu_string_copy(0x8009AB6C, command + 3);
    w_u8(0x80058C8C, 1);
    return 1;
}

void gtmenu_page_command_clear(void)
{
    FUNCTION_MARKER(0x80024ADC, "GTMENU.EXE");
    w_u8(0x80058C8C, 0);
}

uint32 gtmenu_page_style_reset(void)
{
    uint32 x, y, right, bottom;
    FUNCTION_MARKER(0x80024C70, "GTMENU.EXE");
    w_u32(0x800A7A04, 0x00808080);
    w_u32(0x800A79F0, 128);
    w_u32(0x800A79FC, 0);
    x = r_u16(0x800A7598);
    y = r_u16(0x800A759A);
    right = r_u16(0x800A759C);
    bottom = r_u16(0x800A759E);
    w_u32(0x800A79F4, 0x00800000);
    w_u32(0x800A79F8, 0x00800080);
    w_u32(0x800A7A00, 0);
    w_u32(0x800A79DC, 0);
    w_u32(0x800A7A7C, 0);
    w_u32(0x800A7A44, 0);
    w_u16(0x800A7A24, x);
    w_u16(0x800A7A26, y);
    w_u16(0x800A7A28, right);
    w_u16(0x800A7A2A, bottom);
    return 0x00800000;
}

uint32 gtmenu_layout_nodes_process(uint32 ordering, uint32 list)
{
    uint32 node, widget, kind, result;
    FUNCTION_MARKER(0x80024AE8, "GTMENU.EXE");
    w_u32(0x800A7A3C, 0);
    node = r_u32(list);
    while (node) {
        widget = r_u32(node);
        kind = r_u32(widget);
        switch (kind) {
        case 0x8000:
            gtmenu_layout_text(ordering, node);
            w_u32(0x800A7A48, r_u32(node));
            break;
        case 0x8001:
            w_u32(0x800A7A58, node);
            gtmenu_layout_image(ordering, r_u32(node));
            w_u32(0x800A7A40, r_u32(node));
            break;
        case 0x8003:
            gtmenu_layout_link(node);
            break;
        case 0x8004:
            gtmenu_layout_table(ordering, node);
            w_u32(0x800A7A4C, r_u32(node));
            break;
        case 0x8005:
            gtmenu_layout_rule(ordering, r_u32(node));
            break;
        default:
            gtmenu_layout_tag(r_u32(node));
            w_u32(0x800A7A50, r_u32(node));
            break;
        }
        w_u32(0x800A7A3C, r_u32(node));
        node = r_u32(node + 4);
    }
    result = r_u32(0x800A7A18);
    if (result) return result;
    gtmenu_hitboxes_build();
    return gtmenu_cursor_nearest(320, 240);
}

uint32 gtmenu_layout_link(uint32 node)
{
    uint32 widget, flags;
    FUNCTION_MARKER(0x800297D0, "GTMENU.EXE");
    widget = r_u32(node);
    flags = r_u32(widget + 4);
    if ((sint32)flags >= 0) {
        w_u32(widget + 4, flags | 0x80000000);
        flags = r_u32(0x800A79DC);
        w_u32(0x800A7A38, node);
        flags |= 0x100;
        w_u32(0x800A79DC, flags);
    }
    return flags;
}

uint32 gtmenu_background_tile_draw(uint32 ordering, uint32 tile, uint32 position, uint32 size)
{
    uint32 packet, mode_packet, link, vertical, horizontal, mode, palette, shift, result;
    FUNCTION_MARKER(0x80024CE0, "GTMENU.EXE");
    vertical = (tile & 0x10) << 4;
    horizontal = (tile & 15) << 6;
    packet = r_u32(0x800A8434);
    w_u32(0x800A8434, packet + 20);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, packet, 7);
    w_masked_u32(packet, link, 7);
    w_u16(packet + 8, position);
    w_u16(packet + 10, position >> 16);
    w_u8(packet + 3, 4);
    w_u32(packet + 4, 0x647F7F7F);
    mode = r_u32(0x800A7B54);
    w_u8(packet + 13, vertical);
    shift = (2 - (mode & 7)) & 31;
    w_u8(packet + 12, (horizontal & 63) << shift);
    palette = r_u16(0x800A7B50);
    mode_packet = r_u32(0x800A8434);
    w_u16(packet + 16, size);
    w_u16(packet + 18, size >> 16);
    w_u16(packet + 14, palette);
    w_u32(0x800A8434, mode_packet + 8);
    link = r_masked_u32(ordering, 7);
    w_masked_u32(ordering, mode_packet, 7);
    w_masked_u32(mode_packet, link, 7);
    w_u8(mode_packet + 3, 1);
    mode = r_u32(0x800A7B50);
    mode = (mode & 0xFFF0FFFF) | (horizontal << 10);
    mode = (mode & 0xFFEFFFFF) | ((vertical != 0) << 20);
    w_u32(0x800A7B50, mode);
    result = r_u16(0x800A7B52) | 0xE1000000;
    w_u32(mode_packet + 4, result);
    return result;
}

uint32 gtmenu_background_texture_initialize(uint32 image, uint32 clut, uint32 pixels)
{
    uint32 flags, block, mode, width, height, shift, y;
    FUNCTION_MARKER(0x80024DE8, "GTMENU.EXE");
    gtmenu_memory_fill(0x800A7B50, 0, 20);
    flags = r_u32(image + 4);
    w_u32(0x800A7B54, flags);
    if (!(r_u32(image + 4) & 8)) {
        gtmenu_tim_block_upload(image + 8, pixels, 0xFFFFFFFF);
        w_u16(0x800A7B5A, pixels >> 16);
        w_u16(0x800A7B58, pixels);
        flags = r_u32(image + 4);
        width = r_u16(image + 16);
        shift = (2 - (flags & 7)) & 31;
        w_u16(0x800A7B5C, width << shift);
        height = r_u16(image + 18);
        w_u16(0x800A7B5E, height);
        return height;
    }
    block = image + 8 + r_u32(image + 8);
    gtmenu_tim_block_upload(image + 8, clut, 0xFFFFFFFF);
    gtmenu_tim_block_upload(block, pixels, 0xFFFFFFFF);
    y = pixels >> 16;
    w_u16(0x800A7B5A, y);
    mode = r_u32(0x800A7B50);
    w_u16(0x800A7B58, pixels);
    mode = (mode & 0xFFFFFFC0) | ((clut >> 4) & 63);
    mode = (mode & 0xFFFF803F) | ((clut >> 10) & 0x7FC0);
    mode = (mode & 0xFFF0FFFF) | (((pixels >> 6) & 15) << 16);
    mode = (mode & 0xFFEFFFFF) | (((y >> 8) & 1) << 20);
    mode &= 0xFF9FFFFF;
    flags = r_u32(image + 4);
    width = r_u16(block + 8);
    shift = (2 - (flags & 7)) & 31;
    w_u16(0x800A7B5C, width << shift);
    height = r_u16(block + 10);
    mode |= 0x200000;
    w_u32(0x800A7B50, mode);
    w_u16(0x800A7B5E, height);
    flags = (r_u32(image + 4) & 3) << 23;
    mode = (mode & 0xFE7FFFFF) | flags;
    w_u32(0x800A7B50, mode);
    return flags;
}

uint32 gtmenu_background_mode_set(uint8 mode)
{
    FUNCTION_MARKER(0x80024FA0, "GTMENU.EXE");
    w_u8(0x800A7B61, 0);
    w_u8(0x800A7B60, mode);
    return 0x800A7B61;
}

uint32 gtmenu_archive_image_upload(uint32 archive, sint32 index, uint16 x, uint16 y, uint16 width, uint16 height)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80024FB4, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(32776, 0x80024FB4);
    gtmenu_archive_entry_unpack(archive, index, frame.address + 8);
    w_u16(frame.address, x);
    w_u16(frame.address + 2, y);
    w_u16(frame.address + 4, width);
    w_u16(frame.address + 6, height);
    gtmenu_gpu_load_image(frame.address, frame.address + 8);
    result = gtmenu_gpu_wait(0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_background_archive_load(uint32 name, uint32 clut, uint32 pixels)
{
    uint32 archive, index, x, y, clut_x, clut_y, packed;
    sint32 dx, dy, width, height;
    FUNCTION_MARKER(0x80025054, "GTMENU.EXE");
    archive = gtmenu_menu_asset_load(name);
    x = pixels & 0xFFFF;
    y = pixels >> 16;
    clut_x = clut & 0xFFFF;
    clut_y = clut >> 16;
    gtmenu_archive_image_upload(archive, 0, clut_x, clut_y, 16, 1);
    for (index = 0; index < 6; ++index) {
        dx = r_s16(0x80070868 + index * 4);
        dy = r_s16(0x8007086A + index * 4);
        width = r_s16(0x80070880 + index * 4);
        height = r_s16(0x80070882 + index * 4);
        gtmenu_archive_image_upload(archive, index + 1, x + dx, y + dy, width, height);
    }
    gtmenu_heap_free(archive);
    gtmenu_memory_fill(0x800A7B50, 0, 20);
    w_u16(0x800A7B5C, 192);
    w_u16(0x800A7B5E, 480);
    packed = r_u32(0x800A7B54);
    w_u16(0x800A7B58, x);
    w_u16(0x800A7B5A, y);
    w_u32(0x800A7B54, (packed & 0xFFFFFFF8) | 8);
    packed = r_u32(0x800A7B50);
    w_u8(0x800A7B61, 1);
    w_u16(0x800A7B62, 2);
    packed = (packed & 0xFFFFFFC0) | ((clut_x >> 4) & 63);
    packed = (packed & 0xFFFF803F) | ((clut_y & 0x1FF) << 6);
    packed = (packed & 0xFFF0FFFF) | (((x >> 6) & 15) << 16);
    packed = (packed & 0xFFEFFFFF) | (((y >> 8) & 1) << 20);
    packed = (packed & 0xFF9FFFFF) | 0x200000;
    w_u32(0x800A7B50, packed & 0xFE7FFFFF);
    return 0;
}

uint32 gtmenu_background_texture_load(uint32 name, uint32 clut, uint32 pixels)
{
    uint32 image;
    FUNCTION_MARKER(0x8002524C, "GTMENU.EXE");
    image = gtmenu_menu_asset_load(name);
    gtmenu_background_texture_initialize(image, clut, pixels);
    gtmenu_heap_free(image);
    w_u8(0x800A7B61, 0);
    gtmenu_background_draw(0);
    return 0;
}

uint32 gtmenu_background_ordering(void)
{
    FUNCTION_MARKER(0x8001D95C, "GTMENU.EXE");
    return r_u32(0x800823F4);
}

uint32 gtmenu_background_draw(uint32 vertical_offset)
{
    uint32 ordering, position, countdown, result = 1;
    FUNCTION_MARKER(0x800252D0, "GTMENU.EXE");
    ordering = gtmenu_background_ordering();
    position = vertical_offset << 16;
    gtmenu_background_tile_draw(ordering, 10, position, 0x01000100);
    gtmenu_background_tile_draw(ordering, 11, position | 256, 0x01000100);
    gtmenu_background_tile_draw(ordering, 12, position | 512, 0x01000080);
    position = (vertical_offset + 256) << 16;
    gtmenu_background_tile_draw(ordering, 26, position, 0x00E00100);
    gtmenu_background_tile_draw(ordering, 27, position | 256, 0x00E00100);
    gtmenu_background_tile_draw(ordering, 28, position | 512, 0x00E00080);
    if (r_s8(0x800A7B60) == 1) {
        countdown = r_u16(0x800A7B62) - 1;
        w_u16(0x800A7B62, countdown);
        result = countdown << 16;
        if (!result) w_u8(0x800A7B61, 0);
    }
    return result;
}

uint32 gtmenu_navigation_owners_release(void)
{
    uint32 owner, next;
    FUNCTION_MARKER(0x80025410, "GTMENU.EXE");
    owner = r_u32(0x800A7A10);
    while (owner) {
        next = r_u32(owner);
        gtmenu_navigation_owner_release(owner);
        gtmenu_heap_free(owner);
        owner = next;
    }
    w_u32(0x800A7A14, 0);
    w_u32(0x800A7A10, 0);
    return 0x800A7A10;
}

uint32 gtmenu_list_create(void)
{
    /* Original occurrences
     * GTMENU.EXE:0x80025470
     * GTMENU.EXE:0x8002AD44
     */
    uint32 list;
    list = gtmenu_heap_alloc(8);
    gtmenu_memory_fill(list, 0, 8);
    return list;
}

uint32 gtmenu_page_action_parse(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 mode, cursor, code, x, y, right, bottom;
    FUNCTION_MARKER(0x80025574, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(88, 0x80025574);
    mode = r_u8(input + 1);
    cursor = gtmenu_stream_u16_read(input + 2, frame.address + 80);
    if (mode == 1) {
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 64);
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 66);
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 68);
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 70);
    } else if (mode == 2) {
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 68);
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 70);
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 64);
    }
    cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 82);
    if (r_s8(frame.address) == 'B' && r_s8(frame.address + 1) == 'P') {
        gtmenu_page_command_set(frame.address);
    } else {
        code = gtmenu_page_action_code(frame.address);
        if (code == 56) {
            gtmenu_layout_mode_record(frame.address, mode, frame.address + 64);
        } else {
            x = r_u16(frame.address + 64);
            y = r_u16(frame.address + 66);
            right = r_u16(frame.address + 68);
            bottom = r_u16(frame.address + 70);
            w_u16(frame.address + 72, x);
            w_u16(frame.address + 74, y);
            w_u16(frame.address + 76, right - x);
            w_u16(frame.address + 78, bottom - y);
            gtmenu_navigation_append(code, frame.address + 72);
            if ((code - 1 < 10) || code == 14)
                gtmenu_layout_mode_record(0x80054F24, mode, frame.address + 64);
            else if (code == 11)
                gtmenu_layout_mode_record(0x80054F30, mode, frame.address + 64);
        }
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_number_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x8002573C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x8002573C);
    result = gtmenu_stream_u16_read(input + 1, frame.address);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_stream_u16_read(uint32 input, uint32 output)
{
    uint32 low, high;
    FUNCTION_MARKER(0x8002A4DC, "GTMENU.EXE");
    low = r_u8(input);
    high = r_u8(input + 1);
    w_u16(output, low | (high << 8));
    return input + 2;
}

uint32 gtmenu_stream_size_read(uint32 input, uint32 output)
{
    uint32 first, second;
    FUNCTION_MARKER(0x8002AD10, "GTMENU.EXE");
    first = r_u8(input);
    ++input;
    w_u16(output, first);
    if (first & 0x80) {
        second = r_u8(input);
        ++input;
        w_u16(output, second | ((first << 8) & 0x7F00));
    }
    return input;
}

uint32 gtmenu_stream_string_read(uint32 input, uint32 output, uint32 length)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 data, size, result;
    FUNCTION_MARKER(0x800262C4, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x800262C4);
    data = gtmenu_stream_size_read(input, frame.address);
    size = r_u16(frame.address);
    gtmenu_string_copy_bounded(output, data, size);
    size = r_u16(frame.address);
    w_u8(output + size, 0);
    size = r_u16(frame.address);
    w_u16(length, size);
    size = r_u16(frame.address);
    result = data + size;
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_page_body_parse(uint32 list, uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 cursor, flags, image = 0;
    FUNCTION_MARKER(0x80025760, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80025760);
    cursor = input + 1;
    if (!(r_s8(input) & 0x80)) {
        flags = r_u8(cursor);
        ++cursor;
        if (flags & 1) cursor = gtmenu_stream_u24_read(cursor, 0x800A79F4);
        if (flags & 2) {
            cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 64);
            gtmenu_body_background_image_create(list, frame.address);
            image = 1;
        }
        if (flags & 4) cursor = gtmenu_stream_u24_read(cursor, 0x800A7A04);
        if (flags & 8) cursor = gtmenu_stream_u24_read(cursor, 0x800A79F0);
        if (flags & 16) {
            cursor = gtmenu_stream_u24_read(cursor, 0x800A79FC);
            w_u32(0x800A7A00, r_u32(0x800A79FC));
        }
        if (flags & 32) cursor = gtmenu_stream_u24_read(cursor, 0x800A79F8);
        if (!image) gtmenu_body_background_color_create(r_u32(0x800A7A04));
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_break_parse(uint32 list, uint32 input)
{
    FUNCTION_MARKER(0x800258B4, "GTMENU.EXE");
    if (!r_u8(input + 1)) gtmenu_page_tag_append(list, 14);
    return input + 2;
}

uint32 gtmenu_page_preformatted_parse(uint32 input)
{
    uint32 cursor, node, widget, flags, extra = 0;
    FUNCTION_MARKER(0x800258F8, "GTMENU.EXE");
    cursor = input + 1;
    if (r_s8(input) & 0x80) {
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags & 0xFFFFFBFF);
    } else {
        if (r_s8(cursor) == 6) extra = 32;
        ++cursor;
        node = r_u32(0x800A7A6C);
        widget = r_u32(node);
        flags = r_u32(widget + 4);
        w_u32(widget + 4, flags | extra);
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags | 0x400);
    }
    return cursor;
}

uint32 gtmenu_page_optional_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 cursor, flags;
    FUNCTION_MARKER(0x80025984, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80025984);
    w_u16(frame.address + 64, 0);
    cursor = input + 1;
    if (!(r_s8(input) & 0x80)) {
        flags = r_u8(cursor);
        ++cursor;
        if (flags & 1) {
            r_u8(cursor);
            ++cursor;
        }
        if (flags & 2)
            cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 64);
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_flag_skip(uint32 input)
{
    uint32 cursor;
    FUNCTION_MARKER(0x800259EC, "GTMENU.EXE");
    cursor = input + 1;
    if (!(r_s8(input) & 0x80)) ++cursor;
    return cursor;
}

uint32 gtmenu_page_attribute_number_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80025A0C, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80025A0C);
    result = gtmenu_stream_u16_read(input + 1, frame.address);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_page_object_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 cursor, flags;
    FUNCTION_MARKER(0x80025A30, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80025A30);
    cursor = input + 1;
    if (!(r_s8(input) & 0x80)) {
        flags = r_u8(cursor);
        ++cursor;
        if (flags & 1)
            cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 64);
        if (flags & 2) {
            r_s8(cursor);
            ++cursor;
        }
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_table_counts_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 cursor;
    sint32 first, second, index;
    FUNCTION_MARKER(0x80025AA0, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80025AA0);
    first = r_s8(input + 1);
    cursor = input + 2;
    second = r_s8(cursor);
    for (index = 0; index < first; ++index)
        cursor = gtmenu_stream_u16_read(cursor, frame.address);
    for (index = 0; index < second; ++index)
        cursor = gtmenu_stream_u16_read(cursor, frame.address);
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_rule_parse(uint32 list, uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 left, right, default_height, flags, style = 0, cursor;
    sint16 height;
    sint32 extent, value;
    FUNCTION_MARKER(0x80025B28, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80025B28);
    left = r_u16(0x800A7A24);
    right = r_u16(0x800A7A26);
    default_height = r_u16(0x800A75BC);
    w_u16(frame.address + 2, right - left);
    w_u16(frame.address, default_height);
    flags = r_u8(input + 1);
    cursor = input + 2;
    if (flags & 1) style = 0x01000000;
    if (flags & 8) cursor = gtmenu_stream_u16_read(cursor, frame.address);
    if (flags & 2) {
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 2);
    } else if (flags & 4) {
        cursor = gtmenu_stream_u16_read(cursor, frame.address + 2);
        extent = (sint16)right - (sint16)left;
        value = r_s16(frame.address + 2);
        if (value < extent) value = (extent / 100) * value;
        else {
            value = (sint16)r_u16(frame.address + 2);
            value /= 100;
            value *= extent;
        }
        w_u16(frame.address + 2, value);
    }
    height = r_s16(frame.address);
    value = r_s16(frame.address + 2);
    gtmenu_page_rule_create(list, style, height, value);
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_stream_u24_read(uint32 input, uint32 output)
{
    uint32 value, next;
    FUNCTION_MARKER(0x8002A584, "GTMENU.EXE");
    value = r_u8(input);
    w_u32(output, value);
    next = r_u8(input + 1);
    value |= next << 8;
    w_u32(output, value);
    next = r_u8(input + 2);
    w_u32(output, value | (next << 16));
    return input + 3;
}

uint32 gtmenu_widget_alloc(uint32 bytes)
{
    FUNCTION_MARKER(0x8002AD80, "GTMENU.EXE");
    return gtmenu_heap_alloc(bytes);
}

uint32 gtmenu_body_background_color_create(uint32 color)
{
    uint32 node, widget;
    FUNCTION_MARKER(0x8002A6F8, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(80);
    w_u32(node, widget);
    w_u32(widget, 0x8001);
    widget = r_u32(node);
    w_u32(widget + 4, 0x03000000);
    widget = r_u32(node);
    w_u32(widget + 76, color);
    widget = r_u32(node);
    w_u8(widget + 12, 0);
    w_u32(node + 4, 0);
    w_u32(0x800A7A38, node);
    return widget;
}

uint32 gtmenu_page_rule_create(uint32 list, uint32 flags, sint16 height, sint16 width)
{
    uint32 node, widget;
    FUNCTION_MARKER(0x8002A76C, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(12);
    w_u32(node, widget);
    w_u32(widget, 0x8005);
    widget = r_u32(node);
    w_u32(widget + 4, flags);
    widget = r_u32(node);
    w_u16(widget + 8, width);
    widget = r_u32(node);
    w_u16(widget + 10, height);
    w_u32(node + 4, 0);
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_page_tag_append(uint32 list, uint8 tag)
{
    uint32 node, widget;
    FUNCTION_MARKER(0x8002643C, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(8);
    w_u32(node, widget);
    w_u32(widget, 0x8008 + tag);
    widget = r_u32(node);
    w_u32(widget + 4, 0);
    w_u32(node + 4, 0);
    if (r_u32(0x800A79DC) & 0x200) list = r_u32(0x800A7A70) + 24;
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_page_list_append(uint32 list, uint32 node)
{
    uint32 tail;
    FUNCTION_MARKER(0x800264D4, "GTMENU.EXE");
    tail = r_u32(list + 4);
    if (tail) {
        w_u32(tail + 4, node);
        w_u32(list + 4, node);
    } else {
        w_u32(list + 4, node);
        w_u32(list, node);
    }
    return tail;
}

uint32 gtmenu_body_background_image_create(uint32 list, uint32 name)
{
    uint32 node, widget, flags, field, clut, pixels, result;
    FUNCTION_MARKER(0x8002A5C0, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(80);
    w_u32(node, widget);
    w_u32(widget, 0x8001);
    widget = r_u32(node);
    gtmenu_string_copy(widget + 12, name);
    flags = gtmenu_image_format_flags(name);
    widget = r_u32(node);
    w_u32(widget + 4, flags | 0x01000000);
    widget = r_u32(node);
    field = r_u16(0x800A7590);
    w_u16(widget + 8, field);
    widget = r_u32(node);
    field = r_u16(0x800A7592);
    w_u16(widget + 10, field);
    widget = r_u32(node);
    w_u32(node + 4, 0);
    flags = r_u32(widget + 4);
    if (flags & 0x100000) {
        gtmenu_page_list_append(list, node);
        w_u32(0x800A7A38, 0);
        widget = r_u32(node);
        w_u16(widget + 10, 480);
        return 480;
    }
    widget = r_u32(node);
    flags = r_u32(widget + 4) & 0x10000;
    if (flags) {
        gtmenu_heap_free(widget);
        gtmenu_heap_free(node);
        clut = r_u32(0x800A75EC);
        pixels = r_u32(0x800A75E8);
        result = gtmenu_background_archive_load(name, clut, pixels);
        w_u32(0x800A7A38, 0);
        return result;
    }
    w_u32(0x800A7A38, node);
    return flags;
}

uint32 gtmenu_image_format_flags(uint32 name)
{
    FUNCTION_MARKER(0x8002C6B4, "GTMENU.EXE");
    if (gtmenu_string_find(name, 0x800555A4)) return 0x100000;
    if (gtmenu_string_find(name, 0x800555AC)) return 0x200000;
    if (gtmenu_string_find(name, 0x800555B4)) return 0x400000;
    if (gtmenu_string_find(name, 0x800555BC)) return 0x800000;
    if (gtmenu_string_find(name, 0x800555C4) && gtmenu_string_find(name, 0x800555CC))
        return 0x40000;
    if (gtmenu_string_find(name, 0x800555C4)) return 0x10000;
    return 0;
}

uint32 gtmenu_page_image_parse(uint32 list, uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 low, high, flags, cursor, type, style = 32, bit, offset;
    FUNCTION_MARKER(0x80025CAC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(272, 0x80025CAC);
    low = r_u8(input + 1);
    high = r_u8(input + 2);
    flags = (uint32)(sint32)(sint8)low | ((uint32)(sint32)(sint8)high << 8);
    cursor = gtmenu_stream_string_read(input + 3, frame.address, frame.address + 256);
    if (flags & 1)
        cursor = gtmenu_stream_string_read(cursor, frame.address + 192, frame.address + 256);
    if (flags & 2) {
        type = r_u8(cursor);
        ++cursor;
        if (type - 1 < 2) style = 8;
        else if (type == 3) style = 128;
        else if (type == 5) style = 64;
        else if (type == 4) style = 16;
    }
    if (flags & 4)
        cursor = gtmenu_stream_string_read(cursor, frame.address + 64, frame.address + 256);
    for (bit = 8, offset = 258; bit <= 0x200; bit <<= 1, offset += 2)
        if (flags & bit) cursor = gtmenu_stream_u16_read(cursor, frame.address + offset);
    if (flags & 0x400)
        cursor = gtmenu_stream_string_read(cursor, frame.address + 128, frame.address + 256);
    gtmenu_page_image_create(list, style, frame.address, frame.address + 128);
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_metadata_skip(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 flags, cursor;
    FUNCTION_MARKER(0x80025EAC, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80025EAC);
    flags = r_u8(input + 1);
    cursor = input + 2;
    if (flags & 1) cursor = gtmenu_stream_u16_read(cursor, frame.address + 64);
    if (flags & 2)
        cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 66);
    if (flags & 4) cursor = gtmenu_stream_u16_read(cursor, frame.address + 64);
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_pair_skip(uint32 input)
{
    FUNCTION_MARKER(0x80025F20, "GTMENU.EXE");
    return input + 2;
}

uint32 gtmenu_page_block_mode_parse(uint32 list, uint32 input)
{
    sint32 value;
    FUNCTION_MARKER(0x80025F28, "GTMENU.EXE");
    value = r_s8(input + 1);
    gtmenu_page_block_mode_create(list, value);
    return input + 2;
}

uint32 gtmenu_page_table_parse(uint32 list, uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 border, padding, spacing, node, widget, rows, flags, cursor;
    sint32 tag;
    sint16 first, second, third, fourth;
    FUNCTION_MARKER(0x80025F58, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80025F58);
    border = r_u16(0x800A75BE);
    padding = r_u16(0x800A75C0);
    spacing = r_u16(0x800A75C2);
    w_u16(frame.address + 6, 0);
    w_u16(frame.address, border);
    w_u16(frame.address + 2, padding);
    w_u16(frame.address + 4, spacing);
    tag = r_s8(input);
    node = r_u32(0x800A7A6C);
    cursor = input + 1;
    if (tag & 0x80) {
        widget = r_u32(node);
        rows = r_u16(0x800A7A7E);
        w_u16(widget + 28, rows);
        gtmenu_page_list_append(list, node);
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags & 0xFFFFFDFF);
    } else {
        flags = r_u8(cursor);
        ++cursor;
        if (flags & 1) {
            cursor = gtmenu_stream_u16_read(cursor, frame.address);
            if (r_u16(frame.address))
                w_u32(0x800A79DC, r_u32(0x800A79DC) | 0x800);
        }
        if (flags & 2) cursor = gtmenu_stream_u16_read(cursor, frame.address + 2);
        if (flags & 4) cursor = gtmenu_stream_u16_read(cursor, frame.address + 4);
        if (flags & 8) cursor = gtmenu_stream_u16_read(cursor, frame.address + 6);
        first = r_s16(frame.address);
        second = r_s16(frame.address + 2);
        third = r_s16(frame.address + 4);
        fourth = r_s16(frame.address + 6);
        gtmenu_table_create(first, second, third, fourth);
        flags = r_u32(0x800A79DC);
        w_u32(0x800A7A7C, 0);
        w_u32(0x800A79DC, flags | 0x200);
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_cell_parse(uint32 input)
{
    uint32 cell, cursor;
    FUNCTION_MARKER(0x800260B0, "GTMENU.EXE");
    cursor = input + 1;
    if (r_s8(input) & 0x80) {
        gtmenu_table_cell_append(r_u32(0x800A7A70));
        return cursor;
    }
    cell = gtmenu_table_cell_create();
    return gtmenu_table_cell_attributes_parse(cursor, cell);
}

uint32 gtmenu_page_header_cell_parse(uint32 input)
{
    uint32 cell, flags, cursor;
    FUNCTION_MARKER(0x80026118, "GTMENU.EXE");
    cursor = input + 1;
    if (r_s8(input) & 0x80) {
        gtmenu_table_cell_append(r_u32(0x800A7A70));
        return cursor;
    }
    cell = gtmenu_table_cell_create();
    flags = r_u32(cell + 4);
    w_u32(cell + 4, flags | 0x8000);
    return gtmenu_table_cell_attributes_parse(cursor, cell);
}

uint32 gtmenu_page_row_parse(uint32 input)
{
    uint32 flags, cursor, node, widget, columns, rows;
    sint32 maximum;
    FUNCTION_MARKER(0x8002618C, "GTMENU.EXE");
    flags = r_u8(input + 1);
    cursor = input + 2;
    if (flags & 1) { r_u8(cursor); ++cursor; }
    if (flags & 2) { r_u8(cursor); ++cursor; }
    node = r_u32(0x800A7A6C);
    widget = r_u32(node);
    columns = r_u16(0x800A7A7C);
    maximum = r_s16(widget + 26);
    if (maximum < (sint32)columns)
        w_u16(widget + 26, r_u16(0x800A7A7C));
    rows = r_u16(0x800A7A7E);
    w_u16(0x800A7A7C, 0);
    w_u16(0x800A7A7E, rows + 1);
    return cursor;
}

uint32 gtmenu_page_byte_skip(uint32 input)
{
    /* Original occurrences
     * GTMENU.EXE:0x80026220
     * GTMENU.EXE:0x80026230
     */
    r_u8(input + 1);
    return input + 2;
}

uint32 gtmenu_page_action_text_parse(uint32 input)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 flags, cursor;
    FUNCTION_MARKER(0x80026240, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80026240);
    cursor = input + 1;
    if (r_s8(input) & 0x80) {
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags & 0xFFFFEFFF);
    } else {
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags | 0x1000);
        cursor = gtmenu_stream_string_read(cursor, frame.address, frame.address + 64);
        gtmenu_named_action_create(frame.address);
    }
    gtmenu_local_stack_pop(frame);
    return cursor;
}

uint32 gtmenu_page_text_append(uint32 list, uint32 text, uint32 length)
{
    uint32 flags, node, widget, result;
    FUNCTION_MARKER(0x8002633C, "GTMENU.EXE");
    flags = r_u32(0x800A79DC);
    if ((flags & 0x600) == 0x600) {
        node = r_u32(0x800A7A6C);
        widget = r_u32(node);
        w_u32(widget + 12, text);
        result = gtmenu_string_length(text);
        node = r_u32(0x800A7A6C);
        widget = r_u32(node);
        w_u16(widget + 16, result);
        return result;
    }
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(20);
    w_u32(node, widget);
    w_u32(widget, 0x8000);
    widget = r_u32(node);
    w_u32(widget + 4, 0);
    widget = r_u32(node);
    w_u32(widget + 16, length);
    widget = r_u32(node);
    w_u32(widget + 12, text);
    w_u32(node + 4, 0);
    if (r_u32(0x800A79DC) & 0x200) list = r_u32(0x800A7A70) + 24;
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_page_image_create(uint32 list, uint32 flags, uint32 name, uint32 action)
{
    uint32 node, widget, format;
    FUNCTION_MARKER(0x8002A800, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(80);
    w_u32(node, widget);
    w_u32(widget, 0x8001);
    widget = r_u32(node);
    gtmenu_string_copy(widget + 12, name);
    widget = r_u32(node);
    gtmenu_string_copy(widget + 44, action);
    format = gtmenu_image_format_flags(name);
    widget = r_u32(node);
    w_u32(widget + 4, format | flags);
    widget = r_u32(node);
    w_u32(widget + 8, 0);
    w_u32(node + 4, 0);
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_page_block_mode_create(uint32 list, sint32 value)
{
    uint32 node, widget;
    FUNCTION_MARKER(0x8002A8B4, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(8);
    w_u32(node, widget);
    w_u32(widget, 0x803D);
    widget = r_u32(node);
    w_u32(widget + 4, value);
    w_u32(node + 4, 0);
    if (r_u32(0x800A79DC) & 0x200) list = r_u32(0x800A7A70) + 24;
    return gtmenu_page_list_append(list, node);
}

uint32 gtmenu_table_create(sint16 border, sint16 padding, sint16 spacing, sint16 attributes)
{
    uint32 node, widget, flags;
    FUNCTION_MARKER(0x8002A944, "GTMENU.EXE");
    node = gtmenu_list_create();
    widget = gtmenu_widget_alloc(120);
    w_u32(node, widget);
    gtmenu_memory_fill(widget, 0, 120);
    widget = r_u32(node);
    w_u32(widget, 0x8004);
    widget = r_u32(node);
    w_u32(widget + 4, 0);
    if (r_u32(0x800A79DC) & 0x800) {
        widget = r_u32(node);
        flags = r_u32(widget + 4);
        w_u32(widget + 4, flags | 0x800);
        flags = r_u32(0x800A79DC);
        w_u32(0x800A79DC, flags & 0xFFFFF7FF);
    }
    widget = r_u32(node);
    w_u16(widget + 18, border);
    widget = r_u32(node);
    w_u16(widget + 20, padding);
    widget = r_u32(node);
    w_u16(widget + 22, spacing);
    widget = r_u32(node);
    w_u16(widget + 24, attributes);
    w_u32(node + 4, 0);
    w_u32(0x800A7A6C, node);
    return widget;
}

uint32 gtmenu_table_cell_append(uint32 cell)
{
    uint32 node, widget, tail;
    FUNCTION_MARKER(0x8002AA48, "GTMENU.EXE");
    node = r_u32(0x800A7A6C);
    widget = r_u32(node);
    tail = r_u32(widget + 116);
    if (tail) {
        w_u32(tail + 32, cell);
        node = r_u32(0x800A7A6C);
        widget = r_u32(node);
        w_u32(widget + 116, cell);
    } else {
        node = r_u32(0x800A7A6C);
        widget = r_u32(node);
        w_u32(widget + 116, cell);
        w_u32(widget + 112, cell);
    }
    return widget;
}

uint32 gtmenu_table_cell_create(void)
{
    uint32 cell;
    FUNCTION_MARKER(0x8002AAAC, "GTMENU.EXE");
    cell = gtmenu_heap_alloc(36);
    gtmenu_memory_fill(cell, 0, 36);
    w_u32(cell + 28, 0);
    w_u32(cell + 24, 0);
    return cell;
}

uint32 gtmenu_table_cell_attributes_parse(uint32 input, uint32 cell)
{
    uint32 flags, cursor, mode, extra, value, columns, rows;
    FUNCTION_MARKER(0x8002AAF0, "GTMENU.EXE");
    flags = r_u8(input);
    cursor = input + 1;
    if (flags & 1) {
        mode = r_u8(cursor);
        ++cursor;
        extra = 0;
        if (!mode || mode == 3) extra = 2;
        else if (mode == 1) extra = 1;
        else if (mode == 5) extra = 4;
        if (extra) w_u32(cell + 4, r_u32(cell + 4) | extra);
    }
    if (flags & 2) cursor = gtmenu_stream_u16_read(cursor, cell + 12);
    if (flags & 4) w_u32(cell + 4, r_u32(cell + 4) | 0x100);
    if (flags & 8) cursor = gtmenu_stream_u16_read(cursor, cell + 14);
    if (flags & 16) {
        mode = r_u8(cursor);
        ++cursor;
        extra = 0;
        if (!mode || mode == 2) extra = 32;
        else if (mode == 4) extra = 8;
        else if (mode == 6) extra = 16;
        if (extra) w_u32(cell + 4, r_u32(cell + 4) | extra);
    }
    if (flags & 32) cursor = gtmenu_stream_u16_read(cursor, cell + 16);
    if (flags & 64) {
        cursor = gtmenu_stream_u24_read(cursor, cell + 20);
        value = r_u32(cell + 4);
        w_u32(cell + 4, value | 0x10000);
    }
    columns = r_u16(0x800A7A7C);
    rows = r_u16(0x800A7A7E);
    w_u32(0x800A7A70, cell);
    w_u16(0x800A7A7C, columns + 1);
    if (!rows) {
        rows = r_u16(0x800A7A7E);
        w_u16(0x800A7A7E, rows + 1);
    }
    return cursor;
}

uint32 gtmenu_named_action_create(uint32 name)
{
    uint32 entry;
    FUNCTION_MARKER(0x8002ACB8, "GTMENU.EXE");
    entry = gtmenu_heap_alloc(48);
    gtmenu_string_copy(entry + 8, name);
    w_u32(entry, 0);
    w_u32(entry + 40, 0);
    w_u32(entry + 44, 0);
    w_u32(entry + 4, 0);
    return gtmenu_named_action_insert(entry);
}

uint32 gtmenu_named_action_insert(uint32 entry)
{
    uint32 tail;
    FUNCTION_MARKER(0x8002C7B4, "GTMENU.EXE");
    tail = r_u32(0x800A7A14);
    if (tail) {
        tail = r_u32(0x800A7A14);
        w_u32(tail, entry);
        w_u32(0x800A7A14, entry);
        return tail;
    }
    w_u32(0x800A7A14, entry);
    w_u32(0x800A7A10, entry);
    return 0x800A7A10;
}

uint32 gtmenu_pointer_delay_reset(void)
{
    uint32 delay;
    FUNCTION_MARKER(0x8002651C, "GTMENU.EXE");
    delay = r_u16(0x800A7580);
    w_u16(0x800A77B0, delay);
    return delay;
}

uint32 gtmenu_garage_callback_reset(void)
{
    FUNCTION_MARKER(0x800265C4, "GTMENU.EXE");
    w_u32(0x80070898, 0);
    w_u32(0x800A77B4, 0x8001FDA4);
    return 0x8001FDA4;
}

sint32 gtmenu_record_pointer_index(uint32 record)
{
    sint32 count, index, result = -1;
    FUNCTION_MARKER(0x800265E4, "GTMENU.EXE");
    count = gtmenu_record_count();
    for (index = 0; index < count; ++index) {
        if (result > 0) return result;
        if (gtmenu_record_address(index) == record) result = index;
    }
    return result;
}

uint32 gtmenu_sell_price_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 record, base, bonus = 0, result;
    sint64 price;
    sint32 index, make;
    FUNCTION_MARKER(0x80026660, "GTMENU.EXE");
    if (gtmenu_current_record_index() < 0) {
        gtmenu_text_color_set(0x808080);
        gtmenu_text_font_set(1);
        return gtmenu_text_draw(ordering, 420, 346, 0x80054F3C);
    }
    frame = gtmenu_local_stack_push(256, 0x80026660);
    index = gtmenu_current_record_index();
    record = gtmenu_record_address(index);
    index = gtmenu_current_record_index();
    base = gtmenu_garage_price(index);
    make = r_s16(0x800708A0);
    if (gtmenu_record_matches_make(record, make)) bonus = base / 10;
    price = (sint64)(base >> 1) + bonus;
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(1);
    price = gtmenu_signed_divide64(price, 100, 0x80026754);
    gtmenu_format_signed64(frame.address, 0x80054F40, price);
    gtmenu_decimal_group(frame.address + 128, frame.address);
    result = gtmenu_text_draw(ordering, 420, 346, frame.address + 128);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_used_sell_activate(void)
{
    sint32 index;
    uint32 page;
    FUNCTION_MARKER(0x800267B4, "GTMENU.EXE");
    if (!gtmenu_string_compare(r_u32(0x800A7A5C), 0x80054F44)) {
        if (gtmenu_current_record_index() >= 0) {
            index = gtmenu_current_record_index();
            if (gtmenu_garage_price(index)) {
                gtmenu_sound_effect(5);
                page = r_u16(0x800A77C4);
                gtmenu_confirmation_open(2, 0x8009ABAC, page, 0);
                return 1;
            }
        }
        gtmenu_sound_effect(6);
        return 0;
    }
    if (!gtmenu_ui_default_activate()) return 0;
    gtmenu_garage_callback_reset();
    return 1;
}
