//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-语法树表示(第1部分) 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//构造语法树对象
ZYSyntaxTree::ZYSyntaxTree(ESyntaxTreeType type,char *name,int lineno,bool hasChildren)
{
    this->type=type;
    this->lineno=lineno;
    parent=NULL;
    this->name=name;
    this->nameTokenStart=0;
    this->nameTokenEnd=0;
    if(hasChildren)
    {
        children=new vector<ZYSyntaxTree *>();
    }
    else
    {
        children=NULL;
    }
}

//析构语法树对象
ZYSyntaxTree::~ZYSyntaxTree(void)
{
    vector<ZYSyntaxTree *>::iterator it;

    Release(name);

    if(children!=NULL)
    {
        for(it=children->begin();it<children->end();it++)
        {
            Release(*it);
        }

        delete children;
    }
}

//设置名称词法标记位置
void ZYSyntaxTree::SetNameTokenLoc(int nameTokenStart,int nameTokenEnd)
{
    this->nameTokenStart=nameTokenStart;
    this->nameTokenEnd=nameTokenEnd;
}

//打印语法树对象
void ZYSyntaxTree::Print(int indent)
{
}

//获取语法树对象类型
ESyntaxTreeType ZYSyntaxTree::GetType(void)
{
    return type;
}

//获取语法树对象名称
char *ZYSyntaxTree::GetName(void)
{
    return name;
}

//获取语法树对象孩子列表
vector<ZYSyntaxTree *> *ZYSyntaxTree::GetChildren(void)
{
    return this->children;
}

//将孩子加入孩子列表
void ZYSyntaxTree::AddChild(ZYSyntaxTree *child)
{
    if(child!=NULL)
    {
        children->push_back(child);
    }
}

//从孩子列表查找孩子
ZYSyntaxTree *ZYSyntaxTree::FindChild(ESyntaxTreeType type,char *name)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==type&&
            strcmp((*it)->GetName(),name)==0)
        {
            return (*it);
        }
    }

    return NULL;
}

//打印一个孩子,带缩进
void ZYSyntaxTree::PrintChild(ZYSyntaxTree *child,int indent)
{
    if(child!=NULL)
    {
        child->Print(indent+2);
    }
}

//打印孩子列表中的所有孩子,带缩进
void ZYSyntaxTree::PrintChildren(int indent)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Print(indent+2);
    }
}

//执行列表中的所有孩子
void ZYSyntaxTree::ExecuteChildren(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Execute(executor);
    }
}

//执行列表中特定类型的孩子
void ZYSyntaxTree::ExecuteChildren(ZYExecutor *executor,ESyntaxTreeType type)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==type)
        {
            (*it)->Execute(executor);
        }
    }
}

//反顺序执行列表中特定类型的孩子
void ZYSyntaxTree::ReverseExecuteChildren(ZYExecutor *executor,ESyntaxTreeType type)
{
    vector<ZYSyntaxTree *>::reverse_iterator it;

    for(it=children->rbegin();it<children->rend();it++)
    {
        if((*it)->GetType()==type)
        {
            (*it)->Execute(executor);
        }
    }
}

//执行列表中的所有孩子(第2遍)
void ZYSyntaxTree::ExecuteChildren1(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Execute1(executor);
    }
}

//安全地释放一棵语法树
void ZYSyntaxTree::Release(ZYSyntaxTree *syntaxTree)
{
    if(syntaxTree!=NULL)
    {
        delete syntaxTree;
    }
}

//安全地释放一条字符串
void ZYSyntaxTree::Release(char *s)
{
    if(s!=NULL&&(strcmp(s,"")!=0))
    {
        delete []s;
    }
}

//打印指定个字符的字符缩进
void ZYSyntaxTree::PrintIndent(int indent)
{
    int i;

    for(i=0;i<indent;i++)
    {
        cout<<" ";
    }
}

//安全地打印一条字符串
void ZYSyntaxTree::PrintString(char *s)
{
    if(s!=NULL)
    {
        cout<<s;
    }
    else
    {
        cout<<"NULL";
    }
}

