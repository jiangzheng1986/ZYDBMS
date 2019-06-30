//---------------------------------------------------------
//逐月查询引擎(ZYSQL)-语法分析 作者:蒋政
//---------------------------------------------------------
#include "ZYSQL1.HPP"

//构造语法分析器对象
ZYParser::ZYParser(ZYScanner *scanner)
{
    this->scanner=scanner;

    this->token=new ZYToken();

    this->tokenType=ETokenType_Unknown;

    this->tokenString=NULL;

    this->error=false;
}

//设置是否显示成功的标记匹配
void ZYParser::SetDisplayMatch(bool displayMatch)
{
    this->displayMatch=displayMatch;
}

//获取是否发生错误
bool ZYParser::IsError(void)
{
    return error;
}

//发生语法分析错误
void ZYParser::Error(void)
{
    this->error=true;
}

//获取下一个词法标记
void ZYParser::GetToken(void)
{
    scanner->GetToken(token);
    tokenType=token->GetTokenType();
    tokenString=token->GetTokenString();
}

//标记不匹配
void ZYParser::Dismatch(void)
{
    Error();

    scanner->SetDismatchFlag(token);

    token->PrintLocation();
    cout<<"语法错误:不匹配:";
    token->Print();
    cout<<endl;

    GetToken();
}

//尝试匹配标记
void ZYParser::Match(ETokenType tokenType)
{
    if(this->tokenType==tokenType)
    {
        if(displayMatch==true)
        {
            token->PrintLocation();
            token->Print();
            cout<<endl;
        }
        
        GetToken();
    }
    else
    {
        Error();

        scanner->SetDismatchFlag(token);

        token->PrintLocation();
        cout<<"语法错误:不匹配:";
        token->Print();
        
        cout<<" 期望:";
        ZYToken *token1=new ZYToken();
        token1->SetTokenType(tokenType);
        token1->SetLineNumber(token->GetLineNumber());
        token1->Print();
        delete token1;
        cout<<endl;     
    }
}

//获取当前行号
int ZYParser::GetLineNumber(void)
{
    return scanner->GetLineNumber();
}

//复制一个字符串,带字符串空间分配
char *ZYParser::CopyString(char *s)
{
    char *s1=new char[strlen(s)+1];
    strcpy(s1,s);
    return s1;
}

//执行语法分析,返回分析得到的语法树
ZYSyntaxTree *ZYParser::Parse(void)
{
    GetToken();

    syntaxTree=ParseZYSQL();

    return syntaxTree;
}

//递归下降分析
//符号说明: ->定义为
//          | 或者
//          []可选
//          {}重复
//          ()括号
//          //注释
//          ''转义
//ZYSQL                   ->[ZYSQLStmtSeq]
ZYGlobal *ZYParser::ParseZYSQL(void)
{
    int lineno=GetLineNumber();

    ZYGlobal *global=new ZYGlobal(lineno);

    if(tokenType!=ETokenType_Eof)
    {
        ParseZYSQLStmtSeq(global);
    }

    Match(ETokenType_Eof);

    return global;
}

//ZYSQLStmtSeq            ->ZYSQLStmt {[;] [ZYSQLStmt]}
void ZYParser::ParseZYSQLStmtSeq(ZYGlobal *global)
{
    global->AddChild(ParseZYSQLStmt());

    while(tokenType!=ETokenType_Eof)
    {
        if(tokenType==ETokenType_FenHao)
        {
            Match(ETokenType_FenHao);
        }

        if(tokenType!=ETokenType_Eof)
        {
            global->AddChild(ParseZYSQLStmt());
        }
    }
}

//ZYSQLStmt               ->CreateTable
//                         |CreateView
//                         |AlterTable
//                         |DropTable
//                         |DropView 
//                         |InsertStmt
//                         |SelectStmt
//                         |UpdateStmt
//                         |DeleteStmt
ZYSyntaxTree *ZYParser::ParseZYSQLStmt(void)
{
    ZYSyntaxTree *syntaxTree=NULL;

    switch(tokenType)
    {
    case ETokenType_Create:
        Match(ETokenType_Create);
        switch(tokenType)
        {
        case ETokenType_Table:
            syntaxTree=ParseCreateTable();
            break;
        case ETokenType_View:
            syntaxTree=ParseCreateView();
            break;
        }
        break;
    case ETokenType_Drop:
        Match(ETokenType_Drop);
        switch(tokenType)
        {
        case ETokenType_Table:
            syntaxTree=ParseDropTable();
            break;
        case ETokenType_View:
            syntaxTree=ParseDropView();
            break;
        }
        break;
    case ETokenType_Alter:
        syntaxTree=ParseAlterTable();
        break;
    case ETokenType_Insert:
        syntaxTree=ParseInsertStmt();
        break;
    case ETokenType_Select:
        syntaxTree=ParseSelectStmt();
        break;
    case ETokenType_Update:
        syntaxTree=ParseUpdateStmt();
        break;
    case ETokenType_Delete:
        syntaxTree=ParseDeleteStmt();
        break;
    default:
        Dismatch();
        break;
    }
    
    return syntaxTree;
}

