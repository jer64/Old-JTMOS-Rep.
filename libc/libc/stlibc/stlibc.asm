;
; LIBC start up file
; stlibc.o will be linked in front of the application.
; stlibc.o will call the init function of libc to
; init the libc and call main() function.
;
extern init_libc
global libc_startup
libc_startup:
			jmp init_libc
fingerprint:
			db "[JTMOS LIBC (C) 2002-2019 Jari Tapio Tuominen (jari.t.tuominen@gmail.com)]"

;


