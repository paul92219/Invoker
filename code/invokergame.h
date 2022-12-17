#if !defined(INVOKERGAME_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
struct keyboardinput
{
    uint32 VKCode;
    bool32 WasDown;
    bool32 IsDown;
};

struct readingfiles
{
    fstream SkillFile;
    fstream EnemyFile;
};

struct spell
{
    char Combin[4];
    char Name[15];
    int  Damage;
};

struct enemy
{
    int EnemyCounter = 0;
    int HP;
    char Susceptibility;
    char Immunity;
    char Name[15];
        
};

struct
{
    int EnemyIndex;
    int HP;
    
} EnemyConfig;

struct
{
    char Buffer[256];
    int DrawingCounter = 0;
    int Size;
    
} DrawConfig;

struct
{
    int SpellIndex;    
    int PressCounter = 0;
    char Spell[5];   
    int NumberOfSymbolsInSpell[3];
    int Damage;
    bool32 CombinationDone = false;
    
} SpellConfig;

#define INVOKERGAME_H
#endif
