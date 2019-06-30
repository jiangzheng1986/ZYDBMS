//---------------------------------------------------------
//���´洢����(ZYRDB) ����:����
//---------------------------------------------------------
#if !defined(ZYRDB_HPP)        //����ZYRDB1_HPP_��
#define ZYRDB_HPP              //ʹ��ZYRDB1.HPP������1��

//����32λWindows API��
#include <windows.h>

//������
enum EColumnAttr
{
    //������-��������������
    EColumnAttr_IndexedA=8,
    //������-��������������
    EColumnAttr_IndexedP=16
};

//������
enum EColumnType
{
    //������-�ַ���
    EColumnType_String=0,
    //������-˫���ȸ���
    EColumnType_Double=1,
    //������-����
    EColumnType_Integer=2
};

//����˳��
enum ESortOrder
{
    //����������
    ESortOrder_Asc=0,
    //����������
    ESortOrder_Desc=1
};

//�������
class IZYTable;

//��������
class IZYIndex;

//���ݿ����
class IZYDatabase;

//����ʱ�����
class IZYDateTime;

//���������
class IZYRandom;

//�ڴ�ռ����
class IZYMemory;

//�������ݿ�Ĵ�����
bool IZYDatabase_Exists(char *path);

//�������ݿ��Ƿ��ѱ���
bool IZYDatabase_Opened(char *path);

//�������ݿ�
IZYDatabase *IZYDatabase_Create(char *path,char *name,char *userid,char *password);

//�����ݿ�
IZYDatabase *IZYDatabase_Open(char *path,char *name,char *userid,char *password);

//�ͷ����ݿ�
void IZYDatabase_Close(IZYDatabase *database);

//�������ݿ�
void IZYDatabase_Drop(IZYDatabase *database);

//�������ݿ�
IZYDatabase *IZYDatabase_Restore(IZYDatabase *database,char *dest_path,char *dest_name,
                        char *dest_userid,char *dest_password,void (*p)(double percent));

//���ݿ����
class IZYDatabase
{
public:
    //��ȡ���ݿ��еı�����Ŀ
    virtual int GetTableCount(void)=0;
    //��ȡ���ݿ��еı���
    virtual IZYTable *GetTableI(int t)=0;
    //��ȡ���ݿ��еı���
    virtual IZYTable *GetTableI(char *name)=0;
    //�����ݿ��д�������
    virtual IZYTable *CreateTable(char *name)=0;
    //�����ݿ��д�����ͼ
    virtual IZYTable *CreateView(char *name)=0;
    //�����ݿ��г�������
    virtual void DropTable(IZYTable *table)=0;
    //�����ݿ��г�������
    virtual void DropTable(char *name)=0;
    //�����ݿ��д�������
    virtual IZYIndex *CreateIndex(char *name,IZYTable *table,char *akey,char *pkey)=0;
    //�������ݿ�
    virtual void Backup(char *dest_folder,void (*p)(double percent))=0;
    //������ݿ⵽��Ļ
    virtual void Print(void)=0;
};

