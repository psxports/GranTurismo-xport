#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include <string.h>

/* TODO Bind the remaining original dependency owners */
static uint32 race_dependency(uint32 owner, uint32 a0, uint32 a1, uint32 a2)
{
    return gt_draft_unresolved_call(owner, a0, a1, a2, 0, 0, 0, 0, 0);
}

static void race_input_map(uint32 state, uint32 packet, uint32 buttons,
    uint32 threshold, uint32 axis_first, uint32 axis_second, uint32 analog)
{

    gtmain_race_pad_map(state, packet, buttons, threshold,
        axis_first, axis_second, analog);
}

/* GTMAIN.EXE 80028F00 */
void gtmain_race_input_update(uint32 state, uint32 configuration)
{
    sint32 port = r_s16(state);
    uint32 packet, connected, previous, kind, held;
    if (port == -1) return;
    packet = gtmain_input_packet((uint32)port);
    if (r_u8(packet)) {
        if (r_u8(state + 2u)) gtmain_race_pad_reset(state);
        return;
    }
    connected = r_u8(state + 2u);
    previous = r_u16(state + 80u);
    w_u8(state + 2u, 1);
    w_u16(state + 80u, 0);
    w_u8(state + 82u, 0);
    gtmain_input_decode(state + 4u, packet);
    kind = r_u8(packet + 1u) & 0xF0u;
    w_u8(state + 3u, (uint8)kind);
    switch (kind) {
    case 0x40:
        race_input_map(state, packet, configuration + 1u, 0x8008F884u, 0x8008F888u, 0x8008F888u, 0);
        break;
    case 0x20:
        race_input_map(state, packet, configuration + 10u, configuration + 19u, configuration + 22u, configuration + 24u, 0);
        w_u8(state + 82u, (uint8)(r_u8(state + 82u) | 8u));
        break;
    case 0x50: case 0x70:
        race_input_map(state, packet, configuration + 26u, 0x8008F884u, configuration + 35u, configuration + 37u, 1);
        break;
    default: gtmain_race_pad_reset(state); break;
    }
    held = r_u16(state + 80u);
    w_u16(state + 78u, (uint16)(connected ? held & (held ^ previous) : 0u));
}

/* GTMAIN.EXE 80029928 */
sint32 gtmain_race_menu_input(void)
{
    uint32 pressed, held;
    sint32 selected, count;
    race_dependency(0x80029898u, 0, 0, 0);
    race_dependency(0x80029830u, 0, 0, 0);
    pressed = r_u16(0x800BC1C8u);
    selected = r_s8(0x8008F88Du);
    held = r_u16(0x800BC1C6u);
    count = r_s8(0x8008F88Eu);
    if (pressed & 0x80u) {
        gtmain_race_sound_effect_default(0);
        w_u8(0x8008F88Cu, 7);
        --selected;
        if (selected == -1) selected = count - 1;
    }
    if (pressed & 0x20u) {
        gtmain_race_sound_effect_default(0);
        w_u8(0x8008F88Cu, 7);
        if (++selected >= count) selected = 0;
    }
    {
        sint32 animation = r_s8(0x8008F892u);
        w_u8(0x8008F88Du, (uint8)selected);
        if (!animation) {
            if (held & 0x10u) {
                if (r_s8(0x8008F88Fu)) w_u8(0x8008F892u, (uint8)-3);
            } else if (held & 0x40u) {
                sint32 first = r_s8(0x8008F88Fu);
                sint32 limit = r_s8(0x8008F890u);
                if (first < limit) w_u8(0x8008F892u, 3);
            }
        }
    }
    if (pressed & 0x6000u) {
        gtmain_race_sound_effect_default(4);
        return 1;
    }
    return 0;
}

