//---------------------------------------------------------
//逐月查询引擎(ZYSQL) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYSQL1_HPP)        //测试ZYSQL_HPP_宏
#define ZYSQL1_HPP              //使得ZYSQL.HPP仅包含1次

//引入32位Windows API库
#include <windows.h>

//引入C++输入输出库
#include <iostream>

//引入C++文件库
#include <fstream>

//引入C++泛型算法库
#include <algorithm>

//引入C++泛型向量库
#include <vector>

//引入存储引擎(ZYRDB)API库
#include <ZYRDB.HPP>

//引入查询引擎(ZYSQL)API库
#include "ZYSQL.HPP"

//应用C++标准库名称空间(std)
using namespace std;

//词法标记类型
typedef enum
{
    //关键字:
    ETokenType_Add,             //ADD 增加
    ETokenType_All,             //ALL 全部
    ETokenType_Alter,           //ALTER 改变
    ETokenType_And,             //AND 与
    ETokenType_As,              //AS 为
    ETokenType_Asc,             //ASC 升序
    ETokenType_Avg,             //AVG 平均值
    ETokenType_By,              //BY 按
    ETokenType_Char,            //CHAR 字符
    ETokenType_Count,           //COUNT 计数
    ETokenType_Create,          //CREATE 创建
    ETokenType_Delete,          //DELETE 删除
    ETokenType_Desc,            //DESC 降序
    ETokenType_Distinct,        //DISTINCT 互异
    ETokenType_Double,          //DOUBLE
    ETokenType_DoublePrecision, //小数
    ETokenType_Drop,            //DROP 撤销
    ETokenType_Exists,          //EXISTS 存在
    ETokenType_Foreign,         //FOREIGN
    ETokenType_ForeignKey,      //外键
    ETokenType_From,            //FROM 从
    ETokenType_Group,           //GROUP 分组
    ETokenType_Having,          //HAVING 仅当
    ETokenType_In,              //IN 在内
    ETokenType_Insert,          //INSERT 插入
    ETokenType_Integer,         //INTEGER 整数
    ETokenType_Into,            //INTO 到
    ETokenType_Key,             //KEY
    ETokenType_Max,             //MAX 最大值
    ETokenType_Min,             //MIN 最小值
    ETokenType_Modify,          //MODIFY 修改
    ETokenType_Not,             //NOT 非
    ETokenType_NotIn,           //NOT IN 不在内
    ETokenType_Or,              //OR 或
    ETokenType_Order,           //ORDER 排序
    ETokenType_Precision,       //PRECISION
    ETokenType_Primary,         //PRIMARY
    ETokenType_PrimaryKey,      //主键
    ETokenType_References,      //REFERENCES 引用
    ETokenType_Select,          //SELECT 选择
    ETokenType_Set,             //SET 使
    ETokenType_Sum,             //SUM 求和
    ETokenType_Table,           //TABLE 表格
    ETokenType_Update,          //UPDATE 修改
    ETokenType_Values,          //VALUES 值表
    ETokenType_View,            //VIEW 视图
    ETokenType_Where,           //WHERE 当  
    //标点符号:
    ETokenType_DouHao,          //逗号 ,
    ETokenType_JvDian,          //句点 .
    ETokenType_LeftParen,       //左括号 (
    ETokenType_RightParen,      //右括号 )
    ETokenType_FenHao,          //分号 ;
    ETokenType_EQ,              //等于 =
    ETokenType_NE,              //不等于 !=
    ETokenType_GT,              //大于 >
    ETokenType_LT,              //小于 <
    ETokenType_GE,              //大于等于 >=
    ETokenType_LE,              //小于等于 <=
    ETokenType_Plus,            //加号 +
    ETokenType_Minus,           //减号 -
    ETokenType_Multiply,        //乘号 *
    ETokenType_Divide,          //除号 /
    //其他符号
    ETokenType_Id,              //标识符
    ETokenType_Num,             //整数
    ETokenType_Float,           //小数
    ETokenType_String,          //字符串
    ETokenType_Unknown,         //未知符号
    ETokenType_Eof              //文件结束
}ETokenType;

//词法扫描状态
typedef enum
{
    EStateType_Start,           //开始
    EStateType_InId,            //在标识符中
    EStateType_InNum,           //在数中
    EStateType_InString,        //在字符串中
    EStateType_InNE,            //在不等于中
    EStateType_InGE,            //在大于等于中
    EStateType_InLE,            //在小于等于中
    EStateType_InComment,       //在注释中1
    EStateType_InComment1,      //在注释中2
    EStateType_Done             //完成
}EStateType;

//语法树类型
typedef enum
{
    ESyntaxTreeType_Unknown,    //未知语法树
    ESyntaxTreeType_Global,     //全局
    ESyntaxTreeType_CreateTable,//创建表格
    ESyntaxTreeType_CreateView, //创建视图
    ESyntaxTreeType_DropTable,  //撤消表格
    ESyntaxTreeType_DropView,   //撤消视图
    ESyntaxTreeType_ColumnDecl, //列声明
    ESyntaxTreeType_AlterTable, //改变表格
    ESyntaxTreeType_ForeignKey, //外键声明
    ESyntaxTreeType_SelectStmt, //选择语句
    ESyntaxTreeType_ColumnRef,  //列引用
    ESyntaxTreeType_TableRef,   //表格引用
    ESyntaxTreeType_OrderBy,    //按列排序
    ESyntaxTreeType_GroupBy,    //按列分组
    ESyntaxTreeType_InsertStmt, //插入语句
    ESyntaxTreeType_UpdateStmt, //更新语句
    ESyntaxTreeType_DeleteStmt, //删除语句
    ESyntaxTreeType_AssignExpression,
                                //赋值语句
    ESyntaxTreeType_BoolExpression,
                                //布尔表达式
    ESyntaxTreeType_AggregateFunction,
                                //聚合函数
    ESyntaxTreeType_Expression, //表达式
    ESyntaxTreeType_RowValue,   //数据行
    ESyntaxTreeType_Value       //单个数据
}ESyntaxTreeType;

//运算结果类型
typedef enum
{
    EResultType_String=0,       //字符串=0
    EResultType_Double=1,       //双精度浮点=1
    EResultType_Integer=2,      //整数=2
    EResultType_Boolean,        //布尔值
    EResultType_Table           //表格
}EResultType;

//性能监视对象
class ZYProfiler;

//查询执行器对象
class ZYExecutor;

//语法树对象
class ZYSyntaxTree;

