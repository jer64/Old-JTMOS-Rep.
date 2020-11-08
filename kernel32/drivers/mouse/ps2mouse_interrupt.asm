;-----------------------------------------------------------
; PS/2 Mouse Low Level ISR Routine
;-----------------------------------------------------------
bits 32
section .text
global ps2mouse_asmirqhandler12
extern ps2mouse_irqhandler12
extern storesegregs
extern restoresegregs
%include "core/lowlevel/corner.mac"
%include "core/lowlevel/segsave.mac"

		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0
		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0
ps2mouse_junkstack:
		dd 0,0

ps2mouse_asmirqhandler12:
		cld
		pushad
		; Push previous segment regs
		S_SEGREGS
		;
		mov ax,0x10
		mov ds,ax
		mov es,ax

		CornerIndicator 2,'$'

		; Call C-language programmed side of the IRQ-handler
		call ps2mouse_irqhandler12

		CornerIndicator 2,' '

		;
		R_SEGREGS
		popad
		iret
;-----------------------------------------------------------------------------------------------
