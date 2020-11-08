;32bit JTMOS boot sector by Jari Tapio Tuominen.
;Radically modified and converted from double segment version to COM stylish
;single segment program layot.
[bits 16]
org 0x100 ; 'COM file' start address (Yes, this is [.COM] file actually).
%include "bios.mac" ;;;; (puts,putchar)
%include "bootmod.inc" ;;;; Boot Module (Calls BIOS to read rest of boot+krn)
jmp start ; Jump to PMODE initialization code.
%include "seg.mac" ;;;; Segment Descriptor Macros
%include "stack.inc" ;;;; Stack
%include "text.inc" ;;;; Any text things bootsector code needs
;----------------------------------------------------------------------------
idt_real	dw	3ffh,0,0		; Real Mode IDT
code16  dw 0
;----------------------------------------------------------------------------
; main procedure, this is the entry point
start:
        mov ax,cs
        mov word [code16],ax

        ;Kill floppy motor
        mov dx,0x3f2
        mov al,0
        out dx,al

        ; Begin protected mode setup working ...
	mov	ax,cs		; load code-segment into DS and ES
	mov	ds,ax
	mov	es,ax

        mov     ax,word [code16]       ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        mov     [ds:code32_descriptor+base_addr0_15],ax ; store it in the dscr
        mov     [ds:data32_descriptor+base_addr0_15],ax
	shr	eax,8
        mov     [ds:code32_descriptor+base_addr16_23],ah
        mov     [ds:data32_descriptor+base_addr16_23],ah

        mov     ax,word [code16]       ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,dummy_descriptor ; calculate physical address of GDT
        mov     dword [ds:gdt_start+2],eax

        mov     ax,word [code16]       ; get '32-bit' code segment into AX
	movzx	eax,ax		; clear high word
	shl	eax,4		; make a physical address
        add     eax,interrupt_0  ; calculate physical address of IDT
        mov     dword [ds:idt_start+2],eax

	cli			; disable interrupts
        lgdt    global_descriptor_table      ; load GDT register
        lidt    interrupt_descriptor_table   ; load IDT register
	mov	eax,cr0		; get CR0 into EAX
	or	al,1		; set Protected Mode bit
	mov	cr0,eax		; after this we are in Protected Mode!
        ;
        ;***************************************************
        ; Stop the CPU here because the memory is not really
        ; functioning rightful, I wonder why?
        ;***************************************************
        jmp $
        ;
        ; This code is a piece of protected mode code which is used
        ; work fine, but it won't, :-(
        mov ax,core32_idx
        mov es,ax
        mov edi,0xb8000
        mov eax,0x41414141
        mov [es:edi],eax

ul:     ;
        jmp ul

;        db      0eah            ; opcode for far jump (to set CS correctly)
;        dw      start32,code32_idx


[bits 32]
stack32         times STACK32_SIZE db 0    ; 32-bit stack
stack32_end:     dw 0

;----------------------------------------------------------------------------

global_descriptor_table:     ; here begins the GDT

gdt_start:        dw                 gdt_size,0,0            ; val for GDT reg


; Note that 'segdes' is a macro just like 'intdes'.
; -------------------------------------------------
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
