;===================================================================
;
;       BOOT MODULE version 1.0
;	by Jari Tuominen(jari.tuominen@kanetti.net)
;       Modified version of my 16-bit kernel loader code.
;       (Contains only 16-bit code)
;
; 11/11/02: Updated the checksum routine.
; 25/03/03: Updated 0x55aa check number
;
; From HelpPC, about INT 13, read sector:
; - a DMA boundary error will occur if the buffer at ES:BX crosses
;   a 64K segment boundary
;
; Max. kernel size is now about 512K, thanks to more advanced loader!
;
%include "kchecksum.inc"
;
; Set this to non-zero if you want the boot module to warn user
; if the kernel didn't load up properly into the memory.
;
;===================================================================
CHECKSUM_FAILURE_MEANS_ERROR	equ 1

SEG_SECTORBUFFER		equ 0x9C00 ; Min. 0x9DC00 should be sufficient for a 9K floppy buffer.
OFFS_SECTORBUFFER		equ 0x0000

LOADLIMIT			equ 0x9000
				; 0x2000 = 64K  kernel
				; 0x3000 = 128K kernel
				; 0x5000 = 256K kernel
				; 0x7000 = 384K kernel
				; 0x8000 = 448K kernel
				; 0x9000 = 512K kernel
				; Much beyond 512K is not quite possible
				; without using pmode functions(featured in kernel).
				; Perhaps I'll have a 32-bit secondary loader
				; that'll load the OS itself(loader kernel).
				;
                                ; Correct CS:IP - So that org 0x0100 works correctly.
                                jmp 0x7B0:0x105
begin:                          ;
                                ; Make a dummy stack (Microsoft style :-)
                                cli
                                xor cx,cx
                                mov ss,cx
                                mov ax,0x7b7c
                                mov sp,ax
                                sti

;----------------------------------------------------------------------------------------------
; Reset boot drive to be sure.
;----------------------------------------------------------------------------------------------
				mov dl,byte [cs:bootdrv]
				mov ah,0
				int 0x13
				jnc nerr1
				jmp disk_reset_error
nerr1:

;----------------------------------------------------------------------------------------------
; Load 9K boot loader (boot record area).
;----------------------------------------------------------------------------------------------
                                mov ax,0x7b0
                                mov es,ax
                                mov si,bootdrv
                                mov al,byte [es:si]
				cmp al,0 ; A:
				jne nf1
				jmp Loader_1.44M
nf1:
				cmp al,1 ; B:
				jne nf2
				jmp Loader_1.44M
nf2:

;----------------------------------------------------------------------------------------------
; IDE hard disk version
; ---------------------
Loader_HD:
				;
			;	call detect_params

				; Hard disk based boot loader.
				puts t2

				; Print params
				mov ax,word [cs:max_c]
				call prhex16
				call between
				mov al,byte [cs:max_h]
				mov ah,0
				call prhex16
				call between
				mov al,byte [cs:max_s]
				mov ah,0
				call prhex16
				call between

tryagainhd:
				; Load whole boot loader.
				mov ax,0x1000
				mov es,ax	; ofs to buffer
				mov bx,0	; seg to buffer
				mov ah,0x02
				mov dl,byte [cs:bootdrv]
				mov dh,0 ; head
				mov cl,1 ; sector 1-17
				mov ch,0 ; cylinder
				mov al,byte [cs:max_s] ; number of sectors to read
				int 0x13
				jnc nerr100
				mov dl,byte [cs:bootdrv]
				mov ah,0
				int 0x13
				dec byte [hdtry]
				jnz tryagainhd
				jmp error100
nerr100:
				; Copy the boot loader into correct place.
				; 0x10000 to 0x7C00
				mov ax,0x1000
				mov es,ax
				mov ax,0x7C0
				mov ds,ax
				mov si,0
				mov cx,0xFFFF
l00:
				mov al,byte [es:si]
				mov byte [ds:si],al
				inc si
				dec cx
				jnz l00

				;
				mov ax,word [cs:checkword]
				call prhex16
				mov ax,word [cs:checkword]
				cmp ax,0xFCE2
				je  okz
				puts er1
				jmp lalala
;--------------------------------------------------------------------------
error100:
				puts er2
				jmp lalala
;--------------------------------------------------------------------------
; HD success.
okz:
				puts t10
				jmp $
;--------------------------------------------------------------------------
disk_reset_error:
				puts t11
;--------------------------------------------------------------------------
lalala:
				; Input key.
				mov ax,0
				int 0x16
				; Call bootstrap loader.
				mov dl,0
				int 0x19

hdtry:				db 10
t1:				db "FDC:",0
t2:				db "HD:",0
er1:				db "ER1",0	; Check word is not valid.
er2:				db "ER2",0	; Read error.
t10:				db "Fine.",0	; No error.
t11:				db "ER3",0	; Disk drive reset error.

;----------------------------------------------------------------------------------------------
between:
				putchar ','
				ret
