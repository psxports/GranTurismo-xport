#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gtmenu_offer_period_table(sint32 period)
{
    uint32 offers, offset;
    FUNCTION_MARKER(0x80012900, "GTMENU.EXE");
    offers = r_u32(0x8008055C);
    offset = r_u32(offers + (uint32)period * 4 + 16);
    return offers + offset;
}

uint32 gtmenu_offer_group_table(sint8 group, uint32 offers)
{
    uint32 skip, index, count;
    FUNCTION_MARKER(0x8001291C, "GTMENU.EXE");
    switch (group) {
    case 'n': skip = 1; break;
    case 'm': skip = 2; break;
    case 'h': skip = 3; break;
    case 'a': skip = 4; break;
    case 's': skip = 5; break;
    default: skip = 0; break;
    }
    for (index = 0; index < skip; ++index) {
        count = r_u32(offers);
        offers += count * 4 + 4;
    }
    return offers;
}

uint32 gtmenu_car_nodes_build(sint8 make, uint32 kind)
{
    FUNCTION_MARKER(0x80011220, "GTMENU.EXE");
    if (kind == 2) return gtmenu_record_nodes_build(2);
    if (kind == 3) return gtmenu_offer_nodes_build(make, 3);
    if (kind == 1) return gtmenu_model_nodes_build(make, kind, 'r', 1);
    return gtmenu_model_nodes_build(make, kind, 'n', 0);
}

uint32 gtmenu_list_node_snapshot(uint32 list, uint32 index)
{
    uint32 node, position = 0;
    FUNCTION_MARKER(0x800116C8, "GTMENU.EXE");
    node = r_u32(list);
    while (node) {
        if (position == index) {
            gtmenu_memory_copy(0x8007DA60, node, 24);
            return 0x8007DA60;
        }
        node = r_u32(node + 4);
        ++position;
    }
    return 0;
}

uint32 gtmenu_primary_list(void)
{
    FUNCTION_MARKER(0x8001173C, "GTMENU.EXE");
    return 0x8007DA40;
}

void gtmenu_days_set(uint32 days)
{
    FUNCTION_MARKER(0x8001175C, "GTMENU.EXE");
    w_u32(0x8009B868, days);
}

uint32 gtmenu_days_add(uint32 amount)
{
    uint32 days;
    FUNCTION_MARKER(0x8001177C, "GTMENU.EXE");
    days = r_u32(0x8009B868);
    w_u32(0x8009B868, days + amount);
    return days;
}

void gtmenu_lists_initialize(void)
{
    FUNCTION_MARKER(0x80011808, "GTMENU.EXE");
    gtmenu_list_initialize(0x8007DA40);
    gtmenu_list_initialize(0x8007DA50);
}

sint32 gtmenu_current_record_index(void)
{
    FUNCTION_MARKER(0x80011CE8, "GTMENU.EXE");
    return r_s16(0x8009B878);
}

uint32 gtmenu_part_nodes_build(uint32 group)
{
    uint32 archive, records, record, selected_record, owner, result, count, node;
    uint32 stride = 0, owner_offset = 0, variant_offset = 0, index = 0;
    sint32 selected;
    FUNCTION_MARKER(0x80011294, "GTMENU.EXE");
    archive = gtmenu_resource_group(group);
    records = archive + 24;
    selected = gtmenu_current_record_index();
    if (selected < 0) return selected;
    selected = gtmenu_current_record_index();
    selected_record = gtmenu_record_address(selected);
    result = r_u16(selected_record) & 0xFFF;
    count = r_u16(archive + 14);
    owner = result + 1;
    if (!count) return result;
    switch (group) {
    case 0x0F: case 0x15: case 0x1C:
        stride = 32; owner_offset = 16; break;
    case 0x10:
        stride = 24; owner_offset = 4; variant_offset = 3; break;
    case 0x12:
        stride = 24; owner_offset = 6; variant_offset = 8; break;
    case 0x14: case 0x18:
        stride = 32; owner_offset = 16; variant_offset = 18; break;
    case 0x16: case 0x1D: case 0x1F: case 0x20: case 0x22:
        stride = 20; owner_offset = 4; variant_offset = 6; break;
    case 0x17:
        stride = 68; owner_offset = 50; variant_offset = 1; break;
    case 0x19:
        stride = 20; owner_offset = 2; break;
    case 0x1A:
        stride = 36; owner_offset = 16; variant_offset = 18; break;
    case 0x1B:
        stride = 36; owner_offset = 16; variant_offset = 19; break;
    case 0x1E:
        stride = 32; owner_offset = 14; break;
    case 0x21:
        stride = 72; owner_offset = 54; variant_offset = 56; break;
    case 0x23:
        stride = 56; owner_offset = 26; variant_offset = 43; break;
    default: break;
    }
    do {
        record = records + index * stride;
        if (stride && r_u16(record + owner_offset) == owner &&
            (!variant_offset || r_u8(record + variant_offset) != 255)) {
            node = gtmenu_list_node_allocate();
            w_u8(node + 8, 1);
            w_u8(node + 9, group);
            w_u32(node + 12, record);
            w_u16(node + 10, index);
            gtmenu_list_append(0x8007DA40, node);
        }
        count = r_u16(archive + 14);
        ++index;
        result = group - 15 < 21;
    } while (index < count);
    return result;
}

void gtmenu_current_record_set(sint16 index)
{
    FUNCTION_MARKER(0x80011CD8, "GTMENU.EXE");
    w_u16(0x8009B878, index);
}

void gtmenu_credits_set(uint32 credits)
{
    FUNCTION_MARKER(0x80012534, "GTMENU.EXE");
    w_u32(0x8009B864, credits);
}

uint32 gtmenu_player_state_initialize(void)
{
    FUNCTION_MARKER(0x80011840, "GTMENU.EXE");
    gtmenu_credits_set(10000);
    gtmenu_current_record_set(-1);
    gtmenu_days_set(1);
    w_u16(0x8009B87A, 0);
    gtmenu_memory_fill(0x8009B820 + 0x1EC, 0, 9600);
    gtmenu_memory_fill(0x8009B820 + 0x276C, 0, 1061);
    gtmenu_memory_fill(0x8009B820 + 0x2B94, 0, 8);
    gtmenu_memory_fill(0x8009B820 + 0x2B9C, 0, 8);
    return gtmenu_memory_fill(0x8009B820 + 0x2BA4, 0, 8);
}

void gtmenu_lists_clear(void)
{
    FUNCTION_MARKER(0x800118D0, "GTMENU.EXE");
    gtmenu_list_clear(0x8007DA40);
    gtmenu_list_clear(0x8007DA50);
}

uint32 gtmenu_part_list_build(uint32 group, uint32 append)
{
    FUNCTION_MARKER(0x80011C7C, "GTMENU.EXE");
    if (!append) gtmenu_lists_clear();
    return gtmenu_part_nodes_build(group);
}

uint32 gtmenu_resource_archive(void)
{
    FUNCTION_MARKER(0x80012A28, "GTMENU.EXE");
    return r_u32(0x80080558);
}

sint32 gtmenu_garage_add(uint32 node, uint8 color)
{
    sint32 count, template_index;
    uint32 record, price_slot, archive, models, templates, model_index, word, price, model;
    FUNCTION_MARKER(0x80011928, "GTMENU.EXE");
    count = r_s16(0x8009B87A);
    record = 0x8009BA0C + (uint32)count * 96;
    price_slot = 0x8009B87C + (uint32)count * 4;
    archive = gtmenu_resource_archive();
    models = gtmenu_resource_group(13) + 24;
    templates = gtmenu_resource_group(0) + 24;
    if (count >= 100) return -1;
    gtmenu_memory_fill(record, 0, 96);
    model_index = r_u16(node + 10);
    template_index = gtmenu_record_template_index(models + model_index * 424, archive, 0);
    /* Original copy spans 108 bytes despite the 96-byte slot stride */
    gtmenu_memory_copy(record, templates + (uint32)template_index * 108, 108);
    word = r_u16(record);
    word = ((word - 1) & 0xFFF) | ((uint32)color << 12);
    w_u16(record, word);
    gtmenu_parts_mark_owned(record);
    price = r_u32(node + 16);
    if (!price) {
        model = r_u32(node + 12);
        price = r_u32(model + 388);
    }
    w_u32(price_slot, price);
    count = r_u16(0x8009B87A);
    w_u16(0x8009B87A, count + 1);
    return 0;
}

sint32 gtmenu_garage_remove(sint32 index)
{
    sint32 count, last;
    uint32 record, price_slot, price;
    FUNCTION_MARKER(0x80011AAC, "GTMENU.EXE");
    count = r_s16(0x8009B87A);
    if (!count) return -1;
    last = count - 1;
    while (index < last) {
        record = 0x8009BA0C + (uint32)index * 96;
        gtmenu_memory_copy(record, record + 96, 96);
        price_slot = 0x8009B87C + (uint32)index * 4;
        price = r_u32(price_slot + 4);
        ++index;
        w_u32(price_slot, price);
    }
    count = r_u16(0x8009B87A);
    w_u16(0x8009B87A, count - 1);
    return 0;
}

uint32 gtmenu_garage_price(sint32 index)
{
    FUNCTION_MARKER(0x80011BBC, "GTMENU.EXE");
    if (index < 0) return 0;
    return r_u32(0x8009B87C + (uint32)index * 4);
}

uint32 gtmenu_car_list_rebuild(sint8 make, sint8 kind)
{
    FUNCTION_MARKER(0x80011BE8, "GTMENU.EXE");
    gtmenu_lists_clear();
    return gtmenu_car_nodes_build(make, kind);
}

