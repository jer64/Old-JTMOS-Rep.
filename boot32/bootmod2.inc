                                ; Correct CS:IP - So that org 0x0100 works
                                jmp 0x7b0:0x105
begin:                          ;
                                ; Make a dummy stack (Microsoft style :-)
                                cli
                                xor cx,cx
                                mov ss,cx
                                mov ax,0x7b7c
                                mov sp,ax
                                sti


;
;	Any key handler
				puts str_nonsysdsk
				mov ax,0
				int 0x16
				; INT 19 - Bootstrap Loader
				; DL = physical drive where boot sector is located
				; (We default to drive A: for now)
                                mov dl,0
                                int 0x19
				; Nothing will be here:

