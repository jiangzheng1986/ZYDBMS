//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-语法树表示(第2部分) 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//构造选择语句对象
ZYSelectStmt::ZYSelectStmt(int lineno)
                    :ZYTableRef("","",lineno)
{
    type=ESyntaxTreeType_SelectStmt;
    destColumns=new vector<ZYSyntaxTree *>();
    sourceTables=new vector<ZYTableRef *>();
    this->condition=NULL;
    this->distinct=false;
    this->executed=false;
    this->associated=false;
    this->tableName[0]='\0';
    this->hasAggreColumn=false;
    this->conditionDescented=false;
}

//析构选择语句对象
ZYSelectStmt::~ZYSelectStmt(void)
{
    Release(condition);
}

//获取目标列列表
vector<ZYSyntaxTree *> *ZYSelectStmt::GetDestColumns(void)
{
    return this->destColumns;
}

//获取源表列表
vector<ZYTableRef *> *ZYSelectStmt::GetSourceTables(void)
{
    return this->sourceTables;
}

//设置选择条件表达式
void ZYSelectStmt::SetCondition(ZYSyntaxTree *condition)
{
    if(condition->GetType()==ESyntaxTreeType_BoolExpression)
    {
        this->condition=(ZYBoolExpression *)condition;
    }
}

//设置行是否互异标志
void ZYSelectStmt::SetDistinct(bool distinct)
{
    this->distinct=distinct;
}

//设置相关子查询标志
void ZYSelectStmt::SetAssociated(bool associated)
{
    this->associated=associated;
}

//设置查询结果名
void ZYSelectStmt::SetTableName(char *tableName)
{
    strcpy(this->tableName,tableName);
}

//获取表格名称
char *ZYSelectStmt::GetTableName(void)
{
    return this->tableName;
}

//打印选择语句对象
void ZYSelectStmt::Print(int indent)
{
    int i;

    ZYBoolExpression *sub_condition;

    PrintIndent(indent);

    cout<<"<选择语句";

    cout<<" 行号="<<lineno;

    if(distinct)
    {
        cout<<" 不重复="<<distinct;
    }

    if(asName!=NULL)
    {
        cout<<" 别名="<<asName;
    }

    cout<<">"<<endl;

    PrintChildren(indent);

    if(conditionDescented==false)
    {
        PrintChild(condition,indent);
    }

    if(pkeyEquals!=NULL)
    {
        pkeyEquals->Print(indent+2);
    }

    if(akeyEquals!=NULL)
    {
        akeyEquals->Print(indent+2);
    }

    for(i=0;i<sub_conditions->size();i++)
    {
        sub_condition=(*sub_conditions)[i];
        sub_condition->Print(indent+2);
    }

    PrintIndent(indent);
        
    cout<<"</选择语句>"<<endl;
}

//对选择语句对象进行语义分析
void ZYSelectStmt::Analyze(ZYAnalyzer *analyzer)
{
    ZYAnalyzer *analyzer1=new ZYAnalyzer(analyzer);

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_TableRef)
        {
            (*it)->Analyze(analyzer1);
        }
    }

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()!=ESyntaxTreeType_TableRef)
        {
            (*it)->Analyze(analyzer1);
        }
    }

    if(condition!=NULL)
    {
        condition->Analyze(analyzer1);
    }

    analyzer1->GetSource()->SetAcknowledgedFlag(asNameTokenStart,asNameTokenEnd);

    delete analyzer1;
}

//执行选择语句对象
void ZYSelectStmt::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    IZYTable *t_dest;
        
    int N;

    int table_no;

    executor->SetCurrentSelectStmt(this);

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    //如果不是相关子查询或未执行过,为其再分配临时表
    if(strcmp(tableName,"")==0)
    {
        table_no=executor->AllocateTempTableNo();
        if(table_no==0)
        {
            sprintf(tableName,"查询结果");
        }
        else
        {
            sprintf(tableName,"临时表%d",table_no);
        }
    }

    t_dest=db->CreateView(tableName);

    tableIndex=t_dest;

    executor->SetCurrentTable(t_dest);//设置目标表

    executor->ClearSourceTables();

    if(ExecuteFromList(executor)==false)
    {
        executor->SetCurrentTable(NULL);//设置结果表为空
        return;
    }

    ExecuteChildren(executor,ESyntaxTreeType_ColumnRef);//建立目标表格列

    DetectAndResetAggreColumns();

    if(conditionDescented==false)
    {
        DescentSubConditionExpressions(executor);
    }

    N=executor->GetSourceTableCount();

    PerformSelectN(executor,N,1);

    ReverseExecuteChildren(executor,ESyntaxTreeType_OrderBy);

    if(table_no==0&&executor->GetDisplayOST())
    {
        cout<<"优化语法树:"<<endl;
        Print(0);
    }
}

//执行源表格列表
bool ZYSelectStmt::ExecuteFromList(ZYExecutor *executor)
{
    IZYTable *t_src;

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_SelectStmt)
        {
            ZYExecutor *executor1=new ZYExecutor(executor,executor->GetCurrentDatabase(),(*it));

            executor1->Execute();

            ((ZYSelectStmt*)(*it))->executed=true;
                
            delete executor1;

            executor->AddSourceTable((ZYTableRef*)*it);
        }
        else if((*it)->GetType()==ESyntaxTreeType_TableRef)
        {
            (*it)->Execute(executor);
            
            t_src=((ZYTableRef *)(*it))->GetTableIndex();

            if(t_src==NULL)
            {
                cout<<lineno<<":执行错误：表格\'"<<((ZYTableRef *)(*it))->GetName()<<"\'不存在，执行取消"<<endl;
                return false;
            }
            else
            {
                if( executor->GetDisplayTables()&&
                    this->executed==false )
                {
                    executor->DisplayTable(t_src,true);
                }
            }
        }
    }

    return true;
}

