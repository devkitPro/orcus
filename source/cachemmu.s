.global enableICache
	
enableICache:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0
	
	mrc 	p15, 0, r0, c1, c0, 0
	orr 	r0, r0, #(0x1 << 12)
	mcr 	p15, 0, r0, c1, c0, 0
	
	bx 	lr
