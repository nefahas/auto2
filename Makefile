all:
	gcc main.c mouse_hook.c -o main.exe \
	$(shell pkg-config --cflags --libs sdl2) \
	-lmingw32 -mwindows -O2
