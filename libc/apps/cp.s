	.file	"cp.c"
	.version	"01.01"
gcc2_compiled.:
.section	.rodata
.LC0:
	.string	"%s/%s"
.LC1:
	.string	"rb"
.LC2:
	.string	"wb"
.text
	.align 16
.globl copyFile
	.type	 copyFile,@function
copyFile:
	pushl %ebp
	movl %esp,%ebp
	subl $260,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%edi
	movl 12(%ebp),%eax
	pushl %edi
	pushl %eax
	pushl $.LC0
	leal -256(%ebp),%ebx
	pushl %ebx
	call sprintf
	pushl $.LC1
	pushl %edi
	call fopen
	movl %eax,%esi
	addl $24,%esp
	testl %esi,%esi
	jne .L2
	movl $1,%eax
	jmp .L5
	.p2align 4,,7
.L2:
	pushl $.LC2
	pushl %ebx
	call fopen
	movl %eax,-260(%ebp)
	addl $8,%esp
	testl %eax,%eax
	jne .L3
	pushl %esi
	call fclose
	movl $1,%eax
	jmp .L5
	.p2align 4,,7
.L3:
	pushl %edi
	call fsizeof
	movl %eax,%ebx
	addl $4,%esp
	testl %ebx,%ebx
	jle .L4
	pushl %ebx
	call malloc
	pushl %esi
	pushl $1
	pushl %ebx
	movl %eax,%edi
	pushl %edi
	call fread
	movl -260(%ebp),%edx
	pushl %edx
	pushl $1
	pushl %ebx
	pushl %edi
	call fwrite
	addl $36,%esp
.L4:
	pushl %esi
	call fclose
	movl -260(%ebp),%edx
	pushl %edx
	call fclose
	xorl %eax,%eax
.L5:
	leal -272(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe1:
	.size	 copyFile,.Lfe1-copyFile
.section	.rodata
.LC3:
	.string	"No files found(%s).\n"
.LC4:
	.string	"copying %s "
	.align 32
.LC5:
	.string	"\nError: Cannot copy file '%s'\n"
.LC6:
	.string	"\n"
.LC7:
	.string	"%d File(s)\n"
.text
	.align 16
.globl copy_now
	.type	 copy_now,@function
copy_now:
	pushl %ebp
	movl %esp,%ebp
	subl $816,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	pushl $250
	leal -552(%ebp),%eax
	pushl %eax
	call getcwd
	leal -296(%ebp),%edx
	movl %edx,-816(%ebp)
	pushl %edx
	pushl %ebx
	call findfirst
	movl %eax,-812(%ebp)
	addl $16,%esp
	testl %eax,%eax
	jne .L7
	pushl %ebx
	pushl $.LC3
	call printf
	jmp .L6
	.p2align 4,,7
.L7:
	movl $1,%esi
	leal -260(%ebp),%edi
	leal -808(%ebp),%ebx
	jmp .L8
	.p2align 4,,7
.L20:
	call getch1
	cmpl $27,%eax
	je .L9
	incl %esi
.L8:
	pushl %edi
	pushl %ebx
	call strcpy
	pushl %ebx
	call fexist
	addl $12,%esp
	cmpl $2,%eax
	je .L13
	testl %eax,%eax
	je .L13
	cmpl $-1,%eax
	je .L13
	cmpl $0,verbose
	je .L14
	pushl %ebx
	pushl $.LC4
	call printf
	addl $8,%esp
	jmp .L15
	.p2align 4,,7
.L14:
	call idle_moment
.L15:
	movl 12(%ebp),%edx
	pushl %edx
	pushl %ebx
	call copyFile
	addl $8,%esp
	testl %eax,%eax
	je .L17
	pushl %ebx
	pushl $.LC5
	call printf
	addl $8,%esp
.L17:
	cmpl $0,verbose
	je .L18
	pushl $.LC6
	call printf
	addl $4,%esp
	jmp .L13
	.p2align 4,,7
.L18:
	call idle_moment
.L13:
	movl -816(%ebp),%edx
	pushl %edx
	movl -812(%ebp),%edx
	pushl %edx
	call findnext
	addl $8,%esp
	testl %eax,%eax
	je .L20
.L9:
	cmpl $0,verbose
	je .L23
	pushl %esi
	pushl $.LC7
	call printf
.L23:
	xorl %eax,%eax
.L6:
	leal -828(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe2:
	.size	 copy_now,.Lfe2-copy_now
.section	.rodata
.LC8:
	.string	"/v"
.LC9:
	.string	"-v"
.LC10:
	.string	"--verbose"
.LC11:
	.string	"/p"
.LC12:
	.string	"-p"
.LC13:
	.string	"--pause"
.LC14:
	.string	"--help"
.LC15:
	.string	"-h"
.LC16:
	.string	"/?"
.LC17:
	.string	"/h"
.text
	.align 16
.globl do_args
	.type	 do_args,@function
do_args:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	movl $1,%esi
	pushl %ebx
	movl 8(%ebp),%edi
	incl %edi
	cmpl %edi,%esi
	jge .L26
	movl 12(%ebp),%ebx
	addl $4,%ebx
	.p2align 4,,7
.L28:
	pushl $.LC8
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L30
	pushl $.LC9
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L30
	pushl $.LC10
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	jne .L29
.L30:
	movl $1,verbose
.L29:
	pushl $.LC11
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L32
	pushl $.LC12
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L32
	pushl $.LC13
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	jne .L31
.L32:
	movl $1,autoPause
.L31:
	pushl $.LC14
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L34
	pushl $.LC15
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L34
	pushl $.LC16
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	je .L34
	pushl $.LC17
	movl (%ebx),%eax
	pushl %eax
	call strcmp
	addl $8,%esp
	testl %eax,%eax
	jne .L27
.L34:
	movl $1,%eax
	jmp .L36
	.p2align 4,,7
.L27:
	addl $4,%ebx
	incl %esi
	cmpl %edi,%esi
	jl .L28
.L26:
	xorl %eax,%eax
.L36:
	leal -12(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe3:
	.size	 do_args,.Lfe3-do_args
.section	.rodata
.LC18:
	.string	"%s: missing file arguments\n"
.text
	.align 16
.globl main
	.type	 main,@function
main:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	pushl %ebx
	movl 8(%ebp),%ebx
	movl 12(%ebp),%esi
	movl $0,verbose
	movl $0,autoPause
	pushl %esi
	pushl %ebx
	call do_args
	addl $8,%esp
	cmpl $2,%ebx
	jle .L38
	movl 8(%esi),%eax
	pushl %eax
	movl 4(%esi),%eax
	pushl %eax
	call copy_now
	jmp .L39
	.p2align 4,,7
.L38:
	movl (%esi),%eax
	pushl %eax
	pushl $.LC18
	call printf
	xorl %eax,%eax
.L39:
	leal -8(%ebp),%esp
	popl %ebx
	popl %esi
	movl %ebp,%esp
	popl %ebp
	ret
.Lfe4:
	.size	 main,.Lfe4-main
	.comm	MALSTRU,8,4
	.comm	verbose,4,4
	.comm	autoPause,4,4
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