//打印一个运算符
void ZYSyntaxTree::PrintOperation(ETokenType operation)
{
    cout<<"\"";
    
    switch(operation)
    {
    case ETokenType_And:
        cout<<"AND";
        break;
    case ETokenType_Avg:
        cout<<"AVG";
        break;
    case ETokenType_Count:
        cout<<"COUNT";
        break;
    case ETokenType_Exists:
        cout<<"EXISTS";
        break;
    case ETokenType_In:
        cout<<"IN";
        break;
    case ETokenType_Not:
        cout<<"NOT";
        break;
    case ETokenType_NotIn:
        cout<<"NOT IN";
        break;
    case ETokenType_Or:
        cout<<"OR";
        break;
    case ETokenType_Sum:
        cout<<"SUM";
        break;
    case ETokenType_Max:
        cout<<"MAX";
        break;
    case ETokenType_Min:
        cout<<"MIN";
        break;
    case ETokenType_EQ:
        cout<<"=";
        break;
    case ETokenType_NE:
        cout<<"!=";
        break;
    case ETokenType_GT:
        cout<<">";
        break;
    case ETokenType_LT:
        cout<<"<";
        break;
    case ETokenType_GE:
        cout<<">=";
        break;
    case ETokenType_LE:
        cout<<"<=";
        break;
    case ETokenType_Plus:
        cout<<"+";
        break;
    case ETokenType_Minus:
        cout<<"-";
        break;
    case ETokenType_Multiply:
        cout<<"*";
        break;
    case ETokenType_Divide:
        cout<<"/";
        break;
    }

    cout<<"\"";
}

//对语法树对象进行语义分析
void ZYSyntaxTree::Analyze(ZYAnalyzer *analyzer)
{
}

//执行语法树对象
void ZYSyntaxTree::Execute(ZYExecutor *executor)
{
}

//执行语法树对象(第2遍)
void ZYSyntaxTree::Execute1(ZYExecutor *executor)
{
}

//动态求语法树对象值
EResultType ZYSyntaxTree::GetResult(ZYExecutor *executor,void *pData)
{
    *(int *)pData=0;

    return EResultType_Integer;
}

//计算应下降到的表格索引
int ZYSyntaxTree::CalcDescentTableIndex(ZYExecutor *executor)
{
    return 0;
}

//执行语句开始信息输出
void ZYSyntaxTree::ExecuteStmtStart(ZYExecutor *executor)
{
    executor->SetTempTableNo(0);//清零临时表编号

    executor->SetTempColumnNo(0);//清零临时列编号

    executor->ClearSourceTables();

    if(executor->GetDisplayAST()==true)
    {
        cout<<"抽象语法树:"<<endl;
        Print(0);
    }

    if( this->GetType()==ESyntaxTreeType_SelectStmt&&
        executor->GetDisplayTables()==true)
    {
        cout<<"查询源表:"<<endl;
    }

    executor->GetProfiler()->Start();
}

//执行语句结束信息输出
void ZYSyntaxTree::ExecuteStmtEnd(ZYExecutor *executor)
{
    executor->GetProfiler()->End();
    
    if(executor->GetDisplayTime()==true)
    {
        cout<<"查询耗时:"<<executor->GetProfiler()->GetTimeMs()<<"ms"<<endl;
    }

    if(executor->GetDisplayResult()==true)
    {
        if(GetType()!=ESyntaxTreeType_DropTable&&
           GetType()!=ESyntaxTreeType_DropView)
        {
            cout<<"查询结果:"<<endl;
            IZYTable *t_dest=executor->GetCurrentTable();
            executor->DisplayTable(t_dest,true);
        }
    }

    cout<<endl;
}

//构造全局对象
ZYGlobal::ZYGlobal(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_Global,"",lineno,true)
{
}

//打印全局对象
void ZYGlobal::Print(int indent)
{
    cout<<fixed;            //定点形式输出

    cout.precision(2);      //保留2个小数点

    PrintIndent(indent);

    cout<<"<全局";

    cout<<" 行号="<<lineno;
    
    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</全局>"<<endl;
}

//对全局对象进行语义分析
void ZYGlobal::Analyze(ZYAnalyzer *analyzer)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Analyze(analyzer);
    }
}

//执行全局对象
void ZYGlobal::Execute(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->ExecuteStmtStart(executor);
        (*it)->Execute(executor);
        (*it)->ExecuteStmtEnd(executor);

        //如果检测到查询执行错误则停止分析

        if(executor->IsError())
        {
            cout<<endl<<"查询执行出错,停止分析..."<<endl;
            break;
        }
    }
}

