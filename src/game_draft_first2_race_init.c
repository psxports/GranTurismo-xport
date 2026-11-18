#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);

/* TODO Resolve the remaining image-qualified game call boundaries */
static uint32 race_call(uint32 address, uint32 first, uint32 second, uint32 third,
    uint32 fourth, uint32 fifth)
{
    if (address == 0x8001E988u) return gtmain_race_driver_configurations(first, second, third, fourth);
    if (address == 0x8002F998u) return gtmain_race_header_update(first);
    if (address == 0x8002A85Cu) return gtmain_race_buffers_bind();
    if (address == 0x8002A8B4u) return gtmain_race_scene_nodes_bind();
    if (address == 0x8002AE40u) return gtmain_race_configuration_snapshot();
    if (address == 0x80029CA4u) return gtmain_race_message_tables_prepare();
    if (address == 0x8002ADDCu) return gtmain_race_resources_load();
    if (address == 0x80028AB4u) return (uint32)gtmain_race_pad_contexts_reset();
    if (address == 0x8002A500u) return gt_draft_8002A500(0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (address == 0x80025C18u) return gt_draft_80025C18();
    if (address == 0x80025C5Cu) return gtmain_race_results_commit();
    if (address == 0x8002AD7Cu) return (uint32)gtmain_race_audio_stop_wait();
    if (address == 0x8002ADA4u) { gtmain_race_rumble_clear(); return 1u; }
    if (address == 0x800205D0u) return gtmain_race_session_save();
    if (address == 0x8002B078u) return gtmain_race_result_snapshot_store();
    return gt_draft_unresolved_call(address, first, second, third, fourth, fifth, 0u, 0u, 0u);
}
#define RACE_CALL0(address) race_call(address, 0u, 0u, 0u, 0u, 0u)
#define RACE_CALL1(address, first) race_call(address, first, 0u, 0u, 0u, 0u)

static uint32 race_stack_context_address(void)
{
    uint32 launch = r_u32(0x8008F1D8u), alternate = r_u32(0x800B1AA4u);
    uint32 top = alternate ? alternate : launch;
    uint32 reserve = r_u32(0x800B1B4Cu);
    uint32 heap = r_u32(0x800B1AACu), bytes = r_u32(0x800B1AA8u);
    uint32 extent = bytes < 16u ? 16u : bytes, address;
    /* Session context stays below the separate loading buffer workspace */
    if (top < 0x80014200u || top > 0x80200000u || (top & 7u))
        gt_draft_failure("GTMAIN race stack range", 0x8002BBD4u);
    address = top - 0x4200u;
    if (address < 0x801FB724u || heap < 0x80010000u || heap > address || extent > address - heap ||
        (!alternate && (reserve < 0x4200u || address < launch - reserve)))
        gt_draft_failure("GTMAIN race stack overlap", 0x8002BBD4u);
    return address;
}

uint8 *gtmain_race_context_get(void)
{
    uint32 address = r_u32(0x800B3990u);
    if (!address) gt_draft_failure("GTMAIN race context unavailable", 0x800B3990u);
    return (uint8 *)psx_addr(address, 40u);
}

static sint32 race_context_run(uint8 *context)
{
    return gtmain_frame_run(context);
}

static uint32 race_table_load(uint32 resource, uint32 *size, uint32 data, uint32 capacity)
{
    GTMainArchiveTable table = gtmain_archive_table_load(resource, size,
        (uint8 *)psx_addr(data, capacity), data, capacity);
    return table.guest_address;
}

static uint32 race_resource_load(uint32 resource, const uint32 *descriptor, uint32 size,
    uint32 mode, uint32 destination, uint32 flag)
{
    return gtmain_archive_section_load(resource, (const uint8 *)descriptor,
        size, mode, destination, flag);
}

static void race_cars_decode(uint32 cars, sint32 count, const uint32 *destinations, uint32 data)
{
    (void)gtmain_car_records_decode(cars, count, destinations, 6u, data);
}

static void race_palette_decode(uint8 *output, const uint8 *input, uint32 palette, uint32 masks)
{
    gtmain_car_palette_tint(output, input, palette, masks);
}

static sint32 race_native_name_lookup(const char *name)
{
    sint32 count = gtmain_course_count(), index;
    for (index = 0; index < count; ++index) {
        uint32 candidate = gtmain_course_name((uint32)index);
        if (!strcmp(name, (const char *)psx_addr(candidate, 1u))) return index;
    }
    return -1;
}

static void race_native_string_copy(char *output, uint32 source)
{
    uint32 offset = source & 3u, cursor = source - offset;
    uint32 word = r_u32(cursor);
    for (;;) {
        uint8 value = (uint8)(word >> (8u * offset));
        *output++ = (char)value;
        if (!value) return;
        if (++offset == 4u) { offset = 0u; cursor += 4u; word = r_u32(cursor); }
    }
}

/* GTMAIN.EXE 80022164 */
/* Original: GTMAIN.EXE:sub_80022164. */ void gtmain_race_cars_invalidate(void) { w_u16(0x8008F3EEu, 0u); }

/* GTMAIN.EXE 80024D38 */
/* Original: GTMAIN.EXE:sub_80024D38. */ sint32 gtmain_race_track_invalidate(void)
{
    w_u32(0x8008F728u, 0xFFFFFFFFu);
    return -1;
}

/* GTMAIN.EXE 8001EFAC */
/* Original: GTMAIN.EXE:sub_8001EFAC. */ uint32 gtmain_race_config_reset(void)
{
    memset(psx_addr(0x800B6650u, 12u), 0, 12u);
    return 0x800B6650u;
}

/* GTMAIN.EXE 8002CE4C */
/* Original: GTMAIN.EXE:sub_8002CE4C. */ void gtmain_race_text_bind(uint32 text)
{
    w_u32(0x8008FBF4u, text ? text : 0x8008FBECu);
}

/* GTMAIN.EXE 80031DCC */
/* Original: GTMAIN.EXE:sub_80031DCC. */ uint32 gtmain_race_render_config(uint32 mode)
{
    return gtmain_sound_bank_load_original(0x800B49ECu, 0x1010u, 0x801BB70Cu, mode);
}

/* GTMAIN.EXE 800274EC */
/* Original: GTMAIN.EXE:sub_800274EC. */ sint32 gtmain_race_track_find(uint32 names, uint32 name)
{
    uint32 index = 0u;
    if (!r_u32(names)) return -1;
    do {
        if (!gtmain_string_compare(r_u32(names + 4u * index), name)) return (sint32)index;
        ++index;
    } while (r_u32(names + 4u * index));
    return -1;
}

/* GTMAIN.EXE 8002756C */
/* Original: GTMAIN.EXE:sub_8002756C. */ sint32 gtmain_race_track_index(uint32 track)
{
    uint32 name = gtmain_course_name(track);
    if (!strncmp(psx_addr(name, 7u), psx_addr(0x8008BC98u, 7u), 7u)) name += 7u;
    return gtmain_race_track_find(0x80091A44u, name);
}

/* GTMAIN.EXE 800275F0 */
/* Original: GTMAIN.EXE:sub_800275F0. */ sint32 gtmain_race_arcade_track(uint32 track)
{
    char name[64];
    uint32 cursor = 0u, suffix;
    race_native_string_copy(name, 0x8008BC98u);
    suffix = gtmain_course_name(track);
    while (name[cursor]) ++cursor;
    race_native_string_copy(name + cursor, suffix);
    return race_native_name_lookup(name);
}

/* GTMAIN.EXE 80024D48 */
/* Original: GTMAIN.EXE:sub_80024D48. */ sint32 gtmain_race_track_load(uint32 track)
{
    uint32 previous = r_u32(0x8008F728u), resource, table, size, descriptor[10];
    uint32 first, second, third, fourth, record, image, workspace_bytes;
    GT_LOCAL_STACK_FRAME workspace;
    if (previous == track) return (sint32)previous;
    w_u32(0x8008F728u, track);
    gtmain_race_cars_invalidate();
    first = gtmain_course_name(track);
    second = gtmain_course_resource_name(track);
    printf("Loading: %d(%s) - %s\n", (sint32)track,
        (const char *)psx_addr(first, 1u), (const char *)psx_addr(second, 1u));
    resource = gtmain_course_file_environment();
    table = race_table_load(resource, &size, 0x801BB70Cu, 4096u);
    first = r_u32(table); second = r_u32(table + 4u);
    third = r_u32(table + 8u); fourth = r_u32(table + 12u);
    descriptor[0] = first; descriptor[1] = second; descriptor[2] = third; descriptor[3] = fourth;
    record = table + 24u * track;
    first = r_u32(record + 28u); second = r_u32(record + 32u); third = r_u32(record + 36u);
    descriptor[4] = first; descriptor[5] = second; descriptor[6] = third;
    first = r_u32(record + 16u); second = r_u32(record + 20u); third = r_u32(record + 24u);
    descriptor[7] = first; descriptor[8] = second; descriptor[9] = third;
    image = race_resource_load(resource, descriptor, size, 1u, 0x800D6FDCu, 0u);
    workspace_bytes = (gtmain_texture_workspace_size(image) + 14u) & ~7u;
    workspace = gt_local_stack_push(workspace_bytes, 0x80024D48u);
    gtmain_texture_workspace_publish(workspace.address);
    gtmain_texture_records_load(image);
    image = race_resource_load(resource, descriptor, size, 0u, 0x800D6FDCu, 0u);
    gtmain_course_resource_bind(image);
    first = gtmain_race_ai_routes_relocate(r_u32(0x800BC1ACu));
    gt_local_stack_pop(workspace);
    return (sint32)first;
}

/* GTMAIN.EXE 800221E8 */
/* Original: GTMAIN.EXE:sub_800221E8. */ sint32 gtmain_race_cars_load(void)
{
    uint32 count = r_u8(0x800B6394u), mode = r_u8(0x800CB7F5u), destinations[6], index;
    w_u16(0x800B6666u, 6u);
    w_u16(0x800BC1A6u, (uint16)count);
    if (mode == 1u && r_u8(0x800CDF6Au))
        w_u16(0x800BC1A6u, (uint16)(count + 1u));
    if (r_u16(0x8008F3EEu)) { gtmain_race_saved_flag(); return 0; }
    w_u16(0x8008F3EEu, 1u);
    w_u16(0x8008F3F0u, 0u);
    gtmain_roster_initialize();
    for (index = 0u; index < 6u; ++index) destinations[index] = 0x80184FDCu + 24576u * index;
    gtmain_race_course_car_filter();
    gtmain_car_archive_load(0x801BB70Cu, 20480u);
    race_cars_decode(0x800B6670u, (sint16)r_u16(0x800BC1A6u), destinations, 0x801C070Cu);
    gtmain_race_saved_flag();
    return 1;
}

/* GTMAIN.EXE 80022468 */
/* Original: GTMAIN.EXE:sub_80022468. */ sint32 gtmain_race_car_palettes(uint32 track)
{
    sint32 value = (sint16)r_u16(0x8008F3F0u), index = 0;
    uint32 palettes;
    uint8 input[512], output[512];
    PSX_RECT rectangle;
    if (value) return value;
    w_u16(0x8008F3F0u, 1u);
    palettes = gtmain_course_entry(track);
    value = (sint32)palettes;
    while (index < (sint16)r_u16(0x800BC1A6u)) {
        uint32 car = 0x800B667Cu + 76u * (uint32)index;
        uint32 location = r_u16(car - 8u), row = location >> 6u;
        rectangle.x = (sint16)((location & 63u) << 4u);
        rectangle.y = (sint16)row; rectangle.w = 64; rectangle.h = 4;
        StoreImage(&rectangle, (uint32 *)input); DrawSync(0);
        race_palette_decode(output, input, palettes + 12u, car);
        rectangle.y = (sint16)row; LoadImagePSX(&rectangle, (uint32 *)output); DrawSync(0);
        race_palette_decode(output, input, palettes + 8u, car);
        rectangle.y = (sint16)(row + 4u); LoadImagePSX(&rectangle, (uint32 *)output); DrawSync(0);
        race_palette_decode(output, input, palettes + 4u, car);
        rectangle.y = (sint16)(row + 8u); LoadImagePSX(&rectangle, (uint32 *)output); DrawSync(0);
        ++index;
        value = index < (sint16)r_u16(0x800BC1A6u);
    }
    return value;
}

/* GTMAIN.EXE 80027430 */
/* Original: GTMAIN.EXE:sub_80027430. */ sint32 gtmain_race_session_prepare(void)
{
    sint32 result;
    w_u8(0x800B63CAu, 0u); w_u32(0x800B630Cu, 0u);
    result = gtmain_race_session_loop();
    if (result == 2) { w_u32(0x800B630Cu, 1u); gtmain_race_session_loop(); }
    return result;
}

/* GTMAIN.EXE 8002BBD4 */
/* Original: GTMAIN.EXE:sub_8002BBD4. */ sint32 gtmain_race_session_loop(void)
{
    uint32 context_address = race_stack_context_address();
    uint8 *context = (uint8 *)psx_addr(context_address, 40u);
    uint32 restarted = 0u, mode, branch;
    sint32 value, selection;
restart:
    w_u32(0x800B3990u, context_address);
    memset(context, 0, 40u);
    {
        uint8 flag = r_u8(0x8008FBE3u);
        w_u32(context_address + 16u, 0x8002BA60u);
        w_u32(context_address + 4u, 0x8002B5A4u);
        w_u32(context_address + 8u, 0x8002B328u);
        w_u32(context_address + 12u, 0x8002B44Cu);
        w_u32(context_address + 32u, 0x8002B5D8u);
        w_u8(context_address + 36u, flag);
    }
    RACE_CALL0(0x8002A85Cu); RACE_CALL0(0x8002A8B4u); RACE_CALL0(0x8002AE40u);
    RACE_CALL0(0x80029CA4u); RACE_CALL0(0x8002ADDCu); gtmain_race_updates_reset();
    gtmain_race_cars_initialize(); gt_draft_80029DE4(); gt_draft_8002AC8C();
    RACE_CALL0(0x80028AB4u); gtmain_race_pause_menu_measure(); gt_draft_8002F960(); gtmain_race_overlay_textures_load();
    mode = r_u8(0x800CB7F5u);
    if (mode == 3u) { branch = r_u8(0x800CB7F6u); if (branch == 2u) goto select; }
    else if ((mode == 1u || mode == 5u) && !restarted && !r_u32(0x800B630Cu)) {
        branch = r_u8(0x800CB7F4u); if (branch == 2u) goto select;
    }
frame:
    gt_draft_8002AD3C(); RACE_CALL0(0x8002A85Cu); RACE_CALL0(0x8002A8B4u);
    gtmain_draw_background(0u, 0u, 0u, 0u);
    gtmain_frame_enabled(r_u32(0x800B6318u)); w_u8(0x800B63C0u, 0u);
    gt_draft_8002A7EC(); value = race_context_run(context); gt_draft_8002A7EC();
    RACE_CALL0(0x8002AD7Cu); RACE_CALL0(0x8002ADA4u);
    if (value != 5) goto done;
select:
    w_u8(0x800B63D8u, 0u); restarted = 1u;
    gtmain_race_track_invalidate(); RACE_CALL1(0x8006C144u, 0x800D6FDCu);
    selection = (sint32)RACE_CALL1(0x8003A1A0u, (uint32)(sint32)(sint8)r_u8(0x800B638Cu));
    if (selection < 0) { value = selection == -1 ? 7 : 4; goto done; }
    if (selection == 1) { value = 3; goto done; }
    if (selection > 2) { value = 4; goto done; }
    RACE_CALL0(0x8001ECA8u); gtmain_race_track_load(r_u8(0x800B63BCu));
    RACE_CALL0(0x80031F18u); gtmain_race_selected_vehicle_initialize(); gtmain_race_updates_reset(); gtmain_race_overlay_textures_load();
    mode = r_u8(0x800CB7F5u);
    if (mode == 1u || mode == 5u) { RACE_CALL0(0x800205D0u); goto restart; }
    goto frame;
done:
    if (value == 7) RACE_CALL0(0x80022FE8u);
    RACE_CALL0(0x800205D0u); RACE_CALL0(0x8002B078u);
    return value;
}

static uint32 session_missing0(uint32 address)
{
    /* TODO Bind the named unselected no-argument session service */
    gt_draft_failure("GTMAIN no-argument session service", address);
}

static uint32 session_missing1(uint32 address, uint32 argument)
{
    /* TODO Bind the named unselected one-argument session service */
    gt_draft_failure("GTMAIN one-argument session service", address);
}

static sint32 session_panel_run(uint32 address)
{
    if (address == 0x80037E00u) return gtmain_menu_quick_end_mode3(1u);
    return (sint32)session_missing1(address, 1u);
}

/* GTMAIN.EXE 80027640 */
/* Original: GTMAIN.EXE:sub_80027640. */ sint32 gtmain_race_session_enter(void)
{
    uint32 first, second, mode;
    sint32 value, branch;
    gtmain_race_render_config(0u);
    first = r_u32(0x800CB76Cu); second = r_u32(0x800CB770u);
    w_u32(0x800CBC06u, first); w_u32(0x800CBC0Au, second);
    w_u8(0x800CBC0Eu, r_u8(0x800CB774u));
    first = r_u8(0x800CB7F8u); mode = r_u8(0x800CB7F5u);
    w_u8(0x800B63BCu, (uint8)first);
    if (mode == 0u || mode == 1u) {
        w_u8(0x800B63BCu, (uint8)gtmain_race_arcade_track(r_u8(0x800B63BCu)));
        w_u8(0x800B63BDu, (uint8)gtmain_race_track_index(r_u8(0x800B63BCu)));
        if (!mode) {
            w_u8(0x800B6394u, 6u); gtmain_race_arcade_roster();
        } else {
            w_u16(0x800B634Cu, 2u); w_u8(0x800B6394u, 1u);
            session_missing0(0x80022018u); gtmain_race_config_reset();
        }
        gtmain_race_driver_configurations(0x800D6FDCu, 1u, mode, 3u);
        gtmain_race_track_invalidate(); gtmain_race_header_update(0u);
        if (mode == 1u) {
            gtmain_race_text_bind(0x8008F7A8u); gtmain_race_track_load(r_u8(0x800B63BCu));
            gtmain_race_cars_load(); gtmain_race_car_palettes(r_u8(0x800B63BCu));
            if ((sint32)session_missing0(0x80037E80u) == -1) return -1;
        }
    } else if (mode == 2u) {
        w_u8(0x800B63D0u, 0u); w_u8(0x800B63D3u, 1u); session_missing0(0x8002748Cu);
        first = r_u8(0x800CB76Bu); w_u8(0x800B6394u, 2u);
        w_u16(0x800B634Cu, r_u8(0x8008F7ACu + first));
        gtmain_race_config_reset(); gtmain_race_driver_configurations(0x800D6FDCu, 1u, 2u, 3u);
        gtmain_race_track_invalidate(); gtmain_race_header_update(0u);
    } else if (mode == 6u) {
        uint32 course = r_u8(0x800B63BCu);
        w_u32(0x800B6318u, 1u); w_u8(0x800CB7F5u, 1u);
        w_u8(0x800B63BDu, (uint8)session_missing1(0x800275C0u, course));
        w_u16(0x800B634Cu, 2u); session_missing0(0x80022018u); w_u8(0x800B6394u, 1u);
        gtmain_race_config_reset(); gtmain_race_driver_configurations(0x800D6FDCu, 1u, 1u, 3u);
        gtmain_race_track_invalidate(); gtmain_race_header_update(0u);
    } else return 1;
setup:
    if (!mode) {
        w_u8(0x800B63D0u, 5u); gtmain_race_cars_invalidate(); w_u16(0x800B634Cu, 2u);
        w_u8(0x800B6394u, 6u); w_u8(0x800CB7F6u, 1u); gt_draft_80025C18();
        if (gtmain_race_session_prepare() != 4) {
            value = (sint32)gtmain_race_results_commit(); if (value) return value;
        }
    } else if (mode == 1u) {
        session_missing1(0x80022120u, 1u); w_u8(0x800B63D0u, 0u); session_missing1(0x800260B8u, 0u);
        if (gtmain_race_session_prepare() != 4) session_missing1(0x80026138u, 0u);
    } else if (mode == 2u) {
        session_missing0(0x80026430u); if (gtmain_race_session_prepare() != 4) session_missing0(0x80026448u);
    } else {
        w_u8(0x800B63D0u, 0u); session_missing1(0x800260B8u, 1u);
        if (gtmain_race_session_prepare() != 4) session_missing1(0x80026138u, 1u);
    }
    for (;;) {
        uint32 address = !mode ? 0x80037E00u : mode == 1u ? 0x80037E40u : mode == 2u ? 0x80037F40u : 0x80037DC0u;
        branch = (sint32)session_panel_run(address);
        if (branch == -1) return -1;
        if (branch) goto setup;
        w_u32(0x800B630Cu, 1u); gtmain_race_session_loop();
    }
}

/* GTMAIN.EXE 80028598 */
/* Original: GTMAIN.EXE:sub_80028598. */ sint32 gtmain_race_session_restore(void)
{
    uint32 source, destination, first, second, third, fourth, mode;
    w_u32(0x800BDD04u, r_u32(0x800CB8E0u));
    gtmain_memory_copy(0x800BDB70u, 0x800CB8E4u, 400u);
    w_u8(0x800B63BCu, r_u8(0x800CBA74u)); gtmain_memory_copy(0x800B6650u, 0x800CBA76u, 10u);
    first = r_u16(0x800CBA80u); second = r_u16(0x800CBA82u);
    w_u16(0x800B63C4u, (uint16)first); w_u16(0x800B63C6u, (uint16)second);
    gtmain_memory_copy(0x800BB790u, 0x800CBA84u, 96u); gtmain_memory_copy(0x800BB7F0u, 0x800CBAE4u, 96u);
    source = 0x800CBB44u; destination = 0x800CB7F8u;
    do {
        first = r_u32(source); second = r_u32(source + 4u); third = r_u32(source + 8u); fourth = r_u32(source + 12u);
        w_u32(destination, first); w_u32(destination + 4u, second); w_u32(destination + 8u, third); w_u32(destination + 12u, fourth);
        source += 16u; destination += 16u;
    } while (source != 0x800CBC04u);
    w_u16(destination, r_u16(source));
    first = r_u8(0x800CBC0Fu); second = r_u8(0x800CBC11u);
    w_u8(0x800B63D3u, 1u); w_u32(0x800B6318u, first); w_u8(0x800CB7F5u, (uint8)second);
    first = r_u8(0x800CBC10u); second = r_u8(0x800CBC12u); third = r_u8(0x800CBC14u); fourth = r_u8(0x800CBC16u);
    mode = r_u8(0x800CB7F5u);
    source = r_u8(0x800CBC17u);
    w_u8(0x800CB7F4u, (uint8)first); w_u8(0x800CB7F6u, (uint8)second);
    w_u8(0x800B63D0u, (uint8)third); w_u8(0x800B6394u, (uint8)fourth); w_u16(0x800B634Cu, (uint16)source);
    gtmain_race_render_config(mode == 4u); gtmain_race_header_update(0u);
    if (r_u8(0x800CB7F4u) == 1u) w_u8(0x800B63BDu, (uint8)gtmain_race_track_index(r_u8(0x800B63BCu)));
    mode = r_u8(0x800CB7F5u);
    if (mode == 4u) gtmain_course_route_caption_update();
    else if (mode == 5u) session_missing1(0x800239E0u, r_u8(0x800CB7F6u));
    mode = r_u8(0x800CB7F5u);
    source = r_u8(0x800CBC15u);
    gtmain_race_driver_configurations(0x800D6FDCu, 3u, mode, source);
    w_u32(0x800B630Cu, 1u);
    return gtmain_race_session_loop();
}
