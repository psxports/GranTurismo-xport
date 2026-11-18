#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "psx_gte.h"

/* TODO Bind remaining original scene owners */
static uint32 scene_call(uint32 owner, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4)
{
    return gt_draft_unresolved_call(owner, a0, a1, a2, a3, a4, 0, 0, 0);
}

static uint32 scene_asr(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value >> (shift & 31u));
}

static uint32 scene_abs(uint32 value)
{
    return (sint32)value < 0 ? 0u - value : value;
}

static void scene_native_view_init(uint8 *view)
{
    (void)gtmain_view_projection_prepare_native(view);
}

static void scene_native_orientation(uint8 *view, uint32 reference)
{
    (void)gtmain_model_orientation_native(view, reference);
}

static void scene_native_view_copy(uint32 destination, const uint8 *view)
{
    (void)gtmain_render_view_copy_native(destination, view);
}

static uint32 scene_gte_flags(void)
{
    uint32 flags;
    gte_stflg(&flags);
    return flags;
}

/* GTMAIN.EXE 80028C48 */
/* Original: GTMAIN.EXE:sub_80028C48. */ uint32 gtmain_race_pad_map(uint32 state, uint32 packet, uint32 buttons, uint32 threshold,
    uint32 first_axis, uint32 second_axis, uint32 analog)
{
    uint32 mapping, held, flags, physical, index;
    gtmain_race_pad_axes_reset(state);
    mapping = r_u8(buttons + 2u);
    held = r_u16(state + 80u);
    if (mapping < 16u) {
        uint32 digital = r_u16(state + 6u);
        if (digital & 0x80u) held |= 1u;
        if (digital & 0x20u) held |= 2u;
        if ((held & 3u) == 3u) held &= ~3u;
    } else {
        sint32 value = r_u8(packet + (mapping & 15u) + 4u);
        sint32 center = r_u8(threshold);
        sint32 maximum = r_u8(threshold + 2u);
        sint32 deadzone = r_u8(threshold + 1u);
        sint32 distance = value - center;
        sint32 negative = distance < 0;
        sint32 span = maximum - deadzone, scaled;
        if (negative) distance = -distance;
        distance -= deadzone;
        if (!span) span = 1;
        if (distance < 0) distance = 0;
        else if (distance >= span) distance = span;
        scaled = (sint32)((uint32)distance << 6) / span;
        if (negative) scaled = -scaled;
        flags = r_u8(state + 82u);
        w_u8(state + 83u, (uint8)scaled);
        w_u8(state + 82u, (uint8)(flags | 1u));
    }
    w_u16(state + 80u, (uint16)held);
    mapping = r_u8(buttons + 3u);
    w_u8(state + 84u, (uint8)gtmain_race_pad_axis_value((sint32)mapping, state, packet, first_axis, analog));
    mapping = r_u8(buttons + 4u);
    w_u8(state + 85u, (uint8)gtmain_race_pad_axis_value((sint32)mapping, state, packet, second_axis, analog));
    if (r_u8(buttons + 3u) >= 16u) w_u8(state + 82u, (uint8)(r_u8(state + 82u) | 2u));
    if (r_u8(buttons + 4u) >= 16u) w_u8(state + 82u, (uint8)(r_u8(state + 82u) | 4u));
    held = r_u16(state + 80u);
    physical = r_u16(state + 6u);
    for (index = 0; index < 6u; ++index) {
        mapping = r_u8(buttons + 3u + index);
        if ((physical >> (mapping & 31u)) & 1u) held |= 4u << index;
    }
    mapping = r_u8(buttons);
    if ((physical >> (mapping & 31u)) & 1u) held |= 256u;
    mapping = r_u8(buttons + 1u);
    if ((physical >> (mapping & 31u)) & 1u) held |= 512u;
    w_u16(state + 80u, (uint16)held);
    return (physical >> (mapping & 31u)) & 1u;
}

/* GTMAIN.EXE 800315A8 */
/* Original: GTMAIN.EXE:sub_800315A8. */ uint32 gtmain_race_sound_car_primary(uint32 car)
{
    return gtmain_race_sound_car_channels_prepare(car, 0u);
}

