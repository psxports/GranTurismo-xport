#include "game_draft_registry.h"
/* Unverified drafts adapted from existing camera translations */
/* TODO Buffer adapters must expose native arrays to raw guest address callees */

uint32 gt_draft_8001DDD0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 base = r_u32(a0 + 180);
    w_u16(a0 + 264, r_u16(a0 + 264) + 16);
    w_u16(a0 + 266, (uint32)-64);
    w_u32(a0 + 268, 0xFFFA8000u);
    w_u32(a0 + 272, 214);
    w_u16(a0 + 244, r_u16(base + 20));
    gt_draft_8001DF30(a0, 214, 0, 0, 0, 0, 0, 0);
    uint32 vector[3] = {0, 0x10000u, r_u32(a0 + 268)};
    uint32 address = gt_draft_buffer_address(vector, sizeof(vector));
    gt_draft_unresolved_call(0x8006B134u, a0 + 140, (uint32)(sint32)(sint16)r_u16(a0 + 264), (uint32)(sint32)(sint16)r_u16(a0 + 266), 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006B2CCu, address, a0 + 140, address, 0, 0, 0, 0, 0);
    w_u32(a0 + 120, r_u32(base + 2228) + vector[0]);
    w_u32(a0 + 124, r_u32(base + 2232) + vector[1]);
    uint32 result = r_u32(base + 2236) + vector[2];
    w_u32(a0 + 128, result);
    gt_draft_buffer_release(address);
    return result;
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
    uint32 base = r_u32(a0 + 180);
    gt_draft_8001DF30(a0, (uint32)(sint32)(sint16)r_u16(a0 + 262), 0, 0, 0, 0, 0, 0);
    sint32 time = (sint16)r_u16(0x800B6312u);
    w_u16(a0 + 244, r_u16(base + 20));
    if (r_u32(0x800B6318u) != 1) time = (sint32)((uint32)time * 2u);
    time = (sint32)((uint32)time - 120u);
    if (time < 0) time = 0;
    uint32 mode = r_u8(0x800CB75Du);
    uint32 positions = mode == 1 ? 0x8008F278u : 0x8008F254u;
    uint32 angles = mode == 0 ? 0x8008F29Cu : 0x8008F2B4u;
    if (mode > 2) angles = 0x8008F29Cu;
    uint32 intervals = mode == 2 ? 0x8008F2D4u : 0x8008F2CCu;
    uint32 index = 0; sint32 duration;
    for (;;) {
        duration = (sint32)r_u32(intervals + 4u * index);
        if (time < duration) break;
        ++index; time = (sint32)((uint32)time - (uint32)duration);
        if (index >= 3) break;
    }
    uint32 numerator = (uint32)time << 12;
    /* TODO Bind original divide exceptions through a fail-fast adapter */
    if (!duration) gt_draft_exception(7);
    if (duration == -1 && numerator == 0x80000000u) gt_draft_exception(6);
    uint32 fraction = (uint32)((sint32)numerator / duration);
    sint16 current[4], next[4], interpolated[4];
    uint32 table = angles + 8u * index;
    current[1] = (sint16)(r_u16(base + 2242) + r_u16(table + 2));
    current[0] = (sint16)r_u16(table); current[2] = (sint16)r_u16(table + 4);
    next[1] = (sint16)(r_u16(base + 2242) + r_u16(table + 10));
    next[0] = (sint16)r_u16(table + 8); next[2] = (sint16)r_u16(table + 12);
    uint32 out_address = gt_draft_buffer_address(interpolated, sizeof(interpolated));
    uint32 current_address = gt_draft_buffer_address(current, sizeof(current));
    uint32 next_address = gt_draft_buffer_address(next, sizeof(next));
    gt_draft_unresolved_call(0x8006B244u, out_address, current_address, next_address, fraction, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006B134u, a0 + 140, (uint32)(sint32)interpolated[1], (uint32)(sint32)interpolated[0], (uint32)(sint32)interpolated[2], 0, 0, 0, 0);
    uint32 vector[3]; uint32 vector_address = gt_draft_buffer_address(vector, sizeof(vector));
    gt_draft_unresolved_call(0x8006B0A8u, vector_address, positions + 12u * index, positions + 12u * index + 12u, fraction, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006B2CCu, vector_address, a0 + 140, vector_address, 0, 0, 0, 0, 0);
    w_u32(a0 + 120, r_u32(base + 2228) + vector[0]);
    w_u32(a0 + 124, r_u32(base + 2232) + vector[1]);
    uint32 result = r_u32(base + 2236) + vector[2];
    w_u32(a0 + 128, result);
    gt_draft_buffer_release(vector_address); gt_draft_buffer_release(next_address);
    gt_draft_buffer_release(current_address); gt_draft_buffer_release(out_address);
    return result;
}
