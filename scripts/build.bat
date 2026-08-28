@echo off
REM Script de build para Windows

echo 🐍 Building Snake Game...

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set JOBS=%2
if "%JOBS%"=="" set JOBS=4

mkdir build 2>nul
cd build

cmake -B . -S .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -G "MinGW Makefiles"

echo 🔨 Compiling with %JOBS% jobs...
cmake --build . --config %BUILD_TYPE% --parallel %JOBS%

echo ✅ Build completed successfully!
echo 🎮 Run with: .\bin\SnakeGame.exe