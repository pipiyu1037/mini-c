#include "ast.h"
#include <fstream>
#define YYSTYPE int   //此行是为了包含parser.tab.hpp不引起错误而加,可以在后面使用相关常量
#include "parser.tab.hpp"

void GenObject(std::list<IRCode> IRCodes)
{
    std::fstream ObjectFile;
    ObjectFile.open("ObjectFile.s",std::ios::out);

    /*预先给出read和write的目标代码*/
    ObjectFile<<".data\n";
    ObjectFile<<"_Prompt: .asciiz \"Enter an integer:  \"\n";
    ObjectFile<<"_ret: .asciiz \"\\n\"\n";
    ObjectFile<<".globl main\n";
    ObjectFile<<".text\n\n";
    ObjectFile<<"read:\n";
    ObjectFile<<"  li $v0,4\n";
    ObjectFile<<"  la $a0,_Prompt\n";
    ObjectFile<<"  syscall\n";
    ObjectFile<<"  li $v0,5\n";
    ObjectFile<<"  syscall\n";
    ObjectFile<<"  jr $ra\n\n";
    ObjectFile<<"write:\n";
    ObjectFile<<"  li $v0,1\n";
    ObjectFile<<"  syscall\n";
    ObjectFile<<"  li $v0,4\n";
    ObjectFile<<"  la $a0,_ret\n";
    ObjectFile<<"  syscall\n";
    ObjectFile<<"  move $v0,$0\n";
    ObjectFile<<"  jr $ra\n";

    for(std::list <IRCode>::iterator it=IRCodes.begin();it!=IRCodes.end();it++)
    {
      switch (it->Op)
        {   
            case READARRAY:
                ObjectFile<< "  lw $t1, "<<it->Opn2.Offset<<"($sp)"<<std::endl;
                ObjectFile<< "  add $t2,$sp,$t1"<<std::endl;    
                ObjectFile<< "  lw $t3, "<<it->Opn1.Offset<<"($t2)"<<std::endl;
                ObjectFile<< "  sw $t3, "<<it->Result.Offset<<"($sp)"<<std::endl;
                break;
            case WRITEARRAY:
                ObjectFile<< "  lw $t1, "<<it->Result.Offset<<"($sp)"<<std::endl;
                ObjectFile<< "  add $t2,$sp,$t1"<<std::endl;
                ObjectFile<< "  lw $t3, "<<it->Opn1.Offset<<"($sp)"<<std::endl;
                ObjectFile<< "  sw $t3, "<<it->Opn2.Offset<<"($t2)"<<std::endl;
            case ASSIGN:
                if (it->Opn1.Name==std::string("_CONST"))  //这里只考虑了整常数
                    ObjectFile<< "  li $t1, "<<it->Opn1.constINT<<std::endl;
                else       //这里只考虑了简单变量，数组则需要扩充
                    ObjectFile<< "  lw $t1, "<<it->Opn1.Offset<<"($sp)"<<std::endl;
                ObjectFile<< "  sw $t1,  "<<it->Result.Offset<<"($sp)"<<std::endl;
                break;
            case DPLUS: case DMINUS:
                ObjectFile<<"  lw $t1, "<<it->Opn1.Offset<<"($sp)"<<std::endl;
                ObjectFile<<"  li $t2, 1"<<std::endl;
                if (it->Op==DPLUS)       ObjectFile<< "  add $t3,$t1,$t2"<<std::endl;
                else if (it->Op==DMINUS) ObjectFile<< "  sub $t3,$t1,$t2"<<std::endl;
                ObjectFile<<"  sw $t3, "<<it->Result.Offset<<"($sp)"<<std::endl;
                break;
            case PLUS:
            case MINUS:
            case STAR:
            case DIV:
                if(it->Opn1.Name==std::string("_CONST")){ //这里修改是算数运算能支持直接数
                    ObjectFile<< "  li $t1, "<<it->Opn1.constINT<<std::endl;
                }else{
                    ObjectFile<< "  lw $t1, "<<it->Opn1.Offset<<"($sp)"<<std::endl;
                }
                if(it->Opn2.Name==std::string("_CONST")){
                    ObjectFile<< "  li $t2, "<<it->Opn2.constINT<<std::endl;
                }else{
                    ObjectFile<< "  lw $t2, "<<it->Opn2.Offset<<"($sp)"<<std::endl;
                }
                if (it->Op==PLUS)       ObjectFile<< "  add $t3,$t1,$t2"<<std::endl;
                else if (it->Op==MINUS) ObjectFile<< "  sub $t3,$t1,$t2"<<std::endl;
                else if (it->Op==STAR)  ObjectFile<< "  mul $t3,$t1,$t2"<<std::endl;
                else  {
                           ObjectFile<< "  div $t1, $t2"<<std::endl;
                           ObjectFile<< "  mflo $t3"<<std::endl;
                       }
                ObjectFile<< "  sw $t3, "<<it->Result.Offset<<"($sp)"<<std::endl;
                break;

            case RETURN:
                ObjectFile<< "  lw $v0,"<<it->Result.Offset<<"($sp)"<<std::endl; //返回值送到$v0
                ObjectFile<< "  jr $ra"<<std::endl;
                break;

            case FUNCTION:
                ObjectFile<< "\n"<<it->Result.Name<<":\n";
                if (it->Result.Name==std::string("main")) //特殊处理main
                    ObjectFile<< "  addi $sp, $sp, -"<<it->Result.Offset<<std::endl;
                break;
            case PARAM: break; //直接跳到后面一条
            case ARG:   break; //直接跳到后面一条,直到函数调用，回头提取实参值，传送到形参单元。
            case LABEL: ObjectFile<<it->Result.Name<<":"<<std::endl;  break;
            case GOTO:  ObjectFile<< "  j "<<it->Result.Name<<std::endl;  break;
            case END:
                ObjectFile<<"  li $v0,10\n  syscall"<<std::endl;
                break;
            case JLE:
            case JLT:
            case JGE:
            case JGT:
            case JEQ:
            case JNE:
                ObjectFile<< "  lw $t1, "<<it->Opn1.Offset<<"($sp)"<<std::endl;
                ObjectFile<< "  lw $t2, "<<it->Opn2.Offset<<"($sp)"<<std::endl;
                if (it->Op==JLE)      ObjectFile<< "  ble $t1,$t2,"<<it->Result.Name<<std::endl;
                else if (it->Op==JLT) ObjectFile<< "  blt $t1,$t2,"<<it->Result.Name<<std::endl;
                else if (it->Op==JGE) ObjectFile<< "  bge $t1,$t2,"<<it->Result.Name<<std::endl;
                else if (it->Op==JGT) ObjectFile<< "  bgt $t1,$t2,"<<it->Result.Name<<std::endl;
                else if (it->Op==JEQ)  ObjectFile<< "  beq $t1,$t2,"<<it->Result.Name<<std::endl;
                else                  ObjectFile<< "  bne $t1,$t2,"<<it->Result.Name<<std::endl;
                break;

            case CALL:  case CALL0:
                std::list<IRCode>::iterator it0=it; it0--;  //it0指向前面可能的ARG
                if (it->Opn1.Name==std::string("read"))//特殊处理read
                {
                    ObjectFile<< "  addi $sp, $sp, -4\n";
                    ObjectFile<< "  sw $ra,0($sp)\n";      //保留返回地址
                    ObjectFile<< "  jal read\n";           //保留返回地址
                    ObjectFile<< "  lw $ra,0($sp)\n";      //恢复返回地址
                    ObjectFile<< "  addi $sp, $sp, 4\n";
                    ObjectFile<< "  sw $v0, "<<it->Result.Offset<<"($sp)\n";
                    break;
                }
                if (it->Opn1.Name==std::string("write"))//特殊处理write
                {
                    ObjectFile<< "  lw $a0, "<<it0->Result.Offset<<"($sp)\n";  //前面ARG的偏移量
                    ObjectFile<< "  addi $sp, $sp, -4\n";
                    ObjectFile<< "  sw $ra,0($sp)\n";
                    ObjectFile<< "  jal write\n";
                    ObjectFile<< "  lw $ra,0($sp)\n";
                    ObjectFile<< "  addi $sp, $sp, 4\n";
                    break;
                }
                ObjectFile<< "  move $t0,$sp"<<std::endl; //保存当前函数的sp到$t0中，为了取实参表达式的值


                ObjectFile<< "  addi $sp, $sp, -"<<static_cast<FuncSymbol *>(it->Opn1.SymPtr)->ARSize<<std::endl;  //开活动记录空间
                ObjectFile<< "  sw $ra,0($sp)"<<std::endl;                      //保留返回地址
                while (it0->Op==ARG)                       //依次取参数值传递到形参单元中
                {
                    //将原栈帧的偏移it0->Result.Offset数据取出，送到新栈帧的it0->Opn1.Offset
                    ObjectFile<< "  lw $t1, "<<it0->Result.Offset<<"($t0)"<<std::endl; //取实参值到$t1中
                    ObjectFile<< "  sw $t1,"<<it0->Opn1.Offset<<"($sp)"<<std::endl;    //送到被调用函数的形参单元
                    it0--;
                }
                ObjectFile<<"  jal "<<it->Opn1.Name<<std::endl;      //跳转到被调用函数
                ObjectFile<<"  lw $ra,0($sp)"<<std::endl;            //恢复返回地址
                ObjectFile<<"  addi $sp,$sp,"<<((FuncSymbol *)(it->Opn1.SymPtr))->ARSize<<std::endl; //释放活动记录空间
                ObjectFile<<"  sw $v0,"<<it->Result.Offset<<"($sp)"<<std::endl;  //取返回值,如void函数，不需要取值
                break;
        }
    }
    ObjectFile.close();
}
