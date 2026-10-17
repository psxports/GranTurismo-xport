#include "game_first2_registry.h"

void gtmain_bios_gpu_link(uint32 guest_link)
{
    /* Ignore NOP writes until their original purpose is established */
    if (guest_link >> 24)
        gt_draft_failure("Native BIOS GP0 word outside NOP contract", 0x8008B608u);
}
void gtmain_bios_exception_hook(uint32 guest_context)
{
    /* Record the supplied context without reconstructing guest registers */
    w_u32(0x75D0u, guest_context);
}
void gtmain_bios_cd_remove(void)
{
    /* The native fixed-disc path has no BIOS CD handler to remove */
}
void gtmain_bios_exception_return(void)
{
    /* Native callbacks finish through the ordinary C calling convention */
}
void gtmain_bios_exception_reset(void)
{
    (void)psx_bios_reset_entry_interrupt();
}