//构造创建表格对象
ZYCreateTable::ZYCreateTable(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_CreateTable,name,lineno,true)
{
    this->primaryKey="";
    this->primaryKeyTokenStart=0;
    this->primaryKeyTokenEnd=0;
}

//析构创建表格对象
ZYCreateTable::~ZYCreateTable(void)
{
    Release(primaryKey);
}

//设置主键词法标记位置
void ZYCreateTable::SetPrimaryKeyTokenLoc(int primaryKeyTokenStart,int primaryKeyTokenEnd)
{
    this->primaryKeyTokenStart=primaryKeyTokenStart;
    this->primaryKeyTokenEnd=primaryKeyTokenEnd;
}
    

//设置表格主键
void ZYCreateTable::SetPrimaryKey(char *primaryKey)
{
    this->primaryKey=primaryKey;
}

//打印创建表格对象
void ZYCreateTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<创建表格 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<" 主键=\""<<primaryKey<<"\"";

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</创建表格>"<<endl;
}

//对创建表格对象进行语义分析
void ZYCreateTable::Analyze(ZYAnalyzer *analyzer)
{
    analyzer->SetCurrentStmt(this);

    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);

    vector<ZYSyntaxTree *>::iterator it;

    bool primaryKeyValid=false;//主键有效

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Analyze(analyzer);
        if((*it)->GetType()==ESyntaxTreeType_ColumnDecl)
        {
            if(strcmp(primaryKey,(*it)->GetName())==0)
            {
                primaryKeyValid=true;
            }
        }
    }

    if(primaryKeyValid)
    {
        analyzer->GetSource()->SetAcknowledgedFlag(primaryKeyTokenStart,primaryKeyTokenEnd);
    }
    else
    {
        cout<<lineno<<":语义错误:主键\'"<<primaryKey<<"\'在其所属表格的定义中不存在..."<<endl;
        analyzer->Error();
    }
}

//执行创建表格对象
void ZYCreateTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    if(t!=NULL)
    {
        cout<<"表格:\'"<<name<<"\'已存在"<<endl;
    }
    else
    {
        t=db->CreateTable(name);

        if(t!=NULL)
        {
            cout<<"表格:\'"<<name<<"\'创建成功"<<endl;
        }
        else
        {
            cout<<"表格:\'"<<name<<"\'创建失败"<<endl;
            return;
        }
    }

    executor->SetCurrentTable(t);

    executor->SetPrimaryKey(primaryKey);

    ExecuteChildren(executor);

    t->SetPrimaryKey(primaryKey);
}

//构造外键声明对象
ZYForeignKey::ZYForeignKey(char *name,int lineno)
                            :ZYSyntaxTree(ESyntaxTreeType_ForeignKey,name,lineno,false)
{
    this->foreignTable=NULL;
    this->foreignColumn=NULL;
    this->foreignTableTokenStart=0;
    this->foreignTableTokenEnd=0;
    this->foreignColumnTokenStart=0;
    this->foreignColumnTokenEnd=0;
}

//设置外键表格名
void ZYForeignKey::SetForeignTable(char *foreignTable)
{
    this->foreignTable=foreignTable;
}

//设置外键列名
void ZYForeignKey::SetForeignColumn(char *foreignColumn)
{
    this->foreignColumn=foreignColumn;
}

//设置外键表格词法标记位置
void ZYForeignKey::SetForeignTableTokenLoc(int foreignTableTokenStart,int foreignTableTokenEnd)
{
    this->foreignTableTokenStart=foreignTableTokenStart;
    this->foreignTableTokenEnd=foreignTableTokenEnd;
}

//设置外键列词法标记位置
void ZYForeignKey::SetForeignColumnTokenLoc(int foreignColumnTokenStart,int foreignColumnTokenEnd)
{
    this->foreignColumnTokenStart=foreignColumnTokenStart;
    this->foreignColumnTokenEnd=foreignColumnTokenEnd;
}

//打印外键声明对象
void ZYForeignKey::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<外键声明 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<"></外键声明>"<<endl;
}

