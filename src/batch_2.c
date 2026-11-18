#include "game_gtend_registry.h"
#include "game_native_policy.h"
#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"
#include <stdio.h>

uint32 gtend_8001d310(void)
{
    uint32 stack, reservation, end, size;
    FUNCTION_MARKER(0x8001D310, "GTEND.EXE");
    stack = r_u32(0x80025608);
    reservation = r_u32(0x80025650);
    end = ((stack - reservation) & ~15) | 0x80000000;
    size = end - 0x8005D870;
    if ((sint32)size < 0) size = 0;
    return gtend_heap_initialize(0x8005D870, size);
}

uint32 gtend_80015e7c(uint32 node)
{
    uint32 link, next, previous = 0, priority;
    FUNCTION_MARKER(0x80015E7C, "GTEND.EXE");
    gt_native_critical_enter();
    link = 0x80040548;
    next = r_u32(link);
    priority = r_u32(node + 12);
    while (next) {
        if (priority < r_u32(next + 12)) break;
        previous = next;
        next = r_u32(next + 4);
    }
    w_u32(node, previous);
    w_u32(node + 4, next);
    if (previous) {
        link = previous + 4;
        w_u32(previous + 4, node);
    }
    if (next) w_u32(next, node);
    w_u32(link, node);
    return gtmain_bios_critical_exit_result();
}

uint32 gtend_callback_remove(uint32 node)
{
    uint32 previous, next, link;
    FUNCTION_MARKER(0x80016368, "GTEND.EXE");
    gt_native_critical_enter();
    previous = r_u32(node);
    next = r_u32(node + 4);
    link = previous ? previous + 4 : 0x80040548;
    w_u32(link, next);
    if (next) w_u32(next, previous);
    return gtmain_bios_critical_exit_result();
}

uint32 gtend_scene_timer_start(void)
{
    FUNCTION_MARKER(0x80010C6C, "GTEND.EXE");
    SetRCnt(0xF2000002, 0x1838, 0x1000);
    ResetRCnt(0xF2000002);
    return StartRCnt(0xF2000002);
}

uint32 gtend_scene_timer_event(void)
{
    uint32 count;
    FUNCTION_MARKER(0x80010C18, "GTEND.EXE");
    count = r_u32(0x80025670);
    w_u32(0x80025670, count + 1);
    gtend_ordering_submit_buffer(1);
    StopRCnt(0xF2000002, 0x80042358, 0x80042360);
    SetRCnt(0xF2000002, 0x1838, 0x2000);
    return 0;
}

uint32 gtend_scene_vblank(void)
{
    uint32 frames, pending, phase;
    FUNCTION_MARKER(0x80010CB0, "GTEND.EXE");
    frames = r_u32(0x80058A0C);
    pending = r_u32(0x8002566C);
    w_u32(0x80058A0C, frames + 1);
    phase = r_u32(0x80058A04);
    return gtend_scene_tick(phase, pending);
}

uint32 gtend_scene_tick(uint32 phase, uint32 pending)
{
    FUNCTION_MARKER(0x80010CD4, "GTEND.EXE");
    w_u32(0x80058A04, phase + 1);
    if (pending) {
        w_u32(0x8002566C, pending - 1);
        return gtend_scene_timer_start();
    }
    return pending - 1;
}

uint32 gtend_frame_callbacks(void)
{
    uint32 node, frames, elapsed;
    FUNCTION_MARKER(0x800164BC, "GTEND.EXE");
    node = r_u32(0x80040548);
    while (node) {
        uint32 callback = r_u32(node + 8);
        uint32 next = r_u32(node + 4);
        switch (callback) {
        case 0: break;
        case 0x800168F0: gtend_sound_tick_if_enabled(); break;
        case 0x80010CB0: gtend_scene_vblank(); break;
        default: gt_draft_failure("Unbound GTEND frame callback", callback);
        }
        node = next;
    }
    frames = r_u32(0x8005952C);
    elapsed = r_u32(0x80059530);
    ++frames;
    ++elapsed;
    w_u32(0x8005952C, frames);
    w_u32(0x80059530, elapsed);
    return frames;
}

static void gtend_native_vblank(void)
{
    gtend_frame_callbacks();
}

uint32 gtend_80015e14(uint32 mode)
{
    FUNCTION_MARKER(0x80015E14, "GTEND.EXE");
    if (r_u32(0x8004054C)) return 0xFFFFFFFF;
    w_u32(0x8004054C, 1);
    w_u32(0x8005952C, 0);
    w_u32(0x80059530, 0);
    gtend_80019eb0(mode);
    w_u32(0x80040548, 0);
    VSyncCallback(gtend_native_vblank);
    return 0;
}

uint32 gtend_80015ff0(void)
{
    FUNCTION_MARKER(0x80015FF0, "GTEND.EXE");
    if (!r_u32(0x8004054C)) return 0xFFFFFFFF;
    w_u32(0x8004054C, 0);
    VSyncCallback(NULL);
    return 0;
}

uint32 gtmain_car_selection_copy(uint32 index, uint32 source, uint32 archive)
{
    FUNCTION_MARKER(0x8001E810, "GTMAIN.EXE");
    gtmain_memory_copy(0x800BB790 + index * 96, source, 96);
    return gtmain_car_selection_bind(index, archive);
}

uint32 gtmain_parts_table(uint32 category)
{
    uint32 offset;
    FUNCTION_MARKER(0x8001EB94, "GTMAIN.EXE");
    offset = r_u32(0x800D6FEC + category * 12);
    return offset + 0x800D6FF4;
}

uint32 gtmain_parts_car_name(uint32 car)
{
    uint32 table;
    sint32 index;
    FUNCTION_MARKER(0x8001EB3C, "GTMAIN.EXE");
    table = gtmain_parts_table(13);
    index = gtmain_car_find(table + car * 424 - 424);
    return gtmain_resource_name(index);
}

uint32 gtmain_parts_count(uint32 category)
{
    uint32 offset;
    FUNCTION_MARKER(0x8001EBC4, "GTMAIN.EXE");
    offset = r_u32(0x800D6FEC + category * 12);
    return gtmain_parts_count_at(offset);
}

uint32 gtmain_parts_count_at(uint32 offset)
{
    FUNCTION_MARKER(0x8001EBDC, "GTMAIN.EXE");
    return r_u16(0x800D6FEA + offset);
}

uint32 gtmain_parts_record(uint32 category, uint32 selection, uint32 archive)
{
    uint32 table, size;
    FUNCTION_MARKER(0x8006C020, "GTMAIN.EXE");
    table = archive + r_u32(archive + category * 12 + 16);
    size = r_u32(table + 20);
    return table + (selection - 1) * size + 24;
}

uint32 gtmain_parts_selected_record(uint32 category, uint32 selection, uint32 archive)
{
    uint32 record;
    FUNCTION_MARKER(0x8006C0F4, "GTMAIN.EXE");
    record = gtmain_parts_record(category, selection, archive);
    return record + r_u8(0x800941A5 + category);
}

uint32 gtmain_parts_label(uint32 category, uint32 selection, uint32 destination)
{
    uint32 table, records, packed, strings, text;
    FUNCTION_MARKER(0x8001EBF4, "GTMAIN.EXE");
    table = r_u32(0x800D6FEC + category * 12) + 0x800D6FDC;
    records = gtmain_parts_selected_record(category, selection, 0x800D6FDC);
    packed = r_u32(records);
    strings = r_u32(table + 16);
    text = r_u32(strings + (packed >> 16) * 4 + 4);
    text = gtmain_resource_records_end(text, (uint16)packed);
    gtmain_string_copy(destination, text + 1);
    packed = r_u32(records + 4);
    strings = r_u32(table + 16);
    text = r_u32(strings + (packed >> 16) * 4 + 4);
    text = gtmain_resource_records_end(text, (uint16)packed);
    return gtmain_string_append(destination, text + 1);
}

uint32 gtmain_race_settings_reset(void)
{
    FUNCTION_MARKER(0x8001F900, "GTMAIN.EXE");
    return gtmain_memory_copy(0x800BC160, 0x80093C08, 69);
}

uint32 gtmain_race_opponent_restore(void)
{
    uint32 selection, value, result;
    FUNCTION_MARKER(0x8001FAE0, "GTMAIN.EXE");
    gtmain_memory_copy(0x800BB7F0, 0x800CDDD8, 96);
    selection = r_u16(0x800BB7F0);
    value = r_u16(0x800CDDD4);
    w_u8(0x800B66BF, 0);
    result = selection >> 12;
    w_u16(0x800B66BC, value);
    w_u8(0x800B66BE, result);
    return result;
}

uint32 gtmain_race_selector_set(sint32 selector)
{
    uint32 result;
    FUNCTION_MARKER(0x800204EC, "GTMAIN.EXE");
    result = gtmain_8004ef9c(0x800B6878, -1, -1, selector);
    w_u8(0x800BB7ED, selector);
    w_u8(0x800CBAE1, selector);
    return result;
}

