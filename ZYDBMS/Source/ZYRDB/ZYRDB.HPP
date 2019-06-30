//---------------------------------------------------------
//逐月存储引擎(ZYRDB) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYRDB_HPP)        //测试ZYRDB1_HPP_宏
#define ZYRDB_HPP              //使得ZYRDB1.HPP仅包含1次

//引入32位Windows API库
#include <windows.h>

//列特性
enum EColumnAttr
{
    //列特性-被索引表辅属性
    EColumnAttr_IndexedA=8,
    //列特性-被索引表主属性
    EColumnAttr_IndexedP=16
};

//列类型
enum EColumnType
{
    //列类型-字符串
    EColumnType_String=0,
    //列类型-双精度浮点
    EColumnType_Double=1,
    //列类型-整数
    EColumnType_Integer=2
};

//排序顺序
enum ESortOrder
{
    //按升序排序
    ESortOrder_Asc=0,
    //按降序排序
    ESortOrder_Desc=1
};

//表格对象
class IZYTable;

//索引对象
class IZYIndex;

//数据库对象
class IZYDatabase;

//日期时间对象
class IZYDateTime;

//随机数对象
class IZYRandom;

//内存空间对象
class IZYMemory;

//测试数据库的存在性
bool IZYDatabase_Exists(char *path);

//测试数据库是否已被打开
bool IZYDatabase_Opened(char *path);

//创建数据库
IZYDatabase *IZYDatabase_Create(char *path,char *name,char *userid,char *password);

//打开数据库
IZYDatabase *IZYDatabase_Open(char *path,char *name,char *userid,char *password);

//释放数据库
void IZYDatabase_Close(IZYDatabase *database);

//撤消数据库
void IZYDatabase_Drop(IZYDatabase *database);

//备份数据库
IZYDatabase *IZYDatabase_Restore(IZYDatabase *database,char *dest_path,char *dest_name,
                        char *dest_userid,char *dest_password,void (*p)(double percent));

//数据库对象
class IZYDatabase
{
public:
    //获取数据库中的表格数目
    virtual int GetTableCount(void)=0;
    //获取数据库中的表格
    virtual IZYTable *GetTableI(int t)=0;
    //获取数据库中的表格
    virtual IZYTable *GetTableI(char *name)=0;
    //在数据库中创建表格
    virtual IZYTable *CreateTable(char *name)=0;
    //在数据库中创建视图
    virtual IZYTable *CreateView(char *name)=0;
    //在数据库中撤消表格
    virtual void DropTable(IZYTable *table)=0;
    //在数据库中撤消表格
    virtual void DropTable(char *name)=0;
    //在数据库中创建索引
    virtual IZYIndex *CreateIndex(char *name,IZYTable *table,char *akey,char *pkey)=0;
    //备份数据库
    virtual void Backup(char *dest_folder,void (*p)(double percent))=0;
    //输出数据库到屏幕
    virtual void Print(void)=0;
};

