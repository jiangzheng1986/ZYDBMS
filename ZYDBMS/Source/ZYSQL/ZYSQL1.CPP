//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-主程序 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//for PathFileExsits
#include <shlwapi.h>

//创建逐月查询编译器对象
IZYCompiler *IZYCompiler_Create(char *listingfile,IZYDatabase *database)
{
    return new ZYCompiler(listingfile,database);
}

//释放逐月查询编译器对象
void IZYCompiler_Release(IZYCompiler *compiler)
{
    delete (ZYCompiler *)compiler;
}

//构造逐月查询编译器对象
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

//析构逐月查询编译器对象
ZYCompiler::~ZYCompiler(void)
{
    cout.flush();
    if(listing!=NULL)
    {
        listing->close();
    }
}

//检查逐月结构化查询语言(ZYSQL)语句
void ZYCompiler::CheckSQL(char *sql,char *format)
{
    ZYToken token;
    
    cout<<"智能检查:"<<endl;

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

    //如果未检测到词法、语法及语义分析错误
    if( !source->IsError()&&
        !scanner->IsError()&&
        !parser->IsError()&&
        !analyzer->IsError()) 
    {
        cout<<"词法、语法及语义检查成功..."<<endl;
    }
    else
    {
        cout<<"检测到词法、语法或语义错误..."<<endl;
    }

    delete analyzer;

    delete syntaxTree;

    delete parser;

    delete scanner;

    delete source;
}

