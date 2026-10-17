#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include <stdlib.h>

/* GTMAIN.EXE 80071964 */
void gtmain_cd_file_retry_hook(void)
{
}

/* GTMAIN.EXE 8006F11C */
void gtmain_loading_callback_noop(void)
{
}

static uint32 loader_exec_thunk_missing(uint32 executable, uint32 argument,
                                        uint32 copied_code, uint32 copied_size)
{
    /* TODO Bind the native owner of the original copied BIOS Exec thunk */
    (void)executable; (void)argument; (void)copied_code; (void)copied_size;
    gt_draft_failure("TODO native copied Exec thunk 8006A2B0", 0x8006A334u);
    abort();
}

uint32 gtmain_loader_exec_image(uint32 executable)
{
    uint32 copied_code = gt_local_stack_workspace(0x2400u, 256u, 0x8006A334u);
    uint32 argument;
    EnterCriticalSectionPSX(); FlushCache();
    gtmain_memory_copy(copied_code, 0x8006A2B0u, 256u);
    argument = r_u32(0x8009997Cu);
    return loader_exec_thunk_missing(executable, argument, copied_code, 256u);
}
