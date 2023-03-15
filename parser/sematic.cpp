#include "ast.h"
#include "parser.tab.hpp"
SymbolStackDef AST::SymbolStack=SymbolStackDef();    //初始化静态成员符号表

std::map <int,int> TypeWidth={{T_CHAR,4},{T_INT,4},{T_FLOAT,8}}; //各类型所占字节数
std::map <char,std::string> KindName={{'V',"变量"},{'F',"函数"},{'P',"形参"},{'A',"数组"},{'C',"常量"}}; 
//std::map <std::string,int> TypeEnum = {{"char",T_CHAR},{"int",T_INT},{"float",T_FLOAT},{"void",T_VOID}};

std::vector <Error>Errors::Errs={};
void Errors::ErrorAdd(int Line,int Column,std::string ErrMsg)
{
    Error e={Line,Column,ErrMsg};
    Errs.push_back(e);
}
void Errors::ErrorsDisplay()
{
    for(auto a:Errs)
        std::cout<<"第"<<a.Line<<"行、第"<<a.Column<<"列处错误: "<<a.ErrMsg<<std::endl;
}

std::string NewAlias()
{
    static int num=0;
    return "V_"+std::to_string(++num);
}
void DisplaySymbolTable(SymbolStackDef *SYM)
{
    for(int i=0;i<SYM->Symbols.size();i++)
    {
        std::cout<<"----------------------------------------------------------------------"<<std::endl;
        std::cout<<" 层号: "<<i<<std::endl;
        std::cout<<" 符 号 名           别名    类型    种 类   其它信息"<<std::endl;
        std::cout<<"----------------------------------------------------------------------"<<std::endl;
        if (SYM->Symbols.at(i)->Symbols.size()==0)
            std::cout<<"  空 表"<<std::endl;
        else
        for(int j=0;j<SYM->Symbols.at(i)->Symbols.size();j++)
        {
            Symbol *SymPtr=SYM->Symbols.at(i)->Symbols.at(j);   //取第i层第j个符号对象的指针
            std::cout.width(20);
            std::cout<<SymPtr->Name;
            std::cout.width(8);
            if (SymPtr->Kind=='V' || SymPtr->Kind=='P'||SymPtr->Kind=='A')   //符号是变量,形参,显示别名
                std::cout<<((VarSymbol*)SymPtr)->Alias;
            else std::cout<<" ";
            std::cout.width(8);
            std::cout<<SymbolMap[SymPtr->Type];
            std::cout.width(8);
            std::cout<<KindName[SymPtr->Kind];
            if (SymPtr->Kind=='V' || SymPtr->Kind=='P')   //符号是变量,形参
                std::cout<<"偏移量: "<<((VarSymbol*)SymPtr)->Offset;
            else if (SymPtr->Kind=='F')  //符号是函数
            {
                std::cout<<"形参数: "<<((FuncSymbol*)SymPtr)->ParamNum;
                std::cout<<"  变量空间: "<<((FuncSymbol*)SymPtr)->ARSize;
            }
            else if (SymPtr->Kind=='A'){
                //符号是数组，需要显示各维大小
                std::cout<<"偏移量: "<<((VarSymbol*)SymPtr)->Offset;
                for(auto d:((VarSymbol*)SymPtr)->dims){
                    std::cout<<" ["<<d<<"]";
                }
                std::cout<<"  变量空间: "<<((VarSymbol*)SymPtr)->ARsize;
            }  
            std::cout<<std::endl;
        }
        std::cout<<"----------------------------------------------------------------------"<<std::endl;
    }
}
bool IsLeftValue(ExpAST *PExp)
{
    if (typeid(*PExp)==typeid(VarAST)) return true; //对简单变量、数组下标变量，结构变量,需要查符号表
    return false;
}

Symbol * SymbolStackDef::LocateNameCurrent(std::string Name)  //在当前(最内层)作用域中查找该符号是否有定义
{
    SymbolsInAScope *curScope=Symbols.back();
    for(int i=0;i<curScope->Symbols.size();i++)
        if (curScope->Symbols.at(i)->Name==Name)
            return curScope->Symbols.at(i);
    return nullptr;
}

Symbol * SymbolStackDef::LocateNameGlobal(std::string Name)//由内向外，整个符号表中查找该符号是否有定义
{
    for(int i=Symbols.size()-1;i>=0;i--)
    {
        for(int j=0;j<Symbols.at(i)->Symbols.size();j++)
            if (Symbols.at(i)->Symbols.at(j)->Name==Name)
                return Symbols.at(i)->Symbols.at(j);

    }
    return nullptr;
}

