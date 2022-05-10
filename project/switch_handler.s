	;; Handles Port 2 interrupts (switches 1-4)
	.text
	.balign	2
	.global Port_1
	.global	Port_2		
	.section	__interrupt_vector_3, "ax"
	.word	Port_1
	.section	__interrupt_vector_4, "ax"
	.word	Port_2
	.text
	.extern	P1IFG
	.extern	P2IFG
	.extern	redrawScreen
	.extern switchPort
	.extern	switch_c_handler
Port_1:
	sub	#2, R1
	mov	P1IFG, R15
	mov	0(R15), 0(R1)
	and	#8, 0(R1)
	cmp	#0, 0(R1)
	jz	done_1
	bic	0(R1), P1IFG
	mov	#1, &switchPort

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
done_1:
	add 	#2, R1
	cmp	#0, &redrawScreen
	jz	dont_wake_1
	and 	#0xffef, 0(R1)	; clear CPU off
dont_wake_1:
	RETI

	
Port_2:
	sub	#2, R1
	mov	P2IFG, R15
	mov	0(R15), 0(R1)
	and	#15, 0(R1)
	cmp	#0, 0(R1)
	jz	done_2
	bic	0(R1), P2IFG
	mov	#2, &switchPort

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
done_2:
	add	#2, R1
	cmp	#0, &redrawScreen
	jz	dont_wake_2
	and	#0xffef, 0(R1)	; clear CPU off
dont_wake_2:
	RETI
	
