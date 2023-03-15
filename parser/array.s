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
  addi $sp, $sp, -96
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 48($sp)
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  36($sp)
Label_1:
  li $t1, 6
  sw $t1,  56($sp)
  lw $t1, 36($sp)
  sw $t1,  60($sp)
  lw $t1, 56($sp)
  sw $t1,  64($sp)
  lw $t1, 60($sp)
  lw $t2, 64($sp)
  blt $t1,$t2,Label_2
  j Label_4
Label_2:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 36($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal read
  lw $ra,0($sp)
  addi $sp, $sp, 4
  sw $v0, 60($sp)
  lw $t1, 60($sp)
  sw $t1,  64($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 64($sp)
  sw $t3, 12($t2)
  lw $t1, 64($sp)
  sw $t1,  48($sp)
Label_3:
  lw $t1, 36($sp)
  sw $t1,  72($sp)
  lw $t1, 72($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  sw $t1,  36($sp)
  j Label_1
Label_4:
  li $t1, 1
  sw $t1,  48($sp)
  lw $t1, 48($sp)
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  36($sp)
Label_5:
  li $t1, 5
  sw $t1,  56($sp)
  lw $t1, 36($sp)
  sw $t1,  60($sp)
  lw $t1, 56($sp)
  sw $t1,  64($sp)
  lw $t1, 60($sp)
  lw $t2, 64($sp)
  blt $t1,$t2,Label_6
  j Label_8
Label_6:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 48($sp)
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  40($sp)
Label_9:
  li $t1, 6
  sw $t1,  56($sp)
  lw $t1, 56($sp)
  sw $t1,  64($sp)
  lw $t1, 36($sp)
  sw $t1,  68($sp)
  lw $t1, 64($sp)
  lw $t2, 68($sp)
  sub $t3,$t1,$t2
  sw $t3, 60($sp)
  lw $t1, 40($sp)
  sw $t1,  72($sp)
  lw $t1, 60($sp)
  sw $t1,  76($sp)
  lw $t1, 72($sp)
  lw $t2, 76($sp)
  blt $t1,$t2,Label_10
  j Label_12
Label_10:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 40($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  li $t1, 0
  sw $t1,  60($sp)
  li $t1, 1
  sw $t1,  72($sp)
  lw $t1, 40($sp)
  sw $t1,  80($sp)
  lw $t1, 72($sp)
  sw $t1,  84($sp)
  lw $t1, 80($sp)
  lw $t2, 84($sp)
  add $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  sw $t1,  64($sp)
  li $t1, 4
  lw $t2, 64($sp)
  mul $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  lw $t2, 60($sp)
  add $t3,$t1,$t2
  sw $t3, 60($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 88($sp)
  lw $t1, 60($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 92($sp)
  lw $t1, 88($sp)
  lw $t2, 92($sp)
  bgt $t1,$t2,Label_13
  j Label_14
Label_13:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 40($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 60($sp)
  lw $t1, 60($sp)
  sw $t1,  44($sp)
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 40($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  li $t1, 0
  sw $t1,  60($sp)
  li $t1, 1
  sw $t1,  72($sp)
  lw $t1, 40($sp)
  sw $t1,  80($sp)
  lw $t1, 72($sp)
  sw $t1,  84($sp)
  lw $t1, 80($sp)
  lw $t2, 84($sp)
  add $t3,$t1,$t2
  sw $t3, 76($sp)
  lw $t1, 76($sp)
  sw $t1,  64($sp)
  li $t1, 4
  lw $t2, 64($sp)
  mul $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  lw $t2, 60($sp)
  add $t3,$t1,$t2
  sw $t3, 60($sp)
  lw $t1, 60($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 88($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 88($sp)
  sw $t3, 12($t2)
  lw $t1, 88($sp)
  sw $t1,  48($sp)
  li $t1, 0
  sw $t1,  48($sp)
  li $t1, 1
  sw $t1,  60($sp)
  lw $t1, 40($sp)
  sw $t1,  68($sp)
  lw $t1, 60($sp)
  sw $t1,  72($sp)
  lw $t1, 68($sp)
  lw $t2, 72($sp)
  add $t3,$t1,$t2
  sw $t3, 64($sp)
  lw $t1, 64($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 44($sp)
  sw $t1,  76($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 76($sp)
  sw $t3, 12($t2)
  lw $t1, 76($sp)
  sw $t1,  48($sp)
Label_14:
Label_11:
  lw $t1, 40($sp)
  sw $t1,  84($sp)
  lw $t1, 84($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 80($sp)
  lw $t1, 80($sp)
  sw $t1,  40($sp)
  j Label_9
Label_12:
Label_7:
  lw $t1, 36($sp)
  sw $t1,  72($sp)
  lw $t1, 72($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  sw $t1,  36($sp)
  j Label_5
Label_8:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 48($sp)
  sw $t1,  52($sp)
  lw $t1, 52($sp)
  sw $t1,  36($sp)
Label_15:
  li $t1, 6
  sw $t1,  56($sp)
  lw $t1, 36($sp)
  sw $t1,  60($sp)
  lw $t1, 56($sp)
  sw $t1,  64($sp)
  lw $t1, 60($sp)
  lw $t2, 64($sp)
  blt $t1,$t2,Label_16
  j Label_18
Label_16:
  li $t1, 0
  sw $t1,  48($sp)
  lw $t1, 36($sp)
  sw $t1,  52($sp)
  li $t1, 4
  lw $t2, 52($sp)
  mul $t3,$t1,$t2
  sw $t3, 56($sp)
  lw $t1, 56($sp)
  lw $t2, 48($sp)
  add $t3,$t1,$t2
  sw $t3, 48($sp)
  lw $t1, 48($sp)
  add $t2,$sp,$t1
  lw $t3, 12($t2)
  sw $t3, 96($sp)
  lw $a0, 96($sp)
  addi $sp, $sp, -4
  sw $ra,0($sp)
  jal write
  lw $ra,0($sp)
  addi $sp, $sp, 4
Label_17:
  lw $t1, 36($sp)
  sw $t1,  72($sp)
  lw $t1, 72($sp)
  li $t2, 1
  add $t3,$t1,$t2
  sw $t3, 68($sp)
  lw $t1, 68($sp)
  sw $t1,  36($sp)
  j Label_15
Label_18:
  li $t1, 1
  sw $t1,  48($sp)
  lw $v0,48($sp)
  jr $ra
  li $v0,10
  syscall
