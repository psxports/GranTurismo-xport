#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include "xport_trace.h"
#include "psx_spu.h"

void gtmain_load_image(void);
void gtmain_load_bios_kernel(void);
uint32 gtmain_launch_stack_bound(void);

/* Original: GTMAIN.EXE:sub_8007BD58. */ uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size)
{
    uint32 cursor = address;
    uint32 pattern = (value & 255u) * 0x01010101u;
    uint32 prefix;
    uint32 words;
    if (!size) return address;
    if (size < 4u) {
        while (size--) w_u8(cursor++, value);
        return address;
    }
    prefix = address & 3u;
    if (prefix) {
        w_masked_u32(address & ~3u, pattern, (15u << prefix) & 15u);
        prefix = 4u - prefix;
        address += prefix;
        size -= prefix;
    }
    cursor = address;
    words = size >> 2;
    while (words--) { w_u32(cursor, pattern); cursor += 4u; }
    if (size & 3u) w_masked_u32(cursor, pattern, (1u << (size & 3u)) - 1u);
    return address;
}

/* Original: GTMAIN.EXE:sub_8007C1D0. */ static uint32 gtmain_heap_initialize(uint32 address, uint32 size)
{
    uint32 available = size < 16u ? 0u : size - 16u;
    uint32 flags;
    w_u32(0x800B1AACu, address);
    w_u32(0x800B1AA8u, size);
    sub_8007BD58(r_u32(0x800B1AACu), 0u, r_u32(0x800B1AA8u));
    address = r_u32(0x800B1AACu);
    flags = r_u32(address + 12u);
    w_u32(address, 0u);
    w_u32(address + 4u, 0u);
    w_u32(address + 8u, available);
    w_u32(address + 12u, flags & ~1u);
    return address;
}

/* Original: GTMAIN.EXE:sub_8007BC84. */ uint32 sub_8007BC84(void)
{
    uint32 start = (0x801FB733u & ~15u) | 0x80000000u;
    uint32 end = ((r_u32(0x8008F1D8u) - r_u32(0x800B1B4Cu)) & ~15u) | 0x80000000u;
    uint32 size = end - start;
    if ((sint32)size < 0) size = 0u;
    gtmain_heap_initialize(start, size);
    w_u8(0x800C0BF0u, 0u);
    w_u32(0x800B1B50u, 1u);
    /* The final empty helper preserves the preceding v0 */
    return 1u;
}

static uint32 gtmain_run_constructors(void)
{
    uint32 count = r_u32(0x801FB710u);
    uint32 cursor = 0x801FB710u;
    uint32 result;
    if (count == 0xFFFFFFFFu) {
        count = 0u;
        if (r_u32(cursor + 4u)) {
            cursor += 4u;
            do { cursor += 4u; ++count; } while (r_u32(cursor));
        }
    }
    result = count << 2;
    if (count) {
        cursor = 0x801FB710u + result;
        do {
            uint32 callback = r_u32(cursor);
            cursor -= 4u;
            --count;
            /* TODO Constructor callbacks need image-qualified native dispatch */
            result = gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        } while (count);
    }
    return result;
}

uint32 sub_80019EC0(uint32 argument0, uint32 argument1)
{
    if (!r_u32(0x800B5F3Cu)) {
        w_u32(0x800B5F3Cu, 1u);
        gtmain_run_constructors();
    }
    /* TODO Draft initialization still contains unresolved SDK boundaries */
    return gt_draft_8001A068();
}

void gtmain_native_main(void)
{
    uint32 cursor = 0x801FB724u;
    uint32 result;
    gtmain_load_image();
    if (!cd_mount_cue("../iso/Gran Turismo (USA) (v1.0).cue"))
        gt_draft_unresolved_call(0x800830F8u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    /* The original launcher bound is only needed by CRT heap sizing */
    w_u32(0x8008F1D8u, gtmain_launch_stack_bound());
    w_u32(0x8008F204u, 0x8008B848u - 0x80010000u);
    w_u32(0x8008F20Cu, 0x800B38F4u - 0x8008F1E0u);
    w_u32(0x8008F214u, 0x801FB724u - 0x800B38F4u);
    /* PSLZ leaves these arguments at the decoded GTMAIN entry */
    w_u32(0x8008F218u, 2u);
    w_u32(0x8008F21Cu, 0x8000FFFFu);
    do {
        cursor -= 4u;
        w_u32(cursor, 0u);
    } while (cursor >= 0x800B38F4u);
    sub_8007BC84();
    result = sub_80019EC0(2u, 0x8000FFFFu);
    /* TODO Translate the original exit/destructor path */
    gt_draft_unresolved_call(0x80082928u, result, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* Original: GTMAIN.EXE:sub_8007C250. */ uint32 gtmain_heap_reset(void)
{
    uint32 start = (0x801FB733u & ~15u) | 0x80000000u;
    uint32 end = ((r_u32(0x8008F1D8u) - r_u32(0x800B1B4Cu)) & ~15u) | 0x80000000u;
    uint32 size = end - start;
    if ((sint32)size < 0) size = 0u;
    return gtmain_heap_initialize(start, size);
}


uint32 gtmain_heap_alloc(uint32 bytes)
{
    uint32 size, block, best = 0, available, flags;
    FUNCTION_MARKER(0x8007C330, "GTMAIN.EXE");
    size = (bytes + 15) & ~15;
    block = r_u32(0x800B1AAC);
    while (block) {
        flags = r_u32(block + 12);
        if (!(flags & 1)) {
            available = r_u32(block + 8);
            if (available >= size && (!best || available < r_u32(best + 8))) best = block;
        }
        block = r_u32(block + 4);
    }
    if (!best) return 0;
    gt_heap_block_split(best, size);
    flags = r_u32(best + 12);
    w_u32(best + 12, flags | 1);
    return best + 16;
}