//执行选择查询(总共N个表格,当前处理第n个表格)
void ZYSelectStmt::PerformSelectN(ZYExecutor *executor,int N,int n)
{
    int i,j,ii,j1;

    ZYBoolExpression *boolExpression;

    ZYColumnRef *columnRef;

    ZYSyntaxTree *value;

    IZYIndex *index;

    char buf[1000];

    char buf1[1000];

    IZYTable *t_src=executor->GetSourceTableIndex(n-1);

    ZYTableRef *tableRef=(ZYTableRef *)executor->GetSourceTable(n-1);

    //[主键]=值 类型子条件表达式

    if(tableRef->GetPKeyEquals()!=NULL)
    {
        boolExpression=tableRef->GetPKeyEquals();

        columnRef=(ZYColumnRef *)boolExpression->GetLeftExpression();

        value=boolExpression->GetRightExpression();

        value->GetResult(executor,buf);

        j=t_src->FindColumn(columnRef->GetName());

        i=t_src->FindRow(j,buf);

        executor->SetSourceTableItem(n-1,i);

        if(tableRef->TestSubConditions(executor))
        {
            if(n!=N)
            {
                PerformSelectN(executor,N,n+1);
            }
            else//(n==N)
            {
                PerformSelect(executor);
            }
        }

        return;
    }   

    //[辅键]=值 类型子条件表达式

    if(tableRef->GetAKeyEquals()!=NULL)
    {
        boolExpression=tableRef->GetAKeyEquals();

        columnRef=(ZYColumnRef *)boolExpression->GetLeftExpression();

        value=boolExpression->GetRightExpression();

        value->GetResult(executor,buf);

        j=t_src->FindColumn(columnRef->GetName());

        j1=t_src->GetPrimaryKey();

        index=t_src->GetIndexOn(j);

        ii=index->FindIndex(buf);

        while(index->IsIndexOf(ii,buf))
        {
            index->GetIndex(ii,buf1);

            i=t_src->FindRow(j1,buf1);

            executor->SetSourceTableItem(n-1,i);

            if(tableRef->TestSubConditions(executor))
            {
                if(n!=N)
                {
                    PerformSelectN(executor,N,n+1);
                }
                else//(n==N)
                {
                    PerformSelect(executor);
                }
            }

            ii++;
        }

        return;
    }

    //全是普通类型子条件表达式

    for(i=0;i<t_src->GetRowCount();i++)
    {
        executor->SetSourceTableItem(n-1,i);

        if(tableRef->TestSubConditions(executor))
        {
            if(n!=N)
            {
                PerformSelectN(executor,N,n+1);
            }
            else//(n==N)
            {
                PerformSelect(executor);
            }
        }
    }
}

//执行选择查询
void ZYSelectStmt::PerformSelect(ZYExecutor *executor)
{
    int i;
        
    IZYTable *t_dest;

    bool b;

    ZYColumnRef *column;

    char buf[1000];

    vector<ZYSyntaxTree *>::iterator it;    

    b=false;

    if(!conditionDescented&&condition!=NULL)
    {
        condition->GetResult(executor,&b);
    }

    if(conditionDescented||condition==NULL||b)
    {
        t_dest=executor->GetCurrentTable();

        //如果是聚合函数
        if(hasAggreColumn)
        {
            i=t_dest->GetRowCount()-1;

            if(i!=-1)
            {
                //删除掉之前的聚合函数元组
                t_dest->DeleteRow(i);
            }
        }

        i=t_dest->AppendRow();
        
        for(it=children->begin();it<children->end();it++)
        {
            if((*it)->GetType()==ESyntaxTreeType_ColumnRef)
            {
                column=(ZYColumnRef *)(*it);
                column->GetResult(executor,buf);
                
                //如果列没有别名
                if(column->HasAsName()==false)
                {
                    t_dest->SetData(i,column->GetName(),buf);
                }
                //如果列有别名
                else
                {
                    t_dest->SetData(i,column->GetAsName(),buf);
                }
            }
        }
        
        //如果不允许重复且元组是重复的(不是不重复的)
        if(this->distinct==true&&executor->IsDistinct(t_dest,i)==false)
        {
            //删除掉新增的重复的元组
            t_dest->DeleteRow(i);
        }
        //允许重复或是不重复的
        else
        {
            //修改表格的自动编号
            t_dest->NewAutoid();
        }
    }
}

//检测并重设聚合函数列
void ZYSelectStmt::DetectAndResetAggreColumns(void)
{
    vector<ZYSyntaxTree *>::iterator it;

    ZYColumnRef *column;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_ColumnRef)
        {
            column=(ZYColumnRef *)(*it);
                    
            //如果列是聚合函数列,重置聚合函数,并置聚合函数存在标志
            if(column->TestAndResetAggreColumn())
            {
                this->hasAggreColumn=true;
            }
        }
    }
}

//下降各子条件表达式到表格
void ZYSelectStmt::DescentSubConditionExpressions(ZYExecutor *executor)
{
    int i;
    vector<ZYBoolExpression *> *expressions=new vector<ZYBoolExpression *>();
    ZYBoolExpression *condition=this->condition;
    if(condition==NULL)
    {
        return;
    }
    while(condition->GetType()==ESyntaxTreeType_BoolExpression&&
          condition->GetOperation()==ETokenType_And)
    {
        expressions->push_back((ZYBoolExpression *)(condition->GetRightExpression()));
        condition=(ZYBoolExpression *)(condition->GetLeftExpression());
    }
    expressions->push_back(condition);
    for(i=expressions->size()-1;i>=0;i--)
    {
        DescentSubConditionExpression(executor,(*expressions)[i]);
    }
    this->conditionDescented=true;
    delete expressions;
}