//CreateTable             ->CREATE TABLE ID '(' [Declarations] ')'
ZYCreateTable *ZYParser::ParseCreateTable(void)
{
    char *name="";
    ZYCreateTable *createTable=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    int lineno=GetLineNumber();
    
    //Match(ETokenType_Create);

    Match(ETokenType_Table);

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

    Match(ETokenType_Id);

    createTable=new ZYCreateTable(name,lineno);

    Match(ETokenType_LeftParen);

    if(tokenType!=ETokenType_RightParen)
    {
        ParseDeclarations(createTable);
    }

    Match(ETokenType_RightParen);

	createTable->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    return createTable;
}

//CreateView              ->CREATE VIEW ID '(' Columns2 ')' AS SelectStmt
ZYCreateView *ZYParser::ParseCreateView(void)
{
    char *name="";
    ZYCreateView *createView=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    int lineno=GetLineNumber();
    
    //Match(ETokenType_Create);

    Match(ETokenType_View);

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    createView=new ZYCreateView(name,lineno);

    Match(ETokenType_LeftParen);

    ParseColumns2(createView);

    Match(ETokenType_RightParen);

    Match(ETokenType_As);

    createView->SetSelectStmt(ParseSelectStmt());

	createView->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    return createView;
}

//DropTable               ->DROP TABLE ID
ZYDropTable *ZYParser::ParseDropTable(void)
{
    char *name="";
    ZYDropTable *dropTable=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    
    int lineno=GetLineNumber();

    //Match(ETokenType_Drop);

    Match(ETokenType_Table);

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    dropTable=new ZYDropTable(name,lineno);

	dropTable->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    return dropTable;
}

//DropView                ->DROP VIEW ID
ZYDropView *ZYParser::ParseDropView(void)
{
    char *name="";
    ZYDropView *dropView=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    
    int lineno=GetLineNumber();

    //Match(ETokenType_Drop);

    Match(ETokenType_View);

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    dropView=new ZYDropView(name,lineno);

	dropView->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    return dropView;
}

//Declarations            ->Declaration {, Declaration}
void ZYParser::ParseDeclarations(ZYCreateTable *createTable)
{
    createTable->AddChild(ParseDeclaration(createTable));

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        createTable->AddChild(ParseDeclaration(createTable));
    }
}

//Declaration             ->ColumnDeclaration
//                         |PrimaryKeyDeclaration
//                         |ForeignKeyDeclaration
ZYSyntaxTree *ZYParser::ParseDeclaration(ZYCreateTable *createTable)
{
    ZYSyntaxTree *syntaxTree=NULL;

    switch(tokenType)
    {
    case ETokenType_Id:
        syntaxTree=ParseColumnDeclaration();
        break;
    case ETokenType_Primary:
    case ETokenType_PrimaryKey:
        ParsePrimaryKeyDeclaration(createTable);
        break;
    case ETokenType_Foreign:
    case ETokenType_ForeignKey:
        ParseForeignKeyDeclaration(createTable);
        break;
    }

    return syntaxTree;
}

//ColumnDeclaration       ->ID ColumnType
ZYColumnDecl *ZYParser::ParseColumnDeclaration(void)
{
    char *name="";
    ZYColumnDecl *columnDecl=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    columnDecl=new ZYColumnDecl(name,lineno);

	columnDecl->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    ParseColumnType(columnDecl);

    return columnDecl;
}

//ColumnType              ->INTEGER
//                         |(DOUBLE PRECISION|DOUBLE_PRECISION)
//                         |CHAR '(' NUM ')'
void ZYParser::ParseColumnType(ZYColumnDecl *columnDecl)
{
    int length=16;

    switch(tokenType)
    {
    case ETokenType_Integer:
        Match(ETokenType_Integer);
        columnDecl->SetDataType(EColumnType_Integer);
        columnDecl->SetDataLength(4);
        break;
    case ETokenType_Double:
    case ETokenType_DoublePrecision:
        if(tokenType==ETokenType_Double)
        {
            Match(ETokenType_Double);
            Match(ETokenType_Precision);
        }
        else
        {
            Match(ETokenType_DoublePrecision);
        }
        columnDecl->SetDataType(EColumnType_Double);
        columnDecl->SetDataLength(8);
        break;
    case ETokenType_Char:
        Match(ETokenType_Char);
        Match(ETokenType_LeftParen);
        if(tokenType==ETokenType_Num)
        {
            length=atoi(tokenString);
        }
        Match(ETokenType_Num);
        columnDecl->SetDataType(EColumnType_String);
        columnDecl->SetDataLength(length);
        Match(ETokenType_RightParen);
        break;
    default:
        Error();
        token->PrintLocation();
        cout<<"错误:列声明缺少类型定义..."<<endl;
        break;
    }
}

