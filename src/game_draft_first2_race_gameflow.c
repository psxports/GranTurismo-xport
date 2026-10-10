#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

/* TODO Bind missing original owners without inventing argument carriers */
static uint32 race_call(uint32 owner, uint32 a0, uint32 a1, uint32 a2)
{
    if (owner == 0x8002FF48u) return (uint32)gtmain_race_banner_draw();
    if (owner == 0x80025FCCu) return gtmain_race_standard_hud_draw();
    return gt_draft_unresolved_call(owner, a0, a1, a2, 0, 0, 0, 0, 0);
}

static uint32 race_asr(uint32 value, uint32 bits)
{
    return (uint32)((sint32)value >> bits);
}

static uint32 race_bounds_native(uint32 ordering, const sint16 *bounds, uint32 rgb)
{
    sint32 x0 = bounds[0], y0 = bounds[1], x1 = bounds[2], y1 = bounds[3];
    uint32 comparison = y1 < y0, packet, previous;
    y0 += (sint32)comparison; y1 += (sint32)(comparison ^ 1u);
    packet = r_u32(0x800BFE4Cu);
    w_u32(packet, 0x05000000u);
    comparison = x1 < x0;
    x0 += (sint32)comparison; x1 += (sint32)(comparison ^ 1u);
    w_u32(packet + 4u, rgb ^ 0x29000000u);
    w_u16(packet + 8u, (uint16)x0); w_u16(packet + 10u, (uint16)y0);
    w_u16(packet + 12u, (uint16)x1); w_u16(packet + 14u, (uint16)y0);
    w_u16(packet + 16u, (uint16)x0); w_u16(packet + 18u, (uint16)y1);
    w_u16(packet + 20u, (uint16)x1); w_u16(packet + 22u, (uint16)y1);
    previous = r_u8(ordering) | ((uint32)r_u8(ordering + 1u) << 8) | ((uint32)r_u8(ordering + 2u) << 16);
    w_u8(ordering, (uint8)packet); w_u8(ordering + 1u, (uint8)(packet >> 8)); w_u8(ordering + 2u, (uint8)(packet >> 16));
    w_u8(packet, (uint8)previous); w_u8(packet + 1u, (uint8)(previous >> 8)); w_u8(packet + 2u, (uint8)(previous >> 16));
    w_u32(0x800BFE4Cu, packet + 24u);
    return packet;
}

static void race_format_native(char *buffer, uint32 capacity, uint32 first, uint32 second)
{
    /* TODO Bind formatter output to the real native destination */
    (void)buffer; (void)capacity; (void)first; (void)second;
    gt_draft_failure("GTMAIN native race menu formatting", 0x8007C014u);
}

static sint32 race_text_width_native(const char *text)
{
    /* TODO Bind native formatted text to the original width algorithm */
    gt_draft_failure("GTMAIN native race menu text width", 0x8006C900u);
}

static void race_text_draw_native(uint32 link, sint16 x, sint16 y, const char *text)
{
    /* TODO Bind native formatted text to the original text renderer */
    (void)link; (void)x; (void)y; (void)text;
    gt_draft_failure("GTMAIN native race menu text draw", 0x8006CA10u);
}

/* GTMAIN.EXE 8002B2A4 */
void gtmain_race_stream_transition(sint32 previous, sint32 current)
{
    if (previous != current) {
        if (previous) {
            gt_draft_8002AB48();
            gtmain_race_sound_callbacks_active();
        } else {
            gtmain_stream_stop_wait(0);
            gtmain_race_sound_callbacks_stopped();
        }
    }
    if (!gtmain_stream_stop_async()) {
        if (current) return;
        gt_draft_8002AB48();
    }
    if (!current) gtmain_race_sound_update();
}

