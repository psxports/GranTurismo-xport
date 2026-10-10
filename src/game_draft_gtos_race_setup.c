#include "game_draft_registry.h"
#include <stdio.h>

static uint32 gtos_race_packed_name(uint32 table, uint32 index)
{
    while (index--) table += r_u8(table) + 1u;
    return table + 1u;
}

static uint32 gtos_race_names_equal(uint32 left, uint32 right)
{
    uint32 first, second;
    do {
        first = r_u8(left++); second = r_u8(right++);
        if (first != second) return 0u;
    } while (first);
    return 1u;
}

static sint32 gtos_race_car_index(sint32 category, sint32 selected)
{
    uint32 name = r_u32(0x8004DA78u + 16u * (uint32)category + 4u * (uint32)selected);
    sint32 count = (sint16)r_u16(0x801B16CEu), index;
    for (index = 0; index < count; ++index) {
        uint32 entry = r_u32(0x801B16E4u) + 32u + 6u * (uint32)index;
        if (gtos_race_names_equal(name, entry)) return index;
    }
    return -1;
}

static sint32 gtos_race_track_index(uint32 name)
{
    sint32 count = (sint16)r_u16(0x801B16CCu), index;
    for (index = 0; index < count; ++index) {
        uint32 entry = gtos_race_packed_name(r_u32(0x801B16D8u), (uint32)index);
        if (gtos_race_names_equal(name, entry)) return index;
    }
    return -1;
}

void gtos_arcade_race_setup(void)
{
    uint32 mode = r_u8(0x800787D1u), level = r_u8(0x800787D2u), track_name;
    uint32 first_color, second_color, first_gear, second_gear, first_transmission, second_transmission;
    sint32 track = (sint8)r_u8(0x800787DDu), car;
    w_u8(0x80081795u, mode);
    w_u8(0x80081799u, level);
    track_name = r_u32(((sint8)mode == 6 ? 0x8004DBA0u : 0x8004DB7Cu) + 4u * (uint32)track);
    w_u8(0x80081798u, (uint32)gtos_race_track_index(track_name));
    car = gtos_race_car_index((sint8)r_u8(0x800787D3u), (sint8)r_u8(0x800787D5u));
    w_u16(0x8008179Au, (uint32)car);
    car = gtos_race_car_index((sint8)r_u8(0x800787D4u), (sint8)r_u8(0x800787D6u));
    w_u16(0x800817A0u, (uint32)car);
    /* Keep the original machine-name diagnostic on the native output */
    printf("1p machine <%s>\n", (const char *)psx_addr(
        gtos_race_packed_name(r_u32(0x801B16ECu), r_u16(0x8008179Au)), 1u));
    first_color = r_u8(0x800787DBu); second_color = r_u8(0x800787DCu);
    first_gear = r_u8(0x800787D7u); second_gear = r_u8(0x800787D8u);
    w_u8(0x8008179Eu, first_color);
    first_transmission = (uint32)(sint32)(sint8)r_u8(0x800787D9u);
    w_u8(0x800817A4u, second_color);
    second_transmission = (uint32)(sint32)(sint8)r_u8(0x800787DAu);
    w_u8(0x8008179Cu, first_gear); w_u8(0x800817A2u, second_gear);
    w_u8(0x8008179Fu, first_transmission != 0u);
    w_u8(0x800817A5u, second_transmission != 0u);
}
