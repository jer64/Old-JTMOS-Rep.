;============================================
;	    GENERAL PURPOSE
;       ASSEMBLY LANGUAGE LIBRARY
;    (C) 2003-2007 by Jari Tuominen.
;============================================
bits 32
section .text

global TripleFault
global vmsleep
global false_jump
global genint
global asfunciton1
global nullinterrupt
global nullinterrupt2
global waitpesc
global setupidt
global GIMMIE_CS
global GIMMIE_DS
global GIMMIE_ES
global GIMMIE_FS
global GIMMIE_GS
global GIMMIE_SS
global GETGDTOFFS
global GETIDTOFFS
global GET_EFLAGS
global GET_LDT
global GetCR3
global SETUPGDT
global SETUPLDT
global NESTEDTASK
global asm_inc
global blinker
global STORETR
global LOADTR
global GETCR0
global SETCR0
;global GETCR1
global GETCR2 ; thats the exception address register
global SETCR3
global GETCR3
extern gdt_table
global TSSJMP
global GETESP
global GETSS
global GETCS
global GETDS
global GETES
global GETFS
global GETGS
global CLEARNT
global TaskSwitched
global tss_goto
global Halt
extern tswitch
global get_tr

;----------------------------------------------------------------
; Interrupted Procedure's and Handler's Stack
;
; (OFFSET INDICATOR)
; 0
; ^-------^
; ^EFLAGS ^ (0)
; ^CS     ^ (-4)
; ^EIP    ^ (-6)
; ^-------^
; -10
;
; OFFS+ESP = Real Address(Depends on Data Segment used too!)
;
; Note: stack grows always backwards as default.
;----------------------------------------------------------------

;----------------------------------------------------------------
tss_goto:
			push ebp
			mov ebp,esp
			;
			mov eax,[ebp+8]
			call tswitch
			;
			mov esp,ebp
			pop ebp
			ret
;----------------------------------------------------------------


TSSJMP: 		;--------------------------------------
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			mov gs,ax
			;
			mov edx,0x00000000
			jmp [gs:edx]
			;
			mov esp,ebp
			pop ebp
			ret

GETESP:                 ; Returns contents of the stack pointer(E)
                        mov eax,esp
                        ret

GETSS:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,ss
			ret

GETCS:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,cs
			ret

GETDS:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,ds
			ret

GETES:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,es
			ret

GETFS:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,fs
			ret

GETGS:			; Returns contents of the stack pointer(E)
			xor eax,eax
			mov ax,gs
			ret

GETCR0:			; Returns contents of the specified register
			mov eax,cr0
			ret

; CR1 cannot be read supposedly
;GETCR1:			; Returns contents of the specified register
;			mov eax,cr1
;			ret

GETCR2:			; Returns contents of the specified register
			mov eax,cr2
			ret

GETCR3:			; Returns contents of the specified register
			mov eax,cr3
			ret

SETCR3:			;--------------------------------------
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			;
			mov cr3,eax
			;
			mov esp,ebp
			pop ebp
			ret

SETCR0:			;--------------------------------------
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			;
			mov cr0,eax
			;
			mov esp,ebp
			pop ebp
			ret

			;--------------------------------------
TripleFault:
			mov eax,cr0
			and al,0xFF-1
			mov cr0,eax
			sti
			lidt [zerro]
			int 0x00
			int 0x01
			int 0x02
			int 0x73
			nop
			nop
			jmp TripleFault
zerro			dd 0

STORETR:		;--------------------------------------
			; USED TO CHANGE A SELECTOR, WHICH
			; SPECIFIES WHICH TASK IS NEXT TO BE RAN.
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			;
			ltr ax
			;
			mov esp,ebp
			pop ebp
			ret

farcall:		;--------------------------------------
			push ebp
			mov ebp,esp
			;
			mov eax,[ebp+8]
			mov ebx,[ebp+12]
			;
			mov esp,ebp
			pop ebp
			ret

LOADTR:			;--------------------------------------
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			;
			ltr ax
			;
			mov esp,ebp
			pop ebp
			ret

get_tr:			;--------------------------------------
			push ebp
			mov ebp,esp
			;
			mov eax,0
			str ax
			;
			mov esp,ebp
			pop ebp
			ret


NESTEDTASK:		;--------------------------------------
			; THIS FUNCTION SETS NT FLAG IN EFLAGS
			push dx
			pushf
			pop word [NESTEDTASKTMP]
			mov dx,word [NESTEDTASKTMP]
			or dx,(1<<14)
			push dx
			popf
			pop dx
			ret
NESTEDTASKTMP		dd 0

CLEARNT:		;--------------------------------------
			; THIS FUNCTION CLEARS NT FLAG IN EFLAGS
			push dx
			pushf
			pop word [NESTEDTASKTMP2]
			mov dx,word [NESTEDTASKTMP2]
			and dx,(0xFFFFFFFF-(1<<14))
			push dx
			popf
			pop dx
			ret
NESTEDTASKTMP2  	dd 0

asm_inc:
			push ebp
			mov ebp,esp
			mov eax,[ebp+8]
			;
			inc eax
			;
			mov esp,ebp
			pop ebp
			ret

asfunction1:            push ebp
                        mov ebp,esp
                        mov eax,[ebp+8]
                        mov ecx,[ebp+12]
                        rol eax,1
                        mov esp,ebp
                        pop ebp
                        ret


