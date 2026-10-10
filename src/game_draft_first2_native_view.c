#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct GTNativeViewBinding
{
    uint32 context;
    uint8 bytes[184];
    struct GTNativeViewBinding *next;
} GTNativeViewBinding;

static GTNativeViewBinding *native_views;

uint8 *gtmain_render_source(uint32 context)
{
    uint32 source = r_u32(context);
    GTNativeViewBinding *binding;
    for (binding = native_views; binding; binding = binding->next)
        if (binding->context == context && source == (uint32)(uintptr_t)binding->bytes)
            return binding->bytes;
    return (uint8 *)psx_addr(source, 184u);
}

uint32 gtmain_render_view_copy_native(uint32 context, const uint8 *view)
{
    GTNativeViewBinding *binding;
    uint32 first, second, third, fourth;
    for (binding = native_views; binding && binding->context != context; binding = binding->next) {}
    if (!binding)
    {
        binding = (GTNativeViewBinding *)malloc(sizeof(*binding));
        if (!binding) gt_draft_failure("Native view allocation", 0x80070448u);
        binding->context = context; binding->next = native_views; native_views = binding;
    }
    /* Keep the published source alive after its caller returns */
    memcpy(binding->bytes, view, sizeof(binding->bytes));
    w_u32(context, (uint32)(uintptr_t)binding->bytes);
    first = xport_load_le32(view + 76u); second = xport_load_le32(view + 80u);
    w_u32(context + 116u, first); first = xport_load_le32(view + 84u);
    w_u32(context + 120u, second); second = xport_load_le32(view + 88u);
    w_u32(context + 124u, first); first = xport_load_le32(view + 92u);
    w_u32(context + 128u, second); w_u32(context + 132u, first);
    first = xport_load_le32(view + 96u); second = xport_load_le32(view + 100u);
    w_u32(context + 136u, first); first = xport_load_le32(view + 104u);
    w_u32(context + 140u, second); second = xport_load_le32(view + 108u);
    w_u32(context + 144u, first); first = xport_load_le32(view + 112u);
    w_u32(context + 148u, second); w_u32(context + 152u, first);
    first = xport_load_le16(view + 116u); second = xport_load_le16(view + 118u);
    w_u16(context + 8u, first); w_u16(context + 10u, second);
    first = (uint32)(sint32)(sint16)xport_load_le16(view + 8u);
    second = (uint32)(sint32)(sint16)xport_load_le16(view + 10u);
    w_u32(context + 24u, first << 16); w_u32(context + 28u, second << 16);
    first = xport_load_le16(view + 12u); second = xport_load_le16(view + 18u);
    w_u16(context + 32u, first); first = xport_load_le16(view + 20u);
    w_u16(context + 4u, second); w_u16(context + 6u, first);
    first = xport_load_le32(view + 120u); second = xport_load_le32(view + 124u); third = xport_load_le32(view + 128u);
    w_u32(context + 44u, first); w_u32(context + 48u, second); w_u32(context + 52u, third);
    first = xport_load_le32(view + 160u); second = xport_load_le32(view + 164u);
    third = xport_load_le32(view + 168u); fourth = xport_load_le32(view + 172u);
    w_u32(context + 64u, first); w_u32(context + 68u, second);
    w_u32(context + 72u, third); w_u32(context + 76u, fourth);
    first = xport_load_le32(view + 176u); w_u32(context + 80u, first);
    return first;
}

static uint32 view_asr(uint32 value, uint32 shift)
{
    return (uint32)((sint32)value >> (shift & 31u));
}

static void projection_normalize(uint32 *scratch)
{
    (void)gtmain_vector_normalize_xy((uint8 *)scratch, (const uint8 *)scratch);
}

static sint32 rectangle_divide(uint32 numerator, sint32 denominator, uint32 address)
{
    if (!denominator) gt_draft_failure("Original rectangle BREAK 7", address);
    if (numerator == 0x80000000u && denominator == -1) gt_draft_failure("Original rectangle BREAK 6", address);
    return (sint32)numerator / denominator;
}

sint32 gtmain_projection_horizontal_native(uint8 *context)
{
    uint32 scratch[8];
    sint32 left = (sint16)xport_load_le16(context), right = (sint16)xport_load_le16(context + 4u);
    sint32 center = (sint16)xport_load_le16(context + 8u), margin = (sint16)xport_load_le16(context + 14u);
    sint32 depth = (sint16)xport_load_le16(context + 12u), first, second;
    right = right - center + margin;
    scratch[0] = (uint16)(left - center - margin) | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    xport_store_le16(context + 78u, 0u);
    xport_store_le16(context + 76u, (uint16)second);
    xport_store_le16(context + 80u, (uint16)(0u - (uint32)first));
    scratch[0] = (uint16)right | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    xport_store_le16(context + 84u, 0u);
    xport_store_le16(context + 88u, 0u);
    xport_store_le16(context + 90u, 0u);
    xport_store_le16(context + 92u, 4096u);
    xport_store_le16(context + 82u, (uint16)(0u - (uint32)second));
    xport_store_le16(context + 86u, (uint16)first);
    return 4096;
}

