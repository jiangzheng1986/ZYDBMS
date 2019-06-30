//---------------------------------------------------------
//���²�ѯ����(ZYSQL)-������ ����:����
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//for PathFileExsits
#include <shlwapi.h>

//�������²�ѯ����������
IZYCompiler *IZYCompiler_Create(char *listingfile,IZYDatabase *database)
{
    return new ZYCompiler(listingfile,database);
}

//�ͷ����²�ѯ����������
void IZYCompiler_Release(IZYCompiler *compiler)
{
    delete (ZYCompiler *)compiler;
}

//�������²�ѯ����������
ZYCompiler::ZYCompiler(char *listingfile,IZYDatabase *database)
{
    this->database=database;
    this->table=NULL;
    this->displaySource=false;
    this->displayMatch=false;
    this->displayAST=false;
    this->displayOST=false;
    this->displayTables=false;
    this->displayTime=false;
    this->displayResult=false;
    this->listing=NULL;
    if(listingfile!=NULL)
    {
        listing=new ofstream(listingfile);
        cout=*listing;
    }
}

//�������²�ѯ����������
ZYCompiler::~ZYCompiler(void)
{
    cout.flush();
    if(listing!=NULL)
    {
        listing->close();
    }
}

//������½ṹ����ѯ����(ZYSQL)���
void ZYCompiler::CheckSQL(char *sql,char *format)
{
    ZYToken token;
    
    cout<<"���ܼ��:"<<endl;

    ZYSource *source=new ZYSource(sql,format);

    ZYScanner *scanner=new ZYScanner(source);

    ZYParser *parser=new ZYParser(scanner);

    ZYSyntaxTree *syntaxTree=parser->Parse();

    ZYAnalyzer *analyzer=new ZYAnalyzer(NULL);

    analyzer->SetSource(source);

    analyzer->SetCurrentDatabase(database);

    if(database!=NULL)
    {
        analyzer->Analyze(syntaxTree);
    }

    //���δ��⵽�ʷ����﷨�������������
    if( !source->IsError()&&
        !scanner->IsError()&&
        !parser->IsError()&&
        !analyzer->IsError()) 
    {
        cout<<"�ʷ����﷨��������ɹ�..."<<endl;
    }
    else
    {
        cout<<"��⵽�ʷ����﷨���������..."<<endl;
    }

    delete analyzer;

    delete syntaxTree;

    delete parser;

    delete scanner;

    delete source;
}

//ִ�����½ṹ����ѯ����(ZYSQL)���
void ZYCompiler::ExecuteSQL(char *sql)
{
    if(sql==NULL)
    {
        cout<<"SQL����ַ���Ϊ��..."<<endl;
        return;
    }

    if(database==NULL)
    {
        cout<<"���ݿ�δ��..."<<endl;
        return;
    }

    //ִ�дʷ����﷨����

    if(displaySource&&!displayMatch)
    {
        cout<<"Դ����:"<<endl;
    }

    if(displaySource&&displayMatch)
    {
        cout<<"Դ������ʷ����:"<<endl;
    }

    ZYSource *source=new ZYSource(sql,NULL);

    source->SetDisplaySource(displaySource);

    ZYScanner *scanner=new ZYScanner(source);

    ZYParser *parser=new ZYParser(scanner);

    parser->SetDisplayMatch(displayMatch);

    ZYSyntaxTree *syntaxTree=parser->Parse();

    ZYAnalyzer *analyzer=new ZYAnalyzer(NULL);

    analyzer->SetSource(source);

    analyzer->SetCurrentDatabase(database);

    analyzer->Analyze(syntaxTree);

    //�����⵽�ʷ����﷨����������ֹͣ����
    if( source->IsError()||
        scanner->IsError()||
        parser->IsError()||
        analyzer->IsError())
    {
        cout<<"��⵽�ʷ����﷨���������,ִֹͣ��..."<<endl;
    }
    else
    {
        //����ִ��ÿ����ѯ

        ZYExecutor *executor=new ZYExecutor(NULL,database,syntaxTree);

        executor->SetDisplayAST(displayAST);

        executor->SetDisplayOST(displayOST);

        executor->SetDisplayTables(displayTables);

        executor->SetDisplayTime(displayTime);

        executor->SetDisplayResult(displayResult);
        
        executor->Execute();

        table=executor->GetCurrentTable();

        delete executor;
    }       

    delete analyzer;

    delete syntaxTree;

    delete parser;

    delete scanner;

    delete source;
}