//全局对象
class ZYGlobal;

//创建表格对象
class ZYCreateTable;

//外键声明对象
class ZYForeignKey;

//撤消表格对象
class ZYDropTable;

//撤消视图对象
class ZYDropView;

//创建视图对象
class ZYCreateView;

//列声明对象
class ZYColumnDecl;

//改变表格对象
class ZYAlterTable;

//选择语句对象
class ZYSelectStmt;

//列引用对象
class ZYColumnRef;

//表格引用对象
class ZYTableRef;

//按列排序对象
class ZYOrderBy;

//按列分组对象
class ZYGroupBy;

//插入语句对象
class ZYInsertStmt;

//更新语句对象
class ZYUpdateStmt;

//删除语句对象
class ZYDeleteStmt;

//赋值语句对象
class ZYAssignExpression;

//布尔表达式对象
class ZYBoolExpression;

//聚合函数表达式对象
class ZYAggregateFunction;

//表达式对象
class ZYExpression;

//数据行对象
class ZYRowValue;

//单个数据对象
class ZYValue;

//逐月查询引擎(ZYSQL)对象
class ZYCompiler:public IZYCompiler
{
public:
    //构造逐月查询引擎(ZYSQL)对象
    ZYCompiler(char *listingfile,IZYDatabase *database);
    //析构逐月查询编译器对象
    ZYCompiler::~ZYCompiler(void);
    //加载结构化查询语言(ZYSQL)语句
    static char *LoadSQL(char *filename);
    //释放结构化查询语言(ZYSQL)语句
    static void FreeSQL(char *s);
    //检查逐月结构化查询语言(ZYSQL)语句
    void CheckSQL(char *sql,char *format);
    //执行逐月结构化查询语言(ZYSQL)语句
    void ExecuteSQL(char *sql);
    //获取查询执行结果表格
    IZYTable *GetResultTable(void);
    //设置是否显示源程序
    void SetDisplaySource(bool displaySource);
    //设置是否显示匹配符号
    void SetDisplayMatch(bool displayMatch);
    //设置是否显示抽象语法树
    void SetDisplayAST(bool displayAST);
    //设置是否显示优化语法树
    void SetDisplayOST(bool displayOST);
    //设置是否显示数据表格
    void SetDisplayTables(bool displayTables);
    //设置是否显示查询耗时
    void SetDisplayTime(bool displayTime);
    //设置是否显示结果表格
    void SetDisplayResult(bool displayResult);
private:
    //当前数据库
    IZYDatabase *database;
    //当前表格
    IZYTable *table;
    //性能监视器
    ZYProfiler *profiler;
    //列表文件
    ofstream *listing;
    //是否列表源程序
    bool displaySource;
    //是否显示匹配符号
    bool displayMatch;
    //是否显示抽象语法树
    bool displayAST;
    //是否显示优化语法树
    bool displayOST;
    //是否显示数据表格
    bool displayTables;
    //是否显示查询耗时
    bool displayTime;
    //是否显示结果表格
    bool displayResult;
};

//性能监视对象
class ZYProfiler:public IZYProfiler
{
public:
    //构造性能监视对象
    ZYProfiler(void);
    //性能监视对象性能监视开始
    void Start(void);
    //性能监视对象性能监视结束
    void End(void);
    //获取性能监视对象监视到的执行时间(秒)
    float GetTime(void);
    //获取性能监视对象监视到的执行时间(毫秒)
    int GetTimeMs(void);
private:
    //计数频率
    LARGE_INTEGER l1;
    //计数开始值
    LARGE_INTEGER l2;
    //计数结束值
    LARGE_INTEGER l3;
    //间隔时间(秒)
    float time;
};

//源程序对象
class ZYSource
{
public:
    //构造源程序对象
    ZYSource(char *sourceString,char *formatString);
    //获取一个字符
    char GetChar(void);
    //回退一个字符
    void UngetChar(void);
    //获取当前索引
    int GetSourceIndex(void);
    //获取当前行数
    int GetLineNumber(void);
    //获取当前列数
    int GetColumnNumber(void);
    //是否在执行语法检查
    bool CheckingSQL(void);
    //设置源程序字符类型
    void SetSourceCharType(int i1,int i2,ESourceCharType type);
    //设置不匹配标志
    void SetDismatchFlag(int i1,int i2);
    //设置已确认标志
    void SetAcknowledgedFlag(int i1,int i2);
    //设置是否显示源代码行
    void SetDisplaySource(bool displaySource);
    //获取是否发生错误
    bool IsError(void);
private:
    //显示一行源代码
    void DisplayLine(void);
    //源代码字符串
    char *sourceString;
    //源代码字符类型
    char *formatString;
    //源代码当前位置索引
    int sourceIndex;        
    //当前行号
    int lineNumber;         
    //当前列号
    int columnNumber;           
    //是否显示源代码行
    bool displaySource;     
    //回退字符时回退了一行
    bool ungetedALine;
    //是否发生错误
    bool error;             
};

//词法标记字符串最大长度
#define TOKEN_STRING_LEN 1001

//词法标记对象
class ZYToken
{
public:
    //构造词法标记对象
    ZYToken(void);
    //重置词法标记对象
    void Reset(void);
    //向词法标记字符串增加字符
    void AddChar(char ch);
    //获取词法标记对象词法标记类型
    ETokenType GetTokenType(void);
    //设置词法标记对象词法标记类型
    void SetTokenType(ETokenType tokenType);
    //获取词法标记对象词法标记字符串
    char *GetTokenString(void);
    //转换关键字词法标记对象
    void TranslateToken(void);
    //设置词法标记对象开始索引
    void SetSourceIndex(int sourceIndex);
    //获取词法标记对象开始索引
    int GetSourceIndex(void);
    //设置词法标记对象开始行号
    void SetLineNumber(int lineNumber);
    //获取词法标记对象开始行号
    int GetLineNumber(void);
    //设置词法标记对象开始列号
    void SetColumnNumber(int columnNumber);
    //获取词法标记对象开始列号
    int GetColumnNumber(void);
    //设置词法标记对象结束索引
    void SetSourceIndex1(int sourceIndex1);
    //获取词法标记对象结束索引
    int GetSourceIndex1(void);
    //设置词法标记对象结束行号
    void SetLineNumber1(int lineNumber1);
    //获取词法标记对象结束行号
    int GetLineNumber1(void);
    //设置词法标记对象结束列号
    void SetColumnNumber1(int columnNumber1);
    //获取词法标记对象结束列号
    int GetColumnNumber1(void);
    //打印词法标记对象位置
    void PrintLocation(void);
    //打印词法标记对象
    void Print(void);
private:
    //词法标记类型
    ETokenType tokenType;   
    //词法标记字符串
    char tokenString[TOKEN_STRING_LEN];
    //词法标记字符串当前字符索引
    int tokenStringIndex;   
    //词法标记对象开始索引
    int sourceIndex;         
    //词法标记对象开始行号
    int lineNumber;         
    //词法标记对象开始列号
    int columnNumber;       
    //词法标记对象结束索引
    int sourceIndex1;
    //词法标记对象结束行号
    int lineNumber1;        
    //词法标记对象结束列号
    int columnNumber1;      
};

