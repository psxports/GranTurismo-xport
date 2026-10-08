#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

/* Unverified draft from complete exported pseudocode with guest address operations */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_80019F10(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  ResetCallback();
  uint32 cd_result = gtmain_cd_init();
  gtmain_sound_init(cd_result);
  /* Native startup does not install or patch the guest BIOS card driver */
  gtmain_pad_init();
  gtmain_graph_reset(3u);
  InitGeom();
  gtmain_frame_init(1u);
  gtmain_frame_events_init();
  w_u16((uint32)(0x1F801128), (uint32)((sint32)(0u - (uint32)(1))));
  gtmain_sound_saved_data_init(0x800C4C00u);
  w_u32((uint32)(0x800B3908u), (uint32)((sint32)(0x80019EE8u)));
  w_u32((uint32)(0x800B390Cu), (uint32)((sint32)(0u - (uint32)(1))));
  gtmain_frame_callback_insert(0x800B3900u);
  gtmain_sound_sequence_init();
  gtmain_sound_tempo(60u);
  w_u16(0x800B59A0u, 60u);
  gtmain_sound_reverb(4u, 0u, 0u);
  gtmain_sound_reverb_enable(1u);
  gtmain_resource_init(0x8008B848u, 0x800C0BF0u, 0x4000u);
  return gtmain_messages_init();
}

