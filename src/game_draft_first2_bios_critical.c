#include "game_first2_registry.h"

/* GTMAIN.EXE:8008AF08 */
uint32 gtmain_bios_critical_enter(void)
{
    return (uint32)EnterCriticalSectionPSX();
}

/* GTMAIN.EXE:8008AF28 */
uint32 gtmain_bios_critical_exit_result(void)
{
    /* TODO Bind meaningful syscall2 result consumed by native callers */
    gt_draft_failure("BIOS syscall2 critical exit result", 0x8008AF28u);
}
