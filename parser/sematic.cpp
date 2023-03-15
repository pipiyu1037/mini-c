#include "ast.h"
#include "parser.tab.hpp"
SymbolStackDef AST::SymbolStack=SymbolStackDef();    //��ʼ����̬��Ա���ű�

std::map <int,int> TypeWidth={{T_CHAR,4},{T_INT,4},{T_FLOAT,8}}; //��������ռ�ֽ���
std::map <char,std::string> KindName={{'V',"����"},{'F',"����"},{'P',"�β�"},{'A',"����"},{'C',"����"}}; 
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
        std::cout<<"��"<<a.Line<<"�С���"<<a.Column<<"�д�����: "<<a.ErrMsg<<std::endl;
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
        std::cout<<" ���: "<<i<<std::endl;
        std::cout<<" �� �� ��           ����    ����    �� ��   ������Ϣ"<<std::endl;
        std::cout<<"----------------------------------------------------------------------"<<std::endl;
        if (SYM->Symbols.at(i)->Symbols.size()==0)
            std::cout<<"  �� ��"<<std::endl;
        else
        for(int j=0;j<SYM->Symbols.at(i)->Symbols.size();j++)
        {
            Symbol *SymPtr=SYM->Symbols.at(i)->Symbols.at(j);   //ȡ��i���j�����Ŷ����ָ��
            std::cout.width(20);
            std::cout<<SymPtr->Name;
            std::cout.width(8);
            if (SymPtr->Kind=='V' || SymPtr->Kind=='P'||SymPtr->Kind=='A')   //�����Ǳ���,�β�,��ʾ����
                std::cout<<((VarSymbol*)SymPtr)->Alias;
            else std::cout<<" ";
            std::cout.width(8);
            std::cout<<SymbolMap[SymPtr->Type];
            std::cout.width(8);
            std::cout<<KindName[SymPtr->Kind];
            if (SymPtr->Kind=='V' || SymPtr->Kind=='P')   //�����Ǳ���,�β�
                std::cout<<"ƫ����: "<<((VarSymbol*)SymPtr)->Offset;
            else if (SymPtr->Kind=='F')  //�����Ǻ���
            {
                std::cout<<"�β���: "<<((FuncSymbol*)SymPtr)->ParamNum;
                std::cout<<"  �����ռ�: "<<((FuncSymbol*)SymPtr)->ARSize;
            }
            else if (SymPtr->Kind=='A'){
                //���������飬��Ҫ��ʾ��ά��С
                std::cout<<"ƫ����: "<<((VarSymbol*)SymPtr)->Offset;
                for(auto d:((VarSymbol*)SymPtr)->dims){
                    std::cout<<" ["<<d<<"]";
                }
                std::cout<<"  �����ռ�: "<<((VarSymbol*)SymPtr)->ARsize;
            }  
            std::cout<<std::endl;
        }
        std::cout<<"----------------------------------------------------------------------"<<std::endl;
    }
}
bool IsLeftValue(ExpAST *PExp)
{
    if (typeid(*PExp)==typeid(VarAST)) return true; //�Լ򵥱����������±�������ṹ����,��Ҫ����ű�
    return false;
}

Symbol * SymbolStackDef::LocateNameCurrent(std::string Name)  //�ڵ�ǰ(���ڲ�)�������в��Ҹ÷����Ƿ��ж���
{
    SymbolsInAScope *curScope=Symbols.back();
    for(int i=0;i<curScope->Symbols.size();i++)
        if (curScope->Symbols.at(i)->Name==Name)
            return curScope->Symbols.at(i);
    return nullptr;
}

Symbol * SymbolStackDef::LocateNameGlobal(std::string Name)//�������⣬�������ű��в��Ҹ÷����Ƿ��ж���
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
    SymbolsInAScope *Global=new SymbolsInAScope();  //ȫ�ֱ�������������ű���¼�ⲿ������������
    SymbolStack.Symbols.push_back(Global);
    GlobalSymbolTable=Global;                       //��������һ��ȫ�ַ��ű�

    //Ԥ������ȱʡ����read��write
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

void ExtVarDefAST::Semantics(int &Offset)  //�ⲿ������������
{
    for(auto a:ExtVars)
        a->Semantics(Offset,Type);
}


