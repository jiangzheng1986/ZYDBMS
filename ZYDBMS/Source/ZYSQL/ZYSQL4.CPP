//---------------------------------------------------------
//���²�ѯ����(ZYSQL)-�﷨����ʾ(��1����) ����:����
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//�����﷨������
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

//�����﷨������
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

//�������ƴʷ����λ��
void ZYSyntaxTree::SetNameTokenLoc(int nameTokenStart,int nameTokenEnd)
{
    this->nameTokenStart=nameTokenStart;
    this->nameTokenEnd=nameTokenEnd;
}

//��ӡ�﷨������
void ZYSyntaxTree::Print(int indent)
{
}

//��ȡ�﷨����������
ESyntaxTreeType ZYSyntaxTree::GetType(void)
{
    return type;
}

//��ȡ�﷨����������
char *ZYSyntaxTree::GetName(void)
{
    return name;
}

//��ȡ�﷨���������б�
vector<ZYSyntaxTree *> *ZYSyntaxTree::GetChildren(void)
{
    return this->children;
}

//�����Ӽ��뺢���б�
void ZYSyntaxTree::AddChild(ZYSyntaxTree *child)
{
    if(child!=NULL)
    {
        children->push_back(child);
    }
}

//�Ӻ����б����Һ���
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

//��ӡһ������,������
void ZYSyntaxTree::PrintChild(ZYSyntaxTree *child,int indent)
{
    if(child!=NULL)
    {
        child->Print(indent+2);
    }
}

//��ӡ�����б��е����к���,������
void ZYSyntaxTree::PrintChildren(int indent)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Print(indent+2);
    }
}

//ִ���б��е����к���
void ZYSyntaxTree::ExecuteChildren(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Execute(executor);
    }
}

//ִ���б����ض����͵ĺ���
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

//��˳��ִ���б����ض����͵ĺ���
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

//ִ���б��е����к���(��2��)
void ZYSyntaxTree::ExecuteChildren1(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Execute1(executor);
    }
}

//��ȫ���ͷ�һ���﷨��
void ZYSyntaxTree::Release(ZYSyntaxTree *syntaxTree)
{
    if(syntaxTree!=NULL)
    {
        delete syntaxTree;
    }
}

//��ȫ���ͷ�һ���ַ���
void ZYSyntaxTree::Release(char *s)
{
    if(s!=NULL&&(strcmp(s,"")!=0))
    {
        delete []s;
    }
}

//��ӡָ�����ַ����ַ�����
void ZYSyntaxTree::PrintIndent(int indent)
{
    int i;

    for(i=0;i<indent;i++)
    {
        cout<<" ";
    }
}

//��ȫ�ش�ӡһ���ַ���
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

//��ӡһ�������
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

//���﷨����������������
void ZYSyntaxTree::Analyze(ZYAnalyzer *analyzer)
{
}

//ִ���﷨������
void ZYSyntaxTree::Execute(ZYExecutor *executor)
{
}

//ִ���﷨������(��2��)
void ZYSyntaxTree::Execute1(ZYExecutor *executor)
{
}

//��̬���﷨������ֵ
EResultType ZYSyntaxTree::GetResult(ZYExecutor *executor,void *pData)
{
    *(int *)pData=0;

    return EResultType_Integer;
}

//����Ӧ�½����ı�������
int ZYSyntaxTree::CalcDescentTableIndex(ZYExecutor *executor)
{
    return 0;
}

//ִ����俪ʼ��Ϣ���
void ZYSyntaxTree::ExecuteStmtStart(ZYExecutor *executor)
{
    executor->SetTempTableNo(0);//������ʱ�����

    executor->SetTempColumnNo(0);//������ʱ�б��

    executor->ClearSourceTables();

    if(executor->GetDisplayAST()==true)
    {
        cout<<"�����﷨��:"<<endl;
        Print(0);
    }

    if( this->GetType()==ESyntaxTreeType_SelectStmt&&
        executor->GetDisplayTables()==true)
    {
        cout<<"��ѯԴ��:"<<endl;
    }

    executor->GetProfiler()->Start();
}

//ִ����������Ϣ���
void ZYSyntaxTree::ExecuteStmtEnd(ZYExecutor *executor)
{
    executor->GetProfiler()->End();
    
    if(executor->GetDisplayTime()==true)
    {
        cout<<"��ѯ��ʱ:"<<executor->GetProfiler()->GetTimeMs()<<"ms"<<endl;
    }

    if(executor->GetDisplayResult()==true)
    {
        if(GetType()!=ESyntaxTreeType_DropTable&&
           GetType()!=ESyntaxTreeType_DropView)
        {
            cout<<"��ѯ���:"<<endl;
            IZYTable *t_dest=executor->GetCurrentTable();
            executor->DisplayTable(t_dest,true);
        }
    }

    cout<<endl;
}

//����ȫ�ֶ���
ZYGlobal::ZYGlobal(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_Global,"",lineno,true)
{
}