//对外键声明对象进行语义分析
void ZYForeignKey::Analyze(ZYAnalyzer *analyzer)
{
    ZYCreateTable *createTable=(ZYCreateTable *)analyzer->GetCurrentStmt();

    vector<ZYSyntaxTree *>::iterator it;

    bool foreignKeyValid=false;//外键有效

    for(it=createTable->GetChildren()->begin();it<createTable->GetChildren()->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_ColumnDecl)
        {
            if(strcmp(name,(*it)->GetName())==0)
            {
                foreignKeyValid=true;
            }
        }
    }

    if(foreignKeyValid)
    {
        analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
    }
    else
    {
        cout<<lineno<<":语义错误:外键\'"<<name<<"\'在其所属表格的定义中不存在..."<<endl;
        analyzer->Error();
    }

    analyzer->GetSource()->SetAcknowledgedFlag(foreignTableTokenStart,foreignTableTokenEnd);

    analyzer->GetSource()->SetAcknowledgedFlag(foreignColumnTokenStart,foreignColumnTokenEnd);
}   

//执行外键声明对象
void ZYForeignKey::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    char *primaryKey;

    char table_name[100];

    char index_name[100];

    db=executor->GetCurrentDatabase();

    t=executor->GetCurrentTable();

    primaryKey=executor->GetPrimaryKey();

    t->GetName(table_name);

    sprintf(index_name,"%s_%s",table_name,this->name);

    IZYIndex *index=db->CreateIndex(index_name,t,name,primaryKey);
}

//构造撤消表格对象
ZYDropTable::ZYDropTable(char *name,int lineno)
                            :ZYSyntaxTree(ESyntaxTreeType_DropTable,name,lineno,false)
{
}

//打印撤消表格对象
void ZYDropTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<撤消表格 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<"></撤消表格>"<<endl;
}

//对撤消表格对象进行语义分析
void ZYDropTable::Analyze(ZYAnalyzer *analyzer)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=analyzer->GetCurrentDatabase();

    t=db->GetTableI(name);

    if(t!=NULL)
    {
        analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
    }
    else
    {
        cout<<lineno<<":语义错误:表格\'"<<name<<"\'不存在..."<<endl;
        analyzer->Error();
    }
}    

//执行撤消表格对象
void ZYDropTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    if(t==NULL)
    {
        cout<<"表格\'"<<name<<"\'不存在..."<<endl;
        return;
    }

    db->DropTable(t);

    cout<<"表格\'"<<name<<"\'撤消成功..."<<endl;
}

//构造撤消视图对象
ZYDropView::ZYDropView(char *name,int lineno)
                            :ZYSyntaxTree(ESyntaxTreeType_DropView,name,lineno,false)
{
}
//打印撤消视图对象
void ZYDropView::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<撤消视图 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<"></撤消视图>"<<endl;
}

//对撤消视图对象进行语义分析
void ZYDropView::Analyze(ZYAnalyzer *analyzer)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=analyzer->GetCurrentDatabase();

    t=db->GetTableI(name);

    if(t!=NULL)
    {
        analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
    }
    else
    {
        cout<<lineno<<":语义错误:视图\'"<<name<<"\'不存在..."<<endl;
        analyzer->Error();
    }
}   

//执行撤消视图对象
void ZYDropView::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    db->DropTable(t);

    cout<<"视图\'"<<name<<"\'撤消成功"<<endl;
}

//构造列声明对象
ZYColumnDecl::ZYColumnDecl(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_ColumnDecl,name,lineno,false)
{
    this->dataType=EColumnType_String;
    this->dataLength=16;
}

//设置数据类型
void ZYColumnDecl::SetDataType(int dataType)
{
    this->dataType=dataType;
}

//设置数据长度
void ZYColumnDecl::SetDataLength(int dataLength)
{
    this->dataLength=dataLength;
}

//打印列声明对象
void ZYColumnDecl::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<列声明 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<" 类型="<<dataType;

    cout<<" 长度="<<dataLength;

    cout<<">";

    cout<<"</列声明>"<<endl;
}

//对列声明对象进行语义分析
void ZYColumnDecl::Analyze(ZYAnalyzer *analyzer)
{
    IZYTable *t_dest;
    char tableName[1000];
    ZYSyntaxTree *currentStmt=analyzer->GetCurrentStmt();
        
    if( currentStmt!=NULL&&
        currentStmt->GetType()==ESyntaxTreeType_AlterTable&&
        ((ZYAlterTable *)currentStmt)->GetOperation()==ETokenType_Modify)
    {
        t_dest=analyzer->GetCurrentTable();

        if(t_dest!=NULL)
        {
            if(t_dest->FindColumn(name)!=-1)
            {
                analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
            }
            else
            {
                if(strcmp(name,"")!=0)
                {
                    t_dest->GetName(tableName);
                    cout<<lineno<<":语义错误:列\'"<<name<<"\'在表格\'"<<tableName<<"\'中不存在..."<<endl;
                    analyzer->Error();
                }
            }
        }
        return;
    }

    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
}    

