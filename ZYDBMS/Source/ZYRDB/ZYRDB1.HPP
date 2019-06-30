//---------------------------------------------------------
//���´洢����(ZYRDB) ����:����
//---------------------------------------------------------
#if !defined(ZYRDB1_HPP)        //����ZYRDB1_HPP_��
#define ZYRDB1_HPP              //ʹ��ZYRDB1.HPP������1��

//����32λWindows API��
#include <windows.h>

//for PathFileExsits
#include <shlwapi.h>

//����C++���������
#include <iostream>

//����C++�����㷨��
#include <algorithm>

//����C++����������
#include <vector>

//Ӧ��C++��׼���ƿռ�
using namespace std;

//�������´洢�����
#include "ZYRDB.HPP"

//���ݿ������ַ�����󳤶�
#define DATABASE_NAME_LEN 128

//�û����ַ�����󳤶�
#define USERID_LEN 32

//�����ַ�����󳤶�
#define PASSWORD_LEN 32

//ÿ�������е������Ŀ
#define MAX_COLUMN 32

//���������е���󳤶�
#define ROW_LEN 32000

//�����������׼�������ĳ���
#define DATA_LEN 1000

//����Ŀ¼���ַ�����󳤶�
#define TABLE_DIR_LEN 16

//����������ַ�����󳤶�
#define TABLE_TAB_LEN 16

//�������ַ�����󳤶�
#define COLUMN_NAME_LEN 16

//��������-������
#define TABLE_ATTR_VIEW         0x00000001

//��������-������
#define TABLE_ATTR_INDEX        0x00000002

//������-��������������
#define COLUMN_ATTR_INDEXEDA    0x00000008

//������-��������������
#define COLUMN_ATTR_INDEXEDP    0x00000010

class ZYFile
{
public:
    //�����ļ�����
    ZYFile(char *filename);
    //�����ļ�����
    ~ZYFile(void);
    //�Կɶ�д��ʽ��һ���ļ�
    void OpenFile(void);
    //�ر�һ���Ѵ򿪵��ļ�
    void CloseFile(void);
    //�ļ��ɹ���
    bool FileOpened(void);
    //��ȡ�ļ�����
    void ReadFile(int fp,void *p,int n);
    //д���ļ�����
    void WriteFile(int fp,void *p,int n);
    //ˢ���ļ�������
    virtual void FlushFile(void);
    //���ļ��ضϵ���������
    void SetEndOfFile(int fp);
    //��ȡ�ļ�����
    int GetFileSize(void);
    //�ļ��Ƿ����
    static bool Exists(char *filename);
private:
    //�����ݽ��м���
    void EncriptData(void *p,int n);
    //�����ݽ��н���
    void UnencriptData(void *p,int n);
    //�����ļ���
    char *filename;
    //�����ļ����
    HANDLE hFile;
};

//���ݿ����
class ZYDatabase;

//���ݿ��ļ�����
class ZYDatabaseFile:public ZYFile
{
public:
    //�������ݿ��ļ�����
    ZYDatabaseFile(ZYDatabase *database,char *filename);
    //�������ݿ��ļ�����
    void LoadData(void);
    //�������ݿ��ļ�����
    void SaveData(void);
    //��ȡ���ݿ��ļ�ͷ
    void ReadHeader(void);
    //д�����ݿ��ļ�ͷ
    void WriteHeader(void);
    //��ȡ���ݿ��ļ�������Ϣ
    void ReadIndex(int j);
    //д�����ݿ��ļ�������Ϣ
    void WriteIndex(int j);
    //��ȡ���ݿ��ļ�ȫ��������Ϣ
    void ReadIndexs(void);
    //д�����ݿ��ļ�ȫ��������Ϣ
    void WriteIndexs(void);
    //�����ݿ��ļ��ضϵ���������
    void SetEndOfFile(void);
private:        
    //���ݿ����
    ZYDatabase *database;
};

//�������
class ZYTable;

//��������
class ZYIndex;

