/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Casey Muratori $
   $Notice: (C) Copyright 2014 by Molly Rocket, Inc. All Rights Reserved. $
   ======================================================================== */
#include <stdint.h>

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

#include <windows.h>

#include "win32_invokergame.h"

// TODO(casey): This is a global now
global_variable bool32 GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
/*
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

        } break;
        
        case WM_PAINT:
        {   
            
            OutputDebugStringA("WM_PAINT\n");

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
        
        GlobalRunning = true;
        
        if(Window)
        {
            HDC DeviceContext = GetDC(Window);
            
            while(GlobalRunning)
            {
                MSG Message;
                
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
