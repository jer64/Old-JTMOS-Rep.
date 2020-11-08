;
str_loadtxt:    db "LOADING: ",0
;
str_vali:	db ",",0
str_progress:   db "o",0
; Texts for boot32
str_loadagain:	db 13,10,"RECALL:",13,10,0
str_enter:	db 13,10,0
str_readerror   db 13,10,"Read error - location: ",0
str_loading:    db "JTMOS SYSTEM LOADER (C) 1997-2020 by Jari Tuominen (jari.t.tuominen@gmail.com).",13,10,0
str_loaded:     db 13,10,"Ready: Kernel loaded, checksum OK.",13,10,0
str_vesafbdet:	db 13,10,"Detected VESA Linear Frame Buffer at 0x",0
str_detecthd:	db 13,10,"컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�",13,10,0
; Press any key to launch the kernel . .
str_key:        db "Press any key to launch the kernel . .",13,10,0
str_loaderror:  db 13,10,"Kernel checksum failure. "
                db 13,10,"Insert a working system disk in DRIVE A: and"
		db 13,10,"Press any key to continue or [ESC] to ignore error: "
str_nonsysdsk:	db 13,10,"This disk does not contain a valid system.",13,10,0
str_measuredchecksum	db "measured checksum = 0x",0
str_correctchecksum	db ", correct checksum = 0x",0
str_bad_vesa	db 13,10,"VESA 2.0 BIOS support not found. System halted.",13,10,0
checksum:	dw 0


