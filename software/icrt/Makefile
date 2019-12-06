MAKE		= make -r
WASM		= wasm

default :
	$(MAKE) crt

crt : icrt0.s Makefile
	$(WASM) -bt=dos -0r -mt -fo=icrt0.o icrt0.s
