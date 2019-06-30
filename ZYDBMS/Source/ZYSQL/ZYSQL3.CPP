//---------------------------------------------------------
//���²�ѯ����(ZYSQL)-��ѯִ�� ����:����
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//�����ѯ����������
ZYAnalyzer::ZYAnalyzer(ZYAnalyzer *analyzer)
{
    if(analyzer==NULL)
    {
        this->source=NULL;
        this->currentDatabase=NULL;
        this->currentTable=NULL;
        this->error=false;
        this->currentStmt=NULL;
        this->sourceTableCount=0;
        this->parentAnalyzer=NULL;
    }
    else
    {
        this->source=analyzer->GetSource();
        this->currentDatabase=analyzer->GetCurrentDatabase();
        this->currentTable=NULL;
        this->error=analyzer->IsError();
        this->currentStmt=NULL;
        this->sourceTableCount=0;
        this->parentAnalyzer=analyzer;
    }
}

//����Դ�������
void ZYAnalyzer::SetSource(ZYSource *source)
{
    this->source=source;
}

//��ȡԴ�������
ZYSource *ZYAnalyzer::GetSource(void)
{
    return this->source;
}

//��ȡ����ѯ������
ZYAnalyzer *ZYAnalyzer::GetParentAnalyzer(void)
{
    return parentAnalyzer;
}    

//���ø�����ѯ������
void ZYAnalyzer::SetParentAnalyzer(ZYAnalyzer *parentAnalyzer)
{
    this->parentAnalyzer=parentAnalyzer;
}

//���õ�ǰ���ݿ�
void ZYAnalyzer::SetCurrentDatabase(IZYDatabase *currentDatabase)
{
    this->currentDatabase=currentDatabase;
}

//��ȡ��ǰ���ݿ�
IZYDatabase *ZYAnalyzer::GetCurrentDatabase(void)
{
    return this->currentDatabase;
}

//���õ�ǰ����
void ZYAnalyzer::SetCurrentTable(IZYTable *currentTable)
{
    this->currentTable=currentTable;
}

//��ȡ��ǰ����
IZYTable *ZYAnalyzer::GetCurrentTable(void)
{
    return this->currentTable;
}

//���õ�ǰִ�����
void ZYAnalyzer::SetCurrentStmt(ZYSyntaxTree *currentStmt)
{
    this->currentStmt=currentStmt;
}

//��ȡ��ǰִ�����
ZYSyntaxTree *ZYAnalyzer::GetCurrentStmt(void)
{
    return this->currentStmt;
}

//���Դ���б�
void ZYAnalyzer::ClearSourceTables(void)
{
    sourceTableCount=0;
}

//����Դ���б�����
void ZYAnalyzer::AddSourceTable(ZYTableRef *tableRef)
{
    sourceTables[sourceTableCount++]=tableRef;
}

//��ȡԴ���б�����
ZYTableRef *ZYAnalyzer::GetSourceTable(int index)
{
    return sourceTables[index];
}

//��ȡԴ���б��������
int ZYAnalyzer::GetSourceTableCount(void)
{
    return sourceTableCount;
}

//��ȡ�Ƿ�������
bool ZYAnalyzer::IsError(void)
{
    return error;
}

//���������������
void ZYAnalyzer::Error(void)
{
    error=true;

    if(parentAnalyzer!=NULL)
    {
        parentAnalyzer->Error();
    }
}

//ִ���������
void ZYAnalyzer::Analyze(ZYSyntaxTree *syntaxTree)
{
    syntaxTree->Analyze(this);
}

//��ʱ�����
int ZYExecutor::tempTableNo=0;

//��ʱ�б��
int ZYExecutor::tempColumnNo=0;

//�����ѯִ��������
ZYExecutor::ZYExecutor(ZYExecutor *parentExecutor,IZYDatabase *database,ZYSyntaxTree *syntaxTree)
{
    this->syntaxTree=syntaxTree;
    this->currentDatabase=database;
    this->currentItem=0;
    this->currentTable=NULL;
    this->primaryKey="";
    this->profiler=new ZYProfiler();
    this->sourceTableCount=0;
    this->parentExecutor=parentExecutor;
    this->error=false;
    this->currentSelectStmt=NULL;
    if(parentExecutor!=NULL)
    {
        this->displayAST=parentExecutor->displayAST;
        this->displayOST=parentExecutor->displayOST;
        this->displayTables=parentExecutor->displayTables;
        this->displayTime=parentExecutor->displayTime;
        this->displayResult=parentExecutor->displayResult;
    }
    else
    {
        this->displayAST=false;
        this->displayOST=false;
        this->displayTables=false;
        this->displayTime=false;
        this->displayResult=false;
    }
}

//������ѯִ��������
ZYExecutor::~ZYExecutor(void)
{
    if(profiler!=NULL)
    {
        delete profiler;
    }
}

