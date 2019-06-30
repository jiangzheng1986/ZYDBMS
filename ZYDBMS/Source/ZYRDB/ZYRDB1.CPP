//---------------------------------------------------------
//���´洢����(ZYRDB)-�����ļ� ����:����
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//���Ŀ�꣺
//1��������󡢽ṹ���á�����չ
//2���ȶ����ɿ����򵥡���Ч

//���ݿ�˵���ļ���ʽ��(.dbd,Database Description File)
//�ļ���Ŀ        ��ʼ�ֽ�  ��ʼ��ַ    ����    Ĭ��ֵ
//1���ļ�����     ��0�ֽ�   0x00000000  4�ֽ�   ='ZYDD'
//2���ļ��汾     ��4�ֽ�   0x00000004  4�ֽ�   =2
//3�����ݿ�����   ��8�ֽ�   0x00000008  128�ֽ�
//4���û���       ��136�ֽ� 0x00000088  32�ֽ�
//5������         ��168�ֽ� 0x000000A8  32�ֽ�
//6�������Զ���� ��200�ֽ� 0x000000C8  4�ֽ�   =0
//7�����ݿ�����   ��204�ֽ� 0x000000CC  4�ֽ�   
//8����������     ��208�ֽ� 0x000000D0  4�ֽ�
//9����������     ��212�ֽ� 0x000000D4  4�ֽ�
//10�������ֽ�    ��216�ֽ� 0x000000D8  244�ֽ� =244 DUP(0)
//11�������б�    ��460�ֽ� 0x000001CC  ??*64�ֽ�

//ÿһ�����������Ľṹ��
//1������������   ��0�ֽ�   0x00000000  16�ֽ�
//2�������������� ��16�ֽ�  0x00000010  16�ֽ�
//3����������     ��32�ֽ�  0x00000020  16�ֽ�
//4����������     ��48�ֽ�  0x00000030  16�ֽ�

//���ݿ�����ļ���ʽ��(.dat,Database ZYTable File)
//�ļ���Ŀ        ��ʼ�ֽ�  ��ʼ��ַ    ����    Ĭ��ֵ
//1���ļ�����     ��0�ֽ�   0x00000000  4�ֽ�   ='ZYDB'
//2���ļ��汾     ��4�ֽ�   0x00000004  4�ֽ�   =2
//3����������     ��8�ֽ�   0x00000008  4�ֽ�
//4���м���       ��12�ֽ�  0x0000000C  4�ֽ�
//5���г���       ��16�ֽ�  0x00000010  4�ֽ�
//6���м���       ��20�ֽ�  0x00000014  4�ֽ�
//7���г���       ��24�ֽ�  0x00000018  4�ֽ�
//8�����Զ����   ��28�ֽ�  0x0000001C  4�ֽ�
//9������󳤶�   ��32�ֽ�  0x00000020  4�ֽ�
//10��Ŀ¼��      ��36�ֽ�  0x00000024  16�ֽ�
//11��������      ��52�ֽ�  0x00000034  16�ֽ�
//12�����б�      ��68�ֽ�  0x00000044  ??*32�ֽ�
//13�����б�      ��??�ֽ�  0x000000??  ??*??�ֽ�

//ÿһ���еĽṹ��
//1��������       ��0�ֽ�   0x00000000  16�ֽ�
//2��������       ��16�ֽ�  0x00000010  4�ֽ�
//3���г���       ��20�ֽ�  0x00000014  4�ֽ�
//4����ƫ��       ��24�ֽ�  0x00000018  4�ֽ�
//5��������       ��28�ֽ�  0x0000001C  4�ֽ�

//�����ļ������Կ�ʼ��λ�õ�ƫ��
#define FILE_ATTR_OFFSET 0x00000044

//������Կ1
#define PATTERN1 (char)0x98

//������Կ2
#define PATTERN2 (char)0x62

//�����ļ�����
ZYFile::ZYFile(char *filename)
{
    this->filename=ZYString::Copy(filename);

    OpenFile();
}

//�����ļ�����
ZYFile::~ZYFile(void)
{
    ZYString::Free(filename);

    if(hFile!=NULL)
    {
        CloseFile();
    }
}

