#include "game_first2_registry.h"

static uint32 pose_model_draw_boundary(uint32 ordering, sint32 x, sint32 y,
    uint32 arg3, uint32 arg4, uint32 arg5, sint32 angle0, sint32 angle1,
    sint32 angle2, uint32 position0, uint32 position1, uint32 position2,
    uint32 arg12, uint32 arg13, uint32 arg14, uint32 arg15, uint32 arg16,
    uint32 arg17, uint32 arg18, uint32 arg19, uint32 arg20)
{
    /* TODO Bind unselected3CF14 with these twenty-one original arguments */
    gt_draft_failure("Menu pose model draw dependency", 0x8003CF14u);
}

/* GTMAIN.EXE 8003D274 */
uint32 gtmain_menu_pose_model_draw(uint32 pose, uint32 ordering, sint32 x,
    sint32 y, uint32 arg4, uint32 arg5, uint32 arg6, uint32 arg7,
    uint32 arg8, uint32 arg9, uint32 arg10, uint32 arg11, uint32 arg12)
{
    sint32 angle0 = r_s16(pose);
    sint32 angle1 = r_s16(pose + 2u);
    sint32 angle2 = r_s16(pose + 4u);
    uint32 position0 = r_u32(pose + 8u);
    uint32 position1 = r_u32(pose + 12u);
    uint32 position2 = r_u32(pose + 16u);
    /* Three literal zeros are explicit original call arguments */
    return pose_model_draw_boundary(ordering, x, y, 0u, 0u, 0u,
        angle0, angle1, angle2, position0, position1, position2,
        arg4, arg5, arg6, (uint16)arg7, arg8, (uint16)arg9,
        arg10, arg11, arg12);
}
