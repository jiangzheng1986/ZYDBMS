//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-查询执行 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//构造查询分析器对象
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

//设置源程序对象
void ZYAnalyzer::SetSource(ZYSource *source)
{
    this->source=source;
}

//获取源程序对象
ZYSource *ZYAnalyzer::GetSource(void)
{
    return this->source;
}

//获取父查询分析器
ZYAnalyzer *ZYAnalyzer::GetParentAnalyzer(void)
{
    return parentAnalyzer;
}    

//设置父父查询分析器
void ZYAnalyzer::SetParentAnalyzer(ZYAnalyzer *parentAnalyzer)
{
    this->parentAnalyzer=parentAnalyzer;
}

//设置当前数据库
void ZYAnalyzer::SetCurrentDatabase(IZYDatabase *currentDatabase)
{
    this->currentDatabase=currentDatabase;
}

//获取当前数据库
IZYDatabase *ZYAnalyzer::GetCurrentDatabase(void)
{
    return this->currentDatabase;
}

//设置当前表格
void ZYAnalyzer::SetCurrentTable(IZYTable *currentTable)
{
    this->currentTable=currentTable;
}

//获取当前表格
IZYTable *ZYAnalyzer::GetCurrentTable(void)
{
    return this->currentTable;
}

//设置当前执行语句
void ZYAnalyzer::SetCurrentStmt(ZYSyntaxTree *currentStmt)
{
    this->currentStmt=currentStmt;
}

//获取当前执行语句
ZYSyntaxTree *ZYAnalyzer::GetCurrentStmt(void)
{
    return this->currentStmt;
}

//清空源表列表
void ZYAnalyzer::ClearSourceTables(void)
{
    sourceTableCount=0;
}

//加入源表列表表格
void ZYAnalyzer::AddSourceTable(ZYTableRef *tableRef)
{
    sourceTables[sourceTableCount++]=tableRef;
}

//获取源表列表表格
ZYTableRef *ZYAnalyzer::GetSourceTable(int index)
{
    return sourceTables[index];
}

//获取源表列表表格计数
int ZYAnalyzer::GetSourceTableCount(void)
{
    return sourceTableCount;
}

//获取是否发生错误
bool ZYAnalyzer::IsError(void)
{
    return error;
}

//发生语义分析错误
void ZYAnalyzer::Error(void)
{
    error=true;

    if(parentAnalyzer!=NULL)
    {
        parentAnalyzer->Error();
    }
}

//执行语义分析
void ZYAnalyzer::Analyze(ZYSyntaxTree *syntaxTree)
{
    syntaxTree->Analyze(this);
}

//临时表编号
int ZYExecutor::tempTableNo=0;

//临时列编号
int ZYExecutor::tempColumnNo=0;

//构造查询执行器对象
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

//析构查询执行器对象
ZYExecutor::~ZYExecutor(void)
{
    if(profiler!=NULL)
    {
        delete profiler;
    }
}

//设置当前选择查询语句
void ZYExecutor::SetCurrentSelectStmt(ZYSelectStmt *currentSelectStmt)
{
    this->currentSelectStmt=currentSelectStmt;
}

//获取当前选择查询语句
ZYSelectStmt *ZYExecutor::GetCurrentSelectStmt(void)
{
    return this->currentSelectStmt;
}

//获取父查询执行器
ZYExecutor *ZYExecutor::GetParentExecutor(void)
{
    return this->parentExecutor;
}

//设置当前数据库
void ZYExecutor::SetCurrentDatabase(IZYDatabase *currentDatabase)
{
    this->currentDatabase=currentDatabase;
}

//获取当前数据库
IZYDatabase *ZYExecutor::GetCurrentDatabase(void)
{
    return this->currentDatabase;
}

//设置临时表编号
void ZYExecutor::SetTempTableNo(int tempTableNo)
{
    this->tempTableNo=tempTableNo;
}

//分配一个临时表编号
int ZYExecutor::AllocateTempTableNo(void)
{
    return tempTableNo++;
}

//设置临时列编号
void ZYExecutor::SetTempColumnNo(int tempColumnNo)
{
    this->tempColumnNo=tempColumnNo;
}

//分配一个临时列编号
int ZYExecutor::AllocateTempColumnNo(void)
{
    return tempColumnNo++;
}

//设置当前表格
void ZYExecutor::SetCurrentTable(IZYTable *currentTable)
{
    this->currentTable=currentTable;
}

