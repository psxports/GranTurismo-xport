#include "game_draft_registry.h"
#include "game_first2_registry.h"

static void menu_model_transform_initialize(uint32 transform)
{
    /* TODO Bind unselected one-argument model initializer 8003D1A8 */
    (void)gt_draft_unresolved_call(0x8003D1A8u, transform,
        0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

static void menu_assets_release(void)
{
    /* TODO Bind unselected no-argument menu release 8003A2DC */
    (void)gt_draft_unresolved_call(0x8003A2DCu,
        0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u);
}

/* GTMAIN.EXE 80038C48 */
/* Original: GTMAIN.EXE:sub_80038C48. */ uint32 gt_draft_80038C48(void)
{
    w_u16(0x801F3004u, 0u);
    w_u8(0x801F2FE5u, 1u);
    menu_model_transform_initialize(0x801F3018u);
    w_u16(0x801F3018u, r_u16(0x801F3018u) + 1024u);
    menu_assets_release();
    (void)gtmain_menu_asset_activate(15u);
    return 0u;
}
