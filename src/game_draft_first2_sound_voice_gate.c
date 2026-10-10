#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE:8007DE18 */
void gtmain_sound_voice_tick_if_enabled(void)
{
    /* TODO Existing voice tick retains unresolved SDK volume-pair adapters */
    if (r_u32(0x800B1BE4u)) gtmain_sound_voice_tick();
}
