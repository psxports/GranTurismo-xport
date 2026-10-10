#include "game_first2_registry.h"

void gtmain_bios_gpu_link(uint32 guest_link)
{
    /* TODO Bind the original A0:49 BIOS GPU link service */
    (void)guest_link;
    gt_draft_failure("Native BIOS GPU link adapter", 0x8008B608u);
}
void gtmain_bios_exception_hook(uint32 guest_context)
{
    /* TODO Bind the original B0:19 BIOS exception hook service */
    (void)guest_context;
    gt_draft_failure("Native BIOS exception hook adapter", 0x8008B628u);
}
void gtmain_bios_cd_remove(void)
{
    /* TODO Bind the original A0:72 BIOS CD removal service */
    gt_draft_failure("Native BIOS CD removal adapter", 0x8008B640u);
}
uint32 gtmain_bios_exception_return(void)
{
    /* TODO Bind original B0:17 exception return without fabricating a result */
    gt_draft_failure("Native BIOS exception return adapter", 0x8008B658u);
}
void gtmain_bios_exception_reset(void)
{
    /* TODO Bind the original B0:18 BIOS exception reset service */
    gt_draft_failure("Native BIOS exception reset adapter", 0x8008B668u);
}
