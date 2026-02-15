all:
	gcc main.c mouse_hook.c -o main.exe -lmingw32 -lSDL2main -lSDL2 -mwindows -O2