//������������
class ZYIndexDecl
{
public:
    //����������������
    ZYIndexDecl(void);
    //����������������
    ZYIndexDecl(char *index_name,char *table_name,char *akey,char *pkey);
    //����������
    char index_name[TABLE_TAB_LEN];
    //������������
    char table_name[TABLE_TAB_LEN];
    //��������
    char akey[COLUMN_NAME_LEN];
    //��������
    char pkey[COLUMN_NAME_LEN];
};

//���ݿ����
class ZYDatabase:public IZYDatabase
{
public:
    //�����ݿ�
    static ZYDatabase *Open(char *path,char *name,char *userid,char *password);
    //�������ݿ�
    static ZYDatabase *Create(char *path,char *name,char *userid,char *password);
    //�������ݿ�
    static ZYDatabase *Restore(ZYDatabase *database,char *dest_path,char *dest_name,
                            char *dest_userid,char *dest_password,void (*p)(double percent));
    //�������ݿ�
    static void Drop(ZYDatabase *database);
    //�������ݿ����
    ZYDatabase(void);
    //�������ݿ����
    ~ZYDatabase(void);
    //�������ݿ�Ĵ�����
    static bool Exists(char *path);
    //�������ݿ��Ƿ��ѱ���
    static bool Opened(char *path);
    //�������ݿ��ļ���
    void NewDatabase(char *path,char *name,char *userid,char *password);
    //�������ݿ��ļ���
    bool LoadDatabase(char *path,char *name,char *userid,char *password);
    //��ȡ���ݿ��еı�����Ŀ
    int GetTableCount(void);
    //��ȡ���ݿ��еı���
    ZYTable *GetTable(int t);
    //��ȡ���ݿ��еı���
    ZYTable *GetTable(char *name);
    //��ȡ���ݿ��еı���
    IZYTable *GetTableI(int t);
    //��ȡ���ݿ��еı���
    IZYTable *GetTableI(char *name);
    //�����ݿ��д�������
    IZYTable *CreateTable(char *name);
    //�����ݿ��д�����ͼ
    IZYTable *CreateView(char *name);
    //�����ݿ��г�������
    void DropTable(ZYTable *table);
    //�����ݿ��г�������
    void DropTable(IZYTable *table);
    //�����ݿ��г�������
    void DropTable(char *name);     
    //�����ݿ��д�������
    ZYIndex *CreateIndex(char *name,ZYTable *table,char *akey,char *pkey);
    //�����ݿ��д�������
    IZYIndex *CreateIndex(char *name,IZYTable *table,char *akey,char *pkey);
    //�������ݿ�
    void Backup(char *dest_folder,void (*p)(double percent));
    //������ݿ⵽��Ļ
    void Print(void);
private:
    //����ZYDatabaseFileΪ��Ԫ��
    friend class ZYDatabaseFile;
    //�������ݿ��ļ���������������ļ�
    void LoadTables(void);
    //�������ݿ��е���������
    void LoadIndexs(void);
    //����ȫ��Ҫ�����ļ����ܼƳ���
    int CountFileSize(void);
    //���һ�������ļ������
    int latest_table;
    //����Ŀ¼��·��
    char *path;
    //���ݿ����
    vector<ZYTable *> *tables;
    //���ݿ��ļ�����
    ZYDatabaseFile *databaseFile;
    //�ļ�����
    int file_type;
    //�ļ��汾
    int file_version;
    //���ݿ�����
    char name[DATABASE_NAME_LEN];
    //�û���
    char userid[USERID_LEN];
    //����
    char password[PASSWORD_LEN];
    //�����Զ����
    int table_autoid;
    //���ݿ�����
    int database_attr;
    //������������
    int index_cnt;
    //����˵������
    int index_size;
    //���������б�
    vector<ZYIndexDecl *> *indexs;
};

//�����ж���
class ZYColumn
{
public:
    //��ȡ������
    void GetName(char *name);
    //��ȡ������
    EColumnType GetType(void);
    //��ȡ�г���
    int GetLength(void);
    //��ȡ������
    int GetAttribute(void);
private:
    //����ZYTableΪ��Ԫ��
    friend class ZYTable;
    //����ZYTableFileΪ��Ԫ��
    friend class ZYTableFile;
    //������
    char name[COLUMN_NAME_LEN];
    //������
    EColumnType type;
    //�г���
    int length;
    //��ƫ��
    int offset;
    //������
    int attribute;
};