//��ӡȫ�ֶ���
void ZYGlobal::Print(int indent)
{
    cout<<fixed;            //������ʽ���

    cout.precision(2);      //����2��С����

    PrintIndent(indent);

    cout<<"<ȫ��";

    cout<<" �к�="<<lineno;
    
    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</ȫ��>"<<endl;
}

//��ȫ�ֶ�������������
void ZYGlobal::Analyze(ZYAnalyzer *analyzer)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Analyze(analyzer);
    }
}

//ִ��ȫ�ֶ���
void ZYGlobal::Execute(ZYExecutor *executor)
{
    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->ExecuteStmtStart(executor);
        (*it)->Execute(executor);
        (*it)->ExecuteStmtEnd(executor);

        //�����⵽��ѯִ�д�����ֹͣ����

        if(executor->IsError())
        {
            cout<<endl<<"��ѯִ�г���,ֹͣ����..."<<endl;
            break;
        }
    }
}

//���촴���������
ZYCreateTable::ZYCreateTable(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_CreateTable,name,lineno,true)
{
    this->primaryKey="";
    this->primaryKeyTokenStart=0;
    this->primaryKeyTokenEnd=0;
}

//���������������
ZYCreateTable::~ZYCreateTable(void)
{
    Release(primaryKey);
}

//���������ʷ����λ��
void ZYCreateTable::SetPrimaryKeyTokenLoc(int primaryKeyTokenStart,int primaryKeyTokenEnd)
{
    this->primaryKeyTokenStart=primaryKeyTokenStart;
    this->primaryKeyTokenEnd=primaryKeyTokenEnd;
}
    

//���ñ�������
void ZYCreateTable::SetPrimaryKey(char *primaryKey)
{
    this->primaryKey=primaryKey;
}

//��ӡ�����������
void ZYCreateTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<�������� ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<" ����=\""<<primaryKey<<"\"";

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</��������>"<<endl;
}

//�Դ��������������������
void ZYCreateTable::Analyze(ZYAnalyzer *analyzer)
{
    analyzer->SetCurrentStmt(this);

    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);

    vector<ZYSyntaxTree *>::iterator it;

    bool primaryKeyValid=false;//������Ч

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
        cout<<lineno<<":�������:����\'"<<primaryKey<<"\'������������Ķ����в�����..."<<endl;
        analyzer->Error();
    }
}

//ִ�д����������
void ZYCreateTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"���ݿ�δ��..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    if(t!=NULL)
    {
        cout<<"����:\'"<<name<<"\'�Ѵ���"<<endl;
    }
    else
    {
        t=db->CreateTable(name);

        if(t!=NULL)
        {
            cout<<"����:\'"<<name<<"\'�����ɹ�"<<endl;
        }
        else
        {
            cout<<"����:\'"<<name<<"\'����ʧ��"<<endl;
            return;
        }
    }

    executor->SetCurrentTable(t);

    executor->SetPrimaryKey(primaryKey);

    ExecuteChildren(executor);

    t->SetPrimaryKey(primaryKey);
}

//���������������
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

//�������������
void ZYForeignKey::SetForeignTable(char *foreignTable)
{
    this->foreignTable=foreignTable;
}

//�����������
void ZYForeignKey::SetForeignColumn(char *foreignColumn)
{
    this->foreignColumn=foreignColumn;
}

//�����������ʷ����λ��
void ZYForeignKey::SetForeignTableTokenLoc(int foreignTableTokenStart,int foreignTableTokenEnd)
{
    this->foreignTableTokenStart=foreignTableTokenStart;
    this->foreignTableTokenEnd=foreignTableTokenEnd;
}

//��������дʷ����λ��
void ZYForeignKey::SetForeignColumnTokenLoc(int foreignColumnTokenStart,int foreignColumnTokenEnd)
{
    this->foreignColumnTokenStart=foreignColumnTokenStart;
    this->foreignColumnTokenEnd=foreignColumnTokenEnd;
}

//��ӡ�����������
void ZYForeignKey::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<������� ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<"></�������>"<<endl;
}

//�����������������������
void ZYForeignKey::Analyze(ZYAnalyzer *analyzer)
{
    ZYCreateTable *createTable=(ZYCreateTable *)analyzer->GetCurrentStmt();

    vector<ZYSyntaxTree *>::iterator it;

    bool foreignKeyValid=false;//�����Ч

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
        cout<<lineno<<":�������:���\'"<<name<<"\'������������Ķ����в�����..."<<endl;
        analyzer->Error();
    }

    analyzer->GetSource()->SetAcknowledgedFlag(foreignTableTokenStart,foreignTableTokenEnd);

    analyzer->GetSource()->SetAcknowledgedFlag(foreignColumnTokenStart,foreignColumnTokenEnd);
}   

//ִ�������������
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

//���쳷���������
ZYDropTable::ZYDropTable(char *name,int lineno)
                            :ZYSyntaxTree(ESyntaxTreeType_DropTable,name,lineno,false)
{
}

//��ӡ�����������
void ZYDropTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<�������� ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<"></��������>"<<endl;
}