//词法扫描器对象
class ZYScanner
{
public:
    //构造词法扫描器对象
    ZYScanner(ZYSource *source);
    //获取当前行号
    int GetLineNumber(void);
    //设置不匹配标志
    void SetDismatchFlag(ZYToken *token);
    //获取是否发生错误
    bool IsError(void);
    //获取下一个词法标记
    void GetToken(ZYToken *token);
private:
    //设置标记的开始位置
    void SetLocation(ZYToken *token);
    //设置标记的结束位置
    void SetLocation1(ZYToken *token);
    //设置源程序字符类型
    void SetSourceCharType(ZYToken *token);
    //相应的源代码对象
    ZYSource *source;
    //是否发生错误
    bool error;
};

//语法分析器对象
class ZYParser
{
public:
    //构造语法分析器对象
    ZYParser(ZYScanner *scanner);
    //设置是否显示成功的标记匹配
    void SetDisplayMatch(bool displayMatch);
    //获取是否发生错误
    bool IsError(void);
    //执行语法分析,返回分析得到的语法树
    ZYSyntaxTree *Parse(void);
    //语法分析器对象测试程序
    static void Test(char *sql,int db);
    //复制一个字符串,带字符串空间分配
    static char *CopyString(char *s);
private:    
    //发生语法分析错误
    void Error(void);
    //获取下一个词法标记
    void GetToken(void);
    //标记不匹配
    void Dismatch(void);
    //尝试匹配标记
    void Match(ETokenType tokenType);
    //获取当前行号
    int GetLineNumber(void);
    //递归下降分析
    //符号说明: ->定义为
    //          | 或者
    //          []可选
    //          {}重复
    //          ()括号
    //          //注释
    //          ''转义
    //ZYSQL                   ->[ZYSQLStmtSeq]
    ZYGlobal *ParseZYSQL(void);
    //ZYSQLStmtSeq            ->ZYSQLStmt {[;] [ZYSQLStmt]}
    void ParseZYSQLStmtSeq(ZYGlobal *global);
    //ZYSQLStmt               ->CreateTable
    //                         |CreateView
    //                         |AlterTable
    //                         |DropTable
    //                         |DropView 
    //                         |InsertStmt
    //                         |SelectStmt
    //                         |UpdateStmt
    //                         |DeleteStmt
    ZYSyntaxTree *ParseZYSQLStmt(void);
    //CreateTable             ->CREATE TABLE ID '(' [Declarations] ')'
    ZYCreateTable *ParseCreateTable(void);
    //CreateView              ->CREATE VIEW ID '(' Columns2 ')' AS SelectStmt
    ZYCreateView *ParseCreateView(void);
    //DropTable               ->DROP TABLE ID
    ZYDropTable *ParseDropTable(void);
    //DropView                ->DROP VIEW ID
    ZYDropView *ParseDropView(void);
    //Declarations            ->Declaration {, Declaration}
    void ParseDeclarations(ZYCreateTable *createTable);
    //Declaration             ->ColumnDeclaration
    //                         |PrimaryKeyDeclaration
    //                         |ForeignKeyDeclaration
    ZYSyntaxTree *ParseDeclaration(ZYCreateTable *createTable);
    //ColumnDeclaration       ->ID ColumnType
    ZYColumnDecl *ParseColumnDeclaration(void);
    //ColumnType              ->INTEGER
    //                         |(DOUBLE PRECISION|DOUBLE_PRECISION)
    //                         |CHAR '(' NUM ')'
    void ParseColumnType(ZYColumnDecl *columnDecl);
    //PrimaryKeyDeclaration   ->(PRIMARY KEY|PRIMARY_KEY) '(' ID ')'
    void ParsePrimaryKeyDeclaration(ZYCreateTable *createTable);
    //ForeignKeyDeclaration   ->(FOREIGN KEY|FOREIGN_KEY) '(' ID ')' REFERENCES ID '(' ID ')'
    void ParseForeignKeyDeclaration(ZYCreateTable *createTable);
    //AlterTable              ->ALTER TABLE ID (AlterAdd|AlterModify|AlterDrop)
    ZYAlterTable *ParseAlterTable(void);
    //AlterAdd                ->ADD ColumnDeclaration
    void ParseAlterAdd(ZYAlterTable *alterTable);
    //AlterModify             ->MODIFY ColumnDeclaration
    void ParseAlterModify(ZYAlterTable *);
    //AlterDrop               ->DROP Column2
    void ParseAlterDrop(ZYAlterTable *);
    //InsertStmt              ->INSERT INTO ID '(' Columns2 ')' VALUES RowValues
    ZYInsertStmt *ParseInsertStmt(void);
    //Columns2                ->Column2 {, Column2}
    void ParseColumns2(ZYSyntaxTree *syntaxTree);
    //Column2                 ->ID
    ZYColumnRef *ParseColumn2(void);
    //RowValues               ->RowValue {, RowValue}
    void ParseRowValues(ZYInsertStmt *insertStmt);
    //RowValue                ->'(' [Values] ')'
    ZYRowValue *ParseRowValue(void);
    //Values                  ->Value {, Value}
    void ParseValues(ZYRowValue *rowValue);
    //Value                   ->NUM
    //                         |FLOAT
    //                         |STRING
    //                         |-NUM
    //                         |-FLOAT
    ZYValue *ParseValue(void);
    //SelectStmt              ->SELECT [ALL|DISTINCT] Columns FROM Tables
    //                          [WHERE BoolExpression] 
    //                          [GROUP BY GroupBys]
    //                          [ORDER BY OrderBys]
    ZYSelectStmt *ParseSelectStmt(void);
    //GroupBys                ->Column1 {,Column1} [HAVING BoolExpression]
    ZYGroupBy *ParseGroupBys(void);
    //OrderBys                ->OrderBy {,OrderBy}
    void ParseOrderBys(ZYSelectStmt *selectStmt);
    //OrderBy                 ->ID [ASC|DESC]
    ZYOrderBy *ParseOrderBy(void);
    //UpdateStmt              ->UPDATE Table1 SET AssignExpressions [WHERE BoolExpression]
    ZYUpdateStmt *ParseUpdateStmt(void);
    //AssignExpressions       ->AssignExpression {, AssignExpression}
    void ParseAssignExpressions(ZYUpdateStmt *updateStmt);
    //AssignExpression        ->Column1 = Expression 
    ZYAssignExpression *ParseAssignExpression(void);
    //DeleteStmt              ->DELETE FROM Table1 [WHERE BoolExpression]
    ZYDeleteStmt *ParseDeleteStmt(void);
    //Columns                 ->Column {, Column}
    //                         |*
    void ParseColumns(ZYSyntaxTree *syntaxTree);
    //Column                  ->Expression [AS ID]
    ZYSyntaxTree *ParseColumn(void);
    //Column1                 ->ID [. ID]
    ZYColumnRef *ParseColumn1(void);
    //Tables                  ->Table {, Table}
    void ParseTables(ZYSyntaxTree *syntaxTree);
    //Table                   ->Table1
    //                         |'(' SelectStmt ')' [AS ID]
    //                         |SelectStmt [AS ID]
    ZYSyntaxTree *ParseTable(void);
    //Table1                  ->ID [AS ID]
    ZYTableRef *ParseTable1(void);
    //BoolExpression          ->BoolExpression1 {OR BoolExpression1}
    ZYSyntaxTree *ParseBoolExpression(void);
    //BoolExpression1         ->BoolExpression2 {AND BoolExpression2}
    ZYSyntaxTree *ParseBoolExpression1(void);
    //BoolExpression2         ->[NOT] BoolExpression3
    ZYSyntaxTree *ParseBoolExpression2(void);
    //BoolExpression3         ->Expression [= Expression]
    //                         |Expression != Expression
    //                         |Expression < Expression
    //                         |Expression > Expression
    //                         |Expression <= Expression
    //                         |Expression >= Expression
    //                         |Expression [NOT] IN Expression
    //                         |EXISTS Expression
    ZYSyntaxTree *ParseBoolExpression3(void);
    //Expression              ->Expression1 {+ Expression1}
    //                         |Expression1 {- Expression1}
    ZYSyntaxTree *ParseExpression(void);
    //Expression1             ->Expression2 {* Expression2}
    //                         |Expression2 {/ Expression2}
    ZYSyntaxTree *ParseExpression1(void);
    //Expression2             ->Expression3
    //                         |COUNT '(' Column1 ')'
    //                         |SUM '(' Column1 ')'
    //                         |AVG '(' Column1 ')'
    //                         |MAX '(' Column1 ')'
    //                         |MIN '(' Column1 ')'
    ZYSyntaxTree *ParseExpression2(void);
    //Expression3             ->Column1
    //                         |SelectStmt
    //                         |NUM
    //                         |FLOAT
    //                         |STRING
    //                         |'(' Expression ')'
    ZYSyntaxTree *ParseExpression3(void);
    //词法分析器对象
    ZYScanner *scanner;
    //当前词法标记
    ZYToken *token;
    //当前词法标记类型
    ETokenType tokenType;
    //当前词法标记字符串
    char *tokenString;
    //抽象语法树
    ZYSyntaxTree *syntaxTree;
    //是否显示成功的标记匹配
    bool displayMatch;
    //是否发生错误
    bool error;
};

