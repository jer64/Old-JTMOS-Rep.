;-------------------------------------------------------------
; HARD DISK IRQ HANDLER
; (C) 2002-2003 by Jari Tuominen(jari.tuominen@kanetti.net)
;-------------------------------------------------------------
bits 32
section .text
global hdLowIRQ14
global hdLowIRQ15
extern hdIRQ14
extern hdIRQ15
extern storesegregs
extern restoresegregs
%include "core/lowlevel/corner.mac"
%include "core/lowlevel/segsave.mac"

;-------------------------------------------------------------------------------
hdLowIRQ14:
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
		call hdIRQ14
		pop edi

		CornerIndicator 6,' '

		;
                R_SEGREGS
		popad
		iret
;-------------------------------------------------------------------------------
hdLowIRQ15:
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
		call hdIRQ15

		CornerIndicator 8,' '

		;
		R_SEGREGS
		popad
		iret
;-------------------------------------------------------------------------------





