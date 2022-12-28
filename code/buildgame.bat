@echo off

set CommonCompilerFlags=-MT -nologo -GR- -Gm- -EHa- -Od -Oi -WX -W4 -wd4456 -wd4201 -wd4100 -wd4189 -DINVOKER_INTERNAL=1 -DINVOKER_SLOW=1 -FC -Z7 -Fmwin32_handmade.map 
set CommonLinkerFlags= -opt:ref user32.lib gdi32.lib winmm.lib
REM TODO - can we just build both with one exe?

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\buildgame

REM 32-bit build
REM cl %CommonCompilerFlags% ..\HandmadeHero\code\win32_handmade.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags% 

REM 64-bit build
cl %CommonCompilerFlags% ..\Invoker\code\win32_invokergame.cpp /link %CommonLinkerFlags% 
popd
