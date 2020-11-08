;-----------------------------------------------------------
; LOW LEVEL INTERRUPT FUNCTIONS
; AND TASK SWITCHING MECHANISM
;
; RELATED TO FOLLOWING:
; RenderThreadStack
;
; Update:
; I isolated the stack renderation code here,
; makes easier to manipulate it so.
;
; RenderThreadStack MUST RETURN ESP IN EAX!
;
; Problem: render thread stack function seems to
; mess up the stack of the caller somehow,
; or atleast PID number and ESP turns into random data,
; for a moment.
;
; (C) 2002-2003 by Jari Tuominen(jari.tuominen@kanetti.net)
;-----------------------------------------------------------

bits 32
section .text

extern set_thread
extern new_thread
;
extern thread_states
extern thread_esps
extern nr_curthread
extern nr_threads
extern thread_tick
extern thread_codedescs
extern thread_datadescs
extern update_gdt_for_appthread
;
extern kill_thread
extern get_prio
extern set_prio
extern pit_handler
;
global tarkitus_ecx
global tarkitus_esp
global RenderThreadStack
global newcs
global newds

%include "core/lowlevel/esclock.mac"

;-----------------------------------------------------------------------------
;
; RenderThreadStack ESP,PC,PID, ss,cs,ds
;
; +8	ESP
; +12	PC
; +16	PID
; +20	ss
; +24	cs
; +28	ds
;
; UPDATED:	has three parameters.
; UPDATED II:	has the segment selector parameters.
;
RenderThreadStack:
		;
		push ebp
		mov ebp,esp
		;
		pushfd
		cli
		; We need to save flags too
		mov dword [regebx],ebx
		;
		; Save the present ss registers
		mov ax,ss
		mov word [oldss],ax
		; ss
		mov ax,word [ebp+20]
		mov word [newss],ax
		; cs
		mov ax,word [ebp+24]
		mov word [newcs],ax
		; ds
		mov ax,word [ebp+28]
		mov word [newds],ax

		;
		pushad
		;
		; Update:
		; Convert to kernel linear addressing
		; by using the thread's data segment's
		; base address.
		;
		; Get the 32-Bit base address
		mov edi,dword [ebp+16]
		shl edi,3
		add edi,thread_datadescs
		; get pieces to ecx and edx
		; first piece
		mov ecx,dword [ds:edi+0]
		mov edx,dword [ds:edi+4]
		; convert ecx and edx to
		; 32-Bit linear base address
		;
		; get the lower 16 bits
		shr ecx,16
		and ecx,0x0000ffff
		; get the upper 16 bits
		mov ebx,edx
		and ebx,0x000000ff
		shl ebx,16
		and edx,0xff000000
		or  edx,ebx
		; ok now we got the bot
		; 16-bit values,
		; no we will just merge
		; these to get the whole
		; 32-bit offset
		or  ecx,edx
		; Bang!
		; We got the 32-bit linear
		; address of the data segment's
		; base at ecx now

		;
		; temporarily store current esp to edx
		mov edx,esp
		; setup the specified stack
		; eax=location of the stack
		mov ax,word [newss]
;		mov ss,ax
		mov eax,dword [ebp+8]
		mov esp,eax
                ; convert to real linear address
		add esp,ecx
		mov dword [tarkitus_ecx],ecx
		mov dword [tarkitus_esp],esp
		; --- HERE WE GENERATE THE STACK:
		; push all the parameters to the stack
		pushfd
		; Enable IRQs on the flags register
		pop eax
		or eax,512 ; hex 0x200
		; Push EFLAGS
		push eax
		; Push Code Segment
		mov ax,word [newcs]
		push ax
		; get the new EIP for the thread
		mov eax,dword [ebp+12]
		push eax	; pushes the EIP
		;
		pushad
		; --- END OF STACK GENERATION
		; We're done here.
		; Restore the previous stack
		; ss of stack
		mov ax,word [oldss]
;		mov ss,ax
		; esp of stack
		mov ecx,esp
		mov esp,edx
		;
		mov dword [tempovar],ecx
		;
		mov ebx,dword [regebx]
		popad
		popfd
		pop ebp
		;
		; return the new stack position
		mov eax,dword [tempovar]
		ret

;- - - - - - - - - - - - - - - - - - - - - - - - - -
oldss		dd 0
newss		dd 0
newcs		dd 0
newds		dd 0
regeax		dd 0
regebx		dd 0
regecx		dd 0
regedx		dd 0
regesi		dd 0
regedi		dd 0
tempovar	dd 0
;
tarkitus_ecx	dd 0
tarkitus_esp	dd 0

;