//��ȡ��ѯִ�н������
IZYTable *ZYCompiler::GetResultTable(void)
{
    return table;
}

//�����Ƿ��б�Դ����
void ZYCompiler::SetDisplaySource(bool displaySource)
{
    this->displaySource=displaySource;
}

//�����Ƿ���ʾƥ�����
void ZYCompiler::SetDisplayMatch(bool displayMatch)
{
    this->displayMatch=displayMatch;
}

//�����Ƿ���ʾ�����﷨��
void ZYCompiler::SetDisplayAST(bool displayAST)
{
    this->displayAST=displayAST;
}

//�����Ƿ���ʾ�Ż��﷨��
void ZYCompiler::SetDisplayOST(bool displayOST)
{
    this->displayOST=displayOST;
}

//�����Ƿ���ʾ���ݱ���
void ZYCompiler::SetDisplayTables(bool displayTables)
{
    this->displayTables=displayTables;
}

//�����Ƿ���ʾ��ѯ��ʱ
void ZYCompiler::SetDisplayTime(bool displayTime)
{
    this->displayTime=displayTime;
}

//�����Ƿ���ʾ�������
void ZYCompiler::SetDisplayResult(bool displayResult)
{
    this->displayResult=displayResult;
}

//�������ܼ��Ӷ���
IZYProfiler *IZYProfiler_Create(void)
{
    return (IZYProfiler *)new ZYProfiler();
}

//�ͷ����ܼ��Ӷ���
void IZYProfiler_Release(IZYProfiler *profiler)
{
    delete (ZYProfiler *)profiler;
}

//�����ı��ļ��ı�������
bool IZYTextFile_Exists(char *filename)
{
    return PathFileExists(filename)==1;
}

//�����ı��ļ��ı�
char *IZYTextFile_LoadText(char *filename)
{
    int i;
    
    int n;

    char *s;

    char buf[1001];

    FILE *fp=fopen(filename,"r");

    if(fp==NULL)
    {
        return NULL;
    }

    fseek(fp,0,SEEK_END);

    n=ftell(fp);

    s=new char[n+10];

    s[0]='\0';

    i=0;

    fseek(fp,0,SEEK_SET);

    while(fp!=NULL&&!feof(fp))
    {
        buf[0]='\0';

        fgets(buf,1000,fp);

        n=strlen(buf);

        strcat(s+i,buf);

        i+=n;
    }

    fclose(fp);

    return s;
}

//�����ı��ļ��ı�
void IZYTextFile_SaveText(char *filename,char *s)
{
    FILE *fp=fopen(filename,"w");
    
    fputs(s,fp);
    
    fclose(fp);
}

//�ͷ��ı��ļ��ı�
void IZYTextFile_FreeText(char *s)
{
    delete [] s;
}

//ɾ���ı��ļ��ı�
void IZYTextFile_DeleteText(char *filename)
{
    DeleteFile(filename);
}

//�������ܼ��Ӷ���
ZYProfiler::ZYProfiler()
{
    QueryPerformanceFrequency(&l1);

    l2.QuadPart=0;

    l3.QuadPart=0;

    time=0.0f;
}

//���ܼ��Ӷ������ܼ��ӿ�ʼ
void ZYProfiler::Start(void)
{
    QueryPerformanceCounter(&l2);
}

//���ܼ��Ӷ������ܼ��ӽ���
void ZYProfiler::End(void)
{
    QueryPerformanceCounter(&l3);

    time=(float)(l3.QuadPart-l2.QuadPart)/(float)(l1.QuadPart);
}

//��ȡ���ܼ��Ӷ�����ӵ���ִ��ʱ��(��)
float ZYProfiler::GetTime(void)
{
    return time;
}

//��ȡ���ܼ��Ӷ�����ӵ���ִ��ʱ��(����)
int ZYProfiler::GetTimeMs(void)
{
    return (int)(1000*time);
}

//---------------------------------------------------------
//���²�ѯ����(ZYSQL)-�ʷ�����
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//����Դ�������
ZYSource::ZYSource(char *sourceString,char *formatString)
{
    this->sourceString=sourceString;

    this->formatString=formatString;

    SetSourceCharType(0,strlen(sourceString)+1,ESourceCharType_Comment);

    this->sourceIndex=0;

    this->lineNumber=1;

    this->columnNumber=1;

    this->displaySource=false;

    this->ungetedALine=false;

    if(sourceString==NULL)
    {
        this->error=true;
    }
    else
    {
        this->error=false;
    }
}

