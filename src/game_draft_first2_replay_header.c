#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE 80048568 */
/* Original: GTMAIN.EXE:sub_80048568. */ sint32 gtmain_replay_payload_import(sint32 index, uint32 ghost)
{
    uint32 entry = r_u32(0x800BEA34u) + 52u * (uint32)index;
    uint32 payload = r_u32(0x800BEA30u) + 6016u;
    uint32 bytes = (uint32)r_u16(entry + 42u) << 7u;
    uint32 checksum = r_u32(entry + 56u);
    if (checksum != gtmain_data_checksum(payload, bytes)) {
        fputs((const char *)psx_addr(0x8008DBCCu, 1u), stdout);
        return -1;
    }
    printf((const char *)psx_addr(0x8008DBDCu, 1u), (sint32)checksum);
    if (ghost) {
        fputs((const char *)psx_addr(0x8008DBECu, 1u), stdout);
        gtmain_race_record_publish(payload);
    } else {
        fputs((const char *)psx_addr(0x8008DC10u, 1u), stdout);
        gtmain_race_records_import(payload);
    }
    return 0;
}

/* GTMAIN.EXE 8004A8E8 */
/* Original: GTMAIN.EXE:sub_8004A8E8. */ sint32 gtmain_replay_header_check(uint32 data)
{
    uint32 checksum;
    printf((const char *)psx_addr(0x8008DD10u, 1u));
    checksum = r_u32(data + 5896u);
    w_u32(data + 5896u, 0u);
    if (checksum == gtmain_data_checksum(data, 6016u)) {
        w_u32(data + 5896u, checksum);
        printf((const char *)psx_addr(0x8008DCFCu, 1u), (unsigned)checksum);
        return 0;
    }
    printf((const char *)psx_addr(0x8008DD30u, 1u));
    return -1;
}
