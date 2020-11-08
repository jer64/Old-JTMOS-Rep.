;-------------------------------------------------------------
; HARD DISK IRQ HANDLER
; (C) 2002 by Jari Tuominen(jari.tuominen@kanetti.net)
;-------------------------------------------------------------
bits 32
section .text
global dhd_asmirqhandler14
global dhd_asmirqhandler15
extern dhd_irqhandler14
extern dhd_irqhandler15
extern storesegregs
extern restoresegregs
%include "heart/int/corner.mac"
%include "heart/int/segsave.mac"

;
			dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dhd_junkstack14:
			dd 0,0,0,0

;
			dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
dhd_junkstack15:
			dd 0,0,0,0

dhd_asmirqhandler14:
		;
		cld
		pushad
		; Push previous segment regs
		S_SEGREGS
		;
		mov ax,0x10
		mov ds,ax
		mov es,ax

		CornerIndicator 6,'@'

		; Call C-language programmed side of the IRQ-handler
		push edi
		call dhd_irqhandler14
		pop edi

		CornerIndicator 6,' '

		;
                R_SEGREGS
		popad
		iret

dhd_asmirqhandler15:
		;
		cld
		pushad
		; Push previous segment regs
		S_SEGREGS
		;
		mov ax,0x10
		mov ds,ax
		mov es,ax

		CornerIndicator 8,'£'

		; Call C-language programmed side of the IRQ-handler
		call dhd_irqhandler15

		CornerIndicator 8,' '

		;
		R_SEGREGS
		popad
		iret
;-----------------------------------------------------------------------------------------------