uint32 gtmain_race_rank_adjust(void)
{
    uint32 rank, index;
    FUNCTION_MARKER(0x80020884, "GTMAIN.EXE");
    rank = r_u8(0x800B6660);
    if (!rank) return 0x800B6660;
    w_u8(0x800B6660, 0);
    for (index = 1; index < 6; ++index) {
        uint32 value = r_u8(0x800B6660 + index);
        if (value < rank) {
            value = r_u8(0x800B6660 + index);
            w_u8(0x800B6660 + index, value + 1);
        }
    }
    return 0;
}

uint32 gtmain_race_state_checkpoint(void)
{
    uint32 previous, count, index;
    FUNCTION_MARKER(0x80021334, "GTMAIN.EXE");
    previous = r_u32(0x8008F3E4);
    count = r_u16(0x800B6666);
    w_u32(0x8008F3E8, previous);
    if ((sint16)count <= 0) return count << 16;
    for (index = 0; index < count; ++index) {
        uint32 entry = 0x800B6850 + index * 3380;
        uint32 value = r_u32(entry + 4);
        w_u32(entry + 8, value);
    }
    return count * 2;
}

sint32 gtmain_race_rank_print(uint32 label)
{
    uint32 length = 0, index;
    FUNCTION_MARKER(0x80020684, "GTMAIN.EXE");
    while (r_u8(label + length)) ++length;
    printf((const char *)psx_addr(0x8008B980, 12),
        (const char *)psx_addr(label, length + 1));
    for (index = 0; index < 6; ++index)
        printf((const char *)psx_addr(0x8008B98C, 4), r_u8(0x800B6660 + index));
    return printf((const char *)psx_addr(0x8008B990, 2));
}

sint32 gtmain_race_ranks_reset(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800206E8, "GTMAIN.EXE");
    sub_8007BD58(0x800B6840, 0, 12);
    for (index = 0; index < 6; ++index) w_u8(0x800B6660 + index, index);
    return gtmain_race_points_print(0x8008B994);
}

sint32 gtmain_race_rank_reorder(void)
{
    uint8 configurations[2316], details[456], scores[12], positions[12];
    uint32 index, offset;
    FUNCTION_MARKER(0x800208F8, "GTMAIN.EXE");
    gtmain_race_rank_print(0x8008B9AC);
    gtmain_race_rank_adjust();
    gtmain_race_rank_print(0x8008B9B8);
    for (index = 0; index < sizeof(configurations); ++index)
        configurations[index] = r_u8(0x800BB850 + index);
    for (index = 0; index < sizeof(details); ++index)
        details[index] = r_u8(0x800B6670 + index);
    for (index = 0; index < sizeof(scores); ++index)
        scores[index] = r_u8(0x800B6840 + index);
    for (index = 0; index < sizeof(positions); ++index)
        positions[index] = r_u8(0x800B6650 + index);
    gtmain_race_points_print(0x8008B9C4);
    for (index = 0; index < 6; ++index) {
        uint32 rank = r_u8(0x800B6660 + index);
        uint32 destination = 0x800BB850 + rank * 386;
        const uint8 *source = configurations + index * 386;
        for (offset = 0; offset < 384; offset += 16) {
            uint32 words[4], word;
            for (word = 0; word < 4; ++word)
                words[word] = xport_load_le32(source + offset + word * 4);
            for (word = 0; word < 4; ++word) {
                uint32 address = destination + offset + word * 4;
                uint32 value = words[word];
                if ((index | rank) & 1) {
                    if (address & 3) {
                        w_masked_u32((address + 3) & ~3, value >> 16, 3);
                        w_masked_u32(address & ~3, value << 16, 12);
                    } else {
                        w_u32(address, value);
                        w_u32(address, value);
                    }
                } else {
                    w_u32(address, value);
                }
            }
        }
        w_u16(destination + 384, xport_load_le16(source + 384));
        source = details + index * 76;
        destination = 0x800B6670 + rank * 76;
        for (offset = 0; offset < 64; offset += 16) {
            uint32 a = xport_load_le32(source + offset);
            uint32 b = xport_load_le32(source + offset + 4);
            uint32 c = xport_load_le32(source + offset + 8);
            uint32 d = xport_load_le32(source + offset + 12);
            w_u32(destination + offset, a);
            w_u32(destination + offset + 4, b);
            w_u32(destination + offset + 8, c);
            w_u32(destination + offset + 12, d);
        }
        {
            uint32 a = xport_load_le32(source + 64);
            uint32 b = xport_load_le32(source + 68);
            uint32 c = xport_load_le32(source + 72);
            w_u32(destination + 64, a);
            w_u32(destination + 68, b);
            w_u32(destination + 72, c);
        }
        w_u16(0x800B6840 + rank * 2, xport_load_le16(scores + index * 2));
        if (index)
            w_u16(0x800B664E + rank * 2, xport_load_le16(positions + (index - 1) * 2));
    }
    return gtmain_race_points_print(0x8008B9CC);
}

uint32 gtmain_8004ef9c(uint32 context, sint32 first, sint32 second, sint32 third)
{
    FUNCTION_MARKER(0x8004EF9C, "GTMAIN.EXE");
    if (first != -1)
        gtmain_race_car_kind_set(context + 404, first);
    if (second != -1) {
        sint32 current = r_s8(context + 1689);
        if (second != current && current != 4 && second != 4)
            gtmain_race_car_mode_set(context, second);
    }
    if (third != -1)
        return gtmain_race_vehicle_variant_set(context, third);
    return 0xFFFFFFFF;
}

uint32 gtmain_race_points_rank_update(void)
{
    sint32 initial_count;
    uint32 result, node, rank = 0;
    FUNCTION_MARKER(0x800213B4, "GTMAIN.EXE");
    initial_count = r_s16(0x800BC1A6);
    w_u32(0x8008F3E8, 0);
    result = initial_count;
    if (initial_count > 0) {
        sint32 count = r_s16(0x800BC1A6);
        uint32 index = 0;
        do {
            uint32 entry = 0x800B6850 + index * 3380;
            uint32 placing = r_u16(entry + 36);
            uint32 next = r_u32(0x8008F3E8), previous = 0;
            sint32 award = r_s8(0x8008F3D0 + placing);
            sint32 points = r_u16(entry + 38) + award;
            uint16 score = points;
            w_u16(entry + 38, points);
            while (next) {
                if (score >= r_u16(next + 38))
                    break;
                previous = next;
                next = r_u32(next + 8);
            }
            w_u32(entry + 8, next);
            if (previous)
                w_u32(previous + 8, entry);
            else
                w_u32(0x8008F3E8, entry);
            ++index;
            result = index * 2;
        } while ((sint32)index < count);
    }
    node = r_u32(0x8008F3E8);
    while (node != 0x800B6850) {
        node = r_u32(node + 8);
        ++rank;
    }
    w_u8(0x800B634E, rank);
    return result;
}

uint32 gtmain_race_record_context_check(void)
{
    uint32 current, result;
    FUNCTION_MARKER(0x80022018, "GTMAIN.EXE");
    current = r_u16(0x800CB7F4);
    result = r_u16(0x800CDF64);
    if (current != result)
        goto clear_record;
    current = r_u8(0x800B63BC);
    result = r_u16(0x800CDDC8);
    if (current != result)
        goto clear_record;
    {
        sint32 race = r_s16(0x800B634C);
        uint8 saved_race = r_u8(0x800CDF6B);
        result = 1;
        if (race != saved_race)
            goto clear_record;
    }
    if (r_u32(0x800B6318) != 1)
        return result;
clear_record:
    w_u8(0x800CDF6A, 0);
    return result;
}

sint32 gtmain_race_driver_setup_if_needed(uint32 kind)
{
    sint32 state;
    FUNCTION_MARKER(0x80022120, "GTMAIN.EXE");
    state = r_s16(0x8008F3EE);
    if (state)
        return state;
    gtmain_race_driver_configurations(0x800D6FDC, kind, 1, 3);
    return gtmain_race_track_invalidate();
}

uint32 gtmain_race_record_update(uint32 total)
{
    sint32 enabled;
    uint32 previous, result;
    uint8 count;
    FUNCTION_MARKER(0x80022090, "GTMAIN.EXE");
    enabled = r_s8(0x800B63CA);
    if (!enabled)
        return enabled;
    previous = r_u32(0x800CDC34);
    count = r_u8(0x800CDF6A);
    result = total < previous;
    if (!result && count)
        return result;
    printf((const char *)psx_addr(0x8008B9F4, 41), total, previous, count);
    gtmain_memory_copy(0x800CDC34, 0x800CB8E0, 9044);
    result = gtmain_memory_copy(0x800BB9D2, 0x800BB850, 386);
    gtmain_race_cars_invalidate();
    return result;
}

uint32 gtmain_event_result_category(void)
{
    FUNCTION_MARKER(0x80022708, "GTMAIN.EXE");
    switch (r_u8(0x800CB7F8)) {
    case 0: return 9;
    case 1: return 10;
    case 2: return 11;
    case 3: case 19: case 20: return 12;
    case 4: case 6: return 6;
    case 5: case 21: return 7;
    case 7: case 8: case 9: return 4;
    case 15: return 8;
    case 16: case 17: case 18: return 5;
    default: return 3;
    }
}

