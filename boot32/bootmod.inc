;=============================================================================================
;
;	BOOT MODULE version 1.0.2003,2014,2018,2020
;	by Jari Tuominen(jari.t.tuominen@gmail.com)
;	Modified version of my 16-bit kernel loader code
;	(contains only 16-bit code).
;
; 11/11/02: Updated the checksum routine.
; 25/03/03: Updated 0x55AA check number. (but didn't got it working that time)
;
; From HelpPC, about INT 13, read sector:
; - a DMA boundary error will occur if the buffer at ES:BX crosses
;   a 64K segment boundary
;
; Max. kernel size is now about 512K, thanks to more advanced loader!
;
%include "kchecksum.inc"
;%include "bios.mac" ;;;; (puts,putchar)
;%include "text.inc"
;
; Set this to non-zero if you want the boot module to warn user
; if the kernel didn't load up properly into the memory.
;
; The boot loader resides at memory region 7C00-FFFF.
; This memory region can hold around program code/data upto 32K.
; Limit for boot loader size is 9 kb, this limit delivers
; from the limit of a single cylinder(9K) on a 1.44M floppy disk.
; The 32-bit kernel will be loaded at memory address 10000.
; Both 1.44M floppy and IDE hard disk version of the boot loader
; is designed to do this.
;
;=============================================================================================
CHECKSUM_FAILURE_MEANS_ERROR	equ 1
DEBUG				equ 0 ; 1=enable debug messages, 0=disable debug messages.
;
SEG_SECTORBUFFER		equ 0x9C00 ; Min. 0x9DC00 should be sufficient for a 9K floppy buffer.
HD_SEG_SECTORBUFFER		equ 0x9000 ; -- same for HDs
OFFS_SECTORBUFFER		equ 0x0000
;
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
                                mov ax,0x7B7C
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
                                mov ax,0x7B0
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
; (WITHIN 512 bytes section)
Loader_HD:
				;
				call detect_params

				; Hard disk based boot loader.

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
				; Loads the whole loader at memory address 0x10000,
				; this avoids 64K DMA page boundary oversteps.
				mov ax,0x1000
				mov es,ax	; ofs to buffer
				mov bx,0	; seg to buffer
				mov ah,0x02
				mov dl,byte [cs:bootdrv]
				mov dh,0 ; head
				mov cl,1 ; sector 1-17
				mov ch,0 ; cylinder
			;	mov al,byte [cs:max_s] ; number of sectors to read
				mov al,18 ; load 9K, 18 sectors
				int 0x13
				jnc nerr100
				mov dl,byte [cs:bootdrv]
				mov ah,0
				int 0x13
				dec byte [cs:hdtry]
				jnz tryagainhd
				jmp error100
nerr100:
				; Copy the boot loader into correct place.
				; Copy 65535 bytes from 0x10000 to 0x7C00.
				; This operation will overwrite us, but
				; that's ok, because it's the same code,
				; just as abit longer than a 512 bytes sector
				; the BIOS loader loads at the start up.
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
			;	jmp $
				jmp hd_load_kernel_image
;--------------------------------------------------------------------------
disk_reset_error:
				puts t11
;--------------------------------------------------------------------------
lalala:
reload_bootstrap:
				; Input key.
				mov ax,0
				int 0x16
				; Call bootstrap loader.
				mov dl,0
				int 0x19

;
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
; (WITHIN 512 bytes section)
Loader_1.44M:
				;
				; Load whole boot track (head 0, track 0).
				mov ax,0x07B0
				mov es,ax
				mov bx,0x0100
				mov dx,0x0000
				mov cx,0x0001
				mov ax,0x0212
				mov dl,byte [cs:bootdrv]
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
				; Jump to bigger loader which'll load
				; the whole kernel.
				; This loader resides beyond first 512 bytes sector.
				jmp floppy_load_kernel_image
;----------------------------------------------------------------------------------------------
prhex16:
				pushad
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
				popad
				ret

hextab				db "0123456789ABCDEF"
;----------------------------------------------------------------------------------------------
; Detect HD parameters.
;        Return  CH = Maximum value for cylinder
;                     (10-bit value; upper 2 bits in CL)
;                CL = Maximum value for sector
;                DH = Maximum value for heads
detect_params:
				; Call get current drive parameters.
				mov ah,0x08
				mov dl,byte [cs:bootdrv]
				int 0x13

				; Get sectors per track.
				push cx
				and cl,63
				mov byte [cs:max_s],cl
				pop cx

				; Get amount of cylinders.
				xchg cl,ch
				shr ch,6
				inc cx
				mov word [cs:max_c],cx

				; Get amount of heads.
				inc dh
				mov byte [cs:max_h],dh

				;
				ret
;----------------------------------------------------------------------------------------------
; Fill with bytes to go to correct offset.
TIMES (512-11)-($-$$) DB 0xBD
;
Boot_record_settings:

; Kernel checksum.
correct_kchecksum		dw kchecksum
; 512K kernel image offset (in bytes) on the hard disk.
; (0000-FFFF)
keroffs				dw 0	; 503
; Hard disk parameters.
max_c				dw 895	; 505	893
max_h				db 5	; 507	5
max_s				db 55	; 508	55
; Within first block:
; 1A) Boot drive.
; 1B) Signature 0xAA55.
;
bootdrv: 	DB 0		; offset 509
signature:	DW 0xAA55	; offset 510

