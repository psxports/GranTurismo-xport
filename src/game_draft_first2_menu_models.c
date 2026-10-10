#include "game_first2_registry.h"

static uint32 menu_model_gte_read(uint32 index)
{
    /* TODO Bind the real GTE data register service */
    (void)index; gt_draft_failure("Menu model GTE data read",0x8003D354u);
}
static void menu_model_gte_write(uint32 index,uint32 value)
{
    /* TODO Bind the real GTE data register service */
    (void)index; (void)value; gt_draft_failure("Menu model GTE data write",0x8003D354u);
}
static void menu_model_gte_control(uint32 index,uint32 value)
{
    /* TODO Bind the real GTE control register service */
    (void)index; (void)value; gt_draft_failure("Menu model GTE control write",0x8003D354u);
}
static void menu_model_gte_command(uint32 command)
{
    /* TODO Bind the original GTE command */
    (void)command; gt_draft_failure("Menu model GTE command",0x8003D354u);
}
static void menu_model_view_prepare(uint8 view[120])
{
    /* TODO Bind native view provenance to the actual projection owner */
    (void)view; gt_draft_failure("Menu model native projection view",0x8006B3E0u);
}
static void menu_model_orientation(uint8 view[120],uint32 reference)
{
    /* TODO Bind native view provenance to the actual orientation owner */
    (void)view; (void)reference; gt_draft_failure("Menu model native orientation view",0x8006B40Cu);
}
static void menu_model_view_copy(uint32 destination,const uint8 view[120])
{
    /* TODO Bind native view source to actual render view copy */
    (void)destination; (void)view; gt_draft_failure("Menu model native view copy",0x80070448u);
}
static void menu_model_vector_init(uint8 vector[12])
{
    /* TODO Bind native initialization without inventing initial field values */
    (void)vector; gt_draft_failure("Menu model native vector initialization",0x80071268u);
}
static void menu_model_half(uint8 *out,uint32 offset,uint32 value) { out[offset]=(uint8)value; out[offset+1u]=(uint8)(value >> 8); }
static void menu_model_word(uint8 *out,uint32 offset,uint32 value)
{
    out[offset]=(uint8)value; out[offset+1u]=(uint8)(value >> 8); out[offset+2u]=(uint8)(value >> 16); out[offset+3u]=(uint8)(value >> 24);
}
static uint32 menu_model_read_word(const uint8 *in,uint32 offset)
{
    return in[offset] | ((uint32)in[offset+1u] << 8) | ((uint32)in[offset+2u] << 16) | ((uint32)in[offset+3u] << 24);
}
uint32 gtmain_menu_asset_activate(uint32 index)
{
    sint32 entry;
    if ((sint8)r_u8(0x800B52FAu)) gt_draft_unresolved_call(0x8003A2DCu,0,0,0,0,0,0,0,0);
    w_u8(0x800B52FAu,1);
    entry=gtmain_stream_entry_find(index);
    return (uint32)gtmain_stream_entry_prepare((uint32)entry,1);
}
uint32 gtmain_menu_scaled_sprite(uint32 descriptor,uint32 ordering,sint16 x,sint16 y,uint8 intensity,sint32 scale)
{
    uint32 width=r_u16(descriptor+8u), uv=r_u32(descriptor+4u), height=r_u16(descriptor+10u);
    uint32 packet=r_u32(0x800BFE4Cu), product=(uint32)scale*width, product_y=(uint32)scale*height;
    uint32 u,v,result,previous;
    w_u32(packet,0x09000000u); w_u32(packet+4u,intensity | ((uint32)intensity << 8) | ((uint32)intensity << 16) | 0x2C000000u);
    w_u16(packet+8u,(uint16)x); w_u16(packet+10u,(uint16)y); w_u16(packet+18u,(uint16)y); w_u16(packet+24u,(uint16)x);
    w_u32(packet+12u,uv);
    w_u16(packet+16u,(uint32)(sint32)x+(uint32)((sint32)product >> 7)); w_u16(packet+32u,(uint32)(sint32)x+(uint32)((sint32)product >> 7));
    u=uv&255u; v=(uv >> 8)&255u;
    w_u16(packet+26u,(uint32)(sint32)y+(uint32)((sint32)product_y >> 7)); w_u16(packet+34u,(uint32)(sint32)y+(uint32)((sint32)product_y >> 7));
    width=r_u16(descriptor+8u); height=(uint32)(sint32)(sint16)r_u16(descriptor);
    w_u32(packet+20u,((u+width-1u)&255u) | (v << 8) | (height << 16));
    height=r_u16(descriptor+10u); w_u32(packet+28u,u | (((v+height-1u)&255u) << 8));
    width=r_u16(descriptor+8u); height=r_u16(descriptor+10u);
    w_u32(packet+36u,((u+width-1u)&255u) | (((v+height-1u)&255u) << 8));
    result=ordering+2u;
    previous=r_u8(ordering); previous|=r_u8(ordering+1u) << 8; previous|=r_u8(ordering+2u) << 16;
    result=(result&255u) | (previous << 8);
    w_u8(ordering,packet); w_u8(ordering+1u,packet >> 8); w_u8(ordering+2u,packet >> 16);
    w_u8(packet,previous); w_u8(packet+1u,previous >> 8); w_u8(packet+2u,previous >> 16);
    w_u32(0x800BFE4Cu,packet+40u);
    return result;
}
uint32 gtmain_menu_model_draw(uint32 ordering,sint16 x,sint16 y,sint16 phase,uint32 model,uint16 depth,uint16 palette,uint32 command,uint16 page,sint32 projection,uint32 flags)
{
    uint8 view[120], vector[12], matrix[20], temporary[24];
    uint32 difference,result;
    menu_model_half(view,0,0); menu_model_half(view,2,0); menu_model_half(view,4,320); menu_model_half(view,6,240);
    menu_model_half(view,8,(uint16)x); menu_model_half(view,10,(uint16)y); menu_model_half(view,14,64); menu_model_half(view,16,64); menu_model_half(view,12,(uint32)projection);
    menu_model_view_prepare(view); menu_model_view_prepare(view);
    menu_model_vector_init(vector);
    (void)gtmain_model_rotation(matrix,(sint16)((uint32)(sint32)phase+2048u),(sint16)r_u16(0x80092264u),0);
    menu_model_word(vector,0,0); menu_model_word(vector,4,0xCCCCu); menu_model_word(vector,8,0xFFF40000u);
    (void)gtmain_model_rotate_vector(vector,matrix,vector);
    menu_model_orientation(view,0x80099490u); menu_model_view_copy(0x1F800000u,view);
    (void)gtmain_render_fog_set(0x1F800000u,0,256,0);
    menu_model_gte_control(26,r_u16(0x1F800020u));
    menu_model_gte_control(24,r_u32(0x1F800018u)); menu_model_gte_control(25,r_u32(0x1F80001Cu));
    w_u32(0x1F800394u,page | ((command|32u) << 16)); w_u16(0x1F800398u,64); w_u16(0x1F80039Au,64); w_u16(0x1F80039Cu,0);
    w_u32(0x1F800388u,(uint32)palette << 16); w_u32(0x1F800390u,0x10000000u); w_u16(0x1F80038Eu,6);
    (void)gtmain_render_context_init(0x1F800360u);
    difference=((uint32)(sint32)(sint16)r_u16(model+50u)-(uint32)(sint32)(sint16)r_u16(model+18u)) << 4;
    w_u16(0x1F80038Cu,0); w_u32(0x1F8003A0u,ordering); w_u32(0x1F800364u,difference);
    (void)gtmain_model_transform_draw(0x1F800000u,model+60u,1);
    {
        sint32 roll=(sint16)r_u16(0x80092266u);
        w_u16(0x1F8003A8u,palette); w_u16(0x1F8003AAu,depth);
        (void)gtmain_car_wheels_draw(0x1F800000u,model,roll,0,0);
    }
    w_u32(0x1F800364u,0); (void)gtmain_model_shadow_draw(0x1F800000u,r_u32(model+56u));
    if (flags&1u) {
        w_u32(0x1F800364u,0u-difference);
        (void)gtmain_matrix_negate_axes((uint8 *)psx_addr(0x1F800374u,20),(const uint8 *)psx_addr(0x1F800374u,20));
        w_u16(0x1F80038Cu,65535); w_u32(0x1F8003A0u,ordering+8192u);
        (void)gtmain_model_transform_draw(0x1F800000u,model+60u,1);
        {
            sint32 roll=(sint16)r_u16(0x80092266u);
            w_u16(0x1F8003A8u,palette); w_u16(0x1F8003AAu,depth);
            (void)gtmain_car_wheels_draw(0x1F800000u,model,roll,0,0);
        }
    }
    result=flags&2u;
    if (result) {
        uint32 vx,vy,vz,magnitude,bits,shift,table_shift,center,previous,current,packet,angle=64;
        (void)gtmain_model_rotation(matrix,0,(sint16)r_u16(0x80092264u),0);
        menu_model_word(vector,0,0); menu_model_word(vector,4,0xCCCCu); menu_model_word(vector,8,0xFFF40000u);
        (void)gtmain_model_rotate_vector(vector,matrix,vector); menu_model_orientation(view,0x80099490u);
        vx=0u-menu_model_read_word(vector,0); vy=0u-menu_model_read_word(vector,4); vz=0u-menu_model_read_word(vector,8);
        magnitude=((sint32)vx<0 ? 0u-vx : vx) | ((sint32)vy<0 ? 0u-vy : vy) | ((sint32)vz<0 ? 0u-vz : vz);
        menu_model_gte_write(30,magnitude); bits=31u-menu_model_gte_read(31);
        if ((sint32)bits<18) bits=18;
        table_shift=bits-18u; shift=bits-12u; if ((sint32)shift<0) shift=0;
        vx=(uint32)((sint32)vx >> (shift&31u)); vy=(uint32)((sint32)vy >> (shift&31u)); vz=(uint32)((sint32)vz >> (shift&31u));
        menu_model_gte_write(0,(vx&65535u) | (vy << 16)); menu_model_gte_write(1,vz);
        /* TODO Recover the original uninitialized twenty byte matrix provenance */
        (void)temporary;
        gt_draft_failure("Menu model projection matrix provenance",0x8007070Cu);
        (void)gtmain_matrix_bind_negated_axes(temporary);
        menu_model_gte_command(0x486012u);
        vx=menu_model_gte_read(25); vy=menu_model_gte_read(26); vz=menu_model_gte_read(27);
        menu_model_gte_control(5,vx); menu_model_gte_control(6,vy); menu_model_gte_control(7,vz);
        menu_model_gte_write(0,0); menu_model_gte_write(1,0); menu_model_gte_command(0x180001u);
        vx=(uint32)((sint32)(sint16)r_u16(0x8009423Cu) >> (table_shift&31u)); vz=(uint32)((sint32)(sint16)r_u16(0x80094A3Cu) >> (table_shift&31u));
        center=menu_model_gte_read(14); menu_model_gte_write(0,vx&65535u); menu_model_gte_write(1,vz); menu_model_gte_command(0x180001u);
        packet=r_u32(0x800BFE4Cu);
        vx=(uint32)((sint32)(sint16)r_u16(0x800942BCu) >> (table_shift&31u)); vz=(uint32)((sint32)(sint16)r_u16(0x80094ABCu) >> (table_shift&31u));
        previous=menu_model_gte_read(14);
        do {
            uint32 index,old,link;
            menu_model_gte_write(0,vx&65535u); menu_model_gte_write(1,vz); menu_model_gte_command(0x180001u);
            angle+=64u; index=angle&4095u;
            vx=(uint32)((sint32)(sint16)r_u16(0x8009423Cu+index*2u) >> (table_shift&31u));
            vz=(uint32)((sint32)(sint16)r_u16(0x80094A3Cu+index*2u) >> (table_shift&31u));
            old=previous; previous=menu_model_gte_read(14); current=packet;
            w_u32(current,0x06000000u); w_u32(current+16u,old); w_u32(current+4u,0x32404040u); w_u32(current+8u,center); w_u32(current+12u,0); w_u32(current+20u,0); w_u32(current+24u,previous);
            link=r_u8(ordering+4096u); link|=r_u8(ordering+4097u) << 8; link|=r_u8(ordering+4098u) << 16;
            w_u8(ordering+4096u,current); w_u8(ordering+4097u,current >> 8); w_u8(ordering+4098u,current >> 16);
            w_u8(current,link); w_u8(current+1u,link >> 8); w_u8(current+2u,link >> 16);
            packet+=28u;
        } while ((sint32)angle<4097);
        w_u32(0x800BFE4Cu,packet); (void)gtmain_packet_page(ordering+4096u,544);
        if (!(flags&4u)) {
            (void)gtmain_packet_rectangle(ordering+4096u,0x80092268u,0x02000000u);
            (void)gtmain_packet_page(ordering+4096u,512);
        }
        result=gtmain_packet_page(ordering+4096u,576);
    }
    return result;
}
