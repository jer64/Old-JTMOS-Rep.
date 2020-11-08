;=================================================================================
; Bootsector for a JTMFS formatted non-system disk.
;
; (C) 2002 by Jari Tuominen(science.fiction@pp.nic.fi).
;
[bits 16]
org 0x100 ; 'COM file' start address (Yes, this is [.COM] file actually).
%include "bios.mac" ;;;; (puts,putchar)
%include "bootmod2.inc" ;;;; Any key handler
jmp start ; Jump to PMODE initialization code.
%include "seg.mac" ;;;; Segment Descriptor Macros
%include "text.inc" ;;;; Any text things bootsector code needs
%include "a20.inc" ;;;; A20 line setup code
%include "detection.inc" ;;;; Detects drives.
;
WAIT_KEY equ 0
;----------------------------------------------------------------------------
code16  dw 0
;----------------------------------------------------------------------------
; main procedure, this is the entry point
start:
	jmp $
;----------------------------------------------------------------------------


