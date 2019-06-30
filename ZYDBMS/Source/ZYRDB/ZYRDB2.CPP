//---------------------------------------------------------
//逐月存储引擎(ZYRDB)-表格管理 作者:蒋政
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//测试数据库的存在性
bool IZYDatabase_Exists(char *path)
{
    return ZYDatabase::Exists(path);
}

//测试数据库是否已被打开
bool IZYDatabase_Opened(char *path)
{
    return ZYDatabase::Opened(path);
}

//创建数据库
IZYDatabase *IZYDatabase_Create(char *path,char *name,char *userid,char *password)
{
    return ZYDatabase::Create(path,name,userid,password);
}

//打开数据库
IZYDatabase *IZYDatabase_Open(char *path,char *name,char *userid,char *password)
{
    return ZYDatabase::Open(path,name,userid,password);
}

//释放数据库
void IZYDatabase_Close(IZYDatabase *database)
{
    delete (ZYDatabase *)database;
}

//撤消数据库
void IZYDatabase_Drop(IZYDatabase *database)
{
    ZYDatabase::Drop((ZYDatabase *)database);
}

//备份数据库文件夹
IZYDatabase *IZYDatabase_Restore(IZYDatabase *database,char *dest_path,char *dest_name,
                                      char *dest_userid,char *dest_password,void (*p)(double percent))
{
    return ZYDatabase::Restore((ZYDatabase *)database,dest_path,dest_name,dest_userid,dest_password,p);
}

//打开数据库
ZYDatabase *ZYDatabase::Open(char *path,char *name,char *userid,char *password)
{
    //如果数据库存在且未打开,则打开数据库
    if(ZYDatabase::Exists(path)&&!ZYDatabase::Opened(path))
    {
        ZYDatabase *database=new ZYDatabase();
        //如果数据库名称、用户名、密码等正确
        if(database->LoadDatabase(path,name,userid,password))
        {
            //成功打开
            return database;
        }
        else
        {
            //打开失败
            delete database;
            return NULL;
        }
    }
    else
    {
        //未打开
        return NULL;
    }
}

//创建数据库
ZYDatabase *ZYDatabase::Create(char *path,char *name,char *userid,char *password)
{
    //如果数据库不存在,则创建数据库
    if(!ZYDatabase::Exists(path))
    {
        ZYDatabase *database=new ZYDatabase();
        database->NewDatabase(path,name,userid,password);
        //创建成功
        return database;
    }
    else
    {
        //未创建
        return NULL;
    }
}

//撤消数据库
void ZYDatabase::Drop(ZYDatabase *database)
{
    //撤消数据库中的所有表格
    while(database->GetTableCount()!=0)
    {
        ZYTable *table=database->GetTable(0);

        database->DropTable(table);
    }
    
    //删除数据库文件夹
    RemoveDirectory(database->path);

    delete database;
}

//构造数据库对象
ZYDatabase::ZYDatabase(void)
{
    latest_table=0;

    tables=new vector<ZYTable *>();

    indexs=new vector<ZYIndexDecl *>();
}

//测试数据库的存在性
bool ZYDatabase::Exists(char *path)
{
    char dbd_filename[MAX_PATH+100];

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    if(ZYFile::Exists(dbd_filename))
    {
        //以只读方式打开成功,故数据库存在
        return true;
    }
    else
    {
        //以只读方式打开不成功,故数据库不存在
        return false;
    }
}

//测试数据库是否已被打开
bool ZYDatabase::Opened(char *path)
{
    char dbd_filename[MAX_PATH+100];

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");
    
    ZYDatabaseFile *databaseFile=new ZYDatabaseFile(NULL,dbd_filename);

    if(!databaseFile->FileOpened())
    {
        //后续打开失败,故数据库已被打开
        delete databaseFile;
        return true;
    }
    else
    {
        //打开成功,故数据库未被打开
        delete databaseFile;
        return false;
    }
}