//下降子条件表达式到表格
void ZYSelectStmt::DescentSubConditionExpression(ZYExecutor *executor,ZYBoolExpression *sub_condition)
{
    int i;
    ZYSyntaxTree *syntaxTree;
    ZYTableRef *tableRef;
    i=sub_condition->CalcDescentTableIndex(executor);
    syntaxTree=executor->GetSourceTable(i);
    if( syntaxTree->GetType()==ESyntaxTreeType_TableRef||
        syntaxTree->GetType()==ESyntaxTreeType_SelectStmt)
    {
        tableRef=(ZYTableRef *)syntaxTree;
        tableRef->AddSubCondition(executor,sub_condition,i);
    }
}

//动态求选择语句对象值
EResultType ZYSelectStmt::GetResult(ZYExecutor *executor,void *pData)
{
    if(associated==true||executed==false)
    {
        ZYExecutor *executor1=new ZYExecutor(executor,executor->GetCurrentDatabase(),this);
        executor1->Execute();
        delete executor1;
        executed=true;
    }

    (*(IZYTable **)pData)=tableIndex;

    return EResultType_Table;
}

//计算应下降到的表格索引
int ZYSelectStmt::CalcDescentTableIndex(ZYExecutor *executor)
{
    return executor->GetSourceTableCount()-1;
}

//构造按列分组对象
ZYGroupBy::ZYGroupBy(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_GroupBy,"",lineno,true)
{
    this->havingCondition=NULL;
}

//析构按列分组对象
ZYGroupBy::~ZYGroupBy(void)
{
    Release(this->havingCondition);
}

//设置筛选条件
void ZYGroupBy::SetHavingCondition(ZYSyntaxTree *havingCondition)
{
    if(havingCondition->GetType()==ESyntaxTreeType_BoolExpression)
    {
        this->havingCondition=(ZYBoolExpression *)havingCondition;
    }   
}

//打印按列分组对象
void ZYGroupBy::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<分组";
        
    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChildren(indent);

    PrintChild(havingCondition,indent);

    PrintIndent(indent);
    
    cout<<"</分组>"<<endl;
}

//执行按列分组对象
void ZYGroupBy::Execute(ZYExecutor *executor)
{
    IZYTable *t;
    
    t=executor->GetCurrentTable();

    t->SelectSort(name,ESortOrder_Asc);
}

//构造列引用对象
ZYColumnRef::ZYColumnRef(char *name,int lineno,char *tableName)
                    :ZYSyntaxTree(ESyntaxTreeType_ColumnRef,name,lineno,false)
{
    this->tableName=tableName;
    columnIndex=-1;
    columnType=0;
    sourceTable=NULL;
    tableSourceIndex=0;
    calculated=false;
    associated=false;
    executor=NULL;
    asName[0]='\0';
    expression=NULL;
    this->tableTokenStart=0;
    this->tableTokenEnd=0;
    this->asNameTokenStart=0;
    this->asNameTokenEnd=0;
    this->tableFound=false;
    this->columnFound=false;
}

//析构列引用对象
ZYColumnRef::~ZYColumnRef(void)
{
    Release(tableName);
    Release(expression);
}

//设置表格词法标记位置
void ZYColumnRef::SetTableTokenLoc(int tableTokenStart,int tableTokenEnd)
{
    this->tableTokenStart=tableTokenStart;
    this->tableTokenEnd=tableTokenEnd;
}    

//设置别名词法标记位置
void ZYColumnRef::SetAsNameTokenLoc(int asNameTokenStart,int asNameTokenEnd)
{
    this->asNameTokenStart=asNameTokenStart;
    this->asNameTokenEnd=asNameTokenEnd;
}

//获取列索引
int ZYColumnRef::GetColumnIndex(void)
{
    return this->columnIndex;
}

//获取列类型
EColumnType ZYColumnRef::GetColumnType(void)
{
    return (EColumnType)columnType;
}

//设置源表格
void ZYColumnRef::SetSourceTable(ZYTableRef *sourceTable)
{
    this->sourceTable=sourceTable;
}

//设置别名
void ZYColumnRef::SetAsName(char *asName)
{
    strcpy(this->asName,asName);
}

//获取别名
char *ZYColumnRef::GetAsName(void)
{
    return asName;
}

//是否有别名
bool ZYColumnRef::HasAsName(void)
{
    return (asName!=NULL&&strcmp(asName,"")!=0);
}

//测试并重设聚合函数列
bool ZYColumnRef::TestAndResetAggreColumn(void)
{
    if(expression==NULL)
    {
        return false;
    }
    if(expression->GetType()==ESyntaxTreeType_AggregateFunction)
    {
        //重置一下聚合函数对象
        ((ZYAggregateFunction *)expression)->Reset();
        return true;
    }
    else
    {
        return false;
    }
}

//设置子表达式
void ZYColumnRef::SetExpression(ZYSyntaxTree *expression)
{
    this->expression=expression;
}

//打印列引用对象
void ZYColumnRef::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<列 名称=\"";

    PrintString(name);

    cout<<"\"";

    cout<<" 行号="<<lineno;

    if(strcmp(asName,"")!=0)
    {
        cout<<" 别名=\"";

        PrintString(asName);

        cout<<"\"";
    }

    if(strcmp(tableName,"")!=0)
    {
        cout<<" 表格=\"";

        PrintString(tableName);

        cout<<"\"";
    }

    if(expression==NULL)
    {
        cout<<">";
    }
    else
    {
        cout<<">"<<endl;
    }

    PrintChild(expression,indent);

    if(expression!=NULL)
    {
        PrintIndent(indent);
    }

    cout<<"</列>"<<endl;
}

