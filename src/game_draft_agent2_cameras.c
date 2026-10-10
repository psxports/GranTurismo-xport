#include "game_draft_registry.h"
#include "game_first2_registry.h"
/* Unverified drafts adapted from existing camera translations */
/* TODO Buffer adapters must expose native arrays to raw guest address callees */

uint32 gt_draft_8001DDD0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_camera_orbit_update(a0);
}

uint32 gt_draft_8001D4DC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 base = r_u32(a0 + 180), config = r_u32(0x800B6400u);
    uint32 params = r_u32(r_u32(base + 2336) + 68);
    gt_draft_8001D308(a0 + 120, config + 28, 0, 0, 0, 0, 0, 0);
    w_u16(a0 + 244, gt_draft_80023DCC(r_u16(config + 6), a0 + 120, 0, 0, 0, 0, 0, 0));
    uint32 vector[3];
    vector[0] = r_u32(base + 2228) - r_u32(a0 + 120);
    vector[1] = r_u32(base + 2232) - r_u32(a0 + 124);
    uint32 z = r_u32(base + 2236) - r_u32(a0 + 128);
    vector[1] += 52428u; vector[2] = z;
    uint32 address = gt_draft_buffer_address(vector, sizeof(vector));
    gt_draft_8001C870(a0 + 140, address, 0, 0, 0, 0, 0, 0);
    uint32 length = gt_draft_8001CCD4(address, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_buffer_release(address);
    sint32 shift = (sint16)r_u16(params + 76) - 16;
    uint32 scaled = length * 10496u;
    scaled = shift < 0 ? scaled << ((0u - (uint32)shift) & 31u) : (uint32)((sint32)scaled >> ((uint32)shift & 31u));
    sint32 divisor = (sint16)r_u16(params + 78);
    /* TODO Bind original divide exceptions through a fail-fast adapter */
    if (!divisor) gt_draft_exception(7);
    if (scaled == 0x80000000u && divisor == -1) gt_draft_exception(6);
    uint32 quotient = (uint32)((sint32)scaled / divisor);
    sint32 delay = (sint32)(quotient * r_u16(config + 20)) >> 12;
    sint32 upper = r_u16(config + 16), lower = r_u16(config + 18);
    if (upper < delay) delay = upper;
    else if (delay < lower) delay = lower;
    return gt_draft_8001DF30(a0, (uint32)delay, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_8001D654(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_camera_countdown_update(a0);
}

uint32 gtmain_camera_countdown_update(uint32 camera)
{
    uint32 car = r_u32(camera + 180u), positions, angles, intervals, kind, index = 0, table, result;
    sint32 time, duration, fraction;
    uint8 current[6], next[6], interpolated[6], vector[12];
    gtmain_camera_projection_set(camera, r_s16(camera + 262u));
    time = r_s16(0x800B6312u);
    result = r_u16(car + 20u);
    kind = r_u32(0x800B6318u);
    w_u16(camera + 244u, result);
    if (kind != 1u) time *= 2;
    time -= 120;
    if (time < 0) time = 0;
    kind = r_u8(0x800CB75Du);
    positions = kind == 1u ? 0x8008F278u : 0x8008F254u;
    angles = kind == 1u || kind == 2u ? 0x8008F2B4u : 0x8008F29Cu;
    intervals = kind == 2u ? 0x8008F2D4u : 0x8008F2CCu;
    for (;;) {
        duration = r_s32(intervals + 4u * index);
        if (time < duration) break;
        ++index; time = (sint32)((uint32)time - (uint32)duration);
        if (index >= 3u) break;
    }
    result = (uint32)time << 12;
    if (!duration) { gt_draft_exception(7u); return 0; }
    if (duration == -1 && result == 0x80000000u) { gt_draft_exception(6u); return 0; }
    fraction = (sint32)result / duration;
    table = angles + 8u * index;
    xport_store_le16(current + 2u, (uint16)(r_u16(car + 2242u) + r_u16(table + 2u)));
    xport_store_le16(current, r_u16(table)); xport_store_le16(current + 4u, r_u16(table + 4u));
    xport_store_le16(next + 2u, (uint16)(r_u16(car + 2242u) + r_u16(table + 10u)));
    xport_store_le16(next, r_u16(table + 8u)); xport_store_le16(next + 4u, r_u16(table + 12u));
    gtmain_angles_lerp(interpolated, current, next, fraction);
    gtmain_model_rotation(psx_addr(camera + 140u, 20u), (sint16)xport_load_le16(interpolated + 2u),
        (sint16)xport_load_le16(interpolated), (sint16)xport_load_le16(interpolated + 4u));
    gtmain_vector_lerp(vector, psx_addr(positions + 12u * index, 12u),
        psx_addr(positions + 12u * index + 12u, 12u), fraction);
    gtmain_model_rotate_vector(vector, psx_addr(camera + 140u, 20u), vector);
    w_u32(camera + 120u, r_u32(car + 2228u) + xport_load_le32(vector));
    w_u32(camera + 124u, r_u32(car + 2232u) + xport_load_le32(vector + 4u));
    result = r_u32(car + 2236u) + xport_load_le32(vector + 8u);
    w_u32(camera + 128u, result);
    return result;
}

uint32 gtmain_camera_countdown_mode(uint32 camera)
{
    if (r_s16(0x800B63DEu) < 2) w_u8(camera + 253u, 0);
    return gtmain_camera_countdown_update(camera);
}

uint32 gtmain_camera_orbit_update(uint32 camera)
{
    uint32 car = r_u32(camera + 180u), angle = r_u16(camera + 264u), yaw = r_u16(car + 20u), result;
    uint8 vector[12];
    w_u16(camera + 264u, angle + 16u); w_u16(camera + 266u, (uint16)-64);
    w_u32(camera + 268u, 0xFFFA8000u); w_u32(camera + 272u, 214u);
    w_u16(camera + 244u, yaw);
    gtmain_camera_projection_set(camera, 214);
    xport_store_le32(vector, 0); xport_store_le32(vector + 4u, 0x10000u);
    xport_store_le32(vector + 8u, r_u32(camera + 268u));
    gtmain_model_rotation(psx_addr(camera + 140u, 20u), r_s16(camera + 264u), r_s16(camera + 266u), 0);
    gtmain_model_rotate_vector(vector, psx_addr(camera + 140u, 20u), vector);
    w_u32(camera + 120u, r_u32(car + 2228u) + xport_load_le32(vector));
    w_u32(camera + 124u, r_u32(car + 2232u) + xport_load_le32(vector + 4u));
    result = r_u32(car + 2236u) + xport_load_le32(vector + 8u);
    w_u32(camera + 128u, result);
    return result;
}
