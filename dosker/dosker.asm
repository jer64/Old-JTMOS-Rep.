;=================================================================================
; JTMOS kernel loader for DOS operating system.
; Entrance to the protected mode kernel for the 32-bit JTMOS operating system.
; (C) 2007-2008 by Jari Tuominen.
;=================================================================================
;
[bits 16]
org 0x100 ; 'COM file' start address (Yes, this is [.COM] file actually).

jmp start

%include "../boot32/seg.mac" ;;;; Segment Descriptor Macros
%include "../boot32/a20.inc" ;;;; A20 line setup code
%include "kchecksum.inc"
%include "dosputs.mac" ;;;; (puts,putchar)

;----------------------------------------------------------------------------
; main procedure, this is the entry point
start:
	;
	puts txt_announce

	; Open File Using Handle
	mov ax,cs
	mov ds,ax
	mov dx,fn_dosker32
	mov ah,0x3D
	mov al,0x00
	int 0x21
	jnc file_opened_ok
	jmp terminate_program
file_opened_ok:
	mov word [cs:fhandle],ax

	; Set a FAR pointer at 0x20000.
	mov ax,0x2000
	mov word [cs:kerloadptr_seg],ax
	mov ax,0x0000
	mov word [cs:kerloadptr_offs],ax
load_loop:
	;
	mov ax,word [cs:kerloadptr_seg]
	mov ds,ax
	mov dx,word [cs:kerloadptr_offs]
	mov bx,word [cs:fhandle] ; file handle
	mov cx,0x1000 ; # of bytes to read
	mov ah,0x3F ; "read from file"
	int 0x21
	; error?
	jnc nerror
	jmp terminate_program
nerror:
	cmp ax,0 ; zero bytes read?
	je end_of_file

	push ax
	mov ax,word [cs:kerloadptr_seg]
	mov ds,ax
	mov si,0
	pop cx
calcl:
	mov al,byte [ds:si]
	mov ah,0
	add word [cs:measkerchksum],ax
	inc si
	dec cx
	jnz calcl

	mov ax,word [cs:kerloadptr_seg]
	add ax,0x100
	mov word [cs:kerloadptr_seg],ax

	puts txt_progress

	jmp load_loop
end_of_file:

	; Close File Using Handle
	mov ah,0x3E
	mov bx,word [cs:fhandle]
	int 0x21

	;
	puts txt_ld_chksum_is
	mov ax,word [cs:measkerchksum]
	call phex16
	puts txt_enter

	;
	puts txt_cr_chksum_is
	mov ax,kchecksum
	call phex16
	puts txt_enter

	;
	mov ax,word [cs:kchecksum]
	mov bx,word [cs:measkerchksum]
	cmp ax,bx
	je  checksum_matches
	puts txt_mismatch
	jmp skipsuc
checksum_matches:

	;
	puts txt_success
skipsuc:

	jmp proceed

	puts txt_f1_to_launch

	; BIOS INPUT KEY
	mov ah,0
	int 0x16
	cmp ax,0x3B00	; is it F1? (scancode)
	je  proceed
	puts txt_enter
	jmp terminate_program1

proceed:
	puts txt_enter

	; SUCCESS, kernel loaded at 0x20000 !
	; .........
	jmp kernel_starter

terminate_program:
	puts txt_failure

terminate_program1:
	mov ah,0
	int 0x21
	jmp $

kerloadptr_seg:
	dw 0 ; segment
kerloadptr_offs:
	dw 0 ; offset

measkerchksum:
	dw 0

fhandle:
	dw 0

fn_dosker32:
	db "dosker32.bin",0

