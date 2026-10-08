#ifndef GT_DRAFT_AGENT3_SUPPORT_H
#define GT_DRAFT_AGENT3_SUPPORT_H
#include "game_draft_registry.h"

/* TODO Integrate the original exception handler through its declared adapter */
static sint32 gt3_sdiv(sint32 numerator, sint32 denominator)
{
    if (!denominator) { gt_draft_exception(7u); return 0; }
    if (numerator == (sint32)0x80000000u && denominator == -1)
    { gt_draft_exception(6u); return (sint32)0x80000000u; }
    return numerator / denominator;
}
static sint32 gt3_smod(sint32 numerator, sint32 denominator)
{
    if (!denominator) { gt_draft_exception(7u); return numerator; }
    if (numerator == (sint32)0x80000000u && denominator == -1)
    { gt_draft_exception(6u); return 0; }
    return numerator % denominator;
}
static uint32 gt3_udiv(uint32 numerator, uint32 denominator)
{
    if (!denominator) { gt_draft_exception(7u); return 0xFFFFFFFFu; }
    return numerator / denominator;
}
static uint32 gt3_umod(uint32 numerator, uint32 denominator)
{
    if (!denominator) { gt_draft_exception(7u); return numerator; }
    return numerator % denominator;
}
static uint32 gt3_post_inc(uint32 address, uint32 width)
{
    uint32 value = xport_memory_read(address, width);
    xport_memory_write(address, width, value + 1u, (1u << width) - 1u);
    return value;
}
static uint32 gt3_post_dec(uint32 address, uint32 width)
{
    uint32 value = xport_memory_read(address, width);
    xport_memory_write(address, width, value - 1u, (1u << width) - 1u);
    return value;
}
#endif
