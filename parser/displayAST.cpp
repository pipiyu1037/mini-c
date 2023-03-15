#include "ast.h"

#define YYSTYPE int   //������Ϊ�˰���parser.tab.hpp������������,�����ں���ʹ����س���
#include "parser.tab.hpp"
int AST::MaxVarSize=0;
int AST::MaxTempVarOffset=0;
//SymbolStackDef AST SymbolStack1={{}};

void space(int indent)
{
       for(int i=0;i<indent;i++) std::cout<<" ";
}
std::map <int,std::string> SymbolMap={{T_CHAR,"char"},{T_INT,"int"},{T_FLOAT,"float"},{T_VOID,"void"},{GT,">"},{GE,">="},{LT,"<"},
                             {LE,"<="},{EQ,"=="},{NE,"!="},{PLUS,"+"},{MINUS,"-"},{UPLUS,"*"},{UMINUS,"-"},{DIV,"/"},{ASSIGN,"="},
                             {AND,"&&"},{OR,"||"},{NOT,"!"}};
void ProgAST::DisplayAST(int indent)
{ //������ʾ����ExtDefs�еĸ����ⲿ����(�ⲿ��������ͺ�������)
    for(auto a:ExtDefs){
        a->DisplayAST(0);
    }
}

void ExtVarDefAST::DisplayAST(int indent)
{   //��ʾ�ⲿ��������
    std::cout<<"�ⲿ��������:"<<std::endl;
    std::cout<<"    �� �� ��: ";
    Type->DisplayAST(indent);
    std::cout<<std::endl<<"    �����б�: "<<std::endl;
    for(auto a:ExtVars)
    {
        a->DisplayAST(15);
    }
}

void BasicTypeAST::DisplayAST(int indent)
{  //��ʾ�������������Ŵ�
    std::cout.width(6);
    std::cout.setf(std::ios::left);
    std::cout<<SymbolMap[Type];
}

void VarDecAST::DisplayAST(int indent)
{ //��ʾ�ⲿ���������еĵ�������
    space(indent);
    std::cout<<Name;       //��ʾ������
    for(auto a:Dims)  //��������飬������ʾ��ά��С
        std::cout<<"["<<a<<"]";
    if (Exp)          //�г�ʼ�����ʽ
    {
        std::cout<<"= ";
        if (typeid(*Exp)==typeid(BinaryExprAST))
        {
            std::cout<<SymbolMap[((BinaryExprAST *)Exp)->Op]<<std::endl;
            ((BinaryExprAST *)Exp)->LeftExp->DisplayAST(indent+Name.length()+5);
            std::cout<<std::endl;
            ((BinaryExprAST *)Exp)->RightExp->DisplayAST(indent+Name.length()+5);
        }
        else Exp->DisplayAST(0);
    }
    std::cout<<std::endl;

}

void FuncDefAST::DisplayAST(int indent)
{  //��ʾ��������
    std::cout<<"��������:"<<std::endl;
    std::cout<<"    �������ͣ�";  //��ʾ��������ֵ����
    Type->DisplayAST(indent);
    std::cout<<std::endl<<"    �� �� ����"<<Name<<std::endl;   //��ʾ������
    std::cout<<"    �� �� ��";         //��ʾ�β�
    if (!Params.size())
        std::cout<<"��"<<std::endl;
    else
    {
        std::cout<<std::endl;
        for(auto a:Params)
            a->DisplayAST(14);
    }
    std::cout<<"    �� �� �壺  "<<std::endl;//��ʾ��������ֵ����
        Body->DisplayAST(0);
    std::cout<<std::endl;
}

void ParamAST::DisplayAST(int indent)
{  //��ʾ��ʽ����
    space(indent);
    Type->DisplayAST(indent);
    ParamName->DisplayAST(indent);
}



/***********���������***************/
void CompStmAST::DisplayAST(int indent)
{  //��ʾ�������
    space(indent);
    if (indent) std::cout<<"������䣺"<<std::endl;
    else indent=8;         //��ʾ������
    if (Decls.size())
    {
        space(indent+2);
        std::cout<<"˵������:"<<std::endl;
        for(auto a:Decls)
            a->DisplayAST(indent+4);

    }
    if (Stms.size())
    {
        space(indent+2);
        std::cout<<"��䲿��:"<<std::endl;
        for(auto a:Stms)
            a->DisplayAST(indent+4);
    }
}

void ExprStmAST::DisplayAST(int indent)
{  //��ʾ���ʽ���
    if (typeid(*Exp)==typeid(FuncCallAST))
        Exp->DisplayAST(indent);  //����ʽΪ�������ú�ӷֺŵĺ����������
    else
    {
        space(indent);
        std::cout<<"���ʽ��䣺"<<std::endl;
        Exp->DisplayAST(indent+4);
        std::cout<<std::endl;

    }
}

void IfStmAST::DisplayAST(int indent)
{   //��ʾ�������1
    space(indent);
    std::cout<<"if���:"<<std::endl;
    space(indent+2);
    std::cout<<"����:"<<std::endl;
    Cond->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"if�Ӿ�:"<<std::endl;
    ThenStm->DisplayAST(indent+8);
}