//PrimaryKeyDeclaration   ->(PRIMARY KEY|PRIMARY_KEY) '(' ID ')'
void ZYParser::ParsePrimaryKeyDeclaration(ZYCreateTable *createTable)
{
    char *primaryKey="";
	int primaryKeyTokenStart=0;
	int primaryKeyTokenEnd=0;

    if(tokenType==ETokenType_Primary)
    {
        Match(ETokenType_Primary);

        Match(ETokenType_Key);
    }
    else
    {
        Match(ETokenType_PrimaryKey);
    }

    Match(ETokenType_LeftParen);

    if(tokenType==ETokenType_Id)
    {
        primaryKey=CopyString(tokenString);
    }

	primaryKeyTokenStart=token->GetSourceIndex();

	primaryKeyTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    createTable->SetPrimaryKey(primaryKey);

	createTable->SetPrimaryKeyTokenLoc(primaryKeyTokenStart,primaryKeyTokenEnd);

    Match(ETokenType_RightParen);
}

//ForeignKeyDeclaration   ->(FOREIGN KEY|FOREIGN_KEY) '(' ID ')' REFERENCES ID '(' ID ')'
void ZYParser::ParseForeignKeyDeclaration(ZYCreateTable *createTable)
{
    char *name="";
    ZYForeignKey *foreignKey=NULL;
	char *foreignTable="";
	char *foreignColumn="";
    int lineno=GetLineNumber();
	int nameTokenStart=0;
	int nameTokenEnd=0;
	int foreignTableTokenStart=0;
	int foreignTableTokenEnd=0;
	int foreignColumnTokenStart=0;
	int foreignColumnTokenEnd=0;

    if(tokenType==ETokenType_Foreign)
    {
        Match(ETokenType_Foreign);

        Match(ETokenType_Key);
    }
    else
    {
        Match(ETokenType_ForeignKey);
    }

    Match(ETokenType_LeftParen);

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    foreignKey=new ZYForeignKey(name,lineno);

	foreignKey->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    Match(ETokenType_RightParen);

    Match(ETokenType_References);

	if(tokenType==ETokenType_Id)
    {
        foreignTable=CopyString(tokenString);
    }

	foreignKey->SetForeignTable(foreignTable);

	foreignTableTokenStart=token->GetSourceIndex();

	foreignTableTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    Match(ETokenType_LeftParen);

	if(tokenType==ETokenType_Id)
    {
        foreignColumn=CopyString(tokenString);
    }

	foreignKey->SetForeignColumn(foreignColumn);

	foreignColumnTokenStart=token->GetSourceIndex();

	foreignColumnTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    Match(ETokenType_RightParen);

	foreignKey->SetForeignTableTokenLoc(foreignTableTokenStart,foreignTableTokenEnd);

	foreignKey->SetForeignColumnTokenLoc(foreignColumnTokenStart,foreignColumnTokenEnd);

	createTable->AddChild(foreignKey);
}

//AlterTable              ->ALTER TABLE ID (AlterAdd|AlterModify|AlterDrop)
ZYAlterTable *ZYParser::ParseAlterTable(void)
{
    char *name="";

    ZYAlterTable *alterTable=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
	int lineno=GetLineNumber();

    Match(ETokenType_Alter);

    Match(ETokenType_Table);

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

	Match(ETokenType_Id);

    alterTable=new ZYAlterTable(name,lineno);

	alterTable->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    switch(tokenType)
    {
    case ETokenType_Add:
        ParseAlterAdd(alterTable);
        break;
    case ETokenType_Modify:
        ParseAlterModify(alterTable);
        break;
    case ETokenType_Drop:
        ParseAlterDrop(alterTable);
        break;
    default:
        Error();
        token->PrintLocation();
        cout<<"错误:后接增加、修改、撤销三者之一..."<<endl;
        break;
    }

    return alterTable;
}

//AlterAdd                ->ADD ColumnDeclaration
void ZYParser::ParseAlterAdd(ZYAlterTable *alterTable)
{
    alterTable->SetOperation(tokenType);

    Match(ETokenType_Add);

    alterTable->AddChild(ParseColumnDeclaration());
}

//AlterModify             ->MODIFY ColumnDeclaration
void ZYParser::ParseAlterModify(ZYAlterTable *alterTable)
{
    alterTable->SetOperation(tokenType);

    Match(ETokenType_Modify);

    alterTable->AddChild(ParseColumnDeclaration());
}

//AlterDrop               ->DROP ID
void ZYParser::ParseAlterDrop(ZYAlterTable *alterTable)
{
    alterTable->SetOperation(tokenType);

    Match(ETokenType_Drop);

    alterTable->AddChild(ParseColumn2());
}

