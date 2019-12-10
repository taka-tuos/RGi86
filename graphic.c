#include "RGi86.h"
#include "libgpu.h"

unsigned char *font6x8;

uint32_t rgb332_table[256];

void init_graphic(unsigned char *mem_v30, SDL_Surface *sdl_screen)
{
	int r,g,b;
	
	for(r = 0; r < 8; r++) {
		for(g = 0; g < 8; g++) {
			for(b = 0; b < 4; b++) {
				int r8, g8, b8;

				r8 = (r << 5) | (r << 2) | (r >> 1);
				g8 = (g << 5) | (g << 2) | (g >> 1);
				b8 = (b << 6) | (b << 4) | (b << 2) | b;
			
				if(r8 > 255) r8 = 255;
				if(g8 > 255) g8 = 255;
				if(b8 > 255) b8 = 255;

				rgb332_table[(r << 5) | (g << 2) | b] = (r8 << 16) | (g8 << 8) | b8;
			}
		}
	}
	
	FILE *fp0 = fopen("6x8.dat","rb");

	font6x8 = (char *)malloc(2048);

	fread(font6x8,sizeof(char),2048,fp0);
	
	fclose(fp0);
}

void refresh_graphic(SDL_Surface *sdl_screen, unsigned char *mem_v30)
{
	memset(sdl_screen->pixels,0,320*200*4);
	int x, y;
	
	for(y = 0; y < SCRN_Y/8; y++) {
		for(x = 0; x < SCRN_X/8; x++) {
			int pix = ((uint8_t *)(mem_v30+0xb8000))[y * (SCRN_X/8) + x];
			libgpu_putc(sdl_screen,x*8,y*8,0xffffff,font6x8 + pix * 8);
		}
	}
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++) {
			if(x >= 0 && x < 320 && y >= 0 && y < 200)vram[y * xsize + x] = c;
		}
	}
	return;
}

void putfont8x8(unsigned char *vram, int xsize, int x, int y, unsigned char c, char *font)
{
	int i;
	unsigned char *p, d;
	
	for (i = 0; i < 8; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	
	return;
}

void putfonts8x8(unsigned char *vram, int xsize, int x, int y, char c, char *s)
{
	for (; *s != 0x00; s++) {
		putfont8x8(vram, xsize, x, y, c, font6x8 + *s * 8);
		x += 6;
	}
	return;
}