uint32 gtmain_event_result_record(void)
{
    uint32 event, race, offset;
    FUNCTION_MARKER(0x8002278C, "GTMAIN.EXE");
    event = r_u8(0x800CB7F8);
    race = r_u8(0x800CB7F9);
    switch (event) {
    case 0: offset = 0x4080; break;
    case 1: offset = 0x417C; break;
    case 2: offset = 0x4278; break;
    case 3: offset = 0x43C8; break;
    case 4: offset = 0x45C0; break;
    case 5: offset = 0x4764; break;
    case 6: offset = 0x4B00; break;
    case 7: offset = 0x4908; break;
    case 8: offset = 0x4A04; break;
    case 9: offset = 0x4BFC; break;
    case 10: case 11: case 12: case 13: case 14:
        race = r_u8(0x800CB7F8);
        offset = 0x49B0;
        break;
    case 15: offset = 0x4E9C; break;
    case 16: offset = 0x4F98; break;
    case 17: offset = 0x513C; break;
    case 18: offset = 0x52E0; break;
    case 19: return 0x800CA0C8;
    case 20: return 0x800CA11C;
    case 21: return 0x800CA170;
    default: return 0;
    }
    return 0x800C4C44 + offset + race * 84;
}

uint32 gtmain_event_opponents_generate(void)
{
    uint32 selected, event, list;
    FUNCTION_MARKER(0x800229D0, "GTMAIN.EXE");
    selected = r_u16(0x800BB790);
    event = r_u8(0x800CB7F8);
    list = r_u32(0x8008F50C + event * 24);
    gtmain_roster_generate(list, 5, (selected & 4095) - 1, selected >> 12);
    /* The five-entry commit exits with a false loop condition in V0 */
    return 0;
}

uint32 gtmain_event_race_parameter(void)
{
    uint32 event, race, table;
    FUNCTION_MARKER(0x80022A24, "GTMAIN.EXE");
    event = r_u8(0x800CB7F8);
    race = r_u8(0x800CB7F9);
    table = r_u32(0x8008F508 + event * 24);
    return r_u32(table + race * 4);
}

uint32 gtmain_event_race_has_next(void)
{
    uint32 event, race, table;
    FUNCTION_MARKER(0x80022A64, "GTMAIN.EXE");
    event = r_u8(0x800CB7F8);
    race = r_u8(0x800CB7F9);
    table = r_u32(0x8008F500 + event * 24);
    return r_u32(table + race * 4 + 4) != 0;
}

uint32 gtmain_event_prize_award(uint32 mode)
{
    uint32 event, prizes, amount = 0, balance;
    sint32 rank;
    FUNCTION_MARKER(0x80022D34, "GTMAIN.EXE");
    event = r_u8(0x800CB7F8);
    rank = r_s8(0x800B63D2);
    prizes = r_u32(0x8008F504 + event * 24);
    if (mode)
        amount = r_u32(prizes + (uint32)rank * 4 + 4);
    else if (!rank)
        amount = r_u32(prizes);
    balance = r_u32(0x800C4C44) + amount;
    if ((sint32)balance > 1999999999)
        balance = 2000000000;
    w_u32(0x800C4C44, balance);
    return gtmain_80034f5c(mode, rank, amount, balance, 0);
}

uint32 gtmain_event_championship_award(void)
{
    uint32 amount = 0, flags = 0, event, table, prizes, balance, result;
    sint32 rank, best;
    FUNCTION_MARKER(0x80022DD4, "GTMAIN.EXE");
    if (r_s8(0x800B3914))
        rank = r_s8(0x800B63D2);
    else
        rank = r_s8(0x800B634E);
    event = r_u8(0x800CB7F8);
    table = 0x8008F500 + event * 24;
    prizes = r_u32(table + 4);
    if (!rank) {
        uint32 cars = r_u32(table + 16);
        amount = r_u32(prizes + 28);
        flags |= 1;
        if (cars) {
            uint32 count = 0;
            while (r_u32(cars + count * 4))
                ++count;
            if (count) {
                uint32 random = gtmain_timer_random();
                uint32 car = r_u32(cars + (random % count) * 4);
                if (gt_draft_8001EE20(car))
                    flags |= 2;
            }
        }
    }
    balance = r_u32(0x800C4C44) + amount;
    if ((sint32)balance > 1999999999) {
        amount = amount + 2000000000 - balance;
        balance = 2000000000;
    }
    w_u32(0x800C4C44, balance);
    if (amount)
        gtmain_80034f5c(1, rank, amount, balance, flags);
    event = r_u8(0x800CB7F8);
    best = r_u8(0x800C8CAC + event);
    if (!best || rank + 1 < best)
        best = rank + 1;
    event = r_u8(0x800CB7F8);
    w_u8(0x800C8CAC + event, best);
    result = r_u32(0x800C8CAC) & 0xFFFFFF;
    if (result == 0x010101) {
        result = 3;
        if (!rank && r_u8(0x800CB7F8) == 3) {
            result = 0x8008BAF0;
            w_u32(0x800B6390, result);
        }
    }
    return result;
}

sint32 gtmain_event_preview_models_load(void)
{
    sint32 first, second;
    uint32 variant, environment;
    uint32 destinations[2];
    FUNCTION_MARKER(0x800225F0, "GTMAIN.EXE");
    first = gtmain_car_find(0x8008BA38);
    variant = r_u8(0x800CB7F8);
    second = gtmain_car_find(r_u32(0x8008F3F4 + variant * 4));
    sub_8007BD58(0x800B6670, 0, 456);
    w_u8(0x800B66BE, 2);
    w_u8(0x800B670A, 1);
    w_u16(0x800B6670, first);
    w_u16(0x800B66BC, first);
    w_u16(0x800B6708, first);
    w_u16(0x800B6754, second);
    w_u8(0x800B6672, 0);
    w_u8(0x800B6756, 0);
    destinations[0] = 0x8018AFDC;
    destinations[1] = 0x80190FDC;
    environment = gtmain_car_file_environment();
    gtmain_car_config_bind(environment, 0x8008B902, 0x8008B90E, 2, 4);
    gtmain_course_car_filter(0);
    gtmain_car_archive_load(0x801BB70C, 20480);
    return gtmain_car_records_decode(0x800B6670, 4, destinations, 2, 0x801C070C);
}

uint32 gtmain_event_results_begin(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x80022FE8, "GTMAIN.EXE");
    mode = r_u8(0x800CB7F6);
    if (mode == 2)
        return 2;
    if (!mode) {
        uint32 delay = gtmain_event_race_parameter();
        gtmain_car_start_delays(delay);
        w_u16(0x800B63D6, 6);
    } else {
        sint32 flag = r_s8(0x800B3914);
        w_u16(0x800B63D6, flag ? 6 : 9);
    }
    w_u16(0x800B63DA, r_u32(0x800BF518) << 3);
    gtmain_race_rank_list_sort();
    gtmain_race_player_positions();
    gtmain_race_player_positions_latch();
    return gtmain_race_state_checkpoint();
}

uint32 gtmain_event_results_tick(void)
{
    sint32 timer, state;
    uint32 frame_step, ready = 0, result = 0;
    FUNCTION_MARKER(0x80023094, "GTMAIN.EXE");
    timer = r_s16(0x800B63DA);
    frame_step = r_u32(0x800BF518);
    if ((timer >> 3) >= 6)
        ready = gt_draft_80030BF0();
    state = (sint16)(r_u16(0x800B63D6) - 6);
    switch (state) {
    case 0:
        --timer;
        result = 1;
        if (!timer) {
            if (r_u32(0x800B630C) == 1) {
                result = 3;
                w_u8(0x800B63C0, result);
            } else {
                w_u16(0x800B63D6, 7);
                w_u8(0x800B63C8, 0);
                result = gtmain_race_recorders_stop();
            }
        }
        break;
    case 1:
        ++timer;
        result = 8;
        if (timer >= 48)
            w_u16(0x800B63D6, 8);
        break;
    case 2: case 7:
        result = gt_draft_80030B20();
        if ((sint32)result >= 0) {
            result = result ? 1 : 2;
            w_u8(0x800B63C0, result);
        }
        break;
    case 3:
        --timer;
        /* This branch retains the original dispatch target in V0 */
        result = 0x8002317C;
        if (!timer) {
            gtmain_race_recorders_stop();
            result = 10;
            w_u16(0x800B63D6, result);
            w_u8(0x800B63C8, 0);
        }
        break;
    case 4: case 5:
        ++timer;
        result = 0;
        if (!ready)
            result = timer < (sint32)(frame_step * 5);
        if (ready || !result) {
            if (state == 4) {
                gtmain_race_state_checkpoint();
                result = 11;
            } else {
                gtmain_race_points_rank_update();
                result = 12;
            }
            w_u16(0x800B63D6, result);
            timer = 0;
        }
        break;
    case 6:
        result = 13;
        if (ready)
            w_u16(0x800B63D6, result);
        else {
            ++timer;
            if (timer >= (sint32)(frame_step * 5))
                w_u16(0x800B63D6, result);
        }
        break;
    default:
        break;
    }
    w_u16(0x800B63DA, timer);
    return result;
}

uint32 gtmain_event_speed_summary_begin(void)
{
    uint32 frame_step, timer;
    FUNCTION_MARKER(0x80023474, "GTMAIN.EXE");
    frame_step = r_u32(0x800BF518);
    w_u16(0x800B63D6, 6);
    timer = frame_step * 3;
    w_u16(0x800B63DA, timer);
    return timer;
}

uint32 gtmain_event_speed_maximum_begin(void)
{
    uint32 frame_step, timer;
    FUNCTION_MARKER(0x800234A0, "GTMAIN.EXE");
    frame_step = r_u32(0x800BF518);
    w_u16(0x800B63D6, 7);
    timer = frame_step * 3;
    w_u16(0x800B63DA, timer);
    return timer;
}

uint32 gtmain_event_speed_maximum_select(void)
{
    FUNCTION_MARKER(0x800234CC, "GTMAIN.EXE");
    if (r_u8(0x800CB7F6) == 2)
        return gtmain_event_speed_maximum_begin();
    return 2;
}

uint32 gtmain_event_speed_summary_tick(void)
{
    sint32 state;
    FUNCTION_MARKER(0x800234FC, "GTMAIN.EXE");
    state = r_s16(0x800B63D6);
    if (state < 6 || state >= 8)
        return state < 6;
    {
        uint32 timer = r_u16(0x800B63DA) - 1;
        w_u16(0x800B63DA, timer);
        if ((sint16)timer == -1)
            w_u8(0x800B63C0, 3);
    }
    return 3;
}

uint32 gtmain_event_speed_summary_draw(void)
{
    uint32 mode, ordering, time_text, heading, result, words[2];
    sint32 state;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80023554, "GTMAIN.EXE");
    mode = r_u8(0x800CB7F6);
    if (mode > 2)
        return 2;
    frame = gt_local_stack_push(0xC8, 0x80023554);
    ordering = r_u32(0x800B6368);
    gtmain_text_font(3);
    gtmain_text_color(0x4080);
    state = r_s16(0x800B63D6);
    time_text = frame.address + (mode == 2 ? 0x18 : 0x38);
    heading = frame.address + (mode == 2 ? 0x58 : 0x78);
    if (state == 6) {
        gtmain_time_string(time_text, r_u32(0x800BDB70));
        words[0] = 0x800BD904;
        words[1] = time_text;
        gtmain_string_format(heading, 0x8008BB40, words, 2);
    } else if (state == 7) {
        uint32 unit = r_u8(0x800CB763);
        words[0] = r_s16(0x800BDB40);
        words[1] = unit ? 0x8008BB5C : 0x8008BB54;
        gtmain_string_format(heading, 0x8008BB48, words, 2);
    }
    if (state == 6 || state == 7) {
        uint32 width = gtmain_text_width(3, heading);
        sint32 x = (sint32)(320 - width) >> 1;
        gtmain_font_text_draw(ordering, x, 68, heading);
    }
    if (mode == 2)
        gtmain_race_countdown_block_draw(ordering, 68, 12);
    else
        gtmain_race_countdown_block_draw(ordering, 68, r_u8(0x800CB7F6) ? 11 : 10);
    gtmain_race_instruments_draw(ordering, 238, 150, 0x800B6850);
    if (mode != 2)
        gtmain_race_record_label_draw();
    gtmain_race_status_draw(ordering, 68, 0x800B6850);
    if (mode != 2) {
        time_text = frame.address + 0x18;
        gtmain_time_string(time_text, r_u32(0x800BDB70));
        gtmain_text_font(2);
        gtmain_text_color(128);
        gtmain_text_draw(ordering, 16, 200, time_text);
        result = gtmain_race_best_time_draw(ordering, 224, 16);
    } else {
        GTMainFormatArguments arguments = { words, 0, 2, 0 };
        gtmain_text_font(2);
        gtmain_text_color(128);
        words[0] = r_s16(0x800BDB40);
        words[1] = r_u8(0x800CB763) ? 0x8008BB5C : 0x8008BB54;
        result = gtmain_text_format(ordering, 16, 200, 0x8008BB48, &arguments);
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_event_points_table_draw(uint32 ordering, sint32 y, sint32 count, uint32 final_scores)
{
    uint32 node, index = 0, result, text;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x800216A4, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x70, 0x800216A4);
    text = frame.address + 0x10;
    result = gtmain_text_font(2);
    node = r_u32(0x8008F3E8);
    while (node) {
        uint32 words[2], name, width;
        result = (sint32)index < count;
        if (!result)
            break;
        gtmain_text_color(r_s16(node + 26) == 1 ? 0x808000 : 0x808080);
        w_u8(text, index + 49);
        w_u8(text + 1, 0);
        gtmain_font_text_draw(ordering, 16, y, text);
        name = gtmain_resource_name(r_u32(node + 2332));
        gtmain_font_text_draw(ordering, 32, y, name);
        if (!final_scores) {
            uint32 placing = r_u16(node + 36);
            words[0] = r_u16(node + 38);
            words[1] = r_s8(0x8008F3D0 + placing);
            gtmain_string_format(text, 0x8008B9E0, words, 2);
        } else {
            words[0] = r_u16(node + 38);
            gtmain_string_format(text, 0x8008B9EC, words, 1);
        }
        width = gtmain_text_width(2, text);
        result = gtmain_font_text_draw(ordering, (sint32)(304 - width), y, text);
        y = (sint32)((uint32)y + 18);
        node = r_u32(node + 8);
        ++index;
    }
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_trial_record_values_load(uint32 alternate)
{
    uint32 base, entry, maximum, result;
    FUNCTION_MARKER(0x80023870, "GTMAIN.EXE");
    base = alternate ? 0x800CA65C : 0x800CA1C4;
    entry = base + r_u8(0x800B63BD) * 56;
    maximum = r_u32(entry + 40);
    result = r_u32(entry + 12);
    w_u32(0x800BDB4C, maximum);
    w_u32(0x800BDB48, result);
    return result;
}

uint32 gtmain_trial_records_commit(uint32 alternate)
{
    uint32 base, first, second, flags = 0, total;
    sint16 laps;
    FUNCTION_MARKER(0x800238C0, "GTMAIN.EXE");
    base = alternate ? 0x800CA65C : 0x800CA1C4;
    first = base + r_u8(0x800B63BD) * 56;
    second = first + 28;
    if (r_s8(0x800BDD01)) {
        w_u32(second + 12, r_u32(0x800BDB4C));
        w_u16(second + 16, r_u16(0x800B6670));
        flags = 2;
    }
    if (r_s8(0x800BDD00)) {
        w_u32(first + 12, r_u32(0x800BDB48));
        w_u16(first + 16, r_u16(0x800B6670));
        flags |= 1;
    }
    total = r_u32(0x800CB8E0);
    gtmain_race_record_update(total);
    laps = r_s16(0x800B634C);
    if (alternate)
        return gtmain_menu_result_mode8_run(flags, first, second, 0x800BDB70, laps, total);
    return gtmain_menu_result_mode7_run(flags, first, second, 0x800BDB70, laps, total);
}

uint32 gtmain_trial_caption_select(sint32 mode)
{
    FUNCTION_MARKER(0x800239E0, "GTMAIN.EXE");
    if (mode >= 3)
        return 0;
    if (mode < 0)
        return (uint32)mode << 2;
    return gtmain_course_caption_copy(r_u32(0x8008F710 + (uint32)mode * 4));
}

uint32 gtmain_trial_ranking_table(sint32 mode)
{
    FUNCTION_MARKER(0x80023A1C, "GTMAIN.EXE");
    if (!mode)
        return 0x800CA7AC;
    if (mode == 1)
        return 0x800CA9DC;
    return 0x800CAC0C;
}

uint32 gtmain_trial_best_value_load(sint32 mode)
{
    uint32 result;
    FUNCTION_MARKER(0x80023A64, "GTMAIN.EXE");
    if (mode == 2)
        return 2;
    result = r_u32(gtmain_trial_ranking_table(mode) + 12);
    if (!result)
        result = 0xFFFFFFFF;
    w_u32(0x800BDB4C, result);
    return result;
}

uint32 gtmain_menu_result_speed_run(uint32 extra, sint16 index, uint32 first)
{
    FUNCTION_MARKER(0x800341CC, "GTMAIN.EXE");
    gtmain_menu_draw_callback_set(0x800919B4);
    return gtmain_menu_result_panel_run(0, index, first, 0, 0, 1, extra, 3, 22, 0);
}

uint32 gtmain_trial_ranking_commit(sint32 mode)
{
    uint32 value, table, position = 0;
    sint16 inserted = -1;
    FUNCTION_MARKER(0x80023AA8, "GTMAIN.EXE");
    if (mode == 2)
        value = r_s16(0x800BDB40);
    else
        value = r_u32(0x800B6870);
    table = gtmain_trial_ranking_table(mode);
    while (position < 20) {
        uint32 previous = r_u32(table + position * 28 + 12);
        if (mode == 2) {
            if (previous < value)
                break;
        } else if (!previous || value < previous) {
            break;
        }
        ++position;
    }
    if (position < 20) {
        uint32 entry = table + position * 28;
        inserted = position;
        if (position != 19)
            gtmain_memory_move(entry + 28, entry, (19 - position) * 28);
        sub_8007BD58(entry, 0, 28);
        w_u32(entry + 12, value);
        w_u16(entry + 16, r_u16(0x800B6670));
    }
    if (mode == 2)
        return gtmain_menu_result_speed_run(value, inserted, table);
    if (!mode)
        return gtmain_menu_result_mode4_run(value, inserted, table);
    return gtmain_menu_result_mode4_alternate_run(value, inserted, table);
}

uint32 gtmain_menu_result_mode6_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra)
{
    FUNCTION_MARKER(0x80033C44, "GTMAIN.EXE");
    gtmain_menu_draw_callback_set(0x8009197C);
    return gtmain_menu_result_panel_run(flags, index, first, second, metric, threshold, extra, 6, 24, 0);
}

