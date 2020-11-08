; Stack
STACK16_SIZE equ 256 ; real
STACK32_SIZE equ 256 ; prot.
stack16:        times STACK16_SIZE db 0  ; 16-bit Real Mode stack
stack16_end: dw 0
