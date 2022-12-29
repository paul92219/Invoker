/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include "invoker.h"
#include <windows.h>

internal void
GameUpdateAndRender(game_memory *Memory, game_input *Input)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);

    game_state *GameState = (game_state *)Memory->PermanentStorage;
    if(!Memory->IsInitialized)
    {

        //TODO(casey): This may be more appropriate to do in platform layer
        Memory->IsInitialized = true;
    }
    
    for(int ControllerIndex = 0;
        ControllerIndex < ArrayCount(Input->Controllers);
        ++ControllerIndex)
    {
        game_controller_input *Controller = GetController(Input, ControllerIndex);
        if(Controller->IsAnalog)
        {
            //NOTE(casey): Use analog movement tuning

        }
        else
        {
            //NOTE(casey): Use digital movement tuning
            if(Controller->MoveLeft.EndedDown)
            {
                OutputDebugStringA("Left\n");
            }

            if(Controller->MoveRight.EndedDown)
            {
                OutputDebugStringA("Right\n");
            }
        }
    }    

}
