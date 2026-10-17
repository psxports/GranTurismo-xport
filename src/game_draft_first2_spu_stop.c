#include "game_first2_registry.h"

static uint32 stop_bios(uint32 owner, uint32 argument)
{
    if (owner == 0x8008AF28u) return gtmain_bios_critical_exit_result();
    /* TODO Bind the actual BIOS event or critical-return service */
    gt_draft_failure("Native SPU stop BIOS adapter", owner);
}
uint32 gtmain_spu_driver_stop(void)
{
    uint32 handle;
    if (r_u32(0x800B2414u) != 1) return 1;
    w_u32(0x800B2414u, 0);
    (void)EnterCriticalSectionPSX();
    w_u32(0x800B237Cu, 0); w_u32(0x800B2380u, 0);
    (void)gtmain_spu_callback_bind(0);
    handle = r_u32(0x800B23ACu); (void)stop_bios(0x8008AF38u, handle);
    handle = r_u32(0x800B23ACu); (void)stop_bios(0x8008B5F8u, handle);
    return stop_bios(0x8008AF28u, 0);
}
