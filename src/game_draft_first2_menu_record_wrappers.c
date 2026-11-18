#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

/* GTMAIN.EXE 80030DE4 */
/* Original: GTMAIN.EXE:sub_80030DE4. */ uint32 gtmain_texture_workspace_size(uint32 image)
{
    uint32 count = r_u32(image);
    return ((count << 1u) + count) << 3u;
}

/* GTMAIN.EXE 8002ADDC */
/* Original: GTMAIN.EXE:sub_8002ADDC. */ uint32 gtmain_race_resources_load(void)
{
    uint32 course = r_u8(0x800B63BCu);
    uint32 camera = r_u8(gtmain_course_entry(course)) & 127u;
    gtmain_stream_mode_set(128u);
    gtmain_camera_resource_load(camera);
    gtmain_race_track_load(course);
    gtmain_race_cars_load();
    return (uint32)gtmain_race_car_palettes(course);
}

/* GTMAIN.EXE 80029B7C */
/* Original: GTMAIN.EXE:sub_80029B7C. */ void gtmain_race_message_flags_reset(void)
{
    w_u8(0x8008F88Fu, 0u);
    w_u8(0x8008F891u, 0u);
    w_u8(0x8008F892u, 0u);
}

/* GTMAIN.EXE 80029CA4 */
/* Original: GTMAIN.EXE:sub_80029CA4. */ uint32 gtmain_race_message_tables_prepare(void)
{
    uint32 group, index, output = 0u, value;
    gtmain_race_message_flags_reset();
    for (group = 0u; group < 3u; ++group) {
        for (index = 0u; index < 8u; ++index) {
            value = r_u16(0x8019CFDCu + group * 16u + index * 2u);
            w_u32(0x800B3920u + output * 4u, 0x8019CFDDu + value + 2u);
            ++output;
        }
        w_u32(0x800B3920u + output * 4u, 0u);
        if (group < 2u) ++output;
    }
    return output * 4u;
}

/* GTMAIN.EXE 8002AE40 */
/* Original: GTMAIN.EXE:sub_8002AE40. */ uint32 gtmain_race_configuration_snapshot(void)
{
    uint32 result = r_u32(0x800B630Cu), first, second, third, fourth;
    uint32 source, destination, index;
    uint8 fields[8], last;
    if (result) return result;
    w_u32(0x800CB8E0u, 0u);
    sub_8007BD58(0x800CB8E4u, 0u, 400u);
    w_u16(0x800CBA74u, r_u8(0x800B63BCu));
    gtmain_memory_copy(0x800CBA76u, 0x800B6650u, 10u);
    first = r_u16(0x800B63C4u); second = r_u16(0x800B63C6u);
    w_u16(0x800CBA80u, (uint16)first); w_u16(0x800CBA82u, (uint16)second);
    gtmain_memory_copy(0x800CBA84u, 0x800BB790u, 96u);
    gtmain_memory_copy(0x800CBAE4u, 0x800BB7F0u, 96u);
    source = 0x800CB7F8u; destination = 0x800CBB44u;
    do {
        first = r_u32(source); second = r_u32(source + 4u);
        third = r_u32(source + 8u); fourth = r_u32(source + 12u);
        w_u32(destination, first); w_u32(destination + 4u, second);
        w_u32(destination + 8u, third); w_u32(destination + 12u, fourth);
        source += 16u; destination += 16u;
    } while (source != 0x800CB8B8u);
    w_u16(destination, r_u16(source));
    source = r_u8(0x800CB7F4u) == 1u ? 0x800CB76Cu : 0x800CB778u;
    first = r_u32(source); second = r_u32(source + 4u); last = r_u8(source + 8u);
    w_u32(0x800CBC06u, first); w_u32(0x800CBC0Au, second); w_u8(0x800CBC0Eu, last);
    fields[0] = r_u8(0x800B6318u); fields[1] = r_u8(0x800CB7F4u);
    fields[2] = r_u8(0x800CB7F5u); fields[3] = r_u8(0x800CB7F6u);
    fields[4] = r_u8(0x800CB7F9u); fields[5] = r_u8(0x800B63D0u);
    fields[6] = r_u8(0x800B6394u); fields[7] = r_u8(0x800B634Cu);
    for (index = 0u; index < 6u; ++index) w_u8(0x800CBC0Fu + index, fields[index]);
    w_u8(0x800CBC16u, fields[6]); w_u8(0x800CBC17u, fields[7]);
    return fields[0];
}

