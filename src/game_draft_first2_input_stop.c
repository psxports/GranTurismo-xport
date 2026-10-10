#include "game_first2_registry.h"

/* GTMAIN.EXE:8007A754 */
void gtmain_input_stop_when_ready(void)
{
    uint32 port;
    do {
        VSync(0);
        for (port = 0u; port < 8u; ++port) {
            if (!gtmain_input_connected(port)) break;
        }
    } while (port != 8u);
    PadStopCom();
}
