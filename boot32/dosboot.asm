;=================================================================================
; JTMOS starter for MS-DOS
; ------------------------
;
; (C) 2006 by Jari Tuominen.
;
; This program will be loaded to 0xF000 (just below where the kernel is laoded.)
;
[bits 16]
org 0x100 ; 'COM file' start address (Yes, this is [.COM] file actually).
%include "bios.mac" ;;;; (puts,putchar)

begin:          ;
		jmp start ; Jump to PMODE initialization code.

%include "header.inc" ;;;; Bootsector header
;%include "bloche.inc" ; obsolete

%include "seg.mac" ;;;; Segment Descriptor Macros
%include "text.inc" ;;;; Any text things bootsector code needs
; moved text.inc to bootmod.inc
%include "a20.inc" ;;;; A20 line setup code
%include "detection.inc" ;;;; Detects drives.
;
WAIT_KEY equ	1
;----------------------------------------------------------------------------
code16  dw 0
;----------------------------------------------------------------------------
; main procedure, this is the entry point
start:
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



        ; Wait A key
	mov ax,WAIT_KEY
	cmp ax,0
	je dont_wait_key
        puts str_key
        mov ax,0
        int 0x16
dont_wait_key:


	; -- VESA job. --
	; 0x100	640x400x8	graphics	yes
	; 0x101	640x480x8	graphics	yes
	; 0x102	800x600x4	graphics	no
	; 0x103	800x600x8	graphics	yes	
	; 0x112	640x480x32	graphics	yes

	; Get linear frame buffer address.
	mov	ax,0x0400 ; ES:DI pointer to structure.
	mov	es,ax
	mov	di,0
	mov	ax,0x4F01 ; Get video mode info.
	mov	cx,0x0101 ; Video mode 0x101.
	int	0x10
	cmp	ah,0
	jne	vesa_error

	; Setup VESA linear frame buffer.
	mov	ax,0x4F02 ; Set VBE mode.
	mov 	bx,0x4101 ; (0x101) 640x480 8-bit | (0x4000) linear frame buffer.
	int	0x10
	cmp	ah,0
	je	vesa_okay
vesa_error:
	;
	puts	str_bad_vesa
	jmp	$
vesa_okay:

	; ^^VESA job done.^^

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
	dd 0x00010000
	dw code32_idx
;	db	0xEA            ; opcode for far jump (to set CS correctly)
;	dw	0x10000,code32_idx


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


