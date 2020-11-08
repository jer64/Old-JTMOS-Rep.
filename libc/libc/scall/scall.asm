;
; systemcall caller function for libc
;
; parameters:
; CALL_NUMBER(EAX),EBX,ECX,EDX,ESI,EDI,EBP
;
global scall

;
scall:
		push ebp
		mov ebp,esp
		;
		pushad
                mov eax,dword [ebp+8]
                mov ebx,dword [ebp+12]
                mov ecx,dword [ebp+16]
                mov edx,dword [ebp+20]
                mov esi,dword [ebp+24]
                mov edi,dword [ebp+28]
		mov ebp,dword [ebp+32]
		int 0x7f
		mov dword [scall_retval],eax
		popad
		;
		pop ebp
		mov eax,dword [scall_retval]
		ret

scall_retval	dd 0

;

