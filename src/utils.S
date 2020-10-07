.global get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.global memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret

.global write32
write32:
	str w1,[x0]
	ret

.global read32
read32:
	ldr w0,[x0]
	ret
