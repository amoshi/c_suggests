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
aplusb	dd	0

.code
start:
	mov	ax,@data
	mov	ds,ax

	xor	eax,eax
	xor	ebx,ebx
	xor	ecx,ecx
	xor	edx,edx
	mov	al,a
	cbw
	cwd		; prepare 'a'
	mov	ebx,eax
	mov	al,b
	cbw
	cwd		; prepare 'b'

	add	eax,ebx	; a+b
	jo	met
	mov	aplusb,eax	; swap a+b

	mov	bx,three
	cmp	bx,0
	je	met
	idiv	bx	; (a+b)/3

	xor	ecx,ecx
	mov	cx,ax	; cx = (a+b)/3

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

	mov	left,eax	; swap left

	

met:
	mov	ax, 4c00h
	int	21h
	end	start