/* GTMAIN.EXE 80029E3C */
/* Original: GTMAIN.EXE:sub_80029E3C. */ uint32 gtmain_race_driver_mode_finalize(void)
{
    if (r_u8(0x800CB7F5u) != 4u) return 4u;
    if (r_u32(0x800B630Cu) == 1u) return 1u;
    return (uint32)gtmain_driver_mode_set(0x800B6850u, 0x800B6410u, 3);
}

/* GTMAIN.EXE 8001DF84 */
/* Original: GTMAIN.EXE:sub_8001DF84. */ uint32 gtmain_race_view_initialize(uint32 view, uint32 car, uint32 pad)
{
    uint32 value;
    sub_8007BD58(view, 0u, 276u);
    w_u16(view + 4u, 320u); w_u16(view + 8u, 160u);
    w_u16(view + 12u, 164u); w_u16(view + 14u, 48u);
    w_u16(view, 0u); w_u16(view + 2u, 0u); w_u16(view + 16u, 32u);
    if (r_u8(0x800CB7F5u) == 2u) {
        w_u16(view + 6u, 120u); w_u16(view + 10u, 52u);
    } else {
        w_u16(view + 6u, 240u); w_u16(view + 10u, 104u);
    }
    gtmain_view_projection_prepare(view);
    w_u32(view + 180u, car); w_u32(view + 184u, pad);
    w_u16(view + 262u, 214u);
    value = r_u8(0x800CB75Du);
    w_u16(view + 258u, (uint16)value);
    value = r_u8(0x800CB75Du);
    w_u16(view + 260u, (uint16)value);
    return value;
}

/* GTMAIN.EXE 8002C74C */
/* Original: GTMAIN.EXE:sub_8002C74C. */ uint8 *gtmain_race_hud_tim_block(uint8 *image, uint32 capacity, sint32 xy)
{
    uint32 offset = 8u, next;
    if (!image || capacity < 12u)
        gt_draft_failure("Native HUD TIM header extent", 0x8002C74Cu);
    if (xport_load_le32(image + 4u) & 8u)
        offset += xport_load_le32(image + 8u);
    if (offset < 8u || offset > capacity)
        gt_draft_failure("Native HUD TIM block extent", 0x8002C74Cu);
    gtmain_tim_block_upload_native(image + offset, capacity - offset, xy, -1);
    next = xport_load_le32(image + offset);
    if (next > capacity - offset)
        gt_draft_failure("Native HUD TIM next pointer extent", 0x8002C74Cu);
    return image + offset + next;
}

/* GTMAIN.EXE 8002C894 */
/* Original: GTMAIN.EXE:sub_8002C894. */ uint32 gtmain_race_icons_load(void)
{
    uint32 workspace[2048];
    gtmain_texture_decode_native((uint8 *)workspace, sizeof(workspace), 0x8008FB5Cu);
    return gtmain_tim_upload_native((uint8 *)workspace, sizeof(workspace),
        0x007C01A0, -1, 0x006201A0, -1, 0u);
}

/* GTMAIN.EXE 8002C7A4 */
/* Original: GTMAIN.EXE:sub_8002C7A4. */ uint32 gtmain_race_hud_textures_load(void)
{
    uint32 workspace[2048];
    uint8 *image = (uint8 *)workspace;
    gtmain_texture_decode_native(image, sizeof(workspace), 0x8009130Cu);
    gtmain_race_hud_tim_block(image, sizeof(workspace), 0x00E70140);
    gtmain_tim_block_upload_native(image + 8u, sizeof(workspace) - 8u, 0x00FF0140, -1);
    return gtmain_race_icons_load();
}

