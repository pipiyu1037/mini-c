#include "ast.h"
void error(int Line,int Colum,std::string errMsg);
#define YYSTYPE int   //此行是为了包含parser.tab.hpp不引起错误而加,可以在后面使用相关常量
#include "parser.tab.hpp"

std::string NewTemp()
{
    static int num=0;
    return "Temp_"+std::to_string(++num);
}

std::string NewLabel()
{
    static int num=0;
    return "Label_"+std::to_string(++num);
}

std::map <int,std::string> Instruction={{LABEL,"LABEL "},{FUNCTION,"FUNCTION  "},{ASSIGN,":="},{PLUS,"+"},{UPLUS,"+"},
                    {MINUS,"-"},{UMINUS,"-"},{STAR,"*"},{DIV,"/"},{GOTO,"  GOTO  "},
                    {GT,">"},{GE,">="},{LT,"<"},{LE,"<="},{EQ,"=="},{NE,"!="},
                    {JGT,">"},{JGE,">="},{JLT,"<"},{JLE,"<="},{JEQ,"=="},{JNE,"!="},
                    {RETURN,"  RETURN  "},{ARG,"  ARG  "},{PARAM,"  PARAM  "}};

void DisplayIR(std::list<IRCode> IRCodes)
{
    for(auto a:IRCodes)
    {
        std::string OpnStr1,OpnStr2=a.Opn2.Name,ResultStr=a.Result.Name;
        if (a.Opn1.Name==std::string("_CONST"))
            switch (a.Opn1.Type)
            {
                case T_CHAR: OpnStr1=std::string("#")+std::to_string(a.Opn1.constCHAR);break;
                case T_INT:  OpnStr1=std::string("#")+std::to_string(a.Opn1.constINT);break;
                case T_FLOAT:OpnStr1=std::string("#")+std::to_string(a.Opn1.constFLOAT);break;
            }
        else OpnStr1=a.Opn1.Name;

        switch (a.Op) {
            case READARRAY:
                std::cout<<"  "<<ResultStr<<" := ["<<OpnStr1<<" : "<<OpnStr2<<"]"<<std::endl;break;
            case WRITEARRAY:
                std::cout<<"  "<<"["<<OpnStr2<<" : "<<ResultStr<<"] := "<<OpnStr1<<std::endl;break;
            case ASSIGN: std::cout<<"  "<<ResultStr<<" := "<<OpnStr1<<std::endl; break;
            case UPLUS: case UMINUS:
                std::cout<<"  "<<ResultStr<<" := "<<Instruction[a.Op]<<" "<<OpnStr1<<std::endl; break;
            case PLUS:  case MINUS:  case STAR:   case DIV:
            case LE:  case LT:   case GE: case GT:  case EQ:  case NE:
                std::cout<<"  "<<ResultStr<<":= "<<OpnStr1<<Instruction[a.Op]<<OpnStr2<<std::endl; break;
            case JLE:  case JLT:   case JGE: case JGT:  case JEQ:  case JNE:
                std::cout<<"  "<<"IF "<<OpnStr1<<Instruction[a.Op]<<OpnStr2<<" GOTO "<<ResultStr<<std::endl;break;
            case GOTO: case PARAM: case ARG: case RETURN:
                std::cout<<Instruction[a.Op]<<ResultStr<<std::endl; break;
            case FUNCTION:case LABEL:
                std::cout<<Instruction[a.Op]<<ResultStr<<":"<<std::endl; break;
            case CALL:   std::cout<<"  "<<ResultStr<<" := "<<"CALL "<<OpnStr1<<std::endl; break;
            case CALL0:  std::cout<<"  CALL "<<OpnStr1<<std::endl; break;
            case END:    std::cout<<"  End Of Program"<<std::endl;break;
        }
    }
}
void GenObject(std::list<IRCode> IRCodes);

void DisplaySymbolTable(SymbolStackDef *SYM);