//建立数据库文件夹
void ZYDatabase::NewDatabase(char *path,char *name,char *userid,char *password)
{
    char dbd_filename[MAX_PATH+100];

    this->path=ZYString::Copy(path);

    CreateDirectory(path,NULL);

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    //新建数据库说明文件
    databaseFile=new ZYDatabaseFile(this,dbd_filename);
    
    file_type=0x4444595A;           //文件标识="ZYDD"
    file_version=2;                 //文件版本=2
    ZYString::Copy1(this->name,name,DATABASE_NAME_LEN);
                                    //数据库名称
    ZYString::Copy1(this->userid,userid,USERID_LEN);
                                    //用户名
    ZYString::Copy1(this->password,password,PASSWORD_LEN);
                                    //密码
    table_autoid=0;                 //表格自动编号
    database_attr=0;                //数据库属性
    index_cnt=0;                    //索引说明数目
    index_size=sizeof(ZYIndexDecl); //索引说明长度

    databaseFile->WriteHeader();
    databaseFile->FlushFile();

    LoadTables();
}

//加载数据库文件夹
bool ZYDatabase::LoadDatabase(char *path,char *name,char *userid,char *password)
{
    char dbd_filename[MAX_PATH+100];

    this->path=ZYString::Copy(path);

    CreateDirectory(path,NULL);

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    //打开数据库说明文件
    databaseFile=new ZYDatabaseFile(this,dbd_filename);

    //访问冲突,数据库已打开
    if(!databaseFile->FileOpened())
    {
        return false;
    }

    //载入数据库说明数据
    databaseFile->LoadData();

    //如果数据库名、用户名、密码等正确
    if( ZYString::Compare(this->name,name,DATABASE_NAME_LEN)==1&&
        ZYString::Compare(this->userid,userid,USERID_LEN)==1&&
        ZYString::Compare(this->password,password,PASSWORD_LEN)==1)
    {
        //载入表格
        LoadTables();

        //载入索引说明
        LoadIndexs();

        return true;
    }
    else
    {
        return false;
    }   
}

