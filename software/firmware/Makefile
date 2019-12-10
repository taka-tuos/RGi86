TARGET = firmware
OBJS_TARGET = main.o xprintf.o
CFLAGS = -s -wx -d0 -fr -ms -zl -i=${WATCOM}/h
ASFLAGS = -bt=dos -0r -mt
LFLAGS = FORMAT DOS COM OPTION MAP=$(TARGET).map, NODEFAULTLIBS, START=crtmain, DOSSEG LIBRARY ${WATCOM}/lib286/dos/clibm.lib

include Makefile.in
