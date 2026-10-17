#include "game_first2_registry.h"

static sint32 descriptor_add(sint32 a, sint32 b) { return (sint32)((uint32)a + (uint32)b); }
static sint32 descriptor_div8(uint32 value) { sint32 signed_value = (sint32)value; return signed_value / 8; }
static uint32 descriptor_xy(uint32 x, uint32 y) { return (x & 65535u) | (y << 16); }
static void descriptor_byte(uint8 *output, uint32 capacity, uint32 offset, uint32 value)
{
    if (!output || offset >= capacity) gt_draft_failure("Native menu descriptor output extent", 0x8003BA30u);
    output[offset] = (uint8)value;
}
static void descriptor_half(uint8 *output, uint32 capacity, uint32 offset, uint32 value)
{
    descriptor_byte(output,capacity,offset,value); descriptor_byte(output,capacity,offset+1u,value >> 8);
}
static void descriptor_rectangle(uint32 ordering, uint32 first, uint32 second, uint32 color)
{
    sint16 corners[4] = {(sint16)first, (sint16)(first >> 16),
        (sint16)second, (sint16)(second >> 16)};
    (void)gtmain_packet_rectangle_native(ordering, corners, color);
}
static void descriptor_arrow(uint32 owner, uint32 ordering, sint32 x, sint32 y, uint32 color)
{
    /* TODO Bind unselected four-argument arrows 8003B5B8 and 8003B600 */
    (void)gt_draft_unresolved_call(owner, ordering, (uint32)x, (uint32)y, color, 0u, 0u, 0u, 0u);
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
uint32 gtmain_menu_mode_setup(sint32 mode)
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
void gtmain_menu_selector_reset(uint32 guest_selector)
{
    w_u8(guest_selector+1u,0); w_u8(guest_selector+12u,0); w_u8(guest_selector+13u,0);
}
uint32 gtmain_menu_indicator_draw(uint32 ordering,sint32 x,sint32 y,sint32 phase,uint32 flags,uint32 descriptor_index,uint32 style,uint32 page)
{
    uint32 descriptor = 0x800B6670u + descriptor_index*76u;
    uint32 texture = r_u32(descriptor+8u), size = r_u16(descriptor+6u), tpage = r_u16(descriptor+4u);
    return indicator_service(ordering,x,y,phase,texture,size,(tpage+(page << 8))&65535u,6,0x7FD3u,flags,style);
}
/* TODO Resolve independent arrow and legacy published-buffer services */
/* GTMAIN.EXE 8003BA30 */
sint32 gtmain_menu_descriptor_draw(uint32 ordering,uint32 selector,sint32 x,sint32 y,sint32 intensity,uint32 publish,uint8 *output,uint32 capacity)
{
    sint32 count, selected, half, start, end, target, total, stride, spacing, align, muted;
    sint32 first = 0, last = 0, offset = 0, position = (uint16)y;
    uint32 cursor, i = 0;
    gtmain_text_flags(0);
    count = (sint8)r_u8(selector+2u); half = count / 2;
    selected = (sint8)r_u8(selector+1u);
    stride = (sint8)r_u8(selector+9u); stride = descriptor_add(stride,(sint8)r_u8(selector+10u));
    align = r_u8(selector+8u); muted = align & 32; align &= 31;
    spacing = (sint8)r_u8(selector+3u);
    start = selected-half; if (start < 0) start=0;
    total = (sint8)r_u8(selector);
    if (total < start+count) start = total-count;
    end = start+count;
    target = (sint8)r_u8(selector+12u)-half; if (target<0) target=0;
    if (total < target+count) target=total-count;
    if (start != target) { w_u8(selector+13u,target < start ? 3 : 4); w_u8(selector+14u,8); }
    if ((sint8)r_u8(selector+13u)) {
        sint32 remaining=(sint8)(uint8)(r_u8(selector+14u)-1u);
        w_u8(selector+14u,(uint32)remaining);
        if (remaining>0) {
            sint32 direction=(sint8)r_u8(selector+13u);
            if (direction==3) {
                --start; offset=descriptor_div8((uint32)stride*(uint32)(8-remaining));
                position=descriptor_add(position,-offset); first=start; last=end-1;
            } else if (direction==4) {
                ++end; offset=descriptor_div8((uint32)stride*(uint32)remaining);
                position=descriptor_add(position,-offset); last=start; first=end-1;
            }
            offset=descriptor_add(offset,-stride);
        } else { w_u8(selector+13u,0); w_u8(selector+14u,0); }
    }
    target=start;
    cursor=r_u32(selector+4u)+(uint32)start*8u;
    while (start<end) {
        sint32 brightness=intensity, draw_x=x;
        uint32 type, resource;
        if (publish) descriptor_byte(output,capacity,2u+i,(uint32)start);
        if (r_u8(cursor)&64u) brightness >>= 1;
        if ((sint8)r_u8(selector+13u)) {
            if (start==first) brightness=descriptor_div8((uint32)brightness*(uint32)(sint32)(sint8)r_u8(selector+14u));
            else if (start==last) brightness=descriptor_add(brightness,-descriptor_div8((uint32)brightness*(uint32)(sint32)(sint8)r_u8(selector+14u)));
        }
        type=r_u8(cursor)&63u;
        if (type<2u || (type>=4u && (type>=12u || type<10u))) {
            uint32 color=(start==selected && !muted) ? gtmain_menu_pulse_color(brightness >> 1) : gtmain_menu_gray((uint8)(brightness >> 1));
            (void)gtmain_text_color(color);
        } else if (start==selected && !muted) brightness=gtmain_menu_pulse_component(0,brightness);
        type=r_u8(cursor)&63u;
        if (type==8u || type==9u || type==10u || type==11u) {
            if (type==9u || type==11u) position=descriptor_add(position,spacing);
        } else if (type==2u || type==3u || type==6u || type==7u) {
            if (type==6u || type==7u) position=descriptor_add(position,spacing);
            resource=r_u32(cursor+4u);
            if (type==3u || type==7u) {
                if (!align || align==2) {
                    uint32 width=r_u16(resource+8u), scale=r_u8(cursor+1u);
                    draw_x=(sint32)((uint32)x-((width*scale) >> (align ? 7 : 8)));
                }
                {
                    uint8 scale = r_u8(cursor+1u);
                    uint32 descriptor = r_u32(cursor+4u);
                    descriptor_scaled_sprite(descriptor,ordering+4u,(sint16)draw_x,
                        (sint16)position,brightness >> 1,scale);
                }
            } else {
                if (!align || align==2) {
                    uint32 width=r_u16(resource+8u); if (!align) width >>= 1;
                    draw_x=(sint32)((uint32)x-width);
                }
                (void)gtmain_menu_sprite(resource,ordering+4u,(uint16)draw_x,(sint16)position,(uint8)(brightness >> 1));
            }
        } else {
            sint32 font=3;
            if (type==4u || type==5u || type==12u || type==13u) position=descriptor_add(position,spacing);
            resource=r_u32(cursor+4u);
            if (type==12u || type==13u || type==14u) {
                sint32 width=type==14u ? gtmain_text_width(2,resource) : 320;
                uint32 a=descriptor_xy((uint32)(sint16)x,(uint32)position+17u);
                uint32 b=descriptor_xy((uint32)(sint16)x+(uint32)(sint16)width-1u,(uint32)position+17u);
                uint32 color=gtmain_menu_palette_color(r_u8(cursor+1u),brightness);
                descriptor_rectangle(ordering+4u,a,b,color|0x02000000u);
                a=descriptor_xy((uint32)(sint16)x-20u,(uint32)position+3u); b=descriptor_xy((uint32)(sint16)x-10u,(uint32)position+13u);
                color=gtmain_menu_palette_color(r_u8(cursor+1u),brightness);
                descriptor_rectangle(ordering+4u,a,b,color); font=2;
            } else if (type==1u || type==5u) font=2;
            (void)gtmain_text_font(font);
            if (!align || align==2) {
                sint32 width=gtmain_text_width(font,resource); if (!align) width >>= 1;
                draw_x=(sint32)((uint32)x-(uint32)width);
            }
            (void)gtmain_font_text_draw(ordering+4u,(sint16)draw_x,(sint16)position,resource);
        }
        if (publish) {
            descriptor_byte(output,capacity,26u+i,(uint32)(brightness >> 1));
            descriptor_half(output,capacity,50u+2u*i,(uint32)draw_x);
            descriptor_half(output,capacity,98u+2u*i,(uint32)position);
        }
        ++start; position=descriptor_add(position,stride); cursor+=8u; ++i;
    }
    if ((sint8)r_u8(0x80091B41u)) {
        uint32 color=gtmain_menu_palette_color(0,intensity);
        if (target) descriptor_arrow(0x8003B5B8u,ordering,x,descriptor_add(y,-8),color);
        if (end!=(sint8)r_u8(selector)) descriptor_arrow(0x8003B600u,ordering,x,
            descriptor_add((sint16)position,descriptor_add(offset,9)),color);
        (void)gtmain_packet_page(ordering,544u);
    }
    if (publish) { descriptor_half(output,capacity,0,(uint32)offset); descriptor_byte(output,capacity,2u+i,255); }
    return -1;
}