//获取当前表格
IZYTable *ZYExecutor::GetCurrentTable(void)
{
    return this->currentTable;
}

//设置当前主键
void ZYExecutor::SetPrimaryKey(char *primaryKey)
{
    this->primaryKey=primaryKey;
}

//获取当前主键
char *ZYExecutor::GetPrimaryKey(void)
{
    return this->primaryKey;
}

//清空源表列表
void ZYExecutor::ClearSourceTables(void)
{
    sourceTableCount=0;
}

//加入源表列表表格
void ZYExecutor::AddSourceTable(ZYTableRef *tableRef)
{
    sourceTables[sourceTableCount++]=tableRef;
}

//获取源表列表表格
ZYTableRef *ZYExecutor::GetSourceTable(int index)
{
    return sourceTables[index];
}

//获取源表列表表格计数
int ZYExecutor::GetSourceTableCount(void)
{
    return sourceTableCount;
}

//设置当前表项
void ZYExecutor::SetCurrentItem(int currentItem)
{
    this->currentItem=currentItem;
}

//获取当前表项
int ZYExecutor::GetCurrentItem(void)
{
    return this->currentItem;
}

//获取源表列表表格
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

//获取源表项列表表项
int ZYExecutor::GetSourceTableItem(int index)
{
    return sourceTables[index]->GetItemIndex();
}

//获取源表项列表表项
void ZYExecutor::SetSourceTableItem(int index,int sourceTableItem)
{
    sourceTables[index]->SetItemIndex(sourceTableItem);
}

//比较同一表格的两行数据是否相等
bool ZYExecutor::CompareTwoRow(IZYTable *t,int i1,int i2)
{
    int n,j,type;

    char buf1[1000];

    char buf2[1000];

    n=t->GetColumnCount();

    for(j=0;j<n;j++)
    {
        t->GetColumn(j,"类型",&type);

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

//测试表中的某行数据是否是不重复的
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

//判断一个字符串是否在一个表格中
bool ZYExecutor::StringInTable(char *s,IZYTable *t)
{
    int i;
    char buf[1000];
    int type=-1;

    if(t==NULL)
    {
        return false;
    }
    
    t->GetColumn(0,"类型",&type);

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

//获取性能监视器对象
ZYProfiler *ZYExecutor::GetProfiler(void)
{
    return this->profiler;
}

//设置是否显示抽象语法树
void ZYExecutor::SetDisplayAST(bool displayAST)
{
    this->displayAST=displayAST;
}

//设置是否显示优化语法树
void ZYExecutor::SetDisplayOST(bool displayOST)
{
    this->displayOST=displayOST;
}

//设置是否显示数据表格
void ZYExecutor::SetDisplayTables(bool displayTables)
{
    this->displayTables=displayTables;
}

//设置是否显示查询耗时
void ZYExecutor::SetDisplayTime(bool displayTime)
{
    this->displayTime=displayTime;
}

//设置是否显示结果表格
void ZYExecutor::SetDisplayResult(bool displayResult)
{
    this->displayResult=displayResult;
}

//获取是否显示抽象语法树
bool ZYExecutor::GetDisplayAST(void)
{
    return displayAST;
}

//获取是否显示优化语法树
bool ZYExecutor::GetDisplayOST(void)
{
    return displayOST;
}

//获取是否显示数据表格
bool ZYExecutor::GetDisplayTables(void)
{
    return displayTables;
}

//获取是否显示查询耗时
bool ZYExecutor::GetDisplayTime(void)
{
    return displayTime;
}

//获取是否显示执行结果
bool ZYExecutor::GetDisplayResult(void)
{
    return displayResult;
}

//获取是否发生错误
bool ZYExecutor::IsError(void)
{
    return error;
}

//发生查询执行错误
void ZYExecutor::Error(void)
{
    error=true;

    if(this->parentExecutor!=NULL)
    {
        parentExecutor->Error();
    }
}

//显示各执行器的状况
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

//显示特定的表格
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
        t->GetColumn(j,"名称",buf);

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
            t->GetColumn(j,"类型",&type);

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
                cout<<fixed;            //定点形式输出
                cout.precision(2);      //保留2个小数点
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

//执行查询,并显示查询时间和结果
void ZYExecutor::Execute(void)
{
    syntaxTree->Execute(this);
}