void ProgAST::Semantics0()
{
    int Offset=0;
    Semantics(Offset);
    Errors::ErrorsDisplay();
}

void ProgAST::Semantics(int &Offset)
{
    SymbolsInAScope *Global=new SymbolsInAScope();  //全局变量的作用域符号表，记录外部变量、函数名
    SymbolStack.Symbols.push_back(Global);
    GlobalSymbolTable=Global;                       //程序对象挂一个全局符号表

    //预先设置缺省函数read和write
    FuncSymbol *FuncDefPtr=new FuncSymbol();
    FuncDefPtr->Name=std::string("read");
    FuncDefPtr->Type=T_INT;
    FuncDefPtr->Kind='F';
    FuncDefPtr->ParamNum=0;
    FuncDefPtr->ARSize=12;
    SymbolStack.Symbols.back()->Symbols.push_back(FuncDefPtr);
    FuncDefPtr=new FuncSymbol();
    FuncDefPtr->Name=std::string("write");
    FuncDefPtr->Type=T_VOID;
    FuncDefPtr->Kind='F';
    FuncDefPtr->ParamNum=1;
    FuncDefPtr->ARSize=4;
    SymbolStack.Symbols.back()->Symbols.push_back(FuncDefPtr);

    VarSymbol *VarDefPtr=new VarSymbol();
    VarDefPtr->Name=VarDefPtr->Alias=std::string("x");
    VarDefPtr->Type=T_INT;
    VarDefPtr->Kind='P';
    VarDefPtr->Offset=4;
    SymbolStack.Symbols.back()->Symbols.push_back(VarDefPtr);

    for(auto a:ExtDefs)
    {
        a->Semantics(Offset);
    }
    DisplaySymbolTable(&SymbolStack);
}

void ExtVarDefAST::Semantics(int &Offset)  //外部定义对象的语义
{
    for(auto a:ExtVars)
        a->Semantics(Offset,Type);
}


void VarDecAST::Semantics(int &Offset,TypeAST *Type)
{
    if (!SymbolStack.LocateNameCurrent(Name))  //当前作用域未定义，将变量加入符号表
    {
        VarDefPtr=new VarSymbol();
        VarDefPtr->Name=Name;
        VarDefPtr->Alias=NewAlias();
        if (typeid(*Type)==typeid(BasicTypeAST))
            VarDefPtr->Type=((BasicTypeAST*)Type)->Type;
        VarDefPtr->Offset=Offset;
        if (!Dims.size()){
           VarDefPtr->Kind='V';
           Offset+=TypeWidth[VarDefPtr->Type];
        }else{
            VarDefPtr->Kind='A';
            int unit = 1;
            for(auto d:Dims){
                VarDefPtr->dims.push_back(d);
                unit *= d;
            }
            VarDefPtr->ARsize = TypeWidth[VarDefPtr->Type]*unit;
            Offset+=VarDefPtr->ARsize;
        } 
        if (Exp)                      //有初值表达式时的处理
           Exp->Semantics(Offset);
        SymbolStack.Symbols.back()->Symbols.push_back(VarDefPtr);
    }
    else Errors::ErrorAdd(Line,Column,"变量 "+Name+" 重复定义") ;

}

void DefAST::Semantics(int &Offset)
{    //依次提取变量符号进行语义分析
     for(auto a:LocVars)
        a->Semantics(Offset,Type);
}

void BasicTypeAST::Semantics(int &Offset)
{
}