/* GTMAIN.EXE 8002B328 */
void gtmain_race_frame_tick(void)
{
    sint32 mode, previous, current;
    gtmain_race_input_poll();
    mode = r_s16(0x800B63D6u);
    previous = r_s8(0x800B63D8u);
    if (mode == 0) gtmain_race_pause_input();
    else if ((mode > 0 && mode < 3) || mode == 4)
        gtmain_race_demo_input();
    current = r_s8(0x800B63D8u);
    gtmain_race_stream_transition(previous, current);
    if (previous | current) {
        gtmain_race_rumble_clear();
        return;
    }
    gtmain_race_countdown_tick();
    gtmain_race_mode_tick();
    gtmain_race_effects_tick();
    gtmain_race_cars_tick();
    gtmain_race_hud_tick();
    gtmain_race_camera_tick(0x800B6410u);
    if (r_u8(0x800CB7F5u) == 2) {
        gtmain_race_camera_tick(0x800B6524u);
        gtmain_race_split_screen_update();
    }
    gtmain_race_pause_blink();
}

/* GTMAIN.EXE 8002D860 */
uint32 gtmain_race_object_flag(uint32 object)
{
    return object ? r_u16(object + 12u) : 1u;
}

/* GTMAIN.EXE 8002B44C */
sint32 gtmain_race_frame_status(void)
{
    if (r_u32(0x800B630Cu) == 1u) {
        if (r_u8(0x800CB7F5u) == 2u &&
            gtmain_race_object_flag(r_u32(0x800B75A0u))) return 3;
        if (gtmain_race_object_flag(r_u32(0x800B686Cu))) return 3;
    }
    return r_s8(0x800B63C0u);
}

/* GTMAIN.EXE 8002B4C0 */
void gtmain_race_mode_prepare(void)
{
    uint32 state = r_u32(0x800B630Cu);
    w_u8(0x800B63CAu, 1);
    if (!state) {
        gtmain_race_rank_list_sort();
        gtmain_race_player_positions();
        gtmain_race_player_positions_latch();
    }
    switch (r_u8(0x800CB7F5u)) {
    case 0: gtmain_race_mode0_prepare(); break;
    case 1: race_call(0x8002633Cu, 0, 0, 0); break;
    case 2: race_call(0x800264D0u, 0, 0, 0); break;
    case 3: race_call(0x80022FE8u, 0, 0, 0); break;
    case 5: race_call(0x800234CCu, 0, 0, 0); return;
    default: return;
    }
    gt_draft_8002AC58(0, 0, 0, 0, 0, 0, 0, 0);
}

/* GTMAIN.EXE 8002B5A4 */
void gtmain_race_ordering_reset(void)
{
    if (r_s8(0x800B633Cu)) {
        gtmain_ordering_clear(0);
        gtmain_packet_buffer_select(0);
    }
}

/* GTMAIN.EXE 800304A8 */
void gtmain_race_pause_draw(void)
{
    uint32 link, text;
    sint16 x;
    if (!r_s16(0x800B6312u) || r_u32(0x800B630Cu) != 1u) return;
    link = r_u32(0x800B6368u);
    gtmain_text_font(3);
    gtmain_text_color(0x00808080u);
    x = r_s16(0x80091228u);
    text = r_u32(0x8009122Cu);
    gtmain_font_text_draw(link, x, 68, text);
    if (r_s8(0x800912A8u) >= 0) {
        x = r_s16(0x80091208u);
        text = r_u32(0x8009120Cu);
        gtmain_font_text_draw(link, x, 96, text);
    }
}

/* GTMAIN.EXE 80030548 */
void gtmain_race_hud_draw(void)
{
    race_call(0x8002FF48u, 0, 0, 0);
    if (r_s8(0x800B63C2u)) race_call(0x80030CA0u, 0, 0, 0);
    switch (r_u8(0x800CB7F5u)) {
    case 0: race_call(0x80025FCCu, 0, 0, 0); break;
    case 1: race_call(0x800263A4u, 0, 0, 0); break;
    case 2:
        race_call(0x8002E368u, 0, 0, 0);
        race_call(0x8003061Cu, r_u32(0x800B6368u), 110, 0);
        break;
    case 3: race_call(0x8002332Cu, 0, 0, 0); break;
    case 4: race_call(0x8002A300u, 0, 0, 0); break;
    case 5: race_call(0x80023554u, 0, 0, 0); break;
    default: break;
    }
}

