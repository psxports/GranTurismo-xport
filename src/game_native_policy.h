#ifndef GT_GAME_NATIVE_POLICY_H
#define GT_GAME_NATIVE_POLICY_H

#include "psx.h"

uint32 gt_native_critical_enter(void);
void gt_native_critical_exit(void);

/* Project code has no asynchronous guest interrupt entry */
#define EnterCriticalSectionPSX gt_native_critical_enter
#define ExitCriticalSection gt_native_critical_exit
#define xport_bios_enter_critical gt_native_critical_enter
#define xport_bios_exit_critical gt_native_critical_exit

#endif
