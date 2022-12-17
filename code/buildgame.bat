@echo off

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\buildgame
cl /EHsc -FC -Zi ..\Invoker\code\win32_invokergame.cpp user32.lib gdi32.lib
popd
