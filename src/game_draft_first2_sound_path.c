#include "game_first2_registry.h"

/* Original: GTMAIN.EXE:sub_80070E9C. */ uint32 gtmain_sound_path(void)
{
    return gtmain_environment_value(0x8008E24Cu);
}