//InsertStmt              ->INSERT INTO ID '(' Columns2 ')' VALUES RowValues
ZYInsertStmt *ZYParser::ParseInsertStmt(void)
{
    ZYInsertStmt *insertStmt=NULL;

    int lineno=GetLineNumber();

    insertStmt=new ZYInsertStmt(lineno);

    Match(ETokenType_Insert);

    Match(ETokenType_Into);

    insertStmt->SetDestTable(ParseTable1());

    Match(ETokenType_LeftParen);

    ParseColumns2(insertStmt);

    Match(ETokenType_RightParen);

    Match(ETokenType_Values);

    ParseRowValues(insertStmt);

    return insertStmt;
}

//Columns2                ->Column2 {, Column2}
void ZYParser::ParseColumns2(ZYSyntaxTree *syntaxTree)
{
    syntaxTree->AddChild(ParseColumn2());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        syntaxTree->AddChild(ParseColumn2());
    }
}

//Column2                 ->ID
ZYColumnRef *ZYParser::ParseColumn2(void)
{
    char *name="";
    ZYColumnRef *column=NULL;
	int nameTokenStart=0;
	int nameTokenEnd=0;
    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    column=new ZYColumnRef(name,lineno,"");

	column->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

    return column;
}

//RowValues               ->RowValue {, RowValue}
void ZYParser::ParseRowValues(ZYInsertStmt *insertStmt)
{
    insertStmt->AddChild(ParseRowValue());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        insertStmt->AddChild(ParseRowValue());
    }
}

//RowValue                ->'(' [Values] ')'
ZYRowValue *ZYParser::ParseRowValue(void)
{
    ZYRowValue *rowValue=NULL;

    int lineno=GetLineNumber();

    rowValue=new ZYRowValue(lineno);

    Match(ETokenType_LeftParen);

    if(tokenType!=ETokenType_RightParen)
    {
        ParseValues(rowValue);
    }

    Match(ETokenType_RightParen);

    return rowValue;
}

//Values                  ->Value {, Value}
void ZYParser::ParseValues(ZYRowValue *rowValue)
{
    rowValue->AddChild(ParseValue());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        rowValue->AddChild(ParseValue());
    }
}

//Value                   ->NUM
//                         |FLOAT
//                         |STRING
//                         |-NUM
//                         |-FLOAT
ZYValue *ZYParser::ParseValue(void)
{
    int a=0;
    double d=0.00;
    char *s="";
    ZYValue *value=NULL;

    int lineno=GetLineNumber();

    switch(tokenType)
    {
    case ETokenType_Num:
        if(tokenType==ETokenType_Num)
        {
            a=atoi(tokenString);
        }
        Match(ETokenType_Num);
        value=new ZYValue(lineno,EColumnType_Integer,&a);
        break;
    case ETokenType_Float:
        if(tokenType==ETokenType_Float)
        {
            d=atof(tokenString);
        }
        Match(ETokenType_Float);
        value=new ZYValue(lineno,EColumnType_Double,&d);
        break;
    case ETokenType_String:
        if(tokenType==ETokenType_String)
        {
            s=CopyString(tokenString);
        }
        Match(ETokenType_String);
        value=new ZYValue(lineno,EColumnType_String,s);
        break;
    case ETokenType_Minus:
        Match(ETokenType_Minus);
        switch(tokenType)
        {
        case ETokenType_Num:
            if(tokenType==ETokenType_Num)
            {
                a=-atoi(tokenString);
            }
            Match(ETokenType_Num);
            value=new ZYValue(lineno,EColumnType_Integer,&a);
            break;
        case ETokenType_Float:
            if(tokenType==ETokenType_Float)
            {
                d=-atof(tokenString);
            }
            Match(ETokenType_Float);
            value=new ZYValue(lineno,EColumnType_Double,&d);
            break;
        default:
            value=new ZYValue(lineno,EColumnType_Integer,&a);
            break;
        }
        break;
    default:
        value=new ZYValue(lineno,EColumnType_Integer,&a);
        break;
    }

    return value;
}

//SelectStmt              ->SELECT [ALL|DISTINCT] Columns FROM Tables
//                          [WHERE BoolExpression] 
//                          [GROUP BY GroupBys]
//                          [ORDER BY OrderBys]
ZYSelectStmt *ZYParser::ParseSelectStmt(void)
{
    ZYSelectStmt *selectStmt=NULL;

    int lineno=GetLineNumber();

    selectStmt=new ZYSelectStmt(lineno);

    Match(ETokenType_Select);

    if(tokenType==ETokenType_All||
       tokenType==ETokenType_Distinct)
    {
        switch(tokenType)
        {
        case ETokenType_All:
            Match(ETokenType_All);
            selectStmt->SetDistinct(false);
            break;
        case ETokenType_Distinct:
            Match(ETokenType_Distinct);
            selectStmt->SetDistinct(true);
            break;
        }
    }

    ParseColumns(selectStmt);

    Match(ETokenType_From);

    ParseTables(selectStmt);

    if(tokenType==ETokenType_Where)
    {
        Match(ETokenType_Where);

        selectStmt->SetCondition(ParseBoolExpression());
    }

    if(tokenType==ETokenType_Group)
    {
        Match(ETokenType_Group);

        Match(ETokenType_By);

        selectStmt->AddChild(ParseGroupBys());
    }

    if(tokenType==ETokenType_Order)
    {
        Match(ETokenType_Order);

        Match(ETokenType_By);

        ParseOrderBys(selectStmt);
    }

    return selectStmt;
}

