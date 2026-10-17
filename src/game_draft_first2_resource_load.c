#include "game_first2_registry.h"
#include "game_sound_stack.h"

static uint8 *texture_workspace;
static uint32 texture_workspace_capacity;
static uint32 texture_workspace_active;

static uint8 *texture_record_native(uint32 id)
{
    if (!texture_workspace_active || !texture_workspace || texture_workspace_capacity < 24u ||
        id > (texture_workspace_capacity - 24u) / 24u)
        gt_draft_failure("Native texture field record extent", 0x80030DFCu);
    return texture_workspace + id * 24u;
}

/* GTMAIN.EXE 80030DFC */
uint8 *gtmain_texture_clut_native(uint32 id) { return texture_record_native(id) + 16u; }

/* GTMAIN.EXE 80030E1C */
uint8 *gtmain_texture_page_native(uint32 id) { return texture_record_native(id) + 18u; }

/* GTMAIN.EXE 80030E3C */
sint32 gtmain_texture_x_native(uint32 id)
{
    return (sint16)xport_load_le16(texture_record_native(id) + 20u);
}

/* GTMAIN.EXE 80030E60 */
sint32 gtmain_texture_y_native(uint32 id)
{
    return (sint16)xport_load_le16(texture_record_native(id) + 22u);
}

/* GTMAIN.EXE 80030DCC */
void gtmain_texture_workspace_publish(uint8 *workspace, uint32 capacity)
{
    if (!workspace && capacity)
        gt_draft_failure("Native texture workspace unavailable", 0x80030DCCu);
    texture_workspace = workspace;
    texture_workspace_capacity = capacity;
    texture_workspace_active = 1u;
    w_u32(0x800B49D8u, 0u);
}

void gtmain_texture_workspace_release(uint8 *workspace)
{
    if (texture_workspace == workspace) {
        texture_workspace = NULL;
        texture_workspace_capacity = 0u;
        texture_workspace_active = 0u;
    }
}

/* GTMAIN.EXE 80030E84 */
sint32 gtmain_texture_name_find_native(const uint8 *name)
{
    uint32 index = 0u, offset;
    if ((sint32)r_u32(0x800B49D8u) <= 0) return -1;
    do {
        uint8 *record;
        if (!texture_workspace_active || !name || texture_workspace_capacity < 24u ||
            index > (texture_workspace_capacity - 24u) / 24u)
            gt_draft_failure("Native texture name lookup extent", 0x80030E84u);
        record = texture_workspace + index * 24u;
        for (offset = 0u; offset < 24u; ++offset) {
            uint8 first = record[offset], second = name[offset];
            if (first != second) break;
            if (!first) return (sint32)index;
        }
        if (offset == 24u)
            gt_draft_failure("Native texture name terminator extent", 0x80030E84u);
        ++index;
    } while ((sint32)index < (sint32)r_u32(0x800B49D8u));
    return -1;
}

static void resource_upload(uint32 block)
{
    PSX_RECT rectangle;
    rectangle.x = (sint16)r_u16(block + 4u);
    rectangle.y = (sint16)r_u16(block + 6u);
    rectangle.w = (sint16)r_u16(block + 8u);
    rectangle.h = (sint16)r_u16(block + 10u);
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(block + 12u, 1u));
    DrawSync(0);
}

static uint32 texture_palette_native(uint8 *record, uint32 block, uint32 image)
{
    uint32 mode, x, next;
    uint16 value;
    if (!(r_u32(image + 4u) & 8u)) {
        xport_store_le16(record + 16u, 0u);
        return block;
    }
    mode = r_u32(image + 4u) & 7u;
    if (mode == 0u || mode == 1u) resource_upload(block);
    x = r_u16(block + 4u);
    value = xport_load_le16(record + 16u);
    next = block + r_u32(block);
    value = (uint16)((value & 0xFFC0u) | ((x >> 4u) & 63u));
    xport_store_le16(record + 16u, value);
    value = (uint16)((value & 0x803Fu) | ((r_u16(block + 6u) & 511u) << 6u));
    xport_store_le16(record + 16u, value);
    return next;
}

static uint32 texture_pixels_native(uint8 *record, uint32 block, uint32 image)
{
    uint32 mode = r_u32(image + 4u) & 7u;
    uint32 depth = mode == 1u ? 1u : mode == 2u ? 2u : 0u;
    uint32 x;
    uint16 value;
    resource_upload(block);
    x = r_u16(block + 4u); value = xport_load_le16(record + 18u);
    value = (uint16)((value & 0xFFF0u) | ((x >> 6u) & 15u));
    xport_store_le16(record + 18u, value);
    value = (uint16)((value & 0xFFEFu) | ((r_u16(block + 6u) >> 4u) & 16u));
    value = (uint16)((value & 0xFE7Fu) | (depth << 7u));
    xport_store_le16(record + 18u, value);
    xport_store_le16(record + 20u, (uint16)((r_u16(block + 4u) & 63u) << (2u - depth)));
    xport_store_le16(record + 22u, r_u16(block + 6u));
    return block + r_u32(block);
}

/* GTMAIN.EXE 80030F00 */
uint32 gtmain_texture_palette_load(uint32 record, uint32 block, uint32 image)
{
    uint32 mode, next;
    uint16 value;
    if (!(r_u32(image + 4u) & 8u)) {
        w_u16(record + 16u, 0);
        return block;
    }
    mode = r_u32(image + 4u) & 7u;
    if (mode == 0u || mode == 1u) resource_upload(block);
    value = (uint16)((r_u16(record + 16u) & 0xFFC0u) | ((r_u16(block + 4u) >> 4) & 0x3Fu));
    next = block + r_u32(block);
    w_u16(record + 16u, value);
    value = (uint16)((value & 0x803Fu) | ((r_u16(block + 6u) & 0x1FFu) << 6));
    w_u16(record + 16u, value);
    return next;
}

