global fardcall

; Far Device Call
; EPI,CS, P1,P2,P3,P4,P5,P6,P7
fardcall:
		;
		mov ebp,esp

		; Get jump offset
		mov eax,dword [ebp+8]
		mov dword [jumpoffs],eax

		; Get code segment selector
		mov eax,dword [ebp+12]
		mov word [jumpseg],ax

		; Get parameters 1-7
		mov eax,dword [ebp+16]
		mov dword [r1],eax
		mov eax,dword [ebp+20]
		mov dword [r2],eax
		mov eax,dword [ebp+24]
		mov dword [r3],eax
		mov eax,dword [ebp+28]
		mov dword [r4],eax
		mov eax,dword [ebp+32]
		mov dword [r5],eax
		mov eax,dword [ebp+36]
		mov dword [r6],eax
		mov eax,dword [ebp+40]
		mov dword [r7],eax

		; Save state
		pushf
		; Mask interrupts
		cli

		; Push parameters
		push dword [r7]
		push dword [r6]
		push dword [r5]
		push dword [r4]
		push dword [r3]
		push dword [r2]
		push dword [r1]

		; 00000000  9A, 7B,E3,E2,FC, 34,12    call 0x1234:0xfce2e37b
		; Do a far call [CALL SSSS : OOOO OOOO]
		db 0x9A
jumpoffs	dd 0
jumpseg		dw 0

		; Restore stack
		add esp,7*4
		; Restore flags state
		popf

		;
		ret
		






r1		dd 0
r2		dd 0
r3		dd 0
r4		dd 0
r5		dd 0
r6		dd 0
r7		dd 0