kernel_starter:



	; -- VESA job. --
	; 0x100	640x400x8	graphics	yes
	; 0x101	640x480x8	graphics	yes
	; 0x102	800x600x4	graphics	no
	; 0x103	800x600x8	graphics	yes	
	; 0x112	640x480x32	graphics	yes

	; Set Vesa Linear Frame Buffer Pointer To Zero As Default.
	push ds
	mov di,0x4028
	mov ax,0
	mov ds,ax
	mov word [ds:di+0],ax
	mov word [ds:di+2],ax
	pop ds

	; Uncomment line below if you VESA to be disabled.
	jmp vesa_okay

	; Get linear frame buffer address.
	mov	ax,0x0400 ; ES:DI pointer to structure.
	mov	es,ax
	mov	di,0
	mov	ax,0x4F01 ; Get video mode info.
	mov	cx,0x0101 ; Video mode 0x101 (640x480x8).
	int	0x10
	cmp	ah,0
	jne	vesa_error

	; Setup VESA linear frame buffer.
	mov	ax,0x4F02 ; Set VBE mode.
	mov 	bx,0x4101 ; (0x101) 640x480x8 | (0x4000) linear frame buffer.
	int	0x10
	cmp	ah,0
	je	vesa_okay
vesa_error:
	;
	puts	txt_bad_vesa
	jmp	$
vesa_okay:

	; ^^VESA job done.^^




        ; Stop floppy motor.
        mov dx,0x3F2
        mov al,0
        out dx,al
	; Fix keyboard buffer(empty it).
	mov cx,2000
	mov dx,0x60
indeed:
	in al,dx
	dec cx
	jnz indeed


        ; Begin protected mode setup code ...
	mov	ax,cs		; load code-segment into DS and ES
	mov	ds,ax
	mov	es,ax

        mov     ax,cs           ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,dummy_descriptor ; calculate physical address of GDT
        mov     dword [ds:gdt_start+2],eax

        mov     ax,cs           ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,interrupt_0  ; calculate physical address of IDT
        mov     dword [ds:idt_start+2],eax

	; Max. keyboard repeat rate.
	mov	ax,0x0305
	xor	bx,bx
	int	0x16

	; Disable NMI.
	mov al,0x80
	mov dx,0x70
	out dx,al

	; A20 setup.
        call a20

        ; -------- Let's go to the kernel.
	; Disable interrupts.
        cli
        ; Load descriptor tables.
        lgdt    [dword global_descriptor_table]
        lidt    [dword interrupt_descriptor_table]
        ; Enter protected mode.
        mov     eax,cr0
        or      al,1
        mov     cr0,eax

	; Make mandatory jump.
        jmp shortjump0
shortjump0:

        ;
        ;***************************************************
        ; ENTER KERNEL32
        ; 
        ; Setup protected mode stack.
        mov esp,0x7BFF
        mov ax,0x18
        mov ss,ax
        ; Also define rest of the segments, just to prevent
        ; additional errors occuring.
        mov ds,ax
        mov es,ax
        mov fs,ax
        mov gs,ax

	; Make another jump.
        jmp shortjump1
shortjump1:

	; Jump at linear address 0x10000.
	; Jump ssss:oooooooo, 16:32, segment:offset.
	db 0x66,0xEA
	dd 0x00020000
	dw code32_idx
;	db	0xEA            ; opcode for far jump (to set CS correctly)
;	dw	0x10000,code32_idx

txt_bad_vesa:
	db 13,10,'Video BIOS has no VESA 2.0 support.','$'
txt_enter:
	db 13,10,'$'
txt_ld_chksum_is:
	db 13,10,'loaded kernel checksum is $'
txt_cr_chksum_is:
	db 13,10,'correct kernel checksum is $'
txt_announce:
	db 'JTMOS kernel loader for DOS operating system',13,10,'$'
txt_success:
	db 13,10,'kernel loaded successfully',13,10,'$'
txt_f1_to_launch:
	db 13,10,'press F1 to launch kernel: ','$'
txt_mismatch:
	db 13,10,'error: checksum mismatch',13,10,'$'
txt_failure:
	db 13,10,'failed to load kernel',13,10,'$'
txt_progress:
	db '.','$'