void gtmenu_list_build_finish(void)
{
    FUNCTION_MARKER(0x80011C2C, "GTMENU.EXE");
    gtmenu_lists_clear();
    gtmenu_list_build_hook();
}

uint32 gtmenu_car_detail_open(uint32 index)
{
    uint32 state, list, node;
    FUNCTION_MARKER(0x80011CF8, "GTMENU.EXE");
    state = r_u16(0x800A77C4);
    switch (state) {
    case 0x35: case 0x39: case 0x3D: case 0x41: case 0x45:
    case 0x49: case 0x4D: case 0x51: case 0x55: case 0x59:
    case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E:
    case 0x5F: case 0x60: case 0x61: case 0x62: case 0x63:
        gtmenu_sound_effect(5);
        list = gtmenu_primary_list();
        node = gtmenu_list_node_snapshot(list, index);
        state = r_u16(0x800A77C4);
        return gtmenu_dealer_detail_open(node, state);
    case 0x6E:
        gtmenu_sound_effect(5);
        state = r_u16(0x800A77C4);
        return gtmenu_garage_detail_open(index, state);
    default:
        return state - 0x35 < 0x3A ? 0x80011D84 : 0;
    }
}

uint32 gtmenu_car_category_open(uint32 state)
{
    sint8 make;
    FUNCTION_MARKER(0x80011D94, "GTMENU.EXE");
    switch (state) {
    case 0x35: case 0x5A: make = 't'; break;
    case 0x39: case 0x5B: make = 'n'; break;
    case 0x3D: case 0x5C: make = 'h'; break;
    case 0x41: case 0x5D: make = 's'; break;
    case 0x45: case 0x5E: make = 'a'; break;
    case 0x49: case 0x5F: make = 'm'; break;
    case 0x4D: case 0x60: make = 'l'; break;
    case 0x51: case 0x61: make = 'v'; break;
    case 0x55: case 0x62: make = 'd'; break;
    case 0x59: case 0x63: make = 'c'; break;
    case 0x6E: return gtmenu_garage_list_open('g', 2);
    default: return state - 0x35 < 0x3A ? 0x80011E88 : 0;
    }
    return gtmenu_dealer_list_open(make, state >= 0x5A ? 3 : 1);
}

uint32 gtmenu_sound_settings_apply(void)
{
    uint32 level;
    FUNCTION_MARKER(0x80011EC0, "GTMENU.EXE");
    level = r_u8(0x800A2387);
    gtmenu_sound_music_volume_set(level * 0x3FFF / 255);
    level = r_u8(0x800A2388);
    w_u16(0x8007DA8C, 0x3FFF);
    w_u16(0x8007DA8E, level * 0x3FFF / 255);
    return 0;
}

uint32 gtmenu_sound_workspaces_clear(void)
{
    FUNCTION_MARKER(0x80012038, "GTMENU.EXE");
    w_u32(0x80080550, 0);
    gtmenu_memory_fill(0x8007DD50, 0, 10240);
    return gtmenu_memory_fill(0x8007DAA8, 0, 680);
}

uint32 gtmenu_sample_play(uint32 sample, uint32 handle, sint32 note, sint32 volume, sint32 pan)
{
    uint32 pitch, reference, value;
    FUNCTION_MARKER(0x800122EC, "GTMENU.EXE");
    gtmenu_voice_pan(0x8007DA98, volume, pan);
    if (note < 0) {
        pitch = r_u8(sample + 12);
        reference = r_u16(sample + 4);
        pitch = gtmenu_voice_pitch(pitch << 8, reference);
    } else {
        reference = r_u16(sample + 4);
        pitch = gtmenu_voice_pitch((uint32)note << 8, reference);
    }
    w_u16(0x8007DA9C, pitch);
    value = r_u16(sample + 6);
    w_u16(0x8007DAA4, value);
    value = r_u32(sample + 16);
    w_u32(0x8007DAA0, value);
    value = r_u16(sample);
    w_u16(0x8007DA9E, value);
    value = r_u8(sample + 8);
    w_u8(0x8007DAA6, value);
    value = r_u8(sample + 10);
    w_u8(0x8007DAA7, value);
    return gtmenu_voice_start(handle, 0x8007DA98);
}

uint32 gtmenu_sound_effect(sint32 effect)
{
    uint32 master, entry, secondary, gain, archive, bank, sample, base, pan;
    sint32 volume, secondary_volume, note;
    FUNCTION_MARKER(0x800123BC, "GTMENU.EXE");
    master = r_u16(0x8007DA8E);
    entry = 0x80056C28 + (uint32)effect * 4;
    gain = r_u8(entry + 2);
    archive = r_u32(0x80080550);
    volume = (master * gain) >> 7;
    if (effect == 8 || effect == 10) {
        secondary = entry + 4;
        master = r_u16(0x8007DA8E);
        gain = r_u8(secondary + 2);
        bank = r_u32(0x8007DA90);
        sample = r_u8(secondary);
        note = r_s8(secondary + 1);
        base = r_u32(archive + 28);
        pan = r_u8(secondary + 3);
        secondary_volume = (master * gain) >> 7;
        gtmenu_sample_play(base + (sample + bank) * 20, 0, note, secondary_volume, pan);
    }
    sample = r_u8(entry);
    bank = r_u32(0x8007DA90);
    note = r_s8(entry + 1);
    base = r_u32(archive + 28);
    pan = r_u8(entry + 3);
    return gtmenu_sample_play(base + (sample + bank) * 20, 0, note, volume, pan);
}

uint32 gtmenu_sound_bank_load(uint32 name)
{
    uint32 resource, archive, samples, bytes, bank, programs, program, index, offset = 0;
    FUNCTION_MARKER(0x80012080, "GTMENU.EXE");
    resource = gtmenu_menu_asset_load(name);
    archive = gtmenu_sound_bank_relocate(resource, 0x1010);
    samples = r_u32(archive + 20);
    bytes = r_u32(archive + 16);
    gtmenu_sound_samples_upload(0x1010, samples, bytes);
    bytes = r_u32(archive + 8);
    gtmenu_memory_copy(0x8007DD50, resource, bytes);
    archive = gtmenu_sound_bank_relocate(0x8007DD50, 0x1010);
    w_u32(0x80080550, archive);
    gtmenu_heap_free(resource);
    bank = r_u32(0x80080550);
    programs = r_u32(bank + 36);
    for (index = 0; index < 9; ++index) {
        program = r_u32(programs + index * 4);
        offset += r_u8(program);
    }
    w_u32(0x8007DA90, offset);
    return 0;
}

uint32 gtmenu_music_select(sint32 track)
{
    uint32 current, bank, name, sequence;
    FUNCTION_MARKER(0x80012150, "GTMENU.EXE");
    current = r_u32(0x8007DA88);
    bank = r_u32(0x80080550);
    if ((uint32)track == current) return current;
    gtmenu_music_stop();
    name = r_u32(0x80056BF0 + (uint32)track * 4);
    sequence = gtmenu_music_resource(name);
    gtmenu_sequence_relocate(sequence);
    gtmenu_sequence_initialize(0x8007DAA8, sequence + 12, bank);
    w_u32(0x8007DA88, track);
    return gtmenu_music_start();
}

uint32 gtmenu_music_start(void)
{
    uint32 volume;
    FUNCTION_MARKER(0x800121E4, "GTMENU.EXE");
    gtmenu_sequence_start(0x8007DAA8);
    volume = r_u16(0x8007DA8C);
    return gtmenu_sequence_fade(0x8007DAA8, volume, 100);
}

uint32 gtmenu_music_stop(void)
{
    FUNCTION_MARKER(0x80012224, "GTMENU.EXE");
    gtmenu_sequence_fade(0x8007DAA8, 0, 1000);
    return gtmenu_sequence_stop(0x8007DAA8);
}

uint32 gtmenu_music_fade_in(sint32 milliseconds)
{
    uint32 volume;
    FUNCTION_MARKER(0x80012260, "GTMENU.EXE");
    gtmenu_sequence_start(0x8007DAA8);
    volume = r_u16(0x8007DA8C);
    return gtmenu_sequence_fade(0x8007DAA8, volume, milliseconds);
}

uint32 gtmenu_sound_tick_register(void)
{
    FUNCTION_MARKER(0x80011FDC, "GTMENU.EXE");
    w_u32(0x8007DA80, 0x80011E98);
    w_u32(0x8007DA84, 0xFFFFFFFF);
    return gtmenu_frame_callback_insert(0x8007DA78);
}

void gtmenu_sound_tick(void)
{
    FUNCTION_MARKER(0x80011E98, "GTMENU.EXE");
    gtmenu_voices_update();
    gtmenu_sequences_update();
}

uint32 gtmenu_sound_music_volume_set(uint16 volume)
{
    uint32 sequence, result = 0x8009B058;
    FUNCTION_MARKER(0x8002F4EC, "GTMENU.EXE");
    sequence = r_u32(0x8009B058);
    w_u16(0x8009B05E, volume);
    while (sequence) {
        result = gtmenu_sequence_volume_apply(sequence);
        sequence = r_u32(sequence);
    }
    return result;
}

void gtmenu_sequence_rate_set(uint16 rate)
{
    FUNCTION_MARKER(0x8002F5DC, "GTMENU.EXE");
    w_u16(0x8009B05C, rate);
}

uint32 gtmenu_voice_rate_set(sint32 rate)
{
    sint32 step;
    FUNCTION_MARKER(0x8002E878, "GTMENU.EXE");
    step = (sint32)((uint32)rate << 8) / 60;
    w_u16(0x8009AC8C, rate);
    w_u16(0x8009AC8E, step);
    return 0x8009AC88;
}

uint32 gtmenu_credits_get(void)
{
    FUNCTION_MARKER(0x80012524, "GTMENU.EXE");
    return r_u32(0x8009B864);
}