//查找是否有该列
void ZYColumnRef::LookUpInTables(ZYAnalyzer *analyzer)
{
    int i,j;
    IZYTable *t;
    ZYSyntaxTree *syntaxTree;
    ZYTableRef *table;

    for(i=0;i<analyzer->GetSourceTableCount();i++)
    {
        syntaxTree=analyzer->GetSourceTable(i);

        table=(ZYTableRef *)syntaxTree;

        if(tableName!=NULL&&strcmp(tableName,"")!=0)
        {//如果有表名,就在该表格的各属性中找
            if(strcmp(tableName,table->GetName())==0||
               strcmp(tableName,table->GetAsName())==0)
            {
                t=table->GetTableIndex();
                j=t->FindColumn(this->name);
                if(j!=-1)
                {
                    this->tableFound=true;
                    this->columnFound=true;
                    return;
                }

                //指定表格中没有指定属性
                cout<<lineno<<":语义错误:\'"<<tableName<<"\'表格中没有\'"<<this->name<<"\'属性"<<endl;
                analyzer->Error();
                this->tableFound=true;
                this->columnFound=false;
                return;
            }
        }
        else
        {//如果没表名,在各表格的各属性中找
            t=table->GetTableIndex();
            j=t->FindColumn(this->name);
            if(j!=-1)
            {
                this->tableFound=true;
                this->columnFound=true;
                return;
            }
        }
    }

    //找不到指定的表格或属性

    //若有父查询,则搜索它的表格
    if(analyzer->GetParentAnalyzer()!=NULL)
    {
        LookUpInTables(analyzer->GetParentAnalyzer());
    }
    //否则,报告错误
    else
    {
        if(tableName!=NULL&&strcmp(tableName,"")!=0)
        {
            //找不到指定的表格
            this->tableFound=false;
            this->columnFound=false;
            cout<<lineno<<":语义错误:找不到\'"<<tableName<<"\'表格"<<endl;
            analyzer->Error();
        }
        else
        {
            //找不到指定的属性
            this->tableFound=true;
            this->columnFound=false;
            cout<<lineno<<":语义错误:找不到\'"<<this->name<<"\'属性"<<endl;
            analyzer->Error();
        }
    }
}

//对列引用对象进行语义分析(SelectStmt调用)
void ZYColumnRef::Analyze(ZYAnalyzer *analyzer)
{
	//如果是数据列
    if(expression==NULL)
    {
		LookUpInTables(analyzer);

		if(this->columnFound)
		{
			analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
		}

		if(this->tableFound)
		{
			analyzer->GetSource()->SetAcknowledgedFlag(tableTokenStart,tableTokenEnd);
		}
	}
	//表达式
	else
	{
		expression->Analyze(analyzer);
	}

	analyzer->GetSource()->SetAcknowledgedFlag(asNameTokenStart,asNameTokenEnd);
}

//对列引用对象进行语义分析(CreateView调用)
void ZYColumnRef::Analyze1(ZYAnalyzer *analyzer)
{
    analyzer->GetSource()->SetAcknowledgedFlag(nameTokenStart,nameTokenEnd);
}

//对列引用对象进行语义分析(InsertStmt、AlterTable调用)
void ZYColumnRef::Analyze2(ZYAnalyzer *analyzer)
{
    IZYTable *t_dest;
    char tableName[1000];

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
}

//执行列引用对象(复制列)
void ZYColumnRef::Execute(ZYExecutor *executor)
{
    IZYTable *t_dest,*t_src;

    char buf[1000];

    t_dest=executor->GetCurrentTable();

    //如果是数据列
    if(name!=NULL&&strcmp(name,"")!=0)
    {
        //计算源表索引和源表

        CalcTableSourceIndex(executor,false);

        t_src=executor->GetSourceTableIndex(tableSourceIndex);

        //如果没有别名
        if(asName==NULL||strcmp(asName,"")==0)
        {
            t_dest->CopyColumn(t_src,name);
        }
        //如果有别名
        else
        {
            int type;
    
            int len;

            int attribute;

            int j=t_src->FindColumn(name);

            t_src->GetColumn(j,"类型",&type);

            t_src->GetColumn(j,"长度",&len);

            t_src->GetColumn(j,"属性",&attribute);

            t_dest->AppendColumn(asName,(EColumnType)type,len,attribute);
        }
    }
    //如果是表达式
    else
    {
        //如果没有别名,分配一个别名
        if(asName==NULL||strcmp(asName,"")==0)
        {
            sprintf(asName,"临时列%d",executor->AllocateTempColumnNo());
        }

        //类型暂时用GetResult的返回值类型代替
        int dataType=(int)expression->GetResult(executor,buf);

        //如果是聚合函数,则调用GetResult后恢复其状态
        if(expression->GetType()==ESyntaxTreeType_AggregateFunction)
        {
            ((ZYAggregateFunction *)expression)->Reset();
        }

        //字符串长度暂时无法得知,用32代替
        t_dest->AppendColumn(asName,(EColumnType)dataType,32,0);
    }
}

//执行列引用对象(获取列信息)
void ZYColumnRef::Execute1(ZYExecutor *executor)
{
    IZYTable *t_dest;

    t_dest=executor->GetCurrentTable();
    
    columnIndex=t_dest->FindColumn(name);

    t_dest->GetColumn(columnIndex,"类型",&columnType);
}

//动态求列引用对象值
EResultType ZYColumnRef::GetResult(ZYExecutor *executor,void *pData)
{
    IZYTable *t_src;

    int i;

    *(int *)pData=0;

    if(expression!=NULL)
    {
        return expression->GetResult(executor,pData);
    }

    CalcTableSourceIndex(executor,false);

    if(this->executor==NULL)
    {
        (*(int *)pData)=0;
        return (EResultType)columnType;
    }

    t_src=this->executor->GetSourceTableIndex(tableSourceIndex);

    columnIndex=t_src->FindColumn(name);

    t_src->GetColumn(columnIndex,"类型",&columnType);

    i=this->executor->GetSourceTableItem(tableSourceIndex);/////////////////////////////////////

    if(columnIndex==-1)
    {
        Execute1(this->executor);
    }

    t_src->GetData(i,columnIndex,pData);

    return (EResultType)columnType;
}

