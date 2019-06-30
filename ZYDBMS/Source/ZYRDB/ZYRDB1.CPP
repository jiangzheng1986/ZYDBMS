//---------------------------------------------------------
//逐月存储引擎(ZYRDB)-表格文件 作者:蒋政
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//设计目标：
//1、面向对象、结构良好、可扩展
//2、稳定、可靠、简单、高效

//数据库说明文件格式：(.dbd,Database Description File)
//文件项目        起始字节  起始地址    长度    默认值
//1、文件类型     第0字节   0x00000000  4字节   ='ZYDD'
//2、文件版本     第4字节   0x00000004  4字节   =2
//3、数据库名称   第8字节   0x00000008  128字节
//4、用户名       第136字节 0x00000088  32字节
//5、密码         第168字节 0x000000A8  32字节
//6、表格自动编号 第200字节 0x000000C8  4字节   =0
//7、数据库属性   第204字节 0x000000CC  4字节   
//8、索引个数     第208字节 0x000000D0  4字节
//9、索引长度     第212字节 0x000000D4  4字节
//10、保留字节    第216字节 0x000000D8  244字节 =244 DUP(0)
//11、索引列表    第460字节 0x000001CC  ??*64字节

//每一个索引声明的结构：
//1、索引表名称   第0字节   0x00000000  16字节
//2、被索引表名称 第16字节  0x00000010  16字节
//3、辅键名称     第32字节  0x00000020  16字节
//4、主键名称     第48字节  0x00000030  16字节

//数据库表格文件格式：(.dat,Database ZYTable File)
//文件项目        起始字节  起始地址    长度    默认值
//1、文件类型     第0字节   0x00000000  4字节   ='ZYDB'
//2、文件版本     第4字节   0x00000004  4字节   =2
//3、表格属性     第8字节   0x00000008  4字节
//4、列计数       第12字节  0x0000000C  4字节
//5、列长度       第16字节  0x00000010  4字节
//6、行计数       第20字节  0x00000014  4字节
//7、行长度       第24字节  0x00000018  4字节
//8、行自动编号   第28字节  0x0000001C  4字节
//9、行最大长度   第32字节  0x00000020  4字节
//10、目录名      第36字节  0x00000024  16字节
//11、表格名      第52字节  0x00000034  16字节
//12、列列表      第68字节  0x00000044  ??*32字节
//13、行列表      第??字节  0x000000??  ??*??字节

//每一个列的结构：
//1、列名称       第0字节   0x00000000  16字节
//2、列类型       第16字节  0x00000010  4字节
//3、列长度       第20字节  0x00000014  4字节
//4、列偏移       第24字节  0x00000018  4字节
//5、列特性       第28字节  0x0000001C  4字节

//表格文件中属性开始的位置的偏移
#define FILE_ATTR_OFFSET 0x00000044

//加密密钥1
#define PATTERN1 (char)0x98

//加密密钥2
#define PATTERN2 (char)0x62

//构造文件对象
ZYFile::ZYFile(char *filename)
{
    this->filename=ZYString::Copy(filename);

    OpenFile();
}

//析构文件对象
ZYFile::~ZYFile(void)
{
    ZYString::Free(filename);

    if(hFile!=NULL)
    {
        CloseFile();
    }
}