/* GTMAIN.EXE 800315C8 */
/* Original: GTMAIN.EXE:sub_800315C8. */ uint32 gtmain_race_sound_car_secondary(uint32 car)
{
    return gtmain_race_sound_car_channels_prepare(car, 2u);
}

/* GTMAIN.EXE 8002D9C4 */
/* Original: GTMAIN.EXE:sub_8002D9C4. */ uint32 gtmain_race_lap_time_append(uint32 time)
{
    sint32 index = r_s8(0x800B399Cu);
    uint32 result = (uint32)index << 2u;
    if (index < 100) {
        w_u32(0x800BDB70u + ((uint32)index << 2), time);
        ++index;
    }
    w_u8(0x800B399Cu, (uint8)index);
    return result;
}

static void race_two_player_balance(uint32 first, uint32 second)
{
    /* TODO Bind the unselected original two-player pair service */
    gtmain_race_pair_boost_update(first, second);
}

/* GTMAIN.EXE 80052C48 */
/* Original: GTMAIN.EXE:sub_80052C48. */ void gtmain_race_physics_tick(void)
{
    sint32 count = r_s16(0x800BC1A6u), index;
    uint32 mode;
    gtmain_race_physics_prepare(0x800B6850u, count);
    count = r_s16(0x800BC1A6u);
    for (index = 0; index < count; ++index) w_u8(0x800B6850u + 3380u * (uint32)index + 1738u, 0);
    if (!r_s16(0x800B63DEu)) gtmain_race_collisions_tick(0x800B6850u, count);
    gtmain_race_wheel_contacts_prepare(0x800B6850u, r_s16(0x800BC1A6u));
    gtmain_race_wheel_contacts_query(0x800B6850u, r_s16(0x800BC1A6u));
    gtmain_race_wheel_contacts_apply(0x800B6850u, r_s16(0x800BC1A6u));
    gtmain_race_pose_update(0x800B6850u, r_s16(0x800BC1A6u));
    gtmain_race_positions_sort(0x800B6850u, r_s16(0x800BC1A6u));
    (void)r_s16(0x800BC1A6u);
    gtmain_race_lap_groups_tick(0x800B6850u);
    gtmain_race_clock_tick();
    for (index = 0; index < r_s16(0x800BC1A6u); ++index)
        gtmain_car_collision_positions(0x800B6850u + 3380u * (uint32)index);
    mode = r_u8(0x800CB7F5u);
    if (mode == 0u || mode == 3u || mode == 5u) {
        for (index = 0; index < r_s8(0x800B6F19u) - 1; ++index) {
            sint32 first = r_s8(0x800BFE41u + (uint32)index);
            sint32 second = r_s8(0x800BFE40u + (uint32)index);
            gtmain_race_balance_update(0x800B6878u + 3380u * (uint32)first, 0x800B6878u + 3380u * (uint32)second);
        }
        for (index = r_s8(0x800B6F19u); index < r_s16(0x800BC1A6u); ++index) {
            sint32 first = r_s8(0x800BFE3Fu + (uint32)index);
            sint32 second = r_s8(0x800BFE40u + (uint32)index);
            gtmain_race_balance_update(0x800B6878u + 3380u * (uint32)first, 0x800B6878u + 3380u * (uint32)second);
        }
    } else if (mode == 2u) race_two_player_balance(0x800B6878u, 0x800B75ACu);
}