//GroupBys                ->Column1 {,Column1} [HAVING BoolExpression]
ZYGroupBy *ZYParser::ParseGroupBys(void)
{
    ZYGroupBy *groupBy=NULL;

    int lineno=GetLineNumber();

    groupBy=new ZYGroupBy(lineno);

    groupBy->AddChild(ParseColumn1());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        groupBy->AddChild(ParseColumn1());
    }

    if(tokenType==ETokenType_Having)
    {
        Match(ETokenType_Having);

        groupBy->SetHavingCondition(ParseBoolExpression());
    }

    return groupBy;
}

//OrderBys                ->OrderBy {,OrderBy}
void ZYParser::ParseOrderBys(ZYSelectStmt *selectStmt)
{
    selectStmt->AddChild(ParseOrderBy());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        selectStmt->AddChild(ParseOrderBy());
    }
}

//OrderBy                 ->ID [ASC|DESC]
ZYOrderBy *ZYParser::ParseOrderBy(void)
{
    ESortOrder sortOrder=ESortOrder_Asc;
    char *name="";
    ZYOrderBy *orderBy=NULL;

    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

    Match(ETokenType_Id);

    switch(tokenType)
    {
    case ETokenType_Asc:
        sortOrder=ESortOrder_Asc;
        Match(ETokenType_Asc);
        break;
    case ETokenType_Desc:
        sortOrder=ESortOrder_Desc;
        Match(ETokenType_Desc);
        break;
    default:
        sortOrder=ESortOrder_Asc;
        break;
    }

    orderBy=new ZYOrderBy(name,lineno,sortOrder);

    return orderBy;
}

//UpdateStmt              ->UPDATE Table1 SET AssignExpressions [WHERE BoolExpression]
ZYUpdateStmt *ZYParser::ParseUpdateStmt(void)
{
    ZYUpdateStmt *updateStmt=NULL;

    int lineno=GetLineNumber();

    updateStmt=new ZYUpdateStmt(lineno);

    Match(ETokenType_Update);

    updateStmt->SetDestTable(ParseTable1());

    Match(ETokenType_Set);

    ParseAssignExpressions(updateStmt);

    if(tokenType==ETokenType_Where)
    {
        Match(ETokenType_Where);

        updateStmt->SetCondition(ParseBoolExpression());
    }

    return updateStmt;
}

//AssignExpressions       ->AssignExpression {, AssignExpression}
void ZYParser::ParseAssignExpressions(ZYUpdateStmt *updateStmt)
{
    updateStmt->AddChild(ParseAssignExpression());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        updateStmt->AddChild(ParseAssignExpression());
    }
}

//AssignExpression        ->Column1 = Expression 
ZYAssignExpression *ZYParser::ParseAssignExpression(void)
{
    ZYAssignExpression *assignExpression=NULL;

    int lineno=GetLineNumber();

    assignExpression=new ZYAssignExpression(lineno);

    assignExpression->SetDestColumn(ParseColumn1());

    Match(ETokenType_EQ);

    assignExpression->SetSrcExpression(ParseExpression());

    return assignExpression;
}

//DeleteStmt              ->DELETE FROM Table1 [WHERE BoolExpression]
ZYDeleteStmt *ZYParser::ParseDeleteStmt(void)
{
    ZYDeleteStmt *deleteStmt=NULL;

    int lineno=GetLineNumber();

    deleteStmt=new ZYDeleteStmt(lineno);

    Match(ETokenType_Delete);

    Match(ETokenType_From);

    deleteStmt->SetDestTable(ParseTable1());

    if(tokenType==ETokenType_Where)
    {
        Match(ETokenType_Where);

        deleteStmt->SetCondition(ParseBoolExpression());
    }

    return deleteStmt;
}

//Columns                 ->Column {, Column}
//                         |*
void ZYParser::ParseColumns(ZYSyntaxTree *syntaxTree)
{
    if(tokenType!=ETokenType_Multiply)
    {
        syntaxTree->AddChild(ParseColumn());

        while(tokenType==ETokenType_DouHao)
        {
            Match(ETokenType_DouHao);

            syntaxTree->AddChild(ParseColumn());
        }
    }
    else
    {
        Match(ETokenType_Multiply);
    }
}