//查询分析器对象
class ZYAnalyzer
{
public:
    //构造查询分析器对象
    ZYAnalyzer(ZYAnalyzer *analyzer);
    //设置源程序对象
    void SetSource(ZYSource *source);
    //获取父查询分析器
    ZYAnalyzer *GetParentAnalyzer(void);
    //设置父父查询分析器
    void SetParentAnalyzer(ZYAnalyzer *parentAnalyzer);
    //获取源程序对象
    ZYSource *GetSource(void);
    //设置当前执行语句
    void SetCurrentStmt(ZYSyntaxTree *currentStmt);
    //获取当前执行语句
    ZYSyntaxTree *GetCurrentStmt(void);
    //设置当前数据库
    void SetCurrentDatabase(IZYDatabase *currentDatabase);
    //获取当前数据库
    IZYDatabase *GetCurrentDatabase(void);
    //设置当前表格
    void SetCurrentTable(IZYTable *currentTable);
    //获取当前表格
    IZYTable *GetCurrentTable(void);
    //清空源表列表
    void ClearSourceTables(void);
    //加入源表列表表格
    void AddSourceTable(ZYTableRef *tableRef);
    //获取源表列表表格
    ZYTableRef *GetSourceTable(int index);
    //获取源表列表表格计数
    int GetSourceTableCount(void);
    //获取是否发生错误
    bool IsError(void);
    //发生语义分析错误
    void Error(void);
    //执行语义分析
    void Analyze(ZYSyntaxTree *syntaxTree);
private:
    //查询分析器对象
    ZYSource *source;
    //当前数据库
    IZYDatabase *currentDatabase;
    //当前表格对象
    IZYTable *currentTable;
    //当前执行语句
    ZYSyntaxTree *currentStmt;
    //源表列表
    ZYTableRef *sourceTables[100];
    //源表计数
    int sourceTableCount;
    //父查询分析器
    ZYAnalyzer *parentAnalyzer;
    //是否发生错误
    bool error;
};

