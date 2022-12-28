/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace std;

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#include "invokergame.h" 
#include "win32_readingfiles.cpp" 
#include "computing.cpp"
#include <windows.h>
#include <time.h>

#include "win32_invokergame.h"

// TODO(casey): This is a global now
global_variable bool32 GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable bool32 draw = false;
//global_variable bool32 CombinationDone = false;

global_variable spell SkillBuffer[10] = {}; 
global_variable enemy EnemyBuffer[3] = {};

char Spells[10][4][4] =
{ {"QQQ", {}, {}, {3, 0, 0}},
  {"QQW", "QWQ", "WQQ", {2, 1, 0}},
  {"QQE", "QEQ", "EQQ", {2, 0, 1}},
  {"WWW", {}, {}, {0, 3, 0}},
  {"WWQ", "WQW", "QWW", {1, 2, 0}},
  {"WWE", "WEW", "EWW", {0, 2, 1}},
  {"EEE", {}, {}, {0, 0, 3}},
  {"EEQ", "EQE", "QEE", {1, 0, 2}},
  {"EEW", "EWE", "WEE", {0, 1, 2}},
  {"QWE", "WEQ", "EQW", {1, 1, 1}}
};

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Height = ClientRect.bottom - ClientRect.top;
    Result.Width = ClientRect.right - ClientRect.left;

    return(Result);
}
/*
  internal void
  RenderWeirdGradient(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
  {
  // TODO(casey): Let's see what the optimizer does
    
  uint8 *Row = (uint8 *)Buffer->Memory;
  for(int Y = 0; Y < Buffer->Height; ++Y)
  {
  uint32 *Pixel = (uint32 *)Row;
  for(int X = 0; X < Buffer->Width; ++X)
  {
  uint8 Blue = (X + BlueOffset);
  uint8 Green = (Y + GreenOffset);

  *Pixel++ = ((Green << 8) | Blue);
  }

  Row += Buffer->Pitch;
  }
  }
*/
internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    // TODO(casey): Bulletproof this
    // Maybe don't free first, free after, then free first if that fails

    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;
    int BytesPerPixel = 4;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width * Buffer->Height) * BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    // TODO(casey): Probably clear this to black

    Buffer->Pitch = Width * BytesPerPixel;

}    
/*
  internal void
  Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer,
  HDC DeviceContext, int WindowWidth, int WindowHeight)
  {
  // TODO(casey): Aspect ratio correction
  StretchDIBits(DeviceContext,
                  
  //X, Y, Width, Height,
  //X, Y, Width, Height,
                  
  0, 0, WindowWidth, WindowHeight,
  0, 0, Buffer->Width, Buffer->Height,
  Buffer->Memory,
  &Buffer->Info,
  DIB_RGB_COLORS, SRCCOPY);
  }
*/

internal void
Win32SpellComputing()
{
    bool32 Equlity = false;
    
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(!strcmp(Spells[i][j], SpellConfig.Spell))
            {
                Equlity = true;
                break;
            }
        }
        if(Equlity)
        {
            SpellConfig.NumberOfSymbolsInSpell[0] = Spells[i][3][0];
            SpellConfig.NumberOfSymbolsInSpell[1] = Spells[i][3][1];
            SpellConfig.NumberOfSymbolsInSpell[2] = Spells[i][3][2];
            SpellConfig.SpellIndex = i;
            break;
        }
    }
}

internal void
DamageComputing(enemy *EnemyBuffer, spell *SkillBuffer)
{
    SpellConfig.Damage = SkillBuffer->Damage;
    
    if(EnemyBuffer->Susceptibility == 'Q')
    {
        SpellConfig.Damage += SpellConfig.NumberOfSymbolsInSpell[0] * 2;
    }
    else if(EnemyBuffer->Susceptibility == 'W')
    {
        SpellConfig.Damage += SpellConfig.NumberOfSymbolsInSpell[1] * 2;
    }
    else if(EnemyBuffer->Susceptibility == 'E')
    {
        SpellConfig.Damage += SpellConfig.NumberOfSymbolsInSpell[2] * 2;
    }

    if(EnemyBuffer->Immunity == 'Q')
    {
        SpellConfig.Damage -= SpellConfig.NumberOfSymbolsInSpell[0] * 2;
    }
    else if(EnemyBuffer->Immunity == 'W')
    {
        SpellConfig.Damage -= SpellConfig.NumberOfSymbolsInSpell[1] * 2;
    }
    else if(EnemyBuffer->Immunity == 'E')
    {
        SpellConfig.Damage -= SpellConfig.NumberOfSymbolsInSpell[2] * 2;
    }
    else if(EnemyBuffer->Immunity == '-')
    {
    }
    
    EnemyConfig.HP -= SpellConfig.Damage;
}


