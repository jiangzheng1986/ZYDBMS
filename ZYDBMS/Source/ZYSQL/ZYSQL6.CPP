//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-语法树表示(第3部分) 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//构造布尔表达式对象
ZYBoolExpression::ZYBoolExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_BoolExpression,"",lineno,false)
{
    this->operation=ETokenType_EQ;
    this->leftExpression=NULL;
    this->rightExpression=NULL;
}

//析构布尔表达式对象
ZYBoolExpression::~ZYBoolExpression(void)
{
    Release(leftExpression);
    Release(rightExpression);
}

//设置操作符
void ZYBoolExpression::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//设置左表达式
void ZYBoolExpression::SetLeftExpression(ZYSyntaxTree *leftExpression)
{
    this->leftExpression=leftExpression;
}

//设置右表达式
void ZYBoolExpression::SetRightExpression(ZYSyntaxTree *rightExpression)
{
    this->rightExpression=rightExpression;
}

//获取操作符
ETokenType ZYBoolExpression::GetOperation(void)
{
    return this->operation;
}

//获取左表达式
ZYSyntaxTree *ZYBoolExpression::GetLeftExpression(void)
{
    return this->leftExpression;
}

//获取右表达式
ZYSyntaxTree *ZYBoolExpression::GetRightExpression(void)
{
    return this->rightExpression;
}

//交换左边和右边的表达式
void ZYBoolExpression::SwapLeftAndRight(void)
{
    ZYSyntaxTree *temp=leftExpression;
    leftExpression=rightExpression;
    rightExpression=temp;
}

//打印布尔表达式对象
void ZYBoolExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<布尔表达式";

    cout<<" 行号="<<lineno;

    cout<<" 运算符=";
        
    PrintOperation(operation);

    cout<<">"<<endl;

    PrintChild(leftExpression,indent);

    PrintChild(rightExpression,indent);

    PrintIndent(indent);
    
    cout<<"</布尔表达式>"<<endl;
}

//对布尔表达式对象进行语义分析
void ZYBoolExpression::Analyze(ZYAnalyzer *analyzer)
{
    if(leftExpression!=NULL)
    {
        leftExpression->Analyze(analyzer);
    }

    if(rightExpression!=NULL)
    {
        rightExpression->Analyze(analyzer);
    }
}

//动态求布尔表达式对象值
EResultType ZYBoolExpression::GetResult(ZYExecutor *executor,void *pData)
{
    bool b=false;
    EResultType type1;
    EResultType type2;
    char buf1[1000];
    char buf2[1000];

    if(leftExpression!=NULL)
    {
        type1=leftExpression->GetResult(executor,buf1);
    }

    if(rightExpression!=NULL)
    {
        type2=rightExpression->GetResult(executor,buf2);
    }

    switch(operation)
    {
    case ETokenType_EQ:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)==(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)==(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)==0);
        }
        break;
    case ETokenType_NE:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)!=(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)!=(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)!=0);
        }
        break;
    case ETokenType_GT:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)>(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)>(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)>0);
        }
        break;
    case ETokenType_LT:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)<(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)<(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)<0);
        }
        break;
    case ETokenType_GE:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)>=(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)>=(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)>=0);
        }
        break;
    case ETokenType_LE:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            b=((*(int *)buf1)<=(*(int *)buf2));
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            b=((*(double *)buf1)<=(*(double *)buf2));
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            b=(strcmp((char *)buf1,(char *)buf2)<=0);
        }
        break;
    case ETokenType_In:
        if(type1==EResultType_String&&type2==EResultType_Table)
        {
            if((*(IZYTable **)buf2)!=NULL)
            {
                b=executor->StringInTable(buf1,(*(IZYTable **)buf2));
            }
        }
        break;
    case ETokenType_NotIn:
        if(type1==EResultType_String&&type2==EResultType_Table)
        {
            if((*(IZYTable **)buf2)!=NULL)
            {
                b=!executor->StringInTable(buf1,(*(IZYTable **)buf2));
            }
        }
        break;
    case ETokenType_Exists:
        if((*(IZYTable **)buf1)!=NULL)
        {
            b=((*(IZYTable **)buf1)->GetRowCount()!=0);
        }
        break;
    case ETokenType_And:
        b=((*(bool *)buf1)&&(*(bool *)buf2));
        break;
    case ETokenType_Or:
        b=((*(bool *)buf1)||(*(bool *)buf2));
        break;
    case ETokenType_Not:
        b=(!(*(bool *)buf1));
        break;
    }

    *(bool *)pData=b;

    return EResultType_Boolean;
}