//查询执行器对象
class ZYExecutor
{
public:
    //构造查询执行器对象
    ZYExecutor(ZYExecutor *parentExecutor,IZYDatabase *database,ZYSyntaxTree *syntaxTree);
    //析构查询执行器对象
    ~ZYExecutor(void);
    //获取父查询执行器
    ZYExecutor *GetParentExecutor(void);
    //设置当前选择查询语句
    void SetCurrentSelectStmt(ZYSelectStmt *currentSelectStmt);
    //获取当前选择查询语句
    ZYSelectStmt *GetCurrentSelectStmt(void);
    //设置当前数据库
    void SetCurrentDatabase(IZYDatabase *currentDatabase);
    //获取当前数据库
    IZYDatabase *GetCurrentDatabase(void);
    //设置临时表编号
    void SetTempTableNo(int tempTableNo);
    //分配一个临时表编号
    int AllocateTempTableNo(void);
    //设置临时列编号
    void SetTempColumnNo(int tempTableNo);
    //分配一个临时列编号
    int AllocateTempColumnNo(void);
    //设置当前表格
    void SetCurrentTable(IZYTable *currentTable);
    //获取当前表格
    IZYTable *GetCurrentTable(void);
    //设置当前主键
    void SetPrimaryKey(char *primaryKey);
    //获取当前主键
    char *GetPrimaryKey(void);
    //设置当前表项
    void SetCurrentItem(int currentItem);
    //获取当前表项
    int GetCurrentItem(void);
    //清空源表列表
    void ClearSourceTables(void);
    //加入源表列表表格
    void AddSourceTable(ZYTableRef *tableRef);
    //获取源表列表表格
    ZYTableRef *GetSourceTable(int index);
    //获取源表列表表格计数
    int GetSourceTableCount(void);
    //获取源表列表表格
    IZYTable *GetSourceTableIndex(int index);
    //获取源表项列表表项
    int GetSourceTableItem(int index);
    //设置源表项列表表项
    void SetSourceTableItem(int index,int sourceTableItem);
    //比较同一表格的两行数据是否相等
    bool CompareTwoRow(IZYTable *t,int i1,int i2);
    //测试表中的某行数据是否是不重复的
    bool IsDistinct(IZYTable *t,int i);
    //判断一个字符串是否在一个表格中
    bool StringInTable(char *s,IZYTable *t);
    //获取性能监视器对象
    ZYProfiler *GetProfiler(void);
    //显示特定的表格
    void DisplayTable(IZYTable *t,bool displayData);
    //设置是否显示抽象语法树
    void SetDisplayAST(bool displayAST);
    //设置是否显示优化语法树
    void SetDisplayOST(bool displayOST);
    //设置是否显示数据表格
    void SetDisplayTables(bool displayTables);
    //设置是否显示查询耗时
    void SetDisplayTime(bool displayTime);
    //设置是否显示结果表格
    void SetDisplayResult(bool displayResult);
    //获取是否显示抽象语法树
    bool GetDisplayAST(void);
    //获取是否显示数据表格
    bool GetDisplayTables(void);
    //获取是否显示优化语法树
    bool GetDisplayOST(void);
    //获取是否显示查询耗时
    bool GetDisplayTime(void);
    //获取是否显示执行结果
    bool GetDisplayResult(void);
    //获取是否发生错误
    bool IsError(void);
    //发生查询执行错误
    void Error(void);
    //执行查询,并显示查询时间和结果
    void Execute(void);
    //显示各执行器的状况
    void Print(void);
private:
    //抽象语法树
    ZYSyntaxTree *syntaxTree;
    //性能监视器对象
    ZYProfiler *profiler;
    //当前数据库
    IZYDatabase *currentDatabase;
    //当前表格对象
    IZYTable *currentTable;
    //当前主键
    char *primaryKey;
    //当前表项
    int currentItem;   
    //是否发生错误
    bool error;
    //父查询执行器
    ZYExecutor *parentExecutor;
    //临时表编号
    static int tempTableNo;
    //临时列编号
    static int tempColumnNo;
    //当前选择查询语句
    ZYSelectStmt *currentSelectStmt;
    //源表列表
    ZYTableRef *sourceTables[100];
    //源表计数
    int sourceTableCount;
    //是否显示抽象语法树
    bool displayAST;
    //是否显示优化语法树
    bool displayOST;
    //是否显示数据表格
    bool displayTables;
    //是否显示查询耗时
    bool displayTime;
    //是否显示结果表格
    bool displayResult;
};

//语法树对象
class ZYSyntaxTree
{
public:
    //构造语法树对象
    ZYSyntaxTree(ESyntaxTreeType type,char *name,int lineno,bool hasChildren);
    //析构语法树对象
    virtual ~ZYSyntaxTree(void);
    //打印语法树对象
    virtual void Print(int indent);
    //对语法树对象进行语义分析
    virtual void Analyze(ZYAnalyzer *analyzer);
    //执行语法树对象
    virtual void Execute(ZYExecutor *executor);   
    //执行语法树对象(第2遍)
    virtual void Execute1(ZYExecutor *executor);
    //动态求语法树对象值
    virtual EResultType GetResult(ZYExecutor *executor,void *pData);
    //计算应下降到的表格索引
    virtual int CalcDescentTableIndex(ZYExecutor *executor);
    //获取语法树对象类型
    ESyntaxTreeType GetType(void);
    //获取语法树对象名称
    char *GetName(void);
    //设置名称词法标记位置
    void SetNameTokenLoc(int nameTokenStart,int nameTokenEnd);
    //执行语句开始信息输出
    void ExecuteStmtStart(ZYExecutor *executor);
    //执行语句结束信息输出
    void ExecuteStmtEnd(ZYExecutor *executor);
    //获取语法树对象孩子列表
    vector<ZYSyntaxTree *> *GetChildren(void);
    //将孩子加入孩子列表
    void AddChild(ZYSyntaxTree *child);
    //从孩子列表查找孩子
    ZYSyntaxTree *FindChild(ESyntaxTreeType type,char *name);
    //打印一个孩子,带缩进
    void PrintChild(ZYSyntaxTree *child,int indent);
    //打印孩子列表中的所有孩子,带缩进
    void PrintChildren(int indent);
    //执行列表中的所有孩子
    void ExecuteChildren(ZYExecutor *executor);
    //执行列表中特定类型的孩子
    void ExecuteChildren(ZYExecutor *executor,ESyntaxTreeType type);
    //反顺序执行列表中特定类型的孩子
    void ReverseExecuteChildren(ZYExecutor *executor,ESyntaxTreeType type);
    //执行列表中的所有孩子(第2遍)
    void ExecuteChildren1(ZYExecutor *executor);
    //安全地释放一棵语法树
    static void Release(ZYSyntaxTree *syntaxTree);
    //安全地释放一条字符串
    static void Release(char *s);
    //打印指定个字符的字符缩进
    static void PrintIndent(int indent);
    //安全地打印一条字符串
    static void PrintString(char *s);
    //打印一个运算符
    static void PrintOperation(ETokenType operation);
protected:
    //语法树对象行号
    int lineno;
    //语法树对象类型
    ESyntaxTreeType type;
    //语法树对象名称
    char *name;
    //语法树对象孩子列表
    vector<ZYSyntaxTree *> *children;
    //语法树对象父结点(暂未使用)
    ZYSyntaxTree *parent;
    //名称词法标记开始位置
    int nameTokenStart;
    //名称词法标记结束位置
    int nameTokenEnd;
};

