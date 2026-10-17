#include "game_first2_registry.h"
#include "psx_gte.h"

sint32 gtmain_light_pair_transform(const uint8 *coefficients, const uint8 *vector)
{
    uint32 first = xport_load_le32(coefficients);
    uint32 second = xport_load_le32(coefficients + 4u);
    gte_ctc2(first, 16u);
    gte_ctc2(second, 17u);
    gte_lwc2(4u, xport_load_le32(vector));
    gte_lwc2(5u, xport_load_le32(vector + 4u));
    xport_gte_execute(0x4D6012u);
    return (sint32)xport_gte_read_data(9u);
}
