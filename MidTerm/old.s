        OSSEG  = 0x1000
	
       .globl _main,_running,_scheduler, _printStack
       .globl _proc, _procSize
       .globl _tswitch,_resume
				.globl _getbp
        jmpi   start,OSSEG
_getbp:
        mov ax, bp
        ret
start:	mov  ax,cs
	mov  ds,ax
	mov  ss,ax
        mov  es,ax
	mov  sp,#_proc
	add  sp,_procSize
		
	call _main

_tswitch:
SAVE:	
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	pushf
	call _printStack
	mov  bx,_running
	mov  2[bx],sp

FIND:	call _scheduler

_resume:	
RESUME:
	mov  bx,_running
	mov  sp,2[bx]
	call _printStack
	popf
	pop  di
call _printStack
	pop  si
	pop  bp
call _printStack
	pop  dx
	pop  cx
	pop  bx
call _printStack
	pop  ax
	call _printStack
	ret