//�Կɶ�д��ʽ��һ���ļ�
void ZYFile::OpenFile(void)
{
    hFile=CreateFile(filename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
                     NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
}

//�ر�һ���Ѵ򿪵��ļ�
void ZYFile::CloseFile(void)
{
    CloseHandle(hFile);
    hFile=INVALID_HANDLE_VALUE;
}

//�ļ��ɹ���
bool ZYFile::FileOpened(void)
{
    return (hFile!=INVALID_HANDLE_VALUE)&&(hFile!=NULL);
}

//ˢ���ļ�������
void ZYFile::FlushFile(void)
{
    FlushFileBuffers(hFile);
}

//�����ݽ��м���
void ZYFile::EncriptData(void *p,int n)
{
    char *p1;

    p1=(char *)p;

    while(n>0)
    {
        (*p1)=(*p1)+PATTERN1;

        (*p1)=(*p1)^PATTERN2;

        p1++;

        n--;
    }
}

//�����ݽ��н���
void ZYFile::UnencriptData(void *p,int n)
{
    char *p1;

    p1=(char *)p;

    while(n>0)
    {
        (*p1)=(*p1)^PATTERN2;

        (*p1)=(*p1)-PATTERN1;

        p1++;

        n--;
    }
}

//��ȡ�ļ�����
void ZYFile::ReadFile(int fp,void *p,int n)
{
    unsigned long i;

    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::ReadFile(hFile,p,n,&i,NULL);

    //UnencriptData(p,n);
}

//д���ļ�����
void ZYFile::WriteFile(int fp,void *p,int n)
{
    unsigned long i;

    //EncriptData(p,n);

    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::WriteFile(hFile,p,n,&i,NULL);

    //UnencriptData(p,n);
}

//�������ļ��ضϵ���������
void ZYFile::SetEndOfFile(int fp)
{
    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::SetEndOfFile(hFile);
}

//�ļ��Ƿ����
bool ZYFile::Exists(char *filename)
{
    return PathFileExists(filename)==1;
}

//��ȡ�ļ�����
int ZYFile::GetFileSize(void)
{
    int a;
    BY_HANDLE_FILE_INFORMATION bhfi;
    
    GetFileInformationByHandle(hFile,&bhfi);
    
    if(bhfi.nFileSizeHigh==0)
    {
        a=bhfi.nFileSizeLow;
    }
    else
    {
        a=-1;
    }
    
    return a;
}

//�������ݿ��ļ�����
ZYDatabaseFile::ZYDatabaseFile(ZYDatabase *database,char *filename):ZYFile(filename)
{
    this->database=database;
}

//�������ݿ��ļ�����
void ZYDatabaseFile::LoadData(void)
{
    ReadHeader();

    ReadIndexs();
}

//�������ݿ��ļ�����
void ZYDatabaseFile::SaveData(void)
{
    WriteHeader();

    WriteIndexs();

    FlushFile();
}

//��ȡ���ݿ��ļ�ͷ
void ZYDatabaseFile::ReadHeader(void)
{
    ReadFile(0x00000000,&database->file_type,sizeof(int));
    ReadFile(0x00000004,&database->file_version,sizeof(int));
    ReadFile(0x00000008,&database->name,DATABASE_NAME_LEN);
    ReadFile(0x00000088,&database->userid,USERID_LEN);
    ReadFile(0x000000A8,&database->password,PASSWORD_LEN);
    ReadFile(0x000000C8,&database->table_autoid,sizeof(int));
    ReadFile(0x000000CC,&database->database_attr,sizeof(int));
    ReadFile(0x000000D0,&database->index_cnt,sizeof(int));
    ReadFile(0x000000D4,&database->index_size,sizeof(int));
}

//д�����ݿ��ļ�ͷ
void ZYDatabaseFile::WriteHeader(void)
{
    WriteFile(0x00000000,&database->file_type,sizeof(int));
    WriteFile(0x00000004,&database->file_version,sizeof(int));
    WriteFile(0x00000008,&database->name,DATABASE_NAME_LEN);
    WriteFile(0x00000088,&database->userid,USERID_LEN);
    WriteFile(0x000000A8,&database->password,PASSWORD_LEN);
    WriteFile(0x000000C8,&database->table_autoid,sizeof(int));
    WriteFile(0x000000CC,&database->database_attr,sizeof(int));
    WriteFile(0x000000D0,&database->index_cnt,sizeof(int));
    WriteFile(0x000000D4,&database->index_size,sizeof(int));
}

//��ȡ���ݿ��ļ�������Ϣ
void ZYDatabaseFile::ReadIndex(int j)
{
    int fp=0x000001CC+j*database->index_size;

    ZYIndexDecl *indexDecl=new ZYIndexDecl();

    ReadFile(fp+0x00000000,&(indexDecl->index_name),TABLE_TAB_LEN);
    ReadFile(fp+0x00000010,&(indexDecl->table_name),TABLE_TAB_LEN);
    ReadFile(fp+0x00000020,&(indexDecl->akey),COLUMN_NAME_LEN);
    ReadFile(fp+0x00000030,&(indexDecl->pkey),COLUMN_NAME_LEN);

    database->indexs->push_back(indexDecl);
}

//д�����ݿ��ļ�������Ϣ
void ZYDatabaseFile::WriteIndex(int j)
{
    int fp=0x000001CC+j*database->index_size;

    WriteFile(fp+0x00000000,&((*database->indexs)[j]->index_name),TABLE_TAB_LEN);
    WriteFile(fp+0x00000010,&((*database->indexs)[j]->table_name),TABLE_TAB_LEN);
    WriteFile(fp+0x00000020,&((*database->indexs)[j]->akey),COLUMN_NAME_LEN);
    WriteFile(fp+0x00000030,&((*database->indexs)[j]->pkey),COLUMN_NAME_LEN);
}

//��ȡ���ݿ��ļ�ȫ��������Ϣ
void ZYDatabaseFile::ReadIndexs(void)
{
    int j;

    for(j=0;j<database->index_cnt;j++)
    {
        ReadIndex(j);
    }
}

//д�����ݿ��ļ�ȫ��������Ϣ
void ZYDatabaseFile::WriteIndexs(void)
{
    int j;

    for(j=0;j<database->index_cnt;j++)
    {
        WriteIndex(j);
    }
}

//�����ݿ��ļ��ضϵ���������
void ZYDatabaseFile::SetEndOfFile(void)
{
    int fp=0x000001CC+database->index_cnt*database->index_size;

    ZYFile::SetEndOfFile(fp);
}

//��������ļ�����
ZYTableFile::ZYTableFile(ZYTable *table,char *filename):ZYFile(filename)
{
    this->table=table;
    this->table->row.i=-1;
    this->RowDataBase=0;    
}

//ˢ���ļ�������
void ZYTableFile::FlushFile(void)
{
    if((table->table_attr&TABLE_ATTR_VIEW)==0)
    {
        ZYFile::FlushFile();
    }
}

//���ر����ļ�����
void ZYTableFile::LoadData(void)
{
    ReadHeader();

    ReadColumns();
}

//��ȡ�����ļ�ͷ
void ZYTableFile::ReadHeader(void)
{
    ReadFile(0x00000000,&table->file_type,sizeof(int));
    ReadFile(0x00000004,&table->file_version,sizeof(int));
    ReadFile(0x00000008,&table->table_attr,sizeof(int));
    ReadFile(0x0000000C,&table->column_cnt,sizeof(int));
    ReadFile(0x00000010,&table->column_size,sizeof(int));
    ReadFile(0x00000014,&table->row_cnt,sizeof(int));
    ReadFile(0x00000018,&table->row_size,sizeof(int));
    ReadFile(0x0000001C,&table->row_autoid,sizeof(int));
    ReadFile(0x00000020,&table->row_max,sizeof(int));
    ReadFile(0x00000024,&table->dir,TABLE_DIR_LEN);
    ReadFile(0x00000034,&table->tab,TABLE_TAB_LEN);
}

//д������ļ�ͷ
void ZYTableFile::WriteHeader(void)
{
    WriteFile(0x00000000,&table->file_type,sizeof(int));
    WriteFile(0x00000004,&table->file_version,sizeof(int));
    WriteFile(0x00000008,&table->table_attr,sizeof(int));
    WriteFile(0x0000000C,&table->column_cnt,sizeof(int));
    WriteFile(0x00000010,&table->column_size,sizeof(int));
    WriteFile(0x00000014,&table->row_cnt,sizeof(int));
    WriteFile(0x00000018,&table->row_size,sizeof(int));
    WriteFile(0x0000001C,&table->row_autoid,sizeof(int));
    WriteFile(0x00000020,&table->row_max,sizeof(int));
    WriteFile(0x00000024,&table->dir,TABLE_DIR_LEN);
    WriteFile(0x00000034,&table->tab,TABLE_TAB_LEN);
}

//��ȡ�����ļ�����Ϣ
void ZYTableFile::ReadColumn(int j)
{
    int fp=FILE_ATTR_OFFSET+j*table->column_size;

    ReadFile(fp+0x00000000,&(table->columns[j].name),COLUMN_NAME_LEN);
    ReadFile(fp+0x00000010,&(table->columns[j].type),sizeof(int));
    ReadFile(fp+0x00000014,&(table->columns[j].length),sizeof(int));
    ReadFile(fp+0x00000018,&(table->columns[j].offset),sizeof(int));
    ReadFile(fp+0x0000001C,&(table->columns[j].attribute),sizeof(int));
}

//д������ļ�����Ϣ
void ZYTableFile::WriteColumn(int j)
{
    int fp=FILE_ATTR_OFFSET+j*table->column_size;

    WriteFile(fp+0x00000000,&(table->columns[j].name),COLUMN_NAME_LEN);
    WriteFile(fp+0x00000010,&(table->columns[j].type),sizeof(int));
    WriteFile(fp+0x00000014,&(table->columns[j].length),sizeof(int));
    WriteFile(fp+0x00000018,&(table->columns[j].offset),sizeof(int));
    WriteFile(fp+0x0000001C,&(table->columns[j].attribute),sizeof(int));
}

//��ȡ�����ļ�ȫ������Ϣ
void ZYTableFile::ReadColumns(void)
{
    int j;

    for(j=0;j<table->column_cnt;j++)
    {
        ReadColumn(j);
    }
}

//д������ļ�ȫ������Ϣ
void ZYTableFile::WriteColumns(void)
{
    int j;

    for(j=0;j<table->column_cnt;j++)
    {
        WriteColumn(j);
    }
}

//��ȡ�����ļ�������(����)
void ZYTableFile::ReadRow(int i)
{
    if(i!=table->row.i)
    {
        int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
        table->row.i=i;
        ReadFile(fp,&table->row.row_data,table->row_size);
    }
}

//д������ļ�������(����)
void ZYTableFile::WriteRow(int i)
{
    int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
    WriteFile(fp,&table->row.row_data,table->row_size);
}

//��������ļ�������(����)
void ZYTableFile::InsertRow(int i)
{
    int fp1=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
    int fp2=fp1+table->row_size;
    int n=(table->row_cnt-i)*table->row_size;
    ZYMemory *memory=new ZYMemory(n);
    ReadFile(fp1,memory->GetPointer(),n);
    WriteFile(fp2,memory->GetPointer(),n);
    delete memory;
    WriteRow(i);
    table->row_cnt++;
}

//ɾ�������ļ�������(����)
void ZYTableFile::DeleteRow(int i)
{
    int fp1=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
    int fp2=fp1+table->row_size;
    int n=(table->row_cnt-(i+1))*table->row_size;
    ZYMemory *memory=new ZYMemory(n);
    ReadFile(fp2,memory->GetPointer(),n);
    WriteFile(fp1,memory->GetPointer(),n);
    delete memory;
    table->row_cnt--;
}

//�������ļ��ضϵ���������
void ZYTableFile::SetEndOfFile(void)
{
    int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+table->row_cnt*table->row_size;

    ZYFile::SetEndOfFile(fp);
}