//�����ж���
class ZYRow
{
public:
    //�����ļ�����������
    int i;
    //���������ݻ�����
    char row_data[ROW_LEN];
};

//�����ļ�����
class ZYTableFile:public ZYFile
{
public:
    //��������ļ�����
    ZYTableFile(ZYTable *table,char *filename);
    //ˢ���ļ�������
    void FlushFile(void);
    //���ر����ļ�����
    void LoadData(void);
    //��ȡ�����ļ�ͷ
    void ReadHeader(void);
    //д������ļ�ͷ
    void WriteHeader(void);
    //��ȡ�����ļ�����Ϣ
    void ReadColumn(int j);
    //д������ļ�����Ϣ
    void WriteColumn(int j);
    //��ȡ�����ļ�ȫ������Ϣ
    void ReadColumns(void);
    //д������ļ�ȫ������Ϣ
    void WriteColumns(void);
    //��ȡ�����ļ�������(����)
    void ReadRow(int i);
    //д������ļ�������(����)
    void WriteRow(int i);
    //��������ļ�������(����)
    void InsertRow(int i);
    //ɾ�������ļ�������(����)
    void DeleteRow(int i);
    //�������ļ��ضϵ���������
    void SetEndOfFile(void);
private:        
    //�������
    ZYTable *table;
    //������������ʼ��ַ
    int RowDataBase;
};

