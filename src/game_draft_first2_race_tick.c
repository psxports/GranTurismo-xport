#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* TODO Bind missing original race owners */
static uint32 race_tick_call(uint32 owner, uint32 a0, uint32 a1)
{
    return gt_draft_unresolved_call(owner, a0, a1, 0, 0, 0, 0, 0, 0);
}

/* TODO Resolve independent input decode and mapping adapter limitations */
/* GTMAIN.EXE 800290C8 */
void gtmain_race_input_poll(void)
{
    gtmain_input_poll();
    gtmain_race_input_update(0x800BC1C0u, 0x800CB782u);
    gtmain_race_input_update(0x800BC280u, 0x800CB7ABu);
    gtmain_race_input_update(0x800BC220u, 0x800CB782u);
}

static void race_demo_command(uint32 owner)
{
    /* TODO Bind unselected noarg demo commands 80029B98 and 80029C28 */
    (void)race_tick_call(owner, 0u, 0u);
}

/* TODO Resolve independent menu-input dependencies */
/* GTMAIN.EXE 80029E90 */
void gtmain_race_demo_input(void)
{
    uint32 action;
    w_u8(0x800B63D8u, 1);
    gtmain_race_camera_tick(0x800B6410u);
    if (r_s16(0x800B63D6u) == 4) {
        if (r_u16(0x800BC1C8u) & 0xFF0Fu) w_u8(0x800B63C0u, 3);
        return;
    }
    action = (uint32)gtmain_race_menu_input();
    if (action == 1u) race_demo_command(0x80029B98u);
    else if (action == 2u) race_demo_command(0x80029C28u);
}

/* GTMAIN.EXE 8002CF84 */
void gtmain_race_pause_input(void)
{
    uint32 pad_second, mode, pad_first, pressed, held_first, held_second, held;
    sint32 selected, count, sound = -1;
    uint32 item, items;
    gt_draft_8002CD40();
    pad_second = r_u16(0x800BC288u);
    mode = r_u8(0x800CB7F5u);
    pad_first = r_u16(0x800BC1C8u);
    pressed = pad_first | (mode == 2u ? pad_second : 0u);
    if (pressed & 1u) w_u8(0x800CB764u, (uint8)(r_u8(0x800CB764u) == 0u));
    if (pressed & 8u) {
        if (r_u32(0x800B630Cu) == 1u) w_u8(0x800B63C0u, 4);
        w_u8(0x800B63D8u, (uint8)(r_s8(0x800B63D8u) == 0));
        gtmain_race_sound_effect_default(2);
        return;
    }
    if (!r_s8(0x800B63D8u)) return;
    pad_first = r_u16(0x800BC1C8u);
    held_first = r_u16(0x800BC1CCu);
    pad_second = r_u16(0x800BC288u);
    held_second = r_u16(0x800BC28Cu);
    mode = r_u8(0x800CB7F5u);
    held = pad_first | held_first;
    if (mode == 2u) held |= pad_second | held_second;
    selected = r_s8(0x8008FBF1u);
    count = r_s8(0x8008FBF2u);
    if ((held & 0x50u) == 0x10u) {
        selected = (sint32)((uint32)selected - 1u);
        sound = 0;
        if (selected == -1) selected = count - 1;
    } else if ((held & 0x50u) == 0x40u) {
        selected = (sint32)((uint32)selected + 1u);
        sound = 0;
        if (selected >= count) selected = 0;
    }
    items = r_u32(0x8008FBF4u);
    w_u8(0x8008FBF1u, (uint8)selected);
    item = r_u8(items + (uint32)selected);
    if (sound >= 0) {
        gtmain_race_sound_effect_default(sound);
        return;
    }
    if (pressed & 0x6000u) {
        sound = 4;
        switch (item) {
        case 1: w_u8(0x800B63D8u, 0); sound = 2; break;
        case 2: w_u8(0x800B63C0u, 5); break;
        case 3: w_u8(0x800B63C0u, 4); break;
        case 4: case 5: w_u8(0x800B63C0u, 1); break;
        case 6: w_u8(0x800B63C0u, 7); break;
        case 7: w_u8(0x800CB764u, (uint8)(r_u8(0x800CB764u) == 0u)); break;
        default: break;
        }
    }
    if (sound >= 0) gtmain_race_sound_effect_default(sound);
}

/* GTMAIN.EXE 8002E4C0 */
void gtmain_race_effects_tick(void)
{
    uint32 group, bit, effect = 0x800B39D4u;
    for (group = 0; group < 4u; ++group) {
        uint32 mask = r_u32(0x800B39C4u + 4u * group);
        for (bit = 0; bit < 32u; ++bit, effect += 32u) {
            if ((mask >> bit) & 1u) {
                uint32 phase = r_u32(effect + 16u);
                uint32 position = r_u16(effect + 24u);
                uint32 velocity = r_u16(effect + 26u);
                uint32 decay = r_u16(effect + 28u);
                uint32 lifetime;
                w_u32(effect + 16u, phase + 1092u);
                lifetime = r_u16(effect + 30u);
                w_u16(effect + 24u, (uint16)(position + velocity));
                lifetime -= decay;
                w_u16(effect + 30u, (uint16)lifetime);
                if (lifetime & 0x8000u) mask &= ~(1u << bit);
            }
        }
        w_u32(0x800B39C4u + 4u * group, mask);
    }
}