/* GTMAIN.EXE 8006CA50 */
uint32 gtmain_time_string(uint32 destination, uint32 milliseconds)
{
    uint32 seconds, minutes, hours, arguments[4];
    if (!milliseconds || milliseconds == 0xFFFFFFFFu)
        return gtmain_string_copy(destination, 0x8008E12Cu);
    if (milliseconds > 35999999u) milliseconds = 35999999u;
    seconds = milliseconds / 1000u;
    minutes = seconds / 60u;
    hours = minutes / 60u;
    milliseconds -= seconds * 1000u;
    seconds -= minutes * 60u;
    minutes -= hours * 60u;
    if (!hours) {
        arguments[0] = minutes; arguments[1] = seconds; arguments[2] = milliseconds;
        gtmain_string_format(destination, 0x8008E138u, arguments, 3);
    } else {
        arguments[0] = hours; arguments[1] = minutes;
        arguments[2] = seconds; arguments[3] = milliseconds / 100u;
        gtmain_string_format(destination, 0x8008E148u, arguments, 4);
    }
    return destination;
}

/* GTMAIN.EXE 8002D8BC */
void gtmain_race_time_set(uint32 time)
{
    if (!r_u32(0x800B630Cu)) {
        w_u32(0x800BDD04u, time);
        gtmain_time_string(0x801A8FDCu, time);
    }
}

/* GTMAIN.EXE 8002D9FC */
void gtmain_race_lap_time_store(uint32 time)
{
    uint32 mode = r_u32(0x800B630Cu);
    sint32 index = r_s8(0x800B399Cu);
    if (!mode && index < 100) w_u32(0x800BDB70u + ((uint32)index << 2), time);
}

static void race_car_callback(uint32 owner, uint32 car)
{
    switch (owner) {
    case 0: return;
    case 0x800315A8u: gtmain_race_sound_car_primary(car); return;
    case 0x800315C8u: gtmain_race_sound_car_secondary(car); return;
    case 0x800315E8u: gtmain_race_sound_car_init(car); return;
    case 0x8003169Cu: gtmain_race_sound_car_stop(car); return;
    default:
        /* TODO Bind remaining original callback identities without casting guest code */
        race_dependency(owner, car, 0, 0);
        return;
    }
}

/* GTMAIN.EXE 800313F8 */
void gtmain_race_sound_callbacks_apply(uint32 first_owner, uint32 second_owner, uint32 third_owner)
{
    sint32 index = 0;
    uint32 car = 0x800B6850u;
    while (index < r_s16(0x800BC1A6u)) {
        sint32 kind = r_s16(car + 26u);
        race_car_callback(kind == 1 ? first_owner : kind == 2 ? second_owner : third_owner, car);
        ++index;
        car += 3380u;
    }
}

/* GTMAIN.EXE 8002E574 */
uint32 gtmain_race_effect_alloc(void)
{
    uint32 group, bit;
    if (r_s32(0x800B6318u) < 2) return 0;
    for (group = 0; group < 4u; ++group) {
        uint32 mask = r_u32(0x800B39C4u + 4u * group);
        for (bit = 0; bit < 32u; ++bit) {
            if (!((mask >> bit) & 1u)) {
                w_u32(0x800B39C4u + 4u * group, mask | (1u << bit));
                return 0x800B39D4u + 32u * (32u * group + bit);
            }
        }
    }
    return 0;
}

/* GTMAIN.EXE 8002E620 */
void gtmain_race_effect_init(uint32 effect)
{
    memset(psx_addr(effect, 32u), 0, 32u);
    w_u32(effect + 8u, 0x0F8F0F80u);
    w_u16(effect + 24u, 16); w_u16(effect + 26u, 17); w_u16(effect + 30u, 4095);
    w_u32(effect, 0x7FC00080u); w_u32(effect + 4u, 0x0026008Fu); w_u16(effect + 28u, 137);
}

