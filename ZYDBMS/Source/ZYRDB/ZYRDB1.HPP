//---------------------------------------------------------
//逐月存储引擎(ZYRDB) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYRDB1_HPP)        //测试ZYRDB1_HPP_宏
#define ZYRDB1_HPP              //使得ZYRDB1.HPP仅包含1次

//引入32位Windows API库
#include <windows.h>

//for PathFileExsits
#include <shlwapi.h>

//引入C++输入输出库
#include <iostream>

//引入C++泛型算法库
#include <algorithm>

//引入C++泛型向量库
#include <vector>

//应用C++标准名称空间
using namespace std;

//引入逐月存储引擎库
#include "ZYRDB.HPP"

//数据库名称字符串最大长度
#define DATABASE_NAME_LEN 128

//用户名字符串最大长度
#define USERID_LEN 32

//密码字符串最大长度
#define PASSWORD_LEN 32

//每个表的列的最大数目
#define MAX_COLUMN 32

//表格数据行的最大长度
#define ROW_LEN 32000

//表格数据项标准缓冲区的长度
#define DATA_LEN 1000

//表格目录名字符串最大长度
#define TABLE_DIR_LEN 16

//表格表格名字符串最大长度
#define TABLE_TAB_LEN 16

//列名称字符串最大长度
#define COLUMN_NAME_LEN 16

//表格属性-索引表
#define TABLE_ATTR_VIEW         0x00000001

//表格属性-索引表
#define TABLE_ATTR_INDEX        0x00000002

//列特性-被索引表辅属性
#define COLUMN_ATTR_INDEXEDA    0x00000008

//列特性-被索引表主属性
#define COLUMN_ATTR_INDEXEDP    0x00000010

class ZYFile
{
public:
    //构造文件对象
    ZYFile(char *filename);
    //析构文件对象
    ~ZYFile(void);
    //以可读写方式打开一个文件
    void OpenFile(void);
    //关闭一个已打开的文件
    void CloseFile(void);
    //文件成功打开
    bool FileOpened(void);
    //读取文件数据
    void ReadFile(int fp,void *p,int n);
    //写入文件数据
    void WriteFile(int fp,void *p,int n);
    //刷新文件缓冲区
    virtual void FlushFile(void);
    //将文件截断到正常结束
    void SetEndOfFile(int fp);
    //获取文件长度
    int GetFileSize(void);
    //文件是否存在
    static bool Exists(char *filename);
private:
    //对数据进行加密
    void EncriptData(void *p,int n);
    //对数据进行解密
    void UnencriptData(void *p,int n);
    //表格文件名
    char *filename;
    //表格文件句柄
    HANDLE hFile;
};

//数据库对象
class ZYDatabase;

//数据库文件对象
class ZYDatabaseFile:public ZYFile
{
public:
    //构造数据库文件对象
    ZYDatabaseFile(ZYDatabase *database,char *filename);
    //加载数据库文件数据
    void LoadData(void);
    //保存数据库文件数据
    void SaveData(void);
    //读取数据库文件头
    void ReadHeader(void);
    //写入数据库文件头
    void WriteHeader(void);
    //读取数据库文件索引信息
    void ReadIndex(int j);
    //写入数据库文件索引信息
    void WriteIndex(int j);
    //读取数据库文件全部索引信息
    void ReadIndexs(void);
    //写入数据库文件全部索引信息
    void WriteIndexs(void);
    //将数据库文件截断到正常结束
    void SetEndOfFile(void);
private:        
    //数据库对象
    ZYDatabase *database;
};

//表格对象
class ZYTable;

//索引对象
class ZYIndex;

//索引声明对象
class ZYIndexDecl
{
public:
    //构造索引声明对象
    ZYIndexDecl(void);
    //构造索引声明对象
    ZYIndexDecl(char *index_name,char *table_name,char *akey,char *pkey);
    //索引表名称
    char index_name[TABLE_TAB_LEN];
    //被索引表名称
    char table_name[TABLE_TAB_LEN];
    //辅键名称
    char akey[COLUMN_NAME_LEN];
    //主键名称
    char pkey[COLUMN_NAME_LEN];
};

