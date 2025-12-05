@echo off
echo ================================================================================
echo                    GAMING HUB - C++ Arcade Compiler
echo ================================================================================
echo.

REM Check if raylib folder exists in common locations
set RAYLIB_PATH=

if exist "C:\raylib\include\raylib.h" (
    set RAYLIB_PATH=C:\raylib
    echo Found raylib at: C:\raylib
) else if exist ".\include\raylib.h" (
    set RAYLIB_PATH=.
    echo Found raylib in current folder
) else if exist "..\raylib\include\raylib.h" (
    set RAYLIB_PATH=..\raylib
    echo Found raylib at: ..\raylib
) else (
    echo.
    echo ERROR: raylib not found!
    echo.
    echo Please download raylib from: https://www.raylib.com/
    echo Extract it to C:\raylib or to the same folder as this project
    echo.
    pause
    exit /b 1
)

echo.
echo Compiling GamingHub.cpp...
echo.

g++ -o GamingHub.exe GamingHub.cpp -I "%RAYLIB_PATH%\include" -L "%RAYLIB_PATH%\lib" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

if %errorlevel%==0 (
    echo.
    echo ================================================================================
    echo                         COMPILATION SUCCESSFUL!
    echo ================================================================================
    echo.
    echo Starting Gaming Hub...
    echo.
    start GamingHub.exe
) else (
    echo.
    echo ================================================================================
    echo                         COMPILATION FAILED!
    echo ================================================================================
    echo.
    echo Make sure you have:
    echo   1. MinGW g++ installed and in your PATH
    echo   2. raylib downloaded and extracted properly
    echo.
)

pause