/* GTMAIN.EXE 8001E07C */
/* Original: GTMAIN.EXE:sub_8001E07C. */ uint32 gtmain_race_camera_tick(uint32 camera)
{
    uint32 mode = r_u32(0x800B630Cu), x, y, z, fourth, last;
    sint32 kind, shift;
    uint32 dot, speed;
    if (mode == 1u && (r_u16(r_u32(camera + 184u) + 8u) & 0xF000u)) {
        kind = r_s16(camera + 246u);
        if (kind == 1) {
            if (r_s16(camera + 248u)) w_u16(camera + 248u, 0);
            else w_u16(camera + 246u, 2);
        } else if (kind == 2) {
            w_u16(camera + 248u, 1); w_u16(camera + 246u, 1);
        }
    }
    x = r_u32(camera + 120u); y = r_u32(camera + 124u); z = r_u32(camera + 128u);
    w_u32(camera + 208u, x); w_u32(camera + 212u, y); w_u32(camera + 216u, z);
    kind = r_s16(camera + 246u);
    w_u8(camera + 254u, 0); w_u8(camera + 255u, 0);
    if (kind == 1) gtmain_camera_interval_update(camera);
    else if (kind == 2) gtmain_camera_mode_update(camera);
    gtmain_model_orientation(camera, 0x800BFF80u);
    gtmain_vector_subtract((uint8 *)psx_addr(camera + 220u, 12u), (const uint8 *)psx_addr(camera + 120u, 12u), (const uint8 *)psx_addr(camera + 208u, 12u));
    shift = gtmain_vector_narrow((uint8 *)psx_addr(camera + 232u, 6u), (const uint8 *)psx_addr(camera + 220u, 12u));
    dot = (uint32)gtmain_vector_normalize_xyz((uint8 *)psx_addr(camera + 232u, 6u), (const uint8 *)psx_addr(camera + 232u, 6u));
    speed = shift < 0 ? scene_asr(dot, (uint32)shift) : dot << ((uint32)shift & 31u);
    speed = scene_asr(speed, 4u);
    w_u32(camera + 240u, (sint32)speed < 11605 ? speed : 0u);
    x = r_u32(camera + 160u); y = r_u32(camera + 164u); z = r_u32(camera + 168u); fourth = r_u32(camera + 172u);
    w_u32(camera + 188u, x); w_u32(camera + 192u, y); w_u32(camera + 196u, z); w_u32(camera + 200u, fourth);
    last = r_u32(camera + 176u); w_u32(camera + 204u, last);
    return last;
}

/* GTMAIN.EXE 8001E218 */
/* Original: GTMAIN.EXE:sub_8001E218. */ uint32 gtmain_race_split_screen_update(void)
{
    sint32 mode = 1, previous;
    uint16 height = 240, width = 104;
    if (r_s16(0x800B6506u) == 1 && r_s16(0x800B661Au) == 1 && r_s16(0x800B6508u) && r_s16(0x800B661Cu)) mode = 2;
    previous = r_s8(0x800B633Cu);
    w_u8(0x800B633Cu, (uint8)mode);
    if (mode == previous) return (uint32)previous;
    if (mode == 1) { height = 120; width = 52; }
    w_u16(0x800B6416u, height); w_u16(0x800B641Au, width);
    w_u16(0x800B652Au, height); w_u16(0x800B652Eu, width);
    gtmain_view_projection_prepare(0x800B6410u);
    return gtmain_view_projection_prepare(0x800B6524u);
}