//数据库对象
class ZYDatabase:public IZYDatabase
{
public:
    //打开数据库
    static ZYDatabase *Open(char *path,char *name,char *userid,char *password);
    //创建数据库
    static ZYDatabase *Create(char *path,char *name,char *userid,char *password);
    //备份数据库
    static ZYDatabase *Restore(ZYDatabase *database,char *dest_path,char *dest_name,
                            char *dest_userid,char *dest_password,void (*p)(double percent));
    //撤消数据库
    static void Drop(ZYDatabase *database);
    //构造数据库对象
    ZYDatabase(void);
    //析构数据库对象
    ~ZYDatabase(void);
    //测试数据库的存在性
    static bool Exists(char *path);
    //测试数据库是否已被打开
    static bool Opened(char *path);
    //建立数据库文件夹
    void NewDatabase(char *path,char *name,char *userid,char *password);
    //加载数据库文件夹
    bool LoadDatabase(char *path,char *name,char *userid,char *password);
    //获取数据库中的表格数目
    int GetTableCount(void);
    //获取数据库中的表格
    ZYTable *GetTable(int t);
    //获取数据库中的表格
    ZYTable *GetTable(char *name);
    //获取数据库中的表格
    IZYTable *GetTableI(int t);
    //获取数据库中的表格
    IZYTable *GetTableI(char *name);
    //在数据库中创建表格
    IZYTable *CreateTable(char *name);
    //在数据库中创建视图
    IZYTable *CreateView(char *name);
    //在数据库中撤消表格
    void DropTable(ZYTable *table);
    //在数据库中撤消表格
    void DropTable(IZYTable *table);
    //在数据库中撤消表格
    void DropTable(char *name);     
    //在数据库中创建索引
    ZYIndex *CreateIndex(char *name,ZYTable *table,char *akey,char *pkey);
    //在数据库中创建索引
    IZYIndex *CreateIndex(char *name,IZYTable *table,char *akey,char *pkey);
    //备份数据库
    void Backup(char *dest_folder,void (*p)(double percent));
    //输出数据库到屏幕
    void Print(void);
private:
    //声明ZYDatabaseFile为友元类
    friend class ZYDatabaseFile;
    //加载数据库文件夹里的所有数据文件
    void LoadTables(void);
    //加载数据库中的所有索引
    void LoadIndexs(void);
    //计算全部要拷贝文件的总计长度
    int CountFileSize(void);
    //最后一个表的文件名编号
    int latest_table;
    //数据目录的路径
    char *path;
    //数据库表格
    vector<ZYTable *> *tables;
    //数据库文件对象
    ZYDatabaseFile *databaseFile;
    //文件类型
    int file_type;
    //文件版本
    int file_version;
    //数据库名称
    char name[DATABASE_NAME_LEN];
    //用户名
    char userid[USERID_LEN];
    //密码
    char password[PASSWORD_LEN];
    //表格自动编号
    int table_autoid;
    //数据库属性
    int database_attr;
    //索引声明个数
    int index_cnt;
    //索引说明长度
    int index_size;
    //索引声明列表
    vector<ZYIndexDecl *> *indexs;
};

//表格列对象
class ZYColumn
{
public:
    //获取列名称
    void GetName(char *name);
    //获取列类型
    EColumnType GetType(void);
    //获取列长度
    int GetLength(void);
    //获取列属性
    int GetAttribute(void);
private:
    //声明ZYTable为友元类
    friend class ZYTable;
    //声明ZYTableFile为友元类
    friend class ZYTableFile;
    //列名称
    char name[COLUMN_NAME_LEN];
    //列类型
    EColumnType type;
    //列长度
    int length;
    //列偏移
    int offset;
    //列特性
    int attribute;
};

//表格行对象
class ZYRow
{
public:
    //表格文件数据行索引
    int i;
    //数据行数据缓冲区
    char row_data[ROW_LEN];
};

//表格文件对象
class ZYTableFile:public ZYFile
{
public:
    //构造表格文件对象
    ZYTableFile(ZYTable *table,char *filename);
    //刷新文件缓冲区
    void FlushFile(void);
    //加载表格文件数据
    void LoadData(void);
    //读取表格文件头
    void ReadHeader(void);
    //写入表格文件头
    void WriteHeader(void);
    //读取表格文件列信息
    void ReadColumn(int j);
    //写入表格文件列信息
    void WriteColumn(int j);
    //读取表格文件全部列信息
    void ReadColumns(void);
    //写入表格文件全部列信息
    void WriteColumns(void);
    //读取表格文件数据行(单行)
    void ReadRow(int i);
    //写入表格文件数据行(单行)
    void WriteRow(int i);
    //插入表格文件数据行(单行)
    void InsertRow(int i);
    //删除表格文件数据行(单行)
    void DeleteRow(int i);
    //将表格文件截断到正常结束
    void SetEndOfFile(void);
private:        
    //表格对象
    ZYTable *table;
    //数据行数据起始地址
    int RowDataBase;
};

