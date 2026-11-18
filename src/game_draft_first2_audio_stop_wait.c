#include "game_first2_registry.h"

/* GTMAIN.EXE 8002AD7C */
/* Original: GTMAIN.EXE:sub_8002AD7C. */ sint32 gtmain_race_audio_stop_wait(void)
{
    gtmain_race_sound_callbacks_stopped();
    return gtmain_stream_stop_wait(1u);
}
