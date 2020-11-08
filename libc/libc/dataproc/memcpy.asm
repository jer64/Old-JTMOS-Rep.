global memcpy

memcpy:
	push	ebp
	mov	esp,ebp
	push	esi
	push	edi
	mov	edi,dword [ebp+8]
	mov	esi,dword [ebp+12]
	mov	ecx,dword [ebp+16]
	rep movsb
	pop	edi
	pop	esi
	mov	eax,dword [ebp+8]
	leave
	ret