//��ȡһ���ַ�
char ZYSource::GetChar(void)
{
    char ch;
    if( displaySource&&
        columnNumber==1)
    {
        if(ungetedALine==false)
        {
            DisplayLine();
        }
        else
        {
            ungetedALine=false;
        }
    }
    ch=sourceString[sourceIndex];
    sourceIndex++;
    columnNumber++;
    if(ch=='\n')
    {
        lineNumber++;
        columnNumber=1;
    }
    return ch;
}

//����һ���ַ�
void ZYSource::UngetChar(void)
{
    char ch;
    ch=sourceString[--sourceIndex];
    columnNumber--;
    if(ch=='\n')
    {
        lineNumber--;
        columnNumber=1;
        ungetedALine=true;
    }
}

//��ȡ��ǰ����
int ZYSource::GetSourceIndex(void)
{
    return sourceIndex;
}

//�Ƿ���ִ���﷨���
bool ZYSource::CheckingSQL(void)
{
    return this->formatString!=NULL;
}

//����Դ�����ַ�����
void ZYSource::SetSourceCharType(int i1,int i2,ESourceCharType type)
{
    if(formatString!=NULL)
    {
        while(i1<i2)
        {
            formatString[i1++]=type;
        }
    }
}

//���ò�ƥ���־
void ZYSource::SetDismatchFlag(int i1,int i2)
{
    if(formatString!=NULL)
    {
        while(i1<i2)
        {
            formatString[i1++]|=DISMATCH_FLAG;
        }
    }
}

//������ȷ�ϱ�־
void ZYSource::SetAcknowledgedFlag(int i1,int i2)
{
    if(formatString!=NULL)
    {
        while(i1<i2)
        {
            formatString[i1++]|=ACKNOWLEDGED_FLAG;
        }
    }
}

//��ȡ��ǰ����
int ZYSource::GetLineNumber(void)
{
    return lineNumber;
}

//��ȡ��ǰ����
int ZYSource::GetColumnNumber(void)
{
    return columnNumber;
}

//�����Ƿ���ʾԴ������
void ZYSource::SetDisplaySource(bool displaySource)
{
    this->displaySource=displaySource;
}

//��ȡ�Ƿ�������
bool ZYSource::IsError(void)
{
    return error;
}

//��ʾһ��Դ����
void ZYSource::DisplayLine(void)
{
    char ch;
    int i=sourceIndex;
    cout<<lineNumber<<":";
    while(1)
    {
        ch=sourceString[i++];
        if(ch=='\n'||ch=='\0')
        {
            break;
        }
        cout<<ch;
    }
    cout<<endl;
}

//����ʷ���Ƕ���
ZYToken::ZYToken(void)
{
    this->tokenType=ETokenType_Unknown;

    this->tokenStringIndex=0;

    this->lineNumber=-1;

    this->columnNumber=-1;
}

//���ôʷ���Ƕ���
void ZYToken::Reset(void)
{
    this->tokenType=ETokenType_Unknown;

    this->tokenStringIndex=0;

    this->sourceIndex=-1;

    this->lineNumber=-1;

    this->columnNumber=-1;

    this->sourceIndex1=-1;

    this->lineNumber1=-1;

    this->columnNumber1=-1;
}

//��ʷ�����ַ��������ַ�
void ZYToken::AddChar(char ch)
{
    if(tokenStringIndex<TOKEN_STRING_LEN-1)
    {
        tokenString[tokenStringIndex++]=ch;
    }
}

//��ȡ�ʷ���Ƕ���ʷ��������
ETokenType ZYToken::GetTokenType(void)
{
    return tokenType;
}

//���ôʷ���Ƕ���ʷ��������
void ZYToken::SetTokenType(ETokenType tokenType)
{
    this->tokenType=tokenType;
}

//��ȡ�ʷ���Ƕ���ʷ�����ַ���
char *ZYToken::GetTokenString(void)
{
    tokenString[tokenStringIndex]='\0';

    return tokenString;
}