phex16:
                                pusha
                                pusha
                                shr ax,8
                                and ax,0xff
                                call phex8
                                popa
                                and ax,0xff
                                call phex8
                                popa
                                ret

; ax = value
phex8:
                                pusha
                                pusha
                                shr ax,4
                                and ax,0x0f
                                call phex4
                                popa
                                and ax,0x0f
                                call phex4
                                popa
                                ret

; ax = value
phex4:
                                and al,0x0f
                                cmp al,0
                                jne n00
                                putchar '0'
                                ret
n00:
                                cmp al,1
                                jne n01
                                putchar '1'
                                ret
n01:
                                cmp al,2
                                jne n02
                                putchar '2'
                                ret
n02:
                                cmp al,3
                                jne n03
                                putchar '3'
                                ret
n03:
                                and al,0x0f
                                cmp al,4
                                jne n04
                                putchar '4'
                                ret
n04:
                                cmp al,5
                                jne n05
                                putchar '5'
                                ret
n05:
                                cmp al,6
                                jne n06
                                putchar '6'
                                ret
n06:
                                cmp al,7
                                jne n07
                                putchar '7'
                                ret
n07:
                                and al,0x0f
                                cmp al,8
                                jne n08
                                putchar '8'
                                ret
n08:
                                cmp al,9
                                jne n09
                                putchar '9'
                                ret
n09:
                                cmp al,10
                                jne n0a
                                putchar 'A'
                                ret
n0a:
                                cmp al,11
                                jne n0b
                                putchar 'B'
                                ret
n0b:
                                and al,0x0f
                                cmp al,12
                                jne n0c
                                putchar 'C'
                                ret
n0c:
                                cmp al,13
                                jne n0d
                                putchar 'D'
                                ret
n0d:
                                cmp al,14
                                jne n0e
                                putchar 'E'
                                ret
n0e:
                                cmp al,15
                                jne n0f
                                putchar 'F'
                                ret
n0f:

                                ret

;----------------------------------------------------------------------------
storeinfo:
		pushf
		; Push info
		push ax
		mov ax,0
		mov ds,ax
		pop ax

		; Pull info to location
		mov word [ds:bp],ax
		add bp,2
		mov word [ds:bp],bx
		add bp,2
		mov word [ds:bp],cx
		add bp,2
		mov word [ds:bp],dx
		add bp,2
		mov ax,es
		mov word [ds:bp],ax
		add bp,2
		mov ax,di
		mov word [ds:bp],ax
		add bp,2

		popf
		jc  case1
		mov ax,0
		jmp ncase1
case1:
		mov ax,1
ncase1:
		mov word [ds:bp],ax
		ret
;----------------------------------------------------------------------------
global_descriptor_table:     ; here begins the GDT
;
gdt_start:        dw                 gdt_size,0,0            ; val for GDT reg
;
gdtoff:
segdes dummy_descriptor,0xFFFF,0,0,0x9A,0xCF,0
segdes code32_descriptor,0xFFFF,0,0,0x9A,0xCF,0
segdes data32_descriptor,0xFFFF,0,0,0x92,0xCF,0
segdes core32_descriptor,0xFFFF,0,0,0x92,0xCF,0
segdes code16_descriptor,0xFFFF,0,0,0x9A,0,0
segdes data16_descriptor,0xFFFF,0,0,0x92,0,0

gdt_size equ $-(dummy_descriptor)

code32_idx      equ   0x08              ; offset of 32-bit code segment in GDT
data32_idx      equ   0x10              ; offset of 32-bit data segment in GDT
core32_idx      equ   0x18              ; offset of 32-bit core segment in GDT

interrupt_descriptor_table: ;*** here begins the IDT ***;

idt_start:      dw                      idt_size,0,0
intdes interrupt_0,demo_int,code32_idx,0,0x8E,0

idt_size equ $-(interrupt_0)
;----------------------------------------------------------------------------
demo_int:
	iretd
;----------------------------------------------------------------------------


