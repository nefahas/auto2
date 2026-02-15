#include <stdio.h>
#include <SDL2/SDL.h>
#include "include/mouse_hook.h"

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int quit = 0;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("hi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 250, 200, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
    
    if (!window) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }
    
    HANDLE mouse_hook = CreateThread(
        NULL,
        0,
        init_hook_thread,
        NULL,
        0,
        NULL
    );

    if (!mouse_hook) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    Uint8 do_click_check = 0;
    Uint8 do_click = 0;
    Uint64 click_delay_last = SDL_GetTicks64();
    Uint64 last_click = click_delay_last;

    // setup the send input struct
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    
    Uint8 last_toggle = toggled;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = 1;
                break;
            }
        }

        if (quit) {
            break;
        }
        
        if (last_toggle != toggled) {
            SDL_SetRenderDrawColor(renderer, 0, (toggled ? 180 : 0), 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);

            last_toggle = toggled;
        }

        if (toggled && m1_down) {
            Uint64 now = SDL_GetTicks64();

            if (!do_click_check) { // start the 100ms delay
                do_click_check = 1;
                click_delay_last = now;
                continue;
            } else { // if within 100ms delay
                if (now - click_delay_last >= 100) { 
                    if (!toggled || !m1_down) { // they let go at the 100 ms time (this allows them to let go and click within the 100 ms time tho but idc its only me using it so ez ez ez)
                        do_click_check = 0;
                        do_click = 0;
                        continue;
                    }

                    do_click = 1;
                    do_click_check = 0;
                }
            }

            if (do_click) { // check passed
                if (now - last_click >= 10) {
                    last_click = now;
                    SendInput(
                        2,
                        inputs,
                        sizeof(INPUT)
                    );
                }
            }
        } else {
            do_click = 0;
            do_click_check = 0;
            SDL_Delay(10); // when not actively clicking just delay 10 ms every frame (should be a ~100 - 110 ms delay total? not too bad since i want it to wait 100ms before clicking)
        }
    }

    CloseHandle(mouse_hook);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;

}
