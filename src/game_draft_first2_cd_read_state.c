#include "game_first2_registry.h"

uint32 gtmain_cd_read_callback_bind(uint32 token)
{
    uint32 previous = r_u32(0x800B1FE4u);
    w_u32(0x800B1FE4u, token);
    return previous;
}
uint32 gtmain_cd_read_mode(uint32 mode)
{
    uint32 previous = r_u32(0x800B2018u);
    w_u32(0x800B2018u, mode);
    return previous;
}
static sint32 original_sector_completion(uint32 guest_destination, uint32 words)
{
    /* TODO Bind original zero-valued controller completion instead of shared boolean */
    (void)guest_destination; (void)words;
    gt_draft_failure("Native original CD sector completion adapter", 0x800855C8u);
}
uint32 gtmain_cd_sector_read(uint32 guest_destination, uint32 words)
{
    return original_sector_completion(guest_destination, words) == 0;
}
