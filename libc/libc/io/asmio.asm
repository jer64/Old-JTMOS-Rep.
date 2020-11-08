;--------------------------------------------------
; LIBC assembly language based functions
;
; (C)2001,2002 by Jari Tuominen(ehc50@kanetti.net)
;--------------------------------------------------
bits 32
section .text
;
global outportd
global inportd
global idle_moment

;
; idle_moment
;
; Sleeps a moment, other threads will take
; the CPU in control meanwhile.
; Note: uses system call #1
;
idle_moment:
		; system call #1: relax_thread(idle)
		mov eax,1
		int 0xc0
		ret

;//void outportd(DWORD portad,DWORD value);
;//DWORD inportd(DWORD portad);

; void outportd(DWORD portad,DWORD value);
;
outportd:
                push ebp
                mov ebp,esp
                ; port
                mov edx,dword [ebp+8]
		; data
		mov eax,dword [ebp+12]
		out dx,eax
		;
		pop ebp
		ret

; DWORD inportd(DWORD portad);
;
inportd:
                push ebp
                mov ebp,esp
                ;
                mov edx,dword [ebp+8]
                in  eax,dx
                ;
                pop ebp
                ret

;