void ProgAST::GenIR()
{
    for(auto a:ExtDefs)     //将各外部项代码依次连接
    {
        a->GenIR();
        std::list<IRCode>::iterator it=IRCodes.end();
        IRCodes.splice(it,a->IRCodes);
    }
    DisplayIR(IRCodes);
    GenObject(IRCodes);
//    DisplaySymbolTable(&SymbolStack);
}

void ExtVarDefAST::GenIR()  //外部变量定义
{ /*由于未考虑全局变量的初始化，所以无中间代码*/ }


void VarDecAST::GenIR()
{//有初始化表达式，需要生成中间代码
    if (!Exp) return;
    Opn Result(VarDefPtr->Alias,VarDefPtr->Type,VarDefPtr->Offset);
    int TempVarOffset=0;
    Opn Opn1=Exp->GenIR(TempVarOffset);
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Exp->IRCodes);
    IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Result));
}
void DefAST::GenIR()
{
    std::list<IRCode>::iterator it;
    for(auto a:LocVars)
    {
        a->GenIR();
        it=IRCodes.end();
        IRCodes.splice(it,a->IRCodes);
    }
}

void BasicTypeAST::GenIR()
{
}

void FuncDefAST::GenIR()
{
    for(auto a:Params)
        IRCodes.push_back(IRCode(PARAM,Opn(),Opn(),Opn(a->ParamName->Name,0,0))); //(PARAM,,,形参名)

    MaxVarSize=FuncDefPtr->ARSize;
    Body->GenIR();
    std::list <IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Body->IRCodes);          //连接函数体语句中间代码
    FuncDefPtr->ARSize+=MaxTempVarOffset;        //函数AR(栈帧)的大小
    IRCode IRFunc=IRCode(FUNCTION,Opn(),Opn(),Opn(Name,0,0));
    if (Name==std::string("main"))
    {
        IRFunc.Result.Offset=FuncDefPtr->ARSize; //主函数的栈帧大小
        IRCodes.push_back(IRCode(END,Opn(),Opn(),Opn()));  //添加程序结束标记
    }
    IRCodes.push_front(IRFunc);                             //函数开始(FUNCTION,,,Name)
}
void ParamAST::GenIR(){}

void CompStmAST::GenIR(std::string LabelEntry,std::string LabelEnd)
{
    std::list<IRCode>::iterator it;
    for(auto a:Decls)
    {
        a->GenIR();
        it=IRCodes.end();
        IRCodes.splice(it,a->IRCodes);
    }
    for(auto a:Stms)
    {
        a->GenIR(LabelEntry,LabelEnd);
        it=IRCodes.end();
        IRCodes.splice(it,a->IRCodes);
    }
}

void ExprStmAST::GenIR()
{
    int TempVarOffset=0;
//    if (typeid(*Exp)==typeid(FuncCallAST) && ((FuncCallAST *)Exp)->FuncRef->Type==T_VOID)
//        cout<<"无参函数的语句"<<((FuncCallAST *)Exp)->FuncRef->Name<<endl;
    Exp->GenIR(TempVarOffset);
    if (TempVarOffset>MaxTempVarOffset)
        MaxTempVarOffset=TempVarOffset;
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Exp->IRCodes);
}
void IfStmAST::GenIR(std::string Entry,std::string End)
{
    std::string LabelThen=NewLabel();
    std::string LabelEnd=NewLabel();

    /*计算条件表达式，后面这条件判断处理可以将修改成短路语句的形式，即将标号LabelThen
      和LabelEnd带入，但表达式一旦能得到真假结果，后续不需要计算，直接转移到目标位置。
      而不是下面把整个条件表达式计算完成后，再根据结果确定转移位置*/
    int TempVarOffset=0;
    Cond->GenIR(TempVarOffset,LabelThen,LabelEnd);     //计算条件表达式
    ThenStm->GenIR(Entry,End);

    std::list <IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Cond->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LabelThen,0,0)));//if子句前的标号
    it=IRCodes.end();
    IRCodes.splice(it,ThenStm->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LabelEnd,0,0))); //if语句出口标号

}
void IfElseStmAST::GenIR(std::string Entry,std::string End)
{
    std::string LabelThen=NewLabel();
    std::string LabelElse=NewLabel();
    std::string LabelEnd=NewLabel();

    int TempVarOffset=0;
    Cond->GenIR(TempVarOffset,LabelThen,LabelElse);
    ThenStm->GenIR(Entry,End);
    ElseStm->GenIR(Entry,End);

    std::list <IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Cond->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LabelThen,0,0)));//if子句前的标号
    it=IRCodes.end();
    IRCodes.splice(it,ThenStm->IRCodes);
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelEnd,0,0)));
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LabelElse,0,0)));//else子句前的标号
    it=IRCodes.end();
    IRCodes.splice(it,ElseStm->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LabelEnd,0,0))); //if语句出口标号

}
void WhileStmAST::GenIR()
{
    std::string LoopCond=NewLabel();
    std::string LoopEntry=NewLabel();
    std::string LoopEnd=NewLabel();

    int TempVarOffset=0;
    Cond->GenIR(TempVarOffset,LoopEntry,LoopEnd);     //计算条件表达式
    Body->GenIR(LoopCond,LoopEnd);

    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopCond,0,0)));
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Cond->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopEntry,0,0)));//循环入口标号
    it=IRCodes.end();
    IRCodes.splice(it,Body->IRCodes);
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LoopCond,0,0))); //结束本次循环，转去重新计算循环条件
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopEnd,0,0))); //循环结束标号
}

