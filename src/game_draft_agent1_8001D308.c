#include "game_first2_registry.h"
#include "game_draft_registry.h"

/* Unverified translation from complete pseudocode and original MIPS */
/* Original: GTMAIN.EXE:sub_8001D308. */ uint32 gtmain_vector_path_dispatch(uint32 output, uint32 path)
{
    uint32 kind = r_u16(path);
    if (!kind)
        return gtmain_vector_path_sample(path, output);
    if (kind == 1u)
        return gtmain_camera_spline_position(path, psx_addr(output, 12u));
    return 1u;
}
