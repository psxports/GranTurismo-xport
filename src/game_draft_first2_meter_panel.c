#include "game_first2_registry.h"

/* GTMAIN.EXE:8002C994 */
uint32 gtmain_race_meter_panel_draw(uint32 ordering, uint32 x, uint32 y, uint32 source)
{
    uint32 left = x + 1u, right = x + 12u, row = y + 2u;
    (void)gtmain_race_meter_rectangle_draw(ordering, (sint16)left, (sint16)row, r_s8(source + 215u));
    (void)gtmain_race_meter_rectangle_draw(ordering, (sint16)right, (sint16)row, r_s8(source + 283u));
    row = y + 17u;
    (void)gtmain_race_meter_rectangle_draw(ordering, (sint16)left, (sint16)row, r_s8(source + 351u));
    (void)gtmain_race_meter_rectangle_draw(ordering, (sint16)right, (sint16)row, r_s8(source + 419u));
    (void)gtmain_packet_sprite(ordering, (x & 65535u) | (y << 16), 0x1F1A6280u, 0x001A0010u, 0);
    return gtmain_packet_page(ordering, 0x206);
}
