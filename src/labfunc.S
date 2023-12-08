# This file contains relevant assembly code from the labs in IS1500

# Authors: 
# Axel Isaksson,
# F Lundevall,
# Elias Hollstrand,
# Mattias Kvist

# Macros written by Axel Isaksson and F Lundevall
# Interrupt enable written by Elias Hollstrand and Mattias Kvist 2023

# Date: 2023-12-07

# For copyright and licensing, see file COPYING 

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

    .data
# Enable interrupts by executing the "ei" instruction
.global enable_interrupt

    .text
enable_interrupt:
    ei         # Enable interrupts
    nop        # No operation (required delay for "ei" to take effect)
    jr $ra     # Return from the function
    nop