//�Գ��������������������
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
        cout<<lineno<<":�������:����\'"<<name<<"\'������..."<<endl;
        analyzer->Error();
    }
}    

//ִ�г����������
void ZYDropTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"���ݿ�δ��..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    if(t==NULL)
    {
        cout<<"����\'"<<name<<"\'������..."<<endl;
        return;
    }

    db->DropTable(t);

    cout<<"����\'"<<name<<"\'�����ɹ�..."<<endl;
}

//���쳷����ͼ����
ZYDropView::ZYDropView(char *name,int lineno)
                            :ZYSyntaxTree(ESyntaxTreeType_DropView,name,lineno,false)
{
}
//��ӡ������ͼ����
void ZYDropView::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<������ͼ ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<"></������ͼ>"<<endl;
}

//�Գ�����ͼ��������������
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
        cout<<lineno<<":�������:��ͼ\'"<<name<<"\'������..."<<endl;
        analyzer->Error();
    }
}   

//ִ�г�����ͼ����
void ZYDropView::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"���ݿ�δ��..."<<endl;
        return;
    }

    t=db->GetTableI(name);

    db->DropTable(t);

    cout<<"��ͼ\'"<<name<<"\'�����ɹ�"<<endl;
}

//��������������
ZYColumnDecl::ZYColumnDecl(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_ColumnDecl,name,lineno,false)
{
    this->dataType=EColumnType_String;
    this->dataLength=16;
}

//������������
void ZYColumnDecl::SetDataType(int dataType)
{
    this->dataType=dataType;
}

//�������ݳ���
void ZYColumnDecl::SetDataLength(int dataLength)
{
    this->dataLength=dataLength;
}

//��ӡ����������
void ZYColumnDecl::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<������ ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<" ����="<<dataType;

    cout<<" ����="<<dataLength;

    cout<<">";

    cout<<"</������>"<<endl;
}

//����������������������
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
                    cout<<lineno<<":�������:��\'"<<name<<"\'�ڱ���\'"<<tableName<<"\'�в�����..."<<endl;
                    analyzer->Error();
                }
            }
        }
        return;
    }

    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
}    

//ִ������������
void ZYColumnDecl::Execute(ZYExecutor *executor)
{
    IZYTable *t;
    
    t=executor->GetCurrentTable();
    
    t->AppendColumn(name,(EColumnType)dataType,dataLength,0);
}

//����ı�������
ZYAlterTable::ZYAlterTable(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AlterTable,name,lineno,true)
{
    this->operation=ETokenType_Add;
    this->columnTokenStart=0;
    this->columnTokenEnd=0;
}

//���������ʷ����λ��
void ZYAlterTable::SetColumnTokenLoc(int columnTokenStart,int columnTokenEnd)
{
    this->columnTokenStart=columnTokenStart;
    this->columnTokenEnd=columnTokenEnd;
}

//���ò�������
void ZYAlterTable::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//��ȡ��������
ETokenType ZYAlterTable::GetOperation(void)
{
    return operation;
}    

//��ӡ�ı�������
void ZYAlterTable::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<�ı���� ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<" ����=\"";

    switch(operation)
    {
    case ETokenType_Add:
        cout<<"����/ADD";
        break;
    case ETokenType_Modify:
        cout<<"�޸�/MODIFY";
        break;
    case ETokenType_Drop:
        cout<<"����/DROP";
        break;
    }

    cout<<"\"";

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</�ı����>"<<endl;
}

//�Ըı�����������������
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

//ִ�иı�������
void ZYAlterTable::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t;

    ZYColumnRef *columnRef;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"���ݿ�δ��..."<<endl;

        return;
    }

    t=db->GetTableI(name);

    if(t==NULL)
    {
        cout<<"����\'"<<name<<"\'������"<<endl;

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
        cout<<"����ı�ɹ�����������..."<<endl;
    }
    else if(operation==ETokenType_Modify)
    {
        cout<<"����ı�ɹ��������޸�..."<<endl;
    }
    else if(operation==ETokenType_Drop)
    {
        cout<<"����ı�ɹ������ѳ���..."<<endl;
    }
}

//���촴����ͼ����
ZYCreateView::ZYCreateView(char *name,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_CreateView,name,lineno,true)
{
    this->selectStmt=NULL;
}

//����������ͼ����
ZYCreateView::~ZYCreateView(void)
{
    Release(selectStmt);
}

//������ͼѡ�����
void ZYCreateView::SetSelectStmt(ZYSelectStmt *selectStmt)
{
    this->selectStmt=selectStmt;
}

//��ӡ������ͼ����
void ZYCreateView::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<������ͼ ����=\"";

    PrintString(name);

    cout<<"\"";
    
    cout<<" �к�="<<lineno;

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintChild(selectStmt,indent);

    PrintIndent(indent);
    
    cout<<"</��������>"<<endl;
}

//�Դ�����ͼ��������������
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

//ִ�д�����ͼ������
void ZYCreateView::Execute(ZYExecutor *executor)
{
    selectStmt->SetTableName(name);
    selectStmt->Execute(executor);
}