//Column                  ->Expression [AS ID]
ZYSyntaxTree *ZYParser::ParseColumn(void)
{
	int asNameTokenStart=0;
	int asNameTokenEnd=0;
    char *asName="";

    ZYSyntaxTree *syntaxTree=NULL;

    ZYColumnRef *column=NULL;

    int lineno=GetLineNumber();
    
    syntaxTree=ParseExpression();
    
    if(syntaxTree->GetType()==ESyntaxTreeType_ColumnRef)
    {
        column=(ZYColumnRef *)syntaxTree;
    }
    else
    {
        column=new ZYColumnRef("",lineno,"");
        column->SetExpression(syntaxTree);
    }       

    if(tokenType==ETokenType_As)
    {
        Match(ETokenType_As);

        if(tokenType==ETokenType_Id)
        {
            asName=CopyString(tokenString);
        }

		asNameTokenStart=token->GetSourceIndex();

		asNameTokenEnd=token->GetSourceIndex1();

        Match(ETokenType_Id);

        column->SetAsName(asName);

		column->SetAsNameTokenLoc(asNameTokenStart,asNameTokenEnd);
    }

    return column;
}

//Column1                 ->ID [. ID]
ZYColumnRef *ZYParser::ParseColumn1(void)
{
    char *name="";
    char *tableName="";
	int nameTokenStart=0;
	int nameTokenEnd=0;
	int tableTokenStart=0;
	int tableTokenEnd=0;
    ZYColumnRef *column=NULL;

    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	nameTokenStart=token->GetSourceIndex();

	nameTokenEnd=token->GetSourceIndex1();
    
    Match(ETokenType_Id);

    if(tokenType==ETokenType_JvDian)
    {
        Match(ETokenType_JvDian);

        if(tokenType==ETokenType_Id)
        {
            tableName=name;

            name=CopyString(tokenString);
        }

		tableTokenStart=nameTokenStart;

		tableTokenEnd=nameTokenEnd;

		nameTokenStart=token->GetSourceIndex();

		nameTokenEnd=token->GetSourceIndex1();

        Match(ETokenType_Id);
    }

    column=new ZYColumnRef(name,lineno,tableName);

	column->SetNameTokenLoc(nameTokenStart,nameTokenEnd);

	column->SetTableTokenLoc(tableTokenStart,tableTokenEnd);

    return column;
}

//Tables                  ->Table {, Table}
void ZYParser::ParseTables(ZYSyntaxTree *syntaxTree)
{
    syntaxTree->AddChild(ParseTable());

    while(tokenType==ETokenType_DouHao)
    {
        Match(ETokenType_DouHao);

        syntaxTree->AddChild(ParseTable());
    }
}

//Table                   ->Table1
//                         |'(' SelectStmt ')' [AS ID]
//                         |SelectStmt [AS ID]
ZYSyntaxTree *ZYParser::ParseTable(void)
{
    ZYSyntaxTree *syntaxTree=NULL;
	int asNameTokenStart=0;
	int asNameTokenEnd=0;
    char *asName="";

    switch(tokenType)
    {
    case ETokenType_Id:
        syntaxTree=ParseTable1();
        break;
    case ETokenType_LeftParen:
        Match(ETokenType_LeftParen);
        syntaxTree=ParseSelectStmt();
        Match(ETokenType_RightParen);
        if(tokenType==ETokenType_As)
        {
            Match(ETokenType_As);

            if(tokenType==ETokenType_Id)
            {
                asName=CopyString(tokenString);
            }

			asNameTokenStart=token->GetSourceIndex();
		
			asNameTokenEnd=token->GetSourceIndex1();

            Match(ETokenType_Id);

            ((ZYSelectStmt *)syntaxTree)->SetAsName(asName);

			((ZYSelectStmt *)syntaxTree)->SetAsNameTokenLoc(asNameTokenStart,asNameTokenEnd);
        }
        break;
    case ETokenType_Select:
        syntaxTree=ParseSelectStmt();
        if(tokenType==ETokenType_As)
        {
            Match(ETokenType_As);

            if(tokenType==ETokenType_Id)
            {
                asName=CopyString(tokenString);
            }

			asNameTokenStart=token->GetSourceIndex();
		
			asNameTokenEnd=token->GetSourceIndex1();

            Match(ETokenType_Id);

            ((ZYSelectStmt *)syntaxTree)->SetAsName(asName);

			((ZYSelectStmt *)syntaxTree)->SetAsNameTokenLoc(asNameTokenStart,asNameTokenEnd);
        }
        break;
    }

    return syntaxTree;
}

