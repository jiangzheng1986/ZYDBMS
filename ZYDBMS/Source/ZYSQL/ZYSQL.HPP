//---------------------------------------------------------
//逐月查询引擎(ZYSQL) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYSQL_HPP)        //测试ZYSQL_HPP_宏
#define ZYSQL_HPP              //使得ZYSQL.HPP仅包含1次

//源程序字符类型
typedef enum
{
    ESourceCharType_Unknown,         //未知类型
    ESourceCharType_Comment,         //注释
    ESourceCharType_Identifier,      //标识符
    ESourceCharType_Keyword,         //关键字
    ESourceCharType_Punctuation,     //标点符号
    ESourceCharType_String,          //字符串
    ESourceCharType_Num,             //整数
    ESourceCharType_Float,           //小数
    ESourceCharType_Eof              //文件结束
}ESourceCharType;

//不匹配标志
#define DISMATCH_FLAG   0x80

//已确认标志
#define ACKNOWLEDGED_FLAG 0x40

//逐月查询编译器对象
class IZYCompiler;

//性能监视对象
class IZYProfiler;

//创建逐月查询编译器对象
IZYCompiler *IZYCompiler_Create(char *listingfile,IZYDatabase *database);

//释放逐月查询编译器对象
void IZYCompiler_Release(IZYCompiler *compiler);

//逐月查询编译器对象
class IZYCompiler
{
public:
    //检查逐月结构化查询语言(ZYSQL)语句
    virtual void CheckSQL(char *sql,char *format)=0;
    //执行逐月结构化查询语言(ZYSQL)语句
    virtual void ExecuteSQL(char *sql)=0;
    //获取查询执行结果表格
    virtual IZYTable *GetResultTable(void)=0;
    //设置是否显示源程序
    virtual void SetDisplaySource(bool displaySource)=0;
    //设置是否显示匹配符号
    virtual void SetDisplayMatch(bool displayMatch)=0;
    //设置是否显示抽象语法树
    virtual void SetDisplayAST(bool displayAST)=0;
    //设置是否显示优化语法树
    virtual void SetDisplayOST(bool displayOST)=0;
    //设置是否显示数据表格
    virtual void SetDisplayTables(bool displayTables)=0;
    //设置是否显示查询耗时
    virtual void SetDisplayTime(bool displayTime)=0;
    //设置是否显示结果表格
    virtual void SetDisplayResult(bool displayResult)=0;
};

//测试文本文件文本存在性
bool IZYTextFile_Exists(char *filename);

//加载文本文件文本
char *IZYTextFile_LoadText(char *filename);

//保存文本文件文本
void IZYTextFile_SaveText(char *filename,char *s);

//释放文本文件文本
void IZYTextFile_FreeText(char *s);

//删除文本文件文本
void IZYTextFile_DeleteText(char *filename);

//创建性能监视对象
IZYProfiler *IZYProfiler_Create(void);

//释放性能监视对象
void IZYProfiler_Release(IZYProfiler *profiler);

//性能监视对象
class IZYProfiler
{
public:
    //性能监视对象性能监视开始
    void Start(void);
    //性能监视对象性能监视结束
    void End(void);
    //获取性能监视对象监视到的执行时间(秒)
    float GetTime(void);
    //获取性能监视对象监视到的执行时间(毫秒)
    int GetTimeMs(void);
};

#endif