/* GTMAIN.EXE 8002D1D0 */
void gtmain_race_menu_draw(void)
{
    sint16 bounds[4];
    char formatted[256];
    sint32 x, y, width, count, selected;
    uint32 packet, link, blink, index;
    if (!r_s8(0x800B63D8u) || r_s16(0x800B63D6u)) return;
    y = r_s16(0x800B3996u);
    x = r_s16(0x800B3994u);
    bounds[0] = (sint16)x; bounds[1] = (sint16)y;
    width = r_s16(0x800B3998u);
    bounds[2] = (sint16)((uint32)x + (uint32)width);
    bounds[3] = (sint16)((uint32)y + (uint32)r_s16(0x800B399Au));
    packet = gtmain_packet_page(r_u32(0x800B6308u), 64);
    link = race_bounds_native(packet, bounds, 0x02606060u);
    gtmain_text_font(1);
    y = (sint32)((uint32)y + 3u);
    blink = r_u8(0x8008FBF3u);
    count = r_s8(0x8008FBF2u);
    selected = r_s8(0x8008FBF1u);
    w_u8(0x8008FBF3u, (uint8)(blink + 1u));
    for (index = 0; (sint32)index < count; ++index) {
        uint32 entry = r_u8(r_u32(0x8008FBF4u) + index);
        uint32 text = r_u32(0x8008FBF8u + 4u * entry);
        sint32 text_width;
        uint32 color = ((blink & 1u) && (sint32)index == selected) ? 128u : 0x00808080u;
        gtmain_text_color(color);
        if (entry == 7u) {
            uint32 second = r_u8(0x800CB764u) ? 0x800BC300u : 0x800BC305u;
            race_format_native(formatted, sizeof(formatted), text, second);
            race_text_width_native(formatted);
            text_width = race_text_width_native(formatted);
            race_text_draw_native(link, (sint16)((uint32)x + race_asr((uint32)width - (uint32)text_width, 1)), (sint16)y, formatted);
        } else {
            text_width = gtmain_text_width(1, text);
            gtmain_font_text_draw(link, (sint16)((uint32)x + race_asr((uint32)width - (uint32)text_width, 1)), (sint16)y, text);
        }
        y = (sint32)((uint32)y + 10u);
    }
}

/* GTMAIN.EXE 8002B620 */
void gtmain_race_overlay_draw(sint32 player)
{
    uint32 y;
    if (r_s16(0x800B6508u + 276u * (uint32)player)) gtmain_race_pause_draw();
    else gtmain_race_hud_draw();
    gtmain_race_menu_draw();
    y = r_u32(0x800BFF68u) ? 240u : 0u;
    if (player) y ^= 240u;
    gtmain_packet_draw_area(r_u32(0x800B6368u), y << 11, y << 10, ((y + 239u) << 10) | 319u);
}

/* GTMAIN.EXE 8002B8D8 */
void gtmain_race_view_draw(sint32 view_index)
{
    uint32 view = r_u32(0x800B6638u), scene, depth, horizontal, vertical, y;
    const uint32 scratch = 0x1F800000u;
    w_u32(0x8008F71Cu, (uint32)r_s16(view + 244u));
    gtmain_render_view_copy(scratch, view);
    gtmain_render_fog_set(scratch, 0, 256, 0);
    depth = r_u16(scratch + 32u);
    horizontal = r_u32(scratch + 24u);
    gte_ctc2(depth, 26u);
    vertical = r_u32(scratch + 28u);
    gte_ctc2(horizontal, 24u); gte_ctc2(vertical, 25u);
    gt_draft_8001C108(scratch, r_u32(0x800B63F8u), (uint32)view_index, 0, 0, 0, 0, 0);
    scene = gtmain_course_scene_draw(scratch, r_u32(0x800B6338u), 0);
    if (!r_s8(0x8008FBE1u)) gt_draft_8002F6EC(scratch, scene, 0, 0, 0, 0, 0, 0);
    gtmain_race_cars_draw(scratch);
    gtmain_race_effects_draw();
    if (view_index) {
        if (!r_s16(view + 248u))
            gtmain_race_player_hud_draw((sint32)((uint32)view_index - 1u), r_u32(view + 180u));
        y = r_u32(0x800BFF68u) == (uint32)view_index - 1u ? 240u : 0u;
        if (view_index == 1) y += 120u;
        gtmain_packet_draw_area(r_u32(0x800B63F8u) + 4u, y << 11, y << 10, (((y + 119u) & 511u) << 10) | 319u);
    }
    if (r_s8(0x800B63C8u) && gtmain_driver_demo_active())
        gtmain_race_demo_view_draw(scratch);
}

