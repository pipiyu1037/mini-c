#ifndef AST_H
#define AST_H

#include <cstdlib>
#include <cctype>
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <list>
#include <map>

class AST;
class ProgAST;
class TypeAST;
class ExtDefAST;
class VarDecAST;
class ParamAST;
class CompStmAST;
class ExpAST;

class VarSymbol;
class FuncSymbol;
class SymbolsInAScope;
class Opn;

extern std::map <int,std::string> SymbolMap;
extern std::map <int,int>    TypeWidth;

#ifndef STD
#define STD
using namespace std;
#endif

enum BasicTypes {
	T_CHAR,
	T_INT,
	T_FLOAT,
    T_VOID
};

typedef struct { int Line,Column;  std::string ErrMsg; }Error;
class Errors            //用来记录语法、语义错误
{
    public:
        static std::vector <Error> Errs;
        static  void ErrorAdd(int Line,int Column,std::string ErrMsg);
        static void ErrorsDisplay();
        static inline bool IsEmpty(){ return Errs.size()==0;}
};

/**************符号表定义**********************/
class Symbol{
    public:
        std::string Name;
        BasicTypes Type;        //符号类型，目前仅基本类型T_CHAR,T_INT,T_FLOAT，T_VOID
        char Kind;       //符号种类：基本变量V，函数名F，参数P，数组A等
};

class VarSymbol:public Symbol{
    public:
        std::string Alias;   //别名，为解决中间代码中，作用域嵌套变量同名的显示时的二义性问题
        int Offset;     //变量在对应AR中的偏移量
        int ARsize;
        std::vector<int> dims;
};

class FuncSymbol:public Symbol{
    public:
        int ARSize;       //函数AR的大小，作为调用时分配单元的依据
        int ParamNum;     //形式参数个数
        SymbolsInAScope *ParamPtr;  //指向参数的符号表
};


class SymbolsInAScope{    //单一作用域的符号名，每个复合语句对应一个符号表
    public:
        std::vector<Symbol*> Symbols;
};

class SymbolStackDef{//符号表类定义,栈结构栈底为全局变量和函数定义，每个复合语句对应一张局部符号表
    public:
        std::vector <SymbolsInAScope *> Symbols;
        Symbol * LocateNameCurrent(std::string Name);//在当前作用域中查找该符号是否有定义
        Symbol * LocateNameGlobal(std::string Name); //由内向外，在全部作用域中查找该符号是否有定义
};

/**************中间代码（四元式）定义**********************/
class Opn
{
    public:
        std::string  Name;       //变量别名（为空时表示常量）或函数名
        int     Type;
        union
        {
            int     Offset;     //AR中的偏移量
            void    *SymPtr;    //符号表指针
            char  constCHAR;
            int   constINT;
            float constFLOAT;
        };

        Opn(std::string Name,int Type,int Offset):Name(Name),Type(Type),Offset(Offset){};
        Opn(){};
};

class IRCode       //四元式结构
{
    public:
        int Op;
        Opn Opn1;
        Opn Opn2;
        Opn Result;

    IRCode(int Op,Opn Opn1,Opn Opn2,Opn Result):Op(Op),Opn1(Opn1),Opn2(Opn2),Result(Result){}
};


/**************抽象语法树结点类型定义**********************/
class AST {                          //所有结点的基类
    public:
        int Line,Column;   //源程序中，语法单位所在位置
        int Offset;
        std::list<IRCode> IRCodes;

        virtual void DisplayAST(int indent){}
        virtual void Semantics(int &Offset){}  //静态语义检查，
        virtual void GenIR() {};               //生成中间代码

        static int MaxVarSize;
        static int MaxTempVarOffset;          //表达式求值时临时变量需要的最大空间
        static SymbolStackDef SymbolStack;     //符号表
        //static
};
class ProgAST :public AST{                     //程序结点，程序由多个外部定义组成
    public:
        std::vector<ExtDefAST*> ExtDefs;            //外部定义序列

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        void Semantics0();
        void GenIR() override;
        //semantic attributes
        SymbolsInAScope *GlobalSymbolTable; //全局符号表
};

/*外部定义：外部变量和函数*/
class ExtDefAST :public AST{};   //外部变量，函数的父类

/*外部变量类定义*/
class ExtVarDefAST :public ExtDefAST{     //外部变量定义
    public:
        TypeAST            *Type;         //外部变量类型
        std::vector<VarDecAST*> ExtVars;       //外部变量列表

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
};

class TypeAST:public AST{};

class BasicTypeAST:public TypeAST{  //用对象存储基本数据类型
    public:
        BasicTypes      Type;

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
};

// class StructTypeAST:public TypeAST{//结构类型名
//     public:
//         std::string Name;

//         void DisplayAST(int indent) override;
//         void Semantics(int &Offset) override;
//         void GenIR() override;
// };

// class StructDefAST:public TypeAST{//结构类型名
// public:
//    std::string Name;
//    std::vector<DefAST*> vars;
//    void DisplayAST(int indent) override;
// };


class VarDecAST :public AST{  //简单变量（标识符）、数组的定义
    public:
        std::string       Name;
        std::vector <int> Dims;              //数组各维大小，dims.size()为0表示非数组变量
        ExpAST       *Exp=nullptr;      //初始化表达式
        VarSymbol    *VarDefPtr;        //符号项的指针，如果有初始化表达式时，生成IR时起作用


        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override{};
        void Semantics(int &Offset,TypeAST *Type);
        void GenIR() override;
};





