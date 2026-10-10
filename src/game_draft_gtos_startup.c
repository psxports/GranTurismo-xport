#include "game_draft_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psx_spu.h"
#include "game_startup.h"

static uint32 gtos_module_enter(uint32 mode);

void gtos_input_reset(void)
{
    uint32 i;
    memset(psx_addr(0x800756E0u, 8u), 0, 8u);
    w_u16(0x800756E8u, 0u);
    for (i = 0u; i < 16u; ++i)
        w_u32(0x800756EAu + 4u * i, r_u32(0x8007370Cu));
}

static uint32 gtos_input_poll_at(uint32 state, uint32 port)
{
    uint32 native;
    /* Process host button events even while movie frames remain queued */
    xport_poll();
    native = PadRead(port);
    uint32 current = ((native & 255u) << 8u) | ((native >> 8u) & 255u);
    uint32 previous = r_u16(state + 2u), valid = r_u16(state), repeat = 0u, i;
    w_u16(state, 65535u);
    w_u16(state + 2u, current);
    w_u16(state + 4u, valid & (previous ^ current) & current);
    w_u16(state + 6u, valid & (previous ^ current) & previous);
    /* Preserve the original per-button delay and repeat counters */
    for (i = 0u; i < 16u; ++i) {
        uint32 row = state + 10u + 4u * i, bit = 1u << i;
        uint32 count = 0u, delay = r_u8(row);
        if (current & bit) {
            count = r_u16(row + 2u) + 1u;
            if (count == delay) repeat |= bit;
            else if (count == delay + r_u8(row + 1u)) { count = delay; repeat |= bit; }
        }
        w_u16(row + 2u, count);
    }
    w_u16(state + 8u, repeat);
    return r_u16(state + 4u);
}

uint32 gtos_input_poll(void)
{
    return gtos_input_poll_at(0x800756E0u, 0u);
}

static void gtos_title_image(uint32 frame, const PSX_RECT *rect)
{
    gtos_movie_unpack(frame, 0x801716ACu);
    gtos_mdec_image(0x801716ACu, (uint16)rect->w, (uint16)rect->h, rect->x, rect->y);
}

static void gtos_title_load(void)
{
    uint32 base = 0x800CCF7Cu, i, size = 0u, mode = r_u8(0x8007ABDDu);
    size_t bytes = 0u;
    PSX_RECT rect;
    if (!xport_cd_file_size(0x8004B390u, "DATA", &size) || size > 0x8015EF7Cu - base ||
        !xport_cd_file_read(0x8004B390u, "DATA", base, size, &bytes) || bytes != size)
        gt_draft_unresolved_call(0x80029DD4u, base, size, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x80073AC0u, 0u);
    for (i = 0u; i < 12u; ++i) {
        rect.x = (sint16)(240u * (i & 3u));
        rect.y = (sint16)(160u * ((i >> 2u) & 3u));
        rect.w = 160; rect.h = 160;
        gtos_title_image(base + r_u32(base + 16u + 12u * i), &rect);
    }
    for (i = 0u; i < 4u; ++i) {
        PSX_RECT *stored = (PSX_RECT *)psx_addr(0x8004D9F0u + 8u * i, 8u);
        uint32 entry;
        rect.x = stored->x; rect.y = stored->y; rect.w = 224; rect.h = 40;
        entry = r_u8(0x8004D9C6u + 12u * i + mode);
        gtos_title_image(base + r_u32(base + 16u + 12u * entry), &rect);
        StoreImage(stored, (uint32 *)psx_addr(0x8008CF7Cu + 27840u * (i + 4u), 27840u));
        DrawSync(0);
        entry = r_u8(0x8004D9C0u + 12u * i + mode);
        gtos_title_image(base + r_u32(base + 16u + 12u * entry), &rect);
        StoreImage(stored, (uint32 *)psx_addr(0x8008CF7Cu + 27840u * i, 27840u));
        DrawSync(0);
    }
}

static void gtos_title_variants(uint32 mask)
{
    uint32 i;
    for (i = 0u; i < 4u; ++i) {
        PSX_RECT *rect = (PSX_RECT *)psx_addr(0x8004D9F0u + 8u * i, 8u);
        uint32 pixels = 0x8008CF7Cu + 27840u * (4u * (mask & 1u) + i);
        LoadImagePSX(rect, (uint32 *)psx_addr(pixels, 27840u));
        DrawSync(0);
        mask = (uint32)((sint32)mask >> 1);
    }
}

static void gtos_title_sound(uint32 index)
{
    gtos_sound_trigger(r_u32(0x800757C4u) + 20u * index, 0x4000, 64, 0);
}

static void gtos_title_tick(void)
{
    uint32 pressed = gtos_input_poll(), selection = r_u32(0x8004DA14u);
    uint32 blink = r_u32(0x8004DA10u) + 1u, direction = pressed & 0xF0u;
    uint32 remaining;
    if (blink >= 12u) blink = 0u;
    w_u32(0x8004DA10u, blink);
    if (pressed) w_u32(0x8004DA18u, 1800u);
    if (direction == 16u || direction == 64u) {
        gtos_title_sound(0u);
        selection ^= 2u;
    } else if (direction == 32u || direction == 128u) {
        gtos_title_sound(0u);
        selection ^= 1u;
    } else if (!direction && (pressed & 0x6008u)) {
        w_u32(0x8004DA1Cu, 1u);
        gtos_title_sound(4u);
        gtos_title_variants(1u << (selection & 31u));
    }
    w_u32(0x8004DA14u, selection);
    remaining = r_u32(0x8004DA18u);
    w_u32(0x8004DA18u, remaining - 1u);
    if (remaining == 1u) {
        gtos_title_variants(0u);
        w_u32(0x8004DA1Cu, 1u);
    }
}

static void gtos_title_loop(void)
{
    uint32 display, draw, ot;
    gtos_graph_reset(1u);
    PutDispEnv((DISPENV *)psx_addr(0x8007A058u + 20u * r_u32(0x8007A138u), 20u));
    PutDrawEnv((DRAWENV *)psx_addr(0x8007A080u + 92u * r_u32(0x8007A13Cu), 92u));
    for (;;) {
        uint32 selection, blink;
        draw = r_u32(0x8007A138u);
        ot = r_u32(0x8007A154u + 4u * draw);
        ClearOTagR((uint32 *)psx_addr(ot, 20480u), 5120);
        w_u32(0x80074344u, (r_u32(0x80074330u) & 0xFFFFFFu) | 0x04000000u);
        w_u32(ot, 0x00074344u);
        gtos_title_tick();
        if (r_u32(0x8004DA1Cu)) break;
        selection = r_u32(0x8004DA14u) & 31u;
        blink = r_u32(0x8004DA10u);
        gtos_title_variants(((blink >= 5u) << selection) | ~(1u << selection));
        DrawSync(0);
        VSync(0);
        display = r_u32(0x8007A13Cu);
        w_u32(0x8007A138u, display);
        w_u32(0x8007A13Cu, draw);
        PutDispEnv((DISPENV *)psx_addr(0x8007A058u + 20u * display, 20u));
        PutDrawEnv((DRAWENV *)psx_addr(0x8007A080u + 92u * draw, 92u));
        if (!r_u32(0x8007A140u)) {
            w_u32(0x8007A140u, 1u);
            SetDispMask(1);
        }
        DrawOTag((uint32 *)psx_addr(r_u32(0x8007A154u + 4u * draw) + 20476u, 4u));
    }
    DrawSync(0);
}

static void gtos_menu_video_init(uint32 mode)
{
    uint32 i, buffers[2] = { 0x8015EF7Cu, 0x80163F7Cu };
    uint32 table = 0x800526C8u + (mode & 7u) * 20u;
    w_u8(0x8007370Cu, 30u);
    w_u8(0x8007370Du, 10u);
    w_u32(0x8007A150u, 0u);
    w_u32(0x8007A154u, buffers[0]);
    w_u32(0x8007A158u, buffers[1]);
    w_u32(0x8007A15Cu, 0u);
    w_u32(0x8007A160u, 5120u);
    for (i = 0u; i < 2u; ++i) {
        ClearOTagR((uint32 *)psx_addr(buffers[i], 20480u), 5120);
        w_u32(0x80074344u, (r_u32(0x80074330u) & 0xFFFFFFu) | 0x04000000u);
        w_u32(buffers[i], 0x00074344u);
    }
    w_u32(0x8007A030u, 0x80168F7Cu);
    w_u32(0x8007A034u, 0x80178F7Cu);
    w_u32(0x8007A020u, 65536u);
    w_u32(0x8007A040u, mode & 7u);
    w_u32(0x8007A044u, r_u16(table));
    w_u16(0x8007A048u, 0u);
    w_u16(0x8007A04Au, 0u);
    for (i = 0u; i < 4u; ++i)
        w_u16(0x8007A04Cu + i * 2u, r_u16(table + 2u + i * 2u));
    w_u16(0x8007A054u, r_u16(table + 2u));
    w_u16(0x8007A056u, r_u16(table + 4u));
    for (i = 0u; i < 2u; ++i) {
        uint32 rect = 0x8007A048u + 8u * i;
        sint32 x = (sint16)r_u16(rect), y = (sint16)r_u16(rect + 2u);
        sint32 width = (sint16)r_u16(rect + 4u), height = (sint16)r_u16(rect + 6u);
        uint32 env = 0x8007A058u + 20u * i;
        SetDefDispEnv((DISPENV *)psx_addr(env, 20u), x, y, width, height);
        SetDefDrawEnv((DRAWENV *)psx_addr(0x8007A080u + 92u * i, 92u), x, y, width, height);
        if (mode & 16u) w_u8(env + 17u, 1u);
    }
    w_u32(0x8007A138u, 0u);
    w_u32(0x8007A13Cu, 1u);
    w_u32(0x8007A144u, 1u);
}

