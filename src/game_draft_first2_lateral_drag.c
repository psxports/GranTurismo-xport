#include "game_first2_registry.h"

/* GTMAIN.EXE 8005BECC */
/* Original: GTMAIN.EXE:sub_8005BECC. */ sint32 gtmain_race_lateral_drag_curve(uint32 curve, sint32 speed)
{
    uint32 magnitude;
    sint32 value;
    if (!speed) return 0;
    magnitude = (speed < 0 ? 0u - (uint32)speed : (uint32)speed) >> 3;
    if (magnitude > 32767u) magnitude = 32767u;
    value = (sint16)gtmain_race_curve16(curve, (sint32)magnitude);
    return speed < 0 ? -value : value;
}

/* GTMAIN.EXE 8005CC9C */
/* Original: GTMAIN.EXE:sub_8005CC9C. */ sint32 gtmain_race_lateral_low_speed(sint32 axle_speed, sint32 projected_speed)
{
    sint32 scale, product, attenuation;
    uint32 magnitude;
    if (!projected_speed) return 0;
    if (projected_speed >= 2276) scale = 8192;
    else if (projected_speed < -2275) scale = -8192;
    else {
        product = (sint32)((uint32)projected_speed * 117u * 126u);
        scale = product / 4096;
    }
    magnitude = axle_speed < 0 ? 0u - (uint32)axle_speed : (uint32)axle_speed;
    product = (sint32)((5690u - magnitude) * 2948u);
    attenuation = product / 4096;
    product = (sint32)((uint32)attenuation * (uint32)scale);
    return (sint16)(product / 4096);
}
