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
  addi $sp, $sp, -72
  li $t1, 3
  sw $t1,  28($sp)
  lw $t1, 28($sp)
  sw $t1,  12($sp)
  li $t1, 8
  sw $t1,  28($sp)
  lw $t1, 28($sp)
  sw $t1,  16($sp)
  li $t1, 10
  sw $t1,  28($sp)
  lw $t1, 28($sp)
  sw $t1,  20($sp)
  lw $t1, 12($sp)
  sw $t1,  32($sp)
  lw $t1, 16($sp)
  sw $t1,  36($sp)
  lw $t1, 32($sp)
  lw $t2, 36($sp)
  mul $t3,$t1,$t2
  sw $t3, 28($sp)
  lw $t1, 28($sp)
  sw $t1,  44($sp)
  lw $t1, 20($sp)
  sw $t1,  48($sp)
  lw $t1, 44($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 40($sp)
  li $t1, 2
  sw $t1,  52($sp)
  lw $t1, 40($sp)
  sw $t1,  60($sp)
  lw $t1, 52($sp)
  sw $t1,  64($sp)
  lw $t1, 60($sp)
  lw $t2, 64($sp)
  div $t1, $t2
  mflo $t3
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  sw $t1,  68($sp)
  lw $t1, 68($sp)
  sw $t1,  24($sp)
  lw $a0, 24($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  li $t1, 0
  sw $t1,  28($sp)
  lw $v0,28($sp)
  jr $ra
  li $v0,10
  syscall