//ת���ؼ��ִʷ���Ƕ���
void ZYToken::TranslateToken(void)
{
    GetTokenString();

    _strupr(tokenString);

    if(strcmp(tokenString,"ADD")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Add;
    }
    else if(strcmp(tokenString,"ALL")==0||strcmp(tokenString,"ȫ��")==0)
    {
        tokenType=ETokenType_All;
    }
    else if(strcmp(tokenString,"ALTER")==0||strcmp(tokenString,"�ı�")==0)
    {
        tokenType=ETokenType_Alter;
    }
    else if(strcmp(tokenString,"AND")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_And;
    }
    else if(strcmp(tokenString,"AS")==0||strcmp(tokenString,"Ϊ")==0)
    {
        tokenType=ETokenType_As;
    }
    else if(strcmp(tokenString,"ASC")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Asc;
    }
    else if(strcmp(tokenString,"AVG")==0||strcmp(tokenString,"ƽ��ֵ")==0)
    {
        tokenType=ETokenType_Avg;
    }
    else if(strcmp(tokenString,"BY")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_By;
    }
    else if(strcmp(tokenString,"CHAR")==0||strcmp(tokenString,"�ַ�")==0)
    {
        tokenType=ETokenType_Char;
    }
    else if(strcmp(tokenString,"COUNT")==0||strcmp(tokenString,"����ֵ")==0)
    {
        tokenType=ETokenType_Count;
    }
    else if(strcmp(tokenString,"CREATE")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Create;
    }
    else if(strcmp(tokenString,"DELETE")==0||strcmp(tokenString,"ɾ��")==0)
    {
        tokenType=ETokenType_Delete;
    }
    else if(strcmp(tokenString,"DESC")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Desc;
    }
    else if(strcmp(tokenString,"DISTINCT")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Distinct;
    }
    else if(strcmp(tokenString,"DOUBLE")==0)
    {
        tokenType=ETokenType_Double;
    }
    else if(strcmp(tokenString,"С��")==0)
    {
        tokenType=ETokenType_DoublePrecision;
    }
    else if(strcmp(tokenString,"DROP")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Drop;
    }
    else if(strcmp(tokenString,"EXISTS")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Exists;
    }
    else if(strcmp(tokenString,"FOREIGN")==0)
    {
        tokenType=ETokenType_Foreign;
    }
    else if(strcmp(tokenString,"���")==0)
    {
        tokenType=ETokenType_ForeignKey;
    }
    else if(strcmp(tokenString,"FROM")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_From;
    }
    else if(strcmp(tokenString,"GROUP")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Group;
    }
    else if(strcmp(tokenString,"HAVING")==0||strcmp(tokenString,"ɸѡ")==0)
    {
        tokenType=ETokenType_Having;
    }
    else if(strcmp(tokenString,"IN")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_In;
    }
    else if(strcmp(tokenString,"INSERT")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Insert;
    }
    else if(strcmp(tokenString,"INTEGER")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Integer;
    }
    else if(strcmp(tokenString,"INTO")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_Into;
    }
    else if(strcmp(tokenString,"KEY")==0)
    {
        tokenType=ETokenType_Key;
    }
    else if(strcmp(tokenString,"MAX")==0||strcmp(tokenString,"���ֵ")==0)
    {
        tokenType=ETokenType_Max;
    }
    else if(strcmp(tokenString,"MIN")==0||strcmp(tokenString,"��Сֵ")==0)
    {
        tokenType=ETokenType_Min;
    }
    else if(strcmp(tokenString,"MODIFY")==0||strcmp(tokenString,"�޸�")==0)
    {
        tokenType=ETokenType_Modify;
    }
    else if(strcmp(tokenString,"NOT")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_Not;
    }
    else if(strcmp(tokenString,"OR")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_Or;
    }
    else if(strcmp(tokenString,"ORDER")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Order;
    }
    else if(strcmp(tokenString,"PRECISION")==0)
    {
        tokenType=ETokenType_Precision;
    }
    else if(strcmp(tokenString,"PRIMARY")==0)
    {
        tokenType=ETokenType_Primary;
    }
    else if(strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_PrimaryKey;
    }
    else if(strcmp(tokenString,"REFERENCES")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_References;
    }
    else if(strcmp(tokenString,"SELECT")==0||strcmp(tokenString,"ѡ��")==0)
    {
        tokenType=ETokenType_Select;
    }
    else if(strcmp(tokenString,"SET")==0||strcmp(tokenString,"ʹ")==0)
    {
        tokenType=ETokenType_Set;
    }
    else if(strcmp(tokenString,"SUM")==0||strcmp(tokenString,"�ϼ�ֵ")==0)
    {
        tokenType=ETokenType_Sum;
    }
    else if(strcmp(tokenString,"TABLE")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Table;
    }
    else if(strcmp(tokenString,"UPDATE")==0||strcmp(tokenString,"����")==0)
    {
        tokenType=ETokenType_Update;
    }
    else if(strcmp(tokenString,"VALUES")==0||strcmp(tokenString,"ֵ��")==0)
    {
        tokenType=ETokenType_Values;
    }
    else if(strcmp(tokenString,"VIEW")==0||strcmp(tokenString,"��ͼ")==0)
    {
        tokenType=ETokenType_View;
    }
    else if(strcmp(tokenString,"WHERE")==0||strcmp(tokenString,"��")==0)
    {
        tokenType=ETokenType_Where;
    }
}