sint32 gtmain_projection_vertical_native(uint8 *context)
{
    uint32 scratch[8];
    sint32 top = (sint16)xport_load_le16(context + 2u), bottom = (sint16)xport_load_le16(context + 6u);
    sint32 center = (sint16)xport_load_le16(context + 10u), margin = (sint16)xport_load_le16(context + 16u);
    sint32 depth = (sint16)xport_load_le16(context + 12u), first, second;
    bottom = bottom - center + margin;
    scratch[0] = (uint16)(top - center - margin) | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    xport_store_le16(context + 96u, 0u);
    xport_store_le16(context + 98u, (uint16)second);
    xport_store_le16(context + 100u, (uint16)(0u - (uint32)first));
    scratch[0] = (uint16)bottom | ((uint32)(uint16)depth << 16);
    projection_normalize(scratch);
    first = (sint16)scratch[0]; second = (sint16)(scratch[0] >> 16);
    xport_store_le16(context + 102u, 0u);
    xport_store_le16(context + 108u, 0u);
    xport_store_le16(context + 110u, 0u);
    xport_store_le16(context + 112u, 0xF000u);
    xport_store_le16(context + 104u, (uint16)(0u - (uint32)second));
    xport_store_le16(context + 106u, (uint16)first);
    return -4096;
}

sint32 gtmain_projection_prepare_native(uint8 *context)
{
    uint32 right = xport_load_le16(context + 4u), left = xport_load_le16(context);
    uint32 bottom = xport_load_le16(context + 6u), top = xport_load_le16(context + 2u);
    sint32 numerator, divisor, quotient;
    uint32 depth, result;
    sint64 product;
    xport_store_le16(context + 18u, (uint16)(right - left));
    xport_store_le16(context + 20u, (uint16)(bottom - top));
    gtmain_projection_horizontal_native(context);
    gtmain_projection_vertical_native(context);
    numerator = (sint32)((uint32)(sint32)(sint16)xport_load_le16(context + 12u) << 13);
    divisor = (sint16)xport_load_le16(context + 18u);
    if (!divisor) gt_draft_exception(7u);
    if (divisor == -1 && (uint32)numerator == 0x80000000u) gt_draft_exception(6u);
    quotient = numerator / divisor;
    product = (sint64)quotient * quotient;
    depth = xport_load_le16(context + 12u);
    xport_store_le16(context + 118u, 0x7FFFu);
    xport_store_le16(context + 116u, (uint16)(view_asr((uint32)(sint32)(sint16)depth, 1u) + 1u));
    xport_store_le32(context + 24u, (uint32)quotient);
    xport_store_le32(context + 28u, (uint32)((uint64)product >> 12));
    /* Preserve LZCS and LZCR through the existing shared runtime */
    gt_draft_gte_write(30u, 0u, (uint32)quotient);
    result = 31u - gt_draft_gte_read(31u, 0u);
    xport_store_le32(context + 32u, result);
    return (sint32)result;
}

uint32 gtmain_view_rectangle_prepare_native(uint8 *view)
{
    uint32 a, b, values[9], result;
    sint32 left, right, top, bottom, denominator;
    a = (uint32)(sint32)(sint16)xport_load_le16(view); b = (uint32)(sint32)(sint16)xport_load_le16(view + 8u);
    denominator = (sint16)xport_load_le16(view + 12u); left = rectangle_divide((a - b) << 12, denominator, 0x800720A4u);
    a = (uint32)(sint32)(sint16)xport_load_le16(view + 4u); b = (uint32)(sint32)(sint16)xport_load_le16(view + 8u);
    denominator = (sint16)xport_load_le16(view + 12u); right = rectangle_divide((a - b) << 12, denominator, 0x800720E8u);
    a = (uint32)(sint32)(sint16)xport_load_le16(view + 10u); b = (uint32)(sint32)(sint16)xport_load_le16(view + 2u);
    denominator = (sint16)xport_load_le16(view + 12u); top = rectangle_divide((a - b) << 12, denominator, 0x8007212Cu);
    a = (uint32)(sint32)(sint16)xport_load_le16(view + 10u); b = (uint32)(sint32)(sint16)xport_load_le16(view + 6u);
    denominator = (sint16)xport_load_le16(view + 12u); bottom = rectangle_divide((a - b) << 12, denominator, 0x80072170u);
    values[0] = xport_load_le16(view + 2u); values[1] = xport_load_le16(view + 4u); values[2] = xport_load_le16(view + 2u);
    values[3] = xport_load_le16(view); values[4] = xport_load_le16(view + 6u); values[5] = xport_load_le16(view + 4u);
    values[6] = xport_load_le16(view + 6u); values[7] = xport_load_le16(view); values[8] = xport_load_le16(view);
    xport_store_le16(view + 46u, values[0]); xport_store_le16(view + 48u, values[1]); xport_store_le16(view + 50u, values[2]);
    xport_store_le16(view + 52u, values[3]); xport_store_le16(view + 54u, values[4]); xport_store_le16(view + 56u, values[5]);
    xport_store_le16(view + 58u, values[6]); xport_store_le16(view + 44u, values[8]);
    xport_store_le16(view + 36u, left); xport_store_le16(view + 40u, right); xport_store_le16(view + 38u, top); xport_store_le16(view + 42u, bottom);
    xport_store_le16(view + 60u, values[7]); result = xport_load_le16(view + 2u); a = xport_load_le16(view + 4u); b = xport_load_le16(view + 6u);
    xport_store_le16(view + 66u, result); xport_store_le16(view + 68u, a); xport_store_le16(view + 74u, b); return result;
}