//计算应下降到的表格索引
int ZYColumnRef::CalcDescentTableIndex(ZYExecutor *executor)
{
    CalcTableSourceIndex(executor,false);
    return tableSourceIndex;
}

//是否是主键列
bool ZYColumnRef::IsPKeyColumn(ZYExecutor *executor)
{
    IZYTable *t_src;

    int attr;

    CalcTableSourceIndex(executor,false);

    t_src=this->executor->GetSourceTableIndex(tableSourceIndex);
    
    int j=t_src->FindColumn(name);

    t_src->GetColumn(j,"属性",&attr);

    if((attr&EColumnAttr_IndexedP)!=0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//是否是辅键列
bool ZYColumnRef::IsAKeyColumn(ZYExecutor *executor)
{
    IZYTable *t_src;

    int attr;

    CalcTableSourceIndex(executor,false);

    t_src=this->executor->GetSourceTableIndex(tableSourceIndex);
    
    int j=t_src->FindColumn(name);

    t_src->GetColumn(j,"属性",&attr);

    if((attr&EColumnAttr_IndexedA)!=0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//获取表格源索引
int ZYColumnRef::GetTableSourceIndex(void)
{
    return tableSourceIndex;
}

//是否相关属性
bool ZYColumnRef::IsAssociated(void)
{
    return associated;
}

//计算表格源索引
void ZYColumnRef::CalcTableSourceIndex(ZYExecutor *executor,bool associated)
{
    int i,j;
    IZYTable *t;
    ZYSyntaxTree *syntaxTree;
    ZYTableRef *table;

    if(calculated)
    {
        return;
    }

    for(i=0;i<executor->GetSourceTableCount();i++)
    {
        syntaxTree=executor->GetSourceTable(i);

        table=(ZYTableRef *)syntaxTree;

        if(tableName!=NULL&&strcmp(tableName,"")!=0)
        {//如果有表名,就在该表格的各属性中找
            if(strcmp(tableName,table->GetName())==0||
               strcmp(tableName,table->GetAsName())==0)
            {
                t=table->GetTableIndex();
                j=t->FindColumn(this->name);
                if(j!=-1)
                {
                    calculated=true;
                    this->executor=executor;
                    tableSourceIndex=i;
                    return;
                }

                //指定表格中没有指定属性
                cout<<lineno<<":执行错误:\'"<<tableName<<"\'表格中没有\'"<<this->name<<"\'属性"<<endl;
                executor->Error();
                this->executor=NULL;
                tableSourceIndex=i;
                return;
            }
        }
        else
        {//如果没表名,在各表格的各属性中找
            t=table->GetTableIndex();
            j=t->FindColumn(this->name);
            if(j!=-1)
            {
                calculated=true;
                this->executor=executor;
                tableSourceIndex=i;
                return;
            }
        }
    }

    //找不到指定的表格或属性

    this->executor=NULL;

    //若有父查询,则搜索它的表格
    if(executor->GetParentExecutor()!=NULL)
    {
        CalcTableSourceIndex(executor->GetParentExecutor(),true);
        executor->GetCurrentSelectStmt()->SetAssociated(true);
        this->associated=true;
    }
    //否则,报告错误
    else
    {
        if(tableName!=NULL&&strcmp(tableName,"")!=0)
        {
            //找不到指定的表格
            calculated=true;
            cout<<lineno<<":执行错误:找不到\'"<<tableName<<"\'表格"<<endl;
            executor->Error();
            tableSourceIndex=0;
        }
        else
        {
            //找不到指定的属性
            calculated=true;
            cout<<lineno<<":执行错误:找不到\'"<<this->name<<"\'属性"<<endl;
            executor->Error();
            tableSourceIndex=0;
        }
    }
}

//构造表格引用对象
ZYTableRef::ZYTableRef(char *name,char *asName,int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_TableRef,name,lineno,true)
{
    itemIndex=-1;
    tableIndex=NULL;
    this->asName=asName;
    akeyEquals=NULL;
    pkeyEquals=NULL;
    this->sub_conditions=new vector<ZYBoolExpression *>();
    this->tableTokenStart=0;
    this->tableTokenEnd=0;
    this->asNameTokenStart=0;
    this->asNameTokenEnd=0;
}

//析构表格引用对象
ZYTableRef::~ZYTableRef(void)
{
    Release(asName);
    delete sub_conditions;
}

//设置表格词法标记位置
void ZYTableRef::SetTableTokenLoc(int tableTokenStart,int tableTokenEnd)
{
    this->tableTokenStart=tableTokenStart;
    this->tableTokenEnd=tableTokenEnd;
}

//设置别名词法标记位置
void ZYTableRef::SetAsNameTokenLoc(int asNameTokenStart,int asNameTokenEnd)
{
    this->asNameTokenStart=asNameTokenStart;
    this->asNameTokenEnd=asNameTokenEnd;
}   

//设置行索引
void ZYTableRef::SetItemIndex(int itemIndex)
{
    this->itemIndex=itemIndex;
}

//获取行索引
int ZYTableRef::GetItemIndex(void)
{
    return itemIndex;
}

//设置表格对象指针
void ZYTableRef::SetTableIndex(IZYTable *tableIndex)
{
    this->tableIndex=tableIndex;
}

//获取表格对象指针
IZYTable *ZYTableRef::GetTableIndex(void)
{
    return this->tableIndex;
}

//设置别名
void ZYTableRef::SetAsName(char *asName)
{
    this->asName=asName;
}

//获取别名
char *ZYTableRef::GetAsName(void)
{
    return this->asName;
}

//打印表格引用对象
void ZYTableRef::Print(int indent)
{
    int i;

    ZYBoolExpression *sub_condition;

    PrintIndent(indent);

    cout<<"<表格 名称=\"";

    PrintString(name);

    cout<<"\"";

    cout<<" 行号="<<lineno;

    if(asName!=NULL&&strcmp(asName,"")!=0)
    {
        cout<<" 别名=\"";

        PrintString(asName);

        cout<<"\"";
    }

    if( akeyEquals==NULL&&
        pkeyEquals==NULL&&
        sub_conditions->size()==0)
    {
        cout<<">";

        cout<<"</表格>"<<endl;
    }
    else
    {
        cout<<">"<<endl;

        if(pkeyEquals!=NULL)
        {
            pkeyEquals->Print(indent+2);            
        }

        if(akeyEquals!=NULL)
        {
            akeyEquals->Print(indent+2);            
        }

        for(i=0;i<sub_conditions->size();i++)
        {
            sub_condition=(*sub_conditions)[i];
            sub_condition->Print(indent+2);
        }

        PrintIndent(indent);
            
        cout<<"</表格>"<<endl;
    }
}

//对选择语句对象进行语义分析
void ZYTableRef::Analyze(ZYAnalyzer *analyzer)
{
    IZYDatabase *db;
    
    db=analyzer->GetCurrentDatabase();

    tableIndex=db->GetTableI(name);

    analyzer->SetCurrentTable(tableIndex);

    if(tableIndex!=NULL)
    {
        analyzer->GetSource()->SetAcknowledgedFlag(tableTokenStart,tableTokenEnd);

        analyzer->GetSource()->SetAcknowledgedFlag(asNameTokenStart,asNameTokenEnd);

        analyzer->AddSourceTable(this);
    }
    else
    {
        cout<<lineno<<":语义错误:表格\'"<<name<<"\'在当前数据库中不存在..."<<endl;
        analyzer->Error();
    }
}

//设置源表
void ZYTableRef::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;
    
    db=executor->GetCurrentDatabase();

    tableIndex=db->GetTableI(name);

    executor->AddSourceTable(this);
}

//设置目标表
void ZYTableRef::SetDestTable(ZYExecutor *executor)
{
    IZYDatabase *db;
    IZYTable *t;
    
    db=executor->GetCurrentDatabase();

    t=db->GetTableI(name);

    executor->SetCurrentTable(t);
}

//增加表格子条件表达式
void ZYTableRef::AddSubCondition(ZYExecutor *executor,ZYBoolExpression *sub_condition,int tableSourceIndex)
{
    ZYColumnRef *columnRef;

    if(sub_condition->GetOperation()==ETokenType_EQ)
    {
        //[主键]=值
        if(sub_condition->GetLeftExpression()->GetType()==ESyntaxTreeType_ColumnRef)
        {
            columnRef=(ZYColumnRef *)sub_condition->GetLeftExpression();

            if( columnRef->IsAssociated()==false&&
                tableSourceIndex==columnRef->GetTableSourceIndex())
            {
                if(columnRef->IsPKeyColumn(executor))
                {
                    pkeyEquals=sub_condition;
                    //[主键]=值 优先于 //[辅键]=值
                    if(akeyEquals!=NULL)
                    {
                        sub_conditions->push_back(akeyEquals);
                        akeyEquals=NULL;
                    }
                    return;
                }
            }
        }

        //值=[主键]
        if(sub_condition->GetRightExpression()->GetType()==ESyntaxTreeType_ColumnRef)
        {
            columnRef=(ZYColumnRef *)sub_condition->GetRightExpression();

            if( columnRef->IsAssociated()==false&&
                tableSourceIndex==columnRef->GetTableSourceIndex())
            {
                if(columnRef->IsPKeyColumn(executor))
                {
                    //交换成 [主键]=值
                    sub_condition->SwapLeftAndRight();
                    pkeyEquals=sub_condition;
                    //[主键]=值 优先于 //[辅键]=值
                    if(akeyEquals!=NULL)
                    {
                        sub_conditions->push_back(akeyEquals);
                        akeyEquals=NULL;
                    }
                    return;
                }
            }
        }

        if(pkeyEquals==NULL&&akeyEquals==NULL)
        {
            //[辅键]=值
            if(sub_condition->GetLeftExpression()->GetType()==ESyntaxTreeType_ColumnRef)
            {
                columnRef=(ZYColumnRef *)sub_condition->GetLeftExpression();

                if( columnRef->IsAssociated()==false&&
                    tableSourceIndex==columnRef->GetTableSourceIndex())
                {
                    if(columnRef->IsAKeyColumn(executor))
                    {
                        akeyEquals=sub_condition;
                        return;
                    }
                }
            }

            //值=[辅键]
            if(sub_condition->GetRightExpression()->GetType()==ESyntaxTreeType_ColumnRef)
            {
                columnRef=(ZYColumnRef *)sub_condition->GetRightExpression();

                if( columnRef->IsAssociated()==false&&
                    tableSourceIndex==columnRef->GetTableSourceIndex())
                {
                    if(columnRef->IsAKeyColumn(executor))
                    {
                        //交换成 [辅键]=值
                        sub_condition->SwapLeftAndRight();
                        akeyEquals=sub_condition;
                        return;
                    }
                }
            }
        }
    }
    sub_conditions->push_back(sub_condition);
}

//获取 [主键]=值 类型子条件表达式
ZYBoolExpression *ZYTableRef::GetPKeyEquals(void)
{
    return this->pkeyEquals;
}

//获取 [辅键]=值 类型子条件表达式
ZYBoolExpression *ZYTableRef::GetAKeyEquals(void)
{
    return this->akeyEquals;
}

//测试各表格子条件表达式
bool ZYTableRef::TestSubConditions(ZYExecutor *executor)
{
    int i;

    bool b;

    ZYBoolExpression *sub_condition;

    for(i=0;i<sub_conditions->size();i++)
    {
        sub_condition=(*sub_conditions)[i];
        
        sub_condition->GetResult(executor,&b);

        if(b==false)
        {
            return false;
        }
    }

    return true;
}

//构造按列排序对象
ZYOrderBy::ZYOrderBy(char *name,int lineno,ESortOrder sortOrder)
                    :ZYSyntaxTree(ESyntaxTreeType_OrderBy,name,lineno,false)
{
    this->sortOrder=sortOrder;
}

//打印按列排序对象
void ZYOrderBy::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<排序";
        
    cout<<" 名称=\"";

    PrintString(name);

    cout<<"\"";

    cout<<" 行号="<<lineno;

    cout<<" 顺序="<<sortOrder;

    cout<<">";
    
    cout<<"</排序>"<<endl;
}

//执行按列排序对象
void ZYOrderBy::Execute(ZYExecutor *executor)
{
    IZYTable *t;
    
    t=executor->GetCurrentTable();

    if(sortOrder==ESortOrder_Asc)
    {
        t->SelectSort(name,ESortOrder_Asc);
    }
    else
    {
        t->SelectSort(name,ESortOrder_Desc);
    }
}

//构造插入语句对象
ZYInsertStmt::ZYInsertStmt(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_InsertStmt,"",lineno,true)
{
    destTable=NULL;
}

//析构插入语句对象
ZYInsertStmt::~ZYInsertStmt(void)
{
    Release(destTable);
}

//设置插入目标表格
void ZYInsertStmt::SetDestTable(ZYTableRef *destTable)
{
    this->destTable=destTable;
}

//打印插入语句对象
void ZYInsertStmt::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<插入语句";
    
    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChild(destTable,indent);

    PrintChildren(indent);

    PrintIndent(indent);
    
    cout<<"</插入语句>"<<endl;
}

//对插入语句对象进行语义分析
void ZYInsertStmt::Analyze(ZYAnalyzer *analyzer)
{
    destTable->Analyze(analyzer);

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        if((*it)->GetType()==ESyntaxTreeType_ColumnRef)
        {
            ((ZYColumnRef*)(*it))->Analyze2(analyzer);
        }
    }

    analyzer->SetCurrentTable(NULL);
}

