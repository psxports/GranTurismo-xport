#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* GTMAIN.EXE 8007A694 */
void gtmain_frame_events_disable(void)
{
    uint32 index;
    EnterCriticalSectionPSX();
    for (index = 0; index < 8u; ++index) DisableEvent(r_u32(0x800C0A60u + 4u * index));
    ExitCriticalSection();
}

/* GTMAIN.EXE 8007B6E4 */
void gtmain_frame_callback_remove(uint32 node)
{
    uint32 previous, next, link;
    EnterCriticalSectionPSX();
    previous = r_u32(node); next = r_u32(node + 4u);
    gtmain_native_frame_guest_removed(node, previous, next);
    link = previous ? previous + 4u : 0x800B1A20u;
    w_u32(link, next);
    if (next) w_u32(next, previous);
    ExitCriticalSection();
}

/* GTMAIN.EXE 8007B734 */
sint32 gtmain_frame_event_open(uint32 event_class, uint32 specification, uint32 mode, uint32 callback_token)
{
    sint32 result;
    EnterCriticalSectionPSX();
    /* TODO Bind unsupported guest event callback tokens in the native SDK */
    result = OpenEventGuest(event_class, specification, mode, callback_token);
    if (result != -1) EnableEvent((uint32)result);
    ExitCriticalSection();
    return result;
}

/* GTMAIN.EXE 8007A7B8 */
sint32 gtmain_frame_shutdown(void)
{
    if (!r_u32(0x800B1A24u)) return -1;
    w_u32(0x800B1A24u, 0);
    VSyncCallback(NULL);
    return 0;
}