void ForStmAST::GenIR(){
    std::string LoopCond=NewLabel();
    std::string LoopEntry=NewLabel();
    std::string EndExp=NewLabel();
    std::string LoopEnd=NewLabel();

    int TempVarOffset = 0;
    Head->GenIR(TempVarOffset);
    Cond->GenIR(TempVarOffset,LoopEntry,LoopEnd);
    Body->GenIR(EndExp,LoopEnd);
    End->GenIR(TempVarOffset);
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Head->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopCond,0,0)));
    it=IRCodes.end();
    IRCodes.splice(it,Cond->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopEntry,0,0)));//循环入口标号
    it=IRCodes.end();
    IRCodes.splice(it,Body->IRCodes);
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(EndExp,0,0)));//循环尾表达式
    it=IRCodes.end();
    IRCodes.splice(it,End->IRCodes);
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LoopCond,0,0))); //结束本次循环，转去重新计算循环条件
    IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(LoopEnd,0,0))); //循环结束标号
}

void BreakStmAST::GenIR(std::string LabelEntry,std::string LabelEnd){
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelEnd,0,0)));
}

void ContinueStmAST::GenIR(std::string LabelEntry,std::string LabelEnd){
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelEntry,0,0)));
}

void ReturnStmAST::GenIR()
{
    if (!Exp) return;
    int TempVarOffset=0;
    Opn Result=Exp->GenIR(TempVarOffset);
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Exp->IRCodes);
    Opn Temp(NewTemp(),Result.Type,TempVarOffset+MaxVarSize);
    if(typeid(*Exp)==typeid(VarAST)&&((VarAST*)Exp)->index.size()){
        IRCodes.push_back(IRCode(READARRAY,Result,((VarAST*)Exp)->ArrOff,Temp));
        IRCodes.push_back(IRCode(RETURN,Opn(),Opn(),Temp));
    }else{
        IRCodes.push_back(IRCode(RETURN,Opn(),Opn(),Result));
    }
}