/* GTMAIN.EXE 8002BB8C */
void gtmain_race_updates_reset(void)
{
    gtmain_race_time_reset();
    gtmain_race_lap_history_reset();
    gtmain_race_split_reset();
    gtmain_race_best_reset();
    gtmain_race_assist_reset();
    gtmain_race_timers_reset();
}

/* GTMAIN.EXE 8002BEB4 */
void gtmain_race_messages_load(void)
{
    uint32 buffer[512], sector;
    uint32 language = r_u8(0x800C4C3Du);
    GTMainArchiveTable table = gtmain_archive_table_load(0x8008C01Cu, &sector, (uint8 *)buffer, 0, sizeof(buffer));
    gtmain_archive_section_load(0x8008C01Cu, table.bytes, sector, language, 0x800BD7F0u, 0);
    gtmain_archive_section_load(0x8008C01Cu, table.bytes, sector, language + 6u, 0x800BC300u, 0);
    if (r_u16(0x800CB7F4u) == 1026u)
        gtmain_archive_section_load(0x8008C01Cu, table.bytes, sector, language + 18u, 0x8019CFDCu, 0);
}

/* GTMAIN.EXE 8002BF84 */
void gtmain_race_point_rotate(uint32 guest_xy, uint32 angle, sint32 origin_x, sint32 origin_y)
{
    uint32 offset = (angle & 4095u) << 1;
    sint32 x = r_s16(guest_xy);
    sint32 cosine = r_s16(0x80094A3Cu + offset);
    uint32 xc = (uint32)x * (uint32)cosine;
    sint32 y = r_s16(guest_xy + 2u);
    uint32 yc = (uint32)y * (uint32)cosine;
    sint32 sine = r_s16(0x8009423Cu + offset);
    uint32 ys = (uint32)y * (uint32)sine;
    uint32 xs = (uint32)x * (uint32)sine;
    uint32 rotated_x = race_asr(race_asr(xc, 12) - race_asr(ys, 12) + 576u, 4);
    w_u16(guest_xy, (uint16)((uint32)origin_x + rotated_x));
    w_u16(guest_xy + 2u, (uint16)((uint32)origin_y + race_asr(race_asr(yc, 12) + race_asr(xs, 12) + 576u, 4)));
}


uint32 gtmain_race_labels_draw(uint32 ordering, sint32 y, uint32 mode, uint32 second_line)
{
    if (r_u16(0x800B63DEu))
    {
        uint32 table = mode == 0u ? 0x80091218u : mode == 1u ? 0x80091228u : 0x80091210u;
        sint32 first_y = y;
        gtmain_text_font(3);
        gtmain_text_color(0x00808080u);
        y = (sint32)((uint32)y + 28u);
        gtmain_font_text_draw(ordering, (sint16)r_u16(table), first_y, r_u32(table + 4u));
        if (second_line)
        {
            uint32 second = table + (mode > 1u ? 16u : 8u);
            gtmain_font_text_draw(ordering, (sint16)r_u16(second), y, r_u32(second + 4u));
        }
        y = (sint32)((uint32)y + 44u);
    }
    else y = (sint32)((uint32)y + (mode ? 56u : 28u));
    return (uint32)gtmain_race_countdown_text_draw(ordering, y);
}

void gtmain_race_times_draw(uint32 ordering)
{
    gtmain_race_current_time_draw(ordering, 16, 48);
    gtmain_race_lap_times_draw(ordering, 16, 70);
    gtmain_race_best_time_draw(ordering, 224, 16);
    gtmain_race_record_time_draw(ordering, 224, 38);
}