void IfElseStmAST::DisplayAST(int indent)
{   //��ʾ�������2
    space(indent);
    std::cout<<"if���:"<<std::endl;
    space(indent+2);
    std::cout<<"������"<<std::endl;
    Cond->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"if�Ӿ�:"<<std::endl;
    ThenStm->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"else�Ӿ�:"<<std::endl;
    ElseStm->DisplayAST(indent+8);
}

void WhileStmAST::DisplayAST(int indent)
{   //��ʾwhileѭ�����
    space(indent);
    std::cout<<"while���:"<<std::endl;
    space(indent+2);
    std::cout<<"ѭ��������"<<std::endl;
    Cond->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"ѭ���壺"<<std::endl;
    Body->DisplayAST(indent+8);
}
void ForStmAST::DisplayAST(int indent){
    space(indent);
    std::cout<<"for���:"<<std::endl;
    space(indent+2);
    std::cout<<"ѭ��ͷ��"<<std::endl;
    Head->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"ѭ��������"<<std::endl;
    Cond->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"ѭ��β��"<<std::endl;
    End->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"ѭ���壺"<<std::endl;
    Body->DisplayAST(indent+8);
}
void ReturnStmAST::DisplayAST(int indent)
{   //��ʾ�������
    space(indent);
    if (Exp)
    {
        std::cout<<std::endl;
        space(indent+2);
        std::cout<<"���ر��ʽ��"<<std::endl;
        Exp->DisplayAST(indent+4);
    }
    else std::cout<<"�������"<<std::endl;
    std::cout<<std::endl;
}

void BreakStmAST::DisplayAST(int indent){
    space(indent);
    cout<<"break;"<<endl;
}

void ContinueStmAST::DisplayAST(int indent){
    space(indent);
    cout<<"continue;"<<endl;
}

void DefAST::DisplayAST(int indent)
{   //��ʾ�ֲ�����
    space(indent);
    std::cout<<"���ͣ�";
    Type->DisplayAST(0);
    std::cout<<std::endl;
    space(indent);
    std::cout<<"�����б�: "<<std::endl;
    for(auto a:LocVars)  //��������飬������ʾ��ά��С
        a->DisplayAST(indent+10);
    std::cout<<std::endl;
}


/***********���ʽ���***************/
void AssignAST::DisplayAST(int indent)
{   //��ʾ��ֵ���ʽ
    space(indent);
    std::cout<<"��ֵ�������"<<SymbolMap[Op]<<std::endl;
    space(indent+2); std::cout<<"��ֵ���ʽ��"<<std::endl;
    LeftValExp->DisplayAST(indent+16);
    std::cout<<std::endl;
    space(indent+2); std::cout<<"��ֵ���ʽ��"<<std::endl;
    RightValExp->DisplayAST(indent+16);
}

void BinaryExprAST::DisplayAST(int indent)
{  //��ʾ��Ԫ������ʽ
    space(indent);
    std::cout<<"��Ԫ�������"+SymbolMap[Op]<<std::endl;
    space(indent);
    std::cout<<"���������"<<std::endl;
    LeftExp->DisplayAST(indent+4);
    std::cout<<std::endl;
    space(indent);
    std::cout<<"�Ҳ�������"<<std::endl;
    RightExp->DisplayAST(indent+4);
    std::cout<<std::endl;
}

void ConstAST::DisplayAST(int indent)
{   //��ʾ����
    space(indent);
    switch (Type)   //��ʾ����ֵ
    {
        case T_CHAR:    std::cout<<"\'"<<ConstVal.constCHAR<<"\'";break;
        case T_INT:     std::cout<<ConstVal.constINT;break;
        case T_FLOAT:   std::cout<<ConstVal.constFLOAT;break;
    }
}
void VarAST::DisplayAST(int indent)
{  //��ʾ����
    space(indent);
    std::cout<<Name;       //��ʾ�ⲿ������
    for(auto in:index){
        std::cout<<"["<<std::endl;
        in->DisplayAST(indent+4);
        std::cout<<"]"<<std::endl;
    } 
}

void ArrExpAST::DisplayAST(int indent){
    space(indent+2);
    std::cout<<"{";
    for(auto exp:IniList){
        if(typeid(*exp)==typeid(ArrExpAST)){
            exp->DisplayAST(0);
        }else{
            std::cout<<"TempVar";
        }
        space(1);
    }
    std::cout<<"}"<<std::endl;
}

void FuncCallAST::DisplayAST(int indent)
{  //��ʾ��������
    space(indent);
    std::cout<<"��������: ";
//    space(indent+4);
    std::cout<<"��������"<<Name;
    if (!Params.size())
    {
        std::cout<<" <��ʵ�α��ʽ>"<<std::endl;
        return;
    }
    std::cout<<std::endl;
    space(indent+10);
    std::cout<<Params.size()<<"��ʵ�α��ʽ:\n"<<std::endl;
    for(auto a:Params)
    {
        a->DisplayAST(indent+14);
    }
}

void UnaryExprAST::DisplayAST(int indent)
{  //��ʾ��Ŀ����
    space(indent);
    std::cout<<"��Ŀ��"<<SymbolMap[Op]<<std::endl;
    Exp->DisplayAST(indent+8);
}