/**************表达式的中间代码生成************************/
Opn VarAST::GenIR(int &TempVarOffset)
{
    //通过语义检查后，VarRef指向对应表项，否则为空，程序会崩溃
    if(VarRef->Kind=='A'){//如果是数组元素，那么计算数组元素在数组内的偏移
        Opn Arr(NewTemp(),T_INT,TempVarOffset+MaxVarSize);//声明一个临时变量用来存储数组内偏移
        TempVarOffset+=TypeWidth[T_INT];           //修改临时变量的偏移量
        Opn Zero("_CONST",T_INT,0);Zero.constINT=0;
        IRCodes.push_back(IRCode(ASSIGN,Zero,Opn(),Arr));//Arr初值为0
        Opn Temp(NewTemp(),T_INT,MaxVarSize+TempVarOffset);//用来存储数组索引值
        TempVarOffset+=TypeWidth[T_INT];           //修改临时变量的偏移量set;
        
        Opn MultRes(NewTemp(),T_INT,MaxVarSize+TempVarOffset);//计算数组元素偏移时用的临时变量
        TempVarOffset+=TypeWidth[T_INT];
        if (TempVarOffset>MaxTempVarOffset)
            MaxTempVarOffset=TempVarOffset;

        Opn Unit("_CONST",T_INT,0);Unit.constINT=TypeWidth[VarRef->Type];
        Opn indexOffset;
        std::list<IRCode>::iterator it=IRCodes.end();
        for(int i = index.size()-1;i>=0;i--){   //计算数组元素在数组内偏移
            indexOffset = index[i]->GenIR(TempVarOffset);
            IRCodes.splice(it,index[i]->IRCodes);
            if(typeid(*index[i])==typeid(VarAST)&&((VarAST*)index[i])->index.size()){
                IRCodes.push_back(IRCode(READARRAY,indexOffset,((VarAST*)index[i])->ArrOff,Temp));
            }else{
                IRCodes.push_back(IRCode(ASSIGN,indexOffset,Opn(),Temp));
            }
            IRCodes.push_back(IRCode(STAR,Unit,Temp,MultRes));
            IRCodes.push_back(IRCode(PLUS,MultRes,Arr,Arr));
            Unit.constINT *= VarRef->dims[i]; 
        }
        this->ArrOff = Arr;
    }
    Opn VarOpn(VarRef->Alias,VarRef->Type,VarRef->Offset);
    return VarOpn;
}

void VarAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
//根据变量的值确定转移方向
    Opn Result=GenIR(TempVarOffset);
    Opn Zero("_CONST",T_INT,0);Zero.constINT=0;
    IRCodes.push_back(IRCode(JNE,Result,Zero,Opn(LabelTrue,0,0)));
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
}

Opn ConstAST::GenIR(int &TempVarOffset)
{
    //将常量赋值给生成的临时变量
    Opn Result(NewTemp(),Type,TempVarOffset+MaxVarSize); //生成临时变量保存常量值
    TempVarOffset+=TypeWidth[Type];           //修改临时变量的偏移量
    if (TempVarOffset>MaxTempVarOffset)
        MaxTempVarOffset=TempVarOffset;
    Opn Opn1("_CONST",Type,0);             //别名或临时变量名为_CONST时，表示常量
  //  Opn1.ConstVal.constCHAR=ConstVal.constCHAR;
  //  Opn1.ConstVal.constINT=ConstVal.constINT;
    Opn1.constFLOAT=ConstVal.constFLOAT;                //按最大长度的成员进行整体复制

    IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Result));
    return Result;

}

void ConstAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
    Opn Result=GenIR(TempVarOffset);
    Opn Zero("_CONST",T_INT,0);Zero.constINT=0;
    IRCodes.push_back(IRCode(JNE,Result,Zero,Opn(LabelTrue,0,0)));
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
}


Opn AssignAST::GenIR(int &TempVarOffset)
{
    Opn Result=LeftValExp->GenIR(TempVarOffset);
    Opn Opn1=RightValExp->GenIR(TempVarOffset);
    Opn Temp(NewTemp(),Result.Type,MaxVarSize+TempVarOffset);
    TempVarOffset+=TypeWidth[Result.Type];           //修改临时变量的偏移量
    if (TempVarOffset>MaxTempVarOffset)
        MaxTempVarOffset=TempVarOffset;
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,LeftValExp->IRCodes);
    it=IRCodes.end();
    IRCodes.splice(it,RightValExp->IRCodes);
    if(typeid(*RightValExp)==typeid(VarAST)&&((VarAST*)RightValExp)->index.size()){
        IRCodes.push_back(IRCode(READARRAY,Opn1,((VarAST*)RightValExp)->ArrOff,Temp));
    }else{
        IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Temp));
    }
    if(typeid(*LeftValExp)==typeid(VarAST)&&((VarAST*)LeftValExp)->index.size()){
        //(写数组，临时变量，数组起始偏移，数组内偏移)
        IRCodes.push_back(IRCode(WRITEARRAY,Temp,Result,((VarAST*)LeftValExp)->ArrOff)); 
    }else{
        IRCodes.push_back(IRCode(ASSIGN,Temp,Opn(),Result));
    }
    return Result;
}

void AssignAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
//根据左值表达式的值确定转移方向
    Opn Result=GenIR(TempVarOffset);
    Opn Zero("_CONST",T_INT,0);Zero.constINT=0;
    Opn Temp(NewTemp(),Result.Type,MaxVarSize+TempVarOffset);
    TempVarOffset+=TypeWidth[Result.Type];           //修改临时变量的偏移量
    if (TempVarOffset>MaxTempVarOffset)
        MaxTempVarOffset=TempVarOffset;
    if(typeid(*LeftValExp)==typeid(VarAST)&&((VarAST*)LeftValExp)->index.size()){
        //(写数组，临时变量，数组起始偏移，数组内偏移)
        IRCodes.push_back(IRCode(READARRAY,Result,((VarAST*)LeftValExp)->ArrOff,Temp)); 
    }else{//如果左值表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
        IRCodes.push_back(IRCode(ASSIGN,Result,Opn(),Temp));
    }
    IRCodes.push_back(IRCode(JNE,Temp,Zero,Opn(LabelTrue,0,0)));
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
}


Opn BinaryExprAST::GenIR(int &TempVarOffset)
{
    Opn Opn1=LeftExp->GenIR(TempVarOffset);
    Opn Opn2=RightExp->GenIR(TempVarOffset);
    Opn Result(NewTemp(),Opn1.Type,TempVarOffset+MaxVarSize); //生成临时变量保存运算结果，结果类型应该根据运算结果来定
    TempVarOffset+=TypeWidth[Opn1.Type];           //这里只是简单处理成和左右操作数类型相同，修改临时变量的偏移量
    Opn Temp1(NewTemp(),Opn1.Type,MaxVarSize+TempVarOffset);
    TempVarOffset+=TypeWidth[Opn1.Type];           //这里只是简单处理成和左右操作数类型相同，修改临时变量的偏移量
    Opn Temp2(NewTemp(),Opn2.Type,MaxVarSize+TempVarOffset);
    TempVarOffset+=TypeWidth[Opn2.Type];           //这里只是简单处理成和左右操作数类型相同，修改临时变量的偏移量
    if (TempVarOffset>MaxTempVarOffset)
            MaxTempVarOffset=TempVarOffset;
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,LeftExp->IRCodes);
    it=IRCodes.end();
    IRCodes.splice(it,RightExp->IRCodes);
    if(typeid(*LeftExp)==typeid(VarAST)&&((VarAST*)LeftExp)->index.size()){
        //(读数组，数组起始偏移，数组内偏移，临时变量)
        IRCodes.push_back(IRCode(READARRAY,Opn1,((VarAST*)LeftExp)->ArrOff,Temp1)); 
    }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
        IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Temp1));
    }
    if(typeid(*RightExp)==typeid(VarAST)&&((VarAST*)RightExp)->index.size()){
        //(读数组，数组起始偏移，数组内偏移，临时变量)
        IRCodes.push_back(IRCode(READARRAY,Opn2,((VarAST*)LeftExp)->ArrOff,Temp2)); 
    }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
        IRCodes.push_back(IRCode(ASSIGN,Opn2,Opn(),Temp2));
    }
    IRCodes.push_back(IRCode(Op,Temp1,Temp2,Result));
    return Result;
}

void BinaryExprAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
    std::list <IRCode>::iterator it;
    switch (Op)
    {
        case AND: case OR:
            {
            std::string Label=NewLabel();
            if (Op==AND) LeftExp->GenIR(TempVarOffset,Label,LabelFalse);
            else LeftExp->GenIR(TempVarOffset,LabelTrue,Label);
            RightExp->GenIR(TempVarOffset,LabelTrue,LabelFalse);
            std::list <IRCode> ::iterator it=IRCodes.end();
            IRCodes.splice(it,LeftExp->IRCodes);
            IRCodes.push_back(IRCode(LABEL,Opn(),Opn(),Opn(Label,0,0)));
            it=IRCodes.end();
            IRCodes.splice(it,RightExp->IRCodes);
            }
            break;
        case PLUS:  case MINUS:  case STAR:  case  DIV:
            {
            Opn Result=GenIR(TempVarOffset);
            Opn Zero("_CONST",T_INT,0);Zero.constINT=0;
            IRCodes.push_back(IRCode(JNE,Result,Zero,Opn(LabelTrue,0,0)));
            IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
            }
            break;
        
        default:     //处理关系运算符
            Opn Opn1=LeftExp->GenIR(TempVarOffset);
            Opn Opn2=RightExp->GenIR(TempVarOffset);
            Opn Temp1(NewTemp(),Opn1.Type,MaxVarSize+TempVarOffset);
            TempVarOffset+=TypeWidth[Opn1.Type];           //这里只是简单处理成和左右操作数类型相同，修改临时变量的偏移量
            Opn Temp2(NewTemp(),Opn2.Type,MaxVarSize+TempVarOffset);
            TempVarOffset+=TypeWidth[Opn2.Type];           //这里只是简单处理成和左右操作数类型相同，修改临时变量的偏移量
            if (TempVarOffset>MaxTempVarOffset)
                    MaxTempVarOffset=TempVarOffset;
            it=IRCodes.end();
            IRCodes.splice(it,LeftExp->IRCodes);
            it=IRCodes.end();
            IRCodes.splice(it,RightExp->IRCodes);
            if(typeid(*LeftExp)==typeid(VarAST)&&((VarAST*)LeftExp)->index.size()){
                //(读数组，数组起始偏移，数组内偏移，临时变量)
                IRCodes.push_back(IRCode(READARRAY,Opn1,((VarAST*)LeftExp)->ArrOff,Temp1)); 
            }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
                IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Temp1));
            }
            if(typeid(*RightExp)==typeid(VarAST)&&((VarAST*)RightExp)->index.size()){
                //(读数组，数组起始偏移，数组内偏移，临时变量)
                IRCodes.push_back(IRCode(READARRAY,Opn2,((VarAST*)RightExp)->ArrOff,Temp2)); 
            }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
                IRCodes.push_back(IRCode(ASSIGN,Opn2,Opn(),Temp2));
            }
            IRCode IR(JGT,Temp1,Temp2,Opn(LabelTrue,0,0));
            if (Op==GE) IR.Op=JGE;
            else if (Op==LT) IR.Op=JLT;
            else if (Op==LE) IR.Op=JLE;
            else if (Op==EQ) IR.Op=JEQ;
            else if (Op==NE) IR.Op=JNE;
            IRCodes.push_back(IR);
            IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
    }
}

Opn UnaryExprAST::GenIR(int &TempVarOffset)
{
    Opn Opn1=Exp->GenIR(TempVarOffset);
    Opn Result(NewTemp(),Exp->Type,TempVarOffset+MaxVarSize);
    TempVarOffset+=TypeWidth[Exp->Type];
    Opn Temp(NewTemp(),Result.Type,MaxVarSize+TempVarOffset);
    TempVarOffset+=TypeWidth[Result.Type];           //修改临时变量的偏移量
    if (TempVarOffset>MaxTempVarOffset)
        MaxTempVarOffset=TempVarOffset;
    std::list<IRCode>::iterator it=IRCodes.end();
    IRCodes.splice(it,Exp->IRCodes);
    if(typeid(*Exp)==typeid(VarAST)&&((VarAST*)Exp)->index.size()){
        //(读数组，数组起始偏移，数组内偏移，临时变量)
        IRCodes.push_back(IRCode(READARRAY,Opn1,((VarAST*)Exp)->ArrOff,Temp)); 
    }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
        IRCodes.push_back(IRCode(ASSIGN,Opn1,Opn(),Temp));
    }
    IRCodes.push_back(IRCode(Op,Temp,Opn(),Result));
    if(Op==DMINUS||Op==DPLUS){//如果是自增自减,则对操作数进行处理
        if(typeid(*Exp)==typeid(VarAST)&&((VarAST*)Exp)->index.size()){
            //(写数组，临时变量,数组起始偏移，数组内偏移)
            IRCodes.push_back(IRCode(WRITEARRAY,Result,Opn1,((VarAST*)Exp)->ArrOff)); 
        }else{//如果左表达式是数组元素,那么读取数组元素值到Temp,否则直接读取左值到Temp
            IRCodes.push_back(IRCode(ASSIGN,Result,Opn(),Opn1));
        }
    }
    return Result;
}

void UnaryExprAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
    // Exp->GenIR(TempVarOffset,LabelFalse,LabelTrue);
    // std::list <IRCode> ::iterator it=IRCodes.end();
    // IRCodes.splice(it,Exp->IRCodes);
    Opn Result = GenIR(TempVarOffset);
    Opn Zero = Opn("_Const",T_INT,0);
    Zero.constINT = 0;
    IRCodes.push_back(IRCode(JNE,Result,Zero,Opn(LabelTrue,0,0)));
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
}


Opn FuncCallAST::GenIR(int &TempVarOffset)
{
    std::list<IRCode> ARGS;
    std::list<IRCode>::iterator it;
    Opn Opn1,Result;
    SymbolsInAScope *ParamPtr=FuncRef->ParamPtr;
    int i=0;

    for(auto a:Params)
    {
        if (Name!=std::string("write"))  //write函数特殊处理，参数传递用的寄存器
        {   //用Opn1的Offset保存形参的偏移量,方便目标代码参数传递,将实参值保存在AR中
            VarSymbol *Sym=(VarSymbol*)((ParamPtr->Symbols).at(i++));
            Opn1.Offset=Sym->Offset;
        }
        Result=a->GenIR(TempVarOffset);        //计算实参表达式的值
        it=IRCodes.end();
        IRCodes.splice(it,a->IRCodes);
        if(typeid(*a)==typeid(VarAST)&&((VarAST*)a)->index.size()){
            Opn Temp = Opn(NewTemp(),Result.Type,MaxTempVarOffset+MaxVarSize);
            TempVarOffset+=TypeWidth[Result.Type];           //修改临时变量的偏移量
            if (TempVarOffset>MaxTempVarOffset)
                MaxTempVarOffset=TempVarOffset;
            ARGS.push_back(IRCode(READARRAY,Result,((VarAST*)a)->ArrOff,Temp));
            ARGS.push_back(IRCode(ARG,Opn1,Opn(),Temp));
        }else{
            ARGS.push_back(IRCode(ARG,Opn1,Opn(),Result));
        }
    }
    it=IRCodes.end();
    IRCodes.splice(it,ARGS);
    Opn1.Name=Name;
    Opn1.Type=FuncRef->Type;
    Opn1.SymPtr=FuncRef;
    if (FuncRef->Type!=T_VOID)
    {
        Result=Opn(NewTemp(),FuncRef->Type,TempVarOffset+MaxVarSize);//临时变量保存返回结果
        TempVarOffset+=TypeWidth[FuncRef->Type];
        if (TempVarOffset>MaxTempVarOffset)
            MaxTempVarOffset=TempVarOffset;
        IRCodes.push_back(IRCode(CALL,Opn1,Opn(),Result));
    }
    else IRCodes.push_back(IRCode(CALL0,Opn1,Opn(),Opn())); //返回值为void
    return Result;
}


void FuncCallAST::GenIR(int &TempVarOffset,std::string LabelTrue,std::string LabelFalse)
{
//根据函数返回值确定转移方向
    Opn Result = GenIR(TempVarOffset);
    Opn Zero = Opn("_Const",T_INT,0);
    Zero.constINT = 0;
    IRCodes.push_back(IRCode(JNE,Result,Zero,Opn(LabelTrue,0,0)));
    IRCodes.push_back(IRCode(GOTO,Opn(),Opn(),Opn(LabelFalse,0,0)));
}