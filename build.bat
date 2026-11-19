@echo off
rem ============================================================
rem  Build the project with MinGW-w64 g++.
rem  Usage:  build.bat                     use g++ from PATH
rem          build.bat "C:\path\g++.exe"   use the given compiler
rem  Note: keep this file ASCII-only; cmd.exe reads .bat in the
rem        console code page and would garble non-ASCII text.
rem ============================================================
setlocal
cd /d "%~dp0"

if not exist build mkdir build

set "GXX=%~1"
if defined GXX goto :compile

where g++ >nul 2>nul
if not errorlevel 1 (
    set "GXX=g++"
    goto :compile
)

if exist "%USERPROFILE%\Downloads\mingw64\bin\g++.exe" (
    set "GXX=%USERPROFILE%\Downloads\mingw64\bin\g++.exe"
)

:compile
echo Compiler: %GXX%
%GXX% -std=c++17 -Wall -Wextra -Wpedantic -O2 -I src ^
    src\Input.cpp src\User.cpp src\Activity.cpp src\Lecture.cpp ^
    src\Competition.cpp src\Sport.cpp src\ActivityManager.cpp src\main.cpp ^
    -o build\CampusActivity.exe

if errorlevel 1 (
    echo.
    echo Build FAILED. Install g++ or run: build.bat "C:\path\g++.exe"
    exit /b 1
)

echo Build OK: build\CampusActivity.exe
echo Run it:   build\CampusActivity.exe
echo Tests:    run_tests.bat
endlocal