uint32 gtmain_race_record_label_draw(void)
{
    sint32 enabled = (sint16)r_u16(0x8009071Au);
    if (!enabled) return (uint32)enabled;
    /* TODO Integrate the saved 8002FDCC owner with its exact native record ABI */
    return gt_draft_8002FDCC((uint32)(sint32)(sint16)r_u16(0x80090718u), 0x800B39A4u, 0u, 0u, 0u, 0u, 0u, 0u);
}

uint32 gtmain_race_driver_hud_draw(uint32 ordering)
{
    /* TODO Integrate saved driver HUD callee bodies with their actual native ABIs */
    gtmain_race_instruments_draw(ordering, 238, 150, 0x800B6850u);
    gtmain_race_status_draw(ordering, 68, 0x800B6850u);
    gtmain_race_lap_counter_draw(ordering, 16, 16, 0x800B6850u);
    gtmain_race_position_hud_draw(ordering, 0x800B6850u);
    return gtmain_race_minimap_draw(ordering, 16, 144);
}

sint32 gtmain_race_countdown_text_draw(uint32 ordering, sint32 y)
{
    sint32 enabled = (sint16)r_u16(0x800B6312u);
    sint32 count = (sint16)r_u16(0x800B63DEu);
    uint32 sequence, color;
    if (!enabled) return enabled;
    if (count >= 4) return 0;
    gtmain_text_font(3);
    sequence = r_u32(0x800912CCu);
    color = r_u32(0x800912C4u + (sequence & 1u) * 4u);
    w_u32(0x800912CCu, sequence + 1u);
    gtmain_text_color(color);
    if (count)
    {
        uint32 words[1] = {(uint32)count};
        GTMainFormatArguments arguments = {words, 0u, 1u, 0u};
        return gtmain_text_format(ordering, 151, (sint16)y, 0x8008C830u, &arguments);
    }
    return gtmain_font_text_draw(ordering, (sint16)r_u16(0x800911F8u), y, r_u32(0x800911FCu));
}

static char *race_time_output;
static uint32 race_time_output_size, race_time_output_used;

static sint32 race_time_emit(sint8 character)
{
    if (!race_time_output || race_time_output_used + 1u >= race_time_output_size)
        gt_draft_failure("Native lap time output extent", 0x8006CA50u);
    race_time_output[race_time_output_used++] = (char)character;
    return 0;
}

static void race_format_buffer(char *output, uint32 capacity, uint32 format, const uint32 *words, uint32 count)
{
    GTMainFormatArguments arguments = {words, 0u, count, 0u};
    if (race_time_output) gt_draft_failure("Native HUD output lifetime", 0x8007C014u);
    race_time_output = output; race_time_output_size = capacity; race_time_output_used = 0u;
    if (gtmain_format_engine(race_time_emit, format, &arguments) < 0)
        gt_draft_failure("Native HUD formatting", 0x8007C014u);
    output[race_time_output_used] = 0;
    race_time_output = NULL;
}

static void race_time_string_native(char *output, uint32 capacity, uint32 milliseconds)
{
    uint32 words[4], seconds, minutes, hours, format;
    GTMainFormatArguments arguments = { words, 0u, 0u, 0u };
    if (!milliseconds || milliseconds == 0xFFFFFFFFu) {
        uint32 cursor = 0x8008E12Cu, used = 0u;
        uint8 character;
        do {
            character = r_u8(cursor++);
            if (used >= capacity) gt_draft_failure("Native lap time copy extent", 0x8006CA50u);
            output[used++] = (char)character;
        } while (character);
        return;
    }
    if (milliseconds > 35999999u) milliseconds = 35999999u;
    seconds = milliseconds / 1000u; minutes = seconds / 60u; hours = minutes / 60u;
    milliseconds -= seconds * 1000u; seconds -= minutes * 60u; minutes -= hours * 60u;
    if (!hours) {
        words[0] = minutes; words[1] = seconds; words[2] = milliseconds;
        arguments.count = 3u; format = 0x8008E138u;
    } else {
        words[0] = hours; words[1] = minutes; words[2] = seconds; words[3] = milliseconds / 100u;
        arguments.count = 4u; format = 0x8008E148u;
    }
    race_format_buffer(output, capacity, format, words, arguments.count);
}

