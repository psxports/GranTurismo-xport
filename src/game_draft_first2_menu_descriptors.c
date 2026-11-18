#include "game_first2_registry.h"
#include "game_local_stack.h"
#include "xport_trace.h"

static sint32 descriptor_add(sint32 a, sint32 b) { return (sint32)((uint32)a + (uint32)b); }
static sint32 descriptor_div8(uint32 value) { sint32 signed_value = (sint32)value; return signed_value / 8; }
static void descriptor_byte(uint8 *output, uint32 capacity, uint32 offset, uint32 value)
{
    if (!output || offset >= capacity) gt_draft_failure("Native menu descriptor output extent", 0x8003BA30u);
    output[offset] = (uint8)value;
}
static void descriptor_half(uint8 *output, uint32 capacity, uint32 offset, uint32 value)
{
    descriptor_byte(output,capacity,offset,value); descriptor_byte(output,capacity,offset+1u,value >> 8);
}
static void descriptor_scaled_sprite(uint32 descriptor, uint32 ordering, sint16 x, sint16 y, sint32 intensity, uint8 scale)
{
    /* Preserve the original byte intensity and full scale argument */
    (void)gtmain_menu_scaled_sprite(descriptor,ordering,x,y,(uint8)intensity,(sint32)scale);
}
static uint32 indicator_service(uint32 ordering,sint32 x,sint32 y,sint32 phase,uint32 texture,uint32 size,uint32 page,uint32 vertices,uint32 angle,uint32 flags,uint32 style)
{
    return gtmain_menu_model_draw(ordering,(sint16)x,(sint16)y,(sint16)phase,texture,(uint16)size,(uint16)page,vertices,(uint16)angle,(sint32)flags,style);
}
/* Original: GTMAIN.EXE:sub_800320E8. */ uint32 gtmain_menu_mode_setup(sint32 mode)
{
    uint32 selection = 18;
    switch (mode) {
    case 0: case 1: case 5: case 6: case 7: case 8:
        if (!r_u8(0x801D2FEDu)) selection = 16;
        break;
    case 3: case 4:
        if ((sint16)r_u16(0x801D2FEEu) < 0) selection = 16;
        break;
    default: break;
    }
    return gtmain_menu_asset_activate(selection);
}
/* Original: GTMAIN.EXE:sub_8003B87C. */ void gtmain_menu_selector_reset(uint32 guest_selector)
{
    w_u8(guest_selector+1u,0); w_u8(guest_selector+12u,0); w_u8(guest_selector+13u,0);
}
/* Original: GTMAIN.EXE:sub_8003C28C. */ uint32 gtmain_menu_indicator_draw(uint32 ordering,sint32 x,sint32 y,sint32 phase,uint32 flags,uint32 descriptor_index,uint32 style,uint32 page)
{
    uint32 descriptor = 0x800B6670u + descriptor_index*76u;
    uint32 texture = r_u32(descriptor+8u), size = r_u16(descriptor+6u), tpage = r_u16(descriptor+4u);
    return indicator_service(ordering,x,y,phase,texture,size,(tpage+(page << 8))&65535u,6,0x7FD3u,flags,style);
}
/* Published output uses the existing bounded native buffer contract */
/* GTMAIN.EXE 8003BA30 */
/* Original: GTMAIN.EXE:sub_8003BA30. */ sint32 gtmain_menu_descriptor_draw(uint32 ordering,uint32 selector,sint32 x,sint32 y,sint32 intensity,uint32 publish,uint8 *output,uint32 capacity)
{
    sint32 count, selected, half, start, end, target, total, stride, spacing, align, muted;
    sint32 first = 0, last = 0, offset = 0, position = (uint16)y;
    uint32 cursor, i = 0, staged, corners, first_word, second_word;
    GT_LOCAL_STACK_FRAME frame;
    FUNCTION_MARKER(0x8003BA30, "GTMAIN.EXE");
    frame = gt_local_stack_push(0xD0, 0x8003BA30);
    staged = frame.address + 0x20;
    corners = frame.address + 0x18;
    gtmain_text_font(0);
    count = r_s8(selector+2); half = count / 2;
    selected = r_s8(selector+1);
    stride = r_s8(selector+9); stride = descriptor_add(stride,r_s8(selector+10));
    align = r_u8(selector+8); muted = align & 32; align &= 31;
    spacing = r_s8(selector+3);
    start = selected-half; if (start < 0) start=0;
    total = r_s8(selector);
    if (total < start+count) start = total-count;
    end = start+count;
    target = r_s8(selector+12)-half; if (target<0) target=0;
    if (total < target+count) target=total-count;
    if (start != target) { w_u8(selector+13,target < start ? 3 : 4); w_u8(selector+14,8); }
    if (r_s8(selector+13)) {
        sint32 remaining=(sint8)(uint8)(r_u8(selector+14)-1);
        w_u8(selector+14,(uint32)remaining);
        if (remaining>0) {
            sint32 direction=r_s8(selector+13);
            if (direction==3) {
                --start; offset=descriptor_div8((uint32)stride*(uint32)(8-remaining));
                position=descriptor_add(position,-offset); first=start; last=end-1;
            } else if (direction==4) {
                ++end; offset=descriptor_div8((uint32)stride*(uint32)remaining);
                position=descriptor_add(position,-offset); last=start; first=end-1;
            }
            offset=descriptor_add(offset,-stride);
        } else { w_u8(selector+13,0); w_u8(selector+14,0); }
    }
    target=start;
    cursor=r_u32(selector+4)+(uint32)start*8;
    while (start<end) {
        sint32 brightness=intensity, draw_x=x;
        uint32 type, resource;
        if (publish) descriptor_byte(output,capacity,2+i,(uint32)start);
        if (r_u8(cursor)&64) brightness >>= 1;
        if (r_s8(selector+13)) {
            if (start==first) brightness=descriptor_div8((uint32)brightness*(uint32)(sint32)r_s8(selector+14));
            else if (start==last) brightness=descriptor_add(brightness,-descriptor_div8((uint32)brightness*(uint32)(sint32)r_s8(selector+14)));
        }
        type=r_u8(cursor)&63;
        if (type<2 || (type>=4 && (type>=12 || type<10))) {
            uint32 color=(start==selected && !muted) ? gtmain_menu_pulse_color(brightness >> 1) : gtmain_menu_gray((uint8)(brightness >> 1));
            (void)gtmain_text_color(color);
        } else if (start==selected && !muted) brightness=gtmain_menu_pulse_component(0,brightness);
        type=r_u8(cursor)&63;
        if (type==8 || type==9 || type==10 || type==11) {
            if (type==9 || type==11) position=descriptor_add(position,spacing);
        } else if (type==2 || type==3 || type==6 || type==7) {
            if (type==6 || type==7) position=descriptor_add(position,spacing);
            resource=r_u32(cursor+4);
            if (type==3 || type==7) {
                if (!align || align==2) {
                    uint32 width=r_u16(resource+8), scale=r_u8(cursor+1);
                    draw_x=(sint32)((uint32)x-((width*scale) >> (align ? 7 : 8)));
                }
                {
                    uint8 scale = r_u8(cursor+1);
                    uint32 descriptor = r_u32(cursor+4);
                    descriptor_scaled_sprite(descriptor,ordering+4,(sint16)draw_x,
                        (sint16)position,brightness >> 1,scale);
                }
            } else {
                if (!align || align==2) {
                    uint32 width=r_u16(resource+8); if (!align) width >>= 1;
                    draw_x=(sint32)((uint32)x-width);
                }
                (void)gtmain_menu_sprite(resource,ordering+4,(uint16)draw_x,(sint16)position,(uint8)(brightness >> 1));
            }
        } else {
            sint32 font=3;
            if (type==4 || type==5 || type==13) position=descriptor_add(position,spacing);
            resource=r_u32(cursor+4);
            if (type==12 || type==13 || type==14) {
                sint32 width=type==14 ? gtmain_text_width(2,resource) : 320;
                uint32 color;
                w_u16(staged, x);
                w_u16(staged + 2, (uint32)position + 17);
                w_u16(staged + 4, (uint32)x + (uint32)width - 1);
                w_u16(staged + 6, (uint32)position + 17);
                first_word = r_u32(staged);
                second_word = r_u32(staged + 4);
                w_u32(corners, first_word);
                w_u32(corners + 4, second_word);
                color = gtmain_menu_palette_color(r_u8(cursor + 1), brightness);
                gtmain_packet_rectangle(ordering + 4, corners, color | 0x02000000);
                w_u16(staged, (uint32)x - 20);
                w_u16(staged + 2, (uint32)position + 3);
                w_u16(staged + 4, (uint32)x - 10);
                w_u16(staged + 6, (uint32)position + 13);
                first_word = r_u32(staged);
                second_word = r_u32(staged + 4);
                w_u32(corners, first_word);
                w_u32(corners + 4, second_word);
                color = gtmain_menu_palette_color(r_u8(cursor + 1), brightness);
                gtmain_packet_rectangle(ordering + 4, corners, color);
                font = 2;
            } else if (type==1 || type==5) font=2;
            gtmain_text_flags((uint8)font);
            if (!align || align==2) {
                sint32 width=gtmain_text_width(font,resource); if (!align) width >>= 1;
                draw_x=(sint32)((uint32)x-(uint32)width);
            }
            (void)gtmain_font_text_draw(ordering+4,(sint16)draw_x,(sint16)position,resource);
        }
        if (publish) {
            descriptor_byte(output,capacity,26+i,(uint32)(brightness >> 1));
            descriptor_half(output,capacity,50+2*i,(uint32)draw_x);
            descriptor_half(output,capacity,98+2*i,(uint32)position);
        }
        ++start; position=descriptor_add(position,stride); cursor+=8; ++i;
    }
    if (r_s8(0x80091B41)) {
        uint32 color=gtmain_menu_palette_color(0,intensity);
        if (target) gtmain_packet_triangle_up(ordering,x,descriptor_add(y,-8),color);
        if (end!=r_s8(selector)) gtmain_packet_triangle_down(ordering,x,
            descriptor_add((sint16)position,descriptor_add(offset,9)),color);
        (void)gtmain_packet_page(ordering,544);
    }
    if (publish) { descriptor_half(output,capacity,0,(uint32)offset); descriptor_byte(output,capacity,2+i,255); }
    gt_local_stack_pop(frame);
    return -1;
}
