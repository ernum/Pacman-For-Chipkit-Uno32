  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
.macro  PUSH reg
  	addi  $sp,$sp,-4
      	sw    \reg,0($sp)
.endm

.macro  POP reg
    	lw    \reg,0($sp)
      	addi  $sp,$sp,4
.endm

	.data
	.text
hexasc:
	andi $t0,$a0,0xf	# Bitwise and with 0xF keeps the last 4 bits (nibble)
	addi $t1, $t0, -9	# Difference of the number - 9.
	
	bgt $t1,0,else		# Will go to else label if the input is larger than nine
	addi $v0, $t0, 0x30
	
	jr $ra
	nop
	
else:
	addi $v0, $t0, 0x37
	
	jr $ra
	nop
	
.global delay
delay:
	PUSH $ra
	
	move $t0,$a0		# Time in ms
	li $t1,0		# The i variable for the loop
	li $t2,3500000		# The constant for the loop

whileloop:
	ble $t0, $0, L1
	addi $t0, $t0, -1
forloop:
	bge $t1, $t2, whileloop
	addi $t1, $t1, 1
	
	j forloop
	
L1:	
	POP $ra
	
	jr $ra
 	nop
 
.global time2string	
 time2string:
 	
 	PUSH $ra		# Push the return address associated to the main function
 	PUSH $s0
 	PUSH $s1
 	
 	move $s0,$a0 		# The memory addrss for the output of time2string
 	move $s1,$a1		# The NBCD-coded digits

 	
 	move $a0, $s1
 	jal tick
 	andi $t0, $s1, 0xf000
	andi $t1, $a0, 0xf000
	bne $t0, $t1, hourstring
 	
 	
 	andi $a0,$s1,0xf000	# Getting the byte at the 4th lsb
 	srl $a0, $a0,12	# Shifting the byte to the position of the 1st lsb so that hexasc can process it
 	jal hexasc		# Getting ASCII code (Not saving the s register because it's caller saved)
 	nop
 	sb $v0,0($s0)		# Write the code to the area in memory pointed to by register $a0
 	
 	andi $a0,$s1,0xf00	# Getting the byte at the 3rd lsb
 	srl $a0, $a0,8		# Shifting the byte to the position of the 1st lsb so that hexasc can process i
 	jal hexasc		# Getting ASCII code (Not saving the s register because it's caller saved)
 	nop
 	sb $v0,1($s0)		# Write the code to the area in memory pointed to by register $a0

 	li $t0,0x3A		# Load immidate hex code for colon character
 	sb $t0,2($s0)		# Write the code to the area in memory pointed to by register $a0
 	
 	andi $a0,$s1,0xf0	# Getting the byte at the 2nd lsb
 	srl $a0, $a0,4		# Shifting the byte to the position of the 1st lsb so that hexasc can process i 
 	jal hexasc		# Getting ASCII code (Not saving the s register because it's caller saved)
 	nop
 	sb $v0,3($s0)		# Write the code to the area in memory pointed to by register $a0
 	
 	andi $a0,$s1,0xf	# Getting the byte at the 1st lsb	
 	jal hexasc		# Getting ASCII code (Not saving the s register because it's caller saved)
 	nop
 	sb $v0,4($s0)		# Write the code to the area in memory pointed to by register $a0
 	
 	li $t0, 0x00		# The null byte use to eliminate any trailing characters
 	sb $t0,5($s0)		# Write the code to the area in memory pointed to by register $a0
 	
 	POP $s1
 	POP $s0
 	POP $ra
 	
 	jr $ra
 	nop
 	
 hourstring:
    	li $t0, 0x52554f48
    	sw $t0, 0($s0)
    	
    	li $t0, 0x00		# The null byte use to eliminate any trailing characters
 	sb $t0,5($s0)		# Write the code to the area in memory pointed to by register $a0
    	
    	POP $s1
 	POP $s0
 	POP $ra
 	
 	jr $ra
 	nop
 
 

