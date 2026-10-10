#include "game_first2_registry.h"

/* GTMAIN.EXE:8001CCD4 */
uint32 gtmain_vector_length10(const uint8 *vector)
{
    uint32 x = (uint32)((sint32)xport_load_le32(vector) >> 10);
    uint32 x_squared = x * x;
    uint32 y = (uint32)((sint32)xport_load_le32(vector + 4u) >> 10);
    uint32 y_squared = y * y;
    uint32 z = (uint32)((sint32)xport_load_le32(vector + 8u) >> 10);
    return gtmain_fixed_sqrt(x_squared + y_squared + z * z, 0u);
}
