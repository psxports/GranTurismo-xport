#include "game_draft_registry.h"
#include "game_first2_registry.h"

/* GTMAIN.EXE 8002FDCC */
sint32 gtmain_race_record_text_draw(uint32 indication, uint32 guest_text)
{
    uint32 ordering = r_u32(0x800B6368u);
    GTMainFormatArguments arguments = {NULL, 0u, 0u, 0u};
    gtmain_text_color(128u);
    gtmain_text_font(2);
    return gtmain_text_format(ordering, 120, 66, guest_text, &arguments);
}
