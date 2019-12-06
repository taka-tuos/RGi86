TARGET		= RGi86
OBJS_TARGET	= RGi86.o libgpu.o libini.o sdl.o graphic.o callback.o audio.o \
			e8086/disasm.o e8086/e8086.o e8086/e80186.o e8086/e80286r.o e8086/ea.o \
			e8086/flags.o e8086/opcodes.o e8086/pqueue.o

CFLAGS += -g -O0 `sdl-config --cflags`
LIBS += `sdl-config --libs` -lm -lc

include Makefile.in
