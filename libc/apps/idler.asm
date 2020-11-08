bits 32

; Idle and do nothing
start:
		int 0x7E
		jmp start
