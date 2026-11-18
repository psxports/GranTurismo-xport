#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_8006B668. */ uint32 gtmain_car_name(uint32 index)
{
    return r_u32(0x800C0C14u) + index * 6u + 32u;
}

/* Original: GTMAIN.EXE:sub_8006B688. */ sint32 gtmain_car_record_find(uint32 name, uint32 archive, uint32 section, uint32 require_alternate)
{
    uint32 records = archive + r_u32(archive + section * 12u + 16u);
    uint32 names = archive + r_u32(archive + 172u) + 24u;
    uint32 count = r_u16(records + 14u);
    uint32 index;
    records += 24u;
    for (index = 0; index < count; ++index)
    {
        uint32 record = records + index * 108u;
        uint32 id = r_u16(record);
        if (id && !gtmain_string_compare(names + id * 424u - 424u, name))
        {
            if (!require_alternate || r_u16(record + 34u))
                return (sint32)index;
        }
    }
    return -1;
}

/* Original: GTMAIN.EXE:sub_8006B828. */ sint32 gtmain_car_find(uint32 name)
{
    sint32 count = gtmain_car_count();
    sint32 index;
    for (index = 0; index < count; ++index)
        if (!gtmain_string_compare(name, gtmain_car_name((uint32)index)))
            return index;
    return -1;
}

/* Original: GTMAIN.EXE:sub_8006B898. */ uint32 gtmain_car_config_copy(uint32 destination, uint32 source)
{
    return gtmain_memory_copy(destination, source, 386u);
}

/* Original: GTMAIN.EXE:sub_8006B8B8. */ uint32 gtmain_car_config_fields(uint32 destination, uint32 source, uint32 descriptors, uint32 archive)
{
    uint32 word = r_u32(descriptors);
    while (word)
    {
        uint32 output = destination + (word & 0x3FFu);
        uint32 input = source + ((word >> 10) & 0x3FFu);
        uint32 type = (word >> 20) & 15u;
        if ((word >> 24) == 1u)
        {
            uint32 size = 0;
            switch (type)
            {
            case 0: w_u8(output, r_u8(input)); size = 1; break;
            case 1: w_u16(output, r_u16(input)); size = 2; break;
            case 2: w_u32(output, r_u32(input)); size = 4; break;
            case 3: size = 6; break;
            case 5: size = 12; break;
            case 6: size = 13; break;
            case 7: size = 17; break;
            case 8: size = 19; break;
            case 9:
            case 10:
            {
                uint32 section = archive + r_u32(archive + (type == 9 ? 36u : 37u) * 12u + 16u);
                uint32 id = (uint32)(sint32)(sint16)r_u16(input);
                uint32 stride = r_u32(section + 20u);
                gtmain_car_config_fields(destination, section + (id - 1u) * stride + 24u,
                    type == 9 ? 0x80093FF4u : 0x80094048u, archive);
                break;
            }
            default: break;
            }
            if (size >= 5u)
                gtmain_memory_copy(output, input, size);
        }
        else
        {
            uint32 factor = r_u8(input);
            if (type)
                w_u16(output, (r_u16(output) * factor) / 100u);
            else
                w_u8(output, (r_u8(output) * factor) / 100u);
        }
        descriptors += 4u;
        word = r_u32(descriptors);
    }
    return word;
}

