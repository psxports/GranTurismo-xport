#include "game_first2_registry.h"

/* GTMAIN.EXE 8002FE20 */
void gtmain_race_signed_text_draw(uint32 guest_text)
{
    sint32 original = r_s8(guest_text), replacement = original;
    uint32 ordering = r_u32(0x800B6368u);
    uint32 first_color = 0x00808080u, second_color = 0x00808080u;
    sint16 y;
    if (original == '+') {
        first_color = 0x00008080u;
        second_color = 0x00008000u;
        replacement = '-';
    } else if (original == '-') {
        first_color = 0x00008000u;
        second_color = 0x00008080u;
        replacement = '+';
    }
    gtmain_text_font(1);
    gtmain_text_color(first_color);
    if (r_s8(0x800B633Cu)) {
        gtmain_text_draw(ordering, 120, 82, guest_text);
        w_u8(guest_text, (uint8)replacement);
        gtmain_text_color(second_color);
        gtmain_text_draw(ordering, 120, 190, guest_text);
        w_u8(guest_text, (uint8)original);
    } else {
        y = r_s16(0x800B63D6u) ? 106 : 82;
        gtmain_text_draw(ordering, 120, y, guest_text);
    }
}
