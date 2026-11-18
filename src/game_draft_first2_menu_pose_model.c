#include "game_first2_registry.h"
#include "xport_trace.h"

/* GTMAIN.EXE 8003D274 */
/* Original: GTMAIN.EXE:sub_8003D274. */ uint32 gtmain_menu_pose_model_draw(uint32 pose, uint32 ordering, sint32 x,
    sint32 y, uint32 arg4, uint32 arg5, uint32 arg6, uint32 arg7,
    uint32 arg8, uint32 arg9, uint32 arg10, uint32 arg11, uint32 arg12)
{
    sint32 angle0, angle1, angle2;
    uint32 position0, position1, position2;
    FUNCTION_MARKER(0x8003D274, "GTMAIN.EXE");
    angle0 = r_s16(pose);
    angle1 = r_s16(pose + 2u);
    angle2 = r_s16(pose + 4u);
    position0 = r_u32(pose + 8u);
    position1 = r_u32(pose + 12u);
    position2 = r_u32(pose + 16u);
    /* Three literal zeros are explicit original call arguments */
    return (uint32)gtmain_menu_scene_model_draw(ordering, x, y, 0u, 0u, 0u,
        angle0, angle1, angle2, position0, position1, position2,
        arg4, arg5, arg6, (uint16)arg7, arg8, (uint16)arg9,
        arg10, arg11, arg12);
}
