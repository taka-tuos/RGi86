#include "RGi86.h"
#include "libgpu.h"
#include "libini.h"
#include <dirent.h>

int ini_mhz = 0;
int ini_sblen = 4410;
int ini_x2s;
int ini_hq;

SDL_Surface *sdl_screen;
SDL_Surface *sdl_screen_m;
unsigned char *mem_v30;
int sclk = 0;

struct dirent *roms[512];

#define FPS 60

void adjustFPS(void) {
	static unsigned long maetime=0;
	static int frame=0;
	long sleeptime;
	if(!maetime) maetime=SDL_GetTicks();
	frame++;
	sleeptime=(frame<FPS)?
		(maetime+(long)((double)frame*(1000.0/FPS))-SDL_GetTicks()):
		(maetime+1000-SDL_GetTicks());
	if(sleeptime>0)SDL_Delay(sleeptime);
	if(frame>=FPS) {
		frame=0;
		maetime=SDL_GetTicks();
	}
}

enum {
	XSAI_A,
	XSAI_B,
	XSAI_C,
	XSAI_D,
	XSAI_1 = 0,
	XSAI_2,
	XSAI_3,
	XSAI_4
};

void SDL_x2Surface(SDL_Surface *dst, SDL_Surface *src)
{
	int x, y;
	int i, j;
	for(y = 0; y < src->h; y++) {
		for(x = 0; x < src->w; x++) {
			int icol[4];

			icol[XSAI_A] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_B] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_C] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_D] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					((int *)dst->pixels)[((y<<1) + i) * dst->w + ((x<<1) + j)] = icol[i * 2 + j];
				}
			}
		}
	}
}

void SDL_sx2Surface(SDL_Surface *dst, SDL_Surface *src)
{
	int x, y;
	int i, j;
	for(y = 0; y < src->h; y++) {
		for(x = 0; x < src->w; x++) {
			int ocol[4];
			int icol[4];

			icol[XSAI_A] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_B] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_C] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			icol[XSAI_D] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];

			if((y-1) >=      0) icol[XSAI_A] = ((int *)src->pixels)[((y-1)) * src->w + ((x+0))];
			if((x+1) <  src->w) icol[XSAI_B] = ((int *)src->pixels)[((y+0)) * src->w + ((x+1))];
			if((x-1) >=      0) icol[XSAI_C] = ((int *)src->pixels)[((y+0)) * src->w + ((x-1))];
			if((y+1) <  src->h) icol[XSAI_D] = ((int *)src->pixels)[((y+1)) * src->w + ((x+0))];

			ocol[XSAI_1] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			ocol[XSAI_2] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			ocol[XSAI_3] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];
			ocol[XSAI_4] = ((int *)src->pixels)[((y+0)) * src->w + ((x+0))];

			if(icol[XSAI_C] == icol[XSAI_A] && icol[XSAI_C] != icol[XSAI_D] && icol[XSAI_A] != icol[XSAI_B]) ocol[XSAI_1] = icol[XSAI_A];
			if(icol[XSAI_A] == icol[XSAI_B] && icol[XSAI_A] != icol[XSAI_C] && icol[XSAI_B] != icol[XSAI_D]) ocol[XSAI_2] = icol[XSAI_B];
			if(icol[XSAI_B] == icol[XSAI_D] && icol[XSAI_B] != icol[XSAI_A] && icol[XSAI_D] != icol[XSAI_C]) ocol[XSAI_4] = icol[XSAI_D];
			if(icol[XSAI_D] == icol[XSAI_C] && icol[XSAI_D] != icol[XSAI_B] && icol[XSAI_C] != icol[XSAI_A]) ocol[XSAI_3] = icol[XSAI_C];

			for(i = 0; i < 2; i++) {
				for(j = 0; j < 2; j++) {
					((int *)dst->pixels)[((y<<1) + i) * dst->w + ((x<<1) + j)] = ocol[i * 2 + j];
				}
			}
		}
	}
}

FILE *chld_rom(SDL_Surface *sdl_screen)
{
	SDL_Event sdl_event;

	DIR *dir = opendir("./roms/");

	int i;
	for (i = 0; i < 512; i++) {
		roms[i] = 0;
	}

	readdir(dir);
	readdir(dir);

	for (i = 0; i < 512; i++) {
		struct dirent *p = readdir(dir);
		if (!p) break;
		roms[i] = (struct dirent *)malloc(sizeof(struct dirent));
		memcpy(roms[i], p, sizeof(struct dirent));
	}

	int j = 0;

	int k1 = 0, k2 = 0;
	int l = 0;

	while (1) {
		adjustFPS();

		memset(sdl_screen->pixels, 0, sdl_screen->pitch*sdl_screen->h);

		if (poll_event(&sdl_event)) {
			SDL_Quit();
			exit(0);
		}

		libgpu_puts(sdl_screen, 0, 0, 0xffffff, "Please select the ROM.");

		for (i = l; i < 512; i++) {
			if (!roms[i]) break;

			libgpu_puts(sdl_screen, 12, (i - l) * 8 + 16, 0xffffff, roms[i]->d_name);
		}

		libgpu_puts(sdl_screen, 0, j * 8 + 16, 0xffffff, ">");

		Uint8 *keystate = SDL_GetKeyState(NULL);

		if (keystate[SDLK_UP]) {
			if (k1 == 0) j--;
			k1 = 1;
		}
		else {
			k1 = 0;
		}
		if (keystate[SDLK_DOWN]) {
			if (k2 == 0) j++;
			k2 = 1;
		}
		else {
			k2 = 0;
		}
		if (keystate[SDLK_RETURN]) break;

		if (j < 0 && l > 0) l--;
		if (j > 21 && l < 512) l++;

		if (j < 0) j = 0;
		if (j > 21) j = 21;
		if (j >= i - l) j = i - l - 1;

		if (ini_x2s == 1) {
			if (ini_hq != 0) SDL_sx2Surface(sdl_screen_m, sdl_screen);
			else SDL_x2Surface(sdl_screen_m, sdl_screen);
			SDL_UpdateRect(sdl_screen_m, 0, 0, 0, 0);
		}
		else {
			SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
		}
	}

	char romname[256];
	sprintf(romname, "roms/%s", roms[j + l]->d_name);
	memset(sdl_screen->pixels, 0, sdl_screen->pitch*sdl_screen->h);

	return fopen(romname, "rb");
}

