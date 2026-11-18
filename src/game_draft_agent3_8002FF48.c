#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_8002FF48. */ sint32 gtmain_race_banner_draw(void)
{
    sint32 result = 1;
    if (r_u32(0x800B630Cu) == 1u)
    {
        result = (sint8)r_u8(0x800912A8u);
        if (result >= 0)
        {
            gtmain_text_font(1);
            gtmain_text_color(0x00808080u);
            result = gtmain_font_text_draw(r_u32(0x800B6368u), 16, 216, 0x800BD9EDu);
        }
    }
    return result;
}