//计算应下降到的表格索引
int ZYBoolExpression::CalcDescentTableIndex(ZYExecutor *executor)
{
    int l=leftExpression->CalcDescentTableIndex(executor);
    int r;
    if(rightExpression!=NULL)
    {
        r=rightExpression->CalcDescentTableIndex(executor);
    }
    else
    {
        r=0;
    }
    return l>r?l:r;
}

//构造赋值语句对象
ZYAssignExpression::ZYAssignExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AssignExpression,"",lineno,false)
{
    this->destColumn=NULL;
    this->srcExpression=NULL;
}

//析构赋值语句对象
ZYAssignExpression::~ZYAssignExpression(void)
{
    Release(destColumn);
    Release(srcExpression);
}

//设置赋值目标列
void ZYAssignExpression::SetDestColumn(ZYColumnRef *destColumn)
{
    this->destColumn=destColumn;
}

//设置赋值源表达式
void ZYAssignExpression::SetSrcExpression(ZYSyntaxTree *srcExpression)
{
    this->srcExpression=srcExpression;
}

//打印赋值语句对象
void ZYAssignExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<赋值表达式";

    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChild(destColumn,indent);

    PrintChild(srcExpression,indent);

    PrintIndent(indent);
    
    cout<<"</赋值表达式>"<<endl;
}

//对赋值语句对象进行语义分析
void ZYAssignExpression::Analyze(ZYAnalyzer *analyzer)
{
    if(destColumn!=NULL)
    {
        destColumn->Analyze(analyzer);
    }

    if(srcExpression!=NULL)
    {
        srcExpression->Analyze(analyzer);
    }
}

//执行赋值语句对象
void ZYAssignExpression::Execute(ZYExecutor *executor)
{
    IZYTable *t_dest;
    
    int i;

    char buf[1000];

    t_dest=executor->GetCurrentTable();

    i=executor->GetCurrentItem();//获取目标表项

    srcExpression->GetResult(executor,buf);

    destColumn->Execute1(executor);

    t_dest->SetData(i,destColumn->GetColumnIndex(),buf);
}

//构造聚合函数表达式对象
ZYAggregateFunction::ZYAggregateFunction(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AggregateFunction,"",lineno,false)
{
    this->function=ETokenType_Count;
    this->column=NULL;
    Reset();
}

//析构聚合函数表达式对象
ZYAggregateFunction::~ZYAggregateFunction(void)
{
    Release(column);
}

//设置聚合函数
void ZYAggregateFunction::SetFunction(ETokenType function)
{
    this->function=function;
}

//设置聚合列
void ZYAggregateFunction::SetColumn(ZYColumnRef *column)
{
    this->column=column;
}

//获取聚合函数
ETokenType ZYAggregateFunction::GetFunction(void)
{
    return this->function;
}

//获取聚合列
ZYColumnRef *ZYAggregateFunction::GetColumn(void)
{
    return this->column;
}

//重置聚合函数表达式对象
void ZYAggregateFunction::Reset(void)
{
    c=0;
    a=0;
    d=0.00;
    inited=false;
}

//打印聚合函数表达式对象
void ZYAggregateFunction::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<聚合函数";

    cout<<" 行号="<<lineno;

    cout<<" 函数名=";
        
    PrintOperation(function);

    cout<<">"<<endl;

    PrintChild(column,indent);

    PrintIndent(indent);
    
    cout<<"</聚合函数>"<<endl;
}

//对聚合函数表达式对象进行语义分析
void ZYAggregateFunction::Analyze(ZYAnalyzer *analyzer)
{
    if(column!=NULL)
    {
        column->Analyze(analyzer);
    }
}

