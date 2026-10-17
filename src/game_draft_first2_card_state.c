#include "game_first2_registry.h"

static uint32 card_bios(uint32 service, uint32 argument)
{
    if (service == 0x8008AF28u) return gtmain_bios_critical_exit_result();
    /* TODO Bind actual BIOS service data and meaningful result */
    gt_draft_failure("Native card BIOS service adapter", service);
}
uint32 gtmain_card_init_result(uint32 mode)
{
    (void)card_bios(0x8008B2C8u, 0);
    (void)EnterCriticalSectionPSX();
    if (!card_bios(0x8008B2E4u, 0)) mode = 0;
    gtmain_bios_card_init(mode);
    (void)gtmain_bios_card_patch_copy();
    gtmain_bios_card_patch(); gtmain_bios_card_patch_second();
    return card_bios(0x8008AF28u, 0);
}
uint32 gtmain_card_start_result(void)
{
    (void)EnterCriticalSectionPSX();
    gtmain_bios_card_start();
    (void)card_bios(0x8008B2C8u, 0);
    return card_bios(0x8008AF28u, 0);
}
uint32 gtmain_card_stop(void)
{
    gtmain_bios_card_stop();
    return gtmain_bios_card_patch_restore();
}
void gtmain_bios_card_patch(void)
{
    uint32 table, base, high, low, destination, source = 0x80082718u;
    (void)EnterCriticalSectionPSX();
    table = card_bios(0xB056u, 0); base = r_u32(table + 24u);
    high = r_u32(base + 112u); low = r_u32(base + 116u);
    destination = ((high & 65535u) << 16) + (low & 65535u) + 40u;
    do { uint32 word = r_u32(source); w_u32(destination, word); source += 4u; destination += 4u; } while (source != 0x8008272Cu);
    w_u32(0x0000DFFCu, destination);
    FlushCache();
}
void gtmain_bios_card_patch_second(void)
{
    uint32 table, destination, source = 0x8008272Cu;
    (void)EnterCriticalSectionPSX();
    table = card_bios(0xB057u, 0); destination = r_u32(table + 364u);
    (void)r_u32(destination + 2504u);
    do { uint32 word = r_u32(source); w_u32(destination + 2504u, word); source += 4u; destination += 4u; } while (source != 0x80082740u);
    FlushCache();
}
uint32 gtmain_bios_card_patch_restore(void)
{
    uint32 table, destination, source = 0x80082908u;
    (void)EnterCriticalSectionPSX();
    table = card_bios(0xB056u, 0); destination = r_u32(table + 24u);
    do { uint32 word = r_u32(source); w_u32(destination + 112u, word); source += 4u; destination += 4u; } while (source != 0x80082914u);
    FlushCache();
    return card_bios(0x8008AF28u, 0);
}