//���ôʷ���Ƕ���ʼ����
void ZYToken::SetSourceIndex(int sourceIndex)
{
    this->sourceIndex=sourceIndex;
}

//��ȡ�ʷ���Ƕ���ʼ����
int ZYToken::GetSourceIndex(void)
{
    return this->sourceIndex;
}

//���ôʷ���Ƕ���ʼ�к�
void ZYToken::SetLineNumber(int lineNumber)
{
    this->lineNumber=lineNumber;
}

//��ȡ�ʷ���Ƕ���ʼ�к�
int ZYToken::GetLineNumber(void)
{
    return lineNumber;
}

//���ôʷ���Ƕ���ʼ�к�
void ZYToken::SetColumnNumber(int columnNumber)
{
    this->columnNumber=columnNumber;
}

//��ȡ�ʷ���Ƕ���ʼ�к�
int ZYToken::GetColumnNumber(void)
{
    return columnNumber;
}

//���ôʷ���Ƕ����������
void ZYToken::SetSourceIndex1(int sourceIndex1)
{
    this->sourceIndex1=sourceIndex1;
}

//��ȡ�ʷ���Ƕ����������
int ZYToken::GetSourceIndex1(void)
{
    return this->sourceIndex1;
}

//���ôʷ���Ƕ�������к�
void ZYToken::SetLineNumber1(int lineNumber1)
{
    this->lineNumber1=lineNumber1;
}

//��ȡ�ʷ���Ƕ�������к�
int ZYToken::GetLineNumber1(void)
{
    return lineNumber1;
}

//���ôʷ���Ƕ�������к�
void ZYToken::SetColumnNumber1(int columnNumber1)
{
    this->columnNumber1=columnNumber1;
}

//��ȡ�ʷ���Ƕ�������к�
int ZYToken::GetColumnNumber1(void)
{
    return columnNumber1;
}

//��ӡ�ʷ���Ƕ���λ��
void ZYToken::PrintLocation(void)
{
    cout<<"  "<<GetLineNumber()<<"-"<<GetColumnNumber()<<":";
}

