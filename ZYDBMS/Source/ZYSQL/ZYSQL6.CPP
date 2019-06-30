//---------------------------------------------------------
//���²�ѯ����(ZYSQL)-�﷨����ʾ(��3����) ����:����
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//���첼������ʽ����
ZYBoolExpression::ZYBoolExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_BoolExpression,"",lineno,false)
{
    this->operation=ETokenType_EQ;
    this->leftExpression=NULL;
    this->rightExpression=NULL;
}

//������������ʽ����
ZYBoolExpression::~ZYBoolExpression(void)
{
    Release(leftExpression);
    Release(rightExpression);
}

//���ò�����
void ZYBoolExpression::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//���������ʽ
void ZYBoolExpression::SetLeftExpression(ZYSyntaxTree *leftExpression)
{
    this->leftExpression=leftExpression;
}

//�����ұ���ʽ
void ZYBoolExpression::SetRightExpression(ZYSyntaxTree *rightExpression)
{
    this->rightExpression=rightExpression;
}

//��ȡ������
ETokenType ZYBoolExpression::GetOperation(void)
{
    return this->operation;
}

//��ȡ�����ʽ
ZYSyntaxTree *ZYBoolExpression::GetLeftExpression(void)
{
    return this->leftExpression;
}

//��ȡ�ұ���ʽ
ZYSyntaxTree *ZYBoolExpression::GetRightExpression(void)
{
    return this->rightExpression;
}

//������ߺ��ұߵı���ʽ
void ZYBoolExpression::SwapLeftAndRight(void)
{
    ZYSyntaxTree *temp=leftExpression;
    leftExpression=rightExpression;
    rightExpression=temp;
}

//��ӡ��������ʽ����
void ZYBoolExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<��������ʽ";

    cout<<" �к�="<<lineno;

    cout<<" �����=";
        
    PrintOperation(operation);

    cout<<">"<<endl;

    PrintChild(leftExpression,indent);

    PrintChild(rightExpression,indent);

    PrintIndent(indent);
    
    cout<<"</��������ʽ>"<<endl;
}

//�Բ�������ʽ��������������
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

//��̬�󲼶�����ʽ����ֵ
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

//����Ӧ�½����ı�������
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

//���츳ֵ������
ZYAssignExpression::ZYAssignExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AssignExpression,"",lineno,false)
{
    this->destColumn=NULL;
    this->srcExpression=NULL;
}

//������ֵ������
ZYAssignExpression::~ZYAssignExpression(void)
{
    Release(destColumn);
    Release(srcExpression);
}

//���ø�ֵĿ����
void ZYAssignExpression::SetDestColumn(ZYColumnRef *destColumn)
{
    this->destColumn=destColumn;
}

//���ø�ֵԴ����ʽ
void ZYAssignExpression::SetSrcExpression(ZYSyntaxTree *srcExpression)
{
    this->srcExpression=srcExpression;
}

//��ӡ��ֵ������
void ZYAssignExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<��ֵ����ʽ";

    cout<<" �к�="<<lineno;

    cout<<">"<<endl;

    PrintChild(destColumn,indent);

    PrintChild(srcExpression,indent);

    PrintIndent(indent);
    
    cout<<"</��ֵ����ʽ>"<<endl;
}

//�Ը�ֵ����������������
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

//ִ�и�ֵ������
void ZYAssignExpression::Execute(ZYExecutor *executor)
{
    IZYTable *t_dest;
    
    int i;

    char buf[1000];

    t_dest=executor->GetCurrentTable();

    i=executor->GetCurrentItem();//��ȡĿ�����

    srcExpression->GetResult(executor,buf);

    destColumn->Execute1(executor);

    t_dest->SetData(i,destColumn->GetColumnIndex(),buf);
}

//����ۺϺ�������ʽ����
ZYAggregateFunction::ZYAggregateFunction(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_AggregateFunction,"",lineno,false)
{
    this->function=ETokenType_Count;
    this->column=NULL;
    Reset();
}