blinker:
                        mov dx,0x3c8
                        mov al,0
                        out dx,al
                        inc dx
blinker_c1:		mov al,0
			inc byte [ds:blinker_c1+1]
                        out dx,al
			out dx,al
			out dx,al
			ret


; this is a handler for only interrupts that
; have been called with int 0xNN opcode
nullinterrupt:          pushad
                        ;
			
;                        mov dx,0x3c8
;                        mov al,0
;                        out dx,al
;                        inc dx
;meterz1:		mov al,0
;			inc byte [ds:meterz1+1]
;                        out dx,al
;			out dx,al
;			out dx,al

                        ;
                        popad
                        iret


; for hardware/timer etc.. (uses port 0x20)
nullinterrupt2:         pushad
                        ;
			
;                        mov dx,0x3c8
;                        mov al,0
;                        out dx,al
;                        inc dx
;meterz2:		mov al,0
;			inc byte [ds:meterz2+1]
;                        out dx,al
;			out dx,al
;			out dx,al

			; flag interrupt done
                        mov dx,0x20
                        mov al,0x20
                        out dx,al
                        ;
                        popad
                        iret




; Reads until port reads ESC value and then waits for ESC release.
waitpesc:               in al,0x60
                        cmp al,1
                        jne waitpesc
.1:                     in al,0x60
                        cmp al,0x81
                        jne .1
                        ret

setupidt:               ;
	                push ebp
                        mov ebp,esp

                        mov eax,[ebp+8]
                        lidt [ds:eax]

                        mov esp,ebp
                        pop ebp
                        ;
                        ret

; GET EFLAGS
; Returns 32-bit flags register in eax.
GET_EFLAGS:		;
			pushfd
			pop eax
			ret

; Get CR3
GetCR3:			;
			mov eax,cr3
			ret

; GET LDT
;
GET_LDT:		;
			sldt [ds:dword GET_LDT_TMP]
			mov eax,[ds:dword GET_LDT_TMP]
			ret
			;
GET_LDT_TMP:		dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

; GETGDTOFFS, returns 32-bit offset to GDT (using SGDT OPCODE)
;
GETGDTOFFS:             ;
                        ;
                        sgdt [ds:dword temp123]
			mov eax,temp123
                        ret
temp123 dd 0,0,0,0,0,0,0,0
temp124 dd 0,0,0,0,0,0,0,0

; GETIDTOFFS, returns 32-bit offset to IDT (using SIDT OPCODE)
;
GETIDTOFFS:             ;
                        ;
                        sidt [ds:dword temp124]
                        mov eax,temp124
                        ret

; Functions GIMMIE_CS-GS gives specified segment register value.
GIMMIE_CS:              ;
                        ;
                        mov eax,0
                        mov ax,cs
                        ;
                        ret

GIMMIE_DS:              ;
                        ;
                        mov eax,0
                        mov ax,ds
                        ;
                        ret

GIMMIE_ES:              ;
                        ;
                        mov eax,0
                        mov ax,es
                        ;
                        ret

GIMMIE_FS:              ;
                        ;
                        mov eax,0
                        mov ax,fs
                        ;
                        ret

GIMMIE_GS:              ;
                        ;
                        mov eax,0
                        mov ax,gs
                        ;
                        ret

GIMMIE_SS:              ;
                        ;
                        mov eax,0
                        mov ax,ss
                        ;
                        ret
;*END OF GIMMIE CS-GS,SS*



SETUPGDT:		push ebp
			mov ebp,esp
			;
			mov eax,[ebp+8]
			;
			mov [dword my_gdt_offset+0],eax
			;
			cli
			lgdt [ds:dword my_gdt]
			;
			mov esp,ebp
			pop ebp
			ret

my_gdt:
			dw 0xFFFF ; n bytes, yeah! thats enough space, no? :-)
my_gdt_offset:		dd 0
			; safe space
			dd 0,0,0,0,0,0


SETUPLDT:		push ebp
			mov ebp,esp
			;
			mov eax,[ebp+8]
			;
			cli
;			lldt [ds:my_ldt_offset]
			lldt [ds:eax]
			;
			mov esp,ebp
			pop ebp
			ret

my_ldt_offset:		dd 0
			dd 0,0,0,0,0,0

genint:                 push ebp
                        mov ebp,esp
                        ;
                        mov eax,[ebp+8]
                        mov byte [.1+1],al
.1:                     int 0x00
                        ;
                        mov esp,ebp
                        pop ebp
                        ret


;----------------------------------------------------------------
false_jump:
			; Jump ssss:oooooooo, 16:32, segment:offset.
			db 0x66,0xea
			dd 0xf0000000
			dw 0xffff
;----------------------------------------------------------------
TaskSwitched:
			; Set task switched flag in CR0
		;	mov	eax,cr0
		;	or	eax,1<<3
		;	mov	cr0,eax

			;
			mov	eax,cr0
			and	eax,0xFFFFFFFF - (1<<3)
			mov	cr0,eax
			ret
;----------------------------------------------------------------
_clts:
			clts
			ret
;----------------------------------------------------------------

Halt:
			hlt
			ret

vmsleep:
			sti
			hlt
			ret
