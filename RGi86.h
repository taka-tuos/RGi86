#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include "e8086/e8086.h"
#include <stdio.h>

// SDL
int poll_event(SDL_Event *sdl_event);

// 8086 I/O
void v30_setup_callbacks(e8086_t *v30);

// Graphics
#define SCRN_X	640
#define	SCRN_Y	480
void init_graphic(unsigned char *mem_v30, SDL_Surface *sdl_screen);
void refresh_graphic(SDL_Surface *sdl_screen, unsigned char *mem_v30);

// Audio
void init_audio(void);