//执行插入语句对象
void ZYInsertStmt::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;

    IZYTable *t_dest;
        
    int j,i1;

    char buf[1000];
    
    vector<ZYSyntaxTree *>::iterator it1;

    vector<ZYSyntaxTree *>::iterator it2;

    vector<ZYSyntaxTree *>::iterator it3;

    vector<ZYColumnRef *> *columns;

    ZYRowValue *rowValue;

    ZYColumnRef *column;

    ZYValue *value;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    columns=new vector<ZYColumnRef *>();

    destTable->SetDestTable(executor);//设置目标表

    t_dest=executor->GetCurrentTable();

    if(t_dest==NULL)
    {
        cout<<"错误:表格\'"<<destTable->GetName()<<"\'不存在，执行取消"<<endl;
        return;
    }

    for(it1=children->begin();it1<children->end();it1++)
    {
        if((*it1)->GetType()==ESyntaxTreeType_ColumnRef)
        {
            column=(ZYColumnRef *)(*it1);

            column->Execute1(executor);

            columns->push_back(column);
        }
    }

    t_dest->EnterBufferedMode();

    for(it2=children->begin();it2<children->end();it2++)
    {
        if((*it2)->GetType()==ESyntaxTreeType_RowValue)
        {
            rowValue=(ZYRowValue *)(*it2);

            i1=t_dest->AppendRow();
            
            for(it3=rowValue->GetChildren()->begin(),j=0;it3<rowValue->GetChildren()->end();it3++,j++)
            {
                if((*it3)->GetType()==ESyntaxTreeType_Value)
                {
                    *((int*)buf)=0;
                    value=(ZYValue *)(*it3);
                    if(j<columns->size())
                    {
                        column=(*columns)[j];
                        value->GetValue(buf);
                        t_dest->SetData(i1,column->GetColumnIndex(),buf);
                    }
                }
            }           
        }
    }

    t_dest->LeaveBufferedMode();

    delete columns;
}