//�������
class ZYTable:public IZYTable
{
public:
    //��ȡ��������
    void GetName(char *name);
    //��ȡ����Ŀ¼(���ϼ���)
    void GetDir(char *dir);
    //��ȡ��������
    int GetAttr(void);
    //��ȡָ�����������
    int GetRowCount(void);
    //��ȡָ����ָ��������,ʹ��������
    void GetData(int i,int j,void *data);
    //��ȡָ����ָ��������,ʹ��������
    void GetData(int i,char *key,void *data);
    //����ָ����ָ��������,ʹ��������
    void SetData(int i,int j,void *data);
    //����ָ����ָ��������,ʹ��������
    void SetData(int i,char *key,void *data);
    //׷�ӱ���������
    int AppendRow(void);
    //�������������
    int InsertRow(int i);
    //ɾ������������
    void DeleteRow(int i);
    //����ָ����������,��������������
    int FindRow(int j,void *data);
    //��ձ���������
    void ClearRows(void);
    //��ȡ�����ɵ��Զ����
    int GetAutoid(void);
    //������һ���Զ����
    int NewAutoid(void);
    //��ȡָ�����������
    int GetColumnCount(void);
    //��ȡָ���ı�����
    ZYColumn *GetColumn(int j);
    //��ȡָ���ı�����
    ZYColumn *GetColumn(char *name);
    //����ָ������ָ��������,δ�ҵ�����-1
    int FindColumn(char *name);
    //ɾ��������
    void DeleteColumn(int j);
    //ɾ��������
    void DeleteColumn(char *name);
    //���������
    int InsertColumn1(int j,char name[],EColumnType type,int length,int attribute);
    //���������(�Ľ��汾)
    int InsertColumn(int j,char name[],EColumnType type,int length,int attribute);
    //׷�ӱ�����
    int AppendColumn1(char name[],EColumnType type,int length,int attribute);
    //׷�ӱ�����(�Ľ��汾)
    int AppendColumn(char name[],EColumnType type,int length,int attribute);
    //��ȡ������
    void GetColumn(int j,char *aspect,void *data);
    //��ȡ������
    void GetColumn(char *name,char *aspect,void *data);
    //�޸ı�����
    void SetColumn(int j,char *aspect,void *data);
    //�޸ı�����
    void SetColumn(char *name,char *aspect,void *data);
    //���Ʊ�����
    int CopyColumn(ZYTable *source,char *column_name);
    //���Ʊ�����
    int CopyColumn(IZYTable *source,char *column_name);
    //��ձ�����
    void ClearColumns(void);
    //���뻺�巽ʽ
    void EnterBufferedMode(void);
    //�뿪���巽ʽ
    void LeaveBufferedMode(void);
    //��ָ���жԱ������ѡ������
    void SelectSort(int j1,ESortOrder sortOrder);
    //��ָ���жԱ������ѡ������
    void SelectSort(char *key,ESortOrder sortOrder);
    //���������Ļ
    void Print(void);
    //�Ƿ���ͼ��
    bool IsView(void);
    //�Ƿ�������
    bool IsIndex(void);
    //��������
    void SetPrimaryKey(int j);
    //��������
    void SetPrimaryKey(char *name);
    //��ȡ����
    int GetPrimaryKey(void);
    //��ȡ�����ϵ�����
    IZYIndex *GetIndexOn(int j);
    //��ȡ�����ϵ�����
    IZYIndex *GetIndexOn(char *name);
protected:
    //����ZYDatabaseΪ��Ԫ��
    friend class ZYDatabase;
    //����ZYTableFileΪ��Ԫ��
    friend class ZYTableFile;
    //����ZYIndexΪ��Ԫ��
    friend class ZYIndex;
    //����������(��)
    ZYTable(char *filename);
    //����������(�½�)
    ZYTable(char *filename,char *tab);
    //�����������
    virtual ~ZYTable(void);
    //��������
    void Drop(void);    
    //��ȡָ���ı���������ָ��
    void *GetDataPointer(int i,int j);
    //��ȡָ���ı���������ָ��(����InsertRow)
    void *GetDataPointer1(int i,int j);
    //�Ƚϱ���������ָ��ֵ
    int CompareRow1(void *data,int i,int j);
    //˳�����ָ����������,��������������
    int FindRow1(int j,void *data);
    //���ֲ���ָ����������,��������������
    int FindRow2(int j,void *data);
    //���ֲ���ָ����������,��������������(����SetData)
    int FindRow3(int j,void *data);
    //��ȡָ���ı�����(����InsertColumn)
    ZYColumn *GetColumn1(int j);
    //����ָ����������Ե�ƫ��
    void CalcOffset(void);
    //�������Ƶ���ʱ����
    ZYTable *CopyToTemp(void);
    //����ʱ����������װ���������
    void TransferBack(ZYTable *temp);
    //���������е�����
    void SwapRow(int i1,int i2);
    //�Ƚϱ��������
    int CompareRow(int j,int i1,int i2);
    //�������ݿ��е�һ������
    void Backup(char *dest_path,int total_size,int *bytes_copied,void (*p)(double percent));
    //�ļ�����
    int file_type;
    //�����ļ��汾
    int file_version;
    //��������
    int table_attr;
    //�м���
    int column_cnt;
    //�г���
    int column_size;
    //�м���
    int row_cnt;
    //�г���
    int row_size;
    //���Զ����
    int row_autoid;
    //�������Ŀ
    int row_max;
    //����Ŀ¼��
    char dir[TABLE_DIR_LEN];
    //���������
    char tab[TABLE_TAB_LEN];
    //����Ϣ���ݻ�����
    ZYColumn columns[MAX_COLUMN];
    //���������ݻ�����
    ZYRow row;       
    //�����ļ�����
    ZYTableFile *tableFile;
    //�������ڵ��ļ���
    char *filename;
    //��������������Ե����������
    ZYIndex *indexs[MAX_COLUMN];  
    //�Ƿ����FlushFile��־
    bool bFlush;
};