//执行逐月结构化查询语言(ZYSQL)语句
void ZYCompiler::ExecuteSQL(char *sql)
{
    if(sql==NULL)
    {
        cout<<"SQL语句字符串为空..."<<endl;
        return;
    }

    if(database==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    //执行词法和语法分析

    if(displaySource&&!displayMatch)
    {
        cout<<"源程序:"<<endl;
    }

    if(displaySource&&displayMatch)
    {
        cout<<"源程序与词法标记:"<<endl;
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

    //如果检测到词法或语法分析错误则停止分析
    if( source->IsError()||
        scanner->IsError()||
        parser->IsError()||
        analyzer->IsError())
    {
        cout<<"检测到词法、语法或语义错误,停止执行..."<<endl;
    }
    else
    {
        //依次执行每条查询

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

//获取查询执行结果表格
IZYTable *ZYCompiler::GetResultTable(void)
{
    return table;
}

//设置是否列表源程序
void ZYCompiler::SetDisplaySource(bool displaySource)
{
    this->displaySource=displaySource;
}

//设置是否显示匹配符号
void ZYCompiler::SetDisplayMatch(bool displayMatch)
{
    this->displayMatch=displayMatch;
}

//设置是否显示抽象语法树
void ZYCompiler::SetDisplayAST(bool displayAST)
{
    this->displayAST=displayAST;
}

//设置是否显示优化语法树
void ZYCompiler::SetDisplayOST(bool displayOST)
{
    this->displayOST=displayOST;
}

//设置是否显示数据表格
void ZYCompiler::SetDisplayTables(bool displayTables)
{
    this->displayTables=displayTables;
}

//设置是否显示查询耗时
void ZYCompiler::SetDisplayTime(bool displayTime)
{
    this->displayTime=displayTime;
}

//设置是否显示结果表格
void ZYCompiler::SetDisplayResult(bool displayResult)
{
    this->displayResult=displayResult;
}

//创建性能监视对象
IZYProfiler *IZYProfiler_Create(void)
{
    return (IZYProfiler *)new ZYProfiler();
}

//释放性能监视对象
void IZYProfiler_Release(IZYProfiler *profiler)
{
    delete (ZYProfiler *)profiler;
}

//测试文本文件文本存在性
bool IZYTextFile_Exists(char *filename)
{
    return PathFileExists(filename)==1;
}

//加载文本文件文本
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

//保存文本文件文本
void IZYTextFile_SaveText(char *filename,char *s)
{
    FILE *fp=fopen(filename,"w");
    
    fputs(s,fp);
    
    fclose(fp);
}

//释放文本文件文本
void IZYTextFile_FreeText(char *s)
{
    delete [] s;
}

//删除文本文件文本
void IZYTextFile_DeleteText(char *filename)
{
    DeleteFile(filename);
}

//构造性能监视对象
ZYProfiler::ZYProfiler()
{
    QueryPerformanceFrequency(&l1);

    l2.QuadPart=0;

    l3.QuadPart=0;

    time=0.0f;
}

//性能监视对象性能监视开始
void ZYProfiler::Start(void)
{
    QueryPerformanceCounter(&l2);
}

//性能监视对象性能监视结束
void ZYProfiler::End(void)
{
    QueryPerformanceCounter(&l3);

    time=(float)(l3.QuadPart-l2.QuadPart)/(float)(l1.QuadPart);
}

//获取性能监视对象监视到的执行时间(秒)
float ZYProfiler::GetTime(void)
{
    return time;
}

//获取性能监视对象监视到的执行时间(毫秒)
int ZYProfiler::GetTimeMs(void)
{
    return (int)(1000*time);
}

//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-词法分析
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//创建源代码对象
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

//获取一个字符
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

//回退一个字符
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

//获取当前索引
int ZYSource::GetSourceIndex(void)
{
    return sourceIndex;
}

//是否在执行语法检查
bool ZYSource::CheckingSQL(void)
{
    return this->formatString!=NULL;
}

//设置源程序字符类型
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

//设置不匹配标志
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

//设置已确认标志
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

//获取当前行数
int ZYSource::GetLineNumber(void)
{
    return lineNumber;
}

//获取当前列数
int ZYSource::GetColumnNumber(void)
{
    return columnNumber;
}

//设置是否显示源代码行
void ZYSource::SetDisplaySource(bool displaySource)
{
    this->displaySource=displaySource;
}

//获取是否发生错误
bool ZYSource::IsError(void)
{
    return error;
}

//显示一行源代码
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

//构造词法标记对象
ZYToken::ZYToken(void)
{
    this->tokenType=ETokenType_Unknown;

    this->tokenStringIndex=0;

    this->lineNumber=-1;

    this->columnNumber=-1;
}

//重置词法标记对象
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

//向词法标记字符串增加字符
void ZYToken::AddChar(char ch)
{
    if(tokenStringIndex<TOKEN_STRING_LEN-1)
    {
        tokenString[tokenStringIndex++]=ch;
    }
}

//获取词法标记对象词法标记类型
ETokenType ZYToken::GetTokenType(void)
{
    return tokenType;
}

//设置词法标记对象词法标记类型
void ZYToken::SetTokenType(ETokenType tokenType)
{
    this->tokenType=tokenType;
}

//获取词法标记对象词法标记字符串
char *ZYToken::GetTokenString(void)
{
    tokenString[tokenStringIndex]='\0';

    return tokenString;
}

//转换关键字词法标记对象
void ZYToken::TranslateToken(void)
{
    GetTokenString();

    _strupr(tokenString);

    if(strcmp(tokenString,"ADD")==0||strcmp(tokenString,"增加")==0)
    {
        tokenType=ETokenType_Add;
    }
    else if(strcmp(tokenString,"ALL")==0||strcmp(tokenString,"全部")==0)
    {
        tokenType=ETokenType_All;
    }
    else if(strcmp(tokenString,"ALTER")==0||strcmp(tokenString,"改变")==0)
    {
        tokenType=ETokenType_Alter;
    }
    else if(strcmp(tokenString,"AND")==0||strcmp(tokenString,"且")==0)
    {
        tokenType=ETokenType_And;
    }
    else if(strcmp(tokenString,"AS")==0||strcmp(tokenString,"为")==0)
    {
        tokenType=ETokenType_As;
    }
    else if(strcmp(tokenString,"ASC")==0||strcmp(tokenString,"升序")==0)
    {
        tokenType=ETokenType_Asc;
    }
    else if(strcmp(tokenString,"AVG")==0||strcmp(tokenString,"平均值")==0)
    {
        tokenType=ETokenType_Avg;
    }
    else if(strcmp(tokenString,"BY")==0||strcmp(tokenString,"按")==0)
    {
        tokenType=ETokenType_By;
    }
    else if(strcmp(tokenString,"CHAR")==0||strcmp(tokenString,"字符")==0)
    {
        tokenType=ETokenType_Char;
    }
    else if(strcmp(tokenString,"COUNT")==0||strcmp(tokenString,"计数值")==0)
    {
        tokenType=ETokenType_Count;
    }
    else if(strcmp(tokenString,"CREATE")==0||strcmp(tokenString,"创建")==0)
    {
        tokenType=ETokenType_Create;
    }
    else if(strcmp(tokenString,"DELETE")==0||strcmp(tokenString,"删除")==0)
    {
        tokenType=ETokenType_Delete;
    }
    else if(strcmp(tokenString,"DESC")==0||strcmp(tokenString,"降序")==0)
    {
        tokenType=ETokenType_Desc;
    }
    else if(strcmp(tokenString,"DISTINCT")==0||strcmp(tokenString,"互异")==0)
    {
        tokenType=ETokenType_Distinct;
    }
    else if(strcmp(tokenString,"DOUBLE")==0)
    {
        tokenType=ETokenType_Double;
    }
    else if(strcmp(tokenString,"小数")==0)
    {
        tokenType=ETokenType_DoublePrecision;
    }
    else if(strcmp(tokenString,"DROP")==0||strcmp(tokenString,"撤销")==0)
    {
        tokenType=ETokenType_Drop;
    }
    else if(strcmp(tokenString,"EXISTS")==0||strcmp(tokenString,"存在")==0)
    {
        tokenType=ETokenType_Exists;
    }
    else if(strcmp(tokenString,"FOREIGN")==0)
    {
        tokenType=ETokenType_Foreign;
    }
    else if(strcmp(tokenString,"外键")==0)
    {
        tokenType=ETokenType_ForeignKey;
    }
    else if(strcmp(tokenString,"FROM")==0||strcmp(tokenString,"从")==0)
    {
        tokenType=ETokenType_From;
    }
    else if(strcmp(tokenString,"GROUP")==0||strcmp(tokenString,"分组")==0)
    {
        tokenType=ETokenType_Group;
    }
    else if(strcmp(tokenString,"HAVING")==0||strcmp(tokenString,"筛选")==0)
    {
        tokenType=ETokenType_Having;
    }
    else if(strcmp(tokenString,"IN")==0||strcmp(tokenString,"含于")==0)
    {
        tokenType=ETokenType_In;
    }
    else if(strcmp(tokenString,"INSERT")==0||strcmp(tokenString,"插入")==0)
    {
        tokenType=ETokenType_Insert;
    }
    else if(strcmp(tokenString,"INTEGER")==0||strcmp(tokenString,"整数")==0)
    {
        tokenType=ETokenType_Integer;
    }
    else if(strcmp(tokenString,"INTO")==0||strcmp(tokenString,"到")==0)
    {
        tokenType=ETokenType_Into;
    }
    else if(strcmp(tokenString,"KEY")==0)
    {
        tokenType=ETokenType_Key;
    }
    else if(strcmp(tokenString,"MAX")==0||strcmp(tokenString,"最大值")==0)
    {
        tokenType=ETokenType_Max;
    }
    else if(strcmp(tokenString,"MIN")==0||strcmp(tokenString,"最小值")==0)
    {
        tokenType=ETokenType_Min;
    }
    else if(strcmp(tokenString,"MODIFY")==0||strcmp(tokenString,"修改")==0)
    {
        tokenType=ETokenType_Modify;
    }
    else if(strcmp(tokenString,"NOT")==0||strcmp(tokenString,"不")==0)
    {
        tokenType=ETokenType_Not;
    }
    else if(strcmp(tokenString,"OR")==0||strcmp(tokenString,"或")==0)
    {
        tokenType=ETokenType_Or;
    }
    else if(strcmp(tokenString,"ORDER")==0||strcmp(tokenString,"排序")==0)
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
    else if(strcmp(tokenString,"主键")==0)
    {
        tokenType=ETokenType_PrimaryKey;
    }
    else if(strcmp(tokenString,"REFERENCES")==0||strcmp(tokenString,"引用")==0)
    {
        tokenType=ETokenType_References;
    }
    else if(strcmp(tokenString,"SELECT")==0||strcmp(tokenString,"选择")==0)
    {
        tokenType=ETokenType_Select;
    }
    else if(strcmp(tokenString,"SET")==0||strcmp(tokenString,"使")==0)
    {
        tokenType=ETokenType_Set;
    }
    else if(strcmp(tokenString,"SUM")==0||strcmp(tokenString,"合计值")==0)
    {
        tokenType=ETokenType_Sum;
    }
    else if(strcmp(tokenString,"TABLE")==0||strcmp(tokenString,"表格")==0)
    {
        tokenType=ETokenType_Table;
    }
    else if(strcmp(tokenString,"UPDATE")==0||strcmp(tokenString,"更新")==0)
    {
        tokenType=ETokenType_Update;
    }
    else if(strcmp(tokenString,"VALUES")==0||strcmp(tokenString,"值表")==0)
    {
        tokenType=ETokenType_Values;
    }
    else if(strcmp(tokenString,"VIEW")==0||strcmp(tokenString,"视图")==0)
    {
        tokenType=ETokenType_View;
    }
    else if(strcmp(tokenString,"WHERE")==0||strcmp(tokenString,"当")==0)
    {
        tokenType=ETokenType_Where;
    }
}

//设置词法标记对象开始索引
void ZYToken::SetSourceIndex(int sourceIndex)
{
    this->sourceIndex=sourceIndex;
}

//获取词法标记对象开始索引
int ZYToken::GetSourceIndex(void)
{
    return this->sourceIndex;
}

//设置词法标记对象开始行号
void ZYToken::SetLineNumber(int lineNumber)
{
    this->lineNumber=lineNumber;
}

//获取词法标记对象开始行号
int ZYToken::GetLineNumber(void)
{
    return lineNumber;
}

//设置词法标记对象开始列号
void ZYToken::SetColumnNumber(int columnNumber)
{
    this->columnNumber=columnNumber;
}

//获取词法标记对象开始列号
int ZYToken::GetColumnNumber(void)
{
    return columnNumber;
}

//设置词法标记对象结束索引
void ZYToken::SetSourceIndex1(int sourceIndex1)
{
    this->sourceIndex1=sourceIndex1;
}

//获取词法标记对象结束索引
int ZYToken::GetSourceIndex1(void)
{
    return this->sourceIndex1;
}

//设置词法标记对象结束行号
void ZYToken::SetLineNumber1(int lineNumber1)
{
    this->lineNumber1=lineNumber1;
}

//获取词法标记对象结束行号
int ZYToken::GetLineNumber1(void)
{
    return lineNumber1;
}

//设置词法标记对象结束列号
void ZYToken::SetColumnNumber1(int columnNumber1)
{
    this->columnNumber1=columnNumber1;
}

//获取词法标记对象结束列号
int ZYToken::GetColumnNumber1(void)
{
    return columnNumber1;
}

//打印词法标记对象位置
void ZYToken::PrintLocation(void)
{
    cout<<"  "<<GetLineNumber()<<"-"<<GetColumnNumber()<<":";
}

//打印词法标记对象
void ZYToken::Print(void)
{
    GetTokenString();

    switch(tokenType)
    {
    case ETokenType_Unknown:
        cout<<"未知符号:"<<tokenString;
        break;
    case ETokenType_Add:
        cout<<"关键字:增加/ADD";
        break;
    case ETokenType_All:
        cout<<"关键字:全部/ALL";
        break;
    case ETokenType_Alter:
        cout<<"关键字:改变/ALTER";
        break;
    case ETokenType_And:
        cout<<"关键字:且/AND";
        break;
    case ETokenType_As:
        cout<<"关键字:为/AS";
        break;
    case ETokenType_Asc:
        cout<<"关键字:升序/ASC";
        break;
    case ETokenType_Avg:
        cout<<"关键字:平均值/AVG";
        break;
    case ETokenType_By:
        cout<<"关键字:按/BY";
        break;
    case ETokenType_Char:
        cout<<"关键字:字符/CHAR";
        break;
    case ETokenType_Count:
        cout<<"关键字:计数值/COUNT";
        break;
    case ETokenType_Create:
        cout<<"关键字:创建/CREATE";
        break;
    case ETokenType_Delete:
        cout<<"关键字:删除/DELETE";
        break;
    case ETokenType_Desc:
        cout<<"关键字:降序/DESC";
        break;
    case ETokenType_Distinct:
        cout<<"关键字:互异/DISTINCT";
        break;
    case ETokenType_Double:
        cout<<"关键字:DOUBLE";
        break;
    case ETokenType_DoublePrecision:
        cout<<"关键字:小数/DOUBLE_PRECISION";
        break;
    case ETokenType_Drop:
        cout<<"关键字:撤销/DROP";
        break;
    case ETokenType_Exists:
        cout<<"关键字:存在/EXISTS";
        break;
    case ETokenType_Foreign:
        cout<<"关键字:FOREIGN";
        break;
    case ETokenType_ForeignKey:
        cout<<"关键字:外键/FOREIGN_KEY";
        break;
    case ETokenType_From:
        cout<<"关键字:从/FROM";
        break;
    case ETokenType_Group:
        cout<<"关键字:分组/GROUP";
        break;
    case ETokenType_Having:
        cout<<"关键字:筛选/HAVING";
        break;
    case ETokenType_In:
        cout<<"关键字:含于/IN";
        break;
    case ETokenType_Insert:
        cout<<"关键字:插入/INSERT";
        break;
    case ETokenType_Integer:
        cout<<"关键字:整数/INTEGER";
        break;
    case ETokenType_Into:
        cout<<"关键字:到/INTO";
        break;
    case ETokenType_Key:
        cout<<"关键字:KEY";
        break;
    case ETokenType_Max:
        cout<<"关键字:最大值/MAX";
        break;
    case ETokenType_Min:
        cout<<"关键字:最小值/MIN";
        break;
    case ETokenType_Modify:
        cout<<"关键字:修改/MODIFY";
        break;
    case ETokenType_Not:
        cout<<"关键字:不/NOT";
        break;
    case ETokenType_Or:
        cout<<"关键字:或/OR";
        break;
    case ETokenType_Order:
        cout<<"关键字:排序/ORDER";
        break;
    case ETokenType_Precision:
        cout<<"关键字:PRECISION";
        break;
    case ETokenType_Primary:
        cout<<"关键字:PRIMARY";
        break;
    case ETokenType_PrimaryKey:
        cout<<"关键字:主键/PRIMARY_KEY";
        break;
    case ETokenType_References:
        cout<<"关键字:引用/REFERENCES";
        break;
    case ETokenType_Select:
        cout<<"关键字:选择/SELECT";
        break;
    case ETokenType_Set:
        cout<<"关键字:使/SET";
        break;
    case ETokenType_Sum:
        cout<<"关键字:合计值/SUM";
        break;
    case ETokenType_Table:
        cout<<"关键字:表格/TABLE";
        break;
    case ETokenType_Update:
        cout<<"关键字:更新/UPDATE";
        break;
    case ETokenType_Values:
        cout<<"关键字:值表/VALUES";
        break;
    case ETokenType_View:
        cout<<"关键字:视图/VIEW";
        break;
    case ETokenType_Where:
        cout<<"关键字:当/WHERE";
        break;
    case ETokenType_DouHao:
        cout<<"逗号:,";
        break;
    case ETokenType_JvDian:
        cout<<"句点:.";
        break;
    case ETokenType_FenHao:
        cout<<"分号:;";
        break;
    case ETokenType_EQ:
        cout<<"等于:=";
        break;
    case ETokenType_NE:
        cout<<"不等于:!=";
        break;
    case ETokenType_GT:
        cout<<"大于:>";
        break;
    case ETokenType_LT:
        cout<<"小于:<";
        break;
    case ETokenType_GE:
        cout<<"大于等于:>=";
        break;
    case ETokenType_LE:
        cout<<"小于等于:<=";
        break;
    case ETokenType_LeftParen:
        cout<<"左括号:(";
        break;
    case ETokenType_RightParen:
        cout<<"右括号:)";
        break;
    case ETokenType_Plus:
        cout<<"加号:+";
        break;
    case ETokenType_Minus:
        cout<<"减号:-";
        break;
    case ETokenType_Multiply:
        cout<<"乘号:*";
        break;
    case ETokenType_Divide:
        cout<<"除号:/";
        break;
    case ETokenType_Id:     
        cout<<"标识符:"<<tokenString;
        break;
    case ETokenType_Num:    
        cout<<"整数:"<<tokenString;
        break;
    case ETokenType_Float:    
        cout<<"小数:"<<tokenString;
        break;
    case ETokenType_String:
        cout<<"字符串:"<<tokenString;
        break;
    case ETokenType_Eof:
        cout<<"文件结束";
        break;
    }
}

//构造词法扫描器对象
ZYScanner::ZYScanner(ZYSource *source)
{
    this->source=source;

    this->error=false;
}

//获取当前行号
int ZYScanner::GetLineNumber(void)
{
    return source->GetLineNumber();
}

//获取是否发生错误
bool ZYScanner::IsError(void)
{
    return error;
}

//设置标记的开始位置
void ZYScanner::SetLocation(ZYToken *token)
{
    token->SetSourceIndex(source->GetSourceIndex());

    token->SetLineNumber(source->GetLineNumber());

    token->SetColumnNumber(source->GetColumnNumber());
}

//设置标记的结束位置
void ZYScanner::SetLocation1(ZYToken *token)
{
    token->SetSourceIndex1(source->GetSourceIndex());

    token->SetLineNumber1(source->GetLineNumber());

    token->SetColumnNumber1(source->GetColumnNumber()-1);
}

//设置源程序字符类型
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
        type=ESourceCharType_Keyword;         //关键字
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
        type=ESourceCharType_Punctuation;     //标点符号
        break;
    case ETokenType_Id:     
        type=ESourceCharType_Identifier;      //标识符
        break;
    case ETokenType_Num:    
        type=ESourceCharType_Num;             //整数
        break;
    case ETokenType_Float:    
        type=ESourceCharType_Float;           //小数
        break;
    case ETokenType_String:
        type=ESourceCharType_String;          //字符串
        break;
    case ETokenType_Eof:
        type=ESourceCharType_Eof;             //文件结束
        break;
    default:
        type=ESourceCharType_Unknown;         //未知类型
        break;
    }

    source->SetSourceCharType(token->GetSourceIndex(),token->GetSourceIndex1(),type);
}

//设置不匹配标志
void ZYScanner::SetDismatchFlag(ZYToken *token)
{
    source->SetDismatchFlag(token->GetSourceIndex(),token->GetSourceIndex1());
}

//获取下一个词法标记
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
            if(ch==' '||ch=='\t'||ch=='\x0D'||ch=='\n')     //空白字符
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
                case '-':                                   //注释
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
        token->TranslateToken();            //转换关键字
    }

    SetSourceCharType(token);
}