//构造更新语句对象
ZYUpdateStmt::ZYUpdateStmt(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_UpdateStmt,"",lineno,true)
{
    this->condition=NULL;
    this->destTable=NULL;
}

//析构更新语句对象
ZYUpdateStmt::~ZYUpdateStmt(void)
{
    Release(destTable);
    Release(condition);
}

//设置目标表格
void ZYUpdateStmt::SetDestTable(ZYTableRef *destTable)
{
    this->destTable=destTable;
}

//设置更新条件
void ZYUpdateStmt::SetCondition(ZYSyntaxTree *condition)
{
    if(condition->GetType()==ESyntaxTreeType_BoolExpression)
    {
        this->condition=(ZYBoolExpression *)condition;
    }
}

//打印更新语句对象
void ZYUpdateStmt::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<修改语句";
    
    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChild(destTable,indent);

    PrintChildren(indent);

    PrintChild(condition,indent);

    PrintIndent(indent);
    
    cout<<"</修改语句>"<<endl;
}

//对更新语句对象进行语义分析
void ZYUpdateStmt::Analyze(ZYAnalyzer *analyzer)
{
    destTable->Analyze(analyzer);

    vector<ZYSyntaxTree *>::iterator it;

    for(it=children->begin();it<children->end();it++)
    {
        (*it)->Analyze(analyzer);
    }

    if(condition!=NULL)
    {
        condition->Analyze(analyzer);
    }

    analyzer->SetCurrentTable(NULL);
}

