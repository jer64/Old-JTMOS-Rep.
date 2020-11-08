;-----------------------------------------------------------------------------------------------
bits 32
section .text
global fdc_asmirqhandler6
extern fdc_irqhandler6
extern storesegregs
extern restoresegregs
%include "core/lowlevel/segsave.mac"
%include "core/lowlevel/corner.mac"

                        dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
fdcint_junkstack6:
                        dd 0,0,0,0

fdc_asmirqhandler6:
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
                call fdc_irqhandler6
		pop edi

		;
		R_SEGREGS
                popad
                iret
;-----------------------------------------------------------------------------------------------
