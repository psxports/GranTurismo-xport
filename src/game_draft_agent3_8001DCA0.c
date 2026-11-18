#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified translation from complete pseudocode and original MIPS */
/* Original: GTMAIN.EXE:sub_8001DCA0. */ uint32 gtmain_camera_selected_pose_prepare(uint32 camera)
{
    uint32 mode = (uint32)(sint32)r_s8(camera + 256u);
    return gtmain_camera_pose_prepare(camera, mode, 164);
}