/* GTMAIN.EXE 8002E690 */
/* Original: GTMAIN.EXE:sub_8002E690. */ uint32 gtmain_race_effects_draw(void)
{
    uint32 effect = 0x800B39D4u, packet, ordering, group, bit;
    gte_ctc2(0, 5); gte_ctc2(0, 6); gte_ctc2(0, 7);
    gtmain_matrix_bind_negated_axes((const uint8 *)psx_addr(r_u32(0x800B6638u) + 160u, 20u));
    gte_ctc2(0, 28);
    packet = r_u32(0x800BFE4Cu); ordering = r_u32(0x800B6338u);
    for (group = 0; group < 4u; ++group) {
        uint32 active = r_u32(0x800B39C4u + 4u * group);
        for (bit = 0; bit < 32u; ++bit, effect += 32u) {
            uint32 ex, ey, ez, camera, x, y, z, ax, ay, az, swap, distance, shift = 0;
            uint32 coefficient, magnitude, leading, xy, rgb, uv, page, screen, flags, radius, depth, lifetime, link;
            sint32 sx, sy;
            if (!((active >> bit) & 1u)) continue;
            ex = r_u32(effect + 12u); camera = r_u32(0x800B6638u);
            ey = r_u32(effect + 16u); x = ex - r_u32(camera + 120u);
            ez = r_u32(effect + 20u); y = ey - r_u32(camera + 124u); z = ez - r_u32(camera + 128u);
            ax = scene_abs(x); ay = scene_abs(y); az = scene_abs(z);
            if ((sint32)ax < (sint32)ay) { swap = ax; ax = ay; ay = swap; }
            if ((sint32)ax < (sint32)az) { swap = ax; ax = az; az = swap; }
            if ((sint32)ay < (sint32)az) { swap = ay; ay = az; az = swap; }
            distance = ax + scene_asr(ay, 1) + scene_asr(az, 2);
            if ((sint32)distance > 0x3FFFFF) continue;
            coefficient = (uint32)r_s16(effect + 24u);
            magnitude = scene_abs(x) | scene_abs(y) | scene_abs(z);
            xport_gte_write_data(30u, magnitude); leading = xport_gte_read_data(31u);
            if ((sint32)(31u - leading) >= 13) {
                shift = 19u - leading;
                x = scene_asr(x, shift); y = scene_asr(y, shift); z = scene_asr(z, shift);
            }
            gte_ctc2(coefficient, 27u);
            xport_gte_write_data(0u, (x & 0xFFFFu) | (y << 16)); xport_gte_write_data(1u, z);
            xport_gte_execute(0x180001u);
            rgb = r_u32(effect); uv = r_u32(effect + 4u); page = r_u32(effect + 8u);
            flags = scene_gte_flags(); screen = xport_gte_read_data(14u);
            if ((sint32)flags < 0) continue;
            radius = xport_gte_read_data(24u); depth = xport_gte_read_data(19u);
            radius = scene_asr(radius, shift + 8u);
            if ((sint32)radius >= 256) continue;
            sy = (sint32)scene_asr(screen, 16); sx = (sint16)screen;
            depth = scene_asr(depth << (shift & 31u), 13u);
            if ((sint32)depth >= 4096) depth = 4095;
            lifetime = r_u16(effect + 30u);
            w_u32(packet, 0x09000000u);
            link = ordering + (depth << 2);
            w_u8(packet + 7u, 46); w_u32(packet + 12u, rgb); w_u32(packet + 20u, uv); w_u32(packet + 28u, page);
            lifetime = scene_asr(lifetime << 16, 23);
            w_u8(packet + 4u, (uint8)lifetime); w_u8(packet + 5u, (uint8)lifetime); w_u8(packet + 6u, (uint8)lifetime);
            w_u8(packet + 36u, (uint8)(page >> 16)); w_u8(packet + 37u, (uint8)(page >> 24));
            w_u16(packet + 10u, (uint16)((uint32)sy - radius)); w_u16(packet + 18u, (uint16)((uint32)sy - radius));
            w_u16(packet + 26u, (uint16)((uint32)sy + radius)); w_u16(packet + 34u, (uint16)((uint32)sy + radius));
            w_u16(packet + 8u, (uint16)((uint32)sx - radius)); w_u16(packet + 16u, (uint16)((uint32)sx + radius));
            w_u16(packet + 24u, (uint16)((uint32)sx - radius)); w_u16(packet + 32u, (uint16)((uint32)sx + radius));
            xy = r_u32(link) & 0x00FFFFFFu;
            w_u16(link, (uint16)packet); w_u8(link + 2u, (uint8)(packet >> 16));
            w_u16(packet, (uint16)xy); w_u8(packet + 2u, (uint8)(xy >> 16));
            packet += 40u;
        }
    }
    w_u32(0x800BFE4Cu, packet);
    return 0u;
}

/* GTMAIN.EXE 8002FFB4 */
/* Original: GTMAIN.EXE:sub_8002FFB4. */ uint32 gtmain_race_player_hud_draw(sint32 player, uint32 source)
{
    uint32 top = player == 0 ? 12u : 0u, lower = top ^ 12u;
    uint32 link = r_u32(0x800B6368u) + 4u;
    gtmain_race_status_draw(link, player ? 20 : 8, source);
    (void)gtmain_race_lap_counter_draw(link, 16, (sint32)(16u - top), source);
    (void)gtmain_race_position_draw(link, 64, (sint32)(16u - top), player ? 0x800912B4u : 0x800912BCu, source);
    (void)gtmain_race_instruments_draw(link, 238, (sint32)(lower + 30u), source);
    return gtmain_race_meter_panel_draw(link, 16u, lower + 72u, source);
}