;----------------------------------------------------------------------------------------------
; 1.44M FLOPPY DISK VERSION: LOAD SYSTEM BOOT LOADER
; --------------------------------------------------
Loader_1.44M:
				;
				puts t1
				; Load whole boot track (head 0, track 0).
				mov ax,0x07b0
				mov es,ax
				mov bx,0x0100
				mov dx,0x0000
				mov cx,0x0001
				mov ax,0x0212
				mov dl,byte [bootdrv]
try_again:			int 0x13
				jnc donebb
				; Reset disk system on error.
				pusha
				mov dx,0
				mov ax,0
				int 0x13
				popa
				jmp try_again
enough:
				mov ax,0x00
				int 0x16
				mov dl,0
				int 0x19
donebb:
				; 1.44M: 9K boot loader has been loaded.
				jmp jumpPast
;----------------------------------------------------------------------------------------------
prhex16:
				mov cx,4
prhex16_l1:
				push ax
				push cx
				mov si,ax
				and si,15
				add si,hextab
				putchar [cs:si]
				pop cx
				pop ax
				shr ax,4
				dec cx
				jnz prhex16_l1
				ret

hextab				db "0123456789ABCDEF"
;----------------------------------------------------------------------------------------------
detect_params:
				; Call get current drive parameters.
				mov ah,0x08
				int 0x13

				; Get sectors per track.
				push cx
				and cl,31
				mov byte [cs:max_s],cl
				pop cx

				; Get amount of cylinders.
				xchg cl,ch
				shr ch,6
				mov word [cs:max_c],cx

				; Get amount of heads.
				mov byte [cs:max_h],dh

				;
				ret
;----------------------------------------------------------------------------------------------
; Hard disk parameters.
max_c				dw 895
max_h				db 5
max_s				db 55
;----------------------------------------------------------------------------------------------
; Within first block:
; 1A) Boot drive.
; 1B) Signature 0xAA55.
;
TIMES 509-($-$$) DB 0xBD
bootdrv: 	DB 0		; offset 509
signature:	DW 0xAA55	; offset 510

; 2) Partition table
;
TIMES 512 DB 0

; 3) Information block
res1:		DD 0
res2:		DD 0
; Boot drive: 0=A:, 1=B:, 0x80=C:, 0x81=D:.
; From INT 13:
; DL = drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
; (bootdrv is specified in first block and in this info block,
;  in two places for a purpose.)
_bootdrv:	DB 0
; Detection string.
; String meaning:
; "KBDSK": Kernel boot disk, no file system included.
; "JTMFS": Regular boot disk functionality, file system featured.
detstr:			DB "KBDSK",0
; Rest can be zeroes, we don't require more info.
TIMES (512-8-1-6)	DB 0

;--------------------------------------------------------------
; 4) Reset is code/data.
;--------------------------------------------------------------

jumpPast:

				;
				puts str_detecthd
				; Copy kernel checksum at location 0x1000
				; kernel will check this as it's first things.
                                ;
                                mov ax,0x0100
                                mov es,ax
                                mov di,0
                                mov ax,kchecksum
                                mov word [es:di],ax
				add di,2
				mov ax,0x55aa
				mov word [es:di],ax
				;
				puts str_loading
				puts str_loadtxt


				; Clear checksum
				mov ax,0
				mov word [cs:checksum],ax
				; Load KERNEL module (head 1, track 0)
				mov ax,0x1000
				mov es,ax
				mov bx,0x0000+(512*18 * 0)
				mov dx,0x0100
				mov cx,0x0001
				mov ax,0x0212
loop1:
				; Reading location will be stored at stack
				; because fixparam plays around with it...
				push es ; push001
				pusha
				; ah=func, al=amount of sectors to read
				mov ax,SEG_SECTORBUFFER
				mov es,ax
				mov bx,OFFS_SECTORBUFFER
				mov ax,0x0212
				; drive #0
				mov dl,0
				int 0x13
				jnc nreadagain
				;-----------------------------------------------------
				; Here read error occured:
				popa
				pop es
				pusha
				; output where the mistake is
				;
				puts str_readerror
				pusha
				mov ax,0
				mov al,ch
				call phex8
				popa
				putchar '/'
				pusha
				mov ax,es
				call phex16
				popa
				putchar 13
				putchar 10
				;
				; Verify some other sector(just make a disk head move).
				mov ah,4
				mov cx,0x0001
				mov dh,0
				int 0x13
				; Reset disk system on error.
				mov ax,0
				mov dx,0
				int 0x13
				popa
				dec byte [cs:tries]
				jz lala
				jmp loop1
lala:
				jmp lala
				;-----------------------------------------------------
nreadagain:
				dec byte [cs:procnt]
				jnz noprsh
				mov al,2
				mov byte [cs:procnt],al
				pusha
				puts str_progress
				popa
noprsh:
				popa ; pop001
				pop es

				; Perform 9K buffer copy.
				push es
				pusha
				call copybuffer
				popa
				pop es

				; Increase ES segment(go to next track area)
				push ax
				mov ax,es
				add ax,(512*18)/16
				mov es,ax
				mov si,ax
				pop ax
				; Already loaded enough?
				cmp  si,LOADLIMIT
				ja  iloaded_enough

				; Inc head
				inc dh
				cmp dh,2
				jne notoverhead
				mov dh,0
				inc ch
