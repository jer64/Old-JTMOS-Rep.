;=================================================================================
; System loader for 32-bit JTMOS operating system
; -----------------------------------------------
;
; 1.44M floppy disk version.
;
; (C) 2002-2003 by Jari Tuominen.
; Radically modified and converted from double segment version to COM stylish
; single segment executable.
; Special thanks to NASM tutorial for providing information:
; "The Netwide Assembler: NASM - Chapter 10: Troubleshooting"
; http://home.comcast.net/~fbkotler/nasmdo10.html
;=================================================================================
;
;	I decided to modify the bootloader so that it automatically
;	boots up the kernel(without asking to press a key to start it).
;	This makes possible to test it without keyboard f.e.
;	Set WAIT_KEY to 1 if you want to it to ask a key press.
;
[bits 16]
org 0x100 ; 'COM file' start address (Yes, this is [.COM] file actually).
%include "bios.mac" ;;;; (puts,putchar)
%include "bootmod.inc" ;;;; Boot Module (Calls BIOS to read rest of boot+krn)
jmp start ; Jump to PMODE initialization code.

%include "header.inc" ;;;; Bootsector header

%include "seg.mac" ;;;; Segment Descriptor Macros
%include "text.inc" ;;;; Any text things bootsector code needs
; moved text.inc to bootmod.inc
%include "a20.inc" ;;;; A20 line setup code
%include "detection.inc" ;;;; Detects drives.
;
WAIT_KEY equ 0
;----------------------------------------------------------------------------
code16  dw 0
;----------------------------------------------------------------------------
; main procedure, this is the entry point
start:
        ; Kill floppy motor
        mov dx,0x3F2
        mov al,0
        out dx,al
	; Fix keyboard buffer(empty it)
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

        mov     ax,cs                  ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,dummy_descriptor ; calculate physical address of GDT
        mov     dword [ds:gdt_start+2],eax

        mov     ax,cs                  ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,interrupt_0  ; calculate physical address of IDT
        mov     dword [ds:idt_start+2],eax



        ; Wait A key
	mov ax,WAIT_KEY
	cmp ax,0
	je dont_wait_key
        puts str_key
        mov ax,0
        int 0x16
dont_wait_key:

	jmp nohddet
	;
	mov	dl,0x0  ; /dev/fd0
	mov	ah,0x08
	int	0x13
	mov	bp,0xf000
	call	storeinfo

	mov	dl,0x1  ; /dev/fd1
	mov	ah,0x08
	int	0x13
	mov	bp,0xf040
	call	storeinfo

	mov	dl,0x80 ; /dev/hda
	mov	ah,0x08
	int	0x13
	mov	bp,0xf080
	call	storeinfo

	mov	dl,0x81 ; /dev/hdb
	mov	ah,0x08
	int	0x13
	mov	bp,0xf0c0
	call	storeinfo

	mov	dl,0x82 ; /dev/hdc
	mov	ah,0x08
	int	0x13
	mov	bp,0xf100
	call	storeinfo

	mov	dl,0x83 ; /dev/hdd
	mov	ah,0x08
	int	0x13
	mov	bp,0xf140
	call	storeinfo
nohddet:

	; Max. keyboard repeat rate
	mov	ax,0x0305
	xor	bx,bx
	int	0x16

	; Disable NMI
	mov al,0x80
	mov dx,0x70
	out dx,al

        ; -------- let's go to the kernel
        call a20


        ; Load descriptor tables
        cli
        lgdt    [dword global_descriptor_table]
        lidt    [dword interrupt_descriptor_table]
        ; Enter protected mode
        mov     eax,cr0
        or      al,1
        mov     cr0,eax

        jmp shortjump0
shortjump0:

        ;
        ;***************************************************
        ; ENTER KERNEL32
        ; 
        ; Setup protected mode stack.
        mov esp,0x7c00
        mov ax,0x18
        mov ss,ax
        ; Also define rest of the segments, just to prevent
        ; additional errors occuring.
        mov ds,ax
        mov es,ax
        mov fs,ax
        mov gs,ax

        jmp shortjump1
shortjump1:

        ;
        ; Jump ssss:oooooooo, 16:32, segment:offset.
        db 0x66,0xea
        dd 0x00010000
        dw code32_idx
;        db      0eah            ; opcode for far jump (to set CS correctly)
;        dw      0x10000,code32_idx


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
segdes dummy_descriptor,0xffff,0,0,0x9a,0xcf,0
segdes code32_descriptor,0xffff,0,0,0x9a,0xcf,0
segdes data32_descriptor,0xffff,0,0,0x92,0xcf,0
segdes core32_descriptor,0xffff,0,0,0x92,0xcf,0
segdes code16_descriptor,0xffff,0,0,0x9a,0,0
segdes data16_descriptor,0xffff,0,0,0x92,0,0

gdt_size equ $-(dummy_descriptor)

code32_idx      equ   0x08              ; offset of 32-bit code segment in GDT
data32_idx      equ   0x10              ; offset of 32-bit data segment in GDT
core32_idx      equ   0x18              ; offset of 32-bit core segment in GDT

interrupt_descriptor_table: ;*** here begins the IDT ***;

idt_start:      dw                      idt_size,0,0
intdes interrupt_0,demo_int,code32_idx,0,0x8e,0

idt_size equ $-(interrupt_0)
;----------------------------------------------------------------------------
demo_int:
	iretd
;----------------------------------------------------------------------------


