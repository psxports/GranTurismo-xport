#include "game_first2_registry.h"

sint32 gtmenu_native_save_action(uint32 writing)
{
    uint32 allocation, payload;
    sint32 result, count, current;
    /* Retain the original 2816C temporary allocation extent */
    allocation = gtmenu_heap_alloc(40960);
    if (!allocation) return -1;
    payload = allocation + 512;
    if (writing) {
        count = r_s16(0x8009B87A);
        current = r_s16(0x8009B878);
        if (count < 0 || count > 100 || current < -1 || current >= count) result = -1;
        else {
            gtmenu_save_payload_export(payload);
            result = xport_save(psx_addr(payload, 27560), 27560);
        }
    } else {
        result = xport_load(psx_addr(payload, 27560), 27560);
        if (result > 0) {
            if (result != 27560 || !gtmenu_save_payload_valid(payload)) result = -1;
            else {
                count = r_s16(payload + 26);
                current = r_s16(payload + 24);
                if (count < 0 || count > 100 || current < -1 || current >= count) result = -1;
                else {
                    gtmenu_loaded_payload_apply(payload);
                    result = 1;
                }
            }
        }
    }
    gtmenu_heap_free(allocation);
    return result;
}
