.model	small
.stack	100h
.386
.data
a	db	4
b	db	3
c	db	4
d	db	5
e	db	2

threed	dd	3
three	dw	3
twod	dd	2

right	dd	0
left	dd	0
adiv2	dw	0
ab	dw	0

.code
start:
	mov	ax,@data
	mov	ds,ax

	xor	eax,eax
	xor	ebx,ebx
	mov	al,b
	cbw
	mov	ebx,eax
	mov	al,a
	cbw

	add	ax,bx		; eax = a+b
	mov	ab,ax		; swap a+b to ab
	jo	met

	mov	bx,three
	cmp	bx,0
	je	met
	idiv	bx		; al = (a+b)/3

	xor	ecx,ecx
	mov	cl, al

	mov	ax, ab
	imul	ax		; (a+b)^2
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax = (a+b)^2

	mov	ebx,eax
	xor	eax,eax
	mov	al, a		; move a to al
	cbw
	cwd
	xchg	eax,ebx
	cmp	ebx,0
	je	met
	idiv	ebx		; (a+b)^2/a
	xor	ebx,ebx
	mov	bx,ax

	xor	eax,eax
	mov	al,cl
	cbw
	imul	bx		; (a+b)^3/3a
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax = (a+b)^3/3a

	mov	left,eax	; swap left


	xor	eax,eax
	mov	al,a
	cbw
	cwde
	mov	ebx,twod
	cmp	ebx,0
	je	met
	idiv	ebx		; a/2
	mov	adiv2,ax	; swap a/2

	xor	eax,eax
	mov	al,d
	imul	d
	imul	d		; d^3
	mov	ecx,eax		; ecx=d^3

	xor	eax,eax
	mov	al,c
	cbw
	xor	ebx,ebx
	mov	bx,three
	imul	bx		; 3c
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax = 3c

	sub	eax,ecx		; eax = 3c-d^3
	jo	met
	mov	ecx,eax		; ecx=3c-d^3

	xor	eax,eax
	mov	al,e
	cbw
	imul	ax		; e^2
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax = e^2
	
	xchg	eax,ecx
	cmp	ecx,0
	je	met
	idiv	ecx		; (3c-d^3)/e^2

	xor	ebx,ebx
	mov	bx,adiv2
	imul	adiv2		; a(3*c-d^3)/2*e^2
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax = a(3*c-d^3)/2*e^2

	mov	right,eax

	xor	eax,eax
	xor	ebx,ebx
	add	eax,ebx		; right+left
	mov	eax,right
	mov	ebx,left
	add	eax,ebx
	jo	met

met:
	mov	ax, 4c00h
	int	21h
	end	start
