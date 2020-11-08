;=====================================================================================
; int 0x7F
; JTMOS SYSTEM CALL HANDLER (for incoming system calls).
; (C) 2001-2005 by Jari Tuominen(jari.tuominen@kanetti.fi).
;
; 30.9.2003: Added system call tracing option.
;=====================================================================================
extern nr_scall_func
extern scall_functions
extern relax_thread
extern scallDeactive
extern scallActive
extern GetCurrentThreadBase
extern thread_base
extern nr_curthread
global scall_dummy
global scall_handler
%include "core/lowlevel/segsave.mac"
%include "core/lowlevel/corner.mac"


			dd 0,0,0,0,0,0,0,0,0,0,0,0
			dd 0,0,0,0,0,0,0,0,0,0,0,0
			dd 0,0,0,0,0,0,0,0,0,0,0,0
			dd 0,0,0,0,0,0,0,0,0,0,0,0
junkstack2:
			dd 0,0,0,0,0,0,0,0,0,0,0,0



; All neccessary registers will be saved & restored.
;
scall_handler:
		; ----------------------------------------
		; ENTER THE SYSTEM CALL HANDLER:
		cld
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		push ebp

		;
		S_SEGREGS
		SaneSelectors

		; ------------------------------------------
		; Store parameters
		mov dword [ds:scall_eax],eax
		mov dword [ds:scall_ebx],ebx
		mov dword [ds:scall_ecx],ecx
		mov dword [ds:scall_edx],edx
		mov dword [ds:scall_esi],esi
		mov dword [ds:scall_edi],edi
		mov dword [ds:scall_ebp],ebp
		;


		;-------------------------------------------
		; CHANGE STACK ...
		;-------------------------------------------
		

		;-------------------------------------------
		; Let's make the call!
		;
		; Get the call address
		mov edi,eax
		shl edi,2 ; x4
		add edi,scall_functions
		mov eax,dword [ds:edi]
		mov dword [ds:scall_tmp1],eax

		;
		mov eax,dword [ds:scall_eax]
                mov ebx,dword [ds:scall_ebx]
                mov ecx,dword [ds:scall_ecx]
                mov edx,dword [ds:scall_edx]
                mov esi,dword [ds:scall_esi]
                mov edi,dword [ds:scall_edi]
                mov ebp,dword [ds:scall_ebp]
		push ebp
		push edi
		push esi
		push edx
		push ecx
		push ebx
		push eax
		;
		mov edx,dword [ds:scall_tmp1]
		mov ebp,esp
		; CALL TO THE SYSTEM CALL:
		call edx
		add esp,7*4
		;mov dword [gs:scall_retval1],eax
		;
scall_handler_l1:
		
		;
		pushad
		call scallDeactive
		popad

		;-------------------------------------------

		;
		R_SEGREGS

                ; Pull back all registers except EAX...
		; (emulate popd)
		pop ebp
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop ebx
		iret
;   Push(EAX);
;   Push(ECX);
;   Push(EDX);
;   Push(EBX);
;   Push(Temp);
;   Push(EBP);
;   Push(ESI);
;   Push(EDI);
;-----------------------------------------------------------------
tbase:
		;
		mov esi,thread_base
		mov edi,DWORD [DS:nr_curthread]
		shl edi,2
		add esi,edi
		mov eax,DWORD [DS:ESI]
		ret
;-----------------------------------------------------------------
scall_retval1:	dd 0
scall_tmp1	dd 0,0,0,0
;-----------------------------------------------------------------
; TEMPORARY VARIABLES
_scall_eax	dd 0
_scall_ebx	dd 0
_scall_ecx	dd 0
_scall_edx	dd 0
_scall_esi	dd 0
_scall_edi	dd 0
_scall_ebp	dd 0
;-----------------------------------------------------------------
scall_eax       dd 0
scall_ebx       dd 0
scall_ecx       dd 0
scall_edx       dd 0
scall_esi       dd 0
scall_edi       dd 0
scall_ebp       dd 0
;-----------------------------------------------------------------
; dummy code
scall_dummy:
		ret
		; dummy data
		db 0,0,0,0
;-----------------------------------------------------------------
