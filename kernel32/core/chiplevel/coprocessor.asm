;----------------------------------------------------------------
global ASMexception_7
global fpu_save
global fpu_restore
extern coprocessor_not_found
;----------------------------------------------------------------
; Coprocessor not found:
; Programmed specifically for TSS switching.
ASMexception_7:
                ;
                cli     ; Disable interrupts
                cld     ; Clear direction flag
                pushad  ; Push all general registers
                ; Push all segment registers
                push ds
                push es
                push fs
                push gs
                ; Get valid segment,
                ; kernel mode, can access
                ; entire memory space.
                ; Used only temporarily.
                ; (CODE32,DATA32 => 0x08,0x10)
                mov ax,0x10
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax

		; Allow FPU usage from now on.
		clts

		; Call C based handler.
		call coprocessor_not_found

                ; Pull back
                ;
                pop gs
                pop fs
                pop es
                pop ds
                ;
                popad
                ;
		iret
;----------------------------------------------------------------
fpu_save:
		;
		push ebp
		mov ebp,esp
		mov edi,[ebp+8]

		; Save FPU registers.
		fsave [ds:edi]
			
		;
		mov esp,ebp
		pop ebp
		ret
;----------------------------------------------------------------
fpu_restore:
		;
		push ebp
		mov ebp,esp
		mov edi,[ebp+8]

		; Restore FPU registers.
		frstor [ds:edi]
			
		;
		mov esp,ebp
		pop ebp
		ret
;----------------------------------------------------------------