uint32 gtmain_view_projection_prepare_native(uint8 *view)
{
    (void)gtmain_projection_prepare_native(view);
    return gtmain_view_rectangle_prepare_native(view);
}

sint32 gtmain_model_orientation_native(uint8 *view, uint32 reference_nullable)
{
    sint32 result;
    (void)gtmain_matrix_angles(view + 132u, view + 140u);
    result = (sint32)gtmain_matrix_transpose(view + 160u, view + 140u);
    if (reference_nullable)
        result = gtmain_matrix_multiply(view + 160u, (const uint8 *)psx_addr(reference_nullable, 20u), view + 160u);
    return result;
}

uint32 gtmain_view_plane_intersections_native(uint8 *view, const uint8 *plane)
{
    sint32 px = (sint16)xport_load_le16(plane), py = (sint16)xport_load_le16(plane + 2u);
    uint32 a = (uint32)(px * (sint16)xport_load_le16(view + 36u));
    uint32 b = (uint32)(px * (sint16)xport_load_le16(view + 40u));
    uint32 c = (uint32)(py * (sint16)xport_load_le16(view + 42u));
    uint32 d = (uint32)(py * (sint16)xport_load_le16(view + 38u));
    uint32 bias = xport_load_le16(plane + 4u), values[4], flags = 0;
    values[0] = bias + (uint32)((sint32)a >> 12) + (uint32)((sint32)d >> 12);
    values[1] = bias + (b >> 12) + (uint32)((sint32)d >> 12);
    values[2] = bias + (uint32)((sint32)a >> 12) + (c >> 12);
    values[3] = bias + (b >> 12) + (c >> 12);
    if (!(values[0] & 0x8000u)) flags = 0x31u;
    if (!(values[1] & 0x8000u)) flags = (flags | 2u) ^ 0x60u;
    if (!(values[2] & 0x8000u)) flags = (flags | 4u) ^ 0x90u;
    if (!(values[3] & 0x8000u)) flags = (flags | 8u) ^ 0xC0u;
    if (flags & 0x10u)
    {
        sint32 first = (sint16)values[0], second = (sint16)values[2];
        sint32 fraction = rectangle_divide((sint16)xport_load_le16(view + 20u) * first, second - first, 0x8006A48Cu);
        xport_store_le16(view + 62u, xport_load_le16(view + 2u) - (uint32)fraction);
    }
    if (flags & 0x20u)
    {
        sint32 first = (sint16)values[0], second = (sint16)values[1];
        sint32 fraction = rectangle_divide((sint16)xport_load_le16(view + 18u) * first, second - first, 0x8006A4F0u);
        xport_store_le16(view + 64u, xport_load_le16(view) - (uint32)fraction);
    }
    if (flags & 0x40u)
    {
        sint32 first = (sint16)values[1], second = (sint16)values[3];
        sint32 fraction = rectangle_divide((sint16)xport_load_le16(view + 20u) * first, second - first, 0x8006A554u);
        xport_store_le16(view + 70u, xport_load_le16(view + 2u) - (uint32)fraction);
    }
    if (flags & 0x80u)
    {
        sint32 first = (sint16)values[2], second = (sint16)values[3];
        sint32 fraction = rectangle_divide((sint16)xport_load_le16(view + 18u) * first, second - first, 0x8006A5B8u);
        xport_store_le16(view + 72u, xport_load_le16(view) - (uint32)fraction);
    }
    return flags & 15u;
}