static void race_time_draw_native(uint32 ordering, sint32 x, sint32 y, const char *text)
{
    sint8 glyph;
    gtmain_text_bind(ordering); gtmain_text_position((sint16)x, (sint16)y);
    glyph = (sint8)*text;
    while (glyph) {
        ++text; gtmain_text_glyph((uint8)glyph); glyph = (sint8)*text;
    }
}

/* GTMAIN.EXE 8002D8F8 */
void gtmain_race_current_time_draw(uint32 ordering, sint32 x, sint32 y)
{
    gtmain_text_font(1); gtmain_text_color(0x4080u);
    gtmain_font_text_draw(ordering, x, y, 0x800BDACBu);
    gtmain_text_draw(ordering, (sint16)x, (sint16)((uint32)y + 10u), 0x801A8FDCu);
}

/* GTMAIN.EXE 8002DA30 */
sint32 gtmain_race_lap_times_draw(uint32 ordering, sint32 x, sint32 y)
{
    sint32 count, lap, first, result, index;
    char text[40];
    gtmain_text_font(1); gtmain_text_color(0x4080u);
    gtmain_font_text_draw(ordering, x, y, 0x800BD915u);
    count = (sint16)r_u16(0x800B634Cu); lap = (sint8)r_u8(0x800B399Cu);
    if (!count) count = 100;
    first = lap < count ? lap - 2 : count - 3;
    result = count < 3;
    if (first < 0) first = 0;
    if (count >= 3) count = 3;
    for (index = 0; index < count; ) {
        uint32 time = r_u32(0x800BDB70u + ((uint32)first + (uint32)index) * 4u);
        y = (sint32)((uint32)y + 10u); ++index;
        race_time_string_native(text, sizeof(text), time);
        race_time_draw_native(ordering, x, y, text);
        result = index < count;
    }
    return result;
}

/* GTMAIN.EXE 8002DBE4 */
void gtmain_race_best_time_draw(uint32 ordering, sint32 x, sint32 y)
{
    gtmain_text_font(1); gtmain_text_color(0x4080u);
    gtmain_font_text_draw(ordering, x, y, 0x800BDABDu);
    gtmain_text_draw(ordering, (sint16)x, (sint16)((uint32)y + 10u), 0x801A8FECu);
}

/* GTMAIN.EXE 8002DCF8 */
void gtmain_race_record_time_draw(uint32 ordering, sint32 x, sint32 y)
{
    gtmain_text_font(1); gtmain_text_color(0x4080u);
    gtmain_font_text_draw(ordering, x, y, 0x800BD865u);
    gtmain_text_draw(ordering, (sint16)x, (sint16)((uint32)y + 10u), 0x801A8FFCu);
}

/* GTMAIN.EXE 8002C264 */
uint32 gtmain_race_gear_draw(uint32 ordering, sint32 x, sint32 y, const sint16 *bounds, uint32 car)
{
    uint32 gear = r_u8(car + 72u), mode = r_u8(car + 1731u);
    char text[8];
    text[0] = gear ? (char)(gear + 48u) : 'R'; text[1] = 0;
    gtmain_text_font(2); gtmain_text_color(mode == 1u ? 0x4080u : 0x2040u);
    race_time_draw_native(ordering, x, y, text);
    return race_bounds_native(ordering, bounds, 0u);
}

/* GTMAIN.EXE 8002FCD4 */
sint32 gtmain_race_lap_counter_draw(uint32 ordering, sint32 x, sint32 y, uint32 car)
{
    sint32 lap = (sint16)r_u16(car + 96u), total, configured;
    uint32 words[2];
    char text[32];
    if (lap <= 0) lap = 1;
    configured = (sint16)r_u16(0x800B634Cu); total = lap;
    if (configured > 0) total = configured;
    if (total < lap) lap = total;
    if (lap >= 100) lap = 99;
    if (total >= 100) total = 99;
    gtmain_text_color(0x4080u); gtmain_text_font(1);
    gtmain_font_text_draw(ordering, x, y, 0x800BD90Eu);
    words[0] = (uint32)lap; words[1] = (uint32)total;
    race_format_buffer(text, sizeof(text), 0x8008C80Cu, words, 2u);
    gtmain_text_font(2);
    return gtmain_font_string_draw_native(0x800C0070u, ordering, x, (sint32)((uint32)y + 9u), text);
}