/* GTMAIN.EXE 80030FFC */
uint32 gtmain_texture_pixels_load(uint32 record, uint32 block, uint32 image)
{
    uint32 mode = r_u32(image + 4u) & 7u;
    uint32 depth = mode == 1u ? 1u : mode == 2u ? 2u : 0u;
    uint16 value;
    resource_upload(block);
    value = (uint16)((r_u16(record + 18u) & 0xFFF0u) | ((r_u16(block + 4u) >> 6) & 0xFu));
    w_u16(record + 18u, value);
    value = (uint16)((value & 0xFFEFu) | ((r_u16(block + 6u) >> 4) & 0x10u));
    value = (uint16)((value & 0xFE7Fu) | (depth << 7));
    w_u16(record + 18u, value);
    w_u16(record + 20u, (uint16)((r_u16(block + 4u) & 0x3Fu) << (2u - depth)));
    w_u16(record + 22u, r_u16(block + 6u));
    return block + r_u32(block);
}

/* GTMAIN.EXE 80031120 */
uint32 gtmain_texture_records_load(uint32 image)
{
    uint32 index = 0, result = r_u32(image);
    if (result) do {
        if (texture_workspace_active) {
            uint8 *native_record;
            uint32 name = image + 20u * index + 4u, length = 0u, section, pixels;
            uint8 byte;
            if (texture_workspace_capacity < 24u || index > (texture_workspace_capacity - 24u) / 24u)
                gt_draft_failure("Native texture record extent", 0x80031120u);
            native_record = texture_workspace + 24u * index;
            do {
                if (length == 24u)
                    gt_draft_failure("Native texture record name extent", 0x80031120u);
                byte = r_u8(name + length);
                native_record[length++] = byte;
            } while (byte);
            section = image + r_u32(image + 20u * index + 20u);
            pixels = texture_palette_native(native_record, section + 8u, section);
            texture_pixels_native(native_record, pixels, section);
            ++index; result = index < r_u32(image);
            continue;
        }
        uint32 record = r_u32(0x800B49D4u) + 24u * index;
        uint32 section, pixels;
        gtmain_string_copy(record, image + 20u * index + 4u);
        section = image + r_u32(image + 20u * index + 20u);
        pixels = gtmain_texture_palette_load(record, section + 8u, section);
        gtmain_texture_pixels_load(record, pixels, section);
        ++index;
        result = index < r_u32(image);
    } while (result);
    w_u32(0x800B49D8u, index);
    return result;
}

/* GTMAIN.EXE 80031E5C */
uint32 gtmain_race_sound_bank_load(uint32 destination, uint32 spu_base,
    const uint8 *table, uint32 sector, uint32 section_index)
{
    uint32 path = gtmain_sound_path();
    uint32 image = gtmain_archive_section_load(path, table, sector, section_index, 0x801BB70Cu, 0);
    gtmain_memory_copy(destination, image, r_u32(image + 8u));
    gtmain_sound_engine_relocate(image, spu_base);
    {
        uint32 relocated = gtmain_sound_engine_relocate(destination, spu_base);
        uint32 address = r_u32(destination + 12u);
        uint32 source = r_u32(image + 20u);
        uint32 size = r_u32(image + 16u);
        (void)gtmain_sound_sample_upload(address, source, size, relocated);
    }
    return destination;
}

/* GTMAIN.EXE 80031F18 */
void gtmain_race_sound_banks_update(void)
{
    uint32 selectors = gt_sound_stack_workspace(0x4160u, 4128u, 0x80031F18u);
    uint32 count = 3, changed = 0, index;
    uint32 temporary = selectors + 16u, sector_address = temporary + 4096u;
    uint32 table;
    {
        uint32 second = r_u16(0x800BB9D0u), first = r_u16(0x800BB9C6u);
        w_u32(selectors + 4u, second);
        w_u32(selectors, first);
    }
    if (r_u8(0x800CB7F5u) == 2u) {
        count = 4;
        uint32 third = r_u16(0x800BBB48u), fourth = r_u16(0x800BBB52u);
        w_u32(selectors + 8u, third);
        w_u32(selectors + 12u, fourth);
    } else {
        w_u32(selectors + 8u, 54u);
        w_u32(selectors + 12u, 0xFFFFFFFFu);
    }
    for (index = 0; index < 4u; ++index) {
        if (index < count && (sint32)(sint16)r_u16(0x800B52ECu + 2u * index) != (sint32)r_u32(selectors + 4u * index))
            changed = 1;
        w_u16(0x800B52ECu + 2u * index, r_u16(selectors + 4u * index));
    }
    if (changed) {
        uint32 path = gtmain_sound_path();
        uint32 destination_offset = 0, spu_base = 0x2C800u;
        table = gtmain_archive_table_load_guest(path, sector_address, temporary, 4096u);
        for (index = 0; index < count; ++index) {
            uint32 destination = 0x800B4EECu + destination_offset;
            uint32 loaded, header_size, sample_size, section, sector;
            w_u32(0x800B49DCu + 4u * index, destination);
            section = r_u32(selectors + 4u * index) + 3u;
            sector = r_u32(sector_address);
            loaded = gtmain_race_sound_bank_load(destination, spu_base,
                table ? (uint8 *)psx_addr(table, 1u) : NULL, sector, section);
            header_size = r_u32(loaded + 8u);
            sample_size = r_u32(loaded + 16u);
            spu_base += (sample_size + 15u) & ~15u;
            destination_offset += (header_size + 3u) & ~3u;
        }
    }
}
