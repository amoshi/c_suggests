.model	small
.stack 100h
.386
.data
wrd1	dw	123
wrd2	dw	10
.code
start:            
	mov	ax,@data
	mov	ds,ax

	xor eax, eax
	xor eax, eax
	xor ecx, ecx
	xor edx, edx
	xor ebx, ebx
	
	
	mov	ax,wrd1
	mov	bx,wrd2
	cmp	wrd2,0
	je	exit
	idiv	bx
	mov	ax,dx
	
	mov	cl,16
vertelka:
	rol	ax,1
	mov	bx,ax
	and	bx,0000000000000111b
	cmp	bx,000b
		
	jne	continue
	inc	dx
		
continue:
	loop vertelka

exit:
	mov	ax, 4c00h
	int	21h
	end	start