//全局对象
class ZYGlobal:public ZYSyntaxTree
{
public:
    //构造全局对象
    ZYGlobal(int lineno);
    //打印全局对象
    void Print(int indent);
    //对全局对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行全局对象
    void Execute(ZYExecutor *executor);
};

//创建表格对象
class ZYCreateTable:public ZYSyntaxTree
{
public:
    //构造创建表格对象
    ZYCreateTable(char *name,int lineno);
    //析构创建表格对象
    ~ZYCreateTable(void);
    //设置主键词法标记位置
    void SetPrimaryKeyTokenLoc(int primaryKeyTokenStart,int primaryKeyTokenEnd);
    //设置表格主键
    void SetPrimaryKey(char *primaryKey);
    //打印创建表格对象
    void Print(int indent);
    //对创建表格对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行创建表格对象
    void Execute(ZYExecutor *executor);
private:
    //表格主键
    char *primaryKey;
    //主键词法标记开始位置
    int primaryKeyTokenStart;
    //主键词法标记结束位置
    int primaryKeyTokenEnd;
};

//外键声明对象
class ZYForeignKey:public ZYSyntaxTree
{
public:
    //构造外键声明对象
    ZYForeignKey(char *name,int lineno);
    //设置外键表格名
    void SetForeignTable(char *foreignTable);
    //设置外键列名
    void SetForeignColumn(char *foreignColumn);
    //设置外键表格词法标记位置
    void SetForeignTableTokenLoc(int foreignTableTokenStart,int foreignTableTokenEnd);
    //设置外键列词法标记位置
    void SetForeignColumnTokenLoc(int foreignColumnTokenStart,int foreignColumnTokenEnd);
    //打印外键声明对象
    void Print(int indent);
    //对外键声明对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行外键声明对象
    void Execute(ZYExecutor *executor);
private:
    //外键表格名
    char *foreignTable;
    //外键列名
    char *foreignColumn;
    //外键表格词法标记开始位置
    int foreignTableTokenStart;
    //外键表格词法标记结束位置
    int foreignTableTokenEnd;
    //外键列词法标记开始位置
    int foreignColumnTokenStart;
    //外键列词法标记结束位置
    int foreignColumnTokenEnd;
};

//撤消表格对象
class ZYDropTable:public ZYSyntaxTree
{
public:
    //构造撤消表格对象
    ZYDropTable(char *name,int lineno);
    //打印撤消表格对象
    void Print(int indent);
    //对撤消表格对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行撤消表格对象
    void Execute(ZYExecutor *executor);
};

//撤消视图对象
class ZYDropView:public ZYSyntaxTree
{
public:
    //构造撤消视图对象
    ZYDropView(char *name,int lineno);
    //打印撤消视图对象
    void Print(int indent);
    //对撤消视图对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行撤消视图对象
    void Execute(ZYExecutor *executor);
};

//创建视图对象
class ZYCreateView:public ZYSyntaxTree
{
public:
    //构造创建视图对象
    ZYCreateView(char *name,int lineno);
    //析构创建视图对象
    ~ZYCreateView(void);
    //设置视图选择语句
    void SetSelectStmt(ZYSelectStmt *selectStmt);
    //打印创建视图对象
    void Print(int indent);
    //对创建视图对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行创建视图对象
    void Execute(ZYExecutor *executor);
private:
    //视图主键
    ZYSelectStmt *selectStmt;
};

//列声明对象
class ZYColumnDecl:public ZYSyntaxTree
{
public:
    //构造列声明对象
    ZYColumnDecl(char *name,int lineno);
    //设置数据类型
    void SetDataType(int dataType);
    //设置数据长度
    void SetDataLength(int dataLength);
    //打印列声明对象
    void Print(int indent);
    //对列声明对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行列声明对象
    void Execute(ZYExecutor *executor);
private:
    //数据类型
    int dataType;
    //数据长度
    int dataLength;
};

//改变表格对象
class ZYAlterTable:public ZYSyntaxTree
{
public:
    //构造改变表格对象
    ZYAlterTable(char *name,int lineno);
    //设置列名词法标记位置
    void SetColumnTokenLoc(int columnTokenStart,int columnTokenEnd);
    //设置操作类型
    void SetOperation(ETokenType operation);
    //获取操作类型
    ETokenType GetOperation(void);
    //打印改变表格对象
    void Print(int indent);
    //对改变表格对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行改变表格对象
    void Execute(ZYExecutor *executor);
private:
    //操作类型
    ETokenType operation;
    //列名词法标记开始位置
    int columnTokenStart;
    //列名词法标记结束位置
    int columnTokenEnd;
};

//表格引用对象
class ZYTableRef:public ZYSyntaxTree
{
public:
    //构造表格引用对象
    ZYTableRef(char *name,char *asName,int lineno);
    //析构表格引用对象
    virtual ~ZYTableRef(void);
    //设置表格词法标记位置
    void SetTableTokenLoc(int tableTokenStart,int tableTokenEnd);
    //设置别名词法标记位置
    void SetAsNameTokenLoc(int asNameTokenStart,int asNameTokenEnd);
    //设置行索引
    void SetItemIndex(int itemIndex);
    //获取行索引
    int GetItemIndex(void);
    //设置表格对象指针
    void SetTableIndex(IZYTable *tableIndex);
    //获取表格对象指针
    IZYTable *GetTableIndex(void);
    //设置别名
    void SetAsName(char *asName);
    //获取别名
    char *GetAsName(void);
    //打印表格引用对象
    void Print(int indent);
    //对选择语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //设置源表
    void Execute(ZYExecutor *executor);
    //设置目标表
    void SetDestTable(ZYExecutor *executor);
    //增加表格子条件表达式
    void AddSubCondition(ZYExecutor *executor,ZYBoolExpression *sub_condition,int tableSourceIndex);
    //测试各表格子条件表达式
    bool TestSubConditions(ZYExecutor *executor);
    //获取 [主键]=值 类型子条件表达式
    ZYBoolExpression *GetPKeyEquals(void);
    //获取 [辅键]=值 类型子条件表达式
    ZYBoolExpression *GetAKeyEquals(void);
protected:
    //行索引
    int itemIndex;  
    //表格指针
    IZYTable *tableIndex;
    //别名
    char *asName;
    //[主键]=值 类型子条件表达式
    ZYBoolExpression *pkeyEquals;
    //[辅键]=值 类型子条件表达式
    ZYBoolExpression *akeyEquals;
    //子条件表达式
    vector<ZYBoolExpression *> *sub_conditions;
    //表格词法标记开始位置
    int tableTokenStart;
    //表格词法标记结束位置
    int tableTokenEnd;
    //别名词法标记开始位置
    int asNameTokenStart;
    //别名词法标记结束位置
    int asNameTokenEnd;
};

