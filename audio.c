#include "RGi86.h"
#include <math.h>

extern int ini_sblen;

void callback_pcm(void *unused, Uint8 *stream, int len);

SDL_AudioSpec Desired;
SDL_AudioSpec Obtained;

void init_audio(void)
{
	Desired.freq= 44100; /* Sampling rate: 22050Hz */
	Desired.format= AUDIO_S16; /* 16-bit signed audio */
	Desired.channels= 1; /* Mono */
	Desired.samples= ini_sblen; /* Buffer size: 2205000B = 10 sec. */
	Desired.callback = callback_pcm;
	Desired.userdata = NULL;

	SDL_OpenAudio(&Desired, &Obtained);
	SDL_PauseAudio(0);
}

void callback_pcm(void *unused, Uint8 *stream, int len)
{
	int i;
	static unsigned int step = 0;
	Sint16 *frames = (Sint16 *) stream;
	int framesize = len / 2;
	for (i = 0; i < framesize; i++, step++) {
		frames[i] = 0;
	}
}