;----------------------------------------------------------------------------------------------
; 2) Partition table.
;
Partition_Table:
TIMES 512 DB 0xFF

;----------------------------------------------------------------------------------------------
; 3) Information block.
;
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
TIMES (512-16-64)	DB 0xBD

;--------------------------------------------------------------
; 4) Rest is code/data.
;--------------------------------------------------------------


;--------------------------------------------------------------
; Clear 512K kernel image region at 10000-8FFFF.
;--------------------------------------------------------------
clear_memory:
				;
				mov ax,0x1000
				mov ds,ax
				mov di,0
				mov ax,0
clear_memory_l1:
				mov cx,8
				mov di,0
				mov ax,0
clear_memory_l2:
				mov word [ds:di],ax
				add di,2
				dec cx
				jne clear_memory_l2
				;
				mov ax,es
				inc ax
				mov es,ax
				;
				cmp ax,0x9000
				jne clear_memory_l1
				ret
;--------------------------------------------------------------
; Hard disk based kernel loader
;--------------------------------------------------------------
hd_load_kernel_image:
				; Show some start up texts.
				call start_up_texts

				; Clear kernel memory region.
				call clear_memory

				;
				mov di,0		; output buffer offset
				mov ax,0x1000		; output buffer segment
				mov es,ax		;
				;
				mov ax,1024			; - amount of sectors to load
				mov si,word [cs:keroffs]	; - block where to start load
				shr si,9			; divide by 512, convert to sectors
				call hd_linear_load_sectors

				;
				pushad
				puts t5
				popad
				mov ax,word [cs:wseg_offs]
				call phex16
				pushad
				puts enterstr
				popad

				; Jump to checksum measurement.
				jmp measurement
				jmp past
			;	jmp reload_bootstrap
;--------------------------------------------------------------
; ax = amount of blocks to read
; si = from which block to begin
;
				;--------------------------------------------
hd_linear_load_sectors:
				;
				mov word [cs:amountnr],ax
				mov word [cs:blocknr],si

				;
				putchar 10
				putchar 13

				;--------------------------------------------
				; Convert linear to CHS.
				mov cx,word [cs:blocknr]
				call linear_to_chs
hd_linear_load_sectors_l1:
				;
				call hd_read_sectors
				; Get next place to read.
				call inc_chs
				;
				call copy_to_place

				; Some debug messages for debug mode.
