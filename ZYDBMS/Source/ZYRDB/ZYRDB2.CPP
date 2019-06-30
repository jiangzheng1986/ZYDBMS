//---------------------------------------------------------
//���´洢����(ZYRDB)-������� ����:����
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//�������ݿ�Ĵ�����
bool IZYDatabase_Exists(char *path)
{
    return ZYDatabase::Exists(path);
}

//�������ݿ��Ƿ��ѱ���
bool IZYDatabase_Opened(char *path)
{
    return ZYDatabase::Opened(path);
}

//�������ݿ�
IZYDatabase *IZYDatabase_Create(char *path,char *name,char *userid,char *password)
{
    return ZYDatabase::Create(path,name,userid,password);
}

//�����ݿ�
IZYDatabase *IZYDatabase_Open(char *path,char *name,char *userid,char *password)
{
    return ZYDatabase::Open(path,name,userid,password);
}

//�ͷ����ݿ�
void IZYDatabase_Close(IZYDatabase *database)
{
    delete (ZYDatabase *)database;
}

//�������ݿ�
void IZYDatabase_Drop(IZYDatabase *database)
{
    ZYDatabase::Drop((ZYDatabase *)database);
}

//�������ݿ��ļ���
IZYDatabase *IZYDatabase_Restore(IZYDatabase *database,char *dest_path,char *dest_name,
                                      char *dest_userid,char *dest_password,void (*p)(double percent))
{
    return ZYDatabase::Restore((ZYDatabase *)database,dest_path,dest_name,dest_userid,dest_password,p);
}

//�����ݿ�
ZYDatabase *ZYDatabase::Open(char *path,char *name,char *userid,char *password)
{
    //������ݿ������δ��,������ݿ�
    if(ZYDatabase::Exists(path)&&!ZYDatabase::Opened(path))
    {
        ZYDatabase *database=new ZYDatabase();
        //������ݿ����ơ��û������������ȷ
        if(database->LoadDatabase(path,name,userid,password))
        {
            //�ɹ���
            return database;
        }
        else
        {
            //��ʧ��
            delete database;
            return NULL;
        }
    }
    else
    {
        //δ��
        return NULL;
    }
}

//�������ݿ�
ZYDatabase *ZYDatabase::Create(char *path,char *name,char *userid,char *password)
{
    //������ݿⲻ����,�򴴽����ݿ�
    if(!ZYDatabase::Exists(path))
    {
        ZYDatabase *database=new ZYDatabase();
        database->NewDatabase(path,name,userid,password);
        //�����ɹ�
        return database;
    }
    else
    {
        //δ����
        return NULL;
    }
}

//�������ݿ�
void ZYDatabase::Drop(ZYDatabase *database)
{
    //�������ݿ��е����б���
    while(database->GetTableCount()!=0)
    {
        ZYTable *table=database->GetTable(0);

        database->DropTable(table);
    }
    
    //ɾ�����ݿ��ļ���
    RemoveDirectory(database->path);

    delete database;
}

//�������ݿ����
ZYDatabase::ZYDatabase(void)
{
    latest_table=0;

    tables=new vector<ZYTable *>();

    indexs=new vector<ZYIndexDecl *>();
}

//�������ݿ�Ĵ�����
bool ZYDatabase::Exists(char *path)
{
    char dbd_filename[MAX_PATH+100];

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    if(ZYFile::Exists(dbd_filename))
    {
        //��ֻ����ʽ�򿪳ɹ�,�����ݿ����
        return true;
    }
    else
    {
        //��ֻ����ʽ�򿪲��ɹ�,�����ݿⲻ����
        return false;
    }
}

//�������ݿ��Ƿ��ѱ���
bool ZYDatabase::Opened(char *path)
{
    char dbd_filename[MAX_PATH+100];

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");
    
    ZYDatabaseFile *databaseFile=new ZYDatabaseFile(NULL,dbd_filename);

    if(!databaseFile->FileOpened())
    {
        //������ʧ��,�����ݿ��ѱ���
        delete databaseFile;
        return true;
    }
    else
    {
        //�򿪳ɹ�,�����ݿ�δ����
        delete databaseFile;
        return false;
    }
}