/*函数类定义*/
class FuncDefAST :public ExtDefAST{
    public:
        TypeAST           *Type;        //返回类型
        std::string Name;                    //函数名
        std::vector<ParamAST*> Params;       //参数列表，为空时为无参函数
        CompStmAST        *Body;        //函数体
        FuncSymbol        *FuncDefPtr;  //指向符号表中函数定义项

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
};

class ParamAST :public AST{             //形参
    public:
        TypeAST     *Type;              //形参类型
        VarDecAST   *ParamName;         //形参名，这里考虑到形参是数组的扩展

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
};


/*局部变量说明类定义*/
class DefAST:public AST{
    public:
        TypeAST              *Type;     //局部变量类型
        std::vector <VarDecAST *> LocVars;   //局部变量名称序列

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
};


/*语句结点类定义*/
class StmAST : public AST{
    public:
        StmAST* Parent;
        virtual void GenIR() override = 0;
        virtual void GenIR(std::string Entry,std::string End){};
};     //所有语句的父类
class CompStmAST : public StmAST {     //复合语句
    public:
        std::vector <DefAST *> Decls;
        std::vector <StmAST *> Stms;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override{GenIR("", "");};
        void GenIR(std::string Entry,std::string End)override;
        //semantic attributes
        SymbolsInAScope *LocalSymbolTable;//每个复合语句对应一个作用域（局部变量）
};


class ExprStmAST : public StmAST {     //表达式语句
    public:
        ExpAST * Exp;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
        void GenIR(std::string Entry,std::string End)override{GenIR();};
};

class IfStmAST : public StmAST {     //条件语句if-then
    public:
        ExpAST *Cond;
        StmAST *ThenStm;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override{GenIR("", "");};
        void GenIR(std::string LabelEntry,std::string LabelEnd)override;
};
class IfElseStmAST : public StmAST {//条件语句if-then-else
    public:
        ExpAST *Cond;
        StmAST *ThenStm;
        StmAST *ElseStm;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override{GenIR("", "");};
        void GenIR(std::string Entry,std::string End)override;
};

class WhileStmAST : public StmAST {     //while语句
    public:
        ExpAST *Cond;
        StmAST *Body;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
        void GenIR(std::string Entry,std::string End)override{GenIR();};
};

class ForStmAST:public StmAST{
    public:
        ExpAST *Head;
        ExpAST *Cond;
        ExpAST *End;
        StmAST *Body;
        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
        void GenIR(std::string Entry,std::string End)override{GenIR();};
};

class ReturnStmAST : public StmAST {     //return语句
    public:
        ExpAST * Exp;

        void DisplayAST(int l) override;
        void Semantics(int &Offset) override;
        void GenIR() override;
        void GenIR(std::string Entry,std::string End)override{GenIR();};
};

class BreakStmAST:public StmAST{
    void DisplayAST(int l) override;
    void Semantics(int &Offset) override;
    void GenIR() override{};
    void GenIR(std::string LabelEntry,std::string LabelEnd) override;
};

class ContinueStmAST:public StmAST{
    void DisplayAST(int l) override;
    void Semantics(int &Offset) override;
    void GenIR() override{};
    void GenIR(std::string LabelEntry,std::string LabelEnd) override;
};
/*表示式结点类定义*/
class ExpAST :public AST{//所有表达式的父类
    public:
        BasicTypes  Type;

        void GenIR()override{};
        virtual Opn GenIR(int &TempOffset)=0;                   //算术表达式，返回运算结果
        virtual void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse){}  //布尔表达式生成短路语句
};

class ConstAST :public ExpAST{
    public:
        union ConstVal{
                char  constCHAR;
                int   constINT;
                float constFLOAT;
        }ConstVal;

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn  GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};

class VarAST :public ExpAST{        //标识符变量
    public:
        std::string      Name;
        VarSymbol   *VarRef;        //指向该变量对应的符号表项
        std::vector<ExpAST* > index;    //数组的下标变量，须在文法处定义各维下标为整型表达式
        Opn ArrOff;
        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};

class ArrExpAST :public ExpAST
{
public:
    std::vector<ExpAST*> IniList;     //Exp或ArrayInitAST的指针序列序列
    void DisplayAST(int indent) override;
    void Semantics(int &Offset) override{};
    Opn GenIR(int &TempOffset)override{return Opn();};
    void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override{};
};

class AssignAST :public ExpAST{   //赋值表达式
    public:
        int     Op;            //=，+=。。。
        ExpAST  *LeftValExp,*RightValExp;

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};

class BinaryExprAST :public ExpAST{   //二元运算符
    public:
        int     Op;
        ExpAST  *LeftExp,*RightExp;
        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};

class UnaryExprAST :public ExpAST{   //一元运算符
    public:
        int     Op;//
        ExpAST  *Exp;

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};

class FuncCallAST :public ExpAST{   //函数调用
    public:
        std::string            Name;     //函数名
        std::vector <ExpAST *> Params;   //实际参数表达式序列
        FuncSymbol        *FuncRef; //指向该函数对应的符号表项

        void DisplayAST(int indent) override;
        void Semantics(int &Offset) override;
        Opn GenIR(int &TempOffset)override;
        void GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)override;
};
#endif // AST_H 在遍历语法树