/* GTMAIN.EXE 8002E41C */
/* Original: GTMAIN.EXE:sub_8002E41C. */ uint32 gtmain_race_overlay_textures_load(void)
{
    uint32 workspace[2048];
    uint8 *image = (uint8 *)workspace, *next;
    gtmain_texture_decode_native(image, sizeof(workspace), 0x800916E8u);
    next = gtmain_race_hud_tim_block(image, sizeof(workspace), 416);
    next = gtmain_race_hud_tim_block(next,
        sizeof(workspace) - (uint32)(next - image), 420);
    next = gtmain_race_hud_tim_block(next,
        sizeof(workspace) - (uint32)(next - image), 424);
    gtmain_race_hud_tim_block(next,
        sizeof(workspace) - (uint32)(next - image), 428);
    gtmain_tim_block_upload_native(image + 8u, sizeof(workspace) - 8u,
        0x01FF0000, -1);
    return gtmain_race_effect_state_clear();
}

/* GTMAIN.EXE 80038248 */
/* Original: GTMAIN.EXE:sub_80038248. */ uint32 gtmain_menu_backdrop_draw(uint32 ordering_link, uint32 x, uint32 y,
    uint32 red, uint32 green, uint32 blue)
{
    uint32 vertical = y << 16;
    uint32 color = (red & 255u) | ((green & 255u) << 8) |
        ((blue & 255u) << 16) | 0x01000000u;
    gtmain_packet_sprite(ordering_link, (x & 65535u) | vertical,
        0x7FD80000u, 0x00F00100u, color);
    gtmain_packet_page(ordering_link, 695u);
    gtmain_packet_sprite(ordering_link, ((x + 256u) & 65535u) | vertical,
        0x7FD80000u, 0x00F00040u, color);
    return gtmain_packet_page(ordering_link, 697u);
}

