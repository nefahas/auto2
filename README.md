# auto2
Hold m1 autoclicker with sdl window to show when toggled
has a ~100 ms delay before starting to autoclick, to allow singular clicks while on.

built on mingw UCRT64 gcc; must have SDL2 installed on ucrt64 (pacman -S mingw-w64-ucrt-x86_64-SDL2)

run makefile, output will be main.exe

on open:
  toggle key is MB3 (scroll wheel)
  ui turns green when on, black when off
  clicks around 100 cps while held down, it starts clicking around 100ms after you first click (allows you to click once)