void VarDecAST::Semantics(int &Offset,TypeAST *Type)
{
    if (!SymbolStack.LocateNameCurrent(Name))  //��ǰ������δ���壬������������ű�
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
        if (Exp)                      //�г�ֵ���ʽʱ�Ĵ���
           Exp->Semantics(Offset);
        SymbolStack.Symbols.back()->Symbols.push_back(VarDefPtr);
    }
    else Errors::ErrorAdd(Line,Column,"���� "+Name+" �ظ�����") ;

}

void DefAST::Semantics(int &Offset)
{    //������ȡ�������Ž����������
     for(auto a:LocVars)
        a->Semantics(Offset,Type);
}

void BasicTypeAST::Semantics(int &Offset)
{
}

void FuncDefAST::Semantics(int &Offset)
{
    if (!SymbolStack.LocateNameCurrent(Name))  //��ǰ������δ���壬������������ű�
    {
        int Offset=12;           //�ֲ�����ƫ������ʼ��,Ԥ��12���ֽڴ�ŷ��ص�ַ����Ϣ���ɸ���ʵ������޸�
        MaxVarSize=12;            //���㺯��������Ҫ���������
        FuncDefPtr=new FuncSymbol();
        FuncDefPtr->Name=Name;
        FuncDefPtr->Kind='F';
        if (typeid(*Type)==typeid(BasicTypeAST))     //���������ķ������ͣ�Ŀǰ����������T_CHAR,T_INT,T_FLOAT
             FuncDefPtr->Type=((BasicTypeAST*)Type)->Type;
        FuncDefPtr->ParamNum=Params.size();

        SymbolsInAScope *Local=new SymbolsInAScope();  //���ɺ����������������
        FuncDefPtr->ParamPtr=Local;                    //�������ű��ָ���β�
        SymbolStack.Symbols.back()->Symbols.push_back(FuncDefPtr);//��д�������ŵ����ű�

        
        SymbolStack.Symbols.push_back(Local);          //��������ű����βΣ���ջ
        Body->LocalSymbolTable=Local;
        for(auto a:Params)
            a->Semantics(Offset);              //δ���ǲ����üĴ�����ֻ�Ǽ���AR�з��䵥Ԫ
        Body->Semantics(Offset);               //�Ժ����еı�������AR�н��ڲ�������䵥Ԫ
        FuncDefPtr->ARSize=MaxVarSize; //����������Ҫ�ռ��С��δ������ʱ�������������ټ���ʱ������Ԫ�õ�AR��С
    }
    else Errors::ErrorAdd(Line,Column,"���� "+Name+" �ظ�����");
}
void ParamAST::Semantics(int &Offset)
{
    if (!SymbolStack.LocateNameCurrent(ParamName->Name))  //��ǰ������δ�ظ����壬���β���������ű�
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
    else Errors::ErrorAdd(Line,Column,"�β��� "+ParamName->Name+" �ظ�����");

}

/**************�����ʾ******************************/
void CompStmAST::Semantics(int &Offset)
{
    if (!LocalSymbolTable)          //������Ǻ�����ĸ�����䣬���������ɾֲ����ű�
    {
        SymbolsInAScope *Local=new SymbolsInAScope();  //ȫ�ֱ�������������ű���¼�ⲿ������������
        SymbolStack.Symbols.push_back(Local);
        LocalSymbolTable=Local;                       //��������һ�����ű�
    }
    for(auto a:Decls){
        a->Semantics(Offset);
    }
        
    if (Offset>MaxVarSize) MaxVarSize=Offset;
    for(auto a:Stms)
    {
        int Offset_S=Offset;      //ǰ����������ʹ��ͬһƬ��Ԫ������ȡ���ֵ�����ﱣ����ʼƫ����
        a->Parent = this;
        a->Semantics(Offset);
        if (Offset>MaxVarSize) MaxVarSize=Offset;
        Offset=Offset_S;
    }
    std::cout<<"\n\n********************��ǰ���������ű�״̬**************************"<<std::endl;
    DisplaySymbolTable(&SymbolStack);
    std::cout<<std::endl<<std::endl;
    SymbolStack.Symbols.pop_back();     //�������ķ��ű���ջ
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
            Errors::ErrorAdd(Line,Column,"�����������Ͳ�ƥ�� "+FunPtr->Name);
            return;
        }
    }else{//���return���û�б��ʽ�Һ����ķ������Ͳ�Ϊvoid������
        if(FunPtr->Type!=T_VOID){
            Errors::ErrorAdd(Line,Column,"�����������Ͳ�ƥ�� "+FunPtr->Name);
            return;
        }
    }
}

