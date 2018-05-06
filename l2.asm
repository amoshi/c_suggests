.model	small
.stack	100h
.386
.data
;wrd1	dw	1000101000100100b
wrd1	dw	0000000000000000b
wrd2	dw	65000
res	dw	0
.code
start:
	mov	ax,@data
	mov	ds,ax

	xor	eax,eax
	xor	ecx,ecx
	xor	edx,edx
	xor	ebx,ebx


	mov	ax,wrd1
	mov	bx,wrd2
	cmp	bx,0
	je	exit
	div	bx
	mov	ax,dx
	xor	dx,dx

	mov	cl,16
vertelka:

	mov	bx,ax
	and	bx,0000000000000111b
	cmp	bx,000b
	jne	continue

	dec	cl
	dec	cl
	dec	cl
	cmp	cl,0
	jl	exit
	inc	dx
	shr	ax,3
	jmp	vertelka

continue:
	dec	cl
	cmp	cl,0
	jl	exit
	shr	ax,1
	jmp	vertelka

exit:
	mov	res,dx
	mov	ax,4c00h
	int	21h
	end	start
