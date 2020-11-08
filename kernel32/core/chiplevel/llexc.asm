;------------------------------------------------------------------
; Exception Handling Low Level ASM Code.
; (C) 2003-2005 Jari Tuominen.
; ASMllexception_0 => llexception_0
; See coprocessor.asm for coprocessor not found handler.
;------------------------------------------------------------------
%include "core/lowlevel/exc.mac"  ; SAFE
extern llexception_0
extern llexception_1
extern llexception_2
extern llexception_3
extern llexception_4
extern llexception_5
extern llexception_6
extern llexception_7
extern llexception_8
extern llexception_9
extern llexception_10
extern llexception_11
extern llexception_12
extern llexception_13
extern llexception_14
extern llexception_15
extern llexception_16
;
global ASMllexception_0
global ASMllexception_1
global ASMllexception_2
global ASMllexception_3
global ASMllexception_4
global ASMllexception_5
global ASMllexception_6
global ASMllexception_7
global ASMllexception_8
global ASMllexception_9
global ASMllexception_10
global ASMllexception_11
global ASMllexception_12
global ASMllexception_13
global ASMllexception_14
global ASMllexception_15
global ASMllexception_16
global llHaltNow

ASMllexception_0:
			SAFE llexception_0

ASMllexception_1:
			SAFE llexception_1

ASMllexception_2:
			SAFE llexception_2

ASMllexception_3:
			SAFE llexception_3

ASMllexception_4:
			SAFE llexception_4

ASMllexception_5:
			SAFE llexception_5

ASMllexception_6:
			SAFE llexception_6

ASMllexception_7:
			SAFE llexception_7

ASMllexception_8:
			SAFE llexception_8

ASMllexception_9:
			SAFE llexception_9

ASMllexception_10:
			SAFE llexception_10

ASMllexception_11:
			SAFE llexception_11

ASMllexception_12:
			SAFE llexception_12

ASMllexception_13:
			SAFE llexception_13

ASMllexception_14:
			SAFE llexception_14

ASMllexception_15:
			SAFE llexception_15

ASMllexception_16:
			SAFE llexception_16

llHaltNow:
			cli
			cld
.loop:			nop
			jmp .loop