//���õ�ǰѡ���ѯ���
void ZYExecutor::SetCurrentSelectStmt(ZYSelectStmt *currentSelectStmt)
{
    this->currentSelectStmt=currentSelectStmt;
}

//��ȡ��ǰѡ���ѯ���
ZYSelectStmt *ZYExecutor::GetCurrentSelectStmt(void)
{
    return this->currentSelectStmt;
}

//��ȡ����ѯִ����
ZYExecutor *ZYExecutor::GetParentExecutor(void)
{
    return this->parentExecutor;
}

//���õ�ǰ���ݿ�
void ZYExecutor::SetCurrentDatabase(IZYDatabase *currentDatabase)
{
    this->currentDatabase=currentDatabase;
}

//��ȡ��ǰ���ݿ�
IZYDatabase *ZYExecutor::GetCurrentDatabase(void)
{
    return this->currentDatabase;
}

//������ʱ�����
void ZYExecutor::SetTempTableNo(int tempTableNo)
{
    this->tempTableNo=tempTableNo;
}

//����һ����ʱ�����
int ZYExecutor::AllocateTempTableNo(void)
{
    return tempTableNo++;
}

//������ʱ�б��
void ZYExecutor::SetTempColumnNo(int tempColumnNo)
{
    this->tempColumnNo=tempColumnNo;
}

//����һ����ʱ�б��
int ZYExecutor::AllocateTempColumnNo(void)
{
    return tempColumnNo++;
}

//���õ�ǰ����
void ZYExecutor::SetCurrentTable(IZYTable *currentTable)
{
    this->currentTable=currentTable;
}

//��ȡ��ǰ����
IZYTable *ZYExecutor::GetCurrentTable(void)
{
    return this->currentTable;
}

//���õ�ǰ����
void ZYExecutor::SetPrimaryKey(char *primaryKey)
{
    this->primaryKey=primaryKey;
}

//��ȡ��ǰ����
char *ZYExecutor::GetPrimaryKey(void)
{
    return this->primaryKey;
}

//���Դ���б�
void ZYExecutor::ClearSourceTables(void)
{
    sourceTableCount=0;
}

//����Դ���б�����
void ZYExecutor::AddSourceTable(ZYTableRef *tableRef)
{
    sourceTables[sourceTableCount++]=tableRef;
}

//��ȡԴ���б�����
ZYTableRef *ZYExecutor::GetSourceTable(int index)
{
    return sourceTables[index];
}

//��ȡԴ���б��������
int ZYExecutor::GetSourceTableCount(void)
{
    return sourceTableCount;
}

//���õ�ǰ����
void ZYExecutor::SetCurrentItem(int currentItem)
{
    this->currentItem=currentItem;
}

//��ȡ��ǰ����
int ZYExecutor::GetCurrentItem(void)
{
    return this->currentItem;
}

//��ȡԴ���б�����
IZYTable *ZYExecutor::GetSourceTableIndex(int index)
{
    if(index<sourceTableCount)
    {
        return sourceTables[index]->GetTableIndex();
    }
    else
    {
        return NULL;
    }
}

//��ȡԴ�����б�����
int ZYExecutor::GetSourceTableItem(int index)
{
    return sourceTables[index]->GetItemIndex();
}

//��ȡԴ�����б�����
void ZYExecutor::SetSourceTableItem(int index,int sourceTableItem)
{
    sourceTables[index]->SetItemIndex(sourceTableItem);
}

//�Ƚ�ͬһ��������������Ƿ����
bool ZYExecutor::CompareTwoRow(IZYTable *t,int i1,int i2)
{
    int n,j,type;

    char buf1[1000];

    char buf2[1000];

    n=t->GetColumnCount();

    for(j=0;j<n;j++)
    {
        t->GetColumn(j,"����",&type);

        t->GetData(i1,j,buf1);

        t->GetData(i2,j,buf2);

        switch((EColumnType)type)
        {
        case EColumnType_String:
            if(strcmp(buf1,buf2)!=0)
            {
                return false;
            }
            break;
        case EColumnType_Double:
            if((*(double *)buf1)!=(*(double *)buf2))
            {
                return false;
            }
            break;
        case EColumnType_Integer:
            if((*(int *)buf1)!=(*(int *)buf2))
            {
                return false;
            }
            break;
        }
    }

    return true;
}

//���Ա��е�ĳ�������Ƿ��ǲ��ظ���
bool ZYExecutor::IsDistinct(IZYTable *t,int i)
{
    int n=0;

    int i1;

    for(i1=0;i1<t->GetRowCount();i1++)
    {
        if(CompareTwoRow(t,i1,i))
        {
            n++;
        }
    }

    return (n==1);
}

