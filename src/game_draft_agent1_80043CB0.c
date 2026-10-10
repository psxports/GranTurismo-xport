#include "game_first2_registry.h"

static void decimal_put(char buffer[64], sint32 at, uint8 value)
{
    /* TODO Resolve storage contract for precision exceeding native buffer */
    if (at < 0 || at >= 63) gt_draft_failure("Menu decimal native storage", 0x80043CB0u);
    buffer[at] = (char)value;
}

static sint32 decimal_width(uint32 style, const char *text)
{
    /* TODO Bind original style width to native string storage */
    gt_draft_failure("Menu decimal style width", 0x80035FC0u);
}

static void decimal_draw(uint32 ordering, sint32 x, sint32 y, const char *text, uint32 style)
{
    /* TODO Bind original five-argument style draw to native string storage */
    gt_draft_failure("Menu decimal style draw", 0x80036068u);
}

uint32 gtmain_menu_decimal_draw(uint32 ordering, sint32 x, sint32 y,
    sint32 value, sint32 precision, uint32 suffix, uint8 intensity, uint32 style)
{
    char buffer[64];
    sint32 at = 62, count = 0, quotient, draw_x;
    uint32 descriptor = 0, icon_x = (uint32)x;
    uint32 color = intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16) | 0x01000000u;
    buffer[63] = 0;
    while (precision < 0)
    {
        value = (sint32)((uint32)value * 10u);
        ++precision;
    }
    quotient = value / 10;
    do
    {
        uint8 digit = (uint8)((uint32)value + 48u - 10u * (uint32)quotient);
        value /= 10;
        --precision;
        decimal_put(buffer, at, digit);
        quotient = value / 10;
        if (!precision)
        {
            --at; ++count;
            decimal_put(buffer, at, '.');
        }
        if (!value) break;
        ++count; --at;
    } while (count < 16);
    if (precision > 0)
    {
        at = 62 - count - 1;
        do
        {
            ++count; --precision;
            decimal_put(buffer, at, '0');
            if (!precision)
            {
                --at; ++count;
                decimal_put(buffer, at, '.');
            }
            --at;
        } while (precision > 0);
    }
    if (!precision)
    {
        ++count;
        decimal_put(buffer, 62 - count, '0');
    }
    draw_x = x;
    if (style != 1u)
        draw_x = (sint32)((uint32)x - (uint32)decimal_width(style, buffer + 62 - count));
    decimal_draw(ordering, draw_x, y, buffer + 62 - count, style);
    switch (suffix)
    {
    case 1: icon_x -= 4u; descriptor = 12; break;
    case 2: icon_x -= 6u; descriptor = 13; break;
    case 3: icon_x -= 6u; descriptor = 14; break;
    case 4: icon_x -= 4u; descriptor = 15; break;
    case 5: icon_x -= 6u; descriptor = 16; break;
    default: break;
    }
    if (descriptor)
    {
        uint32 record = 0x80092334u + 16u * descriptor;
        uint32 texture = r_u32(record + 12u);
        uint32 dimensions = r_u32(record + 8u);
        gtmain_packet_sprite(ordering, (icon_x & 65535u) | ((uint32)y << 16), texture, dimensions, color);
    }
    return gtmain_packet_page(ordering, 552);
}
