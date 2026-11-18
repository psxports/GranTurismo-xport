#include "game_first2_registry.h"
#include "xport_trace.h"

uint32 gtmain_menu_two_line_caption_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 first_text, uint32 second_text, sint32 intensity)
{
    sint32 width;
    FUNCTION_MARKER(0x8003D9D8, "GTMAIN.EXE");
    gtmain_menu_caption_style(intensity);
    width = gtmain_menu_caption_width(first_text);
    gtmain_menu_caption_plain_draw(ordering, (uint32)x - ((uint32)(width >> 1) - 160),
        (uint32)y + 114, first_text);
    width = gtmain_menu_caption_width(second_text);
    return gtmain_menu_caption_plain_draw(ordering, (uint32)x - ((uint32)(width >> 1) - 160),
        (uint32)y + 130, second_text);
}