//�������
class IZYTable
{
public:
    //��ȡ��������
    virtual void GetName(char *name)=0;
    //��ȡ����Ŀ¼(���ϼ���)
    virtual void GetDir(char *dir)=0;
    //��ȡ��������
    virtual int GetAttr(void)=0;
    //��ȡָ�����������
    virtual int GetRowCount(void)=0;
    //��ȡָ����ָ��������,ʹ��������
    virtual void GetData(int i,int j,void *data)=0;
    //��ȡָ����ָ��������,ʹ��������
    virtual void GetData(int i,char *key,void *data)=0;
    //����ָ����ָ��������,ʹ��������
    virtual void SetData(int i,int j,void *data)=0;
    //����ָ����ָ��������,ʹ��������
    virtual void SetData(int i,char *key,void *data)=0;
    //׷�ӱ���������
    virtual int AppendRow(void)=0;
    //�������������
    virtual int InsertRow(int i)=0;
    //ɾ������������
    virtual void DeleteRow(int i)=0;
    //����ָ����������,��������������
    virtual int FindRow(int j,void *data)=0;
    //��ձ���������
    virtual void ClearRows(void)=0;
    //��ȡ�����ɵ��Զ����
    virtual int GetAutoid(void)=0;
    //������һ���Զ����
    virtual int NewAutoid(void)=0;
    //��ȡָ�����������
    virtual int GetColumnCount(void)=0;
    //����ָ������ָ��������,δ�ҵ�����-1
    virtual int FindColumn(char *name)=0;
    //ɾ��������
    virtual void DeleteColumn(int j)=0;
    //ɾ��������
    virtual void DeleteColumn(char *name)=0;
    //���������
    virtual int InsertColumn1(int j,char name[],EColumnType type,int length,int attribute)=0;
    //���������(�Ľ��汾)
    virtual int InsertColumn(int j,char name[],EColumnType type,int length,int attribute)=0;
    //׷�ӱ�����
    virtual int AppendColumn1(char name[],EColumnType type,int length,int attribute)=0;
    //׷�ӱ�����(�Ľ��汾)
    virtual int AppendColumn(char name[],EColumnType type,int length,int attribute)=0;
    //��ȡ������
    virtual void GetColumn(int j,char *aspect,void *data)=0;
    //��ȡ������
    virtual void GetColumn(char *name,char *aspect,void *data)=0;
    //�޸ı�����
    virtual void SetColumn(int j,char *aspect,void *data)=0;
    //�޸ı�����
    virtual void SetColumn(char *name,char *aspect,void *data)=0;
    //���Ʊ�����
    virtual int CopyColumn(IZYTable *source,char *column_name)=0;
    //��ձ�����
    virtual void ClearColumns(void)=0;
    //���뻺�巽ʽ
    virtual void EnterBufferedMode(void)=0;
    //�뿪���巽ʽ
    virtual void LeaveBufferedMode(void)=0;
    //��ָ���жԱ������ѡ������
    virtual void SelectSort(int j1,ESortOrder sortOrder)=0;
    //��ָ���жԱ������ѡ������
    virtual void SelectSort(char *key,ESortOrder sortOrder)=0;
    //���������Ļ
    virtual void Print(void)=0;
    //�Ƿ���ͼ��
    virtual bool IsView(void)=0;
    //�Ƿ�������
    virtual bool IsIndex(void)=0;
    //��������
    virtual void SetPrimaryKey(int j)=0;
    //��������
    virtual void SetPrimaryKey(char *name)=0;
    //��ȡ����
    virtual int GetPrimaryKey(void)=0;
    //��ȡ�����ϵ�����
    virtual IZYIndex *GetIndexOn(int j)=0;
    //��ȡ�����ϵ�����
    virtual IZYIndex *GetIndexOn(char *name)=0;
};

//��������
class IZYIndex
{
public:
    //ת��Ϊ�������
    virtual IZYTable *ToTable(void)=0;
    //����������
    virtual int FindIndex(void *adata)=0;
    //�Ƿ񸨼�ֵ��������
    virtual bool IsIndexOf(int i,void *adata)=0;
    //��ȡ������
    virtual void GetIndex(int i,void *pdata)=0;
};

//��������ʱ�����
IZYDateTime *IZYDateTime_Create(int year,int month,int day);

//�ͷ�����ʱ�����
void IZYDateTime_Release(IZYDateTime *datetime);

//��ȡ����ʱ�����
IZYDateTime *IZYDateTime_Today(void);

//����ʱ�����
class IZYDateTime
{
public:
    //��ȡ����Ϣ
    virtual int GetYear(void)=0;
    //��ȡ����Ϣ
    virtual int GetMonth(void)=0;
    //��ȡ����Ϣ
    virtual int GetDay(void)=0;
    //��ȡʱ��Ϣ
    virtual int GetHour(void)=0;
    //��ȡ����Ϣ
    virtual int GetMinute(void)=0;
    //��ȡ����Ϣ
    virtual int GetSecond(void)=0;
    //��������֮��ļ��
    virtual int Substract(IZYDateTime *datetime)=0;
};

//�������������������
IZYRandom *IZYRandom_Create(void);

//�ͷ����������������
void IZYRandom_Release(IZYRandom *random);

//���������������
class IZYRandom
{
public:
    //��������[min,max]������������
    virtual int RandInt(int min,int max)=0;
    //��������[min,max]��������С��
    virtual double RandDouble(double min,double max)=0;
};

//�����ڴ�ռ����
IZYMemory *IZYMemory_Create(int size);

//�ͷ��ڴ�ռ����
void IZYMemory_Release(IZYMemory *memory);

//�ڴ�ռ����
class IZYMemory
{
public:
    //��ȡ�ڴ�ռ�ָ��
    virtual void *GetPointer(void)=0;
    //��ȡ�ڴ�ռ��С
    virtual int GetSize(void)=0;
    //�����ڴ�ռ��С
    virtual void *Resize(int size)=0;
};

#endif