//表格对象
class ZYTable:public IZYTable
{
public:
    //获取表格名称
    void GetName(char *name);
    //获取表格目录(向上兼容)
    void GetDir(char *dir);
    //获取表格属性
    int GetAttr(void);
    //获取指定表格的行数
    int GetRowCount(void);
    //获取指定表指定数据项,使用列索引
    void GetData(int i,int j,void *data);
    //获取指定表指定数据项,使用列名称
    void GetData(int i,char *key,void *data);
    //设置指定表指定数据项,使用列索引
    void SetData(int i,int j,void *data);
    //设置指定表指定数据项,使用列名称
    void SetData(int i,char *key,void *data);
    //追加表格数据行
    int AppendRow(void);
    //插入表格数据行
    int InsertRow(int i);
    //删除表格数据行
    void DeleteRow(int i);
    //查找指定的数据行,返回数据项索引
    int FindRow(int j,void *data);
    //清空表格数据行
    void ClearRows(void);
    //获取将生成的自动编号
    int GetAutoid(void);
    //新生成一个自动编号
    int NewAutoid(void);
    //获取指定表格的列数
    int GetColumnCount(void);
    //获取指定的表格列
    ZYColumn *GetColumn(int j);
    //获取指定的表格列
    ZYColumn *GetColumn(char *name);
    //查找指定表的指定列索引,未找到返回-1
    int FindColumn(char *name);
    //删除表格列
    void DeleteColumn(int j);
    //删除表格列
    void DeleteColumn(char *name);
    //插入表格列
    int InsertColumn1(int j,char name[],EColumnType type,int length,int attribute);
    //插入表格列(改进版本)
    int InsertColumn(int j,char name[],EColumnType type,int length,int attribute);
    //追加表格列
    int AppendColumn1(char name[],EColumnType type,int length,int attribute);
    //追加表格列(改进版本)
    int AppendColumn(char name[],EColumnType type,int length,int attribute);
    //获取表格列
    void GetColumn(int j,char *aspect,void *data);
    //获取表格列
    void GetColumn(char *name,char *aspect,void *data);
    //修改表格列
    void SetColumn(int j,char *aspect,void *data);
    //修改表格列
    void SetColumn(char *name,char *aspect,void *data);
    //复制表格列
    int CopyColumn(ZYTable *source,char *column_name);
    //复制表格列
    int CopyColumn(IZYTable *source,char *column_name);
    //清空表格列
    void ClearColumns(void);
    //进入缓冲方式
    void EnterBufferedMode(void);
    //离开缓冲方式
    void LeaveBufferedMode(void);
    //按指定列对表格进行选择排序
    void SelectSort(int j1,ESortOrder sortOrder);
    //按指定列对表格进行选择排序
    void SelectSort(char *key,ESortOrder sortOrder);
    //输出表格到屏幕
    void Print(void);
    //是否视图表
    bool IsView(void);
    //是否索引表
    bool IsIndex(void);
    //设置主键
    void SetPrimaryKey(int j);
    //设置主键
    void SetPrimaryKey(char *name);
    //获取主键
    int GetPrimaryKey(void);
    //获取辅键上的索引
    IZYIndex *GetIndexOn(int j);
    //获取辅键上的索引
    IZYIndex *GetIndexOn(char *name);
protected:
    //声明ZYDatabase为友元类
    friend class ZYDatabase;
    //声明ZYTableFile为友元类
    friend class ZYTableFile;
    //声明ZYIndex为友元类
    friend class ZYIndex;
    //构造表格对象(打开)
    ZYTable(char *filename);
    //构造表格对象(新建)
    ZYTable(char *filename,char *tab);
    //析构表格对象
    virtual ~ZYTable(void);
    //撤消表格
    void Drop(void);    
    //获取指定的表格数据项指针
    void *GetDataPointer(int i,int j);
    //获取指定的表格数据项指针(用于InsertRow)
    void *GetDataPointer1(int i,int j);
    //比较表中属性与指定值
    int CompareRow1(void *data,int i,int j);
    //顺序查找指定的数据行,返回数据项索引
    int FindRow1(int j,void *data);
    //二分查找指定的数据行,返回数据项索引
    int FindRow2(int j,void *data);
    //二分查找指定的数据行,返回数据项索引(用于SetData)
    int FindRow3(int j,void *data);
    //获取指定的表格列(用于InsertColumn)
    ZYColumn *GetColumn1(int j);
    //计算指定表格各属性的偏移
    void CalcOffset(void);
    //将表格复制到临时表格
    ZYTable *CopyToTemp(void);
    //从临时表格中重新装入表格数据
    void TransferBack(ZYTable *temp);
    //交换表格中的两行
    void SwapRow(int i1,int i2);
    //比较表格的两行
    int CompareRow(int j,int i1,int i2);
    //备份数据库中的一个表格
    void Backup(char *dest_path,int total_size,int *bytes_copied,void (*p)(double percent));
    //文件类型
    int file_type;
    //表格文件版本
    int file_version;
    //表格属性
    int table_attr;
    //列计数
    int column_cnt;
    //列长度
    int column_size;
    //行计数
    int row_cnt;
    //行长度
    int row_size;
    //行自动编号
    int row_autoid;
    //行最大数目
    int row_max;
    //表格目录名
    char dir[TABLE_DIR_LEN];
    //表格表格名
    char tab[TABLE_TAB_LEN];
    //列信息数据缓冲区
    ZYColumn columns[MAX_COLUMN];
    //数据行数据缓冲区
    ZYRow row;       
    //表格文件对象
    ZYTableFile *tableFile;
    //表格所在的文件名
    char *filename;
    //表格各带索引属性的索引表句柄
    ZYIndex *indexs[MAX_COLUMN];  
    //是否调用FlushFile标志
    bool bFlush;
};

