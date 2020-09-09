@echo off
g++ -o bin\gl.exe src\main.cpp -Iinclude -Llib\glfw -lglfw3 -Llib\ffmpeg -lavcodec  -lavdevice  -lavfilter  -lavformat  -lavutil  -lpostproc  -lswresample  -lswscale -lgdi32 -lopengl32
if %errorlevel% neq 0 EXIT /B
bin\gl.exe