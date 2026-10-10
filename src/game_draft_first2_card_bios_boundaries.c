#include "game_first2_registry.h"

void gtmain_bios_card_init(uint32 mode)
{
    /* TODO Bind original BIOS B0:4A card initialization */
    (void)mode; gt_draft_failure("Native BIOS card initialization adapter", 0x80082698u);
}
void gtmain_bios_card_start(void)
{
    /* TODO Bind original BIOS B0:4B card start */
    gt_draft_failure("Native BIOS card start adapter", 0x80082878u);
}
void gtmain_bios_card_stop(void)
{
    /* TODO Bind original BIOS B0:4C card stop */
    gt_draft_failure("Native BIOS card stop adapter", 0x80082888u);
}
