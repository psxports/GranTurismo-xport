#include "game_draft_registry.h"
#include "game_first2_registry.h"

static uint32 gtmain_card_kernel_table(uint32 selector)
{
    /* SCPH-5502 B0 selectors return these initialized kernel tables */
    if (selector == 0x56u) return 0x674u;
    if (selector == 0x57u) return 0x874u;
    return gt_draft_unresolved_call(0xB0u, selector, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void gtmain_card_patch_info(void)
{
    uint32 owner, destination, source;
    xport_bios_enter_critical();
    owner = r_u32(gtmain_card_kernel_table(0x56u) + 0x18u);
    destination = ((r_u32(owner + 0x70u) & 0xFFFFu) << 16)
        + (r_u32(owner + 0x74u) & 0xFFFFu) + 0x28u;
    for (source = 0x80082718u; source != 0x8008272Cu; source += 4u) {
        w_u32(destination, r_u32(source));
        destination += 4u;
    }
    w_u32(0xDFFCu, destination);
    FlushCache();
}

static void gtmain_card_patch_load(void)
{
    uint32 owner, source;
    xport_bios_enter_critical();
    owner = r_u32(gtmain_card_kernel_table(0x57u) + 0x16Cu);
    (void)r_u32(owner + 0x9C8u);
    for (source = 0x8008272Cu; source != 0x80082740u; source += 4u) {
        w_u32(owner + 0x9C8u, r_u32(source));
        owner += 4u;
    }
    FlushCache();
}

void gtmain_bios_clear_event(uint32 event_class, uint32 spec)
{
    uint32 cursor = r_u32(0xA0000120u);
    uint32 end = cursor + (r_u32(0xA0000124u) / 28u) * 28u;
    while (cursor < end) {
        if (r_u32(cursor + 4u) == 0x4000u && r_u32(cursor) == event_class &&
            r_u32(cursor + 8u) == spec && r_u32(cursor + 12u) == 0x2000u)
            w_u32(cursor + 4u, 0x2000u);
        cursor += 28u;
    }
}

void gtmain_card_clear_events(void)
{
    static const uint32 specs[] = {4u, 0x8000u, 0x2000u, 0x100u};
    uint32 index, cursor, end;
    for (index = 0u; index < 4u; ++index) {
        cursor = r_u32(0xA0000120u);
        end = cursor + (r_u32(0xA0000124u) / 28u) * 28u;
        while (cursor < end) {
            if (r_u32(cursor + 4u) == 0x4000u &&
                r_u32(cursor) == 0xF4000001u &&
                r_u32(cursor + 8u) == specs[index] &&
                r_u32(cursor + 12u) == 0x2000u)
                w_u32(cursor + 4u, 0x2000u);
            cursor += 28u;
        }
    }
}

uint32 gtmain_card_filesystem_init(void)
{
    uint32 cursor, word;
    w_u32(0xA0019F24u, 0u);
    w_u32(0xA0019F20u, 0u);
    w_u32(0xA0019F90u, 0u);
    for (cursor = 0xA001B9D0u; cursor != 0xA001B9E4u; cursor += 4u)
        w_u32(cursor, 0u);
    gtmain_card_clear_events();
    for (cursor = 0xA001BA88u; cursor != 0xA001BC68u; cursor += 32u) {
        for (word = 0u; word < 32u; word += 4u) {
            w_u32(cursor + word, 0u);
            w_u32(cursor + 0x1E0u + word, 0u);
        }
        w_u32(cursor, 0xA0u);
        w_u32(cursor + 0x1E0u, 0xA0u);
        w_u16(cursor + 8u, 0xFFFFu);
        w_u16(cursor + 0x1E8u, 0xFFFFu);
    }
    gtmain_card_directory_load(0u);
    gtmain_card_directory_load(16u);
    return 0u;
}

void gtmain_card_start(void)
{
    xport_bios_enter_critical();
    MemCardStart();
    xport_bios_start_card();
    ChangeClearPAD(0u);
    xport_bios_exit_critical();
}

static void gtmain_card_driver_init(uint32 mode)
{
    uint32 source, destination;
    /* InitCARD first installs its interrupt queue callbacks */
    w_u32(0x74ACu, 0x49BCu);
    w_u32(0x74B0u, 0x4A4Cu);
    w_u32(0x74A8u, 0u);
    w_u32(0x74B4u, 0u);
    w_u8(0x755Au, 0u);
    w_u8(0x7264u, 0u);
    w_u8(0x7568u, 1u);
    w_u8(0x7569u, 1u);
    w_u32(0x7544u, 0x4D6Cu);
    w_u32(0x7548u, 0x4F90u);
    w_u32(0x7540u, 0u);
    w_u32(0x754Cu, 0u);
    w_u32(0x75C0u, 0u);
    destination = 0xCF0u;
    for (source = 0x6408u; source != 0x6418u; source += 4u) {
        w_u32(destination, r_u32(source));
        destination += 4u;
    }
    FlushCache();
    w_u32(0x756Cu, 1u);
    w_u32(0x74B8u, mode);
}

void gtmain_card_init(uint32 mode)
{
    uint32 source;
    uint32 destination = 0xDF80u;
    gtmain_card_native_absent_init(mode);
    ChangeClearPAD(0u);
    xport_bios_enter_critical();
    if (!r_u32(0x800B38E4u)) mode = 0u;
    gtmain_card_driver_init(mode);
    for (source = 0x800826A8u; source != 0x80082718u; source += 4u) {
        w_u32(destination, r_u32(source));
        destination += 4u;
    }
    gtmain_card_patch_info();
    gtmain_card_patch_load();
    xport_bios_exit_critical();
}