//Table1                  ->ID [AS ID]
ZYTableRef *ZYParser::ParseTable1(void)
{
    char *name="";
    char *asName="";
    ZYTableRef *table=NULL;
	int tableTokenStart=0;
	int tableTokenEnd=0;
	int asNameTokenStart=0;
	int asNameTokenEnd=0;
    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Id)
    {
        name=CopyString(tokenString);
    }

	tableTokenStart=token->GetSourceIndex();

	tableTokenEnd=token->GetSourceIndex1();

    Match(ETokenType_Id);

    if(tokenType==ETokenType_As)
    {
        Match(ETokenType_As);

        if(tokenType==ETokenType_Id)
        {
            asName=CopyString(tokenString);
        }

		asNameTokenStart=token->GetSourceIndex();
		
		asNameTokenEnd=token->GetSourceIndex1();
    
        Match(ETokenType_Id);
    }

    table=new ZYTableRef(name,asName,lineno);

	table->SetTableTokenLoc(tableTokenStart,tableTokenEnd);

	table->SetAsNameTokenLoc(asNameTokenStart,asNameTokenEnd);

    return table;
}

//BoolExpression          ->BoolExpression1 {OR BoolExpression1}
ZYSyntaxTree *ZYParser::ParseBoolExpression(void)
{
    ZYBoolExpression *boolExpression=NULL;
    ZYSyntaxTree *expression1=NULL;
    ZYSyntaxTree *expression2=NULL;

    int lineno=GetLineNumber();

    expression1=ParseBoolExpression1();

    while(tokenType==ETokenType_Or)
    {
        Match(ETokenType_Or);

        expression2=ParseBoolExpression1();
        
        boolExpression=new ZYBoolExpression(lineno);
        boolExpression->SetOperation(ETokenType_Or);
        boolExpression->SetLeftExpression(expression1);
        boolExpression->SetRightExpression(expression2);
        expression1=boolExpression;
    }

    return expression1;
}

//BoolExpression1         ->BoolExpression2 {AND BoolExpression2}
ZYSyntaxTree *ZYParser::ParseBoolExpression1(void)
{
    ZYBoolExpression *boolExpression=NULL;
    ZYSyntaxTree *expression1=NULL;
    ZYSyntaxTree *expression2=NULL;

    int lineno=GetLineNumber();

    expression1=ParseBoolExpression2();

    while(tokenType==ETokenType_And)
    {
        Match(ETokenType_And);

        expression2=ParseBoolExpression2();

        boolExpression=new ZYBoolExpression(lineno);
        boolExpression->SetOperation(ETokenType_And);
        boolExpression->SetLeftExpression(expression1);
        boolExpression->SetRightExpression(expression2);
        expression1=boolExpression;
    }

    return expression1;
}

//BoolExpression2         ->[NOT] BoolExpression3
ZYSyntaxTree *ZYParser::ParseBoolExpression2(void)
{
    ZYBoolExpression *boolExpression=NULL;
    ZYSyntaxTree *expression1=NULL;
    bool hasNot=false;

    int lineno=GetLineNumber();
    
    if(tokenType==ETokenType_Not)
    {
        hasNot=true;
        Match(ETokenType_Not);
    }

    expression1=ParseBoolExpression3();

    if(hasNot)
    {
        boolExpression=new ZYBoolExpression(lineno);
        boolExpression->SetOperation(ETokenType_Not);
        boolExpression->SetLeftExpression(expression1);
        return boolExpression;
    }
    else
    {
        return expression1;
    }
}

//BoolExpression3         ->Expression [= Expression]
//                         |Expression != Expression
//                         |Expression < Expression
//                         |Expression > Expression
//                         |Expression <= Expression
//                         |Expression >= Expression
//                         |Expression [NOT] IN Expression
//                         |EXISTS Expression
ZYSyntaxTree *ZYParser::ParseBoolExpression3(void)
{
    ZYBoolExpression *boolExpression=NULL;
    ETokenType operation;
    ZYSyntaxTree *expression1=NULL;
    ZYSyntaxTree *expression2=NULL;

    int lineno=GetLineNumber();

    if(tokenType==ETokenType_Exists)
    {
        operation=tokenType;

        Match(ETokenType_Exists);

        expression1=ParseExpression();

        boolExpression=new ZYBoolExpression(lineno);
        boolExpression->SetOperation(operation);
        boolExpression->SetLeftExpression(expression1);

        return boolExpression;
    }
    
    expression1=ParseExpression();

    if( tokenType==ETokenType_EQ||
        tokenType==ETokenType_NE||
        tokenType==ETokenType_GT||
        tokenType==ETokenType_LT||
        tokenType==ETokenType_GE||
        tokenType==ETokenType_LE||
        tokenType==ETokenType_In||
        tokenType==ETokenType_Not||
        tokenType==ETokenType_NotIn)
    {
        operation=tokenType;

        switch(tokenType)
        {
        case ETokenType_EQ:
            Match(ETokenType_EQ);
            break;
        case ETokenType_NE:
            Match(ETokenType_NE);
            break;
        case ETokenType_GT:
            Match(ETokenType_GT);
            break;
        case ETokenType_LT:
            Match(ETokenType_LT);
            break;
        case ETokenType_GE:
            Match(ETokenType_GE);
            break;
        case ETokenType_LE:
            Match(ETokenType_LE);
            break;
        case ETokenType_In:
            Match(ETokenType_In);
            break;
        case ETokenType_Not:
            Match(ETokenType_Not);
            Match(ETokenType_In);
            break;
        case ETokenType_NotIn:
            Match(ETokenType_NotIn);
            break;
        }

        expression2=ParseExpression();
    }

    if(expression2!=NULL)
    {
        boolExpression=new ZYBoolExpression(lineno);
        if(operation==ETokenType_Not)
        {
            boolExpression->SetOperation(ETokenType_NotIn);
        }
        else
        {
            boolExpression->SetOperation(operation);
        }
        boolExpression->SetLeftExpression(expression1);
        boolExpression->SetRightExpression(expression2);
        return boolExpression;
    }
    else
    {
        return expression1;
    }
}

