#include "game_first2_registry.h"

uint32 gtmain_menu_player_indicator_draw(uint32 ordering, sint32 x, sint32 y)
{
    /* Original occurrences
     * GTMAIN.EXE:0x800362E4
     * GTMAIN.EXE:0x80038CA8
     */
    gtmain_menu_stream_check();
    return gtmain_menu_indicator_draw(ordering, (sint32)((uint32)x + 160u),
        (sint32)((uint32)y + 136u), -512, 0x208u, 0u, 2u, 1u);
}