//选择语句对象
class ZYSelectStmt:public ZYTableRef
{
public:
    //构造选择语句对象
    ZYSelectStmt(int lineno);
    //析构选择语句对象
    ~ZYSelectStmt(void);
    //获取目标列列表
    vector<ZYSyntaxTree *> *GetDestColumns(void);
    //获取源表列表
    vector<ZYTableRef *> *GetSourceTables(void);
    //设置选择条件表达式
    void SetCondition(ZYSyntaxTree *condition);
    //设置行是否互异标志
    void SetDistinct(bool distinct);
    //设置相关子查询标志
    void SetAssociated(bool associated);
    //设置查询结果名
    void SetTableName(char *tableName);
    //检测并重设聚合函数列
    void DetectAndResetAggreColumns(void);
    //下降条件表达式到表格
    void DescentSubConditionExpressions(ZYExecutor *executor);
    //下降子条件表达式到表格
    void DescentSubConditionExpression(ZYExecutor *executor,ZYBoolExpression *sub_condition);
    //打印选择语句对象
    void Print(int indent);
    //动态求选择语句对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //对选择语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行选择语句对象
    void Execute(ZYExecutor *executor);
    //计算应下降到的表格索引
    int CalcDescentTableIndex(ZYExecutor *executor);
    //获取表格名称
    char *GetTableName(void);
private:
    //执行源表格列表
    bool ExecuteFromList(ZYExecutor *executor);
    //执行选择查询
    void PerformSelect(ZYExecutor *executor);
    //执行选择查询(总共N个表格,当前处理第n个表格)
    void PerformSelectN(ZYExecutor *executor,int N,int n);
    //行是否互异标志
    bool distinct;
    //已执行标志
    bool executed;
    //相关子查询标志
    bool associated;
    //临时表名
    char tableName[100];
    //目标列列表
    vector<ZYSyntaxTree *> *destColumns;
    //源表列表
    vector<ZYTableRef *> *sourceTables;
    //选择条件表达式
    ZYBoolExpression *condition;
    //是否有聚合函数列
    bool hasAggreColumn;
    //条件是否下降标志
    bool conditionDescented;
};

//列引用对象
class ZYColumnRef:public ZYSyntaxTree
{
public:
    //构造列引用对象
    ZYColumnRef(char *name,int lineno,char *tableName);
    //析构列引用对象
    ~ZYColumnRef(void);
    //设置表格词法标记位置
    void SetTableTokenLoc(int tableTokenStart,int tableTokenEnd);
    //设置别名词法标记位置
    void SetAsNameTokenLoc(int asNameTokenStart,int asNameTokenEnd);
    //获取列索引
    int GetColumnIndex(void);
    //获取列类型
    EColumnType GetColumnType(void);
    //设置源表格
    void SetSourceTable(ZYTableRef *sourceTable);
    //设置别名
    void SetAsName(char *asName);
    //获取别名
    char *GetAsName(void);
    //是否有别名
    bool HasAsName(void);
    //测试并重设聚合函数列
    bool TestAndResetAggreColumn(void);
    //设置子表达式
    void SetExpression(ZYSyntaxTree *expression);
    //动态求列引用对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //打印列引用对象
    void Print(int indent);
    //对列引用对象进行语义分析(SelectStmt调用)
    void Analyze(ZYAnalyzer *analyzer);
    //对列引用对象进行语义分析(CreateView调用)
    void Analyze1(ZYAnalyzer *analyzer);
    //对列引用对象进行语义分析(InsertStmt、AlterTable调用)
    void Analyze2(ZYAnalyzer *analyzer);
    //执行列引用对象(复制列)
    void Execute(ZYExecutor *executor);
    //执行列引用对象(获取列信息)
    void Execute1(ZYExecutor *executor);
    //计算表格源索引
    void CalcTableSourceIndex(ZYExecutor *executor,bool associated);
    //计算应下降到的表格索引
    int CalcDescentTableIndex(ZYExecutor *executor);
    //是否是主键列
    bool IsPKeyColumn(ZYExecutor *executor);
    //是否是辅键列
    bool IsAKeyColumn(ZYExecutor *executor);
    //获取表格源索引
    int GetTableSourceIndex(void);
    //是否相关属性
    bool IsAssociated(void);
    //查找是否有该列
    void LookUpInTables(ZYAnalyzer *analyzer);
private:
    //所在表格源
    ZYTableRef *sourceTable;
    //表格名
    char *tableName;
    //列索引
    int columnIndex;
    //列类型
    int columnType;
    //列所在表格在语句中的索引位置
    int tableSourceIndex;
    //列所在表格在语句中的索引位置已计算
    bool calculated;
    //所在的SELECT语句对应的查询执行器对象
    ZYExecutor *executor;
    //列别名
    char asName[20];
    //列值表达式
    ZYSyntaxTree *expression;
    //相关属性
    bool associated;
    //表格词法标记开始位置
    int tableTokenStart;
    //表格词法标记结束位置
    int tableTokenEnd;
    //别名词法标记开始位置
    int asNameTokenStart;
    //别名词法标记结束位置
    int asNameTokenEnd;
    //分析时找到表格
    bool tableFound;
    //分析时找到列
    bool columnFound;
};

//按列排序对象
class ZYOrderBy:public ZYSyntaxTree
{
public:
    //构造按列排序对象
    ZYOrderBy(char *name,int lineno,ESortOrder sortOrder);
    //打印按列排序对象
    void Print(int indent);
    //执行按列排序对象
    void Execute(ZYExecutor *executor);
private:
    //排序顺序
    ESortOrder sortOrder;
};

//按列分组对象
class ZYGroupBy:public ZYSyntaxTree
{
public:
    //构造按列分组对象
    ZYGroupBy(int lineno);
    //析构按列分组对象
    ~ZYGroupBy(void);
    //设置筛选条件
    void SetHavingCondition(ZYSyntaxTree *havingCondition);
    //打印按列分组对象
    void Print(int indent);
    //执行按列分组对象
    void Execute(ZYExecutor *executor);
private:
    //筛选条件
    ZYBoolExpression *havingCondition;
};

