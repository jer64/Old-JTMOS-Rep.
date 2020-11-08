;====================================================================
;
; SYSTEM LOADER:
; HARD DISK DETECTION CODE
; Detects hard disk parameters for the kernel in
; the boot procedure.
; Information will be stored at the system area at 0x1000.
;
; Area description:
; 1000		WORD	KERNEL CHECKSUM I
; 1002		WORD	KERNEL CHECKSUM II
; (Drive 0x80)
; 1010		WORD	CYLINDERS
; 1012		WORD	SECTORS
; 1014		BYTE	HEADS
; 1015		BYTE	DRIVES ATTACHED
; 1016		BYTE	CMOS TYPE
;
; (C) 2002 by Jari Tuominen(ehc50@kanetti.net)
;
;====================================================================
detecthd:
		;
reset80:
		mov ah,0
		mov dl,0x80
		int 0x13
		dec byte [resetcounter]
		jnz reset80
		;
		mov ah,8
		mov dl,0x80
		int 0x13
		jc detecthd_error
		;
		mov ax,0
		mov ah,cl
		shr ah,6
		mov al,ch
		and cx,0x3f
		; AX = CYLINDERS
		; CX = SECTORS
		; DH = HEADS
		; DL = DRIVES ATTACHED
                ;
		; Get offset at 0x1002
		mov bp,0x100
		mov es,bp
                mov bp,0x10
                mov word [es:bp],ax
                add bp,2
                mov word [es:bp],cx
                add bp,1
                mov byte [es:bp],dh
                add bp,1
                mov byte [es:bp],dl
                add bp,1
                mov byte [es:bp],bl
		ret
detecthd_error:
                push word 0x100
                pop es
		mov ax,0
                mov bp,0x10
                mov word [es:bp],ax
                add bp,2
                mov word [es:bp],ax
                add bp,1
                mov byte [es:bp],al
                add bp,1
                mov byte [es:bp],al
                add bp,1
                mov byte [es:bp],al
		ret
;
resetcounter	db 2
;


