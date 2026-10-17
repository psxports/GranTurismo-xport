#include "game_first2_registry.h"

/* GTMAIN.EXE 8006CA10 */
sint32 gtmain_font_text_draw(uint32 ordering_link, sint32 x, sint32 y, uint32 guest_string)
{
    return gtmain_font_string_draw(0x800C0070u, ordering_link, x, y, guest_string);
}
