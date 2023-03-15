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

main:
  addi $sp, $sp, -112
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 2
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  88($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 88($sp)
  sw $t3, 24($t2)
  lw $t1, 88($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  80($sp)
  li $t1, 0
  sw $t1,  92($sp)
  lw $t1, 92($sp)
  sw $t1,  84($sp)
  li $t1, 4
  lw $t2, 84($sp)
  mul $t3,$t1,$t2
  sw $t3, 88($sp)
  lw $t1, 88($sp)
  lw $t2, 80($sp)
  add $t3,$t1,$t2
  sw $t3, 80($sp)
  lw $t1, 80($sp)
  add $t2,$sp,$t1
  lw $t3, 24($t2)
  sw $t3, 72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 1
  sw $t1,  96($sp)
  lw $t1, 96($sp)
  sw $t1,  100($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 100($sp)
  sw $t3, 12($t2)
  lw $t1, 100($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 1
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 1
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  88($sp)
  li $t1, 0
  sw $t1,  100($sp)
  lw $t1, 100($sp)
  sw $t1,  92($sp)
  li $t1, 4
  lw $t2, 92($sp)
  mul $t3,$t1,$t2
  sw $t3, 96($sp)
  lw $t1, 96($sp)
  lw $t2, 88($sp)
  add $t3,$t1,$t2
  sw $t3, 88($sp)
  lw $t1, 88($sp)
  add $t2,$sp,$t1
  lw $t3, 24($t2)
  sw $t3, 104($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 104($sp)
  sw $t3, 32($t2)
  lw $t1, 104($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  88($sp)
  li $t1, 2
  sw $t1,  100($sp)
  lw $t1, 100($sp)
  sw $t1,  92($sp)
  li $t1, 4
  lw $t2, 92($sp)
  mul $t3,$t1,$t2
  sw $t3, 96($sp)
  lw $t1, 96($sp)
  lw $t2, 88($sp)
  add $t3,$t1,$t2
  sw $t3, 88($sp)
  lw $t1, 88($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 104($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 104($sp)
  sw $t3, 32($t2)
  lw $t1, 104($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 1
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  88($sp)
  li $t1, 0
  sw $t1,  100($sp)
  lw $t1, 100($sp)
  sw $t1,  92($sp)
  li $t1, 4
  lw $t2, 92($sp)
  mul $t3,$t1,$t2
  sw $t3, 96($sp)
  lw $t1, 96($sp)
  lw $t2, 88($sp)
  add $t3,$t1,$t2
  sw $t3, 88($sp)
  li $t1, 0
  sw $t1,  104($sp)
  lw $t1, 104($sp)
  sw $t1,  92($sp)
  li $t1, 12
  lw $t2, 92($sp)
  mul $t3,$t1,$t2
  sw $t3, 96($sp)
  lw $t1, 96($sp)
  lw $t2, 88($sp)
  add $t3,$t1,$t2
  sw $t3, 88($sp)
  lw $t1, 88($sp)
  add $t2,$sp,$t1
  lw $t3, 32($t2)
  sw $t3, 108($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 108($sp)
  sw $t3, 32($t2)
  lw $t1, 108($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 2
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 112($sp)
  lw $a0, 112($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 32($t2)
  sw $t3, 112($sp)
  lw $a0, 112($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 1
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 0
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 32($t2)
  sw $t3, 112($sp)
  lw $a0, 112($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 1
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 1
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 32($t2)
  sw $t3, 92($sp)
  lw $t1, 92($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 88($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 88($sp)
  sw $t3, 32($t2)
  lw $t1, 88($sp)
  sw $t1,  68($sp)
  li $t1, 0
  sw $t1,  68($sp)
  li $t1, 1
  sw $t1,  80($sp)
  lw $t1, 80($sp)
  sw $t1,  72($sp)
  li $t1, 4
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  li $t1, 1
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  sw $t1,  72($sp)
  li $t1, 12
  lw $t2, 72($sp)
  mul $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  lw $t2, 68($sp)
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  add $t2,$sp,$t1
  lw $t3, 32($t2)
  sw $t3, 112($sp)
  lw $a0, 112($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  68($sp)
  lw $v0,68($sp)
  jr $ra
  li $v0,10
  syscall
