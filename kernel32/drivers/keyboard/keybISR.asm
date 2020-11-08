bits 32
section .text
global keyb_ISR
extern keyb_handler
extern storesegregs
extern restoresegregs
%include "core/lowlevel/segsave.mac"
%include "core/lowlevel/corner.mac"

		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
keyb_junkstack:
		dd 0,0

keyb_ISR:
                ;
                cld
                pushad
                ; Push previous segment regs
		S_SEGREGS
                ;
                mov ax,0x10
                mov ds,ax
                mov es,ax

		CornerIndicator 4,'%'

		;
		call keyb_handler

		CornerIndicator 4,' '

                ;
		R_SEGREGS
                popad
		iret

;