static uint32 gtos_menu_prepare(void)
{
    gtos_menu_video_init(22u);
    w_u32(0x80075FC8u, 0u);
    w_u32(0x80075FCCu, 32u);
    if (!gtos_render_node_insert(0x80075FC0u))
        return gt_draft_unresolved_call(0x8002A3ACu, 0x80075FC0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    VSync(0);
    w_u32(0x8007A140u, 0u);
    SetDispMask(0);
    gtos_title_load();
    gtos_input_reset();
    w_u32(0x8004DA14u, 0u);
    w_u32(0x8004DA10u, 0u);
    w_u32(0x8004DA18u, 1800u);
    w_u32(0x8004DA1Cu, 0u);
    w_u8(0x8007ABDFu, 0u);
    gtos_title_loop();
    return r_u32(0x8004DA18u);
}

static void gtos_menu_hide(void)
{
    VSync(0);
    w_u32(0x8007A140u, 0u);
    SetDispMask(0);
}

static void gtos_menu_layout(uint32 state, uint32 descriptor)
{
    uint32 callback = r_u32(descriptor + 16u);
    if (callback == 0x80017CCCu) {
        w_u8(0x800762A4u, 24u);
        w_u8(0x800762A5u, 0u);
    } else {
        /* TODO Translate other original menu layout callbacks */
        gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    w_u8(state + 274u, 1u);
    w_u16(state + 266u, 320u);
    w_u16(state + 268u, 240u);
    w_u16(state + 270u, 27u);
    w_u8(state + 256u, 0u);
    w_u32(state + 192u, descriptor);
    w_u16(state + 272u, 12u);
}

static void gtos_menu_push(uint32 descriptor)
{
    uint32 state = r_u32(0x80078588u);
    sint32 depth = (sint8)r_u8(state + 256u);
    if (depth < 15) {
        ++depth;
        w_u8(state + 256u, (uint8)depth);
        w_u32(state + 192u + 4u * (uint32)depth, descriptor);
    }
}

static sint32 gtos_menu_list_input(uint32 list, uint32 input)
{
    sint32 selection = (sint8)r_u8(list + 1u), count = (sint8)r_u8(list);
    uint32 pressed = r_u16(input + 4u), direction;
    w_u8(list + 12u, (uint8)selection);
    if (pressed & 0x9000u) return -2;
    if (pressed & 0x6000u) {
        if (!(r_u8(r_u32(list + 4u) + 8u * (uint32)selection) & 0x40u)) return selection;
        gtos_title_sound(6u);
    }
    direction = pressed | r_u16(input + 8u);
    if (direction & 16u) {
        gtos_title_sound(0u);
        --selection;
        if (selection < 0) selection = r_u8(list + 11u) ? count - 1 : 0;
    }
    if (direction & 64u) {
        gtos_title_sound(0u);
        ++selection;
        if (selection >= count) selection = r_u8(list + 11u) ? 0 : count - 1;
    }
    w_u8(list + 1u, (uint8)selection);
    return -1;
}

static sint32 gtos_arcade_root_input(void)
{
    static const uint32 pages[4] = { 0x8004E358u, 0x8004E374u, 0x8004E3C8u, 0x8004EB00u };
    uint32 state = r_u32(0x80078588u), value;
    sint32 selected;
    gtos_music_tick();
    if (!r_u8(state)) return 0;
    selected = gtos_menu_list_input(0x8004E0ECu, state + 2u);
    if (selected == -1) return 0;
    if (selected == -2) {
        uint8 response[8] = { 0 };
        w_u8(0x800762E1u, 0u);
        w_u8(0x8004F6A3u, 2u);
        if (!CdControl(9u, NULL, response))
            return gt_draft_unresolved_call(0x8002BA84u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u8(0x8004F6A6u, 0u);
        printf("stop xa\n");
        gtos_title_sound(2u);
        return 1;
    }
    if (selected >= 0 && selected < 4) {
        gtos_title_sound(4u);
        w_u8(0x800787D1u, selected == 3 ? 6u : (uint32)selected);
        gtos_menu_push(pages[selected]);
        if (selected < 3) {
            value = r_u8(0x8004E334u + (uint32)selected);
            w_u8(0x8004E38Cu, value);
            w_u8(0x8004E3A8u, value);
            w_u8(0x8004E3C4u, value);
        }
        w_u8(0x800762E0u, (uint8)selected);
        return 3;
    }
    if (selected == 4 || selected == 5) {
        if (selected == 5) w_u8(0x800762E4u, 1u);
        gtos_title_sound(4u);
        if (selected == 4) w_u8(0x800762E4u, 0u);
        w_u32(0x801A7F7Cu, 0x80188F7Cu);
        w_u32(0x801A7F80u, 5u);
        w_u8(0x801A7F84u, 0u);
        w_u8(0x801A7F85u, selected == 4);
        w_u32(0x801A7F88u, 0x8004BEC0u);
        memcpy(psx_addr(0x801A7F8Cu, 25u), psx_addr(0x8004BED0u, 25u), 25u);
        w_u32(0x801A8428u, 0x80019B00u);
        w_u32(0x8007AA2Cu, 0x80019AB4u);
        gtos_menu_push(0x8004DC84u);
        return 3;
    }
    gtos_title_sound(6u);
    return 0;
}

static void gtos_midi_unlink(uint32 sequence)
{
    uint32 current = r_u32(0x8007672Cu), previous = 0u;
    while (current) {
        if (current == sequence) {
            w_u32(previous ? previous : 0x8007672Cu, r_u32(current));
            return;
        }
        previous = current;
        current = r_u32(current);
    }
}

static void gtos_midi_stop(uint32 sequence)
{
    uint32 i;
    gtos_midi_unlink(sequence);
    if (r_u32(0x80076744u) == sequence) w_u32(0x80076744u, 0u);
    for (i = 0u; i < 24u; ++i) {
        uint32 owner = 0x80076760u + 28u * i;
        if (r_u32(owner) == sequence) w_u32(owner, 0u);
    }
}

static void gtos_midi_start(uint32 index)
{
    uint32 sequence = 0x80075FECu, data, i, tempo, volume, product;
    printf("start midi %u.\n", index);
    if ((sint8)r_u8(0x80076298u)) gtos_midi_stop(sequence);
    data = r_u32(0x80076294u) + 72u * index + 12u;
    gtos_midi_unlink(sequence);
    w_u32(sequence, 0u);
    w_u16(sequence + 4u, r_u16(data));
    product = (uint32)r_u16(0x80076732u) * r_u16(data);
    w_u16(sequence + 6u, (uint16)((sint32)product >> 14u));
    w_u32(sequence + 24u, r_u32(0x80075FE4u));
    tempo = r_u16(0x80076730u);
    if (!tempo) {
        /* TODO Report the original MIDI division trap through a native adapter */
        gt_draft_unresolved_call(0x8003D130u, sequence, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        return;
    }
    tempo = 480000000u / tempo;
    w_u32(sequence + 16u, tempo); w_u32(sequence + 12u, tempo);
    w_u32(sequence + 8u, r_u32(data + 4u));
    for (i = 0u; i < 16u; ++i) {
        uint32 row = sequence + 40u + 40u * i;
        uint32 cursor = r_u32(data + 8u + 4u * i), delta, byte;
        w_u32(row, cursor);
        byte = r_u8(cursor++); delta = byte;
        w_u32(row, cursor); w_u32(row + 4u, delta);
        if (byte & 128u) {
            delta = byte & 127u;
            w_u32(row + 4u, delta);
            do {
                byte = r_u8(cursor++);
                delta = (delta << 7u) + (byte & 127u);
                w_u32(row, cursor); w_u32(row + 4u, delta);
            } while (byte & 128u);
        }
        w_u16(row + 8u, 0x4000u);
        w_u16(row + 12u, 0u);
        w_u8(row + 16u, 0u); w_u8(row + 17u, 0u); w_u8(row + 18u, 255u);
        w_u16(row + 10u, r_u16(sequence + 6u));
    }
    gtos_midi_unlink(sequence);
    product = (uint32)r_u16(0x80076732u) * r_u16(sequence + 4u);
    volume = (uint32)((sint32)product >> 14u);
    w_u16(sequence + 6u, (uint16)volume);
    for (i = 0u; i < 16u; ++i) {
        uint32 row = sequence + 40u + 40u * i;
        w_u16(row + 10u, (uint16)(((uint32)r_u16(row + 8u) * volume) >> 14u));
    }
    w_u32(sequence, r_u32(0x8007672Cu));
    w_u32(0x8007672Cu, sequence);
    w_u8(0x80076298u, 1u);
    printf("start midi %u.\n", index);
}

static sint32 gtos_arcade_car_type_input(void)
{
    uint32 state = r_u32(0x80078588u);
    sint32 selected;
    if (!(sint8)r_u8(state)) return 0;
    selected = gtos_menu_list_input(0x8004E14Cu, state + 2u);
    if (selected == -2) {
        printf("stop midi.\n");
        if ((sint8)r_u8(0x80076298u)) gtos_midi_stop(0x80075FECu);
        w_u8(0x80076298u, 0u);
        printf("stop midi.\n");
        gtos_title_sound(2u);
        return 1;
    }
    if (selected == -1) return 0;
    gtos_title_sound(4u);
    w_u8(0x800787D3u, (uint8)selected);
    gtos_menu_push(0x8004E390u);
    w_u8(0x8004E414u, r_u8(0x8004DF20u + (uint32)selected));
    return 3;
}

static sint32 gtos_arcade_car_type_init(void)
{
    uint8 response[8] = { 0 };
    uint32 i, count = 6u;
    w_u8(0x800762E1u, 0u);
    w_u8(0x8004F6A3u, 2u);
    if (!CdControl(9u, NULL, response))
        return gt_draft_unresolved_call(0x8002BA84u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u8(0x8004F6A6u, 0u);
    printf("stop xa\n");
    if (!r_u8(0x8007ABDDu)) w_u8(0x8004E104u, 8u);
    for (i = 8u; i < 12u; ++i) {
        uint32 row = 0x800816C8u + 4u * i;
        if (r_u8(row) && r_u8(row + 1u) && r_u8(row + 2u)) ++count;
    }
    w_u8(0x8004E14Cu, (uint8)count);
    gtos_midi_start(1u);
    w_u8(0x8004E14Du, 0u);
    w_u8(0x8004E158u, 0u);
    w_u8(0x8004E159u, 0u);
    return 0;
}

static void gtos_arcade_car_reset(void)
{
    uint32 pending = r_u8(0x8004F6A6u);
    if (!pending || pending - 1u < 8u) {
        if (pending == 6u) {
            /* TODO Cancel an original CD read already in progress */
            gt_draft_unresolved_call(0x8002C57Cu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
        w_u8(0x8004F6A3u, !pending || pending == 6u ? 0u : 2u);
    }
    w_u8(0x8004E2D0u, 0u);
    w_u8(0x8004E330u, 0u);
}

static void gtos_arcade_car_buffers(uint32 player)
{
    sint32 category = (sint8)r_u8(0x800787D3u + player);
    uint32 cursor = r_u32(player ? 0x800786E0u : 0x800786E4u), i;
    uint32 index = (uint32)(sint32)(sint8)r_u8(0x8004E2FCu + (uint32)category);
    w_u8(0x800762E2u + player, 1u);
    for (i = 0u; i < 4u; ++i, index += 2u) {
        uint32 row = 0x800787F0u + 80u * player + 20u * i;
        uint32 archive = r_u32(0x800762A8u), entry = archive + 12u * index;
        w_u32(row, cursor);
        w_u32(row + 4u, r_u32(entry + 24u));
        cursor = (cursor + r_u32(entry + 20u) + 3u) & 0xFFFFFFFCu;
        entry = archive + 12u * (index + 1u);
        w_u32(row + 8u, cursor);
        w_u32(row + 12u, r_u32(entry + 24u));
        cursor = (cursor + r_u32(entry + 20u) + 3u) & 0xFFFFFFFCu;
        w_u8(row + 16u, 0u);
    }
    w_u8(0x800762D2u + player, 0u);
    w_u8(0x800762D6u + player, 0u);
}

static sint32 gtos_arcade_car_init(void)
{
    sint32 category = (sint8)r_u8(0x800787D3u), count, selected;
    uint32 i;
    if (!r_u8(0x8007ABDDu)) {
        w_u8(0x8004DF54u, 12u);
        w_u8(0x8004DF68u, 12u);
    }
    w_u8(0x800792B0u, 1u);
    count = (sint8)r_u8(0x80078560u + 4u * (uint32)category);
    w_u8(0x80079278u, (uint8)count);
    selected = count - 1;
    if (selected < 0) selected = 0;
    w_u8(0x80079270u, (uint8)selected);
    w_u16(0x80079274u, 3584u);
    w_u8(0x8004E415u, 0u);
    w_u8(0x8004E421u, 0u);
    w_u8(0x8004E422u, 0u);
    for (i = 0u; i < 4u; ++i) {
        uint32 entry = 0x8004DF30u + 40u * (uint32)category + 10u * i;
        uint32 row = 0x800792A0u + 4u * i;
        w_u8(row, 0u); w_u8(row + 1u, r_u8(entry - 2u));
        w_u8(row + 2u, r_u8(entry - 1u)); w_u8(row + 3u, r_u8(entry));
    }
    /* TODO Resolve the remaining car menu preparation helpers */
    w_u8(0x80079290u, 0u);
    gtos_arcade_category_image(category, (sint8)r_u8(0x8004E415u), 0u);
    selected = (sint8)r_u8(0x8004E415u);
    w_u32(0x8004E3E8u + 12u * (uint32)selected, 0x8004E4E4u);
    w_u8(0x800792A0u + 4u * (uint32)selected, 0u);
    gtos_arcade_car_reset();
    gtos_arcade_car_buffers(0u);
    if ((sint8)r_u8(0x800762D2u) != 0) {
        w_u8(0x800762D2u, 0u);
        w_u8(0x800762D6u, 0u);
    }
    return 0;
}

static sint32 gtos_menu_binary_input(uint32 list, uint32 input)
{
    uint32 pressed, directions;
    if ((sint8)r_u8(list + 13u) > 0) return -1;
    pressed = r_u16(input + 4u);
    directions = pressed | r_u16(input + 8u);
    if (directions & 128u) { gtos_sound_trigger(0u, 0x4000, 64, 0); w_u8(list + 12u, 0u); }
    if (directions & 32u) { gtos_sound_trigger(0u, 0x4000, 64, 0); w_u8(list + 12u, 1u); }
    pressed = r_u16(input + 4u);
    if (pressed & 0x9000u) return -2;
    return pressed & 0x6000u ? (sint8)r_u8(list + 12u) : -1;
}

static sint32 gtos_menu_category_input(uint32 list, uint32 input)
{
    sint32 selected = r_u8(list + 1u);
    uint32 layout = r_u8(list + 12u) & 31u, entries = r_u32(list + 8u);
    uint32 pressed, directions, back, forward;
    if ((sint8)r_u8(list + 14u)) {
        uint8 tick = (uint8)(r_u8(list + 15u) + 1u);
        w_u8(list + 15u, tick);
        if ((sint8)tick >= 12) { w_u8(list + 14u, 0u); w_u8(list + 15u, 0u); }
        return -1;
    }
    w_u8(list + 13u, (uint8)selected);
    pressed = r_u16(input + 4u);
    if (pressed & 0x6000u) return (sint8)selected;
    if (pressed & 0x9000u) return -2;
    directions = pressed | r_u16(input + 8u);
    back = directions & (layout == 1u ? 16u : 128u);
    forward = directions & (layout == 1u ? 64u : 32u);
    if (back && forward) return -1;
    if (back) {
        gtos_sound_trigger(1u, 0x4000, 64, 0);
        if (layout < 2u) w_u8(list + 14u, layout == 1u ? 3u : 1u);
        do {
            --selected;
            if ((sint8)selected < 0) selected = r_u8(list) - 1;
        } while (r_u8(entries + 12u * (uint32)(sint32)(sint8)selected) & 64u);
    } else if (forward) {
        gtos_sound_trigger(1u, 0x4000, 64, 0);
        if (layout < 2u) { w_u8(list + 14u, layout == 1u ? 4u : 2u); w_u8(list + 15u, 0u); }
        do {
            ++selected;
            if ((sint8)selected >= (sint8)r_u8(list)) selected = 0;
        } while (r_u8(entries + 12u * (uint32)(sint32)(sint8)selected) & 64u);
    }
    w_u8(list + 1u, (uint8)selected);
    return (sint8)r_u8(list + 13u) == (sint8)selected ? -1 : -3;
}

static sint32 gtos_arcade_archive_read(sint32 index, uint32 destination)
{
    sint32 state = (sint8)r_u8(0x800762C8u);
    if (state == 1) {
        FILE *file = NULL;
        uint32 entry = r_u32(0x800762A8u) + 12u * (uint32)index;
        uint32 offset = r_u32(entry + 16u) & 0xFFFFF800u;
        uint32 bytes = (r_u32(entry + 20u) + 3u) & 0xFFFFFFFCu;
        size_t received = 0u;
        uint32 error = 0u;
        if (fopen_s(&file, "DATA/CARCADE.DAT", "rb") || !file) return -1;
        if (_fseeki64(file, offset, SEEK_SET)) error = 1u;
        else {
            received = fread(psx_addr(destination, bytes), 1u, bytes, file);
            if (received != bytes) error = 1u;
        }
        fclose(file);
        /* Native file completion replaces guest CD command execution */
        w_u8(0x8004F6A5u, error);
        w_u8(0x8004F6A6u, 0u);
        w_u8(0x800762C8u, 2u);
        return 2;
    }
    if (state == 2) {
        if (r_u8(0x8004F6A6u)) return 3;
        w_u8(0x800762C8u, 3u);
        if (r_u8(0x8004F6A5u)) { w_u8(0x800762C8u, 1u); return 1; }
        return 3;
    }
    return 2;
}

static sint32 gtos_arcade_car_read_step(sint32 category, uint32 player)
{
    sint32 state = (sint8)r_u8(0x800762C8u), part = (sint8)r_u8(0x800762D1u);
    sint32 pair = (sint8)r_u8(0x800762D0u);
    uint32 row = 0x800787F0u + 80u * player + 20u * (uint32)pair;
    if (state < 3) {
        if (state < 0) return 0;
        if (!state) w_u8(0x800762C8u, 1u);
        if (part == 0 || part == 1) {
            sint32 index = 2 * pair + (sint8)r_u8(0x8004E2FCu + (uint32)category) + part;
            gtos_arcade_archive_read(index, r_u32(row + (part ? 8u : 0u)));
        }
        return 0;
    }
    if (state != 3) return 0;
    w_u8(0x800762C8u, 0u);
    if (!part) w_u8(0x800762D1u, 1u);
    else if (part == 1) {
        w_u8(row + 16u, 1u);
        w_u8(0x800762D1u, 0u);
        w_u8(0x800762D0u, (uint8)(r_u8(0x800762D0u) + 1u));
    }
    if ((sint8)r_u8(0x800762D0u) >= 4) { w_u8(0x8004E2D0u, 0u); return 1; }
    return 0;
}

static sint32 gtos_arcade_car_load_tick(uint32 player)
{
    sint32 owner;
    if ((sint8)r_u8(0x800762E2u + player) != 1) return 0;
    owner = (sint8)r_u8(0x8004E330u);
    if (owner == (sint32)(player + 1u)) {
        if (gtos_arcade_car_read_step((sint8)r_u8(0x800787D3u + player), player) == 1) {
            w_u8(0x8004E330u, 0u); w_u8(0x800762E2u + player, 2u);
            return 1;
        }
    } else if (!owner) {
        w_u8(0x8004E330u, player + 1u);
        if ((sint8)r_u8(0x8004E2D0u) != 1) {
            w_u8(0x800762D0u, 0u); w_u8(0x800762D1u, 0u);
            w_u8(0x800762C8u, 0u); w_u8(0x8004E2D0u, 1u);
        }
    }
    return 0;
}

static uint32 gtos_arcade_model_group(uint32 group)
{
    uint32 cursor = group + 80u, i;
    w_u32(group, cursor);
    cursor += 8u * r_u16(group + 40u);
    w_u32(group + 4u, cursor);
    cursor += 8u * r_u16(group + 42u);
    for (i = 0u; i < 8u; ++i) {
        w_u32(group + 8u + 4u * i, cursor);
        cursor += (uint32)r_u16(group + 44u + 2u * i) * r_u16(0x8004C9B8u + 2u * i);
    }
    return cursor;
}

static void gtos_arcade_model_bind(uint32 model, uint32 page)
{
    uint32 count = r_u16(model + 60u), cursor = model + 64u + 8u * count;
    uint32 i, kind, primitive;
    for (i = 0u; i < count; ++i) {
        w_u32(model + 68u + 8u * i, cursor);
        cursor = gtos_arcade_model_group(cursor);
    }
    for (i = 0u; i < count; ++i) {
        uint32 address = model + 64u + 8u * i;
        uint32 scaled = (uint32)((sint32)r_u32(address) >> 14u);
        w_u32(address, scaled * scaled);
    }
    w_u32(model + 56u, cursor);
    w_u32(cursor + 24u, cursor + 32u);
    w_u32(cursor + 28u, cursor + 32u + 24u * r_u16(cursor));
    for (i = 0u; i < count; ++i) {
        uint32 group = r_u32(model + 68u + 8u * i);
        for (kind = 4u; kind < 8u; ++kind) {
            uint32 packet = r_u32(group + 8u + 4u * kind);
            uint32 total = r_u16(group + 44u + 2u * kind), stride = r_u16(0x8004C9B8u + 2u * kind);
            for (primitive = 0u; primitive < total; ++primitive, packet += stride)
                w_u32(packet + 20u, (r_u32(packet + 20u) & 0xFFE0FFFFu) | ((page & 31u) << 16u));
        }
    }
}

static void gtos_arcade_car_prepare(sint32 category, sint32 choice, uint32 player)
{
    sint32 group = (sint8)r_u8(0x8004E2D4u + 4u * (uint32)category + (uint32)choice);
    uint32 row = 0x800787F0u + 20u * (uint32)group + 80u * player;
    uint32 stage = (uint32)(sint32)(sint8)r_u8(0x800762D6u + player);
    uint32 image = player ? 0x80152F7Cu : 0x80146F7Cu;
    uint32 page = player ? 23u : 21u, i;
    PSX_RECT rect;
    if (!r_u8(row + 16u)) { w_u8(0x800762D4u + player, 0u); return; }
    if (stage < 5u) w_u8(0x800762D6u + player, stage + 1u);
    else if (stage <= 7u && (r_u8(0x8004DC78u) & 1u) == (player ? 1u : 0u)) {
        if (stage == 5u) {
            gtos_data_unpack(image, r_u32(row), r_u32(row + 4u));
            w_u8(0x800762D6u + player, 6u);
        } else if (stage == 6u) {
            sint32 palettes;
            rect.x = (sint16)((page & 15u) << 6u); rect.y = (sint16)((page >> 4u) << 8u);
            rect.w = 64; rect.h = 256;
            DrawSync(0); LoadImagePSX(&rect, (uint32 *)psx_addr(image + 96u, 32768u)); DrawSync(0);
            palettes = (sint16)r_u16(image + 14u);
            for (i = 0u; (sint32)i < palettes; ++i) {
                rect.x = player ? 512 : 384; rect.y = (sint16)(256u + 4u * i);
                rect.w = 64; rect.h = 4;
                DrawSync(0); LoadImagePSX(&rect, (uint32 *)psx_addr(image + 32864u + 512u * i, 512u)); DrawSync(0);
            }
            w_u8(0x800762D6u + player, 7u);
        } else {
            uint32 model = player ? 0x80140F7Cu : 0x8013AF7Cu;
            gtos_data_unpack(model, r_u32(row + 8u), r_u32(row + 12u));
            w_u8(0x800762D6u + player, 8u);
            gtos_arcade_model_bind(model, page);
        }
    }
    w_u8(0x800762D4u + player, 1u);
}

static sint32 gtos_arcade_car_input(void)
{
    uint32 state = r_u32(0x80078588u), mode, list, pulse;
    sint32 selected, category, choice, count;
    uint8 frame = (uint8)(r_u8(0x8004DA74u) + 1u);
    if ((sint8)frame >= 13) frame = 0u;
    w_u8(0x8004DA74u, frame);
    w_u32(0x8004DB1Cu, r_u32(0x8004DB28u + 4u * (uint32)(sint32)(sint8)frame));
    /* TODO Advance original car resource loading and model preparation */
    gtos_arcade_car_load_tick(0u);
    category = (sint8)r_u8(0x800787D3u);
    choice = (sint8)r_u8(0x8004E415u);
    gtos_arcade_car_prepare(category, choice, 0u);
    if (!(sint8)r_u8(state)) return 0;
    mode = (uint32)(sint32)(sint8)r_u8(0x800792B0u);
    if (mode == 1u) {
        selected = gtos_menu_category_input(0x8004E414u, state + 2u);
        if (selected == -2) {
            gtos_title_sound(2u); gtos_arcade_car_reset();
            if ((sint8)r_u8(0x8004E330u) == 1) gtos_arcade_car_reset();
            w_u8(0x800762E2u, 0u);
            return 1;
        }
        if (selected == -3) {
            choice = (sint8)r_u8(0x8004E415u);
            pulse = r_u8(0x80079290u) ^ 1u;
            w_u8(0x80079290u, pulse);
            gtos_arcade_category_image((sint8)r_u8(0x800787D3u), choice, (uint32)(sint32)(sint8)pulse);
            w_u32(0x8004E3E8u + 12u * (uint32)choice, 0x8004E4E4u + 16u * pulse);
            w_u8(0x800792A0u + 4u * (uint32)choice, 0u);
            if ((sint8)r_u8(0x800762D2u) != choice) {
                w_u8(0x800762D2u, (uint8)choice); w_u8(0x800762D6u, 0u);
            }
            count = (sint8)r_u8(0x80078560u + 4u * (uint32)(sint32)(sint8)r_u8(0x800787D3u) + (uint32)choice);
            w_u8(0x80079278u, (uint8)count);
            selected = count - (choice + 1);
            w_u8(0x80079270u, (uint8)(selected < 0 ? 0 : selected));
            w_u16(0x80079274u, 3584u);
            return 0;
        }
        if (selected == -1) {
            uint32 pressed = r_u16(state + 6u);
            sint32 car = (sint8)r_u8(0x80079270u), previous = car;
            if (pressed & 16u) { --car; if (car < 0) car = (sint8)r_u8(0x80079278u) - 1; }
            if (pressed & 64u) { ++car; if (car >= (sint8)r_u8(0x80079278u)) car = 0; }
            if (car != previous) gtos_title_sound(0u);
            w_u8(0x80079270u, (uint8)car);
            return 0;
        }
        gtos_title_sound(5u);
        w_u8(0x800787D5u, (uint8)selected); w_u8(0x800787D6u, (uint8)selected);
        w_u8(0x800792B0u, 2u); w_u8(0x800787D7u, r_u8(0x80079270u));
        w_u8(0x8004E470u, 0u); w_u8(0x8004E471u, 12u);
        return 0;
    }
    if (mode != 2u && mode != 24u) return 0;
    list = mode == 2u ? 0x8004E464u : 0x8004E474u;
    selected = gtos_menu_binary_input(list, state + 2u);
    if (selected == -2) {
        gtos_title_sound(3u);
        w_u8(0x800792B0u, mode == 2u ? 1u : 2u);
        if (mode == 24u) w_u8(0x8004E471u, 12u);
        return 0;
    }
    if (selected == -1) return 0;
    if (mode == 2u) {
        gtos_title_sound(5u);
        w_u8(0x800787D9u, (uint8)selected); w_u8(0x800787DAu, (uint8)selected);
        w_u8(0x800792B0u, 24u);
        w_u8(0x8004E480u, 0u); w_u8(0x8004E481u, 12u);
        return 0;
    }
    gtos_title_sound(4u); gtos_arcade_car_reset(); gtos_arcade_car_buffers(0u);
    choice = (sint8)r_u8(0x8004E415u);
    if ((sint8)r_u8(0x800762D2u) != choice) {
        w_u8(0x800762D2u, (uint8)choice); w_u8(0x800762D6u, 0u);
    }
    w_u8(0x800787DBu, (uint8)selected); w_u8(0x800787DCu, (uint8)selected);
    gtos_menu_push(0x8004E3ACu);
    return 3;
}

sint32 gtos_arcade_track_input(void)
{
    uint32 state = r_u32(0x80078588u), list = 0x8004E20Cu;
    uint32 tracks = 0x8004E184u, label = 36u;
    sint32 selected;
    /* The descriptor entry at 8001AB74 loads the mode before the exported owner */
    if ((sint8)r_u8(0x800787D1u) == 6) {
        list = 0x8004E250u; tracks = 0x8004E21Cu; label = 44u;
    }
    gtos_arcade_track_preview_tick();
    if (!(sint8)r_u8(state)) return 0;
    selected = gtos_menu_category_input(list, state + 2u);
    if (selected == -2) {
        gtos_arcade_track_preview_stop();
        gtos_title_sound(2u);
        return 1;
    }
    if (selected == -1) return 0;
    if (selected == -3) {
        sint32 index = (sint8)r_u8(list + 1u);
        gtos_arcade_track_label(label + (uint32)index);
        gtos_arcade_track_preview_pause();
        gtos_arcade_track_preview_request(r_u8(tracks + (uint32)index));
        return 0;
    }
    gtos_arcade_track_preview_stop();
    gtos_title_sound(4u);
    w_u8(0x800787DDu, (uint8)selected);
    w_u8(0x800787D0u, 1u);
    gtos_menu_push(0x8004DD44u);
    printf("stop midi.\n");
    if ((sint8)r_u8(0x80076298u)) gtos_midi_stop(0x80075FECu);
    w_u8(0x80076298u, 0u);
    printf("stop midi.\n");
    return 3;
}

static sint32 gtos_arcade_track_init(void)
{
    uint32 track, entry = 0x8004E1DCu, list, label, selected;
    for (track = 4u; track < 8u; ++track, entry += 12u) {
        uint32 progress = 0x800816C8u + 4u * track;
        uint32 unlocked = r_u8(progress) && r_u8(progress + 1u) && r_u8(progress + 2u);
        w_u8(entry, unlocked ? r_u8(entry) & 0x3Fu : r_u8(entry) | 0x40u);
    }
    {
        uint32 loader = 0x800791F0u, pixels = r_u32(0x8007925Cu), compressed = r_u32(0x80079254u);
        w_u8(0x80079250u, 0u);
        memset(psx_addr(loader, 28u), 0, 28u);
        memset(psx_addr(loader + 28u, 28u), 0, 28u);
        w_u8(loader + 36u, 1u); w_u8(loader + 37u, 0u);
        w_u32(0x80079F10u, loader);
        w_u32(loader + 52u, 0x80018A20u);
        w_u16(loader + 10u, 4u); w_u32(loader + 12u, 0x80079240u);
        w_u32(loader + 56u, pixels); w_u32(loader + 60u, pixels + 0x8000u);
        w_u32(loader + 44u, compressed); w_u32(loader + 48u, compressed + 2560u);
    }
    if ((sint8)r_u8(0x800787D1u) == 6) {
        list = 0x8004E250u; label = 44u; selected = 0x8004E21Cu;
    } else {
        list = 0x8004E20Cu; label = 36u; selected = 0x8004E184u;
    }
    w_u8(list + 1u, 0u); w_u8(list + 13u, 0u); w_u8(list + 14u, 0u);
    gtos_arcade_track_label(label);
    gtos_arcade_track_preview_request(r_u8(selected));
    return 0;
}

static sint32 gtos_menu_callback(uint32 callback)
{
    if (callback == 0x80017D08u) {
        uint32 remaining = r_u8(0x800762A4u) - 1u;
        w_u8(0x800762A4u, remaining);
        return (sint32)(remaining << 24u) < 1 ? 7 : 0;
    }
    if (callback == 0x80017D3Cu) {
        w_u8(0x800762A4u, 24u);
        return 0;
    }
    if (callback == 0x80019BA8u) return gtos_arcade_root_input();
    if (callback == 0x800146ACu) return 0;
    if (callback == 0x8001AA2Cu) return gtos_arcade_car_init();
    if (callback == 0x8001A2F8u) return gtos_arcade_car_input();
    if (callback == 0x8001A07Cu) return gtos_arcade_car_type_input();
    if (callback == 0x8001A290u) return gtos_arcade_car_type_init();
    if (callback == 0x8001B338u) return gtos_arcade_track_init();
    if (callback == 0x8001AB74u || callback == 0x8001AB7Cu) return gtos_arcade_track_input();
    if (callback == 0x8001A014u) {
        sint32 count = (sint8)r_u8(0x8004E174u);
        w_u8(0x8004E175u, (uint8)(count > 1 ? 1 : count - 1));
        w_u8(0x8004E180u, 0u);
        w_u8(0x8004E181u, 0u);
        return 0;
    }
    if (callback == 0x80019EE0u) {
        uint32 state = r_u32(0x80078588u);
        sint32 selected;
        gtos_music_tick();
        if (!r_u8(state)) return 0;
        selected = gtos_menu_list_input(0x8004E174u, state + 2u);
        if (selected == -2) { gtos_title_sound(2u); return 1; }
        if (selected == -1) return 0;
        gtos_title_sound(4u);
        w_u8(0x800787D2u, (uint8)selected);
        gtos_menu_push(0x8004E374u);
        return 3;
    }
    if (callback == 0x8001A050u) {
        printf("level ret ok\n");
        w_u8(0x800762E1u, 1u);
        gtos_music_prepare(23u, 1u);
        return 0;
    }
    if (callback == 0x80019EA0u) {
        sint32 previous = (sint8)r_u8(0x800762E0u);
        if (previous > 0 && previous < 3) {
            w_u8(0x800762E1u, 1u);
            gtos_music_prepare(23u, 1u);
            printf("set xa\n");
        }
        return -1;
    }
    if (callback == 0x80019E70u) {
        w_u8(0x800762E1u, 1u);
        gtos_music_prepare(23u, 1u);
        printf("set xa\n");
        w_u8(0x8004E0EDu, 0u);
        w_u8(0x8004E0F8u, 0u);
        w_u8(0x8004E0F9u, 0u);
        return 0;
    }
    if (callback == 0x80017C5Cu) {
        if (r_u8(0x800762A5u)) {
            uint8 remaining = (uint8)(r_u8(0x800762A4u) - 1u);
            w_u8(0x800762A4u, remaining);
            return (sint8)remaining <= 0 ? 7 : 0;
        } else {
            gtos_menu_push(r_u32(0x800762A0u));
            return 3;
        }
    }
    if (callback == 0x80017CCCu || callback == 0x80017CE8u) {
        w_u8(0x800762A4u, 24u);
        w_u8(0x800762A5u, callback == 0x80017CE8u);
        return 0;
    }
    /* TODO Translate the remaining menu descriptor callbacks */
    return (sint32)gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static uint32 gtos_menu_descriptor(uint32 state)
{
    return r_u32(state + 192u + 4u * (uint32)(sint32)(sint8)r_u8(state + 256u));
}

static sint32 gtos_menu_pop(uint32 state)
{
    sint32 depth = (sint8)r_u8(state + 256u);
    if (depth <= 0) return -1;
    w_u8(state + 256u, (uint8)(depth - 1));
    return 0;
}

static void gtos_menu_motion(uint32 state, uint32 next)
{
    sint32 old = (sint8)r_u8(state + 259u);
    if (old == 0 || (old == 1 && next == 1u) || (old == 2 && next == 2u))
        w_u16(state + 262u, r_u16(state + 266u));
    else if ((old == 1 && next == 2u) || (old == 2 && next == 1u))
        w_u16(state + 262u, 320u - r_u16(state + 262u));
    if (next == 3u || next == 4u) w_u16(state + 264u, r_u16(state + 268u));
    w_u8(state + 259u, next);
}

static void gtos_arcade_update(void)
{
    uint32 state = r_u32(0x80078588u), descriptor, active, motion, offset, value;
    sint32 result;
    w_u8(0x8004DC71u, 0u);
    w_u8(state + 258u, 4u);
    value = r_u8(0x8004DC70u);
    w_u8(0x8004DC70u, (sint8)value < 16 ? value + 1u : 0u);
    value = r_u8(0x8004DC78u);
    w_u8(0x8004DC78u, (sint8)value < 32 ? value + 1u : 0u);
    w_u8(0x8004DC79u, (r_u32(0x8004DC74u) >> (r_u8(0x8004DC78u) & 31u)) & 1u);
    motion = r_u8(state + 259u);
    if ((sint8)motion > 0 && (sint8)motion < 5) {
        offset = motion < 3u ? 262u : 264u;
        value = (uint16)(r_u16(state + offset) - r_u16(state + offset + 8u));
        w_u16(state + offset, value);
        if ((sint16)value < 0) w_u8(state + 259u, 0u);
    }
    gtos_input_poll_at(state + 2u, 0u);
    gtos_input_poll_at(state + 76u, 1u);
    w_u32(state + 152u, 0x8007AA90u);
    w_u32(state + 156u, 0x8007AAD0u);
    active = (uint32)(r_u8(state + 259u) - 3u) >= 2u;
    w_u8(state, active); w_u8(state + 1u, active);
    descriptor = gtos_menu_descriptor(state);
    result = gtos_menu_callback(r_u32(descriptor + 8u));
    if (!active) return;
    switch (result) {
    case 0:
        if (r_u8(descriptor) == 1u && (r_u16(state + 6u) & 8u)) w_u8(state + 257u, 1u);
        return;
    case 2:
        if (gtos_menu_pop(state) < 0) w_u8(state + 257u, 1u);
        /* The original case two pops twice */
    case 1:
        if (gtos_menu_pop(state) < 0) w_u8(state + 257u, 1u);
        else gtos_menu_callback(r_u32(gtos_menu_descriptor(state) + 20u));
        motion = 3u;
        break;
    case 3:
        w_u32(state + 188u, descriptor);
        gtos_menu_callback(r_u32(gtos_menu_descriptor(state) + 16u));
        gtos_menu_motion(state, 4u);
        return;
    case 5: motion = 1u; break;
    case 6: motion = 2u; break;
    case 7:
        w_u8(state + 257u, 1u);
        printf("menu escape com.\n");
        return;
    default: return;
    }
    w_u32(state + 188u, descriptor);
    gtos_menu_motion(state, motion);
}

static void gtos_arcade_draw(void)
{
    uint32 state = r_u32(0x80078588u);
    sint32 horizontal = (sint16)r_u16(state + 262u), vertical = (sint16)r_u16(state + 264u);
    sint32 width = (sint16)r_u16(state + 266u), height = (sint16)r_u16(state + 268u);
    if (r_u8(0x8004DC71u))
        gt_draft_unresolved_call(0x800146E4u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    switch ((sint8)r_u8(state + 259u)) {
    case 0: gtos_menu_page_draw(state, 0, 0, 0u); break;
    case 1:
        gtos_menu_page_draw(state, -horizontal, 0, 0u);
        gtos_menu_page_draw(r_u32(0x80078588u), width - horizontal, 0, 1u); break;
    case 2:
        gtos_menu_page_draw(state, horizontal - width, 0, 1u);
        gtos_menu_page_draw(r_u32(0x80078588u), horizontal, 0, 0u); break;
    case 3:
        gtos_menu_page_draw(state, 0, -vertical, 0u);
        gtos_menu_page_draw(r_u32(0x80078588u), 0, height - vertical, 1u); break;
    case 4:
        gtos_menu_page_draw(state, 0, vertical - height, 1u);
        gtos_menu_page_draw(r_u32(0x80078588u), 0, vertical, 0u); break;
    default: break;
    }
}

static uint32 gtos_arcade_loop_prepare(void)
{
    uint32 i;
    w_u8(0x800762A6u, 0u);
    gtos_menu_video_init(1u);
    w_u8(0x8007370Cu, 30u);
    w_u8(0x8007370Du, 10u);
    w_u32(0x8007A144u, 1u);
    for (i = 0u; i < 2u; ++i) w_u32(0x8007A098u + 92u * i, 1u);
    gtos_arcade_font_init();
    {
        uint32 state = 0x80078590u, input, j;
        w_u32(state + 168u, 0u);
        w_u32(state + 172u, 4097u);
        if (!gtos_render_node_insert(state + 160u))
            return gt_draft_unresolved_call(0x8002A3ACu, state + 160u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        for (input = 0u; input < 2u; ++input) {
            uint32 buffer = state + 2u + 74u * input;
            memset(psx_addr(buffer, 8u), 0, 8u);
            w_u16(buffer + 8u, 0u);
            for (j = 0u; j < 16u; ++j) w_u32(buffer + 10u + 4u * j, r_u32(0x8007370Cu));
        }
        w_u8(state + 257u, 0u);
        w_u8(state + 259u, 0u);
        w_u32(0x80078588u, state);
    }
    DrawSync(0);
    PutDispEnv((DISPENV *)psx_addr(0x8007A058u + 20u * r_u32(0x8007A138u), 20u));
    PutDrawEnv((DRAWENV *)psx_addr(0x8007A080u + 92u * r_u32(0x8007A13Cu), 92u));
    for (;;) {
        uint32 buffer = r_u32(0x8007A138u), node = r_u32(0x8007A150u);
        uint32 ot = r_u32(0x8007A154u + 4u * buffer), pool = r_u32(0x8007A030u + 4u * buffer);
        ClearOTagR((uint32 *)psx_addr(ot, 20480u), 5120);
        w_u32(0x80074344u, (r_u32(0x80074330u) & 0xFFFFFFu) | 0x04000000u);
        w_u32(ot, 0x00074344u);
        while (node) {
            w_u32(node + 24u, r_u32(node + 16u + 4u * buffer));
            node = r_u32(node + 4u);
        }
        w_u32(0x8007A038u, buffer);
        w_u32(0x8007A024u, pool);
        w_u32(0x8007A028u, pool + r_u32(0x8007A020u));
        gtos_arcade_update();
        if (r_u8(r_u32(0x80078588u) + 257u)) break;
        gtos_arcade_draw();
        DrawSync(0);
        VSync(0);
        w_u32(0x8007A138u, r_u32(0x8007A13Cu));
        w_u32(0x8007A13Cu, buffer);
        PutDispEnv((DISPENV *)psx_addr(0x8007A058u + 20u * r_u32(0x8007A138u), 20u));
        PutDrawEnv((DRAWENV *)psx_addr(0x8007A080u + 92u * buffer, 92u));
        if (!r_u32(0x8007A140u)) { w_u32(0x8007A140u, 1u); SetDispMask(1); }
        if (r_u8(0x8004DC71u))
            gt_draft_unresolved_call(0x8002BD18u, 0x801A9B7Cu, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        DrawOTag((uint32 *)psx_addr(r_u32(0x8007A154u + 4u * buffer) + 20476u, 4u));
    }
    DrawSync(0);
    return 0xFFFFFFFFu;
}

static uint32 gtos_arcade_enter(void)
{
    printf("ArcMainLoop\n");
    w_u8(0x800787D0u, 0u);
    w_u32(0x800762A0u, 0x8004E33Cu);
    gtos_menu_layout(0x80078590u, 0x8004DD28u);
    gtos_arcade_sound_bank_load();
    gtos_arcade_graphics_load();
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 1u);
    gtos_music_location_init();
    w_u8(0x8004F6A3u, 2u);
    {
        uint8 response[8] = { 0 };
        if (!CdControl(9u, NULL, response))
            return gt_draft_unresolved_call(0x8002BA84u, 1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u8(0x8004F6A6u, 0u);
    }
    gtos_arcade_loop_prepare();
    {
        uint8 response[8] = { 0 };
        if (!CdControl(9u, NULL, response))
            return gt_draft_unresolved_call(0x8002BA84u, 1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        w_u8(0x8004F6A6u, 0u);
    }
    printf("stoped music\n");
    if ((sint8)r_u8(0x800787D0u) == 1) {
        gtos_arcade_race_setup();
        printf("com : race\n");
        return 0u;
    }
    if ((sint8)r_u8(0x800787D0u) == 2) {
        printf("com :ending\n");
        return 1u;
    }
    if (!r_u8(0x800787D0u)) printf("com : exit\n");
    return 0xFFFFFFFFu;
}

static uint32 gtos_replay_restore_player(uint32 destination, uint32 source)
{
    uint32 size = (r_u16(source + 844u) + 854u) & 0xFFFFFFFCu, index;
    memset(psx_addr(destination, 9044u), 0, 9044u);
    if (destination != source)
        for (index = 0u; index < size; ++index) w_u8(destination + index, r_u8(source + index));
    return size;
}

uint32 gtos_replay_restore(uint32 source)
{
    uint32 size = gtos_replay_restore_player(0x80081880u, source);
    if (r_u8(source + 817u) == 2u) {
        gtos_replay_restore_player(0x80083BD4u, source + size);
        return 0x80083BD4u;
    }
    return 2u;
}

uint32 gtos_replay_load_record(uint32 archive, const char *name, uint32 destination)
{
    uint32 directory = archive + 512u, count = r_u8(directory), entry, cursor, size, expected;
    sint32 ordinal = 0, sector;
    for (entry = 0u; entry < count; ++entry) {
        uint32 address = directory + 8u + 52u * entry, index = 0u;
        while (r_u8(address + index) == (uint8)name[index]) {
            if (!name[index]) break;
            ++index;
        }
        if (!r_u8(address + index) && !name[index]) break;
    }
    if (entry == count) {
        printf("file <%s> not found.\n", name);
        return 0u;
    }
    printf("found %u:<%s>\n", entry, name);
    cursor = destination;
    for (;;) {
        uint32 source, offset, words[4], index;
        sector = gtos_replay_sector(archive, entry, ordinal);
        printf("%d ", sector);
        if (sector < 0) break;
        source = directory + 5504u + ((uint32)sector << 7u);
        for (offset = 0u; offset < 128u; offset += 16u) {
            for (index = 0u; index < 4u; ++index) words[index] = r_u32(source + offset + 4u * index);
            for (index = 0u; index < 4u; ++index) w_u32(cursor + offset + 4u * index, words[index]);
        }
        cursor += 128u;
        ++ordinal;
    }
    size = r_u16(directory + 52u * entry + 42u) << 7u;
    expected = r_u32(directory + 52u * entry + 56u);
    if (gtos_replay_checksum(destination, size) != expected) {
        printf("edc error\n");
        return 0u;
    }
    printf("   edc <%u> ok.\n", expected);
    gtos_replay_restore(destination);
    return 1u;
}

uint32 gtos_replay_checksum(uint32 data, uint32 size)
{
    uint32 sum = 0xAAAAu, polynomial = 0x3770u, index, bit;
    for (index = 0u; index < size; ++index) {
        uint32 value = r_u8(data + index);
        sum = (sum + value) ^ (value << 8u);
        for (bit = 0u; bit < 8u; ++bit) {
            polynomial <<= 1u;
            if (polynomial & 65536u) polynomial ^= 0x11021u;
            polynomial |= (value >> 7u) & 1u;
            value <<= 1u;
        }
    }
    return (polynomial & 65535u) | (sum << 16u);
}

sint32 gtos_replay_sector(uint32 archive, uint32 entry, sint32 ordinal)
{
    uint32 directory = archive + 512u;
    sint32 sector = (sint32)r_u16(directory + 52u * entry + 40u), index = 0;
    if (ordinal <= 0) return sector;
    do {
        sector = (sint16)r_u16(directory + 3336u + ((uint32)sector << 1u));
        ++index;
        if (sector == -1) return -1;
    } while (index < ordinal);
    return sector;
}

static uint32 gtos_attract_demo(void)
{
    uint32 archive = 0x8008CF7Cu, size = 0u, demo;
    size_t bytes = 0u;
    char name[32];
    /* Use the shared file adapter for the original fixed-address archive */
    if (!xport_cd_file_size(0x8004B37Cu, "DATA", &size) ||
        size > 0x80200000u - archive ||
        !xport_cd_file_read(0x8004B37Cu, "DATA", archive, size, &bytes) || bytes != size)
        return gt_draft_unresolved_call(0x80029DD4u, 0x8004B37Cu, archive, size, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x80073AC0u, 0u);
    demo = (r_u8(0x8007ABDEu) + 1u) & 255u;
    w_u8(0x8007ABDEu, demo);
    sprintf_s(name, sizeof(name), "Demo %u", demo);
    if (gtos_replay_load_record(archive, name, archive + 0x20000u)) {
        w_u8(0x80081794u, 4u);
        return gtos_module_enter(2u);
    }
    w_u8(0x8007ABDEu, 0u);
    return 4u;
}
static uint32 gtos_menu_main(void)
{
    uint32 selection, resume = r_u8(0x8007ABDFu);
    /* TODO Restore the saved mode-six menu context */
    if (r_u8(0x8007ABDDu) == 6u)
        return gt_draft_unresolved_call(0x80012504u, 1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (resume == 1u) selection = 0u;
    else if (resume == 2u) selection = 1u;
    else {
        if (!gtos_menu_prepare()) {
            return gtos_attract_demo();
        }
        selection = r_u32(0x8004DA14u);
    }
    if (selection == 0u) {
        uint32 arcade_result;
        gtos_menu_hide();
        w_u8(0x8007ABDFu, 1u);
        w_u8(0x80081794u, 1u);
        arcade_result = gtos_arcade_enter();
        if (!arcade_result) return gtos_module_enter(0u);
        if (arcade_result == 1u) return gtos_module_enter(3u);
        return arcade_result;
    }
    if (selection == 1u) {
        gtos_menu_hide();
        w_u8(0x8007ABDFu, 2u);
        return gt_draft_unresolved_call(0x80017B70u, 0x8004E7DCu, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    if (selection == 2u)
        return gtos_module_enter(1u);
    if (selection == 3u) {
        gtos_menu_hide();
        printf("OptionMenu\n");
        return gt_draft_unresolved_call(0x80017B70u, 0x8004E774u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    }
    /* TODO Handle any out-of-range title selection */
    return gt_draft_unresolved_call(0x800117A8u, selection, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static uint32 gtos_native_main(void)
{
    uint32 count, cursor;
    if (!r_u32(0x80076CCCu)) {
        w_u32(0x80076CCCu, 1u);
        count = r_u32(0x801B16B0u);
        cursor = 0x801B16B0u;
        if (count == 0xFFFFFFFFu) {
            count = 0u;
            if (r_u32(cursor + 4u)) {
                cursor += 4u;
                do { cursor += 4u; ++count; } while (r_u32(cursor));
            }
        }
        cursor = 0x801B16B0u + count * 4u;
        while (count) {
            uint32 callback = r_u32(cursor);
            cursor -= 4u;
            --count;
            /* TODO Dispatch any nonempty GTOS constructor list */
            gt_draft_unresolved_call(callback, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
        }
    }
    {
        uint32 start = r_u32(0x80073ABCu), size = r_u32(0x80073AB8u);
        printf("GTOS heap: area = %08X, size = %08X, end = %08X\n", start, size, start + size);
    }
    /* Resume native callbacks disabled by GTMAIN shutdown */
    ResetCallbackPSX();
    if (!CdInit())
        return gt_draft_unresolved_call(0x80042B90u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    gtos_sound_init();
    /* Guest BIOS card driver setup is skipped for the native build */
    memset(psx_addr(0x8007AA30u, 96u), 0, 96u);
    if (!InitPAD(0x8007AA90u, 64u, 0x8007AAD0u, 64u) || !StartPAD())
        return gt_draft_unresolved_call(0x8003A6ACu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    /* TODO Adapt SDK port state processing when GTOS begins polling controls */
    gtos_graph_reset(3u);
    gtos_mdec_reset(0u);
    InitGeom();
    gtos_frame_init(1u);
    gtos_frame_events_init();
    gtos_saved_data_init(0x8007ABA0u);
    w_u32(0x800756D8u, 0x80010154u);
    w_u32(0x800756DCu, 0xFFFFFFFFu);
    gtos_frame_callback_insert(0x800756D0u);
    gtos_sequence_init();
    w_u16(0x80076360u, 60u);
    w_u16(0x80076362u, 256u);
    w_u16(0x80076730u, 60u);
    gtos_reverb_init();
    w_u16(0x1F801DAAu, (r_u16(0x1F801DAAu) & 0xFF7Fu) | 0x80u);
    gtos_audio_settings_init();
    gtos_resources_init();
    gtos_sound_bank_load();
    gtos_music_location_init();
    w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 1u);
    if (!r_u8(0x8007ABDCu)) {
        w_u8(0x8007ABDCu, 1u);
        gtos_intro_prepare();
        gtos_movie_prepare();
    }
    w_u16(0x1F801D84u, 10922u);
    w_u16(0x1F801D86u, 10922u);
    return gtos_menu_main();
}

uint32 gtos_saved_data_store(uint32 state)
{
    sint32 elapsed = (sint32)r_u32(0x8007AA24u);
    sint64 product = (sint64)-2004318071 * elapsed;
    uint32 combined = (uint32)(product >> 32) + (uint32)elapsed;
    sint32 increment = ((sint32)combined >> 5) - (elapsed >> 31);
    uint32 channel, registers;
    w_u32(state, 0x47546F73u);
    w_u32(state + 64u, r_u32(state + 64u) + (uint32)increment);
    w_u32(state + 0xB398u, gtos_replay_checksum(state, 0xB398u));
    for (channel = 0u; channel < 24u; ++channel) {
        w_u16(0x1F801C02u + channel * 16u, 0u);
        w_u16(0x1F801C00u + channel * 16u, 0u);
    }
    w_u32(0x8007412Cu, 0x400u);
    w_u32(0x80074184u, 0u);
    w_u32(0x80074130u, 0u);
    registers = r_u32(0x80074114u);
    w_u16(registers + 422u, 0x400u);
    w_u32(0x80074164u, 0u);
    w_u16(registers + 426u, (r_u16(registers + 426u) & 0xFFCFu) | 0x20u);
    w_u32(0x80074168u, state);
    w_u32(0x8007416Cu, 0x2CFu);
    /* The native upload completes synchronously before the SDK wait returns */
    if (!spu_upload(0x2000u, psx_addr(state, 0xB3C0u), 0xB3C0u))
        return gt_draft_unresolved_call(0x80045440u, state, 0xB3C0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u16(registers + 426u, r_u16(registers + 426u) & 0xFFCFu);
    /* TODO Dispatch a nonzero original SPU transfer callback */
    if (r_u32(0x8007414Cu))
        return gt_draft_unresolved_call(r_u32(0x8007414Cu), 0xF0000000u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x80074148u, 1u);
    return 1u;
}

static sint32 gtos_shutdown(void)
{
    uint32 i;
    /* Native services do not install the guest BIOS card or event patches */
    PadStopCom();
    if (r_u32(0x80073734u)) {
        w_u32(0x80073734u, 0u);
        for (i = 0u; i < 24u; ++i) {
            uint32 voice = 0x80076364u + 40u * i, owner = r_u32(voice + 20u);
            w_u8(voice + 5u, 2u); w_u32(voice + 8u, 0u);
            w_u8(voice + 7u, 1u); w_u8(voice + 6u, 0u);
            w_u8(voice + 4u, 0u); w_u8(voice, 0u);
            if (owner) { w_u8(owner, 255u); w_u32(voice + 20u, 0u); }
        }
        gtos_voice_tick();
        if (r_u8(0x8007635Du)) {
            for (i = 0u; i < 32u; ++i)
                w_u16(0x1F801DC0u + 2u * i, r_u16(0x80073768u + 2u * i));
            w_u16(0x1F801D84u, 0u); w_u16(0x1F801D86u, 0u);
            w_u16(0x1F801DA2u, r_u16(0x80073754u));
            w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) | 128u);
            w_u16(0x1F801DAAu, r_u16(0x1F801DAAu) & ~128u);
        }
        if (r_u32(0x800741E4u) == 1u) {
            w_u32(0x800741E4u, 0u); w_u32(0x8007414Cu, 0u); w_u32(0x80074150u, 0u);
            DMACallback(4u, NULL);
        }
    }
    if (r_u32(0x800736A4u)) { w_u32(0x800736A4u, 0u); VSyncCallback(NULL); }
    return StopCallback();
}

static uint32 gtos_module_enter(uint32 mode)
{
    uint32 start = 0x801B56D0u;
    uint32 end = ((r_u32(0x8004D924u) - r_u32(0x80073B5Cu)) & ~15u) | 0x80000000u;
    uint32 size = end - start, path, executable = 0x8008CF7Cu, file_size = 0u;
    size_t bytes = 0u;
    if ((sint32)size < 0) size = 0u;
    w_u32(0x80073ABCu, start); w_u32(0x80073AB8u, size);
    memset(psx_addr(start, size), 0, size);
    w_u32(start, 0u); w_u32(start + 4u, 0u);
    w_u32(start + 8u, size < 16u ? 0u : size - 16u); w_u32(start + 12u, 0u);
    path = r_u32(0x8004D96Cu + 4u * mode);
    if (!xport_cd_file_size(path, "DATA", &file_size) || file_size < 2048u ||
        file_size > 0x80200000u - executable ||
        !xport_cd_file_read(path, "DATA", executable, file_size, &bytes) || bytes != file_size)
        return gt_draft_unresolved_call(0x80029DD4u, path, executable, 0u, 0u, 0u, 0u, 0u, 0u);
    w_u32(0x80073AC0u, 0u);
    w_u32(0x8007AA24u, 0u);
    gtos_saved_data_store(0x8007ABA0u);
    gtos_shutdown();
    /* Select the native entry from the loaded original executable header */
    if (memcmp(psx_addr(executable, 8u), "PS-X EXE", 8u) ||
        r_u32(executable + 16u) != 0x80063CE4u || r_u32(executable + 24u) != 0x80010000u)
        /* TODO Dispatch the other translated module images */
        return gt_draft_unresolved_call(0x80029FB8u, executable, mode, 0u, 0u, 0u, 0u, 0u, 0u);
    /* PSLZ ends with a0=2 and a1=8000FFFF at the decoded GTMAIN entry */
    gtmain_native_main();
    return 0u;
}

uint32 gtos_native_transition(uint32 executable)
{
    uint32 bound = r_u32(0x8008F1D8u);
    uint32 cursor, start, end, size;
    uint8 *payload;
    FILE *file = NULL;
    size_t received;
    int extra, failed;
    /* Select the translated image from the actual loaded PS-X EXE */
    if (memcmp(psx_addr(executable, 8u), "PS-X EXE", 8u) ||
        r_u32(executable + 16u) != 0x80045E80u || r_u32(executable + 24u) != 0x80010000u)
        return gt_draft_unresolved_call(0x8006A334u, executable, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
    if (fopen_s(&file, "DATA/GTOS.payload.bin", "rb") || !file) goto failed;
    payload = (uint8 *)malloc(415744u);
    if (!payload) { fclose(file); goto failed; }
    received = fread(payload, 1u, 415744u, file);
    extra = fgetc(file);
    failed = ferror(file);
    fclose(file);
    if (received != 415744u || extra != EOF || failed) { free(payload); goto failed; }
    memcpy(psx_addr(0x80010000u, 415744u), payload, 415744u);
    free(payload);
    /* The native entry loads decoded data without executing the PSLZ or BIOS loader */
    w_u32(0x8004D924u, bound);
    w_u32(0x8004D950u, 0x8004B2E0u - 0x80010000u);
    w_u32(0x8004D958u, 0x800756C4u - 0x8004D930u);
    w_u32(0x8004D960u, 0x801B56C4u - 0x800756C4u);
    w_u32(0x8004D964u, 1u);
    w_u32(0x8004D968u, 0u);
    cursor = 0x801B56C4u;
    do { cursor -= 4u; w_u32(cursor, 0u); } while (cursor >= 0x800756C4u);
    start = 0x801B56D0u;
    end = ((bound - r_u32(0x80073B5Cu)) & ~15u) | 0x80000000u;
    size = end - start;
    if ((sint32)size < 0) size = 0u;
    w_u32(0x80073ABCu, start);
    w_u32(0x80073AB8u, size);
    memset(psx_addr(start, size), 0, size);
    w_u32(start, 0u);
    w_u32(start + 4u, 0u);
    w_u32(start + 8u, size < 16u ? 0u : size - 16u);
    w_u32(start + 12u, r_u32(start + 12u) & ~1u);
    w_u8(0x801B16C0u, 0u);
    w_u32(0x80073B60u, 1u);
    return gtos_native_main();
failed:
    return gt_draft_unresolved_call(0x8006A334u, executable, bound, 0u, 0u, 0u, 0u, 0u, 0u);
}
