#include "game_first2_registry.h"
#include "game_draft_registry.h"
/* Unverified draft translations */
/* TODO Resolve unspecified outgoing argument carriers during integration */

static void menu_status_asset_cleanup(void)
{
    /* TODO Bind unrecorded original menu asset cleanup */
    (void)gt_draft_unresolved_call(0x8003A2DCu, 0, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_8002C894(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    char buffer[8192];
    uint32 address = gt_draft_buffer_address(buffer, sizeof(buffer));
    gt_draft_unresolved_call(0x80070598u, address, 0x8008FB5Cu, 0, 0, 0, 0, 0, 0);
    uint32 result = gt_draft_unresolved_call(0x800705F4u, address, 0x7C01A0u, 0xFFFFFFFFu, 0x6201A0u, 0xFFFFFFFFu, 0, 0, 0);
    gt_draft_buffer_release(address);
    return result;
}

uint32 gt_draft_8002C944(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_race_meter_rectangle_draw(a0, (sint16)a1, (sint16)a2, (sint32)a3);
}

/* GTMAIN.EXE 8003CC54 */
uint32 gtmain_menu_status_input(void)
{
    uint32 input = r_u32(0x800BDD08u), context;
    sint32 phase;
    gtmain_menu_stream_check();
    if (!(sint8)r_u8(input)) return 0u;
    context = r_u32(0x800B52FCu);
    w_u8(context + 8u, r_u8(context + 8u) + 1u);
    context = r_u32(0x800B52FCu);
    w_u8(context + 8u, r_u8(context + 8u) & 1u);
    context = r_u32(0x800B52FCu);
    phase = (sint8)r_u8(context);
    if (phase != 60) {
        w_u8(context, (uint32)phase + 1u);
        return 0u;
    }
    if (!(r_u16(input + 6u) & 0xFF0Fu)) return 0u;
    (void)gtmain_menu_sound(4);
    context = r_u32(0x800B52FCu);
    (void)gtmain_menu_push(r_u32(context + 4u));
    context = r_u32(0x800B52FCu);
    if ((sint8)r_u8(context + 1u)) {
        menu_status_asset_cleanup();
    }
    return 3u;
}

uint32 gt_draft_80041FE0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    w_u8(0x800928A0u, r_u8(0x800928A0u) & 31u);
    uint32 count = r_u8(r_u32(0x800BEA60u) + 42);
    w_u8(0x80092864u, count + 1);
    for (uint32 i = 0; i < count; ++i) {
        w_u32(0x80092868u + 4u * i, r_u32(0x80092844u + 4u * i));
        w_u16(0x80092888u + 2u * i, r_u16(r_u32(0x800BEA60u) + 2u * i + 26));
    }
    w_u32(0x80092868u + 4u * count, 0x800BD29Cu);
    w_u16(0x80092888u + 2u * count, r_u16(r_u32(0x800BEA60u) + 40));
    return 3;
}

uint32 gt_draft_8002FCD4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    (void)a4; (void)a5; (void)a6; (void)a7;
    return (uint32)gtmain_race_lap_counter_draw(a0, (sint32)a1, (sint32)a2, a3);
}

uint32 gt_draft_80030CA0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_race_choice_boxes_draw();
}

uint32 gt_draft_8001E6D4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_car_selection_bind(a0, a1);
}

static uint32 installed_parts_resource(uint32 category)
{
    /* TODO Bind unselected parts resource lookup 8001EB94 */
    return gt_draft_unresolved_call(0x8001EB94u, category,
        0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

uint32 gt_draft_80042F1C(void)
{
    uint32 first = installed_parts_resource(31u);
    uint32 second = installed_parts_resource(32u);
    uint32 first_offset = r_u8(0x800941DCu);
    uint32 owner = r_u32(0x800BEA64u);
    uint32 second_offset = r_u8(0x800941DDu);
    sint32 first_index = (sint32)r_u16(owner + first_offset) - 1;
    sint32 second_index = (sint32)r_u16(owner + second_offset) - 1;
    uint32 selected = 0u, value = 0u, installed = 0u;
    if (first_index >= 0) {
        selected = r_u8(owner + 75u) + 1u;
        value = r_u8(first + 20u * (uint32)first_index + 1u);
        installed = 1u;
    }
    w_u16(0x800929ECu, selected);
    w_u16(0x800929E8u, installed);
    w_u16(0x800929EAu, value);
    selected = 0u; value = 0u; installed = 0u;
    if (second_index >= 0) {
        owner = r_u32(0x800BEA64u);
        value = r_u8(second + 20u * (uint32)second_index + 1u);
        selected = r_u8(owner + 88u) + 1u;
        installed = 1u;
    }
    w_u16(0x800929FCu, selected);
    w_u16(0x800929F8u, installed);
    w_u16(0x800929FAu, value);
    return selected;
}

/* GTMAIN.EXE 8002F6EC */
uint32 gt_draft_8002F6EC(uint32 context, uint32 mask)
{
    uint32 color = r_u32(0x800B6338u);
    uint32 packets = r_u32(0x800BFE4Cu);
    uint32 camera = r_u32(0x800B6638u);
    uint32 index, result, i;
    sint32 value;
    w_u32(context + 920u, 0x801A900Cu);
    w_u32(context + 924u, color);
    w_u32(context + 928u, packets);
    index = r_u16(camera + 134u) & 0xFFFu;
    value = (sint16)r_u16(0x8009423Cu + 2u * index);
    w_u16(context + 866u, value / 2);
    index = r_u16(camera + 134u) & 0xFFFu;
    value = (sint16)r_u16(0x80094A3Cu + 2u * index);
    w_u16(context + 868u, value / 2);
    for (i = 0; i < 32u; ++i, mask >>= 1)
    {
        if (mask & 1u) gt_draft_8002F28C(context, r_u32(r_u32(0x800BC1B0u) + 272u + 4u * i));
    }
    gt_draft_8002F28C(context, r_u32(r_u32(0x800BC1B0u) + 400u));
    result = r_u32(context + 928u);
    w_u32(0x800BFE4Cu, result);
    return result;
}

uint32 gt_draft_8002B190(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 result = (uint32)(sint32)(sint16)r_u16(0x800B63D6u);
    if (!result) return result;
    switch (r_u8(0x800CB7F5u)) {
    case 0: return gt_draft_80025EAC(0, 0, 0, 0, 0, 0, 0, 0);
    case 1: return gt_draft_unresolved_call(0x80026364u, 0, 0, 0, 0, 0, 0, 0, 0);
    case 2: return gt_draft_unresolved_call(0x800264F8u, 0, 0, 0, 0, 0, 0, 0, 0);
    case 3: return gt_draft_unresolved_call(0x80023094u, 0, 0, 0, 0, 0, 0, 0, 0);
    case 4:
        result = 3;
        if ((sint16)r_u16(0x800B63D6u) == 3) {
            w_u16(0x800B63DAu, r_u16(0x800B63DAu) - 1u);
            result = (uint32)(sint32)(sint16)r_u16(0x800B63DAu);
            if (result == 0xFFFFFFFFu) {
                gt_draft_unresolved_call(0x8001E324u, 0x800B6850u, 0x800B6410u, 3, 0, 0, 0, 0, 0);
                result = 4;
                w_u16(0x800B63D6u, 4); w_u16(0x800B63DAu, 0); w_u32(0x800B63C8u, 0);
            }
        }
        break;
    case 5: return gt_draft_unresolved_call(0x800234FCu, 0, 0, 0, 0, 0, 0, 0, 0);
    }
    return result;
}

uint32 gt_draft_800300A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_race_label_font2_draw(a0, (sint32)a1, a2, a3);
}

uint32 gt_draft_8002C264(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 format = r_u8(a4 + 72);
    char text[8]; text[0] = format ? (char)(format + 48u) : 82; text[1] = 0;
    uint32 address = gt_draft_buffer_address(text, sizeof(text));
    uint32 color = r_u8(a4 + 1731) == 1 ? 16512u : 8256u;
    gt_draft_unresolved_call(0x8006C790u, 2, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006C874u, color, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006C720u, a0, a1, a2, address, 0, 0, 0, 0);
    gt_draft_buffer_release(address);
    return gt_draft_unresolved_call(0x8006F4F0u, a0, a3, 0, 0, 0, 0, 0, 0);
}
