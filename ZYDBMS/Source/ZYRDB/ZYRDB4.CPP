//---------------------------------------------------------
//���´洢����(ZYRDB)-�в��� ����:����
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//��ȡ������
void ZYColumn::GetName(char *name)
{
    ZYString::Copy2(name,this->name,COLUMN_NAME_LEN);
}

//��ȡ������
EColumnType ZYColumn::GetType(void)
{
    return this->type;
}

//��ȡ�г���
int ZYColumn::GetLength(void)
{
    return this->length;
}

//��ȡ������
int ZYColumn::GetAttribute(void)
{
    return this->attribute;
}

//��ȡָ�����������
int ZYTable::GetColumnCount(void)
{
    return column_cnt;
}

//��ȡָ���ı�����
ZYColumn *ZYTable::GetColumn(int j)
{
    if(j>=0&&j<column_cnt)
    {
        return &columns[j];
    }
    else
    {
        return NULL;
    }
}

//��ȡָ���ı�����(����InsertColumn)
ZYColumn *ZYTable::GetColumn1(int j)
{
    if(j>=0&&j<=column_cnt)
    {
        return &columns[j];
    }
    else
    {
        return NULL;
    }
}

//��ȡָ���ı�����
ZYColumn *ZYTable::GetColumn(char *name)
{
    int j=FindColumn(name);

    return GetColumn(j);
}

//����ָ������ָ��������,δ�ҵ�����-1
int ZYTable::FindColumn(char *name)
{
    int j;

    //���μ���ڴ��б����������,����ָ�����Ƶ�����

    for(j=0;j<column_cnt;j++)
    {
        if(ZYString::Compare((columns+j)->name,name,COLUMN_NAME_LEN)==1)
        {
            //�ҵ�ָ�����Ƶ�����
            return j;
        }
    }

    //δ�ҵ�
    return -1;
}

//�������Ƶ���ʱ����
ZYTable *ZYTable::CopyToTemp(void)
{
    char temp_file[MAX_PATH+100];

    //���������Զ�����
    ZYIndex::DisableIndexUpdate();
    
    sprintf(temp_file,"%s.temp",filename);

    tableFile->CloseFile();

    CopyFile(filename,temp_file,0);

    tableFile->OpenFile();

    ZYTable *temp=new ZYTable(temp_file);

    return temp;
}

//����ָ����������Ե�ƫ��
void ZYTable::CalcOffset(void)
{
    ZYColumn *column;
    int j;
    int offset=0;

    //�ַ�����ָ������,������8�ֽ�,�����Ͱ�4�ֽ�,���μ���ƫ��
    
    for(j=0;j<GetColumnCount();j++)
    {
        column=GetColumn(j);

        column->offset=offset;

        switch(column->type)
        {
        case EColumnType_String:
            offset+=column->length;
            break;
        case EColumnType_Double:
            offset+=8;
            break;
        case EColumnType_Integer:
            offset+=4;
            break;
        }
    }

    //�����г���

    row_size=offset;

    //���±����ļ�ͷ
    tableFile->WriteHeader();
    tableFile->WriteColumns();
    tableFile->SetEndOfFile();
    tableFile->FlushFile();
}

//����ʱ����������װ���������
void ZYTable::TransferBack(ZYTable *temp)
{
    int i,a;
    double d;
    int j1,j2;
    ZYColumn *column1,*column2;
    char name[COLUMN_NAME_LEN];
    char data[DATA_LEN];
    
    //���¼���ƫ��
    this->CalcOffset();

    if(this->row_size>0&&this->row_cnt>0)
    {
        EnterBufferedMode();
        for(i=0;i<this->row_cnt;i++)
        {
            for(j1=0;j1<this->column_cnt;j1++)
            {
                column1=GetColumn(j1);
                column1->GetName(name);
                j2=temp->FindColumn(name);
                column2=temp->GetColumn(j2);

                //����Ǹı�����͵����Ի���������
                if(j2==-1||column1->GetType()!=column2->GetType())
                {
                    switch(column1->GetType())
                    {
                    case EColumnType_String:
                        this->SetData(i,j1,"");
                        break;
                    case EColumnType_Double:
                        d=0;
                        this->SetData(i,j1,&d);
                        break;
                    case EColumnType_Integer:
                        a=0;
                        this->SetData(i,j1,&a);
                        break;
                    }
                }
                //�����ֻ�ı��˳��ȵ����Ի���ԭ�е�����
                else
                {
                    temp->GetData(i,j2,data);
                    this->SetData(i,j1,data);
                }
            }
        }
        LeaveBufferedMode();
    }

    //�������������Զ�����
    ZYIndex::EnableIndexUpdate();
}

