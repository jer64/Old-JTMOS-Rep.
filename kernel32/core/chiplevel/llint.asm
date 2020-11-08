;-----------------------------------------------------
; System Interrupt Handling Code / Assembler Section
;
; RELATED TO FOLLOWING:
; - PIT interrupt handling
; - Calling of C based "sub-handlers"
;
; Thread states:
; 1 full speed (priority 1)
; 2 priority 1/2
; 3 priority 1/3
; ...............
; 100 mega idle (priority 1/100)
;
; So the higher state a thread has,
; the lower it's running priority is,
; priority one means it will be most
; probably be executed, as
; priority 100 would mean it will be
; executed only at a very slow speed,
; but if the computer is in idle otherwise,
; and the priority 100 thread is the only
; "active" thread, the active one will get
; all the CPU time.
;
; Added inportd and outportd here,
; nasm understands these functions,
; but x86 asm that comes with gcc does not..
; (needed by pci.c)
;
; SEE pit_low function.
; SEE LLSchedule.
;
; (C) 2001-2005 by Jari Tuominen
;		(jari@vunet.org)...
;-----------------------------------------------------
bits 32
section .text

global jump1234
global pit_ISR
global get_eflags
global set_eflags
global storesegregs
global restoresegregs
;
extern set_thread
extern new_thread
;
global pit_low
extern force_switch_thread_nr
extern irqdebugger
extern thread_states
extern thread_esps
extern nr_curthread
extern nr_threads
extern thread_CPUspending
extern thread_tick
extern thread_codedescs
extern thread_datadescs
extern update_gdt_for_appthread
;
global outportd
global inportd
;
extern copysregsfromthread
extern copysregstothread
extern showappgdt
;
global tmpds
global tmpes
global tmpfs
global tmpgs
global tmpss
;
global relax_thread
extern Multitasking
extern kill_thread
extern get_prio
extern set_prio
extern pit_handler
; render_thread_stack returns ESP in EAX
global render_thread_stack
global idle_moment
;
extern scallsIsWorking
;
extern _boot_up
global boot_up
;
; ** INCLUDES FOR MACROS ********************************
%include "core/lowlevel/timerhandler.mac"
%include "core/lowlevel/quicktext.mac"

%macro idle 0
		call relax_thread
%endmacro

;
; idle_moment
;
; Sleeps a moment, other threads will take
; the CPU in control meanwhile.
; Note: uses system call #1
;
idle_moment:
		;
		; system call #1: relax_thread(idle)
		mov al,byte [scallsIsWorking]
		cmp al,0
		je  idle_moment_l1
		mov al,byte [Multitasking]
		cmp al,0
		je  idle_moment_l1
		int 0x7E
idle_moment_l1:
		ret

;
pit_low:
		; Make task switch.
		int 0x7d
		;
		iret

;******* TASK SWITCHER & PIT ISR ******************************
; The last parameter means:
; If non-zero task switch is forced(Task switcher, e.g. relax_t.)
; If zero task switch is made when tick enables it(PIT handler)
segm		dw 0
;TiHandler pit_ISR,pit_handler,0
;TiHandler relax_thread,0,1
;**************************************************************

dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
dd 0,0,0,0,0,0,0,0,0,0,0
junkstack:
dd 0,0,0,0

;//void outportd(DWORD portad,DWORD value);
;//DWORD inportd(DWORD portad);

; void outportd(DWORD portad,DWORD value);
;
outportd:
                push ebp
                mov ebp,esp
                ; port
                mov edx,dword [ebp+8]
		; data
		mov eax,dword [ebp+12]
		out dx,eax
		;
		pop ebp
		ret

; DWORD inportd(DWORD portad);
;
inportd:
                push ebp
                mov ebp,esp
                ;
                mov edx,dword [ebp+8]
                in  eax,dx
                ;
                pop ebp
                ret

%include "core/lowlevel/segreg.mac"

; Returns flags.
;
get_eflags:
		pushfd
		pop eax
		ret

; restore_flags(DWORD flags);
;
set_eflags:
		push ebp
		mov ebp,esp

		; get eflags
		mov eax,dword [ebp+8]
		push eax
		popfd

		pop ebp
		ret

flags_data:	dd 0,0,0,0,0

%include "core/lowlevel/exc.inc"

;------------------------------------------------------
;
;------- various variables ----------------------------
;
tempoffs	dd 0,0
;
debugdata1	dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
;
llint_temp	dd 0,0,0,0,0,0,0,0
; ------- end of variables -----------------------------



; ======================================================
; Setup a new stack for kernel at the very beginning
; ======================================================
boot_up:
;		mov esp,0x9FF00
		jmp _boot_up
; ======================================================

jump1234:
		mov eax,0x1234
		jmp eax
