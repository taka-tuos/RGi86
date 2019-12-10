.8086

_TEXT	segment byte use16 'CODE'

public	crtmain
extrn	kernel_main_ : proc
	assume cs:_TEXT, ds:_DATA
crtmain proc
	mov		ax,cs
	mov		ds,ax
	mov		es,ax
	
	mov		ax,seg _STACK
	mov		ss,ax
	mov		sp,0xfffe
	
	call	kernel_main_
crtmain endp

_TEXT	ends

_DATA	segment byte use16 'DATA'
	org	0h
_DATA	ends

_STACK	segment byte use16 'STACK'
_STACK	ends

end crtmain