//�������ݿ��ļ���
void ZYDatabase::NewDatabase(char *path,char *name,char *userid,char *password)
{
    char dbd_filename[MAX_PATH+100];

    this->path=ZYString::Copy(path);

    CreateDirectory(path,NULL);

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    //�½����ݿ�˵���ļ�
    databaseFile=new ZYDatabaseFile(this,dbd_filename);
    
    file_type=0x4444595A;           //�ļ���ʶ="ZYDD"
    file_version=2;                 //�ļ��汾=2
    ZYString::Copy1(this->name,name,DATABASE_NAME_LEN);
                                    //���ݿ�����
    ZYString::Copy1(this->userid,userid,USERID_LEN);
                                    //�û���
    ZYString::Copy1(this->password,password,PASSWORD_LEN);
                                    //����
    table_autoid=0;                 //�����Զ����
    database_attr=0;                //���ݿ�����
    index_cnt=0;                    //����˵����Ŀ
    index_size=sizeof(ZYIndexDecl); //����˵������

    databaseFile->WriteHeader();
    databaseFile->FlushFile();

    LoadTables();
}

//�������ݿ��ļ���
bool ZYDatabase::LoadDatabase(char *path,char *name,char *userid,char *password)
{
    char dbd_filename[MAX_PATH+100];

    this->path=ZYString::Copy(path);

    CreateDirectory(path,NULL);

    sprintf(dbd_filename,"%s\\%s",path,"0.dbd");

    //�����ݿ�˵���ļ�
    databaseFile=new ZYDatabaseFile(this,dbd_filename);

    //���ʳ�ͻ,���ݿ��Ѵ�
    if(!databaseFile->FileOpened())
    {
        return false;
    }

    //�������ݿ�˵������
    databaseFile->LoadData();

    //������ݿ������û������������ȷ
    if( ZYString::Compare(this->name,name,DATABASE_NAME_LEN)==1&&
        ZYString::Compare(this->userid,userid,USERID_LEN)==1&&
        ZYString::Compare(this->password,password,PASSWORD_LEN)==1)
    {
        //�������
        LoadTables();

        //��������˵��
        LoadIndexs();

        return true;
    }
    else
    {
        return false;
    }   
}