//Expression              ->Expression1 {+ Expression1}
//                         |Expression1 {- Expression1}
ZYSyntaxTree *ZYParser::ParseExpression(void)
{
    ZYExpression *expression=NULL;
    ETokenType operation;
    ZYSyntaxTree *expression1=NULL;
    ZYSyntaxTree *expression2=NULL;

    int lineno=GetLineNumber();

    expression1=ParseExpression1();

    while(tokenType==ETokenType_Plus||
        tokenType==ETokenType_Minus)
    {
        operation=tokenType;

        switch(tokenType)
        {
        case ETokenType_Plus:
            Match(ETokenType_Plus);
            break;
        case ETokenType_Minus:
            Match(ETokenType_Minus);
            break;
        }

        expression2=ParseExpression1();
        
        expression=new ZYExpression(lineno);
        expression->SetOperation(operation);
        expression->SetLeftExpression(expression1);
        expression->SetRightExpression(expression2);
        expression1=expression;
    }

    return expression1;
}

//Expression1             ->Expression2 {* Expression2}
//                         |Expression2 {/ Expression2}
ZYSyntaxTree *ZYParser::ParseExpression1(void)
{
    ZYExpression *expression=NULL;
    ETokenType operation;
    ZYSyntaxTree *expression1=NULL;
    ZYSyntaxTree *expression2=NULL;

    int lineno=GetLineNumber();

    expression1=ParseExpression2();

    while(tokenType==ETokenType_Multiply||
        tokenType==ETokenType_Divide)
    {
        operation=tokenType;

        switch(tokenType)
        {
        case ETokenType_Multiply:
            Match(ETokenType_Multiply);
            break;
        case ETokenType_Divide:
            Match(ETokenType_Divide);
            break;
        }

        expression2=ParseExpression2();
        
        expression=new ZYExpression(lineno);
        expression->SetOperation(operation);
        expression->SetLeftExpression(expression1);
        expression->SetRightExpression(expression2);
        expression1=expression;
    }

    return expression1;
}

//Expression2             ->Expression3
//                         |COUNT '(' Column1 ')'
//                         |SUM '(' Column1 ')'
//                         |AVG '(' Column1 ')'
//                         |MAX '(' Column1 ')'
//                         |MIN '(' Column1 ')'
ZYSyntaxTree *ZYParser::ParseExpression2(void)
{
    ZYAggregateFunction *aggregateFunction=NULL;

    int lineno=GetLineNumber();

    switch(tokenType)
    {
    case ETokenType_Count:
    case ETokenType_Sum:
    case ETokenType_Avg:
    case ETokenType_Max:
    case ETokenType_Min:
        aggregateFunction=new ZYAggregateFunction(lineno);
        aggregateFunction->SetFunction(tokenType);
        Match(tokenType);
        Match(ETokenType_LeftParen);
        aggregateFunction->SetColumn(ParseColumn1());
        Match(ETokenType_RightParen);
        return aggregateFunction;
    default:
        return ParseExpression3();
    }   
}

//Expression3             ->Column1
//                         |SelectStmt
//                         |NUM
//                         |FLOAT
//                         |STRING
//                         |'(' Expression ')'
ZYSyntaxTree *ZYParser::ParseExpression3(void)
{
    int a=0;
    ZYSyntaxTree *syntaxTree=NULL;

    int lineno=GetLineNumber();

    switch(tokenType)
    {
    case ETokenType_Id:
        syntaxTree=ParseColumn1();
        break;
    case ETokenType_Select:
        syntaxTree=ParseSelectStmt();
        break;
    case ETokenType_Num:
    case ETokenType_Float:
    case ETokenType_String:
        syntaxTree=ParseValue();
        break;
    case ETokenType_LeftParen:
        Match(ETokenType_LeftParen);
        syntaxTree=ParseBoolExpression();
        Match(ETokenType_RightParen);
        break;
    default:
        syntaxTree=new ZYValue(lineno,EColumnType_Integer,&a);
        break;
    }

    return syntaxTree;
}