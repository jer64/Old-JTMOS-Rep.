;-----------------------------------------------------------------------------------------------
bits 32
section .text
global serial_asmirqhandler4
extern serial_irqhandler4
global serial_asmirqhandler3
extern serial_irqhandler3
extern storesegregs
extern restoresegregs
%include "core/int/segsave.mac"
%include "core/int/corner.mac"

;-----------------------------------------------------------------------------------------------
                        dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
serialint_junkstack4:
                        dd 0,0,0,0

serial_asmirqhandler4:
		;
		cld
		pushad
		; Push previous segment regs
		S_SEGREGS
                ;
                mov ax,0x10
		mov ds,ax
		mov es,ax

                ; Call C-language programmed side of the IRQ-handler
		push edi
                call serial_irqhandler4
		pop edi

		;
		R_SEGREGS
                popad
                iret
;-----------------------------------------------------------------------------------------------
                        dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
serialint_junkstack3:
                        dd 0,0,0,0

serial_asmirqhandler3:
		;
		cld
		pushad
		; Push previous segment regs
		S_SEGREGS
                ;
                mov ax,0x10
		mov ds,ax
		mov es,ax

                ; Call C-language programmed side of the IRQ-handler
		push edi
                call serial_irqhandler3
		pop edi

		;
		R_SEGREGS
                popad
                iret
;-----------------------------------------------------------------------------------------------