//执行列声明对象
void ZYColumnDecl::Execute(ZYExecutor *executor)
{
    IZYTable *t;
    
    t=executor->GetCurrentTable();
    
    t->AppendColumn(name,(EColumnType)dataType,dataLength,0);
}

//构造改变表格对象
ZYAlterTable::ZYAlterTable(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AlterTable,name,lineno,true)
{
    this->operation=ETokenType_Add;
    this->columnTokenStart=0;
    this->columnTokenEnd=0;
}

//设置列名词法标记位置
void ZYAlterTable::SetColumnTokenLoc(int columnTokenStart,int columnTokenEnd)
{
    this->columnTokenStart=columnTokenStart;
    this->columnTokenEnd=columnTokenEnd;
}

//设置操作类型
void ZYAlterTable::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//获取操作类型
ETokenType ZYAlterTable::GetOperation(void)
{
    return operation;
}    

//打印改变表格对象
void ZYAlterTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<改变表格 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<" 操作=\"";

    switch(operation)
    {
    case ETokenType_Add:
        cout<<"增加/ADD";
        break;
    case ETokenType_Modify:
        cout<<"修改/MODIFY";
        break;
    case ETokenType_Drop:
        cout<<"撤销/DROP";
        break;
    }

    cout<<"\"";

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</改变表格>"<<endl;
}

//对改变表格对象进行语义分析
void ZYAlterTable::Analyze(ZYAnalyzer *analyzer)
{
    IZYDatabase *db;

    IZYTable *t_dest;

    analyzer->SetCurrentStmt(this);

    db=analyzer->GetCurrentDatabase();

    t_dest=db->GetTableI(name);

    analyzer->SetCurrentTable(t_dest);

    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_ColumnRef)
        {
            ((ZYColumnRef*)(*it))->Analyze2(analyzer);
        }
        else
        {
            (*it)->Analyze(analyzer);
        }
    }

    analyzer->SetCurrentTable(NULL);
}

//执行改变表格对象
void ZYAlterTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    ZYColumnRef *columnRef;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;

        return;
    }

    t=db->GetTableI(name);

    if(t==NULL)
    {
        cout<<"表格\'"<<name<<"\'不存在"<<endl;

        return;
    }

    if( operation==ETokenType_Add||
        operation==ETokenType_Modify)
    {
        executor->SetCurrentTable(t);

        ExecuteChildren(executor);
    }
    else if(operation==ETokenType_Drop)
    {
        if(children->size()==1)
        {
            columnRef=(ZYColumnRef *)((*children)[0]);

            t->DeleteColumn(columnRef->GetName());
        }
    }

    if(operation==ETokenType_Add)
    {
        cout<<"表格改变成功，列已增加..."<<endl;
    }
    else if(operation==ETokenType_Modify)
    {
        cout<<"表格改变成功，列已修改..."<<endl;
    }
    else if(operation==ETokenType_Drop)
    {
        cout<<"表格改变成功，列已撤销..."<<endl;
    }
}

//构造创建视图对象
ZYCreateView::ZYCreateView(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_CreateView,name,lineno,true)
{
    this->selectStmt=NULL;
}

//析构创建视图对象
ZYCreateView::~ZYCreateView(void)
{
    Release(selectStmt);
}

//设置视图选择语句
void ZYCreateView::SetSelectStmt(ZYSelectStmt *selectStmt)
{
    this->selectStmt=selectStmt;
}

//打印创建视图对象
void ZYCreateView::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<创建视图 名称=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintChild(selectStmt,indent);

    PrintIndent(indent);
    
    cout<<"</创建表格>"<<endl;
}

//对创建视图对象进行语义分析
void ZYCreateView::Analyze(ZYAnalyzer *analyzer)
{
    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_ColumnRef)
        {
            ((ZYColumnRef *)(*it))->Analyze1(analyzer);
        }
    }

    selectStmt->Analyze(analyzer);
}

//执行创建视图树对象
void ZYCreateView::Execute(ZYExecutor *executor)
{
    selectStmt->SetTableName(name);
    selectStmt->Execute(executor);
}