//�������ݿ��ļ���������������ļ�
void ZYDatabase::LoadTables(void)
{
    int a=0;

    char pattern[MAX_PATH+1];

    char filename[MAX_PATH+1];

    ZYTable *table;

    HANDLE hFindFile;

    WIN32_FIND_DATA wfd;

    //����ϵͳ����FindFirstFile��FindNextFile�����ļ����е��ļ�

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

//�������ݿ��е���������
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

//�������ݿ����
ZYDatabase::~ZYDatabase(void)
{
    vector<ZYTable *>::iterator it;

    for(it=tables->begin();it<tables->end();it++)
    {
        if(((*it)->table_attr&TABLE_ATTR_VIEW)==0)
        {
            delete (*it);
        }
        else//ɾ����ʱ��
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

//��ȡ���ݿ��еı�����Ŀ
int ZYDatabase::GetTableCount(void)
{
    return tables->size();
}

//��ȡ���ݿ��еı���
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

//��ȡ���ݿ��еı���
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

//��ȡ���ݿ��еı���
IZYTable *ZYDatabase::GetTableI(int t)
{
    return GetTable(t);
}

//��ȡ���ݿ��еı���
IZYTable *ZYDatabase::GetTableI(char *name)
{
    return GetTable(name);
}   

//�����ݿ��д�������
IZYTable *ZYDatabase::CreateTable(char *name)
{
    char filename[1000];

    ZYTable *table=GetTable(name);

    //������ݿ��в����ڸñ���
    if(table==NULL)
    {
        //ȡ������ʽ�����ļ����ļ���
        sprintf(filename,"%s\\%d.dat",path,++latest_table);

        //�����ñ���
        table=new ZYTable(filename,name);
        tables->push_back(table);
    }

    return table;
}   

//�����ݿ��д�����ͼ
IZYTable *ZYDatabase::CreateView(char *name)
{
    //��������
    IZYTable *table=CreateTable(name);

    //����к�����
    ((ZYTable *)table)->table_attr|=TABLE_ATTR_VIEW;
    table->ClearColumns();
    table->ClearRows();

    return table;
}

//�����ݿ��г�������
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

//�����ݿ��г�������
void ZYDatabase::DropTable(IZYTable *table)
{
    DropTable((ZYTable *)table);
}   

//�����ݿ��г�������
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

//�����ݿ��д�������
ZYIndex *ZYDatabase::CreateIndex(char *name,ZYTable *table,char *akey,char *pkey)
{
    ZYIndex *index;
    char filename[1000];
    char table_name[100];

    ZYTable *table1=GetTable(name);

    //������ݿ��в����ڸ�����
    if(table1==NULL)
    {
        //ȡ������ʽ�����ļ����ļ���
        sprintf(filename,"%s\\%d.dat",path,++latest_table);

        //����������
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
        //�򿪲����������
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

//�����ݿ��д�������
IZYIndex *ZYDatabase::CreateIndex(char *name,IZYTable *table,char *akey,char *pkey)
{
    return (IZYIndex *)CreateIndex(name,(ZYTable *)table,akey,pkey);
}

//����ȫ��Ҫ�����ļ����ܼƳ���
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

//�������ݿ�
void ZYDatabase::Backup(char *dest_path,void (*p)(double percent))
{
    int t;

    //�ܹ�Ҫ���Ƶ��ֽ���
    int total_size=CountFileSize();

    //��ǰ�����˵��ֽ���
    int bytes_copied=0;

    CreateDirectory(dest_path,NULL);

    for(t=0;t<GetTableCount();t++)
    {
        ZYTable *table=GetTable(t);

        table->Backup(dest_path,total_size,&bytes_copied,p);
    }
}

//�������ݿ�
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

//������ݿ⵽��Ļ
void ZYDatabase::Print(void)
{
    int t;

    for(t=0;t<GetTableCount();t++)
    {
        ZYTable *table=GetTable(t);

        table->Print();
    }
}

//����������(��)
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

//����������(�½�)
ZYTable::ZYTable(char *filename,char *table_name)
{
    int i;

    this->filename=ZYString::Copy(filename);

    //���������ļ�
    tableFile=new ZYTableFile(this,filename);

    //��ʼ�������ļ�������
    file_type=0x4244595A;           //�ļ���ʶ="ZYDB"
    file_version=2;                 //�ļ��汾=2
    table_attr=0;                   //��������=������
    column_cnt=0;                   //����=0
    column_size=sizeof(ZYColumn);   //�г���=16
    row_cnt=0;                      //����=0
    row_size=0;                     //�г���=0
    row_autoid=0;                   //���Զ����=0
    row_max=0;                      //�������Ŀ(���ϼ���)

    //����Ŀ¼���ͱ�����
    ZYString::Copy1(dir,"",TABLE_DIR_LEN);
    ZYString::Copy1(tab,table_name,TABLE_TAB_LEN);

    //��������ļ�ͷ
    tableFile->WriteHeader();
    tableFile->FlushFile();

    for(i=0;i<MAX_COLUMN;i++)
    {
        indexs[i]=NULL;
    }

    bFlush=true;
}

//�����������
ZYTable::~ZYTable(void)
{
    ZYString::Free(filename);

    delete tableFile;
}

//��ȡ��������
void ZYTable::GetName(char *name)
{
    ZYString::Copy2(name,tab,TABLE_TAB_LEN);
}

//��ȡ����Ŀ¼(���ϼ���)
void ZYTable::GetDir(char *dir)
{
    ZYString::Copy2(dir,this->dir,TABLE_DIR_LEN);
}

//��ȡ��������
int ZYTable::GetAttr(void)
{
    return table_attr;
}

//��������
void ZYTable::Drop(void)
{
    tableFile->CloseFile();
    DeleteFile(filename);
}

//�������ݿ��е�һ������
void ZYTable::Backup(char *dest_path,int total_size,int *bytes_copied,void (*p)(double percent))
{
    char filename1[100];

    char filename2[MAX_PATH+100];

    int i,j,c;

    //��ȡ�����ļ���*.dat��file_name1

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

    //���Ʊ����ļ���Ŀ���ļ���

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

//���������Ļ
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
                cout<<fixed;            //������ʽ���
                cout.precision(2);      //����2��С����
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

//�Ƿ���ͼ��
bool ZYTable::IsView(void)
{
    return (table_attr&TABLE_ATTR_VIEW)!=0; 
}

//�Ƿ�������
bool ZYTable::IsIndex(void)
{
    return (table_attr&TABLE_ATTR_INDEX)!=0;    
}

//��ȡ�����ϵ�����
IZYIndex *ZYTable::GetIndexOn(int j)
{
    return (IZYIndex *)indexs[j];
}

//��ȡ�����ϵ�����
IZYIndex *ZYTable::GetIndexOn(char *name)
{
    int j=FindColumn(name);
    
    return GetIndexOn(j);
}