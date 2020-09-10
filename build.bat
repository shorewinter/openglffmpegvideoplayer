@echo off
REM cant get ffmpeg libs to link. will have to build from source
REM g++ -o bin\videoplayer.exe src\main.cpp -Iinclude\ffmpeg -Iinclude -Llib\glfw -lglfw3 -Llib\ffmpeg -lavformat.dll -lgdi32 -lopengl32
REM if %errorlevel% neq 0 EXIT /B
REM bin\videoplayer.exe

cl /Zi src\main.cpp /Febin\videoplayer.exe /I include /I include\ffmpeg /link lib\glfw\glfw3.lib lib\ffmpeg\*.lib user32.lib shell32.lib opengl32.lib gdi32.lib kernel32.lib msvcrt.lib msvcmrt.lib /nologo
if %errorlevel% neq 0 EXIT /B
del *.obj *.ilk vc140.pdb
bin\videoplayer.exe