//加载数据库文件夹里的所有数据文件
void ZYDatabase::LoadTables(void)
{
    int a=0;

    char pattern[MAX_PATH+1];

    char filename[MAX_PATH+1];

    ZYTable *table;

    HANDLE hFindFile;

    WIN32_FIND_DATA wfd;

    //调用系统函数FindFirstFile和FindNextFile遍历文件夹中的文件

    sprintf(pattern,"%s\\*.dat",path);

    hFindFile=FindFirstFile(pattern,&wfd);

    if(hFindFile==INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if((wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
        {
            sprintf(filename,"%s\\%s",path,wfd.cFileName);

            table=new ZYTable(filename);

            tables->push_back(table);

            a=atoi(wfd.cFileName);

            if(a>latest_table)
            {
                latest_table=a;
            }
        }
    }
    while(FindNextFile(hFindFile,&wfd));

    FindClose(hFindFile);
}

//加载数据库中的所有索引
void ZYDatabase::LoadIndexs(void)
{
    int j;
    char index_name[TABLE_TAB_LEN+1];
    char table_name[TABLE_TAB_LEN+1];
    char akey[COLUMN_NAME_LEN+1];
    char pkey[COLUMN_NAME_LEN+1];

    for(j=0;j<indexs->size();j++)
    {
        ZYString::Copy2(index_name,(*indexs)[j]->index_name,TABLE_TAB_LEN);
        ZYString::Copy2(table_name,(*indexs)[j]->table_name,TABLE_TAB_LEN);
        ZYString::Copy2(akey,(*indexs)[j]->akey,COLUMN_NAME_LEN);
        ZYString::Copy2(pkey,(*indexs)[j]->pkey,COLUMN_NAME_LEN);
        
        ZYTable *table=GetTable(table_name);
        if(table!=NULL)
        {
            CreateIndex(index_name,table,akey,pkey);
        }
    }
}

//析构数据库对象
ZYDatabase::~ZYDatabase(void)
{
    vector<ZYTable *>::iterator it;

    for(it=tables->begin();it<tables->end();it++)
    {
        if(((*it)->table_attr&TABLE_ATTR_VIEW)==0)
        {
            delete (*it);
        }
        else//删除临时表
        {
            (*it)->Drop();
        }
    }

    delete tables;

    vector<ZYIndexDecl *>::iterator it1;

    for(it1=indexs->begin();it1<indexs->end();it1++)
    {
        delete (*it1);
    }

    delete indexs;

    delete databaseFile;

    ZYString::Free(path);
}

//获取数据库中的表格数目
int ZYDatabase::GetTableCount(void)
{
    return tables->size();
}

//获取数据库中的表格
ZYTable *ZYDatabase::GetTable(int t)
{
    if(t>=0&&t<tables->size())
    {
        return (*tables)[t];
    }
    else
    {
        return NULL;
    }
}

//获取数据库中的表格
ZYTable *ZYDatabase::GetTable(char *name)
{
    vector<ZYTable *>::iterator it;

    char table_name1[100];

    char table_dir1[100];

    for(it=tables->begin();it<tables->end();it++)
    {
        (*it)->GetName(table_name1);

        (*it)->GetDir(table_dir1);

        if( strcmp(table_name1,name)==0&&
            strcmp(table_dir1,"")==0)
            return (*it);
    }

    return NULL;
}   

//获取数据库中的表格
IZYTable *ZYDatabase::GetTableI(int t)
{
    return GetTable(t);
}

//获取数据库中的表格
IZYTable *ZYDatabase::GetTableI(char *name)
{
    return GetTable(name);
}   

//在数据库中创建表格
IZYTable *ZYDatabase::CreateTable(char *name)
{
    char filename[1000];

    ZYTable *table=GetTable(name);

    //如果数据库中不存在该表格
    if(table==NULL)
    {
        //取出并格式化新文件的文件名
        sprintf(filename,"%s\\%d.dat",path,++latest_table);

        //创建该表格
        table=new ZYTable(filename,name);
        tables->push_back(table);
    }

    return table;
}   

//在数据库中创建视图
IZYTable *ZYDatabase::CreateView(char *name)
{
    //创建表格
    IZYTable *table=CreateTable(name);

    //清空列和数据
    ((ZYTable *)table)->table_attr|=TABLE_ATTR_VIEW;
    table->ClearColumns();
    table->ClearRows();

    return table;
}

//在数据库中撤消表格
void ZYDatabase::DropTable(ZYTable *table)
{
    vector<ZYTable *>::iterator it;

    for(it=tables->begin();it<tables->end();it++)
    {
        if((*it)==table)
        {
            (*it)->Drop();

            delete (*it);

            tables->erase(it);

            return;
        }
    }
}

//在数据库中撤消表格
void ZYDatabase::DropTable(IZYTable *table)
{
    DropTable((ZYTable *)table);
}   

//在数据库中撤消表格
void ZYDatabase::DropTable(char *name)
{
    vector<ZYTable *>::iterator it;

    char table_name1[100];

    char table_dir1[100];

    for(it=tables->begin();it<tables->end();it++)
    {
        (*it)->GetName(table_name1);

        (*it)->GetDir(table_dir1);

        if( strcmp(table_name1,name)==0&&
            strcmp(table_dir1,"")==0)
        {
            (*it)->Drop();

            delete (*it);

            tables->erase(it);

            return;
        }
    }
}

//在数据库中创建索引
ZYIndex *ZYDatabase::CreateIndex(char *name,ZYTable *table,char *akey,char *pkey)
{
    ZYIndex *index;
    char filename[1000];
    char table_name[100];

    ZYTable *table1=GetTable(name);

    //如果数据库中不存在该索引
    if(table1==NULL)
    {
        //取出并格式化新文件的文件名
        sprintf(filename,"%s\\%d.dat",path,++latest_table);

        //创建该索引
        index=new ZYIndex(filename,name,table,akey,pkey);
        tables->push_back(index);
        
        table->GetName(table_name);

        ZYIndexDecl *indexDecl=new ZYIndexDecl(name,table_name,akey,pkey);

        indexs->push_back(indexDecl);

        index_cnt++;

        databaseFile->SaveData();
    }
    else
    {
        //打开并保存该索引
        int t;

        char *filename1=ZYString::Copy(table1->filename);
        
        for(t=0;t<GetTableCount();t++)
        {
            if((*tables)[t]==table1)
            {
                delete table1;

                break;
            }
        }

        index=new ZYIndex(filename1,table,akey,pkey);

        (*tables)[t]=index;

        ZYString::Free(filename1);
    }   

    return index;
}

//在数据库中创建索引
IZYIndex *ZYDatabase::CreateIndex(char *name,IZYTable *table,char *akey,char *pkey)
{
    return (IZYIndex *)CreateIndex(name,(ZYTable *)table,akey,pkey);
}

//计算全部要拷贝文件的总计长度
int ZYDatabase::CountFileSize(void)
{
    int t;

    int total_size=0;

    for(t=0;t<GetTableCount();t++)
    {
        ZYTable *table=GetTable(t);

        total_size+=table->tableFile->GetFileSize();
    }

    return total_size;
}

//备份数据库
void ZYDatabase::Backup(char *dest_path,void (*p)(double percent))
{
    int t;

    //总共要复制的字节数
    int total_size=CountFileSize();

    //当前复制了的字节数
    int bytes_copied=0;

    CreateDirectory(dest_path,NULL);

    for(t=0;t<GetTableCount();t++)
    {
        ZYTable *table=GetTable(t);

        table->Backup(dest_path,total_size,&bytes_copied,p);
    }
}

//备份数据库
ZYDatabase *ZYDatabase::Restore(ZYDatabase *database,char *src_path,char *src_name,
                                char *src_userid,char *src_password,void (*p)(double percent))
{
    ZYDatabase *database1;

    char *path=ZYString::Copy(database->path);

    ZYDatabase::Drop(database);

    database1=(ZYDatabase *)ZYDatabase::Open(src_path,src_name,src_userid,src_password);

    if(database1!=NULL)
    {   
        database1->Backup(path,p);

        IZYDatabase_Close(database1);

        return (ZYDatabase *)ZYDatabase::Create(src_path,src_name,src_userid,src_password);
    }
    else
    {
        return NULL;
    }
}

//输出数据库到屏幕
void ZYDatabase::Print(void)
{
    int t;

    for(t=0;t<GetTableCount();t++)
    {
        ZYTable *table=GetTable(t);

        table->Print();
    }
}

//构造表格对象(打开)
ZYTable::ZYTable(char *filename)
{
    int i;

    this->filename=ZYString::Copy(filename);
    
    tableFile=new ZYTableFile(this,filename);

    tableFile->LoadData();

    for(i=0;i<MAX_COLUMN;i++)
    {
        indexs[i]=NULL;
    }

    bFlush=true;
}

//构造表格对象(新建)
ZYTable::ZYTable(char *filename,char *table_name)
{
    int i;

    this->filename=ZYString::Copy(filename);

    //创建表格文件
    tableFile=new ZYTableFile(this,filename);

    //初始化表格文件的数据
    file_type=0x4244595A;           //文件标识="ZYDB"
    file_version=2;                 //文件版本=2
    table_attr=0;                   //表格属性=基本表
    column_cnt=0;                   //列数=0
    column_size=sizeof(ZYColumn);   //列长度=16
    row_cnt=0;                      //行数=0
    row_size=0;                     //行长度=0
    row_autoid=0;                   //行自动编号=0
    row_max=0;                      //行最大数目(向上兼容)

    //保存目录名和表格名
    ZYString::Copy1(dir,"",TABLE_DIR_LEN);
    ZYString::Copy1(tab,table_name,TABLE_TAB_LEN);

    //保存表格文件头
    tableFile->WriteHeader();
    tableFile->FlushFile();

    for(i=0;i<MAX_COLUMN;i++)
    {
        indexs[i]=NULL;
    }

    bFlush=true;
}

//析构表格对象
ZYTable::~ZYTable(void)
{
    ZYString::Free(filename);

    delete tableFile;
}

//获取表格名称
void ZYTable::GetName(char *name)
{
    ZYString::Copy2(name,tab,TABLE_TAB_LEN);
}

//获取表格目录(向上兼容)
void ZYTable::GetDir(char *dir)
{
    ZYString::Copy2(dir,this->dir,TABLE_DIR_LEN);
}

//获取表格属性
int ZYTable::GetAttr(void)
{
    return table_attr;
}

//撤消表格
void ZYTable::Drop(void)
{
    tableFile->CloseFile();
    DeleteFile(filename);
}

//备份数据库中的一个表格
void ZYTable::Backup(char *dest_path,int total_size,int *bytes_copied,void (*p)(double percent))
{
    char filename1[100];

    char filename2[MAX_PATH+100];

    int i,j,c;

    //截取表格文件名*.dat到file_name1

    c=lstrlen(filename);

    for(i=c-1,j=0;i>=0;i--)
    {
        if(filename[i]!='\\')
        {
            j++;
        }
        else
        {
            break;
        }
    }

    filename1[j--]='\0';

    for(i=c-1;j>=0;i--,j--)
    {
        filename1[j]=filename[i];
    }

    //复制表格文件到目标文件夹

    sprintf(filename2,"%s\\%s",dest_path,filename1);

    tableFile->CloseFile();

    CopyFile(filename,filename2,0);

    tableFile->OpenFile();

    (*bytes_copied)+=tableFile->GetFileSize();

    if(p!=NULL)
    {
        (*p)((double)(*bytes_copied)/(double)total_size);
    }
}

//输出表格到屏幕
void ZYTable::Print(void)
{
    char table_name[1000];

    char column_name[1000];

    char buf[1000];

    ZYColumn *column;

    int i,j,a;

    double d;

    table_name[0]='\0';

    GetName(table_name);

    cout<<"{"<<table_name<<"}"<<endl;

    cout<<"[";

    for(j=0;j<GetColumnCount();j++)
    {
        column=GetColumn(j);

        column_name[0]='\0';
        
        column->GetName(column_name);

        cout<<column_name;

        if(j!=GetColumnCount()-1)
        {
            cout<<",";
        }
    }

    cout<<"]"<<endl;

    for(i=0;i<GetRowCount();i++)
    {
        cout<<"(";

        for(j=0;j<GetColumnCount();j++)
        {
            column=GetColumn(j);

            switch(column->GetType())
            {
            case EColumnType_String:
                GetData(i,j,buf);
                cout<<buf;
                break;
            case EColumnType_Double:
                GetData(i,j,&d);
                cout<<fixed;            //定点形式输出
                cout.precision(2);      //保留2个小数点
                cout<<d;
                break;
            case EColumnType_Integer:
                GetData(i,j,&a);
                cout<<a;
                break;
            }

            if(j!=GetColumnCount()-1)
            {
                cout<<",";
            }
        }

        cout<<")"<<endl;
    }   
}

//是否视图表
bool ZYTable::IsView(void)
{
    return (table_attr&TABLE_ATTR_VIEW)!=0; 
}

//是否索引表
bool ZYTable::IsIndex(void)
{
    return (table_attr&TABLE_ATTR_INDEX)!=0;    
}

//获取辅键上的索引
IZYIndex *ZYTable::GetIndexOn(int j)
{
    return (IZYIndex *)indexs[j];
}

//获取辅键上的索引
IZYIndex *ZYTable::GetIndexOn(char *name)
{
    int j=FindColumn(name);
    
    return GetIndexOn(j);
}