uint32 gtmain_menu_result_mode1_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra)
{
    FUNCTION_MARKER(0x80033D9C, "GTMAIN.EXE");
    gtmain_menu_draw_callback_set(0x8009197C);
    return gtmain_menu_result_panel_run(flags, index, first, second, metric, threshold, extra, 1, 13, 0);
}

uint32 gtmain_race_summary_delay_begin(void)
{
    /* Original occurrences
     * GTMAIN.EXE:0x8002633C
     * GTMAIN.EXE:0x800264D0
     */
    uint32 timer = r_u32(0x800BF518);
    w_u16(0x800B63D6, 6);
    timer <<= 3;
    w_u16(0x800B63DA, timer);
    return timer;
}

uint32 gtmain_race_summary_delay_tick(void)
{
    /* Original occurrences
     * GTMAIN.EXE:0x80026364
     * GTMAIN.EXE:0x800264F8
     */
    sint32 state = r_s16(0x800B63D6);
    sint32 timer = r_s16(0x800B63DA);
    uint32 result = 6;
    if (state == 6) {
        --timer;
        result = 2;
        if (!timer)
            w_u8(0x800B63C0, result);
    }
    w_u16(0x800B63DA, timer);
    return result;
}

uint32 gtmain_race_signed_text_if_enabled(void)
{
    sint32 enabled;
    FUNCTION_MARKER(0x8002E368, "GTMAIN.EXE");
    enabled = r_s16(0x8009071C);
    if (enabled)
        return gtmain_race_signed_text_draw(0x800B39B4);
    return enabled;
}

uint32 gtmain_arcade_summary_draw(void)
{
    sint32 state;
    uint32 ordering;
    FUNCTION_MARKER(0x800263A4, "GTMAIN.EXE");
    state = r_s16(0x800B63D6);
    ordering = r_u32(0x800B6368);
    if (state)
        gtmain_race_label_font3_draw(ordering, 68, 0x808080, 1);
    gtmain_race_countdown_block_draw(ordering, 68, r_u8(0x800CB7F4) == 1 ? 8 : 9);
    gtmain_race_times_draw(ordering);
    gtmain_race_record_label_draw();
    gtmain_race_signed_text_if_enabled();
    return gtmain_race_driver_hud_draw(ordering);
}

void gtmain_two_player_times_reset(void)
{
    FUNCTION_MARKER(0x80026430, "GTMAIN.EXE");
    w_u32(0x800BDB44, 0);
    w_u32(0x800BDB64, 0);
}

uint32 gtmain_menu_result_mode2_run(uint8 player, uint32 record, uint32 time)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80033F7C, "GTMAIN.EXE");
    frame = gt_local_stack_push(8, 0x80033F7C);
    w_u32(frame.address, time);
    gtmain_menu_draw_callback_set(0x80091998);
    result = gtmain_menu_result_panel_run(player, 0, record, record, frame.address, 1, time, 2, 14, 0);
    gt_local_stack_pop(frame);
    return result;
}

uint32 gtmain_two_player_result_show(void)
{
    uint32 second_time, first_time, player = 0, time, record;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80026448, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x30, 0x80026448);
    second_time = r_u32(0x800BDB64);
    first_time = r_u32(0x800BDB44);
    if (second_time && (!first_time || second_time < first_time))
        player = 1;
    time = player ? second_time : first_time;
    record = frame.address + 0x10;
    w_u32(record + 4, time);
    w_u16(record + 8, r_u16(0x800B6670 + player * 76));
    gtmain_menu_result_mode2_run(player, record, time);
    gt_local_stack_pop(frame);
    return 0;
}

sint32 gtmain_arcade_car_count_setting(void)
{
    uint32 text;
    FUNCTION_MARKER(0x80026538, "GTMAIN.EXE");
    text = gtmain_environment_value(0x8008BC00);
    return gtmain_string_decimal(text);
}

uint32 gtmain_arcade_car_name_setting(sint32 index)
{
    FUNCTION_MARKER(0x80026564, "GTMAIN.EXE");
    return gtmain_environment_field(0x8008BC0C, index, 0x800B3918);
}

uint32 gtmain_course_segment_radial_draw(uint32 context, uint32 segment)
{
    uint32 records, count;
    FUNCTION_MARKER(0x8002723C, "GTMAIN.EXE");
    w_u32(context + 928, segment);
    w_u32(context + 936, r_u32(segment + 164));
    gtmain_course_segment_transform(context, segment);
    records = r_u32(segment + 204);
    count = r_u16(segment + 230);
    return gtmain_model_radial_draw(context, records, count);
}

void gtmain_scene_resources_invalidate(void)
{
    FUNCTION_MARKER(0x80027400, "GTMAIN.EXE");
    gtmain_camera_resource_reset();
    gtmain_race_track_invalidate();
    gtmain_race_cars_invalidate();
}

sint32 gtmain_two_player_course_select(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 text, name;
    sint32 result;
    FUNCTION_MARKER(0x8002748C, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x38, 0x8002748C);
    text = gtmain_string_copy(frame.address + 0x10, 0x8008BC94);
    name = gtmain_course_name(r_u8(0x800B63BC));
    text = gtmain_string_append(text, name);
    result = gtmain_course_name_find(text);
    if (result >= 0)
        w_u8(0x800B63BC, result);
    gt_local_stack_pop(frame);
    return result;
}

sint32 gtmain_arcade_course_index(uint32 course)
{
    uint32 name;
    FUNCTION_MARKER(0x800275C0, "GTMAIN.EXE");
    name = gtmain_course_name(course);
    return gtmain_race_track_find(0x80091A68, name);
}

sint32 gtmain_championship_continue(uint8 disabled, uint8 has_next)
{
    FUNCTION_MARKER(0x80037F80, "GTMAIN.EXE");
    w_u8(0x801F2FEA, has_next);
    return gtmain_menu_quick_end_run(0x80091E38, disabled, 0x80091E28, 0, 6);
}

uint32 gtmain_championship_run(void)
{
    uint32 race = 0, category, result;
    sint32 status;
    FUNCTION_MARKER(0x80027B48, "GTMAIN.EXE");
    w_u8(0x800CB7F9, 0);
    w_u8(0x800B62E0, 0);
    w_u8(0x800B638C, 0);
    gtmain_event_opponents_generate();
    category = gtmain_event_result_category();
    gtmain_race_driver_configurations(0x800D6FDC, 2, 3, category);
    gtmain_race_ranks_reset();
next_race:
    {
        uint32 sessions = r_u32(0x800C4C48);
        w_u8(0x800CB7F9, race);
        w_u8(0x800B63D0, 0);
        w_u32(0x800C4C48, sessions + 1);
    }
    gtmain_race_text_bind(0x8008F7C4);
    w_u8(0x800B638C, 0);
    if (gtmain_course_selection_apply(2))
        return gtmain_event_championship_award();
    gtmain_race_header_update(0);
    gt_draft_80022BFC(0);
    w_u8(0x800B63CA, 0);
    w_u32(0x800B630C, 0);
    status = gtmain_race_session_loop();
    result = gt_draft_80027AE8();
    if (status == 4)
        return result;
    if (status == 7)
        goto last_start_position;
    gtmain_race_text_bind(0x8008F7CC);
    gtmain_course_selection_apply(0);
    status = gtmain_race_session_prepare();
    result = gt_draft_80027AE8();
    if (status == 4)
        return result;
    if (status == 7) {
last_start_position:
        w_u8(0x800B63D0, 5);
    } else {
        gt_draft_80022C4C(0);
        gtmain_event_prize_award(0);
        w_u8(0x800B63D0, r_u8(0x800B63D2));
    }
    gtmain_race_cars_invalidate();
    gtmain_race_rank_reorder();
    {
        sint32 position = r_s8(0x800B63D0);
        sint32 rank = r_s8(0x800B63D2);
        sint32 previous = r_s8(0x800B63D1);
        printf((const char *)psx_addr(0x8008BCA0, 29), position, rank, previous);
    }
    gtmain_race_text_bind(0x8008F7D0);
    w_u8(0x800B638C, 1);
    gtmain_course_selection_apply(2);
    gtmain_race_header_update(1);
    gt_draft_80022BFC(1);
    w_u8(0x800B63CA, 0);
    w_u32(0x800B630C, 0);
    status = gtmain_race_session_loop();
    result = gt_draft_80027AE8();
    if (status == 4 || status == 7)
        return result;
    gtmain_race_text_bind(0);
    gtmain_course_selection_apply(1);
    gtmain_race_header_update(1);
    status = gtmain_race_session_prepare();
    if (status == 4 || status == 7)
        return gt_draft_80027AE8();
    gt_draft_80022C4C(1);
    gtmain_event_prize_award(1);
    for (;;) {
        uint32 has_next = gtmain_event_race_has_next();
        sint32 selection = gtmain_championship_continue(1, has_next);
        if (selection == -1)
            return gt_draft_80027AE8();
        if (selection) {
            ++race;
            gt_draft_80027AE8();
            w_u8(0x800B63D0, 0);
            gtmain_roster_initialize();
            goto next_race;
        }
        w_u32(0x800B630C, 1);
        gtmain_race_session_loop();
    }
}