uint32 gtmenu_license_results(sint32 group)
{
    FUNCTION_MARKER(0x80012544, "GTMENU.EXE");
    switch (group) {
    case 1: return 0x8009E3B4;
    case 2: return 0x8009E3BC;
    case 3: return 0x8009E3C4;
    default: return 0;
    }
}

uint32 gtmenu_license_grade(sint32 group)
{
    FUNCTION_MARKER(0x800125A4, "GTMENU.EXE");
    switch (group) {
    case 1: return r_u8(0x8009E3BB);
    case 2: return r_u8(0x8009E3C3);
    case 3: return r_u8(0x8009E3CB);
    default: return 0;
    }
}

uint32 gtmenu_license_tests_complete(sint32 group)
{
    uint32 results = 0, count = 1, index;
    FUNCTION_MARKER(0x8001260C, "GTMENU.EXE");
    switch (group) {
    case 1: results = 0x8009E3B4; count = 7; break;
    case 2: results = 0x8009E3BC; count = 7; break;
    case 3: results = 0x8009E3C4; count = 7; break;
    default: break;
    }
    /* The original default path reads one guest byte at address zero */
    for (index = 0; index < count; ++index) {
        if (!r_s8(results + index)) return 0;
    }
    return 1;
}

uint32 gtmenu_license_entry_unlocked(sint32 entry)
{
    FUNCTION_MARKER(0x800126B4, "GTMENU.EXE");
    switch (entry) {
    case 2: return r_u8(0x8009E3BB) != 0;
    case 3: return r_u8(0x8009E3C3) != 0;
    case 4: return r_u8(0x8009E3CB) != 0;
    default: return 1;
    }
}

uint32 gtmenu_progress_byte(uint32 index)
{
    FUNCTION_MARKER(0x80012718, "GTMENU.EXE");
    return r_u8(0x8009F8CC + index);
}

void gtmenu_progress_selection_set(uint16 selection)
{
    FUNCTION_MARKER(0x8001272C, "GTMENU.EXE");
    w_u16(0x800A240C, selection);
}

sint32 gtmenu_save_record_ids_decode(uint32 data)
{
    sint32 count, index = 0;
    uint32 record, value;
    FUNCTION_MARKER(0x8001274C, "GTMENU.EXE");
    count = r_s16(data + 26);
    if (count <= 0) return count;
    do {
        record = data + (uint32)index * 96 + 428;
        value = r_u16(record);
        w_u16(record, value - 1);
        count = r_s16(data + 26);
        ++index;
    } while (index < count);
    return index * 2;
}

sint32 gtmenu_save_payload_import(uint32 data)
{
    FUNCTION_MARKER(0x800127A0, "GTMENU.EXE");
    gtmenu_memory_copy(0x8009B860, data, 27560);
    return gtmenu_save_record_ids_decode(0x8009B860);
}

uint32 gtmenu_save_payload_export(uint32 data)
{
    sint32 count, index = 0;
    uint32 record, value, checksum;
    FUNCTION_MARKER(0x800127DC, "GTMENU.EXE");
    gtmenu_memory_copy(data, 0x8009B860, 27560);
    count = r_s16(data + 26);
    if (count > 0) {
        do {
            record = data + (uint32)index * 96 + 428;
            value = r_u16(record);
            w_u16(record, value + 1);
            count = r_s16(data + 26);
            ++index;
        } while (index < count);
    }
    checksum = gtend_data_checksum(data, 27556);
    w_u32(data + 27556, checksum);
    return checksum;
}

void gtmenu_resource_archives_clear(void)
{
    FUNCTION_MARKER(0x80012898, "GTMENU.EXE");
    w_u32(0x80080558, 0);
    w_u32(0x8008055C, 0);
}

uint32 gtmenu_resource_archive_load(void)
{
    uint32 archive;
    FUNCTION_MARKER(0x800129AC, "GTMENU.EXE");
    archive = gtmenu_parts_archive_acquire(0);
    w_u32(0x80080558, archive);
    return archive;
}

uint32 gtmenu_resource_archive_release(uint32 archive)
{
    uint32 result;
    FUNCTION_MARKER(0x800129DC, "GTMENU.EXE");
    result = gtmenu_heap_free(archive);
    w_u32(0x80080558, 0);
    return result;
}