void FuncDefAST::Semantics(int &Offset)
{
    if (!SymbolStack.LocateNameCurrent(Name))  //当前作用域未定义，将变量加入符号表
    {
        int Offset=12;           //局部变量偏移量初始化,预留12个字节存放返回地址等信息，可根据实际情况修改
        MaxVarSize=12;            //计算函数变量需要的最大容量
        FuncDefPtr=new FuncSymbol();
        FuncDefPtr->Name=Name;
        FuncDefPtr->Kind='F';
        if (typeid(*Type)==typeid(BasicTypeAST))     //处理符号项的返回类型，目前仅基本类型T_CHAR,T_INT,T_FLOAT
             FuncDefPtr->Type=((BasicTypeAST*)Type)->Type;
        FuncDefPtr->ParamNum=Params.size();

        SymbolsInAScope *Local=new SymbolsInAScope();  //生成函数体作用域变量表
        FuncDefPtr->ParamPtr=Local;                    //函数符号表项，指向形参
        SymbolStack.Symbols.back()->Symbols.push_back(FuncDefPtr);//填写函数符号到符号表

        
        SymbolStack.Symbols.push_back(Local);          //函数体符号表（含形参）进栈
        Body->LocalSymbolTable=Local;
        for(auto a:Params)
            a->Semantics(Offset);              //未考虑参数用寄存器，只是简单在AR中分配单元
        Body->Semantics(Offset);               //对函数中的变量，在AR中接在参数后分配单元
        FuncDefPtr->ARSize=MaxVarSize; //函数变量需要空间大小（未考虑临时变量），后续再加临时变量单元得到AR大小
    }
    else Errors::ErrorAdd(Line,Column,"函数 "+Name+" 重复定义");
}
void ParamAST::Semantics(int &Offset)
{
    if (!SymbolStack.LocateNameCurrent(ParamName->Name))  //当前作用域未重复定义，将形参名加入符号表
    {
         VarSymbol *SymPtr=new VarSymbol();
         SymPtr->Name=ParamName->Name;
         SymPtr->Kind='P';
         SymPtr->Alias=NewAlias();
         if (typeid(*Type)==typeid(BasicTypeAST))
             SymPtr->Type=((BasicTypeAST*)Type)->Type;
         SymPtr->Offset=Offset;   Offset+=TypeWidth[SymPtr->Type];
         SymbolStack.Symbols.back()->Symbols.push_back(SymPtr);
    }
    else Errors::ErrorAdd(Line,Column,"形参名 "+ParamName->Name+" 重复定义");

}

/**************语句显示******************************/
void CompStmAST::Semantics(int &Offset)
{
    if (!LocalSymbolTable)          //如果不是函数体的复合语句，需自行生成局部符号表
    {
        SymbolsInAScope *Local=new SymbolsInAScope();  //全局变量的作用域符号表，记录外部变量、函数名
        SymbolStack.Symbols.push_back(Local);
        LocalSymbolTable=Local;                       //程序对象挂一个符号表
    }
    for(auto a:Decls){
        a->Semantics(Offset);
    }
        
    if (Offset>MaxVarSize) MaxVarSize=Offset;
    for(auto a:Stms)
    {
        int Offset_S=Offset;      //前后并列语句可以使用同一片单元，所以取最大值，这里保存起始偏移量
        a->Parent = this;
        a->Semantics(Offset);
        if (Offset>MaxVarSize) MaxVarSize=Offset;
        Offset=Offset_S;
    }
    std::cout<<"\n\n********************当前复合语句符号表状态**************************"<<std::endl;
    DisplaySymbolTable(&SymbolStack);
    std::cout<<std::endl<<std::endl;
    SymbolStack.Symbols.pop_back();     //复合语句的符号表退栈
}

void ExprStmAST::Semantics(int &Offset)
{
    Exp->Semantics(Offset);
}
void IfStmAST::Semantics(int &Offset)
{
    Cond->Semantics(Offset);
    ThenStm->Parent = this;
    ThenStm->Semantics(Offset);
}
void IfElseStmAST::Semantics(int &Offset)
{
    Cond->Semantics(Offset);
    ThenStm->Parent = this;
    ThenStm->Semantics(Offset);
    ElseStm->Parent = this;
    ElseStm->Semantics(Offset);
}
void WhileStmAST::Semantics(int &Offset)
{
    Cond->Semantics(Offset);
    Body->Parent = this;
    Body->Semantics(Offset);
}
void ForStmAST::Semantics(int &Offset){
    Head->Semantics(Offset);
    Cond->Semantics(Offset);
    End->Semantics(Offset);
    Body->Parent = this;
    Body->Semantics(Offset);
}
void ReturnStmAST::Semantics(int &Offset)
{
    auto FunPtr = (*(SymbolStack.Symbols.end()-2))->Symbols.back();
    if (Exp){
        Exp->Semantics(Offset);
        if(FunPtr->Type!=Exp->Type){
            Errors::ErrorAdd(Line,Column,"函数返回类型不匹配 "+FunPtr->Name);
            return;
        }
    }else{//如果return语句没有表达式且函数的返回类型不为void，报错
        if(FunPtr->Type!=T_VOID){
            Errors::ErrorAdd(Line,Column,"函数返回类型不匹配 "+FunPtr->Name);
            return;
        }
    }
}