sint32 gtmain_trial_course_options(void)
{
    FUNCTION_MARKER(0x80037F00, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091C4C, 0, 0x80091C20, 0x80091C1C, 7);
}

sint32 gtmain_trial_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80037EC0, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091CD4, disabled, 0x80091CA8, 0x80091CA0, 7);
}

sint32 gtmain_two_player_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80037F40, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091D5C, disabled, 0x80091D4C, 0x80091D48, 5);
}

sint32 gtmain_trial_record_continue(uint8 disabled, uint8 style, uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x80037FC8, "GTMAIN.EXE");
    w_u32(0x80091DBC, first);
    w_u32(0x80091DD8, second);
    return gtmain_menu_quick_end_run(0x80091DB8, disabled, 0x80091DA8, 0x80091DA0, style);
}

sint32 gtmain_trial_mode0_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80038018, "GTMAIN.EXE");
    return gtmain_trial_record_continue(disabled, 0, 0x800BCD55, 0x800BCD97);
}

sint32 gtmain_trial_mode1_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80038044, "GTMAIN.EXE");
    return gtmain_trial_record_continue(disabled, 1, 0x800BCD68, 0x800BCDAC);
}

sint32 gtmain_trial_mode2_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80038070, "GTMAIN.EXE");
    return gtmain_trial_record_continue(disabled, 2, 0x800BCD7C, 0x800BCDC2);
}

sint32 gtmain_alternate_trial_continue(uint8 disabled)
{
    FUNCTION_MARKER(0x80037D80, "GTMAIN.EXE");
    return gtmain_menu_quick_end_run(0x80091EB0, disabled, 0x80091E84, 0x80091E7C, 9);
}

uint32 gtmain_game_mode_run(void)
{
    uint32 mode, subtype, name, selection_index;
    sint32 status, selection;
    FUNCTION_MARKER(0x80027E18, "GTMAIN.EXE");
    mode = r_u8(0x800CB7F5);
    w_u8(0x800B638C, 2);
    if (mode != 4)
        gtmain_race_render_config(0);
    switch (mode) {
    case 1:
        w_u8(0x800B63BD, r_u8(0x800CB7F6));
        selection_index = r_u8(0x800B63BD);
        name = r_u32(0x8008F7E4 + selection_index * 4);
        w_u8(0x800B63BC, gtmain_course_name_find(name));
        w_u16(0x800B634C, 2);
        w_u8(0x800B6394, 1);
        gtmain_race_record_context_check();
        gtmain_race_config_reset();
        gtmain_race_driver_configurations(0x800D6FDC, 2, 1, 3);
        gtmain_race_track_invalidate();
        gtmain_race_header_update(0);
        gtmain_race_track_load(r_u8(0x800B63BC));
        gtmain_race_cars_load();
        gtmain_race_car_palettes(r_u8(0x800B63BC));
        selection = gtmain_trial_course_options();
        if (selection == -1)
            return selection;
        gtmain_race_driver_setup_if_needed(2);
        gtmain_race_text_bind(0x8008F7BC);
trial_race:
        w_u8(0x800B63D0, 0);
        gtmain_trial_record_values_load(0);
        if (gtmain_race_session_prepare() != 4)
            gtmain_trial_records_commit(0);
        for (;;) {
            selection = gtmain_trial_continue(1);
            if (selection == -1)
                return 0xFFFFFFFF;
            if (selection) {
                gtmain_race_driver_setup_if_needed(2);
                goto trial_race;
            }
            w_u32(0x800B630C, 1);
            gtmain_race_session_loop();
        }
    case 2:
        w_u8(0x800B63BD, r_u8(0x800CB7F8));
        selection_index = r_u8(0x800B63BD);
        w_u8(0x800B63D0, 0);
        w_u8(0x800B63D3, 1);
        name = r_u32(0x8008F858 + selection_index * 4);
        {
            sint32 course = gtmain_course_name_find(name);
            uint8 printed = r_u8(0x800B63BD);
            w_u8(0x800B63BC, course);
            printf((const char *)psx_addr(0x8008BE10, 20), printed);
        }
        gtmain_two_player_course_select();
        selection_index = r_u8(0x800CB776);
        w_u8(0x800B6394, 2);
        w_u16(0x800B634C, r_u8(0x8008F7AC + selection_index));
        gtmain_race_config_reset();
        gtmain_race_driver_configurations(0x800D6FDC, 2, 2, 3);
        gtmain_race_track_invalidate();
        gtmain_race_header_update(0);
two_player_race:
        gtmain_two_player_times_reset();
        if (gtmain_race_session_prepare() != 4)
            gtmain_two_player_result_show();
        for (;;) {
            selection = gtmain_two_player_continue(1);
            if (selection == -1)
                return 0xFFFFFFFF;
            if (selection)
                goto two_player_race;
            w_u32(0x800B630C, 1);
            gtmain_race_session_loop();
        }
    case 3:
        return gtmain_championship_run();
    case 4:
        gtmain_race_render_config(1);
        gtmain_event_preview_models_load();
        gtmain_race_config_reset();
        goto license_prepare;
    case 5:
        subtype = r_u8(0x800CB7F6);
        if (subtype >= 11)
            return subtype << 2;
        if (subtype == 10)
            return 0x80028580;
        if (subtype >= 3) {
            w_u8(0x800CB7F5, 3);
            w_u8(0x800CB7F8, subtype + 12);
            return gtmain_championship_run();
        }
        w_u8(0x800B6394, 1);
        w_u16(0x800B634C, 1);
        name = subtype == 2 ? 0x8008BCE4 : 0x8008BCD8;
        w_u8(0x800B63BC, gtmain_course_name_find(name));
        gtmain_race_config_reset();
        gtmain_race_driver_configurations(0x800D6FDC, 2, 5, 3);
        if (subtype == 2) {
            w_u16(0x800CB7FA, r_u16(0x800B63C4));
            gtmain_trial_caption_select(2);
        } else {
            uint32 saved = r_u16(0x800B63C4);
            uint32 current = r_u8(0x800CB7F6);
            w_u16(0x800CB7FA, saved);
            gtmain_trial_caption_select(current);
        }
        gtmain_race_text_bind(0x8008F7B8);
record_attempt:
        w_u8(0x800B63D0, 0);
        gtmain_trial_best_value_load(subtype);
        w_u8(0x800B63CA, 0);
        w_u32(0x800B630C, 0);
        status = gtmain_race_session_loop();
        gt_draft_80027AE8();
        if (status != 4)
            gtmain_trial_ranking_commit(subtype);
        for (;;) {
            if (subtype == 2)
                selection = gtmain_trial_mode2_continue(1);
            else if (subtype)
                selection = gtmain_trial_mode1_continue(1);
            else
                selection = gtmain_trial_mode0_continue(1);
            if (selection == -1)
                return 0xFFFFFFFF;
            if (selection)
                goto record_attempt;
            w_u32(0x800B630C, 1);
            gtmain_race_session_loop();
        }
    case 6:
        w_u8(0x800B63BD, r_u8(0x800CB7F6));
        selection_index = r_u8(0x800B63BD);
        w_u8(0x800CB7F5, 1);
        name = r_u32(0x8008F83C + selection_index * 4);
        w_u32(0x800B6318, 1);
        w_u8(0x800B63BC, gtmain_course_name_find(name));
        w_u16(0x800B634C, 2);
        gtmain_race_record_context_check();
        w_u8(0x800B6394, 1);
        gtmain_race_config_reset();
        gtmain_race_track_invalidate();
        gtmain_race_driver_configurations(0x800D6FDC, 2, 1, 3);
        gtmain_race_header_update(0);
alternate_trial:
        w_u8(0x800B63D0, 0);
        gtmain_trial_record_values_load(1);
        if (gtmain_race_session_prepare() != 4)
            gtmain_trial_records_commit(1);
        for (;;) {
            selection = gtmain_alternate_trial_continue(1);
            if (selection == -1)
                return 0xFFFFFFFF;
            if (selection)
                goto alternate_trial;
            w_u32(0x800B630C, 1);
            gtmain_race_session_loop();
        }
    default:
        return (mode - 1) << 2;
    }
license_prepare:
    gtmain_race_record_selection_prepare();
license_attempt:
    {
        sint32 index = -1, position = -1;
        uint32 flags = 0, sessions = r_u32(0x800C4C48);
        w_u8(0x800B63CA, 0);
        w_u32(0x800B630C, 0);
        w_u32(0x800C4C48, sessions + 1);
        status = gtmain_race_session_loop();
        if (status != 4 && status != 6) {
            uint32 packed = gtmain_license_results_commit();
            index = packed & 0xFFFF;
            position = (sint32)packed >> 24;
            flags = (packed >> 16) & 255;
        }
        if (flags & 2) {
            uint32 event = r_u8(0x800CB7F8);
            if (gt_draft_8001EE20(r_u32(0x8008F7D8 + event * 4)))
                flags |= 4;
        }
        for (;;) {
            selection = gtmain_license_result_menu(index, position, flags);
            if (selection == 1)
                goto license_attempt;
            if (selection >= 2) {
                if (selection == 2)
                    goto license_prepare;
                return 2;
            }
            if (selection)
                return 1;
            w_u32(0x800B630C, 1);
            index = -1;
            gtmain_race_session_loop();
            flags = 0;
            position = -1;
        }
    }
}