%if DEBUG!=0
				call ega_copy_to_place

				;
				pushad
			;	puts str_progress
				mov ah,0
				mov al,byte [cs:cylnr]
				call phex8
				puts str_vali
				mov ah,0
				mov al,byte [cs:headnr]
				call phex8
				puts str_vali
				mov ah,0
				mov al,byte [cs:sectnr]
				call phex8
				puts str_vali
				mov ax,word [cs:wseg_offs]
				call phex16
				puts str_vali
				mov ax,word [cs:block_chksum]
				call phex16
				putchar 13

				; Input key. XXXX
				mov ax,0
				int 0x16

				;
				popad
%endif

				; Increase linear block.
				inc word [cs:blocknr]
				; Increase block counter.
				inc word [cs:bnr]

				; Decrease amount of blocks -counter.
				dec word [cs:amountnr]
				jnz hd_linear_load_sectors_l1

				;--------------------------------------------
				; We're done here.
				ret

; These values are determined by linear_to_chs converter function.
cylnr				db 0
headnr				db 0
sectnr				db 1
;
bnr				dw 0
detsum				dw 0	; determined checksum
corsum				dw 0	; correct checksum
block_chksum:			dw 0
;--------------------------------------------------------------
; Copy from sector read buffer to kernel memory location.
; (0x10000 ++).
; DS:SI = source
; ES:SI = destination
copy_to_place:
				;
				pushad
				;
				mov ax,HD_SEG_SECTORBUFFER
				mov ds,ax
				mov ax,word [cs:wseg_offs]
				mov es,ax
				mov si,0
				mov cx,512
				mov dx,0
copy_to_place_l1:
				mov al,byte [ds:si]
				mov byte [es:si],al
				mov ah,0
				add dx,ax
				inc si
				dec cx
				jnz copy_to_place_l1
				;
				mov ax,es
				add ax,(512/16)
				mov word [cs:wseg_offs],ax
				;
				mov word [cs:block_chksum],dx
				;
				popad
				ret

wseg_offs			dw 0x1000

; ES:SI = source		(9000:0000)
; DS:DI = destination		(B800:0000)
ega_copy_to_place:
				; Define source.
				mov ax,HD_SEG_SECTORBUFFER
				mov es,ax
				; Define destination.
				mov ax,0xB800
				mov ds,ax
				; Define source/destination index registers.
				mov si,0
				mov di,0

				;
				mov cx,80*12
wipeloop:
				mov al,32
				mov byte [ds:di],al
				add di,2
				dec cx
				jnz wipeloop

				;
				mov di,80*4
				; Define amount of bytes to transfer.
				mov cx,512
xcopy_to_place_l1:
				; Copy first 8 bytes.
				mov al,byte [es:si]
				mov byte [ds:di],al
				add di,2
				inc si
				;
				dec cx
				jnz xcopy_to_place_l1
				ret
;--------------------------------------------------------------
; These are our options.
blocknr:			dw 0 ; where to start load
amountnr:			dw 0 ; amount to load in sectors
;
hd_read_sectors:
				; Load whole boot track (head 0, track 0).
				pushad
				mov ax,HD_SEG_SECTORBUFFER	; Load to 9000:0000.
				mov es,ax
				mov bx,0
				mov ch,byte [cs:cylnr]	; cylinder number
				mov dh,byte [cs:headnr]	; head number
				mov cl,byte [cs:sectnr]	; sector number
				mov ah,0x02		; function (read sectors)
				mov al,1		; number of sectors to load
				mov dl,byte [cs:bootdrv]; drive number
trya:				int 0x13
				jnc read_success
				; Reset disk system on error.
				pushad
				mov dx,0
				mov ax,0
				mov dl,byte [cs:bootdrv]
				int 0x13
				popad
				jmp trya
read_success:
				popad
				ret
;--------------------------------------------------------------
; CX = which linear block to convert to CHS
linear_to_chs:
				pushad
				mov al,1
				mov byte [cs:sectnr],al
				mov al,0
				mov byte [cs:headnr],al
				mov byte [cs:cylnr],al
				cmp cx,0
				je  outti
