#include "game_first2_registry.h"
#include "game_draft_registry.h"

static void name_log_string(uint32 format, uint32 text)
{
    /* TODO Bind guest format and guest string diagnostic output */
    gt_draft_failure("Name commit string log adapter", 0x8008B7D8u);
}
static void name_log_record(uint32 format, sint32 index, uint32 source, uint32 destination)
{
    /* TODO Bind signed index and two guest strings to diagnostic output */
    gt_draft_failure("Name commit record log adapter", 0x8008B7D8u);
}
static void name_log_message(uint32 format)
{
    /* TODO Bind diagnostic output without fabricated arguments */
    gt_draft_failure("Name commit message log adapter", 0x8008B7D8u);
}

/* GTMAIN.EXE:800336D8 */
sint32 gtmain_menu_name_commit_tick(void)
{
    uint32 input = r_u32(0x800BDD08u), copy_default = 0, descriptor, record;
    sint32 selected, index;
    gtmain_menu_stream_check();
    if (!r_s8(input)) return 0;
    selected = gtmain_menu_name_edit(0x8009196Cu, input + 2u);
    if (selected != 0) {
        if (selected == -2) (void)gtmain_menu_sound(6);
        return 0;
    }
    switch (r_s8(0x801D2FECu)) {
    case 0:
        if (r_u8(0x801D2FEDu) & 1u) {
            (void)gtmain_string_copy(r_u32(0x801D2FF0u), 0x801D300Cu);
            name_log_string(0x8008CA68u, r_u32(0x801D2FF0u));
        }
        if (r_u8(0x801D2FEDu) & 2u) {
            (void)gtmain_string_copy(r_u32(0x801D2FF4u), 0x801D300Cu);
            name_log_string(0x8008CA78u, r_u32(0x801D2FF4u));
        }
        break;
    case 1: case 6:
        if (r_u8(0x801D2FEDu) & 1u) {
            (void)gtmain_string_copy(r_u32(0x801D2FF0u), 0x801D300Cu);
            name_log_string(0x8008CA88u, r_u32(0x801D2FF0u));
        }
        index = r_s16(0x801D2FEEu);
        if (index >= 0) {
            (void)gtmain_string_copy(r_u32(0x801D2FF4u) + 20u * (uint32)index, 0x801D300Cu);
            index = r_s16(0x801D2FEEu);
            record = r_u32(0x801D2FF4u) + 20u * (uint32)index;
            name_log_string(0x8008CA78u, record);
        }
        break;
    case 3: case 4:
        index = r_s16(0x801D2FEEu);
        if (index >= 0) {
            (void)gtmain_string_copy(r_u32(0x801D2FF0u) + 28u * (uint32)index, 0x801D300Cu);
            index = r_s16(0x801D2FEEu);
            record = r_u32(0x801D2FF0u) + 28u * (uint32)index;
            name_log_record(0x8008CA98u, index, 0x801D300Cu, record);
        }
        copy_default = 1;
        break;
    case 5: case 7: case 8:
        if (r_u8(0x801D2FEDu) & 1u) {
            (void)gtmain_string_copy(r_u32(0x801D2FF0u), 0x801D300Cu);
            name_log_string(0x8008CA68u, r_u32(0x801D2FF0u));
        }
        if (r_u8(0x801D2FEDu) & 2u) {
            (void)gtmain_string_copy(r_u32(0x801D2FF4u), 0x801D300Cu);
            name_log_string(0x8008CA78u, r_u32(0x801D2FF4u));
        }
        copy_default = 1;
        break;
    default:
        name_log_message(0x8008CAB0u);
        break;
    }
    if (copy_default) (void)gtmain_string_copy(0x800CFF88u, 0x801D300Cu);
    (void)gtmain_menu_sound(4);
    if (r_s8(0x801D2FEBu)) {
        (void)gt_draft_8003CC24(0x801D310Cu, 1, 0x80092170u, 17, 1);
        descriptor = 0x80092248u;
    } else descriptor = 0x80092170u;
    (void)gtmain_menu_push(descriptor);
    return 3;
}
