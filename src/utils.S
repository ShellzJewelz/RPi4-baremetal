.global memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret

.global memset
memset:
	mov x3, x0
	str x1, [x0], #8
	subs x2, x2, #8
	b.gt memset
	mov x0, x3
	ret

.global write32
write32:
	str w1,[x0]
	ret

.global read32
read32:
	ldr w0,[x0]
	ret
