@echo off
call ndisasm -u kernel32.bin > kernel32.dis
call objdump --disassemble-all kernel32.o > kernel32_o.dis