//动态求聚合函数表达式对象值
EResultType ZYAggregateFunction::GetResult(ZYExecutor *executor,void *pData)
{
    EResultType type=EResultType_Integer;
    char buf1[1000];
    EResultType type1;

    type1=column->GetResult(executor,buf1);

    *(int *)pData=0;

    switch(function)
    {
    case ETokenType_Count:
        c++;
        *(int *)pData=c;
        type=EResultType_Integer;
        break;
    case ETokenType_Sum:
        if(type1==EResultType_Integer)
        {
            a+=(*(int *)buf1);
            *(int *)pData=a;
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Double)
        {
            d+=(*(double *)buf1);
            *(double *)pData=d;
            type=EResultType_Double;
        }
        break;
    case ETokenType_Avg:
        c++;
        if(type1==EResultType_Integer)
        {
            a+=(*(int *)buf1);
            *(double *)pData=(double)a/(double)c;
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double)
        {
            d+=(*(double *)buf1);
            *(double *)pData=d/(double)c;
            type=EResultType_Double;
        }
        break;
    case ETokenType_Max:
        if(type1==EResultType_Integer)
        {
            if(!inited||a<(*(int *)buf1))
            {
                a=(*(int *)buf1);
                inited=true;
            }
            *(int *)pData=a;
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Double)
        {
            if(!inited||d<(*(double *)buf1))
            {
                d=(*(double *)buf1);
                inited=true;
            }
            *(double *)pData=d;
            type=EResultType_Double;
        }
        break;
    case ETokenType_Min:
        if(type1==EResultType_Integer)
        {
            if(!inited||a>(*(int *)buf1))
            {
                a=(*(int *)buf1);
                inited=true;
            }
            *(int *)pData=a;
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Double)
        {
            if(!inited||d>(*(double *)buf1))
            {
                d=(*(double *)buf1);
                inited=true;
            }
            *(double *)pData=d;
            type=EResultType_Double;
        }
        break;
    }

    return type;
}

//构造表达式对象
ZYExpression::ZYExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_Expression,"",lineno,false)
{
    this->operation=ETokenType_Plus;
    this->leftExpression=NULL;
    this->rightExpression=NULL;
}

//析构表达式对象
ZYExpression::~ZYExpression(void)
{
    Release(leftExpression);
    Release(rightExpression);
}

//设置操作符
void ZYExpression::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//设置左表达式
void ZYExpression::SetLeftExpression(ZYSyntaxTree *leftExpression)
{
    this->leftExpression=leftExpression;
}

//设置右表达式
void ZYExpression::SetRightExpression(ZYSyntaxTree *rightExpression)
{
    this->rightExpression=rightExpression;
}

//打印表达式对象
void ZYExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<表达式";

    cout<<" 行号="<<lineno;

    cout<<" 运算符=";

    PrintOperation(operation);

    cout<<">"<<endl;

    PrintChild(leftExpression,indent);

    PrintChild(rightExpression,indent);

    PrintIndent(indent);
    
    cout<<"</表达式>"<<endl;
}

//对表达式对象进行语义分析
void ZYExpression::Analyze(ZYAnalyzer *analyzer)
{
    if(leftExpression!=NULL)
    {
        leftExpression->Analyze(analyzer);
    }

    if(rightExpression!=NULL)
    {
        rightExpression->Analyze(analyzer);
    }
}