notoverhead:
				
				
				jmp loop1
procnt				db 2
				; ------------- load next track
overflow1:
				;-------------------------------------------------------------------
iloaded_enough:
				pushad
				puts t5
				popad
				pushad
				mov ax,es
				call phex16
				puts enterstr
				popad
				;
				; Situation right now:
				; max. size of kernel is about 256K,
				; just a dummy specificaiton of size of it,
				; not that it'll not be larger in future!
				;

				; -----------------------------------------
				; Measure checksum for the loaded
				; kernel image and store it at 0x1002
				;
				mov ax,0x1000
				mov es,ax
				; zero the checksum register
				mov ax,0
lo1:
				; pointer=0
				mov si,0
lo2:
				mov bl,byte [es:si]
				mov bh,0
				add ax,bx
				add si,1
				cmp si,0x1000
				jne lo2
				;
				mov dx,es
				add dx,0x0100
				mov es,dx
				;
				mov dx,es
				cmp dx,LOADLIMIT
				jne lo1

				; ----------------------------------------
				; store the measured checksum
				mov dx,0x0100
				mov es,dx
				mov si,2
				mov word [es:si],ax
				cmp ax,word [es:si-2]
				jne nloaded_fine2
				jmp loaded_fine
nloaded_fine2:
				push es
				push si
                                putchar 13
                                putchar 10
				pop si
				pop es
				push es
				push si
				mov ax,word [es:si]
				call phex16
				putchar '/'
				pop si
				pop es
				mov ax,word [es:si-2]
				call phex16
				mov ax,CHECKSUM_FAILURE_MEANS_ERROR
				cmp ax,0
				jne nloaded_fine
				jmp loaded_fine
nloaded_fine:
				puts str_loaderror
				mov ax,0
				int 0x16
				cmp al,27
				jne try_again_now
				jmp ignore_error
try_again_now:
				puts str_loadagain
				mov dl,0
				int 0x19

				; some fixes(limits) 
fixparam:
				; Are we in a segment that must be fixed?
				push ax
				mov ax,es
				and ax,0x0f00
				cmp ax,0x0c00
				je  fixparam0c00
                                cmp ax,0x0d00
                                je  fixparam0d00
                                cmp ax,0x0e00
                                je  fixparam0e00
                                cmp ax,0x0f00
                                je  fixparam0f00
				jmp no_faultseg
fixparam0c00:
				mov ax,es
				sub ax,0x0500
				mov es,ax
				add bx,0x5000
				jmp no_faultseg
fixparam0d00:
				mov ax,es
                                sub ax,0x0600
                                mov es,ax
                                add bx,0x6000
                                jmp no_faultseg
fixparam0e00:
				mov ax,es
                                sub ax,0x0700
                                mov es,ax
                                add bx,0x7000
                                jmp no_faultseg
fixparam0f00:
				mov ax,es
                                sub ax,0x0800
                                mov es,ax
                                add bx,0x8000
                                jmp no_faultseg
no_faultseg:
				pop ax
				;
				push es
				pusha
				mov ax,es
				call phex16
				putchar ':'
				popa
				pusha
				mov ax,bx
				call phex16
				putchar 13
				putchar 10
				popa
				pop es
				ret




; copies SEG_SECTORBUFFER:OFFS_SECTORBUFFER(DS:SI) to ES:BX(ES:DI)
copybuffer:
				mov di,bx
                                mov ax,SEG_SECTORBUFFER
				mov ds,ax
				mov si,OFFS_SECTORBUFFER
                                ; counter to 0
				mov cx,0
loo1:
                                ; pointer=0
				; COPY 16 BYTES
				; -------------------
                                mov ax,word [ds:si+0]
				mov word [es:di+0],ax
                                mov ax,word [ds:si+2]
                                mov word [es:di+2],ax
                                mov ax,word [ds:si+4]
                                mov word [es:di+4],ax
                                mov ax,word [ds:si+6]
                                mov word [es:di+6],ax
				;
                                mov ax,word [ds:si+8]
                                mov word [es:di+8],ax
                                mov ax,word [ds:si+10]
                                mov word [es:di+10],ax
                                mov ax,word [ds:si+12]
                                mov word [es:di+12],ax
                                mov ax,word [ds:si+14]
                                mov word [es:di+14],ax
				; --------------------
				mov ax,ds
				add ax,1
				mov ds,ax
                                ;
                                mov ax,es
				add ax,1
				mov es,ax
                                ;
                                inc cx
                                cmp cx,((512*18)/16)
                                jne loo1
				ret




hexstr				db "0123456789ABCDEF"


tries:				db 2

;===================================================================================================
loaded_fine:
				;
				puts str_loaded
                                ; Input key.
			;	mov ax,0
			;	int 0x16
ignore_error:
				nop
				jmp past

checkword			dw 0xFCE2

; ax = value
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

t5:				db 13,10,"Kernel loaded upto segment 0x",0,
enterstr:			db 13,10,0,


past:
