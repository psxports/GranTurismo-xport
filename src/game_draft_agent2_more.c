#include "game_first2_registry.h"
#include "game_draft_registry.h"
/* Unverified draft translations */
/* TODO Resolve unspecified outgoing argument carriers during integration */

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

uint32 gt_draft_8003CC54(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 device = r_u32(0x800BDD08u);
    gt_draft_unresolved_call(0x8003A304u, 0, 0, 0, 0, 0, 0, 0, 0);
    if (!r_u8(device)) return 0;
    uint32 status = r_u32(0x800B52FCu);
    w_u8(status + 8, r_u8(status + 8) + 1);
    status = r_u32(0x800B52FCu);
    w_u8(status + 8, r_u8(status + 8) & 1u);
    status = r_u32(0x800B52FCu);
    if (r_u8(status) != 60) { w_u8(status, r_u8(status) + 1); return 0; }
    if (!(r_u16(device + 6) & 0xFF0Fu)) return 0;
    gt_draft_unresolved_call(0x80035778u, 4, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x800352DCu, r_u32(r_u32(0x800B52FCu) + 4), 0, 0, 0, 0, 0, 0, 0);
    if (r_u8(r_u32(0x800B52FCu) + 1)) gt_draft_unresolved_call(0x8003A2DCu, 0, 0, 0, 0, 0, 0, 0, 0);
    return 3;
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

uint32 gt_draft_80042F1C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 first = gt_draft_unresolved_call(0x8001EB94u, 31, 0, 0, 0, 0, 0, 0, 0);
    uint32 second = gt_draft_unresolved_call(0x8001EB94u, 32, 0, 0, 0, 0, 0, 0, 0);
    sint32 i = (sint32)r_u16(r_u32(0x800BEA64u) + r_u8(0x800941DCu)) - 1;
    sint32 j = (sint32)r_u16(r_u32(0x800BEA64u) + r_u8(0x800941DDu)) - 1;
    uint32 selected = 0, value = 0, installed = 0;
    if (i >= 0) { selected = r_u8(r_u32(0x800BEA64u) + 75) + 1u; value = r_u8(first + 20u * (uint32)i + 1u); installed = 1; }
    w_u16(0x800929ECu, selected); w_u16(0x800929E8u, installed); w_u16(0x800929EAu, value);
    selected = 0; value = 0; installed = 0;
    if (j >= 0) { value = r_u8(second + 20u * (uint32)j + 1u); selected = r_u8(r_u32(0x800BEA64u) + 88) + 1u; installed = 1; }
    w_u16(0x800929FCu, selected); w_u16(0x800929F8u, installed); w_u16(0x800929FAu, value);
    return selected;
}

uint32 gt_draft_8002F6EC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    w_u32(a0 + 920, 0x801A8FFCu);
    w_u32(a0 + 924, r_u32(0x800B6338u));
    w_u32(a0 + 928, r_u32(0x800BFE4Cu));
    uint32 index = r_u16(r_u32(0x800B6638u) + 134) & 0xFFFu;
    w_u16(a0 + 866, (uint32)((sint16)r_u16(0x8009423Cu + 2u * index) / 2));
    w_u16(a0 + 868, (uint32)((sint16)r_u16(0x80094A3Cu + 2u * index) / 2));
    for (uint32 i = 0; i < 32; ++i) {
        if (a1 & 1u) gt_draft_8002F28C(a0, r_u32(r_u32(0x800BC1B0u) + 272u + 4u * i), 0, 0, 0, 0, 0, 0);
        a1 >>= 1;
    }
    gt_draft_8002F28C(a0, r_u32(r_u32(0x800BC1B0u) + 400u), 0, 0, 0, 0, 0, 0);
    uint32 result = r_u32(a0 + 928);
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