//��ӡ�ʷ���Ƕ���
void ZYToken::Print(void)
{
    GetTokenString();

    switch(tokenType)
    {
    case ETokenType_Unknown:
        cout<<"δ֪����:"<<tokenString;
        break;
    case ETokenType_Add:
        cout<<"�ؼ���:����/ADD";
        break;
    case ETokenType_All:
        cout<<"�ؼ���:ȫ��/ALL";
        break;
    case ETokenType_Alter:
        cout<<"�ؼ���:�ı�/ALTER";
        break;
    case ETokenType_And:
        cout<<"�ؼ���:��/AND";
        break;
    case ETokenType_As:
        cout<<"�ؼ���:Ϊ/AS";
        break;
    case ETokenType_Asc:
        cout<<"�ؼ���:����/ASC";
        break;
    case ETokenType_Avg:
        cout<<"�ؼ���:ƽ��ֵ/AVG";
        break;
    case ETokenType_By:
        cout<<"�ؼ���:��/BY";
        break;
    case ETokenType_Char:
        cout<<"�ؼ���:�ַ�/CHAR";
        break;
    case ETokenType_Count:
        cout<<"�ؼ���:����ֵ/COUNT";
        break;
    case ETokenType_Create:
        cout<<"�ؼ���:����/CREATE";
        break;
    case ETokenType_Delete:
        cout<<"�ؼ���:ɾ��/DELETE";
        break;
    case ETokenType_Desc:
        cout<<"�ؼ���:����/DESC";
        break;
    case ETokenType_Distinct:
        cout<<"�ؼ���:����/DISTINCT";
        break;
    case ETokenType_Double:
        cout<<"�ؼ���:DOUBLE";
        break;
    case ETokenType_DoublePrecision:
        cout<<"�ؼ���:С��/DOUBLE_PRECISION";
        break;
    case ETokenType_Drop:
        cout<<"�ؼ���:����/DROP";
        break;
    case ETokenType_Exists:
        cout<<"�ؼ���:����/EXISTS";
        break;
    case ETokenType_Foreign:
        cout<<"�ؼ���:FOREIGN";
        break;
    case ETokenType_ForeignKey:
        cout<<"�ؼ���:���/FOREIGN_KEY";
        break;
    case ETokenType_From:
        cout<<"�ؼ���:��/FROM";
        break;
    case ETokenType_Group:
        cout<<"�ؼ���:����/GROUP";
        break;
    case ETokenType_Having:
        cout<<"�ؼ���:ɸѡ/HAVING";
        break;
    case ETokenType_In:
        cout<<"�ؼ���:����/IN";
        break;
    case ETokenType_Insert:
        cout<<"�ؼ���:����/INSERT";
        break;
    case ETokenType_Integer:
        cout<<"�ؼ���:����/INTEGER";
        break;
    case ETokenType_Into:
        cout<<"�ؼ���:��/INTO";
        break;
    case ETokenType_Key:
        cout<<"�ؼ���:KEY";
        break;
    case ETokenType_Max:
        cout<<"�ؼ���:���ֵ/MAX";
        break;
    case ETokenType_Min:
        cout<<"�ؼ���:��Сֵ/MIN";
        break;
    case ETokenType_Modify:
        cout<<"�ؼ���:�޸�/MODIFY";
        break;
    case ETokenType_Not:
        cout<<"�ؼ���:��/NOT";
        break;
    case ETokenType_Or:
        cout<<"�ؼ���:��/OR";
        break;
    case ETokenType_Order:
        cout<<"�ؼ���:����/ORDER";
        break;
    case ETokenType_Precision:
        cout<<"�ؼ���:PRECISION";
        break;
    case ETokenType_Primary:
        cout<<"�ؼ���:PRIMARY";
        break;
    case ETokenType_PrimaryKey:
        cout<<"�ؼ���:����/PRIMARY_KEY";
        break;
    case ETokenType_References:
        cout<<"�ؼ���:����/REFERENCES";
        break;
    case ETokenType_Select:
        cout<<"�ؼ���:ѡ��/SELECT";
        break;
    case ETokenType_Set:
        cout<<"�ؼ���:ʹ/SET";
        break;
    case ETokenType_Sum:
        cout<<"�ؼ���:�ϼ�ֵ/SUM";
        break;
    case ETokenType_Table:
        cout<<"�ؼ���:����/TABLE";
        break;
    case ETokenType_Update:
        cout<<"�ؼ���:����/UPDATE";
        break;
    case ETokenType_Values:
        cout<<"�ؼ���:ֵ��/VALUES";
        break;
    case ETokenType_View:
        cout<<"�ؼ���:��ͼ/VIEW";
        break;
    case ETokenType_Where:
        cout<<"�ؼ���:��/WHERE";
        break;
    case ETokenType_DouHao:
        cout<<"����:,";
        break;
    case ETokenType_JvDian:
        cout<<"���:.";
        break;
    case ETokenType_FenHao:
        cout<<"�ֺ�:;";
        break;
    case ETokenType_EQ:
        cout<<"����:=";
        break;
    case ETokenType_NE:
        cout<<"������:!=";
        break;
    case ETokenType_GT:
        cout<<"����:>";
        break;
    case ETokenType_LT:
        cout<<"С��:<";
        break;
    case ETokenType_GE:
        cout<<"���ڵ���:>=";
        break;
    case ETokenType_LE:
        cout<<"С�ڵ���:<=";
        break;
    case ETokenType_LeftParen:
        cout<<"������:(";
        break;
    case ETokenType_RightParen:
        cout<<"������:)";
        break;
    case ETokenType_Plus:
        cout<<"�Ӻ�:+";
        break;
    case ETokenType_Minus:
        cout<<"����:-";
        break;
    case ETokenType_Multiply:
        cout<<"�˺�:*";
        break;
    case ETokenType_Divide:
        cout<<"����:/";
        break;
    case ETokenType_Id:     
        cout<<"��ʶ��:"<<tokenString;
        break;
    case ETokenType_Num:    
        cout<<"����:"<<tokenString;
        break;
    case ETokenType_Float:    
        cout<<"С��:"<<tokenString;
        break;
    case ETokenType_String:
        cout<<"�ַ���:"<<tokenString;
        break;
    case ETokenType_Eof:
        cout<<"�ļ�����";
        break;
    }
}

//����ʷ�ɨ��������
ZYScanner::ZYScanner(ZYSource *source)
{
    this->source=source;

    this->error=false;
}

//��ȡ��ǰ�к�
int ZYScanner::GetLineNumber(void)
{
    return source->GetLineNumber();
}

//��ȡ�Ƿ�������
bool ZYScanner::IsError(void)
{
    return error;
}

