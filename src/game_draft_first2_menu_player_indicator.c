#include "game_first2_registry.h"

/* GTMAIN.EXE 80038CA8 */
/* TODO Indicator model geometry dependency remains incomplete */
uint32 gtmain_menu_player_indicator_draw(uint32 ordering, sint32 x, sint32 y)
{
    gtmain_menu_stream_check();
    return gtmain_menu_indicator_draw(ordering, (sint32)((uint32)x + 160u),
        (sint32)((uint32)y + 136u), -512, 0x208u, 0u, 2u, 1u);
}