/* Original: GTMAIN.EXE:sub_8006BAD4. */ uint32 gtmain_car_config_build(uint32 destination, uint32 selectors, uint32 archive)
{
    uint32 table = 0x800940C4u;
    uint32 selector = selectors;
    uint32 type_word = r_u16(table);
    uint32 result = type_word;
    while (type_word)
    {
        uint32 id = r_u16(selector);
        result = type_word << 16;
        if (id)
        {
            sint32 type = (sint16)type_word;
            uint32 section_index = (uint32)(sint32)(sint16)r_u16(table + 2u);
            uint32 section;
            uint32 record;
            uint32 apply = 0;
            if (type == 2)
                id &= 0xFFFu;
            section = archive + r_u32(archive + section_index * 12u + 16u);
            record = section + (id - 1u) * r_u32(section + 20u) + 24u;
            result = (uint32)type << 2;
            if ((uint32)type < 13u)
            {
                result = r_u32(0x8008E098u + (uint32)type * 4u);
                switch (type)
                {
                case 0: return result;
                case 1: apply = 1; break;
                case 2: result = gtmain_car_config_copy(destination, record); break;
                case 3:
                case 7:
                {
                    uint32 bonus = r_u8(record + 18u);
                    uint32 index;
                    uint32 second;
                    for (index = 0; index < 16u; ++index)
                        w_u8(destination + 52u + index, r_u8(destination + 52u + index) + bonus);
                    result = r_u8(destination + 50u);
                    second = r_u8(destination + 49u);
                    result += bonus;
                    second += bonus;
                    w_u8(destination + 50u, result);
                    w_u8(destination + 49u, second);
                    apply = 1;
                    break;
                }
                case 4:
                    result = r_u8(selectors + 64u); w_u8(destination + 46u, result);
                    result = r_u8(selectors + 77u); w_u8(destination + 47u, result);
                    break;
                case 5:
                    gtmain_memory_copy(destination + 26u, selectors + 46u, 16u);
                    apply = 1;
                    break;
                case 6:
                    result = r_u16(record + 20u); w_u16(destination + 384u, result);
                    apply = 1;
                    break;
                case 8:
                    result = r_u8(selectors + 65u); w_u8(destination + 136u, result);
                    result = r_u8(selectors + 78u); w_u8(destination + 137u, result);
                    apply = 1;
                    break;
                case 9:
                    result = r_u8(selectors + 66u); w_u8(destination + 111u, result);
                    break;
                case 10:
                    result = r_u8(selectors + 79u); w_u8(destination + 123u, result);
                    break;
                case 11:
                    w_u8(destination + 278u, r_u8(selectors + 68u));
                    w_u8(destination + 108u, r_u8(selectors + 67u));
                    w_u8(destination + 110u, r_u8(selectors + 69u));
                    w_u8(destination + 115u, r_u8(selectors + 70u));
                    w_u8(destination + 119u, r_u8(selectors + 71u));
                    w_u8(destination + 117u, r_u8(selectors + 72u));
                    w_u8(destination + 121u, r_u8(selectors + 73u));
                    w_u8(destination + 279u, r_u8(selectors + 81u));
                    w_u8(destination + 109u, r_u8(selectors + 80u));
                    w_u8(destination + 122u, r_u8(selectors + 82u));
                    w_u8(destination + 127u, r_u8(selectors + 83u));
                    w_u8(destination + 131u, r_u8(selectors + 84u));
                    w_u8(destination + 129u, r_u8(selectors + 85u));
                    w_u8(destination + 133u, r_u8(selectors + 86u));
                    apply = 1;
                    break;
                case 12:
                    w_u8(destination + 45u, r_u8(selectors + 63u));
                    gtmain_car_config_fields(destination, record, 0x80094064u, archive);
                    apply = 1;
                    break;
                }
            }
            if (apply)
                result = gtmain_car_config_fields(destination, record, r_u32(table + 4u), archive);
        }
        table += 8u;
        type_word = r_u16(table);
        selector += 2u;
    }
    return result;
}

/* Original: GTMAIN.EXE:sub_8006C31C. */ uint32 gtmain_random_step(uint32 state)
{
    uint32 result = r_u32(state) * 17u + 17u;
    w_u32(state, result);
    return result;
}

/* Original: GTMAIN.EXE:sub_8006C3DC. */ uint32 gtmain_car_config_bind(uint32 first, uint32 second, uint32 third, sint16 fourth, uint16 fifth)
{
    w_u32(0x800C00D4u, first);
    w_u32(0x800C00CCu, second);
    w_u32(0x800C00DCu, third);
    w_u16(0x800C00C8u, (uint16)fourth);
    w_u16(0x800C00CAu, fifth);
    return fifth;
}