void v30_execute(e8086_t *v30)
{
	int elim = e86_get_clock(v30);

	e86_clock(v30,ini_mhz*1000*1000/FPS);

	sclk += e86_get_clock(v30)-elim;
}

int v30_loadrom(e8086_t *v30)
{
	FILE *romfile;

	romfile = chld_rom(sdl_screen);
	if (!romfile) {
		printf("fatal: Unable to load program!\n");
		exit(-1);
	}
	
	fread(mem_v30+0x10000,1,65536,romfile);
	fclose(romfile);

	e86_set_cs(v30,0x1000);
	e86_set_ip(v30,0x0000);
	e86_set_sp(v30,0xfffe);

	return 1;
}

int main(int argc, char **argv) {
	SDL_Event sdl_event;
	e8086_t* v30;
	int retval;
	
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)) exit(1);
	
	SDL_WM_SetCaption("RGi86",NULL);
	
	SDL_WM_SetIcon(SDL_LoadBMP("favicon.bmp"),NULL);
	
	libini_iniopen("config.ini");

	ini_x2s = libini_iniread_n("x2surface");
	ini_hq = libini_iniread_n("scale2x"); 
	ini_mhz = libini_iniread_n("cpucycle");
	ini_sblen = libini_iniread_n("soundbuffer");
	
	puts("RGi86-git " __DATE__ " build by kagura1050");

	// <cpuinfo>
	printf("CPU Extensions : ");
	if(SDL_HasRDTSC())		printf("RDTSC ");
	if(SDL_HasMMX())		printf("MMX ");
	if(SDL_HasMMXExt())		printf("MMX+ ");
	if(SDL_Has3DNow())		printf("3DNow ");
	if(SDL_Has3DNowExt())	printf("3DNow+ ");
	if(SDL_HasSSE())		printf("SSE ");
	if(SDL_HasSSE2())		printf("SSE2 ");
	if(SDL_HasAltiVec())	printf("AltiVec ");
	printf("\n");
	
	if(ini_x2s == 1) {
		sdl_screen_m = SDL_SetVideoMode(SCRN_X*2,SCRN_Y*2,32,SDL_SWSURFACE);
		sdl_screen = SDL_CreateRGBSurface(SDL_SWSURFACE,320,200,32,0xff0000,0x00ff00,0x0000ff,0xff0000);
	} else {
		sdl_screen = SDL_SetVideoMode(SCRN_X,SCRN_Y,32,SDL_SWSURFACE);
	}

	v30 = e86_new();
	
	mem_v30 = (unsigned char *)malloc(1024*1024);
	
	libgpu_init();
	init_audio();
	init_graphic(mem_v30, sdl_screen);

	e86_init(v30);
	e86_set_8086(v30);

	v30_setup_callbacks(v30);
	e86_reset(v30);
	
	v30_loadrom(v30);
	
	int fc = SDL_GetTicks();
	int frame = 0;
	int sframe = 0;
	int stime = SDL_GetTicks();
	int errfps = 0;

	while(!poll_event(&sdl_event)) {
		refresh_graphic(sdl_screen, mem_v30);
		if(ini_x2s == 1) {
			if(ini_hq != 0) SDL_sx2Surface(sdl_screen_m,sdl_screen);
			else SDL_x2Surface(sdl_screen_m,sdl_screen);
			SDL_UpdateRect(sdl_screen_m,0,0,0,0);
		} else {
			SDL_UpdateRect(sdl_screen,0,0,0,0);
		}
		frame++;
		
		Uint8 *keystate = SDL_GetKeyState(NULL);

		if ((keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) && keystate[SDLK_o]) {
			init_audio();
			init_graphic(mem_v30, sdl_screen);

			e86_reset(v30);

			v30_loadrom(v30);
		}

		if ((keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) && keystate[SDLK_r]) {
			init_audio();
			init_graphic(mem_v30, sdl_screen);

			e86_reset(v30);

			e86_set_cs(v30,0x1000);
			e86_set_ip(v30,0x0000);
			e86_set_sp(v30,0xfffe);
		}

		v30_execute(v30);
		adjustFPS();

		if((SDL_GetTicks() - fc) >= 1000) {
			fc = SDL_GetTicks();
			char fss[256];
			sprintf(fss,"Xstation86 - %d fps %.1f MHz",frame,(float)sclk/1000.0f/1000.0f);
			SDL_WM_SetCaption(fss,NULL);
			frame = 0;
			sclk = 0;
		}

		sframe++;
	}
}
