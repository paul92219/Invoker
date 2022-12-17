/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include "invokergame.h"

internal void
Win32ReadingSkillsFile(spell SkillsBuffer[10], readingfiles *Pointer)
{
    Pointer->SkillFile.open("D:\\Projects\\mygame\\Invoker\\InvokersSkills.txt");

    if(Pointer->SkillFile.is_open())
    {
        int count = 0;
        
        while(Pointer->SkillFile)
        {

            Pointer->SkillFile >> SkillsBuffer[count].Combin;
            Pointer->SkillFile >> SkillsBuffer[count].Name;
            Pointer->SkillFile >> SkillsBuffer[count].Damage;

            if(count == 9)
            {
                break;
            }
            else
            {
                count++;
            }
        }
        
        Pointer->SkillFile.close();
    }
    
}

internal void
Win32ReadingEnemiesFile(enemy EnemiesBuffer[3], readingfiles *Pointer)
{
    Pointer->EnemyFile.open("D:\\Projects\\mygame\\Invoker\\Enemies.txt");

    if(Pointer->EnemyFile.is_open())
    {
        int count = 0;
        
        while(Pointer->EnemyFile)
        {
            Pointer->EnemyFile >> EnemiesBuffer[count].Name;
            Pointer->EnemyFile >> EnemiesBuffer[count].Susceptibility;
            Pointer->EnemyFile >> EnemiesBuffer[count].Immunity;
            Pointer->EnemyFile >> EnemiesBuffer[count].HP;            

            if(count == 3)
            {
                break;
            }
            else
            {
                count++;
            }
        }
        
        Pointer->EnemyFile.close();
    }
    
}
