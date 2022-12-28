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
