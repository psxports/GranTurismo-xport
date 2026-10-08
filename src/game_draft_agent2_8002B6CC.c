#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_8002B6CC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v2 = 0;
    sint32 v6 = 0;
    sint16 v8[60] = {0};
    sint32 v9[5] = {0};
    sint8 v10[44] = {0};
    uint32 carrier_v0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
  v8[2] = 110;
  v8[3] = 32;
  v8[5] = 16;
  v8[0] = 0;
  v8[1] = 0;
  v8[4] = 55;
  v8[6] = 55;
  v8[7] = 8;
  v8[8] = 8;
  gt_draft_unresolved_call(0x8006B3E0u, gt_draft_buffer_address(v8, sizeof(v8)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0, 0);
  v2 = r_u32(0x800B6638u);
  gt_draft_unresolved_call(0x8006B134u, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, 0, -32, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80070320u, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, r_u32(0x80099534u), 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80070320u, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, v2 + 140, gt_draft_buffer_address(v10, sizeof(v10)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0);
  v9[1] = 13107;
  v9[0] = 0;
  v9[2] = 0x8000;
  gt_draft_unresolved_call(0x8006B2CCu, gt_draft_buffer_address(v9, sizeof(v9)) /* TODO Adapter lifecycle */, v2 + 140, gt_draft_buffer_address(v9, sizeof(v9)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80070410u, gt_draft_buffer_address(v9, sizeof(v9)) /* TODO Adapter lifecycle */, gt_draft_buffer_address(v9, sizeof(v9)) /* TODO Adapter lifecycle */, v2 + 120, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006B40Cu, gt_draft_buffer_address(v8, sizeof(v8)) /* TODO Adapter lifecycle */, -2146697344, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80070448u, a0, gt_draft_buffer_address(v8, sizeof(v8)) /* TODO Adapter lifecycle */, 0, 0, 0, 0, 0, 0);
  carrier_v0 = r_u16(a0 + 32);
  carrier_v1 = r_u32(a0 + 24);
gt_draft_gte_write(26, 1, carrier_v0);
  carrier_v0 = r_u32(a0 + 28);
gt_draft_gte_write(24, 1, carrier_v1);
gt_draft_gte_write(25, 1, carrier_v0);
  v6 = r_u32(0x800BFF6Cu) == 0 ? 0xF0 : 0;
  gt_draft_unresolved_call(0x8006F498u, r_u32(0x800B6388u), v6 << 11, v6 << 10, ((v6 + 239) << 10) | 0x13F, 0, 0, 0, 0);
    w_u32(a0 + 772, r_u32(0x800B6388u) + 16768);
  gt_draft_8001A0E0((uint32)a0, 0, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x800250C8u, a0, r_u32(0x800B6388u), 1, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x80021EE4u, a0, r_u32(0x800B6388u), 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, r_u32(0x800B6388u), 0x8008FB4Cu, 0, 0, 0, 0, 0, 0);
  gt_draft_unresolved_call(0x8006F624u, r_u32(0x800B6388u), r_u32(0x8008FB54u), 0, 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x8006F498u, r_u32(0x800B6388u) + 16772, ((v6 + 14) << 11) | 0x69, ((v6 + 14) << 10) | 0x69, ((v6 + 45) << 10) | 0xD6, 0, 0, 0, 0);
}