void BreakStmAST::Semantics(int &Offset){
    while(Parent){
        if(typeid(*Parent)==typeid(ForStmAST)||typeid(*Parent)==typeid(WhileStmAST)){return;}
        Parent = Parent->Parent;
    }
    Errors::ErrorAdd(Line,Column,"break��䲻��for����while�����");
}
void ContinueStmAST::Semantics(int &Offset){
    while(Parent){
        if(typeid(*Parent)==typeid(ForStmAST)||typeid(*Parent)==typeid(WhileStmAST)){return;}
        Parent = Parent->Parent;
    }
    Errors::ErrorAdd(Line,Column,"continue��䲻��for����while�����");
}
/**************���ʽ��ʾ******************************/
void VarAST::Semantics(int &Offset)
{
    if (VarRef=(VarSymbol *)SymbolStack.LocateNameGlobal(Name))
    {
        //����Ǻ�����������
        if(VarRef->Kind=='F'){
            Errors::ErrorAdd(Line,Column,"�Ժ��������÷Ǻ���������ʽ���� "+Name) ;
            return;
        }
        //���������
        if(VarRef->Kind=='A'){
            if(index.size()>VarRef->dims.size()){
                Errors::ErrorAdd(Line,Column,Name+"�����±��������� ");
            }
            for(auto i:index){
                i->Semantics(Offset);
                if(i->Type!=T_INT){
                    int a = i->Type;
                    Errors::ErrorAdd(Line,Column,Name+"�����±겻������ ");
                }
            }
        }else{//����������飬�������±����
            if(index.size()){
                Errors::ErrorAdd(Line,Column,"��������������±���� ");
            }
        }
        //�򵥱�������ȡ������������
        this->Offset = VarRef->Offset;
        this->Type = VarRef->Type;
    }
    else Errors::ErrorAdd(Line,Column,"����δ����ķ��� "+Name);

}

void ConstAST::Semantics(int &Offset)
{
    //��ȡ��������
    
}

void AssignAST::Semantics(int &Offset)
{
    LeftValExp->Semantics(Offset);
    if (!IsLeftValue(LeftValExp))
        Errors::ErrorAdd(Line,Column,"�Է���ֵ���ʽ��ֵ");
    RightValExp->Semantics(Offset);
    if(LeftValExp->Type!=RightValExp->Type){
        Errors::ErrorAdd(Line,Column,"���ʽ���Ͳ�ƥ��");
    }
    Type = LeftValExp->Type;
}

void BinaryExprAST::Semantics(int &Offset)
{
    LeftExp->Semantics(Offset);
    RightExp->Semantics(Offset);
    //��������ֵ���ͣ��ж��Ƿ��ܽ������㣬��ȷ������������
    if(LeftExp->Type!=RightExp->Type){
        Errors::ErrorAdd(Line,Column,"���ʽ���Ͳ�ƥ��");
    }
    Type = LeftExp->Type;
}

void UnaryExprAST::Semantics(int &Offset)
{
    Exp->Semantics(Offset);
    this->Type = Exp->Type;
    if((Op==DMINUS||Op==DPLUS)&&!IsLeftValue(Exp)){
        Errors::ErrorAdd(Line,Column,"�Է���ֵ���������Լ� ");
        return;
    }
}

void FuncCallAST::Semantics(int &Offset)
{
    if (FuncRef=(FuncSymbol *)SymbolStack.LocateNameGlobal(Name))
    {
        this->Type = FuncRef->Type;
        if(FuncRef->Kind!='F'){
            Errors::ErrorAdd(Line,Column,"�ԷǺ��������ú���������ʽ "+Name);
            return;
        }
        if(FuncRef->ParamNum!=Params.size()){
            Errors::ErrorAdd(Line,Column,"��������������ƥ�� "+Name);
            return;
        }
        if(FuncRef->Name==string("write")){
            Params[0]->Semantics(Offset);
        }else{
            for(int i = 0;i<Params.size();i++)
            {
            //���ʵ�α��ʽ�������β����Ƿ�һ�£������Ƿ�һ��
                Params[i]->Semantics(Offset);
                if(Params[i]->Type!=FuncRef->ParamPtr->Symbols[i]->Type){
                    Errors::ErrorAdd(Line,Column,"�����������Ͳ�ƥ�� "+Name);
                    return;
                }
            }
        }
    }
    else Errors::ErrorAdd(Line,Column,"����δ����ĺ��� "+Name);
}