//��������
class ZYIndex:public ZYTable,public IZYIndex
{
public:
    //ת��Ϊ�������
    IZYTable *ToTable(void);
    //����������
    int FindIndex(void *adata);
    //����������
    int FindIndex1(void *adata,void *pdata);
    //����������
    int InsertIndex(void *adata,void *pdata);
    //�Ƿ񸨼�ֵ��������
    bool IsIndexOf(int i,void *adata);
    //��ȡ������
    void GetIndex(int i,void *pdata);
    //ɾ��������
    void DeleteIndex(void *adata,void *pdata);
    //�޸�������(AKEY�仯)
    void SetIndex(void *adata1,void *adata2,void *pdata);
    //�޸�������(PKEY�仯)
    void SetIndex1(void *adata,void *pdata1,void *pdata2);
private:
    //����ZYDatabaseΪ��Ԫ��
    friend class ZYDatabase;
    //����ZYTableΪ��Ԫ��
    friend class ZYTable;
    //������������(��)
    ZYIndex(char *filename,ZYTable *table,char *akey,char *pkey);
    //������������(����)
    ZYIndex(char *filename,char *name,ZYTable *table,char *akey,char *pkey);
    //�����Ƿ��Զ�����
    static bool index_update;
    //�����Ƿ��Զ�����
    static bool IfIndexUpdate(void);
    //���������Զ�����
    static void EnableIndexUpdate(void);
    //���������Զ�����
    static void DisableIndexUpdate(void);
    //��������,����AppendRow
    static void IndexUpdate1(ZYTable *table,int i);
    //��������,����DeleteRow
    static void IndexUpdate2(ZYTable *table,int i);
    //��������,����SetData
    static void IndexUpdate3(ZYTable *table,int i,int j,void *data);
    //��������,����ClearRows
    static void IndexUpdate4(ZYTable *table);
};

//����ʱ�����
class ZYDateTime:public IZYDateTime
{
public:
    //��������ʱ�����
    ZYDateTime(void);
    //��������ʱ�����
    ZYDateTime(int year,int month,int day);
    //��ȡ����Ϣ
    int GetYear(void);
    //��ȡ����Ϣ
    int GetMonth(void);
    //��ȡ����Ϣ
    int GetDay(void);
    //��ȡʱ��Ϣ
    int GetHour(void);
    //��ȡ����Ϣ
    int GetMinute(void);
    //��ȡ����Ϣ
    int GetSecond(void);
    //��������֮��ļ��
    int Substract(IZYDateTime *datetime);
private:
    //�Ƿ�������
    bool IsRunNian(int year);
    //���㵽1980.01.01������
    int Substract1980(void);
    //ϵͳ����ʱ��ṹ
    SYSTEMTIME datetime;
};

//���������������
class ZYRandom:public IZYRandom
{
public:
    //�������������������
    ZYRandom(void);
    //��������[min,max]������������
    int RandInt(int min,int max);
    //��������[min,max]��������С��
    double RandDouble(double min,double max);
};

//���ݿ��ַ�������
class ZYString
{
public:
    //����һ���䳤�ַ����������ַ�����
    static void Copy1(char *dest,char *src,int cnt);
    //����һ�������ַ������䳤�ַ�����
    static void Copy2(char *dest,char *src,int cnt);
    //��һ�������ַ�����䳤�ַ����Ƚ�,�ж��Ƿ����
    static int Compare(char *sa,char *sb,int cnt);
    //����һ���ַ���
    static char *Copy(char *s);
    //�ͷŸ��Ƶ��ַ���
    static void Free(char *s);
};

//�ڴ�ռ����
class ZYMemory:public IZYMemory
{
public:
    //�����ڴ�ռ����
    ZYMemory(int size);
    //�����ڴ�ռ����
    ~ZYMemory(void);
    //��ȡ�ڴ�ռ�ָ��
    void *GetPointer(void);
    //��ȡ�ڴ�ռ��С
    int GetSize(void);
    //�����ڴ�ռ��С
    void *Resize(int size);
private:
    //�����ڴ�ռ�
    static void *ApplyMemory(int byte_count);
    //��ȡ�ڴ�ռ��С
    static int SizeOfMemory(void *lp);
    //�����ڴ�ռ��С
    static void *ResizeMemory(void *lp,int byte_count);
    //�ͷ��ڴ�ռ�
    static void FreeMemory(void *lp);
    //�ڴ�ռ�ָ��
    void *lp;
    //�ڴ�ռ��С
    int size;
};

#endif