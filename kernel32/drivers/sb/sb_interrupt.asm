;-----------------------------------------------------------
; sb_interrupt.asm SOUND BLASTER INTERRUPT HANDLING
; (C) 2002,2017 by Jari Tuominen(jari.t.tuominen@gmail.com)
;-----------------------------------------------------------
bits 32
section .text
global sb_ISR
extern sb_irqhandler
extern storesegregs
extern restoresegregs

%include "core/lowlevel/segsave.mac"

;
		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
sbint_junkstack:
		dd 0,0,0,0

;
sb_ISR:
                cld
                pushad
                ; Push previous segment regs
		S_SEGREGS

		mov ax,0x10
		mov ds,ax
		mov es,ax

		; Call C-language programmed side of the IRQ-handler
		call sb_irqhandler

		;
		R_SEGREGS
		popad
		iret
;-----------------------------------------------------------------------------------------------
