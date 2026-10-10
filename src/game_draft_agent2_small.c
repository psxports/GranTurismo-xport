#include "game_first2_registry.h"
#include "game_draft_registry.h"
/* Unverified draft translations */
/* TODO Resolve unused call argument carriers during integration */

uint32 gtmain_camera_config_pose_prepare(uint32 camera)
{
    uint32 config = r_u32(0x800B6400u);
    uint32 mode = r_u16(config + 16u);
    sint32 distance = r_u16(config + 18u);
    return gtmain_camera_pose_prepare(camera, mode, distance);
}

uint32 gt_draft_80019EE8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gtmain_sound_voice_tick_if_enabled();
    return gtmain_sound_sequence_tick();
}

uint32 gt_draft_8001A098(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gtmain_file_load(r_u32(0x800B6390u), 0x800D6FDCu, 0u, 0u);
    gtmain_shutdown();
    return gtos_native_transition(0x800D6FDCu);
}

uint32 gt_draft_8001A008(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gt_draft_unresolved_call(0x80031710u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8006A150u, 0x800C4C00u, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8007A694u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x800825DCu, 0, 0, 0, 0, 0, 0, 0, 0);
    gtmain_input_stop_when_ready();
    gt_draft_unresolved_call(0x8007ED98u, 0, 0, 0, 0, 0, 0, 0, 0);
    gt_draft_unresolved_call(0x8007A7B8u, 0, 0, 0, 0, 0, 0, 0, 0);
    return gt_draft_unresolved_call(0x8008A33Cu, 0, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_8004A260(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 index = (sint32)a1;
    uint32 item = a0 + 512u + a1 * 2u;
    while (index < 1024) {
        if ((sint16)r_u16(item + 3336u) == 1024) return (uint32)index;
        index = (sint32)((uint32)index + 1u);
        item += 2u;
    }
    return 0xFFFFFFFFu;
}

uint32 gt_draft_8004A990(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return 0;
}

uint32 gt_draft_8004A998(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    w_u8(0x800BEA70u, 1);
    w_u16(0x800B53BCu, 24);
    return 0;
}

/* GTMAIN.EXE:8004635C */
uint32 gtmain_menu_table_sprite_draw(uint32 ordering, uint32 x, uint32 y,
    uint32 index, uint32 color)
{
    uint32 table = 0x80092594u + (index << 4u);
    uint32 uv_clut = r_u32(table + 12u);
    uint32 wh = r_u32(table + 8u);
    return gtmain_packet_sprite(ordering, (x & 0xFFFFu) | (y << 16u), uv_clut, wh, color);
}

uint32 gt_draft_80029DE4(void)
{
    if (r_u8(0x800CB7F5u) == 4 && r_u32(0x800B630Cu) != 1) {
        w_u16(0x800B63D6u, 1);
        w_u8(0x8008F88Cu, 7);
        w_u8(0x8008F88Du, 0);
        w_u8(0x8008F88Eu, 2);
        return 2;
    }
    return 1;
}

uint32 gt_draft_8002AD7C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (uint32)gtmain_race_audio_stop_wait();
}

uint32 gt_draft_80029E3C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    if (r_u8(0x800CB7F5u) != 4) return 4;
    if (r_u32(0x800B630Cu) == 1) return 1;
    return gt_draft_unresolved_call(0x8001E324u, 0x800B6850u, 0x800B6410u, 3, 0, 0, 0, 0, 0);
}

uint32 gt_draft_8002FC74(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 mode = r_u8(0x800CB7F5u);
    if (mode == 1 || (mode == 3 && r_u8(0x800CB7F6u) == 2)) {
        /* TODO Original early return carries incoming v0 when mode is one */
        return mode == 3 ? 2 : 0;
    }
    return gt_draft_unresolved_call(0x8002FBA8u, a0, 64, 16, 0x800912ACu, a1, 0, 0, 0);
}

uint32 gt_draft_8002AC58(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gt_draft_8002ABC0(r_u16(0x800B6874u) ? 13u : 12u, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_800433B0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 result;
    w_u8(r_u32(0x800BEA64u) + 69, r_u16(0x8009296Cu));
    result = r_u16(0x8009297Cu);
    w_u8(r_u32(0x800BEA64u) + 82, result);
    return result;
}

uint32 gt_draft_80029C28(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 mode = (sint16)r_u16(0x800B63D6u);
    if (mode == 1) { w_u8(0x800B63C0u, 6); return 6; }
    if (mode == 2) w_u16(0x800B63D6u, 1);
    return 1;
}

uint32 gtmain_menu_transition_enter(uint32 a0)
{
    w_u8(a0 + 12, 0);
    w_u8(a0 + 13, 12);
    return 12;
}

uint32 gt_draft_8002A390(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return r_u8(0x8008FB38u + r_u8(0x800CB7F8u));
}

uint32 gt_draft_8002A3CC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return 0x800C77ACu + 1792u * r_u8(0x800CB7F8u) + 224u * r_u8(0x800CB7F9u);
}

uint32 gt_draft_8002A500(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_course_route_caption_update();
}

uint32 gt_draft_80047160(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return (sint32)a0 < 120;
}

uint32 gt_draft_80030E1C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return a0 * 24u + r_u32(0x800B49D4u) + 18u;
}

uint32 gt_draft_80030938(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_race_choice_highlight(a0);
}

uint32 gt_draft_8002F8B4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return a0 + r_u32(a0) * 16u + 4u;
}

uint32 gt_draft_8001ECA8(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gtmain_player_configuration_build();
}

uint32 gt_draft_8003DBF4(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    w_u8(0x801F2041u, 1);
    return 1;
}

uint32 gt_draft_8001DD9C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    if ((sint16)r_u16(0x800B63DEu) < 2) w_u8(a0 + 253, 0);
    return gt_draft_8001D654(a0, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_8001EE00(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    return gt_draft_unresolved_call(0x800204ECu, a0 != 0, 0, 0, 0, 0, 0, 0, 0);
}

uint32 gt_draft_80022BFC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    uint32 object = gt_draft_unresolved_call(0x8002278Cu, 0, 0, 0, 0, 0, 0, 0, 0);
    uint32 result = r_u32(object + (a0 ? 56u : 28u) + 12u);
    uint32 other = r_u32(object + 12u);
    w_u32(0x800BDB4Cu, result);
    w_u32(0x800BDB48u, other);
    return result;
}