/* GTMAIN.EXE 8002B6CC */
/* Original: GTMAIN.EXE:sub_8002B6CC. */ uint32 gtmain_race_demo_view_draw(uint32 context)
{
    uint32 storage = gt_local_stack_workspace(0x2780u, 184u, 0x8002B6CCu);
    uint8 *view = (uint8 *)psx_addr(storage, 184u), *vector = view + 120u, *matrix = view + 140u;
    uint32 camera, y, depth, horizontal, vertical;
    xport_store_le16(view + 4u, 110); xport_store_le16(view + 6u, 32); xport_store_le16(view + 10u, 16);
    xport_store_le16(view, 0); xport_store_le16(view + 2u, 0); xport_store_le16(view + 8u, 55);
    xport_store_le16(view + 12u, 55); xport_store_le16(view + 14u, 8); xport_store_le16(view + 16u, 8);
    scene_native_view_init(view);
    camera = r_u32(0x800B6638u);
    gtmain_model_rotation(matrix, 0, -32, 0);
    gtmain_matrix_multiply(matrix, matrix, (const uint8 *)psx_addr(0x80099534u, 32u));
    gtmain_matrix_multiply(matrix, (const uint8 *)psx_addr(camera + 140u, 32u), matrix);
    xport_store_le32(vector + 4u, 13107); xport_store_le32(vector, 0); xport_store_le32(vector + 8u, 32768);
    gtmain_model_rotate_vector(vector, (const uint8 *)psx_addr(camera + 140u, 32u), vector);
    gtmain_vector_add(vector, vector, (const uint8 *)psx_addr(camera + 120u, 12u));
    scene_native_orientation(view, 0x800BFF80u);
    scene_native_view_copy(context, view);
    depth = r_u16(context + 32u); horizontal = r_u32(context + 24u); gte_ctc2(depth, 26);
    vertical = r_u32(context + 28u); gte_ctc2(horizontal, 24); gte_ctc2(vertical, 25);
    y = r_u32(0x800BFF6Cu) == 0u ? 240u : 0u;
    gtmain_packet_draw_area(r_u32(0x800B6388u), y << 11, y << 10, ((y + 239u) << 10) | 319u);
    w_u32(context + 772u, r_u32(0x800B6388u) + 16768u);
    (void)gt_draft_8001A0E0(context);
    gtmain_course_scene_draw(context, r_u32(0x800B6388u), 1);
    (void)gtmain_race_cars_secondary_draw(context, r_u32(0x800B6388u));
    (void)gtmain_packet_outline(r_u32(0x800B6388u), 0x8008FB4Cu, 0);
    (void)gtmain_packet_outline(r_u32(0x800B6388u), 0x8008FB54u, 0);
    return gtmain_packet_draw_area(r_u32(0x800B6388u) + 16772u, ((y + 14u) << 11) | 105u, ((y + 14u) << 10) | 105u, ((y + 45u) << 10) | 214u);
}

/* GTMAIN.EXE 8002ABC0 */
/* Original: GTMAIN.EXE:sub_8002ABC0. */ uint32 gtmain_race_stream_start(sint32 entry, sint32 parameter)
{
    uint32 enabled = r_u8(0x800CB767u);
    w_u8(0x800B63D9u, 0);
    if (enabled && entry >= 0) {
        sint32 found = gtmain_stream_entry_find((uint32)entry);
        gtmain_stream_entry_prepare((uint32)found, (uint32)parameter);
        gtmain_stream_begin_if_loaded();
        w_u8(0x800B63D9u, 1);
        w_u8(0x800B63C1u, (uint8)(parameter ? entry : -1));
        w_u8(0x800B6314u, (uint8)parameter);
        return 0xFFFFFFFFu;
    }
    return enabled;
}