void BreakStmAST::Semantics(int &Offset){
    while(Parent){
        if(typeid(*Parent)==typeid(ForStmAST)||typeid(*Parent)==typeid(WhileStmAST)){return;}
        Parent = Parent->Parent;
    }
    Errors::ErrorAdd(Line,Column,"break语句不在for语句或while语句内");
}
void ContinueStmAST::Semantics(int &Offset){
    while(Parent){
        if(typeid(*Parent)==typeid(ForStmAST)||typeid(*Parent)==typeid(WhileStmAST)){return;}
        Parent = Parent->Parent;
    }
    Errors::ErrorAdd(Line,Column,"continue语句不在for语句或while语句内");
}
/**************表达式显示******************************/
void VarAST::Semantics(int &Offset)
{
    if (VarRef=(VarSymbol *)SymbolStack.LocateNameGlobal(Name))
    {
        //如果是函数名，报错，
        if(VarRef->Kind=='F'){
            Errors::ErrorAdd(Line,Column,"对函数名采用非函数调用形式访问 "+Name) ;
            return;
        }
        //如果是数组
        if(VarRef->Kind=='A'){
            if(index.size()>VarRef->dims.size()){
                Errors::ErrorAdd(Line,Column,Name+"数组下标数量过多 ");
            }
            for(auto i:index){
                i->Semantics(Offset);
                if(i->Type!=T_INT){
                    int a = i->Type;
                    Errors::ErrorAdd(Line,Column,Name+"数组下标不是整形 ");
                }
            }
        }else{//如果不是数组，采用了下标访问
            if(index.size()){
                Errors::ErrorAdd(Line,Column,"非数组变量采用下标访问 ");
            }
        }
        //简单变量则提取变量类型属性
        this->Offset = VarRef->Offset;
        this->Type = VarRef->Type;
    }
    else Errors::ErrorAdd(Line,Column,"引用未定义的符号 "+Name);

}

void ConstAST::Semantics(int &Offset)
{
    //提取类型属性
    
}

void AssignAST::Semantics(int &Offset)
{
    LeftValExp->Semantics(Offset);
    if (!IsLeftValue(LeftValExp))
        Errors::ErrorAdd(Line,Column,"对非左值表达式赋值");
    RightValExp->Semantics(Offset);
    if(LeftValExp->Type!=RightValExp->Type){
        Errors::ErrorAdd(Line,Column,"表达式类型不匹配");
    }
    Type = LeftValExp->Type;
}

void BinaryExprAST::Semantics(int &Offset)
{
    LeftExp->Semantics(Offset);
    RightExp->Semantics(Offset);
    //根据左右值类型，判断是否能进行运算，并确定运算结果类型
    if(LeftExp->Type!=RightExp->Type){
        Errors::ErrorAdd(Line,Column,"表达式类型不匹配");
    }
    Type = LeftExp->Type;
}

void UnaryExprAST::Semantics(int &Offset)
{
    Exp->Semantics(Offset);
    this->Type = Exp->Type;
    if((Op==DMINUS||Op==DPLUS)&&!IsLeftValue(Exp)){
        Errors::ErrorAdd(Line,Column,"对非左值进行自增自减 ");
        return;
    }
}

void FuncCallAST::Semantics(int &Offset)
{
    if (FuncRef=(FuncSymbol *)SymbolStack.LocateNameGlobal(Name))
    {
        this->Type = FuncRef->Type;
        if(FuncRef->Kind!='F'){
            Errors::ErrorAdd(Line,Column,"对非函数名采用函数调用形式 "+Name);
            return;
        }
        if(FuncRef->ParamNum!=Params.size()){
            Errors::ErrorAdd(Line,Column,"函数参数数量不匹配 "+Name);
            return;
        }
        if(FuncRef->Name==string("write")){
            Params[0]->Semantics(Offset);
        }else{
            for(int i = 0;i<Params.size();i++)
            {
            //检查实参表达式个数和形参数是否一致，类型是否一致
                Params[i]->Semantics(Offset);
                if(Params[i]->Type!=FuncRef->ParamPtr->Symbols[i]->Type){
                    Errors::ErrorAdd(Line,Column,"函数参数类型不匹配 "+Name);
                    return;
                }
            }
        }
    }
    else Errors::ErrorAdd(Line,Column,"引用未定义的函数 "+Name);
}
