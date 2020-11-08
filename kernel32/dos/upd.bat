call mode co80 50

call nasm -f elf jk32\asm\aslib.asm
call move jk32\asm\aslib.o .

call gcc -w -c sysansi.c     -Wall -O
call gcc -w -c sysapp.c      -Wall -O
call gcc -w -c sysfile.c     -Wall -O
call gcc -w -c sysgfx.c      -Wall -O
call gcc -w -c sysglvar.c    -Wall -O
call gcc -w -c sysmem.c      -Wall -O
call gcc -w -c sysmisc.c     -Wall -O
call gcc -w -c system.c      -Wall -O
call gcc -w -c sysusr.c      -Wall -O
call gcc -w -c sysvga.c      -Wall -O
call gcc -w -c io.c          -Wall -O
call gcc -w -c int.c         -Wall -O
call gcc -w -c kernel32.c    -Wall -O

call ld aslib.o sysansi.o sysapp.o sysfile.o sysgfx.o sysglvar.o sysmem.o sysmisc.o system.o sysusr.o sysvga.o io.o int.o kernel32.o -o kernel32.bin -oformat binary -Tkernel32.src


call dis.bat

dir kernel32*.* /w