/* GTMAIN.EXE 8002FBA8 */
uint32 gtmain_race_position_draw(uint32 ordering, sint32 x, sint32 y, uint32 corners, uint32 car)
{
    sint32 position = (sint8)r_u8(car + 1737u);
    uint32 suffix;
    char text[8];
    if (position) --position;
    suffix = r_u32(0x80091190u + (uint32)position * 4u);
    text[0] = (char)((uint32)position + 49u); text[1] = 0;
    gtmain_text_color(0x4080u); gtmain_text_font(3);
    gtmain_font_string_draw_native(0x800C0070u, ordering, (sint32)((uint32)x + 3u), y, text);
    gtmain_text_font(2);
    gtmain_font_text_draw(ordering, (sint32)((uint32)x + 16u), (sint32)((uint32)y + 7u), suffix + 1u);
    return gtmain_packet_rectangle(ordering, corners, 0u);
}

/* GTMAIN.EXE 8002FC74 */
uint32 gtmain_race_position_hud_draw(uint32 ordering, uint32 car)
{
    uint32 mode = r_u8(0x800CB7F5u);
    if (mode == 1u) return 1u;
    if (mode == 3u && r_u8(0x800CB7F6u) == 2u) return 2u;
    return gtmain_race_position_draw(ordering, 64, 16, 0x800912ACu, car);
}

/* GTMAIN.EXE 800301F4 */
uint32 gtmain_race_minimap_draw(uint32 ordering, sint32 x, sint32 y)
{
    uint32 disabled = r_u8(0x800CB75Cu);
    sint16 boxes[6][8];
    uint8 present[6] = {0};
    sint32 initial, limit, index, active = 0;
    uint32 left;
    if (disabled) return disabled;
    left = (uint32)x - 16u;
    initial = (sint16)r_u16(0x800BC1A6u);
    if (initial > 0) {
        limit = (sint16)r_u16(0x800BC1A6u); index = 0;
        do {
            uint32 car = 0x800B6850u + (uint32)index * 3380u;
            /* TODO Resolve original local output extent for more than six records */
            if (index >= 6) gt_draft_failure("Native minimap record extent", 0x800301F4u);
            if (r_u32(car + 12u) & 1u) {
                sint32 px = (sint16)r_u16(car + 2230u), pz = (sint16)r_u16(car + 2238u);
                uint32 mx = left + (uint32)((px * 163) >> 12) + 48u;
                uint32 my = (uint32)y - ((uint32)((pz * 163) >> 12) - 48u);
                ++active;
                boxes[index][0] = (sint16)(mx - 2u); boxes[index][1] = (sint16)(my - 2u);
                boxes[index][2] = (sint16)(mx + 1u); boxes[index][3] = (sint16)(my + 1u);
                boxes[index][4] = (sint16)(mx - 1u); boxes[index][5] = (sint16)(my - 1u);
                boxes[index][6] = (sint16)mx; boxes[index][7] = (sint16)my;
                present[index] = 1u;
            }
            ++index;
        } while (index < limit);
    }
    /* TODO Original sparse flags leave output records unreadable; do not fabricate their contents */
    if (!present[0]) gt_draft_failure("Original minimap first record uninitialized", 0x80030338u);
    for (index = 1; index < active; ++index)
        if (!present[index]) gt_draft_failure("Original minimap sparse record uninitialized", 0x80030370u);
    race_bounds_native(ordering, boxes[0] + 4, 255u);
    race_bounds_native(ordering, boxes[0], 0u);
    for (index = 1; index < active; ++index) race_bounds_native(ordering, boxes[index] + 4, 65280u);
    for (index = 1; index < active; ++index) race_bounds_native(ordering, boxes[index], 0u);
    gtmain_packet_sprite(ordering, (left & 65535u) | (((uint32)y + 16u) << 16), 0x181A1080u, 0x00500060u, 0x00808080u);
    return gtmain_packet_page(ordering, 518u);
}
