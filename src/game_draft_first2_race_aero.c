#include "game_first2_registry.h"
#include <string.h>

static sint32 aero_fixed(uint32 product)
{
    if ((sint32)product < 0) product += 4095u;
    return (sint32)product >> 12;
}

static sint32 aero_square(sint32 speed)
{
    uint32 magnitude = (uint32)speed;
    sint32 scaled;
    if (speed < 0) magnitude = 0u - magnitude;
    scaled = (sint32)magnitude >> 5;
    return aero_fixed((uint32)scaled * (uint32)scaled);
}

sint32 gtmain_race_aero_force(uint32 car, sint32 speed)
{
    sint32 square = aero_square(speed);
    sint32 force = aero_fixed((uint32)square * r_u32(car + 0x4E4u));
    return speed < 0 ? force : (sint32)(0u - (uint32)force);
}

sint32 gtmain_race_aero_components(uint32 car, sint32 speed, sint32 *first, sint32 *second)
{
    sint32 square = aero_square(speed);
    sint32 force = aero_fixed((uint32)square * r_u32(car + 0x4E8u));
    memcpy(first, &force, sizeof(force));
    force = aero_fixed((uint32)square * r_u32(car + 0x4ECu));
    memcpy(second, &force, sizeof(force));
    return force;
}
