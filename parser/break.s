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
  addi $sp, $sp, -28
  li $t1, 6
  sw $t1,  16($sp)
  lw $t1, 16($sp)
  sw $t1,  12($sp)
Label_1:
  li $t1, 0
  sw $t1,  16($sp)
  lw $t1, 12($sp)
  sw $t1,  20($sp)
  lw $t1, 16($sp)
  sw $t1,  24($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  bgt $t1,$t2,Label_2
  j Label_3
Label_2:
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal read
  lw $ra,0($sp)
  addi $sp, $sp, 4
  sw $v0, 16($sp)
  lw $t1, 16($sp)
  sw $t1,  20($sp)
  lw $t1, 20($sp)
  sw $t1,  0($sp)
  li $t1, 2
  sw $t1,  16($sp)
  lw $t1, 0($sp)
  sw $t1,  20($sp)
  lw $t1, 16($sp)
  sw $t1,  24($sp)
  lw $t1, 20($sp)
  lw $t2, 24($sp)
  blt $t1,$t2,Label_4
  j Label_5
Label_4:
  j Label_3
Label_5:
  lw $t1, 12($sp)
  sw $t1,  20($sp)
  lw $t1, 20($sp)
  li $t2, 1
  sub $t3,$t1,$t2
  sw $t3, 16($sp)
  lw $t1, 16($sp)
  sw $t1,  12($sp)
  lw $a0, 12($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
  j Label_1
Label_3:
  li $v0,10
  syscall
