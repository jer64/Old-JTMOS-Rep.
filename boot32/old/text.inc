;
str_loadtxt:    db "[Loading Operating System]: ",0
;
str_progress:   db "¯",0
; Texts for boot32
str_loadagain:	db 13,10,"RECALL:",13,10,0
str_enter:	db 13,10,0
str_readerror   db 13,10,"Read error - Location: ",0
str_loading:    db "JTMOS SYSTEM LOADER (C) 1997-2003 by Jari Tuominen",13,10,0
str_loaded:     db 13,10,"Ready: Kernel loaded, checksum OK.",13,10,0
str_detecthd:	db 13,10,"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ",13,10,0
; Press any key to launch the kernel . .
str_key:        db "Any key to kernel",13,10,0
str_loaderror:  db 13,10,"CHKFAIL "
                db 13,10,"Insert a working system disk in DRIVE A: and"
		db 13,10,"Press any key to continue or [ESC] to ignore error: "
str_nonsysdsk:	db "This disk contains no system, press any key . .",13,10,0
checksum:	dw 0