//插入语句对象
class ZYInsertStmt:public ZYSyntaxTree
{
public:
    //构造插入语句对象
    ZYInsertStmt(int lineno);
    //析构插入语句对象
    ~ZYInsertStmt(void);
    //设置插入目标表格
    void SetDestTable(ZYTableRef *destTable);
    //打印插入语句对象
    void Print(int indent);
    //对插入语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行插入语句对象
    void Execute(ZYExecutor *executor);
private:
    //插入目标表格
    ZYTableRef *destTable;
};

//更新语句对象
class ZYUpdateStmt:public ZYSyntaxTree
{
public:
    //构造更新语句对象
    ZYUpdateStmt(int lineno);
    //析构更新语句对象
    ~ZYUpdateStmt(void);
    //设置目标表格
    void SetDestTable(ZYTableRef *destTable);
    //设置更新条件
    void SetCondition(ZYSyntaxTree *condition);
    //打印更新语句对象
    void Print(int indent);
    //对更新语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行更新语句对象
    void Execute(ZYExecutor *executor);
private:
    //目标表格
    ZYTableRef *destTable;
    //更新条件
    ZYBoolExpression *condition;
};

//删除语句对象
class ZYDeleteStmt:public ZYSyntaxTree
{
public:
    //构造删除语句对象
    ZYDeleteStmt(int lineno);
    //析构删除语句对象
    ~ZYDeleteStmt(void);
    //设置目标表格
    void SetDestTable(ZYTableRef *destTable);
    //设置删除条件
    void SetCondition(ZYSyntaxTree *condition);
    //打印删除语句对象
    void Print(int indent);
    //对删除语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行删除语句对象
    void Execute(ZYExecutor *executor);
private:
    //目标表格
    ZYTableRef *destTable;
    //删除条件
    ZYBoolExpression *condition;
};

//赋值语句对象
class ZYAssignExpression:public ZYSyntaxTree
{
public:
    //构造赋值语句对象
    ZYAssignExpression(int lineno);
    //析构赋值语句对象
    ~ZYAssignExpression(void);
    //设置赋值目标列
    void SetDestColumn(ZYColumnRef *destColumn);
    //设置赋值源表达式
    void SetSrcExpression(ZYSyntaxTree *srcExpression);
    //打印赋值语句对象
    void Print(int indent);
    //对赋值语句对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
    //执行赋值语句对象
    void Execute(ZYExecutor *executor);
private:
    //赋值目标列
    ZYColumnRef *destColumn;
    //赋值源表达式
    ZYSyntaxTree *srcExpression;
};

//布尔表达式对象
class ZYBoolExpression:public ZYSyntaxTree
{
public:
    //构造布尔表达式对象
    ZYBoolExpression(int lineno);
    //析构布尔表达式对象
    ~ZYBoolExpression(void);
    //设置操作符
    void SetOperation(ETokenType operation);
    //设置左表达式
    void SetLeftExpression(ZYSyntaxTree *leftExpression);
    //设置右表达式
    void SetRightExpression(ZYSyntaxTree *rightExpression);
    //获取操作符
    ETokenType GetOperation(void);
    //获取左表达式
    ZYSyntaxTree *GetLeftExpression(void);
    //获取右表达式
    ZYSyntaxTree *GetRightExpression(void);
    //动态求布尔表达式对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //计算应下降到的表格索引
    int CalcDescentTableIndex(ZYExecutor *executor);
    //交换左边和右边的表达式
    void SwapLeftAndRight(void);
    //打印布尔表达式对象
    void Print(int indent);
    //对布尔表达式对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
private:
    //操作符
    ETokenType operation;
    //左表达式
    ZYSyntaxTree *leftExpression;
    //右表达式
    ZYSyntaxTree *rightExpression;
};

//聚合函数表达式对象
class ZYAggregateFunction:public ZYSyntaxTree
{
public:
    //构造聚合函数表达式对象
    ZYAggregateFunction(int lineno);
    //析构聚合函数表达式对象
    ~ZYAggregateFunction(void);
    //设置聚合函数
    void SetFunction(ETokenType function);
    //设置聚合列
    void SetColumn(ZYColumnRef *column);
    //获取聚合函数
    ETokenType GetFunction(void);
    //获取聚合列
    ZYColumnRef *GetColumn(void);
    //重置聚合函数表达式对象
    void Reset(void);
    //动态求聚合函数表达式对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //打印聚合函数表达式对象
    void Print(int indent);
    //对聚合函数表达式对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
private:
    //聚合函数
    ETokenType function;
    //聚合列
    ZYColumnRef *column;
    //计数值
    int c;
    //整数值
    int a;
    //小数值
    double d;
    //MIN,MAX等是否已初始化
    bool inited;
};

//表达式对象
class ZYExpression:public ZYSyntaxTree
{
public:
    //构造表达式对象
    ZYExpression(int lineno);
    //析构表达式对象
    ~ZYExpression(void);
    //设置操作符
    void SetOperation(ETokenType operation);
    //设置左表达式
    void SetLeftExpression(ZYSyntaxTree *leftExpression);
    //设置右表达式
    void SetRightExpression(ZYSyntaxTree *rightExpression);
    //动态求表达式对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //计算应下降到的表格索引
    int CalcDescentTableIndex(ZYExecutor *executor);
    //打印表达式对象
    void Print(int indent);
    //对表达式对象进行语义分析
    void Analyze(ZYAnalyzer *analyzer);
private:
    //操作符
    ETokenType operation;
    //左表达式
    ZYSyntaxTree *leftExpression;
    //右表达式
    ZYSyntaxTree *rightExpression;
};

//数据行对象
class ZYRowValue:public ZYSyntaxTree
{
public:
    //构造数据行对象
    ZYRowValue(int lineno);
    //打印数据行对象
    void Print(int indent);
};

//单个数据对象
class ZYValue:public ZYSyntaxTree
{
public:
    //构造单个数据对象
    ZYValue(int lineno,EColumnType dataType,void *pData);
    //获取数据对象数据值
    void GetValue(void *pData);
    //动态求单个数据对象值
    EResultType GetResult(ZYExecutor *executor,void *pData);
    //打印单个数据对象
    void Print(int indent);
private:
    //数据类型
    EColumnType dataType;
    //整数值
    int a;
    //小数值
    double d;
};

#endif