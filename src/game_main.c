#include "game_startup.h"
#include "psx.h"
#include "game_spu_access.h"

/* Original PsyQ graph type storage */
const uint32 xport_gpu_graph_type_address = 0x800B24B4u;

void gtmain_restore_loader_context(GTMainStartupState *state)
{
    state->return_address = r_u32(0x8008F1DCu);
    state->stack_pointer = r_u32(0x8008F1D8u);
    state->saved0 = r_u32(0x8008F1E0u);
    state->saved1 = r_u32(0x8008F1E4u);
    state->saved2 = r_u32(0x8008F1E8u);
    state->saved3 = r_u32(0x8008F1ECu);
    state->saved4 = r_u32(0x8008F1F0u);
    state->saved5 = r_u32(0x8008F1F4u);
    state->saved6 = r_u32(0x8008F1F8u);
    state->saved7 = r_u32(0x8008F1FCu);
    state->result = state->argument0;
}

void gtmain_startup(GTMainStartupState *state)
{
    FUNCTION_MARKER(0x80010000u, "GTMAIN.EXE");
    w_u32(0x8008F1E0u, state->saved0);
    w_u32(0x8008F1E4u, state->saved1);
    w_u32(0x8008F1E8u, state->saved2);
    w_u32(0x8008F1ECu, state->saved3);
    w_u32(0x8008F1F0u, state->saved4);
    w_u32(0x8008F1F4u, state->saved5);
    w_u32(0x8008F1F8u, state->saved6);
    w_u32(0x8008F1FCu, state->saved7);
    state->result = r_u32(0x800B1AA4u);
    state->assembler_temporary = 0x80090000u;
    w_u32(0x8008F1D8u, state->stack_pointer);
    if (state->result != 0u)
        state->stack_pointer = state->result;
    w_u32(0x8008F1DCu, state->return_address);

    state->stack_pointer -= 0x20u;
    w_u32(state->stack_pointer + 0x10u, state->saved0);
    state->saved0 = state->argument0;
    w_u32(state->stack_pointer + 0x14u, state->saved1);
    state->saved1 = state->argument1;
    state->argument0 = 0x801FB724u;
    w_u32(0x8008F204u, 0x8008B848u - 0x80010000u);
    w_u32(0x8008F20Cu, 0x800B38F4u - 0x8008F1E0u);
    w_u32(0x8008F214u, state->argument0 - 0x800B38F4u);
    state->secondary_result = 0x800B38F4u;
    state->result = state->argument0 < state->secondary_result;
    w_u32(state->stack_pointer + 0x18u, state->return_address);
    w_u32(0x8008F218u, state->saved0);
    w_u32(0x8008F21Cu, state->saved1);
    if (state->result == 0u)
    {
        do
        {
            state->argument0 -= 4u;
            state->result = state->argument0 < state->secondary_result;
            w_u32(state->argument0, 0u);
        } while (state->result == 0u);
    }

    state->return_address = 0x8001012Cu;
    gtmain_8007BC84(state);
    state->argument0 = state->saved0;
    state->return_address = 0x80010138u;
    state->argument1 = state->saved1;
    gtmain_80019EC0(state);
    state->return_address = 0x80010140u;
    state->argument0 = state->result;
    gtmain_80082928(state);
    state->return_address = r_u32(state->stack_pointer + 0x18u);
    state->saved1 = r_u32(state->stack_pointer + 0x14u);
    state->saved0 = r_u32(state->stack_pointer + 0x10u);
    state->stack_pointer += 0x20u;
}

void xport_main(void)
{
    gt_native_spu_bind_mmio();
    gtmain_native_main();
}