//�ж�һ���ַ����Ƿ���һ��������
bool ZYExecutor::StringInTable(char *s,IZYTable *t)
{
    int i;
    char buf[1000];
    int type=-1;

    if(t==NULL)
    {
        return false;
    }
    
    t->GetColumn(0,"����",&type);

    if(type!=(int)EColumnType_String)
    {
        return false;
    }

    for(i=0;i<t->GetRowCount();i++)
    {
        t->GetData(i,0,buf);

        if(strcmp(s,buf)==0)
        {
            return true;
        }
    }

    return false;
}

//��ȡ���ܼ���������
ZYProfiler *ZYExecutor::GetProfiler(void)
{
    return this->profiler;
}

//�����Ƿ���ʾ�����﷨��
void ZYExecutor::SetDisplayAST(bool displayAST)
{
    this->displayAST=displayAST;
}

//�����Ƿ���ʾ�Ż��﷨��
void ZYExecutor::SetDisplayOST(bool displayOST)
{
    this->displayOST=displayOST;
}

//�����Ƿ���ʾ���ݱ���
void ZYExecutor::SetDisplayTables(bool displayTables)
{
    this->displayTables=displayTables;
}

//�����Ƿ���ʾ��ѯ��ʱ
void ZYExecutor::SetDisplayTime(bool displayTime)
{
    this->displayTime=displayTime;
}

//�����Ƿ���ʾ�������
void ZYExecutor::SetDisplayResult(bool displayResult)
{
    this->displayResult=displayResult;
}

//��ȡ�Ƿ���ʾ�����﷨��
bool ZYExecutor::GetDisplayAST(void)
{
    return displayAST;
}

//��ȡ�Ƿ���ʾ�Ż��﷨��
bool ZYExecutor::GetDisplayOST(void)
{
    return displayOST;
}

//��ȡ�Ƿ���ʾ���ݱ���
bool ZYExecutor::GetDisplayTables(void)
{
    return displayTables;
}

//��ȡ�Ƿ���ʾ��ѯ��ʱ
bool ZYExecutor::GetDisplayTime(void)
{
    return displayTime;
}

//��ȡ�Ƿ���ʾִ�н��
bool ZYExecutor::GetDisplayResult(void)
{
    return displayResult;
}

//��ȡ�Ƿ�������
bool ZYExecutor::IsError(void)
{
    return error;
}

//������ѯִ�д���
void ZYExecutor::Error(void)
{
    error=true;

    if(this->parentExecutor!=NULL)
    {
        parentExecutor->Error();
    }
}

//��ʾ��ִ������״��
void ZYExecutor::Print(void)
{
    int i;

    IZYTable *t;

    char tname[100];

    if(parentExecutor!=NULL)
    {
        parentExecutor->Print();
    }

    if(currentSelectStmt!=NULL)
    {
        cout<<currentSelectStmt->GetTableName()<<" ";
    }

    for(i=0;i<GetSourceTableCount();i++)
    {
        t=sourceTables[i]->GetTableIndex();

        t->GetName(tname);
        if(i!=GetSourceTableCount()-1)
        {
            cout<<tname<<",";
        }
        else
        {
            cout<<tname;
        }
    }

    cout<<endl;
}

//��ʾ�ض��ı���
void ZYExecutor::DisplayTable(IZYTable *t,bool displayData)
{
    char buf[1000];

    int i,j,type,a;

    double d;

    buf[0]='\0';

    if(t==NULL)
    {
        return;
    }

    t->GetName(buf);

    cout<<"{";

    cout<<buf;
    
    cout<<"}"<<endl;

    cout<<"[";

    for(j=0;j<t->GetColumnCount();j++)
    {
        t->GetColumn(j,"����",buf);

        cout.width(12);

        cout<<left;

        cout<<buf;

        if(j!=t->GetColumnCount()-1)
        {
            cout<<" ";
        }
    }

    cout<<"]"<<endl;

    if(displayData==false)
    {
        return;
    }

    for(i=0;i<t->GetRowCount()&&i<100;i++)
    {
        cout<<"(";

        for(j=0;j<t->GetColumnCount();j++)
        {
            t->GetColumn(j,"����",&type);

            cout.width(12);

            cout<<left;

            switch(type)
            {
            case EColumnType_String:
                t->GetData(i,j,buf);
                cout<<buf;
                break;
            case EColumnType_Double:
                t->GetData(i,j,&d);
                cout<<fixed;            //������ʽ���
                cout.precision(2);      //����2��С����
                cout<<d;
                break;
            case EColumnType_Integer:
                t->GetData(i,j,&a);
                cout<<a;
                break;
            }

            if(j!=t->GetColumnCount()-1)
            {
                cout<<" ";
            }
        }

        cout<<")"<<endl;
    }   

    if(i<t->GetRowCount())
    {
        cout<<"..."<<endl;
    }
}

//ִ�в�ѯ,����ʾ��ѯʱ��ͽ��
void ZYExecutor::Execute(void)
{
    syntaxTree->Execute(this);
}