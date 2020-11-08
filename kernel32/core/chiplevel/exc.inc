;------------------------------------------------------------------
; Exception Handling Low Level ASM Code.
; (C) 2003-2005 Jari Tuominen.
; ASMexception_0 => exception_0
; See coprocessor.asm for coprocessor not found handler.
; INCLUDED BY llint.asm !
;------------------------------------------------------------------
%include "core/lowlevel/exc.mac"	; SAFE
global whereAD
extern exception_0
extern exception_1
extern exception_2
extern exception_3
extern exception_4
extern exception_5
extern exception_6
extern exception_7
extern exception_8
extern exception_9
extern exception_10
extern exception_11
extern exception_12
extern exception_13
extern exception_14
extern exception_15
extern exception_16
;
global ASMexception_0
global ASMexception_1
global ASMexception_2
global ASMexception_3
global ASMexception_4
global ASMexception_5
global ASMexception_6
global ASMexception_7
global ASMexception_8
global ASMexception_9
global ASMexception_10
global ASMexception_11
global ASMexception_12
global ASMexception_13
global ASMexception_14
global ASMexception_15
global ASMexception_16
global HaltNow

ASMexception_0:
			SAFE exception_0

ASMexception_1:
			SAFE exception_1

ASMexception_2:
			SAFE exception_2

ASMexception_3:
			SAFE exception_3

ASMexception_4:
			SAFE exception_4

ASMexception_5:
			SAFE exception_5

ASMexception_6:
			SAFE exception_6

ASMexception_8:
			SAFE exception_8

ASMexception_9:
			SAFE exception_9

ASMexception_10:
			SAFE exception_10

ASMexception_11:
			SAFE exception_11

ASMexception_12:
			SAFE exception_12

ASMexception_13:
			SAFE exception_13

ASMexception_14:
			SAFE exception_14

ASMexception_15:
			SAFE exception_15

ASMexception_16:
			SAFE exception_16

HaltNow:
			cli
			cld
.loop:			nop
			jmp .loop

whereAD:		dd 0