//ɾ��������
void ZYTable::DeleteColumn(int j)
{
    char *p1,*p2;
    int i;

    ZYColumn *column=GetColumn(j);

    //�������Ч��,��ֱ�ӷ���
    if(column==NULL)
    {
        return;
    }

    //�������Ƶ���ʱ����
    ZYTable *temp=CopyToTemp();

    p1=(char *)column;
    p2=p1+sizeof(ZYColumn);

    //ǰ�Ʊ�ɾ���к��������

    for(;j+1<column_cnt;j++)
    {
        for(i=0;i<sizeof(ZYColumn);i++)
        {
            p1[i]=p2[i];
        }

        p1+=sizeof(ZYColumn);
        p2+=sizeof(ZYColumn);

        indexs[j]=indexs[j+1];
    }

    //���ԭ�������һ����

    for(i=0;i<sizeof(ZYColumn);i++)
    {
        p1[i]=0;
    }

    indexs[j]=NULL;

    //�м�������
    column_cnt--;

    //�����ر�����װ���������
    TransferBack(temp);

    //ɾ����ʱ����
    temp->Drop();
    delete temp;
}

//ɾ��������
void ZYTable::DeleteColumn(char *name)
{
    int j=FindColumn(name);

    DeleteColumn(j);
}

//���������
int ZYTable::InsertColumn1(int j1,char name[],EColumnType type,int length,int attribute)
{
    int i;
    int j;
    char *p1,*p2;

    //��������������,����-1
    if(column_cnt>=MAX_COLUMN)
    {
        return -1;
    }   

    ZYColumn *column=GetColumn1(j1);

    //�������Ч��,����-1
    if(column==NULL)
    {
        return -1;
    }

    //�������Ƶ���ʱ����
    ZYTable *temp=CopyToTemp();

    column_cnt++;

    ZYColumn *column1=GetColumn(column_cnt-1);

    p1=(char *)column1;
    p2=p1-sizeof(ZYColumn);

    //���Ʋ����к��������

    for(j=column_cnt-1;j>j1;j--)
    {
        for(i=0;i<sizeof(ZYColumn);i++)
        {
            p1[i]=p2[i];
        }

        p1-=sizeof(ZYColumn);
        p2-=sizeof(ZYColumn);

        indexs[j]=indexs[j-1];
    }

    indexs[j]=NULL;
    
    j=j1;

    ZYString::Copy1(column->name,name,COLUMN_NAME_LEN);
    
    column->type=type;

    switch(column->type)
    {
    case EColumnType_String:
        column->length=length;
        break;
    case EColumnType_Double:
        column->length=8;
        break;
    case EColumnType_Integer:
        column->length=4;
        break;
    }

    column->attribute=attribute;

    //�����ر�����װ���������
    TransferBack(temp);

    //ɾ����ʱ����
    temp->Drop();
    delete temp;

    return j;
}

//׷�ӱ�����
int ZYTable::AppendColumn1(char name[],EColumnType type,int length,int attribute)
{
    int j1=GetColumnCount();

    return InsertColumn1(j1,name,type,length,attribute);
}

//��ȡ������
void ZYTable::GetColumn(int j,char *aspect,void *data)
{
    ZYColumn *column=GetColumn(j);

    //�������Ч��,��ֱ�ӷ���
    if(column==NULL)
    {
        return;
    }

    if(strcmp(aspect,"����")==0)
    {
        column->GetName((char *)data);
    }
    else if(strcmp(aspect,"����")==0)
    {
        *((int *)data)=column->GetType();
    }
    else if(strcmp(aspect,"����")==0)
    {
        *((int *)data)=column->GetLength();
    }
    else if(strcmp(aspect,"����")==0)
    {
        *((int *)data)=column->GetAttribute();
    }
}

//��ȡ������
void ZYTable::GetColumn(char *name,char *aspect,void *data)
{
    int j=FindColumn(name);

    GetColumn(j,aspect,data);
}

