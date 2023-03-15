#include "ast.h"
#include <fstream>
#define YYSTYPE int   //������Ϊ�˰���parser.tab.hpp������������,�����ں���ʹ����س���
#include "parser.tab.hpp"

void GenObject(std::list<IRCode> IRCodes)
{
    std::fstream ObjectFile;
    ObjectFile.open("ObjectFile.s",std::ios::out);

    /*Ԥ�ȸ���read��write��Ŀ�����*/
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
                if (it->Opn1.Name==std::string("_CONST"))  //����ֻ������������
                    ObjectFile<< "  li $t1, "<<it->Opn1.constINT<<std::endl;
                else       //����ֻ�����˼򵥱�������������Ҫ����
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
                if(it->Opn1.Name==std::string("_CONST")){ //�����޸�������������֧��ֱ����
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
                ObjectFile<< "  lw $v0,"<<it->Result.Offset<<"($sp)"<<std::endl; //����ֵ�͵�$v0
                ObjectFile<< "  jr $ra"<<std::endl;
                break;

            case FUNCTION:
                ObjectFile<< "\n"<<it->Result.Name<<":\n";
                if (it->Result.Name==std::string("main")) //���⴦��main
                    ObjectFile<< "  addi $sp, $sp, -"<<it->Result.Offset<<std::endl;
                break;
            case PARAM: break; //ֱ����������һ��
            case ARG:   break; //ֱ����������һ��,ֱ���������ã���ͷ��ȡʵ��ֵ�����͵��βε�Ԫ��
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
                std::list<IRCode>::iterator it0=it; it0--;  //it0ָ��ǰ����ܵ�ARG
                if (it->Opn1.Name==std::string("read"))//���⴦��read
                {
                    ObjectFile<< "  addi $sp, $sp, -4\n";
                    ObjectFile<< "  sw $ra,0($sp)\n";      //�������ص�ַ
                    ObjectFile<< "  jal read\n";           //�������ص�ַ
                    ObjectFile<< "  lw $ra,0($sp)\n";      //�ָ����ص�ַ
                    ObjectFile<< "  addi $sp, $sp, 4\n";
                    ObjectFile<< "  sw $v0, "<<it->Result.Offset<<"($sp)\n";
                    break;
                }
                if (it->Opn1.Name==std::string("write"))//���⴦��write
                {
                    ObjectFile<< "  lw $a0, "<<it0->Result.Offset<<"($sp)\n";  //ǰ��ARG��ƫ����
                    ObjectFile<< "  addi $sp, $sp, -4\n";
                    ObjectFile<< "  sw $ra,0($sp)\n";
                    ObjectFile<< "  jal write\n";
                    ObjectFile<< "  lw $ra,0($sp)\n";
                    ObjectFile<< "  addi $sp, $sp, 4\n";
                    break;
                }
                ObjectFile<< "  move $t0,$sp"<<std::endl; //���浱ǰ������sp��$t0�У�Ϊ��ȡʵ�α��ʽ��ֵ


                ObjectFile<< "  addi $sp, $sp, -"<<static_cast<FuncSymbol *>(it->Opn1.SymPtr)->ARSize<<std::endl;  //�����¼�ռ�
                ObjectFile<< "  sw $ra,0($sp)"<<std::endl;                      //�������ص�ַ
                while (it0->Op==ARG)                       //����ȡ����ֵ���ݵ��βε�Ԫ��
                {
                    //��ԭջ֡��ƫ��it0->Result.Offset����ȡ�����͵���ջ֡��it0->Opn1.Offset
                    ObjectFile<< "  lw $t1, "<<it0->Result.Offset<<"($t0)"<<std::endl; //ȡʵ��ֵ��$t1��
                    ObjectFile<< "  sw $t1,"<<it0->Opn1.Offset<<"($sp)"<<std::endl;    //�͵������ú������βε�Ԫ
                    it0--;
                }
                ObjectFile<<"  jal "<<it->Opn1.Name<<std::endl;      //��ת�������ú���
                ObjectFile<<"  lw $ra,0($sp)"<<std::endl;            //�ָ����ص�ַ
                ObjectFile<<"  addi $sp,$sp,"<<((FuncSymbol *)(it->Opn1.SymPtr))->ARSize<<std::endl; //�ͷŻ��¼�ռ�
                ObjectFile<<"  sw $v0,"<<it->Result.Offset<<"($sp)"<<std::endl;  //ȡ����ֵ,��void����������Ҫȡֵ
                break;
        }
    }
    ObjectFile.close();
}
