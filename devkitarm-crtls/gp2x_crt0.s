/*--------------------------------------------------------------------------------
	This Source Code Form is subject to the terms of the Mozilla Public License,
	v. 2.0. If a copy of the MPL was not distributed with this file, You can
	obtain one at https://mozilla.org/MPL/2.0/.
--------------------------------------------------------------------------------*/

    .section	".crt0","ax"
	.code 32
	.align
	.global _start, _undefined_instruction, _software_interrupt, _prefetch_abort, _data_abort, _not_used, _irq, _fiq

        .set  Mode_USR, 0x10
        .set  Mode_FIQ, 0x11
        .set  Mode_IRQ, 0x12
        .set  Mode_SVC, 0x13
        .set  Mode_ABT, 0x17
        .set  Mode_UND, 0x1B
        .set  Mode_SYS, 0x1F

        .equ  I_Bit, 0x80
        .equ  F_Bit, 0x40	

        .equ  __int_stack_size, 256

@---------------------------------------------------------------------------------
_start:
@---------------------------------------------------------------------------------
	b       _start2
        ldr     pc, _undefined_instruction
        ldr     pc, _software_interrupt
        ldr     pc, _prefetch_abort
        ldr     pc, _data_abort
        ldr     pc, _not_used
        ldr     pc, _irq
        ldr     pc, _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:    .word software_interrupt
_prefetch_abort:        .word prefetch_abort
_data_abort:            .word data_abort
_not_used:              .word not_used
_irq:                   .word irq
_fiq:                   .word fiq

@---------------------------------------------------------------------------------
@ AXF addresses
@---------------------------------------------------------------------------------

_text_start:
	.word   __text_start
_ro_end:
	.word   __ro_end
_data_start:
	.word   __data_start
	.word   __bss_end
_bss_start:
	.word   __bss_start
_bss_end:
	.word   __bss_end	

undefined_instruction:
	subs pc,lr,#4

software_interrupt:
	subs pc,lr,#4
	
prefetch_abort:
	subs pc,lr,#4
	
data_abort:
	subs pc,lr,#4
	
not_used:
	subs pc,lr,#4
	
irq:
	subs pc,lr,#4
	
fiq:
	subs pc,lr,#4


@---------------------------------------------------------------------------------
_start2:
@---------------------------------------------------------------------------------
	ldr		R0,=__stack_base
	
	msr		CPSR_c, #Mode_FIQ|I_Bit|F_Bit
	mov		sp, R0
	sub		R0, R0,#__int_stack_size

	msr		CPSR_c, #Mode_IRQ|I_Bit|F_Bit
	mov		sp, R0
	sub		R0, R0,#__int_stack_size

	msr		CPSR_c, #Mode_SVC|I_Bit|F_Bit
	mov		sp, R0

	mov 		R0, #_bss_start
	mov 		R1, #_bss_end
	mov		R2, #0
clear_bss_loop:
	cmp 		r0, r1
	strlt 		r2, [r0], #4
	blt		clear_bss_loop
	
@---------------------------------------------------------------------------------
@ global constructors
@---------------------------------------------------------------------------------
	ldr		r3,=_call_main
	mov		lr,r3
	ldr		r3,=__libc_init_array
	bx		r3
@---------------------------------------------------------------------------------
@ Jump to user code
@---------------------------------------------------------------------------------
_call_main:
@---------------------------------------------------------------------------------
	mov		lr, #0
	ldr		r3, =main
	bx		r3

	.pool
	.end