//���ñ�ǵĿ�ʼλ��
void ZYScanner::SetLocation(ZYToken *token)
{
    token->SetSourceIndex(source->GetSourceIndex());

    token->SetLineNumber(source->GetLineNumber());

    token->SetColumnNumber(source->GetColumnNumber());
}

//���ñ�ǵĽ���λ��
void ZYScanner::SetLocation1(ZYToken *token)
{
    token->SetSourceIndex1(source->GetSourceIndex());

    token->SetLineNumber1(source->GetLineNumber());

    token->SetColumnNumber1(source->GetColumnNumber()-1);
}

//����Դ�����ַ�����
void ZYScanner::SetSourceCharType(ZYToken *token)
{
    ESourceCharType type;

    if(source->CheckingSQL()==false)
    {
        return;
    }

    switch(token->GetTokenType())
    {
    case ETokenType_Add:
    case ETokenType_All:
    case ETokenType_Alter:
    case ETokenType_And:
    case ETokenType_As:
    case ETokenType_Asc:
    case ETokenType_Avg:
    case ETokenType_By:
    case ETokenType_Char:
    case ETokenType_Count:
    case ETokenType_Create:
    case ETokenType_Delete:
    case ETokenType_Desc:
    case ETokenType_Distinct:
    case ETokenType_Double:
    case ETokenType_DoublePrecision:
    case ETokenType_Drop:
    case ETokenType_Exists:
    case ETokenType_Foreign:
    case ETokenType_ForeignKey:
    case ETokenType_From:
    case ETokenType_Group:
    case ETokenType_Having:
    case ETokenType_In:
    case ETokenType_Insert:
    case ETokenType_Integer:
    case ETokenType_Into:
    case ETokenType_Key:
    case ETokenType_Max:
    case ETokenType_Min:
    case ETokenType_Modify:
    case ETokenType_Not:
    case ETokenType_Or:
    case ETokenType_Order:
    case ETokenType_Precision:
    case ETokenType_Primary:
    case ETokenType_PrimaryKey:
    case ETokenType_References:
    case ETokenType_Select:
    case ETokenType_Set:
    case ETokenType_Sum:
    case ETokenType_Table:
    case ETokenType_Update:
    case ETokenType_Values:
    case ETokenType_View:
    case ETokenType_Where:
        type=ESourceCharType_Keyword;         //�ؼ���
        break;
    case ETokenType_DouHao:
    case ETokenType_JvDian:
    case ETokenType_FenHao:
    case ETokenType_EQ:
    case ETokenType_NE:
    case ETokenType_GT:
    case ETokenType_LT:
    case ETokenType_GE:
    case ETokenType_LE:
    case ETokenType_LeftParen:
    case ETokenType_RightParen:
    case ETokenType_Plus:
    case ETokenType_Minus:
    case ETokenType_Multiply:
    case ETokenType_Divide:
        type=ESourceCharType_Punctuation;     //������
        break;
    case ETokenType_Id:     
        type=ESourceCharType_Identifier;      //��ʶ��
        break;
    case ETokenType_Num:    
        type=ESourceCharType_Num;             //����
        break;
    case ETokenType_Float:    
        type=ESourceCharType_Float;           //С��
        break;
    case ETokenType_String:
        type=ESourceCharType_String;          //�ַ���
        break;
    case ETokenType_Eof:
        type=ESourceCharType_Eof;             //�ļ�����
        break;
    default:
        type=ESourceCharType_Unknown;         //δ֪����
        break;
    }

    source->SetSourceCharType(token->GetSourceIndex(),token->GetSourceIndex1(),type);
}

//���ò�ƥ���־
void ZYScanner::SetDismatchFlag(ZYToken *token)
{
    source->SetDismatchFlag(token->GetSourceIndex(),token->GetSourceIndex1());
}