internal void
Win32PaintGameInWindow(HDC DeviceContext, HWND Window,
                       spell *SkillBuffer, enemy *EnemyBuffer)
{
    if(DrawConfig.DrawingCounter == 1)
    {
        sprintf(DrawConfig.Buffer, "Your enemy is: %s                    ",
                EnemyBuffer->Name);
        TextOut(DeviceContext, 10, 30, DrawConfig.Buffer, sizeof(DrawConfig.Buffer));
    }
    else if(DrawConfig.DrawingCounter == 2)
    {
        sprintf(DrawConfig.Buffer, "You Killed %s by: %s                    ",
                EnemyBuffer->Name, SkillBuffer->Name);
        
        TextOut(DeviceContext, 10, 70, DrawConfig.Buffer, sizeof(DrawConfig.Buffer));
    }
    else if(DrawConfig.DrawingCounter == 3)
    {
        sprintf(DrawConfig.Buffer, "You deal %d damage                    ", SpellConfig.Damage);
        
        TextOut(DeviceContext, 10, 50, DrawConfig.Buffer, sizeof(DrawConfig.Buffer));
    }    
               
}

internal LRESULT CALLBACK
Win32MainWindowCallBack(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam
                        )
{
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_DESTROY:
        {
            // TODO(casey): Handle this with a message to the user?
            GlobalRunning = false;
        } break;

        case WM_CLOSE:
        {
            // TODO(casey): Handle this as an error - recreate window?
            GlobalRunning = false;
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            keyboardinput Input = {};
            Input.VKCode = (uint32)WParam;
            Input.WasDown = ((LParam & (1 << 30)) != 0);
            Input.IsDown = ((LParam & (1 << 31)) == 0);

            if(Input.WasDown)
            {
                if(UsersSkill(&Input))
                {
                    Win32SpellComputing();
                }
            }

        } break;
        
        case WM_PAINT:
        {   
            
            OutputDebugStringA("WM_PAINT\n");
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);

            //InvalidateRect(Window, 0, TRUE);
            TextOut(DeviceContext, 10, 10, "Srart Game", 10);

            if(EnemyConfig.HP <= 0)
            {
                DrawConfig.DrawingCounter = 2;
                Win32PaintGameInWindow(DeviceContext, Window,
                                       &SkillBuffer[SpellConfig.SpellIndex],
                                       &EnemyBuffer[EnemyConfig.EnemyIndex]);                           
            }

            Win32PaintGameInWindow(DeviceContext, Window,
                                   &SkillBuffer[SpellConfig.SpellIndex],
                                   &EnemyBuffer[EnemyConfig.EnemyIndex]);           

            EndPaint(Window, &Paint);

        } break;

        default:
        {
            //OutputDebugStringA("WM_ACTIVATEAPP\n");
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;


    }

    return(Result);
}


int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
    
    WNDCLASS WindowClass = {};
    
    Win32ResizeDIBSection(&GlobalBackBuffer, 1280, 720);

    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallBack;
    WindowClass.hInstance = Instance;
//  WindowClass.hIcon;
    WindowClass.lpszClassName = "InvokerShooterWindowsClass";

    if(RegisterClass(&WindowClass))
    {
        HWND Window = 
            CreateWindowEx(
                0, 
                WindowClass.lpszClassName, 
                "Invoker Shooter", 
                WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
                CW_USEDEFAULT, 
                CW_USEDEFAULT, 
                CW_USEDEFAULT, 
                CW_USEDEFAULT, 
                0, 
                0, 
                Instance, 
                0);
        /*
        HWND WindowButton = CreateWindowEx(
            0,  
            "BUTTON",  // Predefined class; Unicode assumed 
            "OK",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            10,         // x position 
            10,         // y position 
            100,        // Button width
            100,        // Button height
            WindowHandle,     // Parent window
            0,       // No menu.
            Instance, 
            0);      // Pointer not needed.
        */
        
        GlobalRunning = true;

        // NOTE(handy): Reading files here        
        readingfiles Pointers;
        
        Win32ReadingSkillsFile(SkillBuffer, &Pointers);
        Win32ReadingEnemiesFile(EnemyBuffer, &Pointers);
        
        srand((unsigned) time(0));
        
        if(Window)
        {
            HDC DeviceContext = GetDC(Window);
            
            while(GlobalRunning)
            {
                MSG Message;
                
                if(EnemyConfig.HP <= 0)
                {
                    EnemyConfig.EnemyIndex = rand() % 3;
                    
                    EnemyConfig.HP = EnemyBuffer[EnemyConfig.EnemyIndex].HP;

                    DrawConfig.DrawingCounter = 1;
                    RedrawWindow(Window, 0, 0, RDW_INVALIDATE);
                }
                
                if(SpellConfig.CombinationDone)
                {
                    SpellConfig.CombinationDone = false;
                    DamageComputing(&EnemyBuffer[EnemyConfig.EnemyIndex],
                                    &SkillBuffer[SpellConfig.SpellIndex]);
                    DrawConfig.DrawingCounter = 3;
                    RedrawWindow(Window, 0, 0, RDW_INVALIDATE);                    
                }
                
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    if(Message.message == WM_QUIT)
                    {
                        GlobalRunning = false;
                    }
                    
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }                
            }
        }
        else
        {
            // TODO(casey): Logging
        }
        
    }
    else
    {
        // TODO(casey): Logging
    }
    
    return(0);
}
