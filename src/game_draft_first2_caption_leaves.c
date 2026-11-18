#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_8002FA74. */ uint32 gtmain_course_caption_copy(uint32 text)
{
    return gtmain_string_copy(r_u32(0x8009122Cu), text);
}

/* Original: GTMAIN.EXE:sub_8003DBE8. */ void gtmain_menu_record_dirty_clear(void)
{
    w_u8(0x801F2041u, 0);
}
