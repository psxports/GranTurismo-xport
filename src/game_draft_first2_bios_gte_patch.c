#include "game_first2_registry.h"

static uint32 bios_gte_table(void)
{
    /* TODO Obtain the actual guest BIOS B0:56 table address */
    gt_draft_failure("Native BIOS GTE table adapter", 0x8008A168u);
}
void gtmain_bios_gte_patch(void)
{
    uint32 table, destination, cursor, expected, value, actual;
    (void)EnterCriticalSectionPSX();
    table = bios_gte_table();
    destination = r_u32(table + 24u) + 40u;
    cursor = destination; expected = 0x8008A214u;
    for (;;) {
        value = r_u32(expected); actual = r_u32(cursor);
        expected += 4u; cursor += 4u;
        if (value != actual) break;
        if (expected == 0x8008A22Cu) {
            cursor = destination; expected = 0x8008A22Cu;
            do {
                value = r_u32(expected); w_u32(cursor, value);
                expected += 4u; cursor += 4u;
            } while (expected != 0x8008A244u);
            break;
        }
    }
    FlushCache();
    ExitCriticalSection();
}