uint32 gtmain_license_completion_masks(void)
{
    uint32 table, completed = 0, gold = 0, index;
    FUNCTION_MARKER(0x8002A408, "GTMAIN.EXE");
    table = gt_draft_8002A3AC();
    for (index = 0; index < 8; ++index) {
        uint32 value = r_u8(table + index);
        uint32 bit = 1 << index;
        if (value) {
            if (value == 3)
                gold |= bit;
            completed |= bit;
        }
    }
    return completed | (gold << 16);
}

uint32 gtmain_license_results_commit(void)
{
    sint32 medal, status, rank = -1;
    uint32 before, after, address, previous, table, time, position, flags = 0;
    FUNCTION_MARKER(0x8002A634, "GTMAIN.EXE");
    medal = r_s8(0x8008F894);
    status = r_s8(0x8008F893);
    if (status != 1)
        return 0xFF000000;
    before = gtmain_license_completion_masks();
    address = gt_draft_8002A3AC();
    address += r_u8(0x800CB7F9);
    previous = r_u8(address);
    if ((sint32)previous < medal)
        w_u8(address, medal);
    after = gtmain_license_completion_masks();
    if ((before & 0xFFFF) != (after & 0xFFFF) && (after & 0xFFFF) == 255)
        flags |= 1;
    if ((before >> 16) != (after >> 16) && (after >> 16) == 255)
        flags |= 2;
    table = gt_draft_8002A3CC();
    time = r_u32(0x800BF52C);
    for (position = 0; position < 8; ++position) {
        previous = r_u32(table + position * 28 + 12);
        if (!previous || time < previous)
            break;
    }
    if (position < 8) {
        uint32 row;
        for (row = 7; row > position; --row) {
            uint32 destination = table + row * 28;
            uint32 a = r_u32(destination - 28);
            uint32 b = r_u32(destination - 24);
            uint32 c = r_u32(destination - 20);
            uint32 d = r_u32(destination - 16);
            w_u32(destination, a);
            w_u32(destination + 4, b);
            w_u32(destination + 8, c);
            w_u32(destination + 12, d);
            a = r_u32(destination - 12);
            b = r_u32(destination - 8);
            c = r_u32(destination - 4);
            w_u32(destination + 16, a);
            w_u32(destination + 20, b);
            w_u32(destination + 24, c);
        }
        w_u32(table + position * 28 + 12, time);
        rank = position;
    }
    return (uint32)medal | (flags << 16) | ((uint32)rank << 24);
}

sint32 gtmain_license_result_menu(sint32 index, sint32 position, uint32 flags)
{
    uint32 event, race;
    FUNCTION_MARKER(0x80039ADC, "GTMAIN.EXE");
    if (!r_s8(0x80091ECC)) {
        uint32 path, table, size;
        GT_LOCAL_STACK_FRAME frame = gt_local_stack_push(0x40, 0x80039ADC);
        w_u8(0x80091ECC, 1);
        gtmain_stream_mode_set(128);
        path = gtmain_environment_value(0x8008CCAC);
        table = gtmain_archive_table_load_guest(path, frame.address + 0x18,
            0x801D2FDC, 4096);
        size = r_u32(table + 84) + 3;
        path = gtmain_environment_value(0x8008CCAC);
        size &= ~3;
        gtmain_archive_section_load(path, (const uint8 *)psx_addr(table, 4096),
            r_u32(frame.address + 0x18), 5, 0x801D6FDC, size);
        gtmain_tim_upload(0x801D6FDC, 0x01FF0180, -1, 0x010001C0, -1, 1);
        gt_local_stack_pop(frame);
    }
    printf((const char *)psx_addr(0x8008CCBC, 21));
    event = r_u8(0x800CB7F8);
    race = r_u8(0x800CB7F9);
    w_u8(0x801F2FE0, index);
    w_u8(0x801F2FE1, position);
    w_u8(0x801F2FE6, flags);
    w_u8(0x801F2FE2, event);
    w_u8(0x801F2FE3, race);
    gt_draft_8003CC24(0x801F300C, 1, 0x8009209C, 18, 0);
    w_u8(0x801F2FE7, 0);
    gtmain_menu_draw_callback_set(index > 0 ? 0x800920B8 : 0x8009209C);
    gtmain_menu_panel_special_bind(0x800BDD10, 0x80092154);
    gtmain_music_sector_init();
    gtmain_stream_stop_wait(1);
    gtmain_menu_frame_run();
    gtmain_stream_stop_wait(1);
    return r_s8(0x801F2FE4);
}

uint32 gtmain_menu_text_layout(uint32 index, uint32 draw, uint32 y, uint32 ordering)
{
    uint32 cursor, groups, group, indent;
    FUNCTION_MARKER(0x80029130, "GTMAIN.EXE");
    cursor = 0x8019CFDC + r_u16(0x8019CFDC + (index << 1));
    groups = r_u8(cursor);
    cursor += 2;
    if (draw) {
        uint32 title = cursor + 1;
        uint32 width, remaining, x;
        gtmain_text_font(2);
        width = gtmain_text_width(2, title);
        remaining = 288 - width;
        x = (uint32)((sint32)remaining / 2) + 16;
        gtmain_font_text_draw(ordering, x, y + 2, title);
        gtmain_text_font(1);
        y += 24;
    }
    cursor += r_u8(cursor) + 1;
    indent = r_u8(0x800C4C3D) != 3 ? 8 : 0;
    for (group = 0; group < groups; ++group) {
        uint32 words = r_u8(cursor++);
        uint32 first = 0;
        while (first < words) {
            uint32 render_cursor = cursor;
            uint32 used = indent, end = first;
            while (end < words) {
                uint32 width = gtmain_text_width(1, cursor + 1);
                uint32 candidate;
                if (end != first)
                    width += 4;
                candidate = used + width;
                if ((sint32)candidate >= 289)
                    break;
                ++end;
                cursor += r_u8(cursor) + 1;
                used = candidate;
            }
            if (draw) {
                uint32 slack = 288 - used;
                sint32 gaps = end - first - 1;
                uint32 x = indent + 16, word;
                if (!gaps)
                    gaps = 1;
                for (word = first; word < end; ++word) {
                    uint32 text = render_cursor + 1;
                    uint32 width = gtmain_text_width(1, text);
                    gtmain_font_text_draw(ordering, x, y, text);
                    x += 4 + width;
                    render_cursor += r_u8(render_cursor) + 1;
                    if (end != words) {
                        uint32 product = slack * (word - first);
                        sint32 before = (sint32)product / gaps;
                        sint32 after;
                        product = slack * (word - first + 1);
                        after = (sint32)product / gaps;
                        x += (uint32)after - (uint32)before;
                    }
                }
                y += 12;
            }
            first = end;
            indent = 0;
        }
    }
    return groups + 2;
}

uint32 gtmain_menu_text_panel_draw(uint32 ordering, uint32 index)
{
    sint32 lines, scroll;
    uint32 y = 64, page, offset;
    FUNCTION_MARKER(0x80029430, "GTMAIN.EXE");
    lines = r_s8(0x8008F88F);
    if (lines)
        y -= 12 * lines;
    scroll = r_s8(0x8008F891);
    page = r_u32(0x800BFF68) ? 240 : 0;
    y -= scroll;
    offset = page << 11;
    gtmain_packet_draw_area(ordering, offset, page << 10, ((page + 239) << 10) | 0x13F);
    gtmain_text_font(1);
    gtmain_text_color(0x808080);
    gtmain_menu_text_layout(index, 1, y, ordering);
    return gtmain_packet_draw_area(ordering, offset,
        ((page + 64) << 10) | 8, ((page + 195) << 10) | 0x137);
}

uint32 gtmain_menu_background_draw(uint32 ordering)
{
    FUNCTION_MARKER(0x80029520, "GTMAIN.EXE");
    gtmain_packet_rectangle(ordering, 0x8008FAF0, 0x02000000);
    return gtmain_packet_page(ordering, 512);
}

sint32 gtmain_menu_heading_draw(uint32 ordering)
{
    FUNCTION_MARKER(0x8002955C, "GTMAIN.EXE");
    gtmain_text_font(3);
    gtmain_text_color(0x808080);
    return gtmain_font_text_draw(ordering, 16, 17, 0x800BC2E0);
}

