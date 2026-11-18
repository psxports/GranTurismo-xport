#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8002F8CC */
/* Original: GTMAIN.EXE:sub_8002F8CC. */ void gt_draft_8002F8CC(sint32 font, uint32 table)
{
    uint32 index = 0u, text = r_u32(table + 4u);
    while (text)
    {
        sint32 width = gtmain_text_width(font, text);
        uint32 entry = table + index * 8u;
        sint32 difference;
        ++index;
        w_u16(entry + 2u, (uint16)width);
        difference = (sint32)(320u - (uint32)width);
        w_u16(entry, (uint16)(difference / 2));
        text = r_u32(table + index * 8u + 4u);
    }
}
