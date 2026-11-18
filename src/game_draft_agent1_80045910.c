#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

uint32 gt_draft_80045910(void)
{
    uint32 input, context, category, part, offset, summary, detail, snapshot;
    uint32 first, second, third, fourth, result = 0;
    uint32 source, destination, word, lane, base, count, shift, index, words[4];
    sint32 pending, selected, limit;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x80045910, "GTMAIN.EXE");
    frame = gt_local_stack_push(0x90, 0x80045910);
    snapshot = frame.address + 0x10;
    input = r_u32(0x800BDD08);
    gtmain_menu_stream_check();
    pending = r_s16(0x801D2FDC);
    if (pending >= 0) {
        if (r_s8(0x801D2FE0 + (uint32)pending)) {
            context = r_u32(0x800BEA64);
            for (offset = 0; offset < 96; offset += 16) {
                source = context + offset;
                destination = snapshot + offset;
                if ((source | destination) & 3) {
                    for (index = 0; index < 4; ++index) {
                        word = source + index * 4;
                        base = (word + 3) & ~3;
                        count = ((word + 3) & 3) + 1;
                        shift = (4 - count) * 8;
                        words[index] = 0;
                        for (lane = 0; lane < count; ++lane)
                            words[index] |= (uint32)r_u8(base + lane) << (shift + lane * 8);
                        base = word & ~3;
                        count = word & 3;
                        for (lane = count; lane < 4; ++lane) {
                            shift = (lane - count) * 8;
                            words[index] = (words[index] & ~((uint32)255 << shift)) |
                                ((uint32)r_u8(base + lane) << shift);
                        }
                    }
                    for (index = 0; index < 4; ++index) {
                        word = destination + index * 4;
                        base = (word + 3) & ~3;
                        count = ((word + 3) & 3) + 1;
                        shift = (4 - count) * 8;
                        for (lane = 0; lane < count; ++lane)
                            w_u8(base + lane, words[index] >> (shift + lane * 8));
                        base = word & ~3;
                        count = word & 3;
                        for (lane = count; lane < 4; ++lane)
                            w_u8(base + lane, words[index] >> ((lane - count) * 8));
                    }
                } else {
                    first = r_u32(source);
                    second = r_u32(source + 4);
                    third = r_u32(source + 8);
                    fourth = r_u32(source + 12);
                    w_u32(destination, first);
                    w_u32(destination + 4, second);
                    w_u32(destination + 8, third);
                    w_u32(destination + 12, fourth);
                }
            }
            category = r_u8(0x800B5374);
            part = r_u16(0x800B5314 + 8 * (uint32)pending);
            gt_draft_8004AB30((sint32)category, (uint16)part);
            category = r_u8(0x800B5374);
            gtmain_menu_parts_parameters_apply(category);
            gtmain_player_configuration_build();
            summary = 0x801D33F8 + 60 * (uint32)pending;
            detail = 0x801D3098 + 108 * (uint32)pending;
            gtmain_race_specification_summary(0x800BB850, summary, detail);
            gtmain_menu_selection_peak_update(0x801D2FE8, summary, detail);
            context = r_u32(0x800BEA64);
            for (offset = 0; offset < 96; offset += 16) {
                source = snapshot + offset;
                destination = context + offset;
                if ((source | destination) & 3) {
                    for (index = 0; index < 4; ++index) {
                        word = source + index * 4;
                        base = (word + 3) & ~3;
                        count = ((word + 3) & 3) + 1;
                        shift = (4 - count) * 8;
                        words[index] = 0;
                        for (lane = 0; lane < count; ++lane)
                            words[index] |= (uint32)r_u8(base + lane) << (shift + lane * 8);
                        base = word & ~3;
                        count = word & 3;
                        for (lane = count; lane < 4; ++lane) {
                            shift = (lane - count) * 8;
                            words[index] = (words[index] & ~((uint32)255 << shift)) |
                                ((uint32)r_u8(base + lane) << shift);
                        }
                    }
                    for (index = 0; index < 4; ++index) {
                        word = destination + index * 4;
                        base = (word + 3) & ~3;
                        count = ((word + 3) & 3) + 1;
                        shift = (4 - count) * 8;
                        for (lane = 0; lane < count; ++lane)
                            w_u8(base + lane, words[index] >> (shift + lane * 8));
                        base = word & ~3;
                        count = word & 3;
                        for (lane = count; lane < 4; ++lane)
                            w_u8(base + lane, words[index] >> ((lane - count) * 8));
                    }
                } else {
                    first = r_u32(source);
                    second = r_u32(source + 4);
                    third = r_u32(source + 8);
                    fourth = r_u32(source + 12);
                    w_u32(destination, first);
                    w_u32(destination + 4, second);
                    w_u32(destination + 8, third);
                    w_u32(destination + 12, fourth);
                }
            }
            gtmain_player_configuration_build();
        }
        limit = r_s16(0x801D2FDE);
        ++pending;
        if (pending >= limit) pending = -1;
        w_u16(0x801D2FDC, pending);
    }
    if (r_s8(input)) {
        gtmain_menu_flags(1);
        selected = gtmain_menu_selector_input(0x80093530, input + 2);
        if (selected == -2) {
            gtmain_menu_sound(2);
            result = 1;
        } else if (selected != -1) {
            category = r_u8(0x800B5374);
            part = r_u16(0x800B5314 + 8 * (uint32)selected);
            gt_draft_8004AB30((sint32)category, (uint16)part);
            category = r_u8(0x800B5374);
            gtmain_menu_parts_parameters_apply(category);
            gtmain_player_configuration_build();
            gtmain_menu_sound(4);
            result = 1;
        }
    }
    gt_local_stack_pop(frame);
    return result;
}
