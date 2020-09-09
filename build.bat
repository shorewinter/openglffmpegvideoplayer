@echo off
g++ -o bin\gl.exe src\main.cpp -Iinclude -Llib -lglfw3 -lgdi32 -lopengl32
if %errorlevel% neq 0 EXIT /B
bin\gl.exe