//索引对象
class ZYIndex:public ZYTable,public IZYIndex
{
public:
    //转换为表格对象
    IZYTable *ToTable(void);
    //查找索引项
    int FindIndex(void *adata);
    //查找索引项
    int FindIndex1(void *adata,void *pdata);
    //插入索引项
    int InsertIndex(void *adata,void *pdata);
    //是否辅键值的索引项
    bool IsIndexOf(int i,void *adata);
    //获取索引项
    void GetIndex(int i,void *pdata);
    //删除索引项
    void DeleteIndex(void *adata,void *pdata);
    //修改索引项(AKEY变化)
    void SetIndex(void *adata1,void *adata2,void *pdata);
    //修改索引项(PKEY变化)
    void SetIndex1(void *adata,void *pdata1,void *pdata2);
private:
    //声明ZYDatabase为友元类
    friend class ZYDatabase;
    //声明ZYTable为友元类
    friend class ZYTable;
    //构造索引对象(打开)
    ZYIndex(char *filename,ZYTable *table,char *akey,char *pkey);
    //构造索引对象(创建)
    ZYIndex(char *filename,char *name,ZYTable *table,char *akey,char *pkey);
    //索引是否自动更新
    static bool index_update;
    //索引是否自动更新
    static bool IfIndexUpdate(void);
    //启用索引自动更新
    static void EnableIndexUpdate(void);
    //禁用索引自动更新
    static void DisableIndexUpdate(void);
    //更新索引,用于AppendRow
    static void IndexUpdate1(ZYTable *table,int i);
    //更新索引,用于DeleteRow
    static void IndexUpdate2(ZYTable *table,int i);
    //更新索引,用于SetData
    static void IndexUpdate3(ZYTable *table,int i,int j,void *data);
    //更新索引,用于ClearRows
    static void IndexUpdate4(ZYTable *table);
};

//日期时间对象
class ZYDateTime:public IZYDateTime
{
public:
    //构造日期时间对象
    ZYDateTime(void);
    //构造日期时间对象
    ZYDateTime(int year,int month,int day);
    //获取年信息
    int GetYear(void);
    //获取月信息
    int GetMonth(void);
    //获取日信息
    int GetDay(void);
    //获取时信息
    int GetHour(void);
    //获取分信息
    int GetMinute(void);
    //获取秒信息
    int GetSecond(void);
    //计算日期之间的间隔
    int Substract(IZYDateTime *datetime);
private:
    //是否是闰年
    bool IsRunNian(int year);
    //计算到1980.01.01的天数
    int Substract1980(void);
    //系统日期时间结构
    SYSTEMTIME datetime;
};

//随机数发生器对象
class ZYRandom:public IZYRandom
{
public:
    //构造随机数发生器对象
    ZYRandom(void);
    //生成属于[min,max]区间的随机整数
    int RandInt(int min,int max);
    //生成属于[min,max]区间的随机小数
    double RandDouble(double min,double max);
};

//数据库字符串对象
class ZYString
{
public:
    //拷贝一个变长字符串到定长字符串中
    static void Copy1(char *dest,char *src,int cnt);
    //拷贝一个定长字符串到变长字符串中
    static void Copy2(char *dest,char *src,int cnt);
    //将一个定长字符串与变长字符串比较,判断是否相等
    static int Compare(char *sa,char *sb,int cnt);
    //复制一个字符串
    static char *Copy(char *s);
    //释放复制的字符串
    static void Free(char *s);
};

//内存空间对象
class ZYMemory:public IZYMemory
{
public:
    //构造内存空间对象
    ZYMemory(int size);
    //析构内存空间对象
    ~ZYMemory(void);
    //获取内存空间指针
    void *GetPointer(void);
    //获取内存空间大小
    int GetSize(void);
    //调整内存空间大小
    void *Resize(int size);
private:
    //申请内存空间
    static void *ApplyMemory(int byte_count);
    //获取内存空间大小
    static int SizeOfMemory(void *lp);
    //调整内存空间大小
    static void *ResizeMemory(void *lp,int byte_count);
    //释放内存空间
    static void FreeMemory(void *lp);
    //内存空间指针
    void *lp;
    //内存空间大小
    int size;
};

#endif
