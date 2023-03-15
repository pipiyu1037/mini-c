#include "ast.h"

#define YYSTYPE int   //此行是为了包含parser.tab.hpp不引起错误而加,可以在后面使用相关常量
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
{ //依次显示向量ExtDefs中的各个外部定义(外部变量定义和函数定义)
    for(auto a:ExtDefs){
        a->DisplayAST(0);
    }
}

void ExtVarDefAST::DisplayAST(int indent)
{   //显示外部变量定义
    std::cout<<"外部变量定义:"<<std::endl;
    std::cout<<"    类 型 名: ";
    Type->DisplayAST(indent);
    std::cout<<std::endl<<"    变量列表: "<<std::endl;
    for(auto a:ExtVars)
    {
        a->DisplayAST(15);
    }
}

void BasicTypeAST::DisplayAST(int indent)
{  //显示基本类型名符号串
    std::cout.width(6);
    std::cout.setf(std::ios::left);
    std::cout<<SymbolMap[Type];
}

void VarDecAST::DisplayAST(int indent)
{ //显示外部变量定义中的单个变量
    space(indent);
    std::cout<<Name;       //显示变量名
    for(auto a:Dims)  //如果是数组，依次显示各维大小
        std::cout<<"["<<a<<"]";
    if (Exp)          //有初始化表达式
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
{  //显示函数定义
    std::cout<<"函数定义:"<<std::endl;
    std::cout<<"    返回类型：";  //显示函数返回值类型
    Type->DisplayAST(indent);
    std::cout<<std::endl<<"    函 数 名："<<Name<<std::endl;   //显示函数名
    std::cout<<"    形 参 表：";         //显示形参
    if (!Params.size())
        std::cout<<"无"<<std::endl;
    else
    {
        std::cout<<std::endl;
        for(auto a:Params)
            a->DisplayAST(14);
    }
    std::cout<<"    函 数 体：  "<<std::endl;//显示函数返回值类型
        Body->DisplayAST(0);
    std::cout<<std::endl;
}

void ParamAST::DisplayAST(int indent)
{  //显示形式参数
    space(indent);
    Type->DisplayAST(indent);
    ParamName->DisplayAST(indent);
}



/***********各种语句结点***************/
void CompStmAST::DisplayAST(int indent)
{  //显示复合语句
    space(indent);
    if (indent) std::cout<<"复合语句："<<std::endl;
    else indent=8;         //显示函数体
    if (Decls.size())
    {
        space(indent+2);
        std::cout<<"说明部分:"<<std::endl;
        for(auto a:Decls)
            a->DisplayAST(indent+4);

    }
    if (Stms.size())
    {
        space(indent+2);
        std::cout<<"语句部分:"<<std::endl;
        for(auto a:Stms)
            a->DisplayAST(indent+4);
    }
}

void ExprStmAST::DisplayAST(int indent)
{  //显示表达式语句
    if (typeid(*Exp)==typeid(FuncCallAST))
        Exp->DisplayAST(indent);  //对形式为函数调用后接分号的函数调用语句
    else
    {
        space(indent);
        std::cout<<"表达式语句："<<std::endl;
        Exp->DisplayAST(indent+4);
        std::cout<<std::endl;

    }
}

void IfStmAST::DisplayAST(int indent)
{   //显示条件语句1
    space(indent);
    std::cout<<"if语句:"<<std::endl;
    space(indent+2);
    std::cout<<"条件:"<<std::endl;
    Cond->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"if子句:"<<std::endl;
    ThenStm->DisplayAST(indent+8);
}

void IfElseStmAST::DisplayAST(int indent)
{   //显示条件语句2
    space(indent);
    std::cout<<"if语句:"<<std::endl;
    space(indent+2);
    std::cout<<"条件："<<std::endl;
    Cond->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"if子句:"<<std::endl;
    ThenStm->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"else子句:"<<std::endl;
    ElseStm->DisplayAST(indent+8);
}

void WhileStmAST::DisplayAST(int indent)
{   //显示while循环语句
    space(indent);
    std::cout<<"while语句:"<<std::endl;
    space(indent+2);
    std::cout<<"循环条件："<<std::endl;
    Cond->DisplayAST(indent+8);
    space(indent+2);
    std::cout<<"循环体："<<std::endl;
    Body->DisplayAST(indent+8);
}
void ForStmAST::DisplayAST(int indent){
    space(indent);
    std::cout<<"for语句:"<<std::endl;
    space(indent+2);
    std::cout<<"循环头："<<std::endl;
    Head->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"循环条件："<<std::endl;
    Cond->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"循环尾："<<std::endl;
    End->DisplayAST(indent+8);
    std::cout<<std::endl;
    space(indent+2);
    std::cout<<"循环体："<<std::endl;
    Body->DisplayAST(indent+8);
}
void ReturnStmAST::DisplayAST(int indent)
{   //显示返回语句
    space(indent);
    if (Exp)
    {
        std::cout<<std::endl;
        space(indent+2);
        std::cout<<"返回表达式："<<std::endl;
        Exp->DisplayAST(indent+4);
    }
    else std::cout<<"返回语句"<<std::endl;
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
{   //显示局部变量
    space(indent);
    std::cout<<"类型：";
    Type->DisplayAST(0);
    std::cout<<std::endl;
    space(indent);
    std::cout<<"变量列表: "<<std::endl;
    for(auto a:LocVars)  //如果是数组，依次显示各维大小
        a->DisplayAST(indent+10);
    std::cout<<std::endl;
}


/***********表达式结点***************/
void AssignAST::DisplayAST(int indent)
{   //显示赋值表达式
    space(indent);
    std::cout<<"赋值运算符："<<SymbolMap[Op]<<std::endl;
    space(indent+2); std::cout<<"左值表达式："<<std::endl;
    LeftValExp->DisplayAST(indent+16);
    std::cout<<std::endl;
    space(indent+2); std::cout<<"右值表达式："<<std::endl;
    RightValExp->DisplayAST(indent+16);
}

void BinaryExprAST::DisplayAST(int indent)
{  //显示二元运算表达式
    space(indent);
    std::cout<<"二元运算符："+SymbolMap[Op]<<std::endl;
    space(indent);
    std::cout<<"左操作数："<<std::endl;
    LeftExp->DisplayAST(indent+4);
    std::cout<<std::endl;
    space(indent);
    std::cout<<"右操作数："<<std::endl;
    RightExp->DisplayAST(indent+4);
    std::cout<<std::endl;
}

void ConstAST::DisplayAST(int indent)
{   //显示常量
    space(indent);
    switch (Type)   //显示常量值
    {
        case T_CHAR:    std::cout<<"\'"<<ConstVal.constCHAR<<"\'";break;
        case T_INT:     std::cout<<ConstVal.constINT;break;
        case T_FLOAT:   std::cout<<ConstVal.constFLOAT;break;
    }
}
void VarAST::DisplayAST(int indent)
{  //显示变量
    space(indent);
    std::cout<<Name;       //显示外部变量名
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
{  //显示函数调用
    space(indent);
    std::cout<<"函数调用: ";
//    space(indent+4);
    std::cout<<"函数名："<<Name;
    if (!Params.size())
    {
        std::cout<<" <无实参表达式>"<<std::endl;
        return;
    }
    std::cout<<std::endl;
    space(indent+10);
    std::cout<<Params.size()<<"个实参表达式:\n"<<std::endl;
    for(auto a:Params)
    {
        a->DisplayAST(indent+14);
    }
}

void UnaryExprAST::DisplayAST(int indent)
{  //显示单目运算
    space(indent);
    std::cout<<"单目："<<SymbolMap[Op]<<std::endl;
    Exp->DisplayAST(indent+8);
}