looppi1:
				call inc_chs
				dec cx
				jnz looppi1
outti:
				popad
				ret
;--------------------------------------------------------------
; Increase CHS, used by linear_to_chs.
inc_chs:
				; Increase sector number.
				inc byte [cs:sectnr]
				mov al,byte [cs:max_s]
				inc al ; XXXX
				cmp byte [cs:sectnr],al
				jne looppi2
				; Set sector back to 1.
				mov al,1
				mov byte [cs:sectnr],al
				; Increase head number.
				inc byte [cs:headnr]
				mov al,byte [cs:max_h]
				cmp al,byte [cs:headnr]
				jne looppi2
				; Set head back to 0.
				mov al,0
				mov byte [cs:headnr],al
				; Increase cylinder number.
				inc byte [cs:cylnr]
looppi2:
				ret
				;
;--------------------------------------------------------------
start_up_texts:
				puts str_detecthd
				puts str_loading
				puts str_loadtxt
				ret
;--------------------------------------------------------------
measure_kernel_checksum:
				; ---------------------------------------------------------------------
				; Measure checksum for the loaded
				; kernel image and store it at memory address 0x1002.
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
				ret
;--------------------------------------------------------------
; Floppy disk based kernel loader
;--------------------------------------------------------------
floppy_load_kernel_image:
				; Copy kernel checksum at location 0x1000
				; kernel will check this as it's first things.
                                ;
                                mov ax,0x0100
                                mov es,ax
                                mov di,0
                                mov ax,word [cs:correct_kchecksum]	; checksum
                                mov word [es:di],ax
				add di,2
				mov ax,0x55AA		; signature
				mov word [es:di],ax
				;
				call start_up_texts

				;
				call clear_memory

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

;----------------------------------------------------------------------------------------------------------
; AFTER KERNEL IS LOADED (BOTH FLOPPY AND HD):
;----------------------------------------------------------------------------------------------------------
iloaded_enough:
				pushad
				puts t5
				popad
				pushad
				mov ax,es
				call phex16
				puts enterstr
				popad
measurement:
				; Checksum measurement.
				call measure_kernel_checksum
				; ----------------------------------------
				; store the measured checksum
				mov dx,0x0100
				mov es,dx
				mov si,0
				mov word [es:si+2],ax
				; Compare to correct one.
			;	cmp ax,word [es:si]
				cmp ax,word [cs:correct_kchecksum]
				jne nloaded_fine2
				jmp loaded_fine
nloaded_fine2:
				push es
				push si
                                putchar 13
                                putchar 10
				pop si
				pop es

				;
				puts str_measuredchecksum

				;
				mov dx,0x0100
				mov es,dx
				mov si,0
				mov ax,word [es:si+2]
				call phex16
				putchar '/'

				;
				puts str_correctchecksum

				;
				mov dx,0x0100
				mov es,dx
				mov si,0
				mov ax,word [cs:correct_kchecksum]
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




; Copies SEG_SECTORBUFFER:OFFS_SECTORBUFFER(DS:SI) to ES:BX(ES:DI).
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
; If we are arrived here normally, the 512K kernel
; should be loaded correctly in the memory.
loaded_fine:
				;
				puts str_loaded
                                ; Input key.
				mov al,byte [cs:bootdrv]
				cmp al,0
				je skipper
				mov ax,0
				int 0x16
skipper:

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
t12:				db "Checksum failure detected for loaded 512 bytes block.",10,13,0
enterstr:			db 13,10,0,


;=================================================================================================================
; Kernel is loaded here, checksum tests(if any) are completed without any errors.
; Now we just store bootdrv variable's contents at memory 0x1004 as a WORD variable.
past:
				; Store at 0x1004
				mov ax,0x0
				mov ds,ax
				mov di,0x1004
				; Convert from 8-bit to 16-bit.
				mov al,byte [cs:bootdrv]
				mov ah,0
				mov word [ds:di],ax

				; Here jump to the 32-bit kernel ... :
