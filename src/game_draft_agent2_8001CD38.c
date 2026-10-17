#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified translation from complete pseudocode and original MIPS */
uint32 gtmain_vector_path_sample(uint32 path, uint32 output)
{
    uint32 count = r_u16(path + 2u);
    uint32 sum = 0u, index, segment = 0u, remaining, length;
    sint32 fraction = 0;
    for (index = 0u; index < count; ++index)
        sum += r_u32(path + (index << 4) + 4u);
    if (!sum || count < 2u)
    {
        uint32 x = r_u32(path + 8u);
        uint32 y = r_u32(path + 12u);
        uint32 z = r_u32(path + 16u);
        w_u32(output, x);
        w_u32(output + 4u, y);
        w_u32(output + 8u, z);
        return x;
    }
    remaining = (uint32)(((sint64)(sint32)sum * (sint32)r_u32(0x800B3910u)) >> 12);
    do
    {
        uint32 next = segment + 1u;
        length = r_u32(path + (next << 4) + 4u);
        if ((sint32)remaining < (sint32)length)
            break;
        segment = next;
        remaining -= length;
    } while (segment < count - 1u);
    if (length)
        fraction = (sint32)(uint32)gtmain_signed_divide64(
            (sint64)(sint32)remaining * 4096, (sint32)length, 0x8001CE3Cu);
    if (fraction < 0)
        fraction = 0;
    else if (fraction >= 4096)
        fraction = 4096;
    gtmain_vector_lerp(psx_addr(output, 12u),
        psx_addr(path + (segment << 4) + 8u, 12u),
        psx_addr(path + (segment << 4) + 24u, 12u), fraction);
    return output;
}
