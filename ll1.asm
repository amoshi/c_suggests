.model	small
.stack	100h
.386
.data
a	db	120
b	db	121
c	db	122
d	db	123
e	db	124

threed	dd	3
three	dw	3
two	dw	2

right	dd	0
lefth	dd	0
leftl	dd	0
adiv2	dw	0
aplusb	dd	0

.code
start:
	mov	ax,@data
	mov	ds,ax

; left
	xor	eax,eax
	xor	ebx,ebx
	xor	ecx,ecx
	xor	edx,edx
	mov	al,a
	cbw
	cwd			; prepare 'a'
	mov	ebx,eax
	mov	al,b
	cbw
	cwd			; prepare 'b'

	add	eax,ebx	; a+b
	jo	met
	mov	aplusb,eax	; swap a+b

	mov	bx,three
	cmp	bx,0
	je	met
	idiv	bx		; (a+b)/3

	xor	ecx,ecx
	mov	cx,ax		; cx = (a+b)/3

	xor	eax,eax
	mov	al,a		; al=a
	cbw
	cwd			; eax=a
	mov	ebx,eax		; ebx=a

	mov	eax,aplusb
	imul	aplusb		; (a+b)^2
	cmp	ebx,0
	je	met
	idiv	ebx		; (a+b)^2/a

	xor	ebx,ebx
	xor	edx,edx
	mov	ebx,aplusb	; ebx=a+b
	imul	ebx

	mov	leftl,eax	; swap left
	mov	lefth,edx	; swap left

; right	
	xor	eax,eax
	xor	ebx,ebx
	xor	ecx,ecx
	xor	edx,edx

	mov	al,a		; al = a
	cbw
	cwd
	idiv	two		; a/2
	mov	adiv2,ax	; move a/2 to adiv2

	xor	eax,eax
	mov	al,d
	imul	d
	imul	d		; d^3
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax=d^3
	mov	ebx,eax		; ebx=d^3

	xor	eax,eax
	xor	edx,edx
	mov	al,c		; al=c
	cbw			; ax=c
	mov	cx,three
	imul	cx		; 3*c
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax=3*c

	sub	eax,ebx		; eax=3c-d^3
	jo	met
	mov	ebx,eax		; ebx=3c-d^3

	xor	eax,eax
	mov	al,e		; al=e
	imul	e		; ax=e^2
	xchg	eax,ebx		; bx=e^2, eax=3c-d^3
	cmp	bx,0
	je	met
	idiv	bx		; ax=3c-d^3/e^2

	mov	bx,adiv2	; bx=a/2
	imul	bx		; dx:ax=ax*bx = a(3c-d^3)/2*e^2
	rol	eax,16
	mov	ax,dx
	ror	eax,16		; eax=a(3c-d^3)/2*e^2
	mov	right,eax

	xor	eax,eax
	xor	ebx,ebx
	mov	eax,leftl
	mov	ebx,right
	add	eax,ebx
	jo	met

met:
	mov	ax, 4c00h
	int	21h
	end	start
