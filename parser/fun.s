.data
_Prompt: .asciiz "Enter an integer:  "
_ret: .asciiz "\n"
.globl main
.text

read:
  li $v0,4
  la $a0,_Prompt
  syscall
  li $v0,5
  syscall
  jr $ra

write:
  li $v0,1
  syscall
  li $v0,4
  la $a0,_ret
  syscall
  move $v0,$0
  jr $ra

max:
  lw $t1, 12($sp)
  sw $t1,  20($sp)
  lw $t1, 16($sp)
  sw $t1,  24($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  bgt $t1,$t2,Label_1
  j Label_2
Label_1:
  lw $v0,12($sp)
  jr $ra
  j Label_3
Label_2:
  lw $v0,16($sp)
  jr $ra
Label_3:

fi:
  li $t1, 1
  sw $t1,  16($sp)
  lw $t1, 12($sp)
  sw $t1,  20($sp)
  lw $t1, 16($sp)
  sw $t1,  24($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  beq $t1,$t2,Label_4
  j Label_5
Label_4:
  li $t1, 1
  sw $t1,  16($sp)
  lw $v0,16($sp)
  jr $ra
Label_5:
  li $t1, 2
  sw $t1,  16($sp)
  lw $t1, 12($sp)
  sw $t1,  20($sp)
  lw $t1, 16($sp)
  sw $t1,  24($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  beq $t1,$t2,Label_6
  j Label_7
Label_6:
  li $t1, 1
  sw $t1,  16($sp)
  lw $v0,16($sp)
  jr $ra
Label_7:
  li $t1, 1
  sw $t1,  16($sp)
  lw $t1, 12($sp)
  sw $t1,  24($sp)
  lw $t1, 16($sp)
  sw $t1,  28($sp)
  lw $t1, 24($sp)
  lw $t2, 28($sp)
  sub $t3,$t1,$t2
  sw $t3, 20($sp)
  move $t0,$sp
  addi $sp, $sp, -68
  sw $ra,0($sp)
  lw $t1, 20($t0)
  sw $t1,12($sp)
  jal fi
  lw $ra,0($sp)
  addi $sp,$sp,68
  sw $v0,32($sp)
  li $t1, 2
  sw $t1,  36($sp)
  lw $t1, 12($sp)
  sw $t1,  44($sp)
  lw $t1, 36($sp)
  sw $t1,  48($sp)
  lw $t1, 44($sp)
  lw $t2, 48($sp)
  sub $t3,$t1,$t2
  sw $t3, 40($sp)
  move $t0,$sp
  addi $sp, $sp, -68
  sw $ra,0($sp)
  lw $t1, 40($t0)
  sw $t1,12($sp)
  jal fi
  lw $ra,0($sp)
  addi $sp,$sp,68
  sw $v0,52($sp)
  lw $t1, 32($sp)
  sw $t1,  60($sp)
  lw $t1, 52($sp)
  sw $t1,  64($sp)
  lw $t1, 60($sp)
  lw $t2, 64($sp)
  add $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $v0,56($sp)
  jr $ra

main:
  addi $sp, $sp, -92
  li $t1, 0
  sw $t1,  40($sp)
  li $t1, 0
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  44($sp)
  li $t1, 4
  lw $t2, 44($sp)
  mul $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  li $t1, 2
  sw $t1,  56($sp)
  lw $t1, 56($sp)
  sw $t1,  60($sp)
  lw $t1, 40($sp)
  add $t2,$sp,$t1
  lw $t3, 60($sp)
  sw $t3, 12($t2)
  lw $t1, 60($sp)
  sw $t1,  40($sp)
  li $t1, 0
  sw $t1,  40($sp)
  li $t1, 1
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  44($sp)
  li $t1, 4
  lw $t2, 44($sp)
  mul $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  li $t1, 3
  sw $t1,  56($sp)
  lw $t1, 56($sp)
  sw $t1,  60($sp)
  lw $t1, 40($sp)
  add $t2,$sp,$t1
  lw $t3, 60($sp)
  sw $t3, 12($t2)
  lw $t1, 60($sp)
  sw $t1,  40($sp)
  li $t1, 0
  sw $t1,  40($sp)
  lw $t1, 40($sp)
  sw $t1,  44($sp)
  lw $t1, 44($sp)
  sw $t1,  32($sp)
  li $t1, 1
  sw $t1,  40($sp)
  lw $t1, 40($sp)
  sw $t1,  44($sp)
  lw $t1, 44($sp)
  sw $t1,  36($sp)
  li $t1, 0
  sw $t1,  40($sp)
  lw $t1, 32($sp)
  sw $t1,  44($sp)
  li $t1, 4
  lw $t2, 44($sp)
  mul $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  li $t1, 0
  sw $t1,  52($sp)
  lw $t1, 36($sp)
  sw $t1,  56($sp)
  li $t1, 4
  lw $t2, 56($sp)
  mul $t3,$t1,$t2
  sw $t3, 60($sp)
  lw $t1, 60($sp)
  lw $t2, 52($sp)
  add $t3,$t1,$t2
  sw $t3, 52($sp)
  lw $t1, 40($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 64($sp)
  lw $t1, 52($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 68($sp)
  lw $t1, 64($sp)
  lw $t2, 68($sp)
  blt $t1,$t2,Label_8
  j Label_9
Label_8:
  li $t1, 0
  sw $t1,  40($sp)
  lw $t1, 32($sp)
  sw $t1,  44($sp)
  li $t1, 4
  lw $t2, 44($sp)
  mul $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  lw $t1, 40($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 92($sp)
  lw $a0, 92($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
Label_9:
  li $t1, 0
  sw $t1,  40($sp)
  lw $t1, 36($sp)
  sw $t1,  44($sp)
  li $t1, 4
  lw $t2, 44($sp)
  mul $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  lw $t2, 40($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  lw $t1, 40($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 92($sp)
  lw $a0, 92($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  40($sp)
  lw $t1, 40($sp)
  sw $t1,  44($sp)
  lw $t1, 44($sp)
  sw $t1,  32($sp)
Label_10:
  li $t1, 5
  sw $t1,  48($sp)
  lw $t1, 32($sp)
  sw $t1,  52($sp)
  lw $t1, 48($sp)
  sw $t1,  56($sp)
  lw $t1, 52($sp)
  lw $t2, 56($sp)
  blt $t1,$t2,Label_11
  j Label_13
Label_11:
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal read
  lw $ra,0($sp)
  addi $sp, $sp, 4
  sw $v0, 40($sp)
  lw $t1, 40($sp)
  sw $t1,  44($sp)
  lw $t1, 44($sp)
  sw $t1,  36($sp)
  move $t0,$sp
  addi $sp, $sp, -68
  sw $ra,0($sp)
  lw $t1, 36($t0)
  sw $t1,12($sp)
  jal fi
  lw $ra,0($sp)
  addi $sp,$sp,68
  sw $v0,40($sp)
  lw $a0, 40($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
Label_12:
  lw $t1, 32($sp)
  sw $t1,  64($sp)
  lw $t1, 64($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 60($sp)
  lw $t1, 60($sp)
  sw $t1,  32($sp)
  j Label_10
Label_13:
  li $t1, 0
  sw $t1,  40($sp)
  lw $v0,40($sp)
  jr $ra
  li $v0,10
  syscall
