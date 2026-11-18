#include "game_first2_registry.h"

void gtmain_menu_parameter_selector_reset(uint32 selector)
{
    FUNCTION_MARKER(0x80040EC4, "GTMAIN.EXE");
    w_u8(selector + 1, 0);
    w_u8(selector + 13, 0);
    w_u8(selector + 14, 0);
}