//�޸ı�����
void ZYTable::SetColumn(int j,char *aspect,void *data)
{
    ZYColumn *column=GetColumn(j);

    //�������Ч����,��ֱ�ӷ���
    if(column==NULL)
    {
        return;
    }

    if(strcmp(aspect,"����")==0)
    {
        if(ZYString::Compare(column->name,(char *)data,COLUMN_NAME_LEN)==0)
        {
            ZYString::Copy1(column->name,(char *)data,COLUMN_NAME_LEN);

            //��������ļ�ͷ
            tableFile->WriteColumn(j);
            tableFile->FlushFile();
        }
    }
    else if(strcmp(aspect,"����")==0)
    {
        if( column->type!=*((int *)data)&&
            *((int *)data)>=EColumnType_String&&
            *((int *)data)<=EColumnType_Integer)
        {
            //�������Ƶ���ʱ����
            ZYTable *temp=CopyToTemp();

            column->type=*((EColumnType *)data);

            switch(column->type)
            {
            case EColumnType_String:
                column->length=16;
                break;
            case EColumnType_Double:
                column->length=8;
                break;
            case EColumnType_Integer:
                column->length=4;
                break;
            }

            //�����ر�����װ���������
            TransferBack(temp);

            //ɾ����ʱ����
            temp->Drop();
            delete temp;
        }
    }
    else if(strcmp(aspect,"����")==0)
    {
        if(column->type==EColumnType_String&&
           column->length!=*((int *)data)&&
           *((int *)data)>=0)
        {
            //�������Ƶ���ʱ����
            ZYTable *temp=CopyToTemp();

            column->length=*((int *)data);

            //�����ر�����װ���������
            TransferBack(temp);

            //ɾ����ʱ����
            temp->Drop();
            delete temp;
        }
    }
    else if(strcmp(aspect,"����")==0)
    {
        if(column->attribute!=*((int *)data))
        {
            column->attribute=*((int *)data);

            //��������ļ�ͷ
            tableFile->WriteColumn(j);
            tableFile->FlushFile();
        }
    }
}

//�޸ı�����
void ZYTable::SetColumn(char *name,char *aspect,void *data)
{
    int j=FindColumn(name);

    SetColumn(j,aspect,data);
}

//���������
int ZYTable::InsertColumn(int j1,char name[],EColumnType type,int length,int attribute)
{
    int j=FindColumn(name);

    //����в�����,�������
    if(j==-1)
    {
        return InsertColumn1(j1,name,type,length,attribute);
    }

    ZYColumn *column=GetColumn(j);

    //ȷ��������ͬ

    if(column->GetType()!=type)
    {
        SetColumn(j,"����",&type);
    }

    //ȷ��������ͬ

    if(column->GetType()==0)
    {
        if(column->GetLength()!=length)
        {
            SetColumn(j,"����",&length);
        }
    }

    return j;
}

//׷�ӱ�����
int ZYTable::AppendColumn(char name[],EColumnType type,int length,int attribute)
{
    int j1=GetColumnCount();

    return InsertColumn(j1,name,type,length,attribute);
}

//���Ʊ�����
int ZYTable::CopyColumn(ZYTable *source,char *column_name)
{
    int j=source->FindColumn(column_name);
    
    if(j!=-1)
    {
        ZYColumn *column=source->GetColumn(j);

        int attr=column->GetAttribute()&~(COLUMN_ATTR_INDEXEDP|COLUMN_ATTR_INDEXEDA);

        return AppendColumn(column_name,column->GetType(),column->GetLength(),attr);
    }
    else
    {
        return -1;
    }
}

//���Ʊ�����
int ZYTable::CopyColumn(IZYTable *source,char *column_name)
{
    return CopyColumn((ZYTable *)source,column_name);
}

//��ձ�����
void ZYTable::ClearColumns(void)
{
    this->column_cnt=0;
    tableFile->WriteHeader();
    tableFile->SetEndOfFile();
    tableFile->FlushFile();
}

//��������
void ZYTable::SetPrimaryKey(int j)
{
    int attribute;

    ZYColumn *column;

    column=GetColumn(j);

    if(column==NULL)
    {
        return;
    }

    attribute=column->GetAttribute();

    if((attribute&COLUMN_ATTR_INDEXEDP)==0)
    {
        attribute|=COLUMN_ATTR_INDEXEDP;

        SetColumn(j,"����",&attribute);
    }
}

//��������
void ZYTable::SetPrimaryKey(char *name)
{
    int j;

    j=FindColumn(name);
    SetPrimaryKey(j);
}

//��ȡ����
int ZYTable::GetPrimaryKey(void)
{
    int j;

    int attribute;

    ZYColumn *column;

    for(j=0;j<GetColumnCount();j++)
    {
        column=GetColumn(j);

        attribute=column->GetAttribute();

        if((attribute&COLUMN_ATTR_INDEXEDP)!=0)
        {
            return j;
        }
    }

    return -1;
}