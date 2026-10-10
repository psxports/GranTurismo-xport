#include "game_first2_registry.h"
#include "game_draft_registry.h"

uint32 gtmain_course_headers_relocate(uint32 course)
{
    uint32 cursor = course + 404u, i;
    for (i = 0; i < 33; ++i) {
        uint32 count;
        w_u32(course + 272u + i * 4u, cursor);
        count = r_u32(cursor);
        cursor += count * 28u + 4u;
    }
    return cursor;
}
uint32 gtmain_model_resource_end(uint32 model)
{
    return gt_draft_8002F8B4(model, 0, 0, 0, 0, 0, 0, 0);
}
uint32 gtmain_model_resource_relocate(uint32 guest_model)
{
    uint32 count = r_u16(guest_model + 44u), cursor = guest_model + 88u, i;
    w_u32(guest_model, cursor); cursor += count * 8u;
    for (i = 0; i < 8; ++i) {
        uint32 length, stride;
        w_u32(guest_model + 4u + i * 4u, cursor);
        length = r_u16(guest_model + 48u + i * 2u); stride = r_u32(0x80091170u + i * 4u);
        cursor += length * stride;
    }
    count = r_u16(guest_model + 64u); w_u32(guest_model + 36u, cursor);
    { uint32 tail_count = r_u16(guest_model + 66u); cursor += count * 28u; w_u32(guest_model + 40u, cursor); return cursor + tail_count * 20u; }
}
uint32 gtmain_course_models_relocate(uint32 guest_models)
{
    uint32 count = r_u32(guest_models), index, cursor = guest_models + count * 4u + 4u;
    for (index = 0; index < count; ++index) {
        w_u32(guest_models + 4u + index * 4u, cursor);
        cursor = gtmain_model_resource_relocate(cursor);
        cursor = gtmain_model_resource_end(cursor);
    }
    return cursor;
}
uint32 gtmain_course_resource_bind(uint32 guest_course)
{
    uint32 header, groups, records, tail, result;
    w_u32(0x800BC1B0u, guest_course);
    header = gtmain_course_headers_relocate(guest_course); w_u32(guest_course + 16u, header);
    header = r_u32(guest_course + 16u); w_u32(0x800BC1A8u, header);
    groups = gtmain_course_segments_relocate(header); w_u32(guest_course + 20u, groups);
    records = gtmain_distance_groups_relocate(groups);
    tail = gtmain_course_models_relocate(records); w_u32(guest_course + 24u, tail);
    gtmain_distance_groups_bind(groups, records);
    result = gtmain_object_records_relocate(tail); w_u32(0x800BC1ACu, result);
    return result;
}