/* GTMAIN.EXE 8002A8B4 */
/* Original: GTMAIN.EXE:sub_8002A8B4. */ uint32 gtmain_race_scene_nodes_bind(void)
{
    uint32 restarted, mode, driver_mode = 2u;
    w_u32(0x800B62F8u, 1u); w_u32(0x800B62FCu, 1u);
    gtmain_ordering_node_insert(0x800B62F0u);
    w_u32(0x800B6358u, 2u); w_u32(0x800B635Cu, 2u);
    gtmain_ordering_node_insert(0x800B6350u);
    w_u32(0x800B6378u, 4u); w_u32(0x800B637Cu, 4194u);
    gtmain_ordering_node_insert(0x800B6370u);
    w_u32(0x800B6328u, 5u); w_u32(0x800B632Cu, 4192u);
    gtmain_ordering_node_insert(0x800B6320u);
    w_u32(0x800B63E8u, 6u); w_u32(0x800B63ECu, 2u);
    gtmain_ordering_node_insert(0x800B63E0u);
    gtmain_frame_prepare(2u);
    gtmain_race_frame_reset(5u);
    /* TODO Existing timing/view callees retain their named adapter boundaries */
    gtmain_race_hud_textures_load();
    restarted = r_u32(0x800B630Cu);
    w_u8(0x800B633Cu, 0u);
    w_u8(0x800B63C8u, 1u);
    w_u8(0x800B63C2u, 0u);
    if (restarted) goto Reload;
    mode = r_u8(0x800CB7F5u);
    if (mode == 2u) {
        w_u8(0x800B633Cu, 1u);
        goto Reload;
    }
    if (mode >= 3u) {
        if (mode == 5u) goto Setup;
        goto Reload;
    }
    if (mode != 1u) goto Flags;
    if (r_u8(0x800CB7F4u) == 1u) goto Reload;
Setup:
    w_u8(0x800B63C2u, 1u);
    /* TODO Bind unrecorded 80030C08 only if this original mode reaches it */
    gt_draft_unresolved_call(0x80030C08u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
Reload:
    mode = r_u8(0x800CB7F5u);
Flags:
    if (mode == 2u || mode == 4u || mode == 5u ||
        (mode == 1u && (sint32)r_u32(0x800B6318u) < 2))
        w_u8(0x800B63C8u, 0u);
    gtmain_race_view_initialize(0x800B6410u, 0x800B6850u, 0x800BC1C0u);
    gtmain_race_view_initialize(0x800B6524u, 0x800B7584u, 0x800BC280u);
    if (r_u32(0x800B630Cu)) {
        if (r_u8(0x800CB7F5u) != 2u) {
            w_u16(0x800B6508u, 1u);
            w_u16(0x800B661Cu, 1u);
        }
        driver_mode = 1u;
    }
    gtmain_driver_mode_set(0x800B6850u, 0x800B6410u, (sint32)driver_mode);
    gtmain_driver_mode_set(0x800B7584u, 0x800B6524u, (sint32)driver_mode);
    return gtmain_race_driver_mode_finalize();
}

/* GTMAIN.EXE 80024BDC */
/* Original: GTMAIN.EXE:sub_80024BDC. */ uint32 gtmain_course_depth_table(uint32 course)
{
    uint32 metadata = gtmain_course_entry(course), index = 0u, value = 0u;
    uint32 start = r_u8(metadata + 1u), end = r_u8(metadata + 2u);
    uint32 curve = r_u8(metadata) & 128u, strength = r_u8(metadata + 3u);
    start = (start * 131u) >> 7;
    end = (end * 131u) >> 7;
    while (index < start) {
        w_u16(0x801A900Cu + index * 2u, 0u);
        ++index;
    }
    if (index <= end) {
        do {
            uint32 ratio = start == end ? 4096u : ((index - start) << 12) / (end - start);
            uint32 inverse = 4096u - ratio;
            value = curve ? 4096u - (uint32)((sint32)(inverse * inverse) >> 12) : 0u;
            value = (uint32)((sint32)(strength * value) >> 12);
            w_u16(0x801A900Cu + index * 2u, (uint16)value);
            ++index;
        } while (index <= end);
    }
    while (index < 4192u) {
        w_u16(0x801A900Cu + index * 2u, (uint16)value);
        ++index;
    }
    return index * 2u;
}

/* GTMAIN.EXE 8002A85C */
/* Original: GTMAIN.EXE:sub_8002A85C. */ uint32 gtmain_race_buffers_bind(void)
{
    gtmain_frame_tables_bind((uint32 *)psx_addr(0x801AB0CCu, 33568u),
        (uint32 *)psx_addr(0x801B33ECu, 33568u), 8392u);
    gtmain_packet_buffers(0x801BB70Cu, 0x801DB70Cu, 0x20000u);
    return gtmain_course_depth_table(r_u8(0x800B63BCu));
}

/* GTMAIN.EXE 80028AB4 */
/* Original: GTMAIN.EXE:sub_80028AB4. */ sint32 gtmain_race_pad_contexts_reset(void)
{
    uint32 mode = r_u8(0x800CB7F5u), first, second;
    w_u8(0x800B63C9u, 255u);
    w_u8(0x800B6344u, 0u);
    if (mode == 2u) w_u8(0x800B63C9u, 4u);
    gtmain_frame_defaults(0x800CB782u);
    gtmain_frame_defaults(0x800CB7ABu);
    w_u8(0x800B1A8Cu, 15u);
    w_u8(0x800B1A8Du, 5u);
    gtmain_race_pad_reset(0x800BC1C0u);
    gtmain_race_pad_reset(0x800BC280u);
    gtmain_race_pad_reset(0x800BC220u);
    first = r_u8(0x800B6344u);
    w_u16(0x800BC220u, 65535u);
    second = r_u8(0x800B63C9u);
    w_u16(0x800BC1C0u, (uint16)(sint16)(sint8)first);
    w_u16(0x800BC280u, (uint16)(sint16)(sint8)second);
    return (sint8)second;
}

/* TODO Complete the independent general formatter 8007C014 */
/* GTMAIN.EXE 8002F998 */
/* Original: GTMAIN.EXE:sub_8002F998. */ uint32 gtmain_race_header_update(uint32 session)
{
    uint32 course = gtmain_course_resource_name(r_u8(0x800B63BCu));
    uint32 arguments[2], name, destination;
    if (!strncmp(psx_addr(course, 7u), psx_addr(0x8008C7C8u, 7u), 7u))
        course += 7u;
    else if (!strncmp(psx_addr(course, 3u), psx_addr(0x8008C7D0u, 3u), 3u))
        course += 3u;
    destination = r_u32(0x80091224u);
    arguments[0] = r_u32(0x80091190u + session * 4u);
    arguments[1] = 0x800BDA65u;
    gtmain_string_format(destination, 0x8008C7D4u, arguments, 2u);
    name = gtmain_resource_name(r_u32(0x800B716Cu));
    gtmain_string_copy(r_u32(0x8009126Cu), name);
    return gtmain_string_copy(r_u32(0x8009122Cu), course);
}

/* GTMAIN.EXE 8003619C */
/* Original: GTMAIN.EXE:sub_8003619C. */ sint32 gtmain_menu_angle_tick(void)
{
    uint32 previous = r_u16(0x801F2FDEu);
    uint16 next = (uint16)(previous + 10u);
    sint32 result = (sint32)previous - 4086;
    w_u16(0x801F2FDEu, next);
    if ((sint16)next >= 4096) w_u16(0x801F2FDEu, (uint16)result);
    return result;
}

/* GTMAIN.EXE 8002CB3C */
/* Original: GTMAIN.EXE:sub_8002CB3C. */ uint32 gtmain_race_records_size(void)
{
    uint32 first = gtmain_race_record_size(0x800CB8E0u), second = 0u;
    if (r_u8(0x800CBC11u) == 2u) second = gtmain_race_record_size(0x800CDC34u);
    return first + second;
}

/* GTMAIN.EXE 8003D334 */
/* Original: GTMAIN.EXE:sub_8003D334. */ uint32 gtmain_menu_model_bundle_relocate(uint32 bundle, uint32 page)
{
    return gtmain_model_bundle_relocate(bundle, page);
}

/* GTMAIN.EXE 8002E3C4 */
/* Original: GTMAIN.EXE:sub_8002E3C4. */ uint32 gtmain_race_tim_block_upload(uint32 image, sint32 xy)
{
    uint32 block = image + 8u;
    if (r_u32(image + 4u) & 8u) block = image + r_u32(image + 8u) + 8u;
    gtmain_tim_block_upload(block, xy, -1);
    return block + r_u32(block);
}

/* GTMAIN.EXE 8003D1E0 */
/* Original: GTMAIN.EXE:sub_8003D1E0. */ sint32 gtmain_menu_model_pose_tick(uint32 pose, uint32 rotate)
{
    sint32 result = (sint16)r_u16(pose + 20u);
    if (result > 0) {
        uint32 remaining = r_u16(pose + 20u);
        uint32 first = r_u16(pose), second = r_u16(pose + 2u), third;
        uint32 x, y, z;
        first = (first + 24u) & 4095u;
        second = (second + 20u) & 4095u;
        w_u16(pose, (uint16)first);
        third = r_u16(pose + 4u);
        w_u16(pose + 2u, (uint16)second);
        x = r_u32(pose + 8u);
        w_u16(pose + 4u, (uint16)(third & 4095u));
        y = r_u32(pose + 12u);
        w_u16(pose + 20u, (uint16)(remaining - 1u));
        w_u32(pose + 8u, x);
        z = r_u32(pose + 16u);
        y -= 100u;
        z += 7200u;
        w_u32(pose + 12u, y);
        w_u32(pose + 16u, z);
        result = (sint32)y;
    } else if (rotate) {
        result = (sint32)((r_u16(pose + 2u) + 12u) & 4095u);
        w_u16(pose + 2u, (uint16)result);
    }
    return result;
}

/* GTMAIN.EXE 8002CC2C */
/* Original: GTMAIN.EXE:sub_8002CC2C. */ uint32 gtmain_race_records_import(uint32 source)
{
    uint32 size = gtmain_race_record_size(source), second;
    gtmain_race_record_copy(0x800CB8E0u, source, size);
    if (r_u8(source + 817u) != 2u) return 2u;
    source += size;
    second = gtmain_race_record_size(source);
    return gtmain_race_record_copy(0x800CDC34u, source, second);
}
