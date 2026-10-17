#include "game_first2_registry.h"

uint32 gtmain_packed_bytes_entry(uint32 base, uint32 index)
{
    while (index)
    {
        uint32 size = r_u8(base);
        --index;
        base += size + 1u;
    }
    return base;
}

uint32 gtmain_packed_pairs_entry(uint32 base, uint32 index)
{
    while (index)
    {
        uint32 size = r_u8(base);
        --index;
        base += size * 2u + 1u;
    }
    return base;
}

uint32 gtmain_resource_name(uint32 index)
{
    return gtmain_packed_bytes_entry(r_u32(0x800C0C1Cu), index) + 1u;
}

uint32 gtmain_resource_pairs(uint32 index)
{
    return gtmain_packed_pairs_entry(r_u32(0x800C0C18u), index);
}

uint32 gtmain_resource_pairs_count(uint32 index)
{
    return r_u8(gtmain_packed_pairs_entry(r_u32(0x800C0C18u), index));
}

uint32 gtmain_resource_records_end(uint32 base, sint32 count)
{
    uint32 index;
    base += 2u;
    if (count > 0)
        for (index = 0u; index < (uint32)count; ++index)
            base += (uint32)r_u8(base) + 2u;
    return base;
}

uint32 gtmain_resource_records_relocate(uint32 base)
{
    sint32 count = (sint32)r_u32(base);
    uint32 index = 0u, result = (uint32)count * 4u + 4u;
    uint32 cursor = base + result;
    if (count > 0)
    {
        do
        {
            w_u32(base + index * 4u + 4u, cursor);
            cursor = gtmain_resource_records_end(cursor, (sint32)r_u16(cursor));
            if (cursor & 1u) ++cursor;
            ++index;
            result = index * 4u;
        }
        while (index < (uint32)count);
    }
    return result;
}

uint32 gtmain_resource_section_relocate(uint32 section)
{
    uint32 count = r_u16(section + 14u);
    uint32 stride = r_u32(section + 20u);
    uint32 records = section + 24u + count * stride;
    w_u32(section + 16u, records);
    return gtmain_resource_records_relocate(records);
}