//以可读写方式打开一个文件
void ZYFile::OpenFile(void)
{
    hFile=CreateFile(filename,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,
                     NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
}

//关闭一个已打开的文件
void ZYFile::CloseFile(void)
{
    CloseHandle(hFile);
    hFile=INVALID_HANDLE_VALUE;
}

//文件成功打开
bool ZYFile::FileOpened(void)
{
    return (hFile!=INVALID_HANDLE_VALUE)&&(hFile!=NULL);
}

//刷新文件缓冲区
void ZYFile::FlushFile(void)
{
    FlushFileBuffers(hFile);
}

//对数据进行加密
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

//对数据进行解密
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

//读取文件数据
void ZYFile::ReadFile(int fp,void *p,int n)
{
    unsigned long i;

    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::ReadFile(hFile,p,n,&i,NULL);

    //UnencriptData(p,n);
}

//写入文件数据
void ZYFile::WriteFile(int fp,void *p,int n)
{
    unsigned long i;

    //EncriptData(p,n);

    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::WriteFile(hFile,p,n,&i,NULL);

    //UnencriptData(p,n);
}

//将表格文件截断到正常结束
void ZYFile::SetEndOfFile(int fp)
{
    SetFilePointer(hFile,fp,NULL,FILE_BEGIN);

    ::SetEndOfFile(hFile);
}

//文件是否存在
bool ZYFile::Exists(char *filename)
{
    return PathFileExists(filename)==1;
}

//获取文件长度
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

//构造数据库文件对象
ZYDatabaseFile::ZYDatabaseFile(ZYDatabase *database,char *filename):ZYFile(filename)
{
    this->database=database;
}

//加载数据库文件数据
void ZYDatabaseFile::LoadData(void)
{
    ReadHeader();

    ReadIndexs();
}

//保存数据库文件数据
void ZYDatabaseFile::SaveData(void)
{
    WriteHeader();

    WriteIndexs();

    FlushFile();
}

//读取数据库文件头
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

//写入数据库文件头
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

//读取数据库文件索引信息
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

//写入数据库文件索引信息
void ZYDatabaseFile::WriteIndex(int j)
{
    int fp=0x000001CC+j*database->index_size;

    WriteFile(fp+0x00000000,&((*database->indexs)[j]->index_name),TABLE_TAB_LEN);
    WriteFile(fp+0x00000010,&((*database->indexs)[j]->table_name),TABLE_TAB_LEN);
    WriteFile(fp+0x00000020,&((*database->indexs)[j]->akey),COLUMN_NAME_LEN);
    WriteFile(fp+0x00000030,&((*database->indexs)[j]->pkey),COLUMN_NAME_LEN);
}

//读取数据库文件全部索引信息
void ZYDatabaseFile::ReadIndexs(void)
{
    int j;

    for(j=0;j<database->index_cnt;j++)
    {
        ReadIndex(j);
    }
}

//写入数据库文件全部索引信息
void ZYDatabaseFile::WriteIndexs(void)
{
    int j;

    for(j=0;j<database->index_cnt;j++)
    {
        WriteIndex(j);
    }
}

//将数据库文件截断到正常结束
void ZYDatabaseFile::SetEndOfFile(void)
{
    int fp=0x000001CC+database->index_cnt*database->index_size;

    ZYFile::SetEndOfFile(fp);
}

//构造表格文件对象
ZYTableFile::ZYTableFile(ZYTable *table,char *filename):ZYFile(filename)
{
    this->table=table;
    this->table->row.i=-1;
    this->RowDataBase=0;    
}

//刷新文件缓冲区
void ZYTableFile::FlushFile(void)
{
    if((table->table_attr&TABLE_ATTR_VIEW)==0)
    {
        ZYFile::FlushFile();
    }
}

//加载表格文件数据
void ZYTableFile::LoadData(void)
{
    ReadHeader();

    ReadColumns();
}

//读取表格文件头
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

//写入表格文件头
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

//读取表格文件列信息
void ZYTableFile::ReadColumn(int j)
{
    int fp=FILE_ATTR_OFFSET+j*table->column_size;

    ReadFile(fp+0x00000000,&(table->columns[j].name),COLUMN_NAME_LEN);
    ReadFile(fp+0x00000010,&(table->columns[j].type),sizeof(int));
    ReadFile(fp+0x00000014,&(table->columns[j].length),sizeof(int));
    ReadFile(fp+0x00000018,&(table->columns[j].offset),sizeof(int));
    ReadFile(fp+0x0000001C,&(table->columns[j].attribute),sizeof(int));
}

//写入表格文件列信息
void ZYTableFile::WriteColumn(int j)
{
    int fp=FILE_ATTR_OFFSET+j*table->column_size;

    WriteFile(fp+0x00000000,&(table->columns[j].name),COLUMN_NAME_LEN);
    WriteFile(fp+0x00000010,&(table->columns[j].type),sizeof(int));
    WriteFile(fp+0x00000014,&(table->columns[j].length),sizeof(int));
    WriteFile(fp+0x00000018,&(table->columns[j].offset),sizeof(int));
    WriteFile(fp+0x0000001C,&(table->columns[j].attribute),sizeof(int));
}

//读取表格文件全部列信息
void ZYTableFile::ReadColumns(void)
{
    int j;

    for(j=0;j<table->column_cnt;j++)
    {
        ReadColumn(j);
    }
}

//写入表格文件全部列信息
void ZYTableFile::WriteColumns(void)
{
    int j;

    for(j=0;j<table->column_cnt;j++)
    {
        WriteColumn(j);
    }
}

//读取表格文件数据行(单行)
void ZYTableFile::ReadRow(int i)
{
    if(i!=table->row.i)
    {
        int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
        table->row.i=i;
        ReadFile(fp,&table->row.row_data,table->row_size);
    }
}

//写入表格文件数据行(单行)
void ZYTableFile::WriteRow(int i)
{
    int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+i*table->row_size;
    WriteFile(fp,&table->row.row_data,table->row_size);
}

//插入表格文件数据行(单行)
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

//删除表格文件数据行(单行)
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

//将表格文件截断到正常结束
void ZYTableFile::SetEndOfFile(void)
{
    int fp=FILE_ATTR_OFFSET+table->column_cnt*table->column_size+table->row_cnt*table->row_size;

    ZYFile::SetEndOfFile(fp);
}

