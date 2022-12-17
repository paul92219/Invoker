/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include "invokergame.h"

internal void
ElemetsOfSkill(uint32 VKCode)
{   
    if(VKCode == 'Q')
    {
        SpellConfig.Spell[SpellConfig.PressCounter] = 'Q';
        SpellConfig.PressCounter++;
    }
    else if(VKCode == 'W')
    {
        SpellConfig.Spell[SpellConfig.PressCounter] = 'W';
        SpellConfig.PressCounter++;
    }
    else if(VKCode == 'E')
    {
        SpellConfig.Spell[SpellConfig.PressCounter] = 'E';
        SpellConfig.PressCounter++;
    }
    
}

internal bool32
UsersSkill(keyboardinput *Input) 
{
    bool32 Result = false;
    
    if(SpellConfig.PressCounter >= 3 && (Input->VKCode == 'Q' ||
                                         Input->VKCode == 'W' || Input->VKCode == 'E'))
    {
        SpellConfig.Spell[0] = SpellConfig.Spell[1];
        SpellConfig.Spell[1] = SpellConfig.Spell[2];
        
        if(Input->VKCode == 'Q')
        {
            SpellConfig.Spell[2] = 'Q';
        }
        else if(Input->VKCode == 'W')
        {
            SpellConfig.Spell[2] = 'W';
        }
        else if(Input->VKCode == 'E')
        {
            SpellConfig.Spell[2] = 'E';
        }    
    }
    else
    {
        ElemetsOfSkill(Input->VKCode);
    }        

    if(Input->VKCode == 'R' && SpellConfig.PressCounter >= 3)
    {
        Result = true;
        SpellConfig.CombinationDone = true;
    }

    return(Result);
}
