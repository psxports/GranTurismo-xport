#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include <stdio.h>
/* Unverified draft translations */
/* TODO Resolve unspecified call arguments during integration */

uint32 gt_draft_80029B98(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gt_draft_unresolved_call(0x80029B7Cu, 0, 0, 0, 0, 0, 0, 0, 0);
    w_u8(0x8008F890u, 0);
    if ((sint16)r_u16(0x800B63D6u) == 1) {
        w_u16(0x800B63D6u, 2);
        gt_draft_unresolved_call(0x800204ECu, r_u8(0x8008F88Du) != 0, 0, 0, 0, 0, 0, 0, 0);
        return gtmain_race_mode_vehicle_prepare();
    }
    if ((sint16)r_u16(0x800B63D6u) == 2) {
        w_u16(0x800B63D6u, 1);
        if (r_u8(0x8008F88Du)) return gtmain_race_mode_vehicle_prepare();
    }
    return 1;
}

/* GTMAIN.EXE 8002AB48 */
void gt_draft_8002AB48(void)
{
    if (!r_u8(0x800CB767u) || !r_s8(0x800B63D9u)) return;
    if (!r_s16(0x800994B4u) && r_u8(0x800994C0u)) {
        w_u8(0x800B63D9u, 0u);
        return;
    }
    /* TODO Transitive stream pause-state dependencies remain partial */
    gtmain_stream_pause_if_loaded();
}

uint32 gt_draft_8002ABC0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 result = r_u8(0x800CB767u);
    w_u8(0x800B63D9u, 0);
    if (result && (sint32)a0 >= 0) {
        uint32 request = gt_draft_unresolved_call(0x80070024u, a0, 0, 0, 0, 0, 0, 0, 0);
        gt_draft_unresolved_call(0x80070054u, request, a1, 0, 0, 0, 0, 0, 0);
        gt_draft_unresolved_call(0x80070118u, 0, 0, 0, 0, 0, 0, 0, 0);
        w_u8(0x800B63D9u, 1);
        w_u8(0x800B63C1u, a1 ? a0 : 255u);
        w_u8(0x800B6314u, a1);
        return 0xFFFFFFFFu;
    }
    return result;
}

uint32 gt_draft_8002AC8C(void)
{
    gtmain_race_sound_banks_update();
    uint32 mode = r_u8(0x800CB7F5u);
    uint32 sound = mode < 4 ? 10u : (mode == 5 ? 11u : 255u);
    uint32 table = r_u32(0x800B630Cu) == 1 ? 0x800CB8D0u : 0x800CB8C0u;
    w_u8(0x800B63C1u, sound);
    w_u8(0x800B6314u, 0);
    w_u8(0x800B63BFu, gtmain_stream_playlist_next(table));
    w_u8(0x800B63D9u, 0);
    return gtmain_spu_master_volume(6553, 6553);
}

uint32 gt_draft_8002ADDC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 car = r_u8(0x800B63BCu);
    uint32 index = r_u8(gt_draft_unresolved_call(0x8006E3D4u, car, 0, 0, 0, 0, 0, 0, 0)) & 127u;
    gt_draft_unresolved_call(0x80070190u, 128, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8001C688u, index, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x80024D48u, car, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x800221E8u, 0, 0, 0, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x80022468u, car, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_80043320(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 owner = r_u32(0x800BEA64u);
    uint32 index = r_u16(owner + r_u8(0x800941DBu));
    uint32 item = gt_draft_unresolved_call(0x8001EB94u, 30, 0, 0, 0, 0, 0, 0, 0) + 32u * (index - 1u);
    w_u16(0x80092ABCu, r_u8(r_u32(0x800BEA64u) + 65));
    w_u16(0x80092AB8u, r_u8(item + 10));
    w_u16(0x80092ABAu, r_u8(item + 11));
    w_u16(0x80092ACCu, r_u8(r_u32(0x800BEA64u) + 78));
    w_u16(0x80092AC8u, r_u8(item + 12));
    uint32 result = r_u8(item + 13);
    w_u16(0x80092ACAu, result);
    return result;
}

/* GTMAIN.EXE 8002A7EC */
uint32 gt_draft_8002A7EC(void)
{
    VSync(0);
    gtmain_display_mask(0u);
    gtmain_frame_draw_apply(0u);
    ClearImage((PSX_RECT *)psx_addr(0x8008FB3Cu, sizeof(PSX_RECT)), 0, 0, 0);
    DrawSync(0);
    ClearImage((PSX_RECT *)psx_addr(0x8008FB44u, sizeof(PSX_RECT)), 0, 0, 0);
    return (uint32)DrawSync(0);
}

uint32 gt_draft_8004A8E8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_replay_header_check(a0);
}

uint32 gt_draft_8004A804(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 base = a0 + 512u;
    sint32 item = r_u16(base + 52u * a1 + 40u);
    for (sint32 i = 0; i < (sint32)a2; ++i) {
        item = (sint16)r_u16(base + 2u * (uint32)item + 3336u);
        if (item == -1) break;
    }
    return (uint32)item;
}

