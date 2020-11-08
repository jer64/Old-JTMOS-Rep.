;--------------------------------------------------------------------------------------------
; systemcall caller function for libc
; (client)
;
; parameters:
; [CALL_NUMBER = EAX],EBX,ECX,EDX,ESI,EDI,EBP
;--------------------------------------------------------------------------------------------
global scall

;
scall:
		push ebp
		mov ebp,esp
		;
		push 0x10
		pop  ds
		inc dword [ds:counter_scall1]
		;
		pushad
                mov eax,dword [ebp+8]
                mov ebx,dword [ebp+12]
                mov ecx,dword [ebp+16]
                mov edx,dword [ebp+20]
                mov esi,dword [ebp+24]
                mov edi,dword [ebp+28]
		mov ebp,dword [ebp+32]
		int 0x7F
		;
		push 0x10
		pop  ds
		;
		mov dword [ds:scall_retval],eax
		popad

		;
		inc dword [ds:counter_scall2]
		;
		pop ebp
		mov eax,dword [ds:scall_retval]
		ret

counter_scall1	dd 0
counter_scall2	dd 0

scall_retval	dd 0

;

