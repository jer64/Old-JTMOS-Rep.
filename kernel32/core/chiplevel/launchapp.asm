;=====================================
; launchapp.asm
;
; Will launch assembly language based
; application in it's own seperate
; memory space.
;
; (C) 2002-2003 by Jari Tuominen
;=====================================
bits 32
section .text
extern asmtestapp1
global asmlaunchapp
global appoffset
global appseg
global appstackoffs
global apprealoffs
global launchapp_cleartogo

; 0x4000 = 16K stack
; 0x10000 = 64K stack
APPLICATION_START_AD	equ	0x10000

;
; Example:
; TESTAPP MEMORY MODEL
; CS =		0x100
; DS =		0x108
; START AD =	0x10000
;
asmlaunchapp:
		cli
		; Flag that we have been here
		mov eax,1
		mov dword [launchapp_cleartogo],eax
		; Calculate data segment
		mov ax,word [appseg]
		add ax,8
		mov ds,ax
		mov es,ax
		mov fs,ax
		; Note: GS is not used
		;
		mov esp,dword [appstackoffs]
		cmp esp,0
		jnz asmlaunchapp_not_illegal_zero
		mov esp,APPLICATION_START_AD-16
asmlaunchapp_not_illegal_zero:
		mov ss,ax
		; Jump ssss:oooooooo, 16:32, segment:offset.
	;	sti
		cli
                db 0xea
appoffset:
		dd APPLICATION_START_AD ;;; IMPORTANT - DEPENDS ON STATIC LIBC/KERNEL SETTING
appseg:
		dw 0x100
		;
		dd 0,0,0,0
appstackoffs:
		dd 0
apprealoffs:
		dd 0

launchapp_cleartogo:
		dd 0

;

