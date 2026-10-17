#ifndef GT_GAME_STARTUP_H
#define GT_GAME_STARTUP_H

#include "xport.h"

/* Guest values crossing the original startup boundary */
typedef struct GTMainStartupState
{
    uint32 argument0;
    uint32 argument1;
    uint32 argument2;
    uint32 argument3;
    uint32 result;
    uint32 secondary_result;
    uint32 assembler_temporary;
    uint32 stack_pointer;
    uint32 return_address;
    uint32 saved0;
    uint32 saved1;
    uint32 saved2;
    uint32 saved3;
    uint32 saved4;
    uint32 saved5;
    uint32 saved6;
    uint32 saved7;
} GTMainStartupState;

void gtmain_startup(GTMainStartupState *state);
void gtmain_restore_loader_context(GTMainStartupState *state);
void gtmain_native_main(void);

/* Unimplemented image-qualified calls retain their boundary state */
void gtmain_8007BC84(GTMainStartupState *state);
void gtmain_80019EC0(GTMainStartupState *state);
void gtmain_80082928(GTMainStartupState *state);

/* Loader adapters must supply guest values from a reviewed launch context */
GTMainStartupState *gtmain_loader_startup_state(void);
void gtmain_loader_return(GTMainStartupState *state);

#endif
