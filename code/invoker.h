#if !defined(INVOKER_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
/*
  NOTE(casey):

  INVOKER_INTERNAL:
    0 - Build for public release
    1 - Build for developer only

  INVOKER_SLOW:
    0 - Not slow code allowed!
    1 - Slow code welcome.
*/

#if INVOKER_SLOW
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif


#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

//TODO(casey): swap, min, max ... macros???

inline uint32
SafeTruncateUint64(uint64 Value)
{
    // TODO(casey): Defines for maximum values
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32)Value;
    return(Result);
    
}

struct game_memory
{
    bool32 IsInitialized;

    uint64 PermanentStorageSize;
    void *PermanentStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

    uint64 TransientStorageSize;
    void *TransientStorage; // NOTE(casey): REQUIRED to be cleared to zero at startup

};


struct game_button_state
{
    int HalfTransitionCount;
    bool32 EndedDown;
};

struct game_controller_input
{
    bool32 IsConnected;
    bool32 IsAnalog;
    real32 StickAverageX;
    real32 StickAverageY;
    
    union
    {
        game_button_state Buttons[12];
        struct
        {
            game_button_state MoveUp;
            game_button_state MoveDown;
            game_button_state MoveLeft;
            game_button_state MoveRight;

            game_button_state ActionUp;
            game_button_state ActionDown;
            game_button_state ActionLeft;
            game_button_state ActionRight;

            game_button_state LeftShoulder;
            game_button_state RightShoulder;

            game_button_state Back;
            game_button_state Start;

            // NOTE(casey): All buttons must be added above this line
            
            game_button_state Terminator;
        };
    };
};

struct game_input
{
    // TODO(casey): Insert clock values here.
    game_controller_input Controllers[5];
};

inline game_controller_input *GetController(game_input *Input, int unsigned ControllerIndex)
{
    Assert(ControllerIndex < ArrayCount(Input->Controllers));
    game_controller_input *Result = &Input->Controllers[ControllerIndex];
    return(Result);
}

internal void GameUpdateAndRender(game_memory *Memory);

//
struct game_state
{
    int ToneHz;
    int GreenOffset;
    int BlueOffset;
};

#define INVOKER_H
#endif
