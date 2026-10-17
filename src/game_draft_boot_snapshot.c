#include "game_draft_registry.h"
#include "psx_spu.h"
#include <string.h>

/* Cold native options from SCUS_941.94 800102DC and80010B98 */
void gt_native_boot_snapshot_seed(void)
{
    static uint8 snapshot[0xB3C0u];
    static const uint8 controls[40] = {0x0Au, 0x0Bu, 0x07u, 0x0Eu, 0x0Fu, 0x0Cu, 0x09u, 0x08u, 0x0Du, 0x0Au, 0x0Bu, 0x10u, 0x11u, 0x12u, 0x0Cu, 0x04u, 0x06u, 0x0Du, 0x80u, 0x08u, 0x40u, 0x10u, 0xC0u, 0x10u, 0xC0u, 0x0Au, 0x0Bu, 0x12u, 0x0Eu, 0x0Fu, 0x0Cu, 0x09u, 0x08u, 0x0Du, 0x00u, 0xFFu, 0x00u, 0xFFu, 0x0Eu, 0x0Fu};
    uint32 i, bit, sum = 0xAAAAu, crc = 0x3770u;
    memset(snapshot, 0, sizeof(snapshot));
    xport_store_le32(snapshot, 0x47546F73u);
    xport_store_le16(snapshot + 0x6B5Au, 3809u);
    xport_store_le16(snapshot + 0x6B58u, 214u);
    snapshot[0x6B5Du] = 2u;
    snapshot[0x6B67u] = 255u; snapshot[0x6B68u] = 255u;
    snapshot[0x6B6Bu] = 1u; snapshot[0x6B76u] = 1u;
    snapshot[0x6B6Du] = 1u; snapshot[0x6B79u] = 1u;
    snapshot[0x6B63u] = 1u;
    snapshot[0x6B82u] = 1u; memcpy(snapshot + 0x6B83u, controls, sizeof(controls));
    snapshot[0x6BABu] = 1u; memcpy(snapshot + 0x6BACu, controls, sizeof(controls));
    /* TODO Translate the boot garage/default-name initialization for garage modes */
    for (i = 0u; i < 0xB398u; ++i) {
        uint32 value = snapshot[i];
        sum = (sum + value) ^ (value << 8u);
        for (bit = 0u; bit < 8u; ++bit) {
            crc <<= 1u;
            if (crc & 0x10000u) crc ^= 0x11021u;
            crc |= (value >> 7u) & 1u; value <<= 1u;
        }
    }
    xport_store_le32(snapshot + 0xB398u, (sum << 16u) | (crc & 65535u));
    if (!spu_upload(0x2000u, snapshot, sizeof(snapshot)))
        gt_draft_failure("Native boot GAMESTATUS upload", 0x2000u);
}