/* GTMAIN.EXE 80029F40 */
void gtmain_race_license_result_tick(void)
{
    uint32 result, thresholds, record, first, second, third, time, category = 0;
    if (r_u8(0x800CB7F5u) != 4u || r_s8(0x8008F893u)) return;
    result = race_dependency(0x80057180u, 0x800B6878u, 0, 0);
    if (!result) return;
    thresholds = gt_draft_8002A3CC(0, 0, 0, 0, 0, 0, 0, 0);
    record = gtmain_race_mode_record();
    second = r_u32(thresholds + 40u);
    third = r_u32(thresholds + 12u);
    first = r_u32(record + 4u);
    if (!second) second = first;
    if (!third) third = second;
    time = r_u32(0x800BF52Cu);
    if (first < time) result = 0xFFFFFFFFu;
    else if (second < time) category = 1;
    else category = third < time ? 2u : 3u;
    {
        sint32 lap = r_s16(0x800B68B0u);
        w_u8(0x8008F893u, (uint8)result);
        w_u8(0x8008F894u, (uint8)category);
        if (lap < 2) {
            if (!category) time = 0;
            w_u16(0x800B68B0u, 2);
            w_u32(0x800B6870u, time);
            gtmain_race_time_set(time);
            gtmain_race_lap_time_append(time);
        }
    }
    {
        uint16 countdown = r_u16(0x800BF518u);
        w_u16(0x800B63D6u, 3);
        w_u16(0x800B63DAu, countdown);
    }
}

/* GTMAIN.EXE 80030C24 */
void gtmain_race_assist_tick(void)
{
    sint32 result = (sint32)gt_draft_80030B20(0, 0, 0, 0, 0, 0, 0, 0);
    if (result >= 0 && result < 2) {
        w_u8(0x800B63C2u, 0);
        gtmain_race_sound_effect_default(4);
    } else if (!r_s16(0x800B63DEu)) w_u8(0x800B63C2u, 0);
    race_dependency(0x800204ECu, r_s16(0x800912E0u) != 0, 0, 0);
}

/* GTMAIN.EXE 80030B0C */
void gtmain_race_assist_reset(void)
{
    w_u16(0x800912E2u, 0);
    w_u16(0x800912E0u, 0);
}

/* GTMAIN.EXE 8002B190 */
void gtmain_race_mode_tick(void)
{
    if (!r_s16(0x800B63D6u)) return;
    switch (r_u8(0x800CB7F5u)) {
    case 0: race_dependency(0x80025EACu, 0, 0, 0); break;
    case 1: race_dependency(0x80026364u, 0, 0, 0); break;
    case 2: race_dependency(0x800264F8u, 0, 0, 0); break;
    case 3: race_dependency(0x80023094u, 0, 0, 0); break;
    case 4:
        if (r_s16(0x800B63D6u) == 3) {
            uint16 countdown = (uint16)(r_u16(0x800B63DAu) - 1u);
            w_u16(0x800B63DAu, countdown);
            if ((sint16)countdown == -1) {
                gtmain_driver_mode_set(0x800B6850u, 0x800B6410u, 3);
                w_u16(0x800B63D6u, 4);
                w_u16(0x800B63DAu, 0);
                w_u8(0x800B63C8u, 0);
            }
        }
        break;
    case 5: race_dependency(0x800234FCu, 0, 0, 0); break;
    default: break;
    }
}

/* GTMAIN.EXE 8002B0DC */
void gtmain_race_countdown_tick(void)
{
    sint32 timer = r_s16(0x800B6312u), divisor, numerator, quotient, previous;
    if (!timer) return;
    numerator = (sint16)(timer - 1);
    divisor = r_s32(0x800BF518u);
    if (!divisor) gt_draft_failure("GTMAIN race countdown division by zero", 0x8002B114u);
    quotient = numerator / divisor;
    previous = r_s16(0x800B63DEu);
    w_u16(0x800B6312u, (uint16)(timer - 1));
    w_u16(0x800B63DEu, (uint16)quotient);
    if (previous != quotient && quotient < 4) {
        sint32 sound = 8;
        if (!quotient) {
            gtmain_race_stream_start(r_s8(0x800B63BFu), 1);
            sound = 10;
        }
        gtmain_race_sound_effect_default(sound);
    }
}

/* GTMAIN.EXE 8002ADA4 */
void gtmain_race_rumble_clear(void)
{
    gtmain_input_rumble(0, 0, 0);
    gtmain_input_rumble(4, 0, 0);
}