sint32 gtmain_menu_entry_type_input(uint32 input)
{
    sint32 candidate, choice;
    w_u8(0x800B53B5u, 0);
    w_u8(0x800B53B6u, 0);
    w_u8(0x800B53B8u, 1);
    w_u8(0x800B53B7u, 0);
    candidate = gtmain_menu_selector_input(0x80092EBCu, input);
    if (candidate == -2) return -2;
    if (candidate == -1) return -1;
    choice = 64;
    if (r_u8(r_u32(0x800B53ACu) + 8u * (uint32)candidate + 1u) < 2u)
        choice = candidate;
    printf((const char *)psx_addr(0x8008DCC0u, 11u), (int)choice);
    return choice;
}

uint32 gtmain_menu_device_selection_input(void)
{
    uint32 device = r_u32(0x800BDD08u);
    sint32 result;
    gtmain_menu_stream_check();
    if (!(sint8)r_u8(device)) return 0;
    gtmain_menu_flags(1);
    /* TODO Bind unselected device selection input */
    result = (sint32)gt_draft_unresolved_call(0x8004D820u,
        0x80092B08u, device + 2u, 0, 0, 0, 0, 0, 0);
    if (result == -2)
    {
        gtmain_menu_sound(2);
        return 1;
    }
    if (result == -3)
    {
        sint32 mode, selection;
        gtmain_menu_sound(4);
        mode = (sint8)r_u8(0x8009278Du);
        selection = (sint8)r_u8(0x800B5375u);
        /* TODO Bind unselected device selection transition */
        gt_draft_unresolved_call(0x800438ECu, (uint32)mode,
            (uint32)selection, 0, 0, 0, 0, 0, 0);
        return 1;
    }
    return 0;
}

uint32 gt_draft_80048AF8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_menu_text_fit(a0, (sint32)a1);
}

uint32 gt_draft_80042E84(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 table = gt_draft_unresolved_call(0x8001EB94u, 33, 0, 0, 0, 0, 0, 0, 0);
    uint32 owner = r_u32(0x800BEA64u);
    uint32 index = r_u16(owner + r_u8(0x800941DEu));
    w_u16(0x800929CCu, r_u8(r_u32(0x800BEA64u) + 67));
    w_u16(0x800929DCu, r_u8(r_u32(0x800BEA64u) + 80));
    uint32 row = table + 72u * (index - 1u);
    w_u16(0x800929C8u, r_u8(row + 24));
    w_u16(0x800929CAu, r_u8(row + 25));
    w_u16(0x800929D8u, r_u8(row + 39));
    uint32 result = r_u8(row + 40);
    w_u16(0x800929DAu, result);
    return result;
}

uint32 gt_draft_80041980(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_menu_range_index((sint32)a0, (sint32)a1, (sint32)a2, (sint32)a3);
}

uint32 gtmain_menu_settings_masks_update(void)
{
    uint32 result;
    result = gtmain_menu_settings_mask(0x80093590u, 6);
    w_u8(0x800BEA6Cu, (uint8)result);
    result = gtmain_menu_settings_mask(0x80093598u, 3);
    w_u8(0x800BEA6Du, (uint8)result);
    result = gtmain_menu_settings_mask(0x8009359Cu, 4);
    w_u8(0x800BEA6Eu, (uint8)result);
    result = gtmain_menu_settings_mask(0x800935A0u, 2);
    w_u8(0x800BEA6Fu, (uint8)result);
    return result;
}

uint32 gt_draft_8001C170(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 packet = a1;
    for (sint32 i = 0; i < (sint32)a2; ++i) {
        uint32 row = a0 + 4u * r_u16(packet + 22);
        uint32 texture = gt_draft_unresolved_call(0x80030DFCu, r_u32(row), 0, 0, 0, 0, 0, 0, 0);
        uint32 palette = gt_draft_80030E1C(r_u32(row), 0, 0, 0, 0, 0, 0, 0);
        uint32 du = gt_draft_80030E3C(r_u32(row), 0, 0, 0, 0, 0, 0, 0);
        uint32 dv = gt_draft_80030E60(r_u32(row), 0, 0, 0, 0, 0, 0, 0);
        uint32 word = r_u32(packet + 28) & 0xFFE0FFFFu;
        w_u16(packet + 22, r_u16(texture));
        word |= (r_u16(palette) & 31u) << 16;
        w_u32(packet + 28, word);
        uint32 color = r_u16(palette);
        w_u8(packet + 36, r_u8(packet + 36) + du);
        w_u8(packet + 20, r_u8(packet + 20) + du);
        w_u32(packet + 28, (word & 0xFE7FFFFFu) | (((color >> 7) & 3u) << 23));
        w_u8(packet + 21, r_u8(packet + 21) + dv);
        w_u8(packet + 28, r_u8(packet + 28) + du);
        w_u8(packet + 37, r_u8(packet + 37) + dv);
        w_u8(packet + 29, r_u8(packet + 29) + dv);
        if (a3 == 4) {
            w_u8(packet + 44, r_u8(packet + 44) + du);
            w_u8(packet + 45, r_u8(packet + 45) + dv);
        }
        packet += a3 == 3 ? 40u : 48u;
    }
    return packet;
}