//表格对象
class IZYTable
{
public:
    //获取表格名称
    virtual void GetName(char *name)=0;
    //获取表格目录(向上兼容)
    virtual void GetDir(char *dir)=0;
    //获取表格属性
    virtual int GetAttr(void)=0;
    //获取指定表格的行数
    virtual int GetRowCount(void)=0;
    //获取指定表指定数据项,使用列索引
    virtual void GetData(int i,int j,void *data)=0;
    //获取指定表指定数据项,使用列名称
    virtual void GetData(int i,char *key,void *data)=0;
    //设置指定表指定数据项,使用列索引
    virtual void SetData(int i,int j,void *data)=0;
    //设置指定表指定数据项,使用列名称
    virtual void SetData(int i,char *key,void *data)=0;
    //追加表格数据行
    virtual int AppendRow(void)=0;
    //插入表格数据行
    virtual int InsertRow(int i)=0;
    //删除表格数据行
    virtual void DeleteRow(int i)=0;
    //查找指定的数据行,返回数据项索引
    virtual int FindRow(int j,void *data)=0;
    //清空表格数据行
    virtual void ClearRows(void)=0;
    //获取将生成的自动编号
    virtual int GetAutoid(void)=0;
    //新生成一个自动编号
    virtual int NewAutoid(void)=0;
    //获取指定表格的列数
    virtual int GetColumnCount(void)=0;
    //查找指定表的指定列索引,未找到返回-1
    virtual int FindColumn(char *name)=0;
    //删除表格列
    virtual void DeleteColumn(int j)=0;
    //删除表格列
    virtual void DeleteColumn(char *name)=0;
    //插入表格列
    virtual int InsertColumn1(int j,char name[],EColumnType type,int length,int attribute)=0;
    //插入表格列(改进版本)
    virtual int InsertColumn(int j,char name[],EColumnType type,int length,int attribute)=0;
    //追加表格列
    virtual int AppendColumn1(char name[],EColumnType type,int length,int attribute)=0;
    //追加表格列(改进版本)
    virtual int AppendColumn(char name[],EColumnType type,int length,int attribute)=0;
    //获取表格列
    virtual void GetColumn(int j,char *aspect,void *data)=0;
    //获取表格列
    virtual void GetColumn(char *name,char *aspect,void *data)=0;
    //修改表格列
    virtual void SetColumn(int j,char *aspect,void *data)=0;
    //修改表格列
    virtual void SetColumn(char *name,char *aspect,void *data)=0;
    //复制表格列
    virtual int CopyColumn(IZYTable *source,char *column_name)=0;
    //清空表格列
    virtual void ClearColumns(void)=0;
    //进入缓冲方式
    virtual void EnterBufferedMode(void)=0;
    //离开缓冲方式
    virtual void LeaveBufferedMode(void)=0;
    //按指定列对表格进行选择排序
    virtual void SelectSort(int j1,ESortOrder sortOrder)=0;
    //按指定列对表格进行选择排序
    virtual void SelectSort(char *key,ESortOrder sortOrder)=0;
    //输出表格到屏幕
    virtual void Print(void)=0;
    //是否视图表
    virtual bool IsView(void)=0;
    //是否索引表
    virtual bool IsIndex(void)=0;
    //设置主键
    virtual void SetPrimaryKey(int j)=0;
    //设置主键
    virtual void SetPrimaryKey(char *name)=0;
    //获取主键
    virtual int GetPrimaryKey(void)=0;
    //获取辅键上的索引
    virtual IZYIndex *GetIndexOn(int j)=0;
    //获取辅键上的索引
    virtual IZYIndex *GetIndexOn(char *name)=0;
};

//索引对象
class IZYIndex
{
public:
    //转换为表格对象
    virtual IZYTable *ToTable(void)=0;
    //查找索引项
    virtual int FindIndex(void *adata)=0;
    //是否辅键值的索引项
    virtual bool IsIndexOf(int i,void *adata)=0;
    //获取索引项
    virtual void GetIndex(int i,void *pdata)=0;
};

//创建日期时间对象
IZYDateTime *IZYDateTime_Create(int year,int month,int day);

//释放日期时间对象
void IZYDateTime_Release(IZYDateTime *datetime);

//获取日期时间对象
IZYDateTime *IZYDateTime_Today(void);

//日期时间对象
class IZYDateTime
{
public:
    //获取年信息
    virtual int GetYear(void)=0;
    //获取月信息
    virtual int GetMonth(void)=0;
    //获取日信息
    virtual int GetDay(void)=0;
    //获取时信息
    virtual int GetHour(void)=0;
    //获取分信息
    virtual int GetMinute(void)=0;
    //获取秒信息
    virtual int GetSecond(void)=0;
    //计算日期之间的间隔
    virtual int Substract(IZYDateTime *datetime)=0;
};

//创建随机数发生器对象
IZYRandom *IZYRandom_Create(void);

//释放随机数发生器对象
void IZYRandom_Release(IZYRandom *random);

//随机数发生器对象
class IZYRandom
{
public:
    //生成属于[min,max]区间的随机整数
    virtual int RandInt(int min,int max)=0;
    //生成属于[min,max]区间的随机小数
    virtual double RandDouble(double min,double max)=0;
};

//创建内存空间对象
IZYMemory *IZYMemory_Create(int size);

//释放内存空间对象
void IZYMemory_Release(IZYMemory *memory);

//内存空间对象
class IZYMemory
{
public:
    //获取内存空间指针
    virtual void *GetPointer(void)=0;
    //获取内存空间大小
    virtual int GetSize(void)=0;
    //调整内存空间大小
    virtual void *Resize(int size)=0;
};

#endif