//动态求表达式对象值
EResultType ZYExpression::GetResult(ZYExecutor *executor,void *pData)
{
    EResultType type=EResultType_Integer;
    EResultType type1;
    EResultType type2;
    char buf1[1000];
    char buf2[1000];

    if(leftExpression!=NULL)
    {
        type1=leftExpression->GetResult(executor,buf1);
    }

    if(rightExpression!=NULL)
    {
        type2=rightExpression->GetResult(executor,buf2);
    }

    switch(operation)
    {
    case ETokenType_Plus:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            *(int *)pData=((*(int *)buf1)+(*(int *)buf2));
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Integer&&type2==EResultType_Double)
        {
            *(double *)pData=((double)(*(int *)buf1)+(*(double *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Integer)
        {
            *(double *)pData=((*(double *)buf1)+(double)(*(int *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            *(double *)pData=((*(double *)buf1)+(*(double *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_String&&type2==EResultType_String)
        {
            char *buf=(char *)pData;
            buf[0]='\0';
            strcat(buf,buf1);
            strcat(buf+strlen(buf1),buf2);
            type=EResultType_String;
        }
        break;
    case ETokenType_Minus:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            *(int *)pData=((*(int *)buf1)-(*(int *)buf2));
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Integer&&type2==EResultType_Double)
        {
            *(double *)pData=((double)(*(int *)buf1)-(*(double *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Integer)
        {
            *(double *)pData=((*(double *)buf1)-(double)(*(int *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            *(double *)pData=((*(double *)buf1)-(*(double *)buf2));
            type=EResultType_Double;
        }
        break;
    case ETokenType_Multiply:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            *(int *)pData=((*(int *)buf1)*(*(int *)buf2));
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Integer&&type2==EResultType_Double)
        {
            *(double *)pData=((double)(*(int *)buf1)*(*(double *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Integer)
        {
            *(double *)pData=((*(double *)buf1)*(double)(*(int *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            *(double *)pData=((*(double *)buf1)*(*(double *)buf2));
            type=EResultType_Double;
        }
        break;
    case ETokenType_Divide:
        if(type1==EResultType_Integer&&type2==EResultType_Integer)
        {
            *(int *)pData=((*(int *)buf1)/(*(int *)buf2));
            type=EResultType_Integer;
        }
        else if(type1==EResultType_Integer&&type2==EResultType_Double)
        {
            *(double *)pData=((double)(*(int *)buf1)/(*(double *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Integer)
        {
            *(double *)pData=((*(double *)buf1)/(double)(*(int *)buf2));
            type=EResultType_Double;
        }
        else if(type1==EResultType_Double&&type2==EResultType_Double)
        {
            *(double *)pData=((*(double *)buf1)/(*(double *)buf2));
            type=EResultType_Double;
        }
        break;
    default:
        *(int *)pData=0;
        break;
    }

    return type;
}

//计算应下降到的表格索引
int ZYExpression::CalcDescentTableIndex(ZYExecutor *executor)
{
    int l=leftExpression->CalcDescentTableIndex(executor);
    int r=rightExpression->CalcDescentTableIndex(executor);
    return l>r?l:r;
}

//构造数据行对象
ZYRowValue::ZYRowValue(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_RowValue,"",lineno,true)
{
}

//打印数据行对象
void ZYRowValue::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<值行";

    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</值行>"<<endl;
}

//构造单个数据对象
ZYValue::ZYValue(int lineno,EColumnType dataType,void *pData)
                    :ZYSyntaxTree(ESyntaxTreeType_Value,"",lineno,false)
{
    this->dataType=dataType;

    switch(dataType)
    {
    case EColumnType_String:
        this->name=(char *)pData;
        break;
    case EColumnType_Double:
        this->d=*(double *)pData;
        break;
    case EColumnType_Integer:
        this->a=*(int *)pData;
        break;
    }
}

//获取数据对象数据值
void ZYValue::GetValue(void *pData)
{
    switch(dataType)
    {
    case EColumnType_String:
        strcpy((char *)pData,name);
        break;
    case EColumnType_Double:
        *(double *)pData=d;
        break;
    case EColumnType_Integer:
        *(int *)pData=a;
        break;
    }
}

//打印单个数据对象
void ZYValue::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<值";

    cout<<" 行号="<<lineno;

    cout<<" 类型="<<dataType;

    switch(dataType)
    {
    case EColumnType_String:
        cout<<" 字符串=\""<<name<<"\"";
        break;
    case EColumnType_Double:
        cout<<" 小数="<<d;
        break;
    case EColumnType_Integer:
        cout<<" 整数="<<a;
        break;
    }

    cout<<">";
    
    cout<<"</值>"<<endl;
}

//动态求单个数据对象值
EResultType ZYValue::GetResult(ZYExecutor *executor,void *pData)
{
    EResultType resultType=EResultType_Integer;

    switch(dataType)
    {
    case EColumnType_String:
        strcpy((char *)pData,name);
        resultType=EResultType_String;
        break;
    case EColumnType_Double:
        *(double *)pData=d;
        resultType=EResultType_Double;
        break;
    case EColumnType_Integer:
        *(int *)pData=a;
        resultType=EResultType_Integer;
        break;
    default:
        *(int *)pData=0;
        break;
    }

    return resultType;
}