;-----------------------------------------------------
; gus_interrupt.asm ULTRASOUND INTERRUPT HANDLING
; (C) 2003 by Jari Tuominen(jari.tuominen@kanetti.net)
;-----------------------------------------------------
bits 32
section .text
global gus_ISR
extern gus_irqhandler
extern storesegregs
extern restoresegregs

%include "core/int/segsave.mac"

;
		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
gusint_junkstack:
		dd 0,0,0,0

;
gus_ISR:
                cld
                pushad
                ; Push previous segment regs
		S_SEGREGS

		mov ax,0x10
		mov ds,ax
		mov es,ax

		; Call C-language programmed side of the IRQ-handler
		call gus_irqhandler

		;
		R_SEGREGS
		popad
		iret
;-----------------------------------------------------------------------------------------------
