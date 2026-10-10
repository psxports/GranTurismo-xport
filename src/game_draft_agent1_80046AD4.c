#include "game_first2_registry.h"

/* GTMAIN.EXE 80046AD4 */
uint32 gtmain_menu_transition_exit(uint32 descriptor)
{
    w_u8(descriptor + 12u, 1u);
    w_u8(descriptor + 13u, 12u);
    return 12u;
}
