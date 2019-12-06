#include "RGi86.h"

extern unsigned char *mem_v30;

#define RIO_WBUF(dat) \
	if(size == 1) *((unsigned char *)buf) = (dat); \
	if(size == 2) *((unsigned short *)buf) = (dat); \

#define WIO_RBUF(dat) \
	if(size == 1) (dat) = *((unsigned char *)buf); \
	if(size == 2) (dat) = *((unsigned short *)buf); \

#define WIO_RBUF2 (size == 1 ? *((unsigned char *)buf) : *((unsigned short *)buf))

void v30_read_memory(void* _ctx,unsigned int address,unsigned char *buf,int size)
{
	memcpy(buf,mem_v30+(address&0xfffff),size);
}

/* called when writing memory */
void v30_write_memory(void* _ctx,unsigned int address,unsigned char *buf,int size)
{
	//if(address >= 0xb8000 && address < 0xb9000) printf("DEBUG : %05x = %04x\n",address,WIO_RBUF2);
	memcpy(mem_v30+(address&0xfffff),buf,size);
}

/* called when reading an I/O port */
void v30_read_io(void* _ctx,unsigned int address,unsigned char *buf,int size)
{
	memset(buf,0,size);

	Uint8 *keystate = SDL_GetKeyState(NULL);
	int keybit = 0;

	if(keystate[SDLK_z]) {
		keybit |= 0x01;	// A
	}
	if(keystate[SDLK_x]) {
		keybit |= 0x02;	// B
	}
	if(keystate[SDLK_b]) {
		keybit |= 0x04;	// select
	}
	if(keystate[SDLK_n]) {
		keybit |= 0x08;	// start
	}

	if(keystate[SDLK_UP]) {
		keybit |= 0x10;	// up
	}
	if(keystate[SDLK_DOWN]) {
		keybit |= 0x20;	// down
	}
	if(keystate[SDLK_LEFT]) {
		keybit |= 0x40;	// left
	}
	if(keystate[SDLK_RIGHT]) {
		keybit |= 0x80;	// right
	}

	switch(address) {
	default:
		printf("Unknown port read : 0x%04x\n",address);
		break;
	}
}

/* called when writing an I/O port */
void v30_write_io(void* _ctx,unsigned int address,unsigned char *buf,int size)
{
	switch(address) {
	default:
		printf("Unknown port write : 0x%04x data 0x%04x\n",address,WIO_RBUF2);
		break;
	}
}

unsigned char v30_read_memory_byte(void *_ctx, unsigned long address)
{
	unsigned char buf;
	v30_read_memory(_ctx, address, (unsigned char *)&buf, 1);
	return buf;
}

void v30_write_memory_byte(void *_ctx, unsigned long address, unsigned char data)
{
	v30_write_memory(_ctx, address, (unsigned char *)&data, 1);
}

unsigned short v30_read_memory_word(void *_ctx, unsigned long address)
{
	unsigned short buf;
	v30_read_memory(_ctx, address, (unsigned char *)&buf, 2);
	return buf;
}

void v30_write_memory_word(void *_ctx, unsigned long address, unsigned short data)
{
	v30_write_memory(_ctx, address, (unsigned char *)&data, 2);
}

unsigned char v30_read_io_byte(void *_ctx, unsigned long address)
{
	unsigned char buf;
	v30_read_io(_ctx, address, (unsigned char *)&buf, 1);
	return buf;
}

void v30_write_io_byte(void *_ctx, unsigned long address, unsigned char data)
{
	v30_write_io(_ctx, address, (unsigned char *)&data, 1);
}

unsigned short v30_read_io_word(void *_ctx, unsigned long address)
{
	unsigned short buf;
	v30_read_io(_ctx, address, (unsigned char *)&buf, 2);
	return buf;
}

void v30_write_io_word(void *_ctx, unsigned long address, unsigned short data)
{
	v30_write_io(_ctx, address, (unsigned char *)&data, 2);
}

/* function to set up the callbacks for given cpu */
void v30_setup_callbacks(e8086_t *v30)
{
	v30->mem = v30;
	v30->prt = v30;

	v30->mem_get_uint8 = v30_read_memory_byte;
	v30->mem_get_uint16 = v30_read_memory_word;

	v30->mem_set_uint8 = v30_write_memory_byte;
	v30->mem_set_uint16 = v30_write_memory_word;

	v30->prt_get_uint8 = v30_read_io_byte;
	v30->prt_get_uint16 = v30_read_io_word;

	v30->prt_set_uint8 = v30_write_io_byte;
	v30->prt_set_uint16 = v30_write_io_word;
}
