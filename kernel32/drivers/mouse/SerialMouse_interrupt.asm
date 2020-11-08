;--------------------------------------------------------------
bits 32
section .text
global SerialMouse_asmirqhandler4
extern SerialMouse_irqhandler4
global SerialMouse_asmirqhandler3
extern SerialMouse_irqhandler3
extern storesegregs
extern restoresegregs
%include "core/int/corner.mac"
%include "core/int/segsave.mac"

			dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0
			dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0
SerialMouse_junkstack:
			dd 0,0

; FOR SERIAL MOUSE COM1/IRQ4:
SerialMouse_asmirqhandler4:
				cld
				pushad
				; Push previous segment regs
				S_SEGREGS
				;
				mov ax,0x10
				mov ds,ax
				mov es,ax

				; ---- CODE HERE: ------------------
				call SerialMouse_irqhandler4
				; ----------------------------------				

				;
				R_SEGREGS
				popad
				iret

; FOR SERIAL MOUSE COM2/IRQ3:
SerialMouse_asmirqhandler3:
				cld
				pushad
				; Push previous segment regs
				S_SEGREGS
				;
				mov ax,0x10
				mov ds,ax
				mov es,ax

				; ---- CODE HERE: ------------------
				call SerialMouse_irqhandler3
				; ----------------------------------				

				;
				R_SEGREGS
				popad
				iret

;-----------------------------------------------------------------------------------------------