//�����ۺϺ�������ʽ����
ZYAggregateFunction::~ZYAggregateFunction(void)
{
    Release(column);
}

//���þۺϺ���
void ZYAggregateFunction::SetFunction(ETokenType function)
{
    this->function=function;
}

//���þۺ���
void ZYAggregateFunction::SetColumn(ZYColumnRef *column)
{
    this->column=column;
}

//��ȡ�ۺϺ���
ETokenType ZYAggregateFunction::GetFunction(void)
{
    return this->function;
}

//��ȡ�ۺ���
ZYColumnRef *ZYAggregateFunction::GetColumn(void)
{
    return this->column;
}

//���þۺϺ�������ʽ����
void ZYAggregateFunction::Reset(void)
{
    c=0;
    a=0;
    d=0.00;
    inited=false;
}

//��ӡ�ۺϺ�������ʽ����
void ZYAggregateFunction::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<�ۺϺ���";

    cout<<" �к�="<<lineno;

    cout<<" ������=";
        
    PrintOperation(function);

    cout<<">"<<endl;

    PrintChild(column,indent);

    PrintIndent(indent);
    
    cout<<"</�ۺϺ���>"<<endl;
}

//�ԾۺϺ�������ʽ��������������
void ZYAggregateFunction::Analyze(ZYAnalyzer *analyzer)
{
    if(column!=NULL)
    {
        column->Analyze(analyzer);
    }
}

//��̬��ۺϺ�������ʽ����ֵ
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

//�������ʽ����
ZYExpression::ZYExpression(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_Expression,"",lineno,false)
{
    this->operation=ETokenType_Plus;
    this->leftExpression=NULL;
    this->rightExpression=NULL;
}

//��������ʽ����
ZYExpression::~ZYExpression(void)
{
    Release(leftExpression);
    Release(rightExpression);
}

//���ò�����
void ZYExpression::SetOperation(ETokenType operation)
{
    this->operation=operation;
}

//���������ʽ
void ZYExpression::SetLeftExpression(ZYSyntaxTree *leftExpression)
{
    this->leftExpression=leftExpression;
}

//�����ұ���ʽ
void ZYExpression::SetRightExpression(ZYSyntaxTree *rightExpression)
{
    this->rightExpression=rightExpression;
}

//��ӡ����ʽ����
void ZYExpression::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<����ʽ";

    cout<<" �к�="<<lineno;

    cout<<" �����=";

    PrintOperation(operation);

    cout<<">"<<endl;

    PrintChild(leftExpression,indent);

    PrintChild(rightExpression,indent);

    PrintIndent(indent);
    
    cout<<"</����ʽ>"<<endl;
}

//�Ա���ʽ��������������
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

//��̬�����ʽ����ֵ
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

//����Ӧ�½����ı�������
int ZYExpression::CalcDescentTableIndex(ZYExecutor *executor)
{
    int l=leftExpression->CalcDescentTableIndex(executor);
    int r=rightExpression->CalcDescentTableIndex(executor);
    return l>r?l:r;
}

//���������ж���
ZYRowValue::ZYRowValue(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_RowValue,"",lineno,true)
{
}

//��ӡ�����ж���
void ZYRowValue::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<ֵ��";

    cout<<" �к�="<<lineno;

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</ֵ��>"<<endl;
}

//���쵥�����ݶ���
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

//��ȡ���ݶ�������ֵ
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

//��ӡ�������ݶ���
void ZYValue::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<ֵ";

    cout<<" �к�="<<lineno;

    cout<<" ����="<<dataType;

    switch(dataType)
    {
    case EColumnType_String:
        cout<<" �ַ���=\""<<name<<"\"";
        break;
    case EColumnType_Double:
        cout<<" С��="<<d;
        break;
    case EColumnType_Integer:
        cout<<" ����="<<a;
        break;
    }

    cout<<">";
    
    cout<<"</ֵ>"<<endl;
}

//��̬�󵥸����ݶ���ֵ
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