//��ȡ��һ���ʷ����
void ZYScanner::GetToken(ZYToken *token)
{
    EStateType state=EStateType_Start;

    char ch='\0';

    bool loop=true;

    token->Reset();

    SetLocation(token);

    while(state!=EStateType_Done)
    {
        ch=source->GetChar();

        switch(state)
        {
        case EStateType_Start:
            if(ch==' '||ch=='\t'||ch=='\x0D'||ch=='\n')     //�հ��ַ�
            {
                SetLocation(token);

                state=EStateType_Start;
            }
            else if(isalpha(ch)||ch<0||ch=='^'||ch=='_')//ID
            {
                token->AddChar(ch);
                token->SetTokenType(ETokenType_Id);
                state=EStateType_InId;
            }
            else if(isdigit(ch))                            //NUM,FLOAT
            {
                token->AddChar(ch);
                token->SetTokenType(ETokenType_Num);
                state=EStateType_InNum;
            }
            else
            {
                switch(ch)
                {
                case '\'':                                  //STRING
                    token->SetTokenType(ETokenType_String);
                    state=EStateType_InString;
                    break;
                case ',':
                    token->SetTokenType(ETokenType_DouHao);
                    state=EStateType_Done;
                    break;
                case '.':
                    token->SetTokenType(ETokenType_JvDian);
                    state=EStateType_Done;
                    break;
                case ';':
                    token->SetTokenType(ETokenType_FenHao);
                    state=EStateType_Done;
                    break;
                case '(':
                    token->SetTokenType(ETokenType_LeftParen);
                    state=EStateType_Done;
                    break;
                case ')':
                    token->SetTokenType(ETokenType_RightParen);
                    state=EStateType_Done;
                    break;
                case '+':
                    token->SetTokenType(ETokenType_Plus);
                    state=EStateType_Done;
                    break;
                case '-':                                   //ע��
                    state=EStateType_InComment;
                    break;
                case '*':
                    token->SetTokenType(ETokenType_Multiply);
                    state=EStateType_Done;
                    break;
                case '/':
                    token->SetTokenType(ETokenType_Divide);
                    state=EStateType_Done;
                    break;
                case '=':
                    token->SetTokenType(ETokenType_EQ);
                    state=EStateType_Done;
                    break;
                case '!':                                   //NE
                    token->AddChar(ch);
                    state=EStateType_InNE;
                    break;
                case '>':                                   //GT,GE
                    state=EStateType_InGE;
                    break;
                case '<':                                   //LT,LE
                    state=EStateType_InLE;
                    break;
                case '\0':                                  //EOF
                    token->SetTokenType(ETokenType_Eof);
                    state=EStateType_Done;
                    break;
                default:
                    token->AddChar(ch);
                    token->SetTokenType(ETokenType_Unknown);
                    state=EStateType_Done;
                    break;
                }
            }
            break;
        case EStateType_InId:
            if(isalpha(ch)||ch<0||isdigit(ch)||ch=='^'||ch=='_'||ch=='#')
            {
                token->AddChar(ch);
            }
            else
            {
                source->UngetChar();
                state=EStateType_Done;
            }
            break;
        case EStateType_InNum:
            if(ch=='.')
            {
                token->AddChar(ch);
                token->SetTokenType(ETokenType_Float);
            }
            else if(isdigit(ch))
            {
                token->AddChar(ch);
            }
            else
            {
                source->UngetChar();
                state=EStateType_Done;
            }
            break;
        case EStateType_InString:
            if(ch=='\'')
            {
                state=EStateType_Done;
            }
            else if(ch=='\0')
            {
                source->UngetChar();

                state=EStateType_Done;
            }
            else
            {
                token->AddChar(ch);
            }
            break;
        case EStateType_InNE:
            if(ch=='=')
            {
                token->SetTokenType(ETokenType_NE);
                state=EStateType_Done;
            }
            else
            {
                source->UngetChar();
                token->SetTokenType(ETokenType_Unknown);
                state=EStateType_Done;
            }
            break;
        case EStateType_InGE:
            if(ch=='=')
            {
                token->SetTokenType(ETokenType_GE);
                state=EStateType_Done;
            }
            else
            {
                source->UngetChar();
                token->SetTokenType(ETokenType_GT);
                state=EStateType_Done;
            }
            break;
        case EStateType_InLE:
            if(ch=='=')
            {
                token->SetTokenType(ETokenType_LE);
                state=EStateType_Done;
            }
            else
            {
                source->UngetChar();
                token->SetTokenType(ETokenType_LT);
                state=EStateType_Done;
            }
            break;
        case EStateType_InComment:
            if(ch=='-')
            {
                state=EStateType_InComment1;
            }
            else
            {
                source->UngetChar();
                token->SetTokenType(ETokenType_Minus);
                state=EStateType_Done;
            }
            break;
        case EStateType_InComment1:
            if(ch=='\n')
            {
                SetLocation(token);

                state=EStateType_Start;
            }
            else if(ch=='\0')
            {
                SetLocation(token);

                source->UngetChar();
                
                state=EStateType_Start;
            }
            break;
        }
    }

    SetLocation1(token);

    if(token->GetTokenType()==ETokenType_Id)
    {
        token->TranslateToken();            //ת���ؼ���
    }

    SetSourceCharType(token);
}