/* GTMAIN.EXE 80020F94 */
void gtmain_race_cars_tick(void)
{
    uint32 car = 0x800B6850u;
    sint32 index = 0;
    gtmain_race_physics_tick();
    if (r_s16(0x800B63D6u)) gtmain_race_rank_list_sort();
    while (index < r_s16(0x800BC1A6u)) {
        uint32 wheel;
        gtmain_car_rumble_update(car);
        for (wheel = 0; wheel < 4u; ++wheel) {
            uint32 strength = r_u8(car + 68u * wheel + 173u);
            if (strength >= 128u) {
                uint32 effect = gtmain_race_effect_alloc();
                if (effect) {
                    uint32 x, y, z;
                    gtmain_race_effect_init(effect);
                    x = r_u32(car + 24u * wheel + 2024u);
                    y = r_u32(car + 24u * wheel + 2028u);
                    z = r_u32(car + 24u * wheel + 2032u);
                    w_u32(effect + 12u, x); w_u32(effect + 16u, y); w_u32(effect + 20u, z);
                    w_u16(effect + 30u, (uint16)(strength << 4));
                }
            }
        }
        ++index;
        car += 3380u;
    }
    gtmain_race_license_result_tick();
    if (r_u16(0x800B6884u) && r_s8(0x800B6310u)) w_u8(0x800B63C0u, 5);
}

/* GTMAIN.EXE 8002E214 */
void gtmain_race_hud_tick(void)
{
    sint32 lap = r_s16(0x800B68B0u);
    sint32 limit = r_s16(0x800B634Cu);
    sint32 timer;
    if (lap) --lap;
    if (!limit) limit = 100;
    if (lap < limit) {
        uint32 ticks = r_u32(0x80093D78u);
        uint32 elapsed;
        gtmain_race_time_set(ticks / 3u);
        ticks = r_u32(0x80093D78u);
        elapsed = r_u32(0x800B68A4u);
        gtmain_race_lap_time_store(ticks / 3u - elapsed);
    }
    if (!r_s16(0x800B63D6u)) {
        sint32 current = r_s16(0x800B689Cu);
        sint32 best = r_s16(0x800BDB40u);
        if (best < current) w_u16(0x800BDB40u, (uint16)current);
    }
    timer = r_s16(0x8009071Au);
    if (timer) w_u16(0x8009071Au, (uint16)(timer - 1));
    timer = r_s16(0x8009071Cu);
    if (timer) w_u16(0x8009071Cu, (uint16)(timer - 1));
}

/* GTMAIN.EXE 80030D3C */
void gtmain_race_pause_blink(void)
{
    uint32 mode = r_u32(0x800B630Cu);
    if (!mode) {
        if (r_s8(0x800B63C2u)) gtmain_race_assist_tick();
    } else if (mode == 1u) {
        uint8 blink = (uint8)(r_u8(0x800912A8u) + 1u);
        w_u8(0x800912A8u, blink);
        if ((sint8)blink >= 7) w_u8(0x800912A8u, (uint8)-3);
    }
}

/* GTMAIN.EXE 80031668 */
void gtmain_race_sound_callbacks_active(void)
{
    sint32 index = 0;
    uint32 car = 0x800B6850u;
    while (index < (sint16)r_u16(0x800BC1A6u)) {
        sint32 kind = (sint16)r_u16(car + 26u);
        if (kind == 1) gtmain_race_sound_car_primary(car);
        else if (kind == 2) gtmain_race_sound_car_secondary(car);
        else gtmain_race_sound_car_init(car);
        ++index;
        car += 3380u;
    }
}

/* GTMAIN.EXE 80031710 */
void gtmain_race_sound_callbacks_stopped(void)
{
    sint32 index = 0;
    uint32 car = 0x800B6850u;
    while (index < (sint16)r_u16(0x800BC1A6u)) {
        /* All three original callback identities select the same stop body */
        (void)r_u16(car + 26u);
        gtmain_race_sound_car_stop(car);
        ++index;
        car += 3380u;
    }
}

/* GTMAIN.EXE 8002D874 */
void gtmain_race_time_reset(void)
{
    if (!r_u32(0x800B630Cu)) gtmain_race_time_set(0xFFFFFFFFu);
    else gtmain_time_string(0x801A8FDCu, r_u32(0x800BDD04u));
}

/* GTMAIN.EXE 8002D970 */
void gtmain_race_lap_history_reset(void)
{
    uint32 mode = r_u32(0x800B630Cu), index;
    w_u16(0x800BDB40u, 0);
    w_u8(0x800B399Cu, 0);
    if (!mode)
        for (index = 0; index < 100u; ++index) w_u32(0x800BDB70u + 4u * index, 0xFFFFFFFFu);
}

/* GTMAIN.EXE 8002DB48 */
void gtmain_race_split_reset(void)
{
    if (r_u32(0x800B630Cu) != 1u) w_u8(0x800BDD01u, 0);
    gtmain_time_string(0x801A8FECu, r_u32(0x800BDB4Cu));
}

/* GTMAIN.EXE 8002DC5C */
void gtmain_race_best_reset(void)
{
    if (r_u32(0x800B630Cu) != 1u) w_u8(0x800BDD00u, 0);
    gtmain_time_string(0x801A8FFCu, r_u32(0x800BDB48u));
}

/* GTMAIN.EXE 8002E1F4 */
void gtmain_race_timers_reset(void)
{
    w_u16(0x8009071Au, 0);
    w_u16(0x8009071Cu, 0);
}
