#include "game_first2_registry.h"

sint32 gtmain_camera_resource_reset(void)
{
    w_u32(0x8008F224u, 0xFFFFFFFFu);
    return -1;
}

sint32 gtmain_menu_effect_tick(void)
{
    uint32 next = r_u8(0x800B52F8u) - 1u;
    w_u8(0x800B52F8u, next);
    return (sint32)(next << 24) < 1 ? 7 : 0;
}

uint32 gtmain_menu_panel_special_bind(uint32 context, uint32 descriptor)
{
    (void)gtmain_menu_callback(r_u32(descriptor + 16u));
    w_u8(context + 274u, 1u);
    w_u16(context + 266u, 320u);
    w_u16(context + 268u, 240u);
    w_u16(context + 270u, 27u);
    w_u8(context + 256u, 0u);
    w_u32(context + 192u, descriptor);
    w_u16(context + 272u, 12u);
    return 12u;
}

uint32 gtmain_menu_effect_begin(void)
{
    uint32 next;
    if (!(sint8)r_u8(0x800B52F9u)) {
        (void)gtmain_menu_push(r_u32(0x800B52F4u));
        return 3u;
    }
    next = r_u8(0x800B52F8u) - 1u;
    w_u8(0x800B52F8u, next);
    return (sint32)(next << 24) > 0 ? 0u : 7u;
}
