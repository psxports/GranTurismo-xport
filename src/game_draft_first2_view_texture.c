#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_gte.h"

static uint32 view_asr(uint32 value, uint32 shift)
{
    shift &= 31u;
    if (!shift) return value;
    return (value >> shift) | ((value & 0x80000000u) ? (~0u << (32u - shift)) : 0u);
}

/* GTMAIN.EXE 80079064 */
sint32 gtmain_stream_name_find(uint32 name)
{
    sint32 count = gtmain_audio_status(), index;
    if (count <= 0) return -1;
    for (index = 0; index < count; ++index) {
        uint32 candidate = gtmain_cd_directory_name((uint32)index);
        if (!gtmain_string_compare(name, candidate)) return index;
    }
    return -1;
}

/* GTMAIN.EXE 80070EC0 */
void gtmain_texture_page_upload(uint32 image, sint32 page)
{
    PSX_RECT rectangle;
    rectangle.x = (sint16)(((uint32)page & 15u) << 6);
    rectangle.y = (sint16)(view_asr((uint32)page, 4u) << 8);
    rectangle.w = 64; rectangle.h = 256;
    DrawSync(0);
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(image + 96u, 32768u));
    DrawSync(0);
}

/* GTMAIN.EXE 80070F24 */
void gtmain_texture_palette_upload(uint32 image, sint32 palette, uint32 index)
{
    PSX_RECT rectangle;
    rectangle.x = (sint16)(((uint32)palette & 63u) << 4);
    rectangle.y = (sint16)view_asr((uint32)palette, 6u);
    rectangle.w = 64; rectangle.h = 4;
    DrawSync(0);
    LoadImagePSX(&rectangle, (uint32 *)psx_addr(image + (index << 9) + 32864u, 512u));
    DrawSync(0);
}

/* GTMAIN.EXE 80079250 */
sint32 gtmain_render_view_bind(uint32 destination, uint32 source)
{
    uint32 first, second;
    sint32 result;
    w_u32(destination, source);
    first = r_u32(source + 76u); second = r_u32(source + 80u);
    w_u32(destination + 116u, first);
    first = r_u32(source + 84u); w_u32(destination + 120u, second);
    second = r_u32(source + 88u); w_u32(destination + 124u, first);
    first = r_u32(source + 92u); w_u32(destination + 128u, second);
    w_u32(destination + 132u, first);
    first = r_u32(source + 96u); second = r_u32(source + 100u);
    w_u32(destination + 136u, first);
    first = r_u32(source + 104u); w_u32(destination + 140u, second);
    second = r_u32(source + 108u); w_u32(destination + 144u, first);
    first = r_u32(source + 112u); w_u32(destination + 148u, second);
    w_u32(destination + 152u, first);
    first = r_u16(source + 116u); second = r_u16(source + 118u);
    w_u16(destination + 8u, (uint16)first); w_u16(destination + 10u, (uint16)second);
    first = r_u16(source + 8u); second = r_u16(source + 10u);
    w_u32(destination + 24u, first << 16); w_u32(destination + 28u, second << 16);
    first = r_u16(source + 12u); result = (sint16)r_u16(source + 18u);
    w_u16(destination + 32u, (uint16)first);
    second = r_u16(source + 20u);
    w_u16(destination + 4u, (uint16)result); w_u16(destination + 6u, (uint16)second);
    return result;
}

/* GTMAIN.EXE 80070448 */
uint32 gtmain_render_view_copy(uint32 destination, uint32 source)
{
    uint32 first, second, third, fourth;
    gtmain_render_view_bind(destination, source);
    first = r_u32(source + 120u); second = r_u32(source + 124u); third = r_u32(source + 128u);
    w_u32(destination + 44u, first); w_u32(destination + 48u, second); w_u32(destination + 52u, third);
    first = r_u32(source + 160u); second = r_u32(source + 164u);
    third = r_u32(source + 168u); fourth = r_u32(source + 172u);
    w_u32(destination + 64u, first); w_u32(destination + 68u, second);
    w_u32(destination + 72u, third); w_u32(destination + 76u, fourth);
    first = r_u32(source + 176u);
    w_u32(destination + 80u, first);
    return first;
}

/* GTMAIN.EXE 80071FA4 */
sint32 gtmain_projection_prepare(uint32 context)
{
    uint32 right = r_u16(context + 4u), left = r_u16(context);
    uint32 bottom = r_u16(context + 6u), top = r_u16(context + 2u);
    sint32 numerator, divisor, quotient;
    uint32 depth, result;
    sint64 product;
    w_u16(context + 18u, (uint16)(right - left));
    w_u16(context + 20u, (uint16)(bottom - top));
    gtmain_projection_horizontal(context);
    gtmain_projection_vertical(context);
    numerator = (sint32)((uint32)(sint32)(sint16)r_u16(context + 12u) << 13);
    divisor = (sint16)r_u16(context + 18u);
    if (!divisor) gt_draft_exception(7u);
    if (divisor == -1 && (uint32)numerator == 0x80000000u) gt_draft_exception(6u);
    quotient = numerator / divisor;
    product = (sint64)quotient * quotient;
    depth = r_u16(context + 12u);
    w_u16(context + 118u, 0x7FFFu);
    w_u16(context + 116u, (uint16)(view_asr((uint32)(sint32)(sint16)depth, 1u) + 1u));
    w_u32(context + 24u, (uint32)quotient);
    w_u32(context + 28u, (uint32)((uint64)product >> 12));
    /* Preserve LZCS and LZCR through the existing shared runtime */
    gt_draft_gte_write(30u, 0u, (uint32)quotient);
    result = 31u - gt_draft_gte_read(31u, 0u);
    w_u32(context + 32u, result);
    return (sint32)result;
}

/* GTMAIN.EXE 8007178C */
sint32 gtmain_angle_atan_wide(sint32 x, sint32 y)
{
    uint32 a = (uint32)x, b = (uint32)y;
    uint32 magnitude = (b ^ view_asr(b, 31u)) | (a ^ view_asr(a, 31u));
    sint32 shift;
    /* Preserve LZCS and LZCR through the existing shared runtime */
    gt_draft_gte_write(30u, 0u, magnitude);
    shift = (sint32)(18u - gt_draft_gte_read(31u, 0u));
    if (shift > 0) { b = view_asr(b, (uint32)shift); a = view_asr(a, (uint32)shift); }
    return (sint16)gtmain_angle_atan((sint16)a, (sint16)b);
}

/* GTMAIN.EXE 800704BC */
uint32 gtmain_render_fog_set(uint32 context, uint32 rgb, uint32 near, uint32 far)
{
    uint32 first = (rgb << 4) & 0xFF0u, second = (rgb >> 4) & 0xFF0u;
    uint32 third = (rgb >> 12) & 0xFF0u;
    gte_ctc2(first, 21u); gte_ctc2(second, 21u); gte_ctc2(third, 21u);
    gte_ctc2(near, 27u); gte_ctc2(far, 28u);
    w_u32(context + 12u, first); w_u32(context + 16u, second); w_u32(context + 20u, third);
    w_u16(context + 34u, (uint16)near); w_u32(context + 36u, far);
    return second;
}
