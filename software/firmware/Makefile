TARGET = firmware
OBJS_TARGET = main.o
CFLAGS = -s -wx -d0 -fr -ms -zl
ASFLAGS = -bt=dos -0r -mt
LFLAGS = FORMAT DOS OUTPUT RAW OFFSET=0x0000 OPTION NODEFAULTLIBS, START=crtmain, DOSSEG

include Makefile.in
