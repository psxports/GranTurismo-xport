#include "game_first2_registry.h"

static void caption_style(sint32 intensity)
{
    /* TODO Bind unselected caption style owner */
    gt_draft_failure("GTMAIN two-line caption style", 0x8003D928u);
}

static sint32 caption_width(uint32 text)
{
    /* TODO Bind unselected caption width owner */
    gt_draft_failure("GTMAIN two-line caption width", 0x80035F7Cu);
}

static uint32 caption_draw(uint32 ordering, sint32 x, sint32 y, uint32 text)
{
    /* TODO Bind unselected conditional caption draw owner */
    gt_draft_failure("GTMAIN two-line caption draw", 0x80035E40u);
}

/* GTMAIN.EXE:8003D9D8 */
uint32 gtmain_menu_two_line_caption_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 first_text, uint32 second_text, sint32 intensity)
{
    sint32 width;
    /* TODO Style, width and draw dependencies remain unresolved */
    caption_style(intensity);
    width = caption_width(first_text);
    caption_draw(ordering, (sint32)((uint32)x - (uint32)(width >> 1) + 160u),
        (sint32)((uint32)y + 114u), first_text);
    width = caption_width(second_text);
    return caption_draw(ordering, (sint32)((uint32)x - (uint32)(width >> 1) + 160u),
        (sint32)((uint32)y + 130u), second_text);
}