//执行更新语句对象
void ZYUpdateStmt::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;

    IZYTable *t_dest;
    
    int i,j,a;
    
    bool b;

    ZYColumnRef *column;

    ZYValue *value;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    destTable->SetDestTable(executor);//设置目标表

    destTable->Execute(executor);//设置源表
    
    t_dest=executor->GetCurrentTable();

    if(t_dest==NULL)
    {
        cout<<"错误:表格\'"<<destTable->GetName()<<"\'不存在，执行取消"<<endl;
        return;
    }

    //如果是UPDATE <表名> SET...型,直接更新全部元组
    if(condition==NULL)
    {
        t_dest->EnterBufferedMode();

        for(i=0;i<t_dest->GetRowCount();i++)
        {
            executor->SetCurrentItem(i);

            executor->SetSourceTableItem(0,i);

            ExecuteChildren(executor,ESyntaxTreeType_AssignExpression);
        }

        t_dest->LeaveBufferedMode();

        return;
    }

    //如果是UPDATE <表名> SET... WHERE 编号=<元组编号>型,用二分查找定位元组
    if(condition!=NULL&&
       condition->GetOperation()==ETokenType_EQ&&
       condition->GetLeftExpression()->GetType()==ESyntaxTreeType_ColumnRef&&
       condition->GetRightExpression()->GetType()==ESyntaxTreeType_Value)
    {
        column=(ZYColumnRef *)(condition->GetLeftExpression());
        
        column->Execute1(executor);

        if(column->IsPKeyColumn(executor))
        {
            value=(ZYValue *)(condition->GetRightExpression());

            value->GetResult(executor,&a);

            j=t_dest->FindColumn(column->GetName());

            i=t_dest->FindRow(j,&a);

            executor->SetCurrentItem(i);

            executor->SetSourceTableItem(0,i);

            ExecuteChildren(executor,ESyntaxTreeType_AssignExpression);

            return;
        }
    }

    //如果是其他类型的UPDATE语句

    t_dest->EnterBufferedMode();

    for(i=0;i<t_dest->GetRowCount();i++)
    {
        executor->SetCurrentItem(i);

        executor->SetSourceTableItem(0,i);

        condition->GetResult(executor,&b);

        if(b)
        {
            ExecuteChildren(executor,ESyntaxTreeType_AssignExpression);
        }
    }

    t_dest->LeaveBufferedMode();
}

//构造删除语句对象
ZYDeleteStmt::ZYDeleteStmt(int lineno)
                    :ZYSyntaxTree(ESyntaxTreeType_DeleteStmt,"",lineno,false)
{
    this->condition=NULL;
    this->destTable=NULL;
}

//析构删除语句对象
ZYDeleteStmt::~ZYDeleteStmt(void)
{
    Release(destTable);
    Release(condition);
}

//设置目标表格
void ZYDeleteStmt::SetDestTable(ZYTableRef *destTable)
{
    this->destTable=destTable;
}

//设置删除条件
void ZYDeleteStmt::SetCondition(ZYSyntaxTree *condition)
{
    if(condition->GetType()==ESyntaxTreeType_BoolExpression)
    {
        this->condition=(ZYBoolExpression *)condition;
    }
}

//打印删除语句对象
void ZYDeleteStmt::Print(int indent)
{
    PrintIndent(indent);

    cout<<"<删除语句";
    
    cout<<" 行号="<<lineno;

    cout<<">"<<endl;

    PrintChild(destTable,indent);

    PrintChild(condition,indent);

    PrintIndent(indent);
    
    cout<<"</删除语句>"<<endl;
}

//对删除语句对象进行语义分析
void ZYDeleteStmt::Analyze(ZYAnalyzer *analyzer)
{
    destTable->Analyze(analyzer);

    if(condition!=NULL)
    {
        condition->Analyze(analyzer);
    }

    analyzer->SetCurrentTable(NULL);
}

//执行删除语句对象
void ZYDeleteStmt::Execute(ZYExecutor *executor)
{
    IZYDatabase *db;

    IZYTable *t_dest;
    
    int i,j,a;

    bool b;

    ZYColumnRef *column;

    ZYValue *value;

    db=executor->GetCurrentDatabase();

    if(db==NULL)
    {
        cout<<"数据库未打开..."<<endl;
        return;
    }

    destTable->SetDestTable(executor);//设置目标表

    destTable->Execute(executor);//设置源表
    
    t_dest=executor->GetCurrentTable();

    if(t_dest==NULL)
    {
        cout<<"错误:表格\'"<<destTable->GetName()<<"\'不存在，执行取消"<<endl;
        return;
    }

    //如果表中没有数据,则直接返回
    if(t_dest->GetRowCount()==0)
    {
        return;
    }

    //如果是DELETE FROM <表名>型,直接清除全部元组
    if(condition==NULL)
    {
        t_dest->ClearRows();

        return;
    }
    
    //如果是DELETE FROM <表名> WHERE 编号=<元组编号>型,用二分查找定位元组
    if(condition->GetOperation()==ETokenType_EQ&&
       condition->GetLeftExpression()->GetType()==ESyntaxTreeType_ColumnRef&&
       condition->GetRightExpression()->GetType()==ESyntaxTreeType_Value)
    {
        column=(ZYColumnRef *)(condition->GetLeftExpression());
        
        column->Execute1(executor);

        if(column->IsPKeyColumn(executor))
        {
            value=(ZYValue *)(condition->GetRightExpression());

            value->GetResult(executor,&a);

            j=t_dest->FindColumn(column->GetName());

            i=t_dest->FindRow(j,&a);

            if(i!=-1)
            {
                t_dest->DeleteRow(i);
            }

            return;
        }
    }

    //其他类型的DELETE语句,这里从后面向前面扫描,避免删除时位置移动带来的错误
    t_dest->EnterBufferedMode();

    for(i=t_dest->GetRowCount()-1;i>=0;i--)
    {
        executor->SetCurrentItem(i);

        executor->SetSourceTableItem(0,i);

        condition->GetResult(executor,&b);

        if(b)
        {
            t_dest->DeleteRow(i);
        }
    }

    t_dest->LeaveBufferedMode();
}