uint32 gtmenu_model_index_by_name(uint32 name)
{
    uint32 archive, models, count, index = 0;
    sint32 selected = -1;
    FUNCTION_MARKER(0x80012A58, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    count = r_u16(archive + 14);
    models = archive + 24;
    while (index < count && selected < 0) {
        if (!gtmenu_string_compare(models + index * 424, name)) selected = index;
        count = r_u16(archive + 14);
        ++index;
    }
    return selected < 0 ? 0 : selected;
}

uint32 gtmenu_model_from_record_id(uint32 id)
{
    uint32 archive;
    FUNCTION_MARKER(0x80012B18, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    return archive + 24 + (id & 0xFFF) * 424;
}

uint32 gtmenu_record_has_parts(uint32 record)
{
    FUNCTION_MARKER(0x80012B68, "GTMENU.EXE");
    return r_u16(record + 34) || r_u16(record + 4) || r_u16(record + 6) ||
        r_u16(record + 8) || r_u16(record + 10) || r_u16(record + 14) ||
        r_u16(record + 16) || r_u16(record + 18) || r_u16(record + 20) ||
        r_u16(record + 22) || r_u16(record + 24) || r_u16(record + 26) ||
        r_u16(record + 28) || r_u16(record + 30) || r_u16(record + 32) ||
        r_u16(record + 44) || r_u16(record + 36) || r_u16(record + 38) ||
        r_u16(record + 40) || r_u16(record + 42);
}

uint32 gtmenu_record_matches_filter(uint32 record, uint32 kind, sint32 value)
{
    uint32 archive, id, model, make, cursor, name;
    FUNCTION_MARKER(0x80012CAC, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    id = r_u16(record);
    model = archive + 24 + (id & 0xFFF) * 424;
    switch (kind) {
    case 0:
        make = r_u8(model);
        if (value == 0) {
            return make == 't' || make == 'n' || make == 'a' ||
                make == 'm' || make == 's' || make == 'h';
        }
        if (value == 1) return make - 'c' < 2;
        if (value == 2) return make == 'l' || make == 'v';
        return 0;
    case 1: return value == r_u8(model + 138);
    case 2: return r_u8(model + 417) != 0;
    case 3:
        for (cursor = 0x80058274; ; cursor += 4) {
            name = r_u32(cursor);
            if (!name) return 0;
            if (!gtmenu_string_compare(model, name)) return 1;
        }
    case 4: return r_u16(record + 34) != 0;
    default: return 0;
    }
}

uint32 gtmenu_engine_curve_build(uint32 model, uint32 curve, sint32 scale)
{
    uint32 count, index, torque, rpm, offset;
    sint32 best_power = 0, best_torque = 0, scaled_torque, power, peak;
    FUNCTION_MARKER(0x80012E60, "GTMENU.EXE");
    count = r_u8(model + 86);
    for (index = 0; index < count; ++index) {
        offset = index * 2;
        torque = r_u16(model + offset + 318);
        scaled_torque = (sint32)(torque * (uint32)scale) / 100;
        w_u32(curve + index * 4 + 64, scaled_torque);
        if (best_torque < scaled_torque) {
            best_torque = scaled_torque;
            w_u8(curve + 131, index);
        }
        torque = r_u16(model + offset + 318);
        rpm = r_u8(model + index + 52);
        power = torque * rpm * 10 / 7162;
        w_u32(curve + index * 4, power);
        if (best_power < power) {
            best_power = power;
            w_u8(curve + 130, index);
        }
    }
    peak = r_s8(curve + 131);
    torque = r_u16(model + (uint32)peak * 2 + 318);
    w_u16(curve + 128, torque);
    return curve;
}

static uint32 installed_part_price(uint32 record, uint32 field, uint32 group, uint32 stride, uint32 price_offset)
{
    uint32 archive, id;
    if (!r_u16(record + field)) return 0;
    archive = gtmenu_resource_group(group);
    id = r_u16(record + field);
    return r_u32(archive + id * stride + price_offset);
}

uint32 gtmenu_installed_parts_value(uint32 record)
{
    uint32 total = 0;
    FUNCTION_MARKER(0x80012F78, "GTMENU.EXE");
    total += installed_part_price(record, 34, 30, 32, 8);
    total += installed_part_price(record, 4, 15, 32, 12);
    total += installed_part_price(record, 6, 16, 24, 8);
    total += installed_part_price(record, 8, 17, 28, 12);
    total += installed_part_price(record, 10, 18, 24, 12);
    total += installed_part_price(record, 14, 20, 32, 12);
    total += installed_part_price(record, 16, 21, 32, 12);
    total += installed_part_price(record, 18, 22, 20, 12);
    total += installed_part_price(record, 20, 23, 68, 12);
    total += installed_part_price(record, 22, 24, 32, 12);
    total += installed_part_price(record, 24, 25, 20, 8);
    total += installed_part_price(record, 26, 26, 36, 12);
    total += installed_part_price(record, 28, 27, 36, 8);
    total += installed_part_price(record, 30, 28, 32, 12);
    total += installed_part_price(record, 32, 29, 20, 12);
    total += installed_part_price(record, 44, 35, 56, 12);
    total += installed_part_price(record, 36, 31, 20, 12);
    total += installed_part_price(record, 38, 32, 20, 12);
    total += installed_part_price(record, 40, 33, 72, 12);
    total += installed_part_price(record, 42, 34, 20, 12);
    return total;
}

uint32 gtmenu_record_matches_make(uint32 record, sint32 make_index)
{
    uint32 archive, id, model;
    sint32 make;
    FUNCTION_MARKER(0x80013954, "GTMENU.EXE");
    archive = gtmenu_resource_group(13);
    id = r_u16(record);
    model = archive + 24 + (id & 0xFFF) * 424;
    make = r_s8(model);
    switch (make) {
    case 't': return make_index == 1;
    case 'n': return make_index == 2;
    case 'a': return make_index == 3;
    case 'm': return make_index == 5;
    case 'h': return make_index == 6;
    case 's': return make_index == 4;
    case 'l': return make_index == 7;
    case 'd': return make_index == 10;
    case 'v': return make_index == 9;
    case 'c': return make_index == 8;
    default: return 0;
    }
}

uint32 gtmenu_catalog_price(sint32 index)
{
    uint32 catalog;
    FUNCTION_MARKER(0x8001406C, "GTMENU.EXE");
    catalog = r_u32(0x800805D0);
    return r_u32(catalog + (uint32)index * 4 + 540);
}

uint32 gtmenu_catalog_choice_prepare(void)
{
    sint32 index;
    uint32 catalog, record, parts, price;
    FUNCTION_MARKER(0x80013AD4, "GTMENU.EXE");
    index = r_s16(0x80080562);
    catalog = r_u32(0x800805D0);
    record = catalog + (uint32)index * 96 + 940;
    w_u32(0x8008057C, record);
    gtmenu_record_display_name(record, 0x80080580);
    parts = gtmenu_installed_parts_value(record);
    w_u32(0x800805C0, parts);
    w_u32(0x800805C4, (sint32)parts >> 31);
    price = gtmenu_catalog_price(index);
    w_u32(0x800805C8, price);
    w_u32(0x800805CC, 0);
    return price;
}

uint32 gtmenu_catalog_purchase_commit(void)
{
    sint32 count;
    uint32 credits, parts_low, parts_high, price_low, price_high, record, source;
    uint64 wallet, cost;
    sint64 remaining, balance;
    FUNCTION_MARKER(0x80013B68, "GTMENU.EXE");
    count = r_s16(0x8009B87A);
    if (count >= 100) return 7;
    credits = gtmenu_credits_get();
    wallet = (sint64)(sint32)credits * 100;
    parts_low = r_u32(0x800805C0);
    parts_high = r_u32(0x800805C4);
    price_high = r_u32(0x800805CC);
    price_low = r_u32(0x800805C8);
    cost = ((uint64)parts_high << 32) | parts_low;
    cost += ((uint64)price_high << 32) | price_low;
    if ((sint64)wallet < (sint64)cost) return 6;
    remaining = (sint64)(wallet - cost);
    balance = gtmenu_signed_divide64(remaining, 100, 0x80013C18);
    gtmenu_credits_set((uint32)balance);
    record = 0x8009BA0C + (uint32)count * 96;
    price_low = r_u32(0x800805C8);
    w_u32(0x8009B87C + (uint32)count * 4, price_low);
    source = r_u32(0x8008057C);
    gtmenu_memory_copy(record, source, 96);
    source = r_u32(0x8008057C);
    gtmenu_catalog_parts_mark_owned(source);
    count = r_u16(0x8009B87A);
    w_u16(0x8009B87A, count + 1);
    return 5;
}

uint32 gtmenu_owned_parts_category(uint32 inventory, uint32 category)
{
    uint32 offset;
    FUNCTION_MARKER(0x80031EAC, "GTMENU.EXE");
    offset = r_u16(0x80071172 + category * 2);
    return inventory + offset;
}

static uint32 mark_installed_part_owned(uint32 record, uint32 field, uint32 category)
{
    uint32 flags, value, bit;
    sint32 id;
    if (!r_u16(record + field)) return 0;
    flags = gtmenu_owned_parts_category(0x8009DF8C, category);
    id = r_u16(record + field);
    --id;
    flags += (uint32)(id >> 3);
    bit = (uint32)id & 7;
    value = r_u8(flags);
    w_u8(flags, value | (1u << bit));
    return flags;
}

uint32 gtmenu_catalog_parts_mark_owned(uint32 record)
{
    FUNCTION_MARKER(0x8001341C, "GTMENU.EXE");
    mark_installed_part_owned(record, 34, 30);
    mark_installed_part_owned(record, 4, 15);
    mark_installed_part_owned(record, 6, 16);
    mark_installed_part_owned(record, 10, 18);
    mark_installed_part_owned(record, 14, 20);
    mark_installed_part_owned(record, 16, 21);
    mark_installed_part_owned(record, 18, 22);
    mark_installed_part_owned(record, 20, 23);
    mark_installed_part_owned(record, 22, 24);
    mark_installed_part_owned(record, 24, 25);
    mark_installed_part_owned(record, 26, 26);
    mark_installed_part_owned(record, 28, 27);
    mark_installed_part_owned(record, 30, 28);
    mark_installed_part_owned(record, 32, 29);
    mark_installed_part_owned(record, 44, 35);
    mark_installed_part_owned(record, 36, 31);
    mark_installed_part_owned(record, 38, 32);
    mark_installed_part_owned(record, 40, 33);
    return mark_installed_part_owned(record, 42, 34);
}

uint32 gtmenu_parts_mark_owned(uint32 record)
{
    FUNCTION_MARKER(0x8001550C, "GTMENU.EXE");
    mark_installed_part_owned(record, 34, 30);
    mark_installed_part_owned(record, 4, 15);
    mark_installed_part_owned(record, 6, 16);
    mark_installed_part_owned(record, 8, 17);
    mark_installed_part_owned(record, 10, 18);
    mark_installed_part_owned(record, 14, 20);
    mark_installed_part_owned(record, 16, 21);
    mark_installed_part_owned(record, 18, 22);
    mark_installed_part_owned(record, 20, 23);
    mark_installed_part_owned(record, 22, 24);
    mark_installed_part_owned(record, 24, 25);
    mark_installed_part_owned(record, 26, 26);
    mark_installed_part_owned(record, 28, 27);
    mark_installed_part_owned(record, 30, 28);
    mark_installed_part_owned(record, 32, 29);
    mark_installed_part_owned(record, 44, 35);
    mark_installed_part_owned(record, 36, 31);
    mark_installed_part_owned(record, 38, 32);
    mark_installed_part_owned(record, 40, 33);
    return mark_installed_part_owned(record, 42, 34);
}

uint32 gtmenu_catalog_caption_draw(uint32 ordering)
{
    uint32 parts_low, parts_high, price_low, price_high, result, rectangle, grouped, decimal;
    sint32 width, x, y = 158;
    uint64 cost;
    sint64 amount;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80013CB8, "GTMENU.EXE");
    parts_low = r_u32(0x800805C0);
    parts_high = r_u32(0x800805C4);
    price_low = r_u32(0x800805C8);
    price_high = r_u32(0x800805CC);
    cost = ((uint64)parts_high << 32) | parts_low;
    cost += ((uint64)price_high << 32) | price_low;
    width = gtmenu_text_width(0, 0x80080580);
    x = (sint32)(640u - (uint32)width) / 2;
    if (r_s16(0x80080564) == 4) y = 172;
    gtmenu_text_color_set(0x7F7F7F);
    gtmenu_text_font_set(0);
    gtmenu_text_draw(ordering, x, y, 0x80080580);
    frame = gtmenu_local_stack_push(72, 0x80013CB8);
    rectangle = frame.address;
    grouped = frame.address + 8;
    decimal = frame.address + 40;
    w_u16(rectangle, 0);
    w_u16(rectangle + 2, y);
    w_u16(rectangle + 4, 638);
    w_u16(rectangle + 6, y + 23);
    gtmenu_rectangle_draw(ordering, rectangle, 0);
    result = 4;
    if (r_s16(0x80080564) == 4) {
        amount = gtmenu_signed_divide64((sint64)cost, 100, 0x80013DCC);
        gtmenu_format_signed64(decimal, 0x8004FD20, amount);
        gtmenu_number_group(grouped, decimal);
        gtmenu_text_font_set(1);
        gtmenu_text_draw(ordering, 320, 205, grouped);
        w_u16(rectangle, 314);
        w_u16(rectangle + 2, 195);
        w_u16(rectangle + 4, 470);
        w_u16(rectangle + 6, 231);
        result = gtmenu_rectangle_draw(ordering, rectangle, 0);
    }
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_catalog_input(uint32 event)
{
    uint32 widget, kind, buttons, list, result;
    sint32 direction, selection, count;
    FUNCTION_MARKER(0x80013E84, "GTMENU.EXE");
    widget = gtmenu_ui_active_list(0);
    kind = r_u32(event);
    if (kind != 24 && kind != 26) return 26;
    buttons = r_u32(event + 12);
    direction = -(sint32)((buttons & 0x10) != 0);
    if (buttons & 0x40) direction = 1;
    if (direction) {
        selection = r_s16(widget + 20);
        list = r_u32(widget + 16);
        selection += direction;
        count = r_s32(list + 8);
        if (selection < 0 || selection >= count) return gtmenu_sound_effect(6);
        result = gtmenu_sound_effect(0);
        w_u16(widget + 20, selection);
        w_u16(0x80080562, selection);
        return result;
    }
    buttons = r_u32(event + 12);
    if (buttons & 0x9000) {
        gtmenu_sound_effect(3);
        return gtmenu_menu_back();
    }
    buttons = r_u32(event + 12);
    result = buttons & 0x6000;
    if (result) {
        gtmenu_sound_effect(5);
        gtmenu_catalog_choice_prepare();
        return gtmenu_menu_open(4);
    }
    return result;
}

uint32 gtmenu_catalog_list_create(uint32 output, uint32 rectangle, uint32 preserve_position)
{
    uint32 first, second, list, widget;
    FUNCTION_MARKER(0x80013F9C, "GTMENU.EXE");
    if (!preserve_position) {
        first = r_u32(rectangle);
        second = r_u32(rectangle + 4);
        w_u32(0x800805D8, first);
        w_u32(0x800805DC, second);
    }
    list = gtmenu_primary_list();
    widget = gtmenu_ui_list_create(rectangle, list, 2);
    gtmenu_ui_active_list_set(widget, 0);
    w_u32(output, widget);
    return output;
}

void gtmenu_ui_active_list_set(uint32 node, uint32 secondary)
{
    FUNCTION_MARKER(0x8001A8FC, "GTMENU.EXE");
    if (secondary) w_u32(0x80058444, node);
    else w_u32(0x80058440, node);
}

uint32 gtmenu_ui_active_list(uint32 secondary)
{
    FUNCTION_MARKER(0x8001A920, "GTMENU.EXE");
    if (secondary) return r_u32(0x80058444);
    return r_u32(0x80058440);
}

uint32 gtmenu_rectangle_draw(uint32 ordering, uint32 rectangle, uint32 color)
{
    uint32 first, second, packet, previous, address, bytes, shift, index, link;
    sint32 x0, y0, x1, y1;
    FUNCTION_MARKER(0x80031EC4, "GTMENU.EXE");
    first = r_u32(rectangle);
    second = r_u32(rectangle + 4);
    x0 = (sint16)first;
    y0 = (sint16)(first >> 16);
    x1 = (sint16)second;
    y1 = (sint16)(second >> 16);
    if (y1 < y0) ++y0;
    else ++y1;
    packet = r_u32(0x800A8434);
    w_u32(packet, 0x05000000);
    if (x1 < x0) ++x0;
    else ++x1;
    w_u32(packet + 4, color ^ 0x29000000);
    w_u16(packet + 8, x0);
    w_u16(packet + 10, y0);
    w_u16(packet + 12, x1);
    w_u16(packet + 14, y0);
    w_u16(packet + 16, x0);
    w_u16(packet + 18, y1);
    w_u16(packet + 20, x1);
    w_u16(packet + 22, y1);
    /* Preserve the original partial tag accesses for every input alignment */
    address = ordering + 2;
    bytes = (address & 3) + 1;
    address &= ~3u;
    shift = (4 - bytes) * 8;
    previous = shift ? (uint32)x1 & ((1u << shift) - 1) : 0;
    for (index = 0; index < bytes; ++index)
        previous |= (uint32)r_u8(address + index) << (shift + index * 8);
    link = packet << 8;
    for (index = 0; index < bytes; ++index)
        w_u8(address + index, link >> (shift + index * 8));
    address = packet + 2;
    bytes = (address & 3) + 1;
    address &= ~3u;
    shift = (4 - bytes) * 8;
    for (index = 0; index < bytes; ++index)
        w_u8(address + index, previous >> (shift + index * 8));
    w_u32(0x800A8434, packet + 24);
    return packet;
}

uint32 gtmenu_catalog_selection_draw(uint32 ordering, uint32 selection)
{
    FUNCTION_MARKER(0x80014088, "GTMENU.EXE");
    gtmenu_rectangle_pulse(ordering, 0x8008056C + selection * 8, 0x80056CF4);
    return gtmenu_rectangle_shade(ordering, 0x8008056C + (selection ^ 1) * 8, 128);
}

uint32 gtmenu_catalog_buttons_draw(uint32 ordering)
{
    FUNCTION_MARKER(0x800140F0, "GTMENU.EXE");
    gtmenu_rectangle_shade(ordering, 0x8008056C, 255);
    return gtmenu_rectangle_shade(ordering, 0x80080574, 255);
}

uint32 gtmenu_catalog_footer_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x80014134, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(8, 0x80014134);
    w_u16(frame.address + 2, 350);
    w_u16(frame.address + 4, 640);
    w_u16(frame.address, 0);
    w_u16(frame.address + 6, 366);
    gtmenu_text_centered(ordering, frame.address, 0x707070, 0x800A6C74);
    result = gtmenu_catalog_buttons_draw(ordering);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_loading_banner_draw(uint32 ordering)
{
    sint32 scroll, brightness, x;
    uint32 phase, index, color;
    FUNCTION_MARKER(0x80014190, "GTMENU.EXE");
    scroll = r_s16(0x80056CF8) + 1;
    if (scroll >= 160) scroll -= 160;
    phase = r_u16(0x80056CFA);
    w_u16(0x80056CF8, scroll);
    phase += 8;
    brightness = (255 - (sint32)(phase & 0x1FF)) >> 1;
    w_u16(0x80056CFA, phase);
    if (brightness < 0) brightness = -brightness;
    gtmenu_draw_area(ordering, 0, 0, 0x77E7F);
    gtmenu_text_font_set(1);
    color = (uint8)brightness;
    gtmenu_text_color_set(color | (color << 8) | 0x800000);
    x = 72 - scroll;
    for (index = 0; index < 3; ++index) {
        gtmenu_text_draw(ordering, x, 272, 0x8004FD24);
        x += 160;
    }
    return gtmenu_draw_area(ordering, 0, 0x36048, 0x5415D);
}

uint32 gtmenu_catalog_mode_draw(uint32 ordering)
{
    sint32 mode;
    FUNCTION_MARKER(0x800142A0, "GTMENU.EXE");
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    mode = r_s16(0x80080564);
    if ((uint32)mode >= 15) return 0;
    switch (mode) {
    case 1:
        return gtmenu_catalog_footer_draw(ordering);
    case 2:
        return gtmenu_loading_banner_draw(ordering);
    case 4:
    case 5:
        gtmenu_catalog_caption_draw(ordering);
        break;
    case 3:
    case 7:
    case 8:
        /* These table entries return their loaded branch target */
        return 0x80014334;
    default:
        break;
    }
    return gtmenu_catalog_selection_draw(ordering, (uint32)r_s16(0x80080562));
}

uint32 gtmenu_catalog_draw_event(uint32 event)
{
    sint32 countdown;
    FUNCTION_MARKER(0x80014344, "GTMENU.EXE");
    if (r_u32(event) != 3) return 3;
    countdown = r_s16(0x80080560);
    if (countdown) w_u16(0x80080560, countdown - 1);
    gtmenu_catalog_mode_draw(r_u32(event + 4));
    return gtmenu_scene_draw(r_u32(event + 4));
}

uint32 gtmenu_catalog_selection_move(uint32 delta)
{
    uint32 selection;
    FUNCTION_MARKER(0x80014658, "GTMENU.EXE");
    selection = (uint32)r_s16(0x80080562) + delta;
    if (selection >= 2) return gtmenu_sound_effect(6);
    gtmenu_sound_effect(0);
    w_u16(0x80080562, selection);
    w_u8(0x80056CF5, 100);
    return 100;
}

uint32 gtmenu_rectangle_shade(uint32 ordering, uint32 rectangle, uint8 intensity)
{
    uint32 color;
    FUNCTION_MARKER(0x8001B810, "GTMENU.EXE");
    color = intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16) | 0x02000000;
    gtmenu_rectangle_draw(ordering, rectangle, color);
    return gtmenu_texture_page_draw(ordering, 64);
}

uint32 gtmenu_rectangle_pulse(uint32 ordering, uint32 rectangle, uint32 animation)
{
    sint32 direction, step, delta;
    uint32 intensity, threshold;
    FUNCTION_MARKER(0x8001B85C, "GTMENU.EXE");
    if (!animation) animation = 0x80081F80;
    if (!r_u8(animation)) {
        w_u8(animation, 3);
        w_u16(animation + 2, 0xFFFF);
        w_u8(animation + 1, 100);
    }
    direction = r_s16(animation + 2);
    step = r_u8(animation);
    delta = direction * step;
    gtmenu_rectangle_shade(ordering, rectangle, r_u8(animation + 1));
    intensity = r_u8(animation + 1);
    w_u8(animation + 1, intensity + (uint32)delta);
    if (r_u8(animation + 1) >= 101) {
        w_u16(animation + 2, 0xFFFF);
        w_u8(animation + 1, 100);
    }
    intensity = r_u8(animation + 1);
    threshold = r_u8(animation);
    if (intensity < threshold) {
        w_u16(animation + 2, 1);
        w_u8(animation + 1, 0);
    }
    return 1;
}

uint32 gtmenu_text_centered(uint32 ordering, uint32 rectangle, uint32 color, uint32 text)
{
    sint32 x0, y0, x1, y1, width, offset, x, y;
    FUNCTION_MARKER(0x800186DC, "GTMENU.EXE");
    x1 = r_s16(rectangle + 4);
    y1 = r_s16(rectangle + 6);
    x0 = r_s16(rectangle);
    y0 = r_s16(rectangle + 2);
    gtmenu_text_font_set(0);
    gtmenu_text_color_set(color);
    width = gtmenu_text_width(0, text);
    offset = (sint32)((uint32)(x1 - x0) - (uint32)width) >> 1;
    x = (sint32)((uint32)x0 + (uint32)offset);
    y = y0 + ((y1 - y0 - 16) >> 1);
    return gtmenu_text_draw(ordering, x, (sint16)y, text);
}

uint32 gtmenu_input_dispatch(uint32 event, GTMenuMoveCallback move,
    GTMenuActionCallback accept, GTMenuActionCallback cancel)
{
    uint32 buttons;
    FUNCTION_MARKER(0x80014530, "GTMENU.EXE");
    if (r_u32(event) != 24) return 24;
    if (r_u32(event + 12) & 0x80) return move(0xFFFFFFFF);
    if (r_u32(event + 12) & 0x20) return move(1);
    if (r_u32(event + 12) & 0x9000) {
        gtmenu_sound_effect(3);
        return cancel();
    }
    buttons = r_u32(event + 12) & 0x6000;
    if (buttons) {
        gtmenu_sound_effect(5);
        return accept();
    }
    return buttons;
}

uint32 gtmenu_input_acknowledge(uint32 event, GTMenuActionCallback action)
{
    uint32 buttons;
    FUNCTION_MARKER(0x80014600, "GTMENU.EXE");
    if (r_u32(event) != 24) return 24;
    buttons = r_u32(event + 12) & 0xF000;
    if (buttons) {
        gtmenu_sound_effect(5);
        return action();
    }
    return buttons;
}

uint32 gtmenu_catalog_purchase_accept(void)
{
    uint32 state;
    FUNCTION_MARKER(0x80014744, "GTMENU.EXE");
    if (r_s16(0x80080562)) state = 3;
    else state = gtmenu_catalog_purchase_commit();
    return gtmenu_menu_open(state);
}

uint32 gtmenu_catalog_purchase_cancel(void)
{
    FUNCTION_MARKER(0x80014784, "GTMENU.EXE");
    return gtmenu_menu_open(3);
}

uint32 gtmenu_catalog_return_accept(void)
{
    FUNCTION_MARKER(0x800147A4, "GTMENU.EXE");
    if (r_s16(0x80080562)) return gtmenu_menu_back();
    return gtmenu_menu_open(3);
}

uint32 gtmenu_catalog_message_input(uint32 event)
{
    FUNCTION_MARKER(0x800147E0, "GTMENU.EXE");
    return gtmenu_input_acknowledge(event, gtmenu_menu_back);
}

uint32 gtmenu_part_stage(sint16 index)
{
    uint32 group;
    FUNCTION_MARKER(0x80014B0C, "GTMENU.EXE");
    group = gtmenu_resource_group(25);
    if (index < 0) return 0;
    return r_u8(group + 24 + (uint32)index * 20 + 1) + 1;
}

sint32 gtmenu_part_install(uint32 node)
{
    uint32 record, category, field, target, index, bit, value, color;
    sint32 current_stage, requested_stage, installed = 0;
    FUNCTION_MARKER(0x80014B64, "GTMENU.EXE");
    record = gtmenu_record_address(gtmenu_current_record_index());
    category = r_u8(node + 9);
    switch (category) {
    case 15: field = 4; break;
    case 21: field = 16; break;
    case 28: field = 30; break;
    case 25:
        index = r_u16(record + 24);
        target = r_u32(node + 12);
        requested_stage = r_u8(target + 1);
        current_stage = (sint32)gtmenu_part_stage((sint16)(index - 1)) - 1;
        if (r_u16(record + 34)) return -5;
        if (r_u16(record + 24)) {
            if (requested_stage == 2 && !current_stage) return -3;
            if (current_stage >= requested_stage) return -1;
        } else {
            if (requested_stage == 2) return -3;
            if (requested_stage == 1) return -2;
        }
        field = 24;
        break;
    case 30:
        if (r_u16(record + 34)) return -1;
        w_u16(record + 34, r_u16(node + 10) + 1);
        color = (uint32)gtmenu_selected_color();
        value = r_u16(record);
        w_u16(record + 24, 0);
        w_u16(record, (value & 0xFFF) | (color << 12));
        category = r_u8(node + 9);
        target = r_u32(node + 12);
        gtmenu_part_settings_apply(record, category, target);
        installed = 1;
        goto mark_owned;
    default:
        target = gtmenu_owned_parts_category(0x8009DF8C, r_u8(node + 9));
        index = r_u16(node + 10);
        target += index >> 3;
        bit = r_u16(node + 10) & 7;
        value = r_u8(target);
        if ((value >> bit) & 1) return -1;
        goto mark_owned;
    }
    if (category != 25 && r_u16(record + field)) return -1;
    w_u16(record + field, r_u16(node + 10) + 1);
    category = r_u8(node + 9);
    target = r_u32(node + 12);
    gtmenu_part_settings_apply(record, category, target);
    installed = 1;
mark_owned:
    target = gtmenu_owned_parts_category(0x8009DF8C, r_u8(node + 9));
    index = r_u16(node + 10);
    bit = r_u16(node + 10) & 7;
    target += index >> 3;
    value = r_u8(target);
    w_u8(target, value | (1u << bit));
    return installed;
}

uint32 gtmenu_part_adjustment_value(uint32 node)
{
    uint32 offset;
    FUNCTION_MARKER(0x80015424, "GTMENU.EXE");
    switch (r_u8(node + 9)) {
    case 15: case 20: case 21: case 24: case 27: case 28:
        offset = 20; break;
    case 16: case 22: case 29: case 31: case 32: case 34:
        offset = 8; break;
    case 17: case 30: offset = 16; break;
    case 18: offset = 12; break;
    case 23: offset = 56; break;
    case 25: offset = 4; break;
    case 26: offset = 24; break;
    case 33: offset = 60; break;
    case 35: offset = 44; break;
    default: return 0;
    }
    return r_u32(r_u32(node + 12) + offset);
}

uint32 gtmenu_category_name(sint32 category)
{
    uint32 index = 0, offset;
    FUNCTION_MARKER(0x80015AC0, "GTMENU.EXE");
    if (r_s16(0x80056D00) < 0) return 0;
    for (;;) {
        offset = index * 8;
        if (r_s16(0x80056D00 + offset) == category)
            return r_u32(0x80056CFC + offset);
        ++index;
        if (r_s16(0x80056D00 + index * 8) < 0) return 0;
    }
}

uint32 gtmenu_category_kind(sint32 category)
{
    uint32 index = 0;
    FUNCTION_MARKER(0x80015B30, "GTMENU.EXE");
    while (r_s16(0x80056D00 + index * 8) != category) ++index;
    return r_u8(0x80056D02 + index * 8);
}

uint32 gtmenu_category_style(sint32 category)
{
    uint32 index = 0;
    FUNCTION_MARKER(0x80015B6C, "GTMENU.EXE");
    while (r_s16(0x80056D00 + index * 8) != category) ++index;
    return r_u8(0x80056D03 + index * 8);
}

uint32 gtmenu_part_caption(uint32 node, uint32 short_caption)
{
    uint32 category, group, record, models, model_base, packed, names;
    uint32 first, second, text = 0x8004FE34, prefix = 0, table = 0;
    uint32 stride = 0, offset = 0, index, item, half_index = 0;
    FUNCTION_MARKER(0x80014DC0, "GTMENU.EXE");
    category = r_u8(node + 9);
    group = gtmenu_resource_group(category);
    record = gtmenu_record_address(gtmenu_current_record_index());
    models = gtmenu_resource_group(13);
    model_base = models + 24;
    index = r_u16(record) & 0xFFF;
    packed = r_u32(model_base + index * 424 + 392);
    names = r_u32(models + 16);
    names = r_u32(names + (packed >> 16) * 4 + 4);
    first = gtmenu_packed_text_entry(names, packed & 0xFFFF) + 1;
    index = r_u16(record) & 0xFFF;
    packed = r_u32(model_base + index * 424 + 396);
    names = r_u32(models + 16);
    names = r_u32(names + (packed >> 16) * 4 + 4);
    second = gtmenu_packed_text_entry(names, packed & 0xFFFF) + 1;
    switch (category) {
    case 15: text = 0x800A6C00; break;
    case 16: prefix = 0x800A6D2B; text = 0x800A72EE; break;
    case 17: text = 0x800A6C1D; break;
    case 18:
        table = 0x800584D4; stride = 24; offset = 33; prefix = 0x800A6D42;
        break;
    case 20: text = 0x800A735D; break;
    case 21: text = 0x800A6C35; break;
    case 22: table = 0x800584E4; stride = 20; offset = 31; break;
    case 23:
        table = 0x800584F0; stride = 68; offset = 76; prefix = 0x800A744E;
        half_index = 1;
        break;
    case 24:
        table = 0x800584FC; stride = 32; offset = 43; prefix = 0x800A7312;
        break;
    case 25: table = 0x80058504; stride = 20; offset = 40; break;
    case 26:
        table = 0x80058510; stride = 36; offset = 46; prefix = 0x800A72DA;
        break;
    case 27:
        table = 0x8005851C; stride = 36; offset = 56; prefix = 0x800A6D33;
        break;
    case 28: text = 0x800A6C57; break;
    case 29:
        table = 0x80058528; stride = 20; offset = 31; prefix = 0x800A72BE;
        break;
    case 30: table = 0x8005852C; stride = 32; offset = 52; break;
    case 31:
        table = 0x80058530; stride = 20; offset = 31; prefix = 0x800A736F;
        break;
    case 32:
        table = 0x80058530; stride = 20; offset = 31; prefix = 0x800A7383;
        break;
    case 33:
        table = 0x8005853C; stride = 72; offset = 81; prefix = 0x800A6D77;
        break;
    case 34:
        table = 0x80058558; stride = 20; offset = 31; prefix = 0x800A6DAF;
        break;
    case 35:
        table = 0x80058548; stride = 56; offset = 54; prefix = 0x800A6D68;
        break;
    default: break;
    }
    if (table) {
        index = r_u16(node + 10);
        item = group + index * stride + offset;
        if (half_index) index = r_u16(item);
        else index = r_u8(item);
        text = r_u32(table + index * 4);
    }
    if (prefix) gtmenu_format_strings2(0x80080668, 0x8004FE3C, prefix, text);
    else gtmenu_string_copy(0x80080668, text);
    if (short_caption) gtmenu_string_copy(0x800805E8, 0x80080668);
    else gtmenu_format_strings3(0x800805E8, 0x8004FE44, 0x80080668, first, second);
    return 0x800805E8;
}

uint32 gtmenu_record_special_parts_mark(uint32 record)
{
    FUNCTION_MARKER(0x80015A88, "GTMENU.EXE");
    if (!gtmenu_record_is_special_model(record)) return 0;
    gtmenu_parts_mark_owned(record);
    return 1;
}

uint32 gtmenu_record_is_special_model(uint32 record)
{
    uint32 model, index = 0, name;
    FUNCTION_MARKER(0x8001D704, "GTMENU.EXE");
    model = gtmenu_model_from_record_id(r_u16(record) & 0xFFF);
    for (;;) {
        name = r_u32(0x80058578 + index * 4);
        if (!name) return 0;
        if (!gtmenu_string_compare(model, name)) return 1;
        ++index;
    }
}

uint32 gtmenu_packed_text_entry(uint32 table, sint32 index)
{
    sint32 current = 0;
    FUNCTION_MARKER(0x80031FBC, "GTMENU.EXE");
    table += 2;
    while (current < index) {
        table += r_u8(table) + 2;
        ++current;
    }
    return table;
}

sint32 gtmenu_page_category_set(uint32 page)
{
    uint32 first, entry, index, offset, previous, value;
    sint32 category = 664;
    FUNCTION_MARKER(0x80015BA8, "GTMENU.EXE");
    first = r_u32(0x80056CFC);
    if (!page) return 664;
    if (!gtmenu_string_compare_folded(page, 0x800533B4)) category = 655;
    else if (!gtmenu_string_compare_folded(page, 0x800533C0)) category = 656;
    else if (!gtmenu_string_compare(page, 0x800A7704)) category = 657;
    else if (!gtmenu_string_compare_folded(page, 0x800533D0) ||
             !gtmenu_string_compare_folded(page, 0x800533E4) ||
             !gtmenu_string_compare_folded(page, 0x800533F8) ||
             !gtmenu_string_compare_folded(page, 0x8005340C) ||
             !gtmenu_string_compare_folded(page, 0x8005341C) ||
             !gtmenu_string_compare_folded(page, 0x8005342C)) category = 658;
    else if (!gtmenu_string_compare(page, 0x800A7704)) category = 657;
    else if (!gtmenu_string_compare_folded(page, 0x8005343C) ||
             !gtmenu_string_compare_folded(page, 0x8005344C) ||
             !gtmenu_string_compare_folded(page, 0x80053460) ||
             !gtmenu_string_compare_folded(page, 0x80053474) ||
             !gtmenu_string_compare_folded(page, 0x8005348C) ||
             !gtmenu_string_compare_folded(page, 0x800534A0) ||
             !gtmenu_string_compare_folded(page, 0x800533D0)) category = 658;
    else if (!first) category = 0;
    else {
        index = 0;
        for (;;) {
            offset = index * 8;
            entry = r_u32(0x80056CFC + offset);
            if (!gtmenu_string_compare_folded(page, entry)) {
                category = r_s16(0x80056D00 + offset);
                if (!entry) category = 0;
                break;
            }
            ++index;
            if (!entry) {
                category = 0;
                break;
            }
        }
    }
    previous = r_u16(0x800A77C4);
    w_u16(0x800A77C4, category);
    w_u16(0x800A77C0, previous);
    value = gtmenu_category_kind(category);
    w_u16(0x800A77C6, value);
    value = gtmenu_category_style(category);
    w_u16(0x800A77C2, value);
    return category;
}

uint32 gtmenu_model_info_page_open(sint32 category)
{
    uint32 page;
    FUNCTION_MARKER(0x80015E48, "GTMENU.EXE");
    switch (category) {
    case 50: case 53: page = 0x80053058; break;
    case 51: case 52: page = 0x800534B4; break;
    case 54: case 57: page = 0x80053018; break;
    case 55: case 56: page = 0x800534E0; break;
    case 58: case 61: page = 0x80052FD8; break;
    case 59: case 60: page = 0x8005350C; break;
    case 62: case 65: page = 0x80052F98; break;
    case 63: case 64: page = 0x80053534; break;
    case 66: case 69: page = 0x80052F58; break;
    case 67: case 68: page = 0x80053560; break;
    case 70: case 73: page = 0x80052F08; break;
    case 71: case 72: page = 0x80053588; break;
    case 74: case 77: page = 0x80052EC8; break;
    case 75: case 76: page = 0x800535CC; break;
    case 78: case 81: page = 0x80052E94; break;
    case 79: case 80: page = 0x800535F4; break;
    case 82: case 85: page = 0x80052E44; break;
    case 83: case 84: page = 0x8005361C; break;
    case 86: case 89: page = 0x80052DF4; break;
    case 87: case 88: page = 0x8005364C; break;
    case 90: page = 0x800534C8; break;
    case 91: page = 0x800534F4; break;
    case 92: page = 0x80053520; break;
    case 93: page = 0x80053548; break;
    case 94: page = 0x80053574; break;
    case 95: page = 0x800535A0; break;
    case 96: page = 0x800535E0; break;
    case 97: page = 0x80053608; break;
    case 98: page = 0x80053634; break;
    case 99: page = 0x80053664; break;
    case 110: page = 0x800535BC; break;
    default: page = gtmenu_category_name(category); break;
    }
    return gtmenu_page_open(page);
}

uint32 gtmenu_model_info_draw(void)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 ordering, descriptor, model, units, use_units, text, rectangle, counter, result;
    sint32 x, value;
    sint64 price;
    FUNCTION_MARKER(0x80016010, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(152, 0x80016010);
    ordering = gtmenu_current_ordering();
    use_units = r_u8(0x8009B85D);
    descriptor = r_u32(0x80058164);
    model = r_u32(descriptor + 12);
    if (use_units) units = r_u32(0x80058610);
    else units = r_u32(0x80058614);
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(0);
    if (!r_u16(0x80080BBA)) {
        text = 0x80080798 + (uint32)gtmenu_model_info_selection() * 64;
        if (!text) text = 0x80053788;
        gtmenu_text_draw(ordering, 128, 98, text);
    }
    descriptor = r_u32(0x80058164);
    if (r_u8(descriptor + 21)) {
        rectangle = frame.address + 128;
        w_u16(rectangle, 521);
        w_u16(rectangle + 2, 298);
        w_u16(rectangle + 4, 612);
        w_u16(rectangle + 6, 389);
        gtmenu_rectangle_shade(ordering, rectangle, 96);
    } else if (r_s8(0x8005815E) != 2) {
        if (r_s32(0x80058180) < 27) gtmenu_text_color_set(128);
        else gtmenu_text_color_set(32896);
        gtmenu_text_font_set(0);
        gtmenu_text_draw(ordering, 108, 276, 0x800A6EB7);
        x = (sint32)((uint32)gtmenu_text_width(0, 0x800A6EB7) + 124);
        descriptor = r_u32(0x80058164);
        if (!r_u32(descriptor + 16)) price = r_u32(model + 388);
        else {
            descriptor = r_u32(0x80058164);
            price = r_s32(descriptor + 16);
        }
        gtmenu_price_draw(ordering, x, 276, 0, price);
    }
    gtmenu_text_color_set(0x808080);
    gtmenu_text_font_set(1);
    text = r_u32(0x8005816C + (uint32)r_u8(model + 138) * 4);
    gtmenu_format_strings2(frame.address, 0x80053790, 0x800A6EBF, text);
    gtmenu_text_draw(ordering, 108, 306, frame.address);
    if (r_u8(0x8005815E) < 2) value = r_u16(model + 408);
    else value = r_u16(0x800A77D0);
    value = gtmenu_power_display_value(value);
    gtmenu_format_integer_string(frame.address, 0x80053798, value, 0x800A7556);
    gtmenu_text_draw(ordering, 108, 336, frame.address);
    if (r_u8(0x8005815E) < 2) value = r_u16(model + 90);
    else value = r_s16(0x800A789A);
    value = gtmenu_mass_display_value(value);
    gtmenu_format_integer_string(frame.address, 0x800537A0, value, units);
    gtmenu_text_draw(ordering, 108, 366, frame.address);
    if ((uint32)r_u8(0x8005815E) - 1 < 2) {
        rectangle = frame.address + 136;
        w_u16(rectangle, 480);
        w_u16(rectangle + 2, 60);
        w_u16(rectangle + 4, 613);
        w_u16(rectangle + 6, 91);
        gtmenu_rectangle_shade(ordering, rectangle, 255);
    }
    if (!r_s8(0x8005815E)) {
        rectangle = frame.address + 144;
        w_u16(rectangle, 98);
        w_u16(rectangle + 2, 219);
        w_u16(rectangle + 4, 185);
        w_u16(rectangle + 6, 266);
        if (gtmenu_model_info_count() < 2) gtmenu_rectangle_shade(ordering, rectangle, 96);
        else gtmenu_rectangle_pulse(ordering, rectangle, 0);
    }
    gtmenu_model_preview_draw(ordering, 352, 240);
    counter = r_u32(0x80058180) + 1;
    w_u32(0x80058180, counter);
    result = (sint32)counter < 56;
    if (!result) w_u32(0x80058180, 0);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_info_close(void)
{
    uint32 category, previous, page;
    FUNCTION_MARKER(0x80016480, "GTMENU.EXE");
    category = r_u16(0x80058160);
    w_u32(0x800A77B4, 0x8001FDA4);
    previous = r_u16(0x800A77C4);
    w_u8(0x8005815D, 0);
    w_u16(0x800A77C0, previous);
    w_u16(0x800A77C4, category);
    w_u16(0x800A77C0, 661);
    page = gtmenu_category_name((sint16)category);
    return gtmenu_page_open(page);
}

uint32 gtmenu_model_info_callback_restore(void)
{
    FUNCTION_MARKER(0x800164F0, "GTMENU.EXE");
    w_u8(0x8005815D, 0);
    w_u32(0x800A77B4, 0x8001FDA4);
    return 0x8001FDA4;
}

uint32 gtmenu_current_ordering(void)
{
    FUNCTION_MARKER(0x8001D93C, "GTMENU.EXE");
    return r_u32(0x800823BC);
}

sint32 gtmenu_model_info_selection(void)
{
    FUNCTION_MARKER(0x8001DB94, "GTMENU.EXE");
    return r_s16(0x80058638);
}

sint32 gtmenu_model_info_count(void)
{
    FUNCTION_MARKER(0x8001DBB0, "GTMENU.EXE");
    return gtmenu_model_color_count(r_u32(0x80058634));
}

uint32 gtmenu_model_color_count(uint32 index)
{
    uint32 entry;
    FUNCTION_MARKER(0x80032078, "GTMENU.EXE");
    entry = gtmenu_color_table_entry(r_u32(0x800A8878), index);
    return r_u8(entry);
}

uint32 gtmenu_color_table_entry(uint32 table, uint32 index)
{
    uint32 count;
    FUNCTION_MARKER(0x80035E0C, "GTMENU.EXE");
    while (index) {
        count = r_u8(table);
        --index;
        table += count * 2 + 1;
    }
    return table;
}

sint32 gtmenu_selected_color(void)
{
    FUNCTION_MARKER(0x8001C1D4, "GTMENU.EXE");
    return r_s8(0x80058468);
}

uint32 gtmenu_model_info_activate(void)
{
    uint32 action, descriptor, spec_mode;
    sint32 selection;
    FUNCTION_MARKER(0x80016510, "GTMENU.EXE");
    action = r_u32(0x800A7A5C);
    selection = gtmenu_model_info_selection();
    if (!action) return 0;
    if (!gtmenu_string_compare(action, 0x800537A8)) {
        if (gtmenu_model_info_count() < 2) {
            gtmenu_sound_effect(6);
            return 0;
        }
        gtmenu_sound_effect(5);
        gtmenu_model_color_next();
        w_u16(0x80080B98, (r_u16(0x80080B98) & 0xFFF) | ((uint32)selection << 12));
        return 1;
    }
    if (!gtmenu_string_compare(action, 0x800537BC)) return 0;
    if (!gtmenu_string_compare(action, 0x800537CC)) return 0;
    if (!gtmenu_string_compare(action, 0x800537DC)) {
        gtmenu_sound_effect(5);
        gtmenu_model_info_close();
        return 1;
    }
    if (!gtmenu_string_compare(action, 0x800537EC)) {
        gtmenu_sound_effect(5);
        gtmenu_model_more_open(0x80080B98, r_s16(0x80058160), 0);
        return 1;
    }
    if (!gtmenu_string_compare(action, 0x800537FC)) {
        gtmenu_sound_effect(5);
        spec_mode = r_u8(0x8005815E) < 2 ? 0 : 2;
        gtmenu_model_spec_open(0x80080B98, 0x800A7840, 0x800A77D0,
            r_s16(0x80058160), spec_mode);
        return 1;
    }
    if (!gtmenu_string_compare(action, 0x8005380C)) {
        descriptor = r_u32(0x80058164);
        if (r_u8(descriptor + 21)) gtmenu_sound_effect(6);
        else {
            gtmenu_sound_effect(5);
            gtmenu_model_purchase_open(0, 0x80080718, r_s16(0x80058160), 0);
        }
        return 1;
    }
    if (!gtmenu_string_compare(action, 0x80053818)) {
        gtmenu_sound_effect(5);
        gtmenu_model_change_prepare();
        gtmenu_model_purchase_open(3, 0x80080718, r_s16(0x80058160), 0);
        return 1;
    }
    gtmenu_model_info_callback_restore();
    return gtmenu_ui_default_activate() != 0;
}

uint32 gtmenu_model_info_event(uint32 event)
{
    uint32 kind, buttons;
    FUNCTION_MARKER(0x800167DC, "GTMENU.EXE");
    kind = r_u32(event);
    if (kind == 3) {
        gtmenu_model_info_angle_add((uint32)r_s16(0x80058168));
        gtmenu_model_info_frame_draw(r_u32(event + 4));
        gtmenu_hud_current_car_draw(r_u32(event + 4));
        gtmenu_hud_credits_draw(r_u32(event + 4));
        gtmenu_hud_days_draw(r_u32(event + 4));
        return gtmenu_scene_draw(r_u32(event + 4));
    }
    if (kind != 24) return 24;
    if (r_u32(event + 12) & 0x9000) {
        gtmenu_sound_effect(3);
        return gtmenu_model_info_close();
    }
    buttons = r_u32(event + 12) & 0x6000;
    if (buttons) return gtmenu_model_info_activate();
    return buttons;
}

uint32 gtmenu_model_info_position_set(uint32 position)
{
    uint32 first, second;
    FUNCTION_MARKER(0x800168B8, "GTMENU.EXE");
    first = r_u32(position);
    second = r_u32(position + 4);
    w_u32(0x80080BF8, first);
    w_u32(0x80080BFC, second);
    return first;
}

uint32 gtmenu_model_info_frame_draw(uint32 ordering)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 result;
    FUNCTION_MARKER(0x800168E8, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(16, 0x800168E8);
    w_u16(frame.address, 150);
    w_u16(frame.address + 2, 55);
    w_u16(frame.address + 4, 458);
    w_u16(frame.address + 6, 242);
    w_u16(frame.address + 8, 448);
    w_u16(frame.address + 10, 322);
    w_u16(frame.address + 12, 517);
    w_u16(frame.address + 14, 391);
    gtmenu_model_info_draw();
    gtmenu_color_picker_bounds_set(frame.address);
    gtmenu_model_preview_frame_draw();
    result = (uint32)r_s8(0x8005815C);
    if (result) result = gtmenu_rectangle_shade(ordering, frame.address + 8, 96);
    gtmenu_local_stack_pop(frame);
    return result;
}

uint32 gtmenu_model_info_change_available(uint32 model)
{
    uint32 category;
    FUNCTION_MARKER(0x800169AC, "GTMENU.EXE");
    category = r_u16(0x800A77C4);
    if (gtmenu_model_has_more_info(model)) return 0;
    switch (category) {
    case 50: case 54: case 62: case 70: case 58: case 66:
        return 0;
    default:
        return 1;
    }
}

uint32 gtmenu_color_picker_bounds_set(uint32 rectangle)
{
    uint32 value;
    FUNCTION_MARKER(0x8001DB60, "GTMENU.EXE");
    value = r_u16(rectangle);
    w_u16(0x8008E8EC, value);
    value = r_u16(rectangle + 2);
    w_u16(0x8008E8EE, value);
    value = r_u16(rectangle + 4);
    w_u16(0x8008E8F0, value);
    value = r_u16(rectangle + 6);
    w_u16(0x8008E8F2, value);
    return value;
}

uint32 gtmenu_model_has_more_info(uint32 model)
{
    FUNCTION_MARKER(0x80020194, "GTMENU.EXE");
    if (r_s8(model + 4) == 'r') return 0;
    if (r_s8(model + 1) == '-') return 0;
    return 1;
}

uint32 gtmenu_model_info_angle_add(uint32 delta)
{
    uint32 result;
    FUNCTION_MARKER(0x8001DB44, "GTMENU.EXE");
    result = r_u16(0x8005863A) + delta;
    w_u16(0x8005863A, result);
    return result;
}

void gtmenu_model_color_next(void)
{
    uint32 count, selection;
    FUNCTION_MARKER(0x8001DC6C, "GTMENU.EXE");
    count = gtmenu_model_color_count(r_u32(0x80058634));
    selection = r_u16(0x80058638) + 1;
    w_u16(0x80058638, selection);
    if ((sint16)selection >= (sint32)count) w_u16(0x80058638, 0);
}

uint32 gtmenu_model_info_open_node(uint32 node, sint32 category)
{
    GT_LOCAL_STACK_FRAME frame;
    uint32 archive, model, available, color_table, count, index, name, previous;
    uint32 descriptor, first, second, result;
    sint32 selection, model_index;
    sint8 code;
    FUNCTION_MARKER(0x80016A20, "GTMENU.EXE");
    frame = gtmenu_local_stack_push(72, 0x80016A20);
    archive = gtmenu_resource_archive();
    model = r_u32(node + 12);
    w_u16(0x80058160, category);
    w_u32(0x80058164, node);
    gtmenu_memory_fill(0x80080718, 0, 128);
    w_u8(0x8005815D, 1);
    w_u8(0x8005815E, 1);
    gtmenu_memory_fill(0x80080B98, 0, 96);
    available = gtmenu_model_info_change_available(model);
    descriptor = r_u32(node + 12);
    w_u8(0x8005815C, available);
    model_index = gtmenu_model_name_index(descriptor);
    count = gtmenu_model_color_count(model_index);
    color_table = gtmenu_model_color_table(model_index);
    selection = gtmenu_model_color_index(r_u8(node + 20), color_table);
    if (selection >= (sint32)count || selection < 0) selection = 0;
    gtmenu_model_info_selection_set((sint16)selection);
    w_u16(0x80080B98, (r_u16(node + 10) + 1) | ((uint32)selection << 12));
    for (index = 0; index < count; ++index) {
        code = (sint8)gtmenu_model_color_code(index, color_table);
        name = gtmenu_model_color_name(r_u16(0x80080B98) & 0xFFF, code, archive);
        gtmenu_string_copy(0x80080798 + index * 64, name);
    }
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
