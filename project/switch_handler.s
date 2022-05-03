	.text
	.balign	2
	.global	Port_2
	.section	__interrupt_vector_4, "ax"
	.word	Port_2

	.text
	.extern	P2IFG
	.extern redrawScreen
	.extern	switch_c_handler
Port_2:
	sub	#2, R1
	mov	&P2IFG, 0(R1)
	and	#0x40, 0(R1)
	cmp	#0, 0(R1)
	jz	done
	bic	0(R1), P2IFG
	PUSH	R15
	PUSH	R14
	PUSH 	R13
	PUSH	R12
	PUSH 	R11
	PUSH	R10
	PUSH	R9
	PUSH	R8
	PUSH 	R7
	PUSH	R6
	PUSH	R5
	PUSH	R4

	CALL	#switch_c_handler

	POP	R4
	POP	R5
	POP	R6
	POP	R7
	POP	R8
	POP	R9
	POP	R10
	POP	R11
	POP	R12
	POP	R13
	POP	R14
	POP	R15
done:
	add	#2, R1
	cmp	#0, &redrawScreen
	jz	dont_wake
	and	#0xff, 0(R1)
dont_wake:
	RETI