sint32 gtmain_menu_status_draw(uint32 ordering)
{
    uint32 width;
    FUNCTION_MARKER(0x800295A8, "GTMAIN.EXE");
    width = gtmain_text_width(1, 0x800BD92F);
    gtmain_text_font(1);
    gtmain_text_color(0x808080);
    return gtmain_font_text_draw(ordering, 304 - width, 32, 0x800BD92F);
}

uint32 gtmain_menu_result_footer_draw(uint32 ordering)
{
    FUNCTION_MARKER(0x80029614, "GTMAIN.EXE");
    gtmain_packet_line_segment(ordering, 0x8008FB1C, 255);
    gtmain_packet_rectangle(ordering, 0x8008FB04, 0x00AA162C);
    gtmain_packet_rectangle(ordering, 0x8008FB0C, 0);
    return gtmain_packet_rectangle(ordering, 0x8008FB14, 0);
}

uint32 gtmain_menu_button_draw(uint32 ordering, uint32 color, uint32 text, uint32 corners, uint32 x)
{
    uint32 width, remaining;
    FUNCTION_MARKER(0x80029684, "GTMAIN.EXE");
    gtmain_text_font(1);
    gtmain_text_color(0x808080);
    width = gtmain_text_width(1, text);
    remaining = 96 - width;
    gtmain_font_text_draw(ordering, x + (uint32)((sint32)remaining / 2), 213, text);
    gtmain_packet_outline(ordering, corners, 0x404040);
    return gtmain_packet_rectangle(ordering, corners, color);
}

uint32 gtmain_packet_line_segment(uint32 ordering, uint32 endpoints, uint32 color)
{
    uint32 first, second, packet, address, lane, byte, previous;
    FUNCTION_MARKER(0x8006F5D4, "GTMAIN.EXE");
    first = r_u32(endpoints);
    second = r_u32(endpoints + 4);
    packet = r_u32(0x800BFE4C);
    w_u32(packet, 0x03000000);
    w_u32(packet + 4, color ^ 0x41000000);
    w_u32(packet + 8, first);
    w_u32(packet + 12, second);
    address = ordering + 2;
    lane = address & 3;
    address &= ~3;
    previous = second;
    /* Preserve LWL merge lanes before either SWL can alias the source */
    for (byte = 0; byte <= lane; ++byte) {
        uint32 shift = (3 - lane + byte) * 8;
        uint32 value = r_u8(address + byte);
        previous = (previous & ~((uint32)255 << shift)) | (value << shift);
    }
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, (packet << 8) >> ((3 - lane + byte) * 8));
    address = packet + 2;
    lane = address & 3;
    address &= ~3;
    for (byte = 0; byte <= lane; ++byte)
        w_u8(address + byte, previous >> ((3 - lane + byte) * 8));
    w_u32(0x800BFE4C, packet + 16);
    return packet;
}

uint32 gtmain_menu_request_draw(uint32 ordering, uint32 first_text, uint32 second_text, uint32 request)
{
    uint32 first, second = 0;
    FUNCTION_MARKER(0x80029740, "GTMAIN.EXE");
    first = gtmain_menu_request_pulse_color(0);
    if (r_s8(0x8008F88D)) {
        second = first;
        first = 0;
    }
    gtmain_menu_button_draw(ordering, first, first_text, 0x8008FB24, 64);
    gtmain_menu_button_draw(ordering, second, second_text, 0x8008FB2C, 160);
    gtmain_menu_text_panel_draw(ordering, request);
    gtmain_menu_background_draw(ordering);
    gtmain_text_flags(2);
    gtmain_menu_heading_draw(ordering);
    gtmain_menu_status_draw(ordering);
    gtmain_text_flags(0);
    return gtmain_menu_result_footer_draw(ordering);
}

uint32 gtmain_menu_scroll_tick(void)
{
    sint32 velocity, offset, line;
    uint32 upward;
    FUNCTION_MARKER(0x80029830, "GTMAIN.EXE");
    velocity = r_s8(0x8008F892);
    offset = r_s8(0x8008F891);
    line = r_s8(0x8008F88F);
    offset += velocity;
    if (offset >= 12) {
        offset = 0;
        velocity = 0;
        ++line;
    }
    upward = offset < -11;
    if (upward) {
        offset = 0;
        velocity = 0;
        --line;
    }
    w_u8(0x8008F892, velocity);
    w_u8(0x8008F891, offset);
    w_u8(0x8008F88F, line);
    return upward;
}

uint32 gtmain_menu_request_phase_tick(void)
{
    uint32 phase;
    FUNCTION_MARKER(0x80029898, "GTMAIN.EXE");
    phase = r_u8(0x8008F88C) + 1;
    w_u8(0x8008F88C, phase);
    return phase;
}

uint32 gtmain_menu_scroll_limit(uint32 request)
{
    sint32 lines;
    FUNCTION_MARKER(0x800298B4, "GTMAIN.EXE");
    lines = gtmain_menu_text_layout(request, 0, 0, 0);
    lines -= 11;
    return lines < 0 ? 0 : lines;
}

uint32 gtmain_menu_request_pulse_color(uint32 flags)
{
    uint32 phase, intensity;
    FUNCTION_MARKER(0x800298EC, "GTMAIN.EXE");
    phase = r_u8(0x8008F88C);
    intensity = phase & 7;
    if (phase & 8)
        intensity ^= 7;
    intensity <<= 5;
    return (flags << 24) | intensity | (intensity << 8);
}

uint32 gtmain_save_payload_read(uint32 source)
{
    uint32 checksum, stored;
    FUNCTION_MARKER(0x8002CA80, "GTMAIN.EXE");
    checksum = gtmain_data_checksum(source, 0x6BA4);
    stored = r_u32(source + 0x6BA4);
    if (stored != checksum)
        return 0;
    gtmain_memory_copy(0x800C4C40, source, 0x6BA8);
    return 1;
}

uint32 gtmain_save_payload_write(uint32 destination)
{
    uint32 checksum;
    FUNCTION_MARKER(0x8002CAD0, "GTMAIN.EXE");
    checksum = gtmain_data_checksum(0x800C4C40, 0x6BA4);
    w_u32(0x800CB7E4, checksum);
    gtmain_memory_copy(destination, 0x800C4C40, 0x6BA8);
    return 0x6BA8;
}

uint32 gtmain_race_time_notice(uint32 unused0, uint32 unused1, uint32 unused2, uint32 time)
{
    uint32 ticks, duration;
    FUNCTION_MARKER(0x8002E06C, "GTMAIN.EXE");
    gtmain_time_string(0x800B39A4, time);
    ticks = r_u32(0x800BF518);
    w_u16(0x80090718, 1);
    duration = ticks * 3;
    w_u16(0x8009071A, duration);
    return duration;
}

uint32 gtmain_race_result_hints_draw(uint32 ordering, uint32 first_text, uint32 second_text)
{
    uint32 first, second = 0;
    FUNCTION_MARKER(0x80030A3C, "GTMAIN.EXE");
    first = gtmain_race_choice_highlight(0);
    if (r_s16(0x800912E0)) {
        second = first;
        first = 0;
    }
    gtmain_race_centered_label_box_draw(ordering, first, first_text, 0x800912E4, 64);
    return gtmain_race_centered_label_box_draw(ordering, second, second_text, 0x800912EC, 160);
}

uint32 gtmain_race_choice_initialize(void)
{
    uint32 selected;
    FUNCTION_MARKER(0x80030C08, "GTMAIN.EXE");
    selected = r_u8(0x800BB7ED) != 0;
    w_u16(0x800912E0, selected);
    return selected;
}

uint32 gtmain_menu_result_mode5_run(uint8 flags, sint16 index, uint32 unused, uint32 alternate,
    uint32 record, uint32 metric, sint16 threshold, uint32 extra, uint8 panel)
{
    uint32 second;
    FUNCTION_MARKER(0x80033FF8, "GTMAIN.EXE");
    if (alternate) {
        second = record + 56;
        printf((const char *)psx_addr(0x8008CB08, 7));
    } else {
        second = record + 28;
        printf((const char *)psx_addr(0x8008CB10, 9));
    }
    gtmain_menu_draw_callback_set(0x8009197C);
    return gtmain_menu_result_panel_run(flags, index, record, second, metric, threshold,
        extra, 5, 23, panel);
}

uint32 gtmain_menu_reward_title_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 palette, sint32 intensity, sint32 elapsed)
{
    uint32 remaining, product, brightness, position, color;
    sint32 quotient;
    FUNCTION_MARKER(0x80034248, "GTMAIN.EXE");
    if (elapsed >= 9)
        elapsed = 8;
    remaining = 8 - (uint32)elapsed;
    product = (uint32)intensity * remaining;
    quotient = (sint32)product / 8;
    brightness = (uint32)intensity - (uint32)quotient;
    position = (uint32)x + remaining * 18;
    if (palette < 0) {
        uint32 animation = r_u8(0x801D3024);
        palette = r_s8(0x80091A08 + animation);
        color = gtmain_menu_palette_color(palette, (sint32)brightness >> 1);
        gtmain_text_color(color);
        return gtmain_menu_fixed_text_draw(ordering, position, y, text, 3);
    }
    color = gtmain_menu_palette_color(palette, (sint32)brightness >> 1);
    gtmain_text_color(color);
    return gtmain_font_text_draw(ordering, position, y, text);
}
