#include "game_first2_registry.h"

uint32 gt_native_critical_enter(void)
{
    /* Dummy entry reports available serialization without changing IRQ state */
    return 1u;
}

void gt_native_critical_exit(void)
{
    /* User-authorized no-op without pending callback dispatch */
}

/* GTMAIN.EXE:8008AF08 */
/* Original: GTMAIN.EXE:sub_8008AF08. */ uint32 gtmain_bios_critical_enter(void)
{
    return (uint32)EnterCriticalSectionPSX();
}

/* GTMAIN.EXE:8008AF28 */
uint32 gtmain_bios_critical_exit_result(void)
{
    /* Native dummy exit uses a neutral result, not a BIOS result claim */
    gt_native_critical_exit();
    return 0u;
}
