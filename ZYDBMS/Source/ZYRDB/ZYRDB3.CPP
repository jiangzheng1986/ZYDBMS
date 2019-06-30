//---------------------------------------------------------
//���´洢����(ZYRDB)-�в��� ����:����
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//��ȡָ���ı���������ָ��
void *ZYTable::GetDataPointer(int i,int j)
{
    ZYColumn *column=GetColumn(j);

    if(column==NULL)
    {
        return NULL;
    }

    if(i>=0&&i<row_cnt)
    {
        return row.row_data+column->offset;
    }
    else
    {
        return NULL;
    }
}

//��ȡָ���ı���������ָ��(����InsertRow)
void *ZYTable::GetDataPointer1(int i,int j)
{
    ZYColumn *column=GetColumn(j);

    if(column==NULL)
    {
        return NULL;
    }

    if(i>=0&&i<row_cnt+1)
    {
        return row.row_data+column->offset;
    }
    else
    {
        return NULL;
    }
}

//��ȡָ����ָ��������,ʹ��������
void ZYTable::GetData(int i,int j,void *data)
{
    ZYColumn *column;

    void *p;

    column=GetColumn(j);

    p=GetDataPointer(i,j);

    if(p==NULL)
    {
        return;
    }

    tableFile->ReadRow(i);

    switch(column->type)
    {
    case EColumnType_String:
        ZYString::Copy2((char *)data,(char *)p,column->length);
        break;
    case EColumnType_Double:
        *((double *)data)=*((double *)p);
        break;
    case EColumnType_Integer:
        *((int *)data)=*((int *)p);
        break;
    }
}

//��ȡָ����ָ��������,ʹ��������
void ZYTable::GetData(int i,char *key,void *data)
{
    int j=FindColumn(key);

    GetData(i,j,data);
}

//���ֲ���ָ����������,��������������(����SetData)
int ZYTable::FindRow3(int j,void *data)
{
    int low,mid,high;

    low=0;

    high=row_cnt-1;

    while(low<=high)
    {
        mid=(low+high)/2;

        switch(CompareRow1(data,mid,j))
        {
        case 0:
            return mid;
        case -1:
            high=mid-1;
            break;
        case 1:
            low=mid+1;
            break;
        }
    }

    return mid;
}

//����ָ����ָ��������,ʹ��������
void ZYTable::SetData(int i,int j,void *data)
{
    ZYColumn *column;

    void *p;

    column=GetColumn(j);

    p=GetDataPointer(i,j);

    if(p==NULL)
    {
        return;
    }

    //����������
    ZYIndex::IndexUpdate3(this,i,j,data);

    tableFile->ReadRow(i);
    
    //�Ż�:����򲻽��к�������

    switch(column->type)
    {
    case EColumnType_String:
        if(ZYString::Compare((char *)p,(char *)data,column->length)==1)
        {
            return;
        }
        ZYString::Copy1((char *)p,(char *)data,column->length);
        break;
    case EColumnType_Double:
        if(*((double *)p)==*((double *)data))
        {
            return;
        }
        *((double *)p)=*((double *)data);
        break;
    case EColumnType_Integer:
        if(*((int *)p)==*((int *)data))
        {
            return;
        }
        *((int *)p)=*((int *)data);
        break;
    }

    tableFile->WriteRow(i);

    if(bFlush)
    {
        tableFile->FlushFile();
    }
}

//����ָ����ָ��������,ʹ��������
void ZYTable::SetData(int i,char *key,void *data)
{
    int j=FindColumn(key);

    SetData(i,j,data);
}

//��ȡָ�����������
int ZYTable::GetRowCount(void)
{
    return row_cnt;
}

//׷�ӱ���������
int ZYTable::AppendRow(void)
{
    int i,j;

    void *p,*p1;

    ZYColumn *column;
    
    i=row_cnt;

    row.i=row_cnt;

    row_cnt++;

    p1=GetDataPointer(i,0);

    //�����г�ʼ��
    for(j=0;j<column_cnt;j++)
    {
        column=GetColumn(j);

        p=(char *)p1+column->offset;

        switch(column->type)
        {
        case EColumnType_String:
            ZYString::Copy1((char *)p,"",column->length);
            break;
        case EColumnType_Double:
            *((double *)p)=0.0f;
            break;
        case EColumnType_Integer:
            //ע��:����Ϊ���Ż���������,����������ų�ʼ��ΪMAX_ROW
            if((column->attribute&COLUMN_ATTR_INDEXEDA)==0)
            {
                *((int *)p)=0;
            }
            else
            {
                *((int *)p)=INT_MAX;
            }
            break;
        }
    }

    tableFile->WriteHeader();

    tableFile->WriteRow(i);

    if(bFlush)
    {
        tableFile->FlushFile();
    }

    //������������
    ZYIndex::IndexUpdate1(this,i);
    
    return i;
}

//�������������
int ZYTable::InsertRow(int i)
{
    int j;

    void *p,*p1;

    ZYColumn *column;
    
    row.i=i;

    p1=GetDataPointer1(i,0);

    //�����г�ʼ��
    for(j=0;j<column_cnt;j++)
    {
        column=GetColumn(j);

        p=(char *)p1+column->offset;

        switch(column->type)
        {
        case EColumnType_String:
            ZYString::Copy1((char *)p,"",column->length);
            break;
        case EColumnType_Double:
            *((double *)p)=0.0f;
            break;
        case EColumnType_Integer:
            //ע��:����Ϊ���Ż���������,����������ų�ʼ��ΪMAX_ROW
            if((column->attribute&COLUMN_ATTR_INDEXEDA)==0)
            {
                *((int *)p)=0;
            }
            else
            {
                *((int *)p)=INT_MAX;
            }
            break;
        }
    }

    tableFile->InsertRow(i);

    tableFile->WriteHeader();

    tableFile->SetEndOfFile();

    if(bFlush)
    {
        tableFile->FlushFile();
    }

    //������������
    ZYIndex::IndexUpdate1(this,i);

    return i;
}

//ɾ������������
void ZYTable::DeleteRow(int i)
{
    if(i<0)
    {
        return;
    }

    //ɾ��������
    ZYIndex::IndexUpdate2(this,i);

    tableFile->DeleteRow(i);

    tableFile->WriteHeader();

    tableFile->SetEndOfFile();

    if(bFlush)
    {
        tableFile->FlushFile(); 
    }
}

//�Ƚϱ���������ָ��ֵ
int ZYTable::CompareRow1(void *data,int i,int j)
{
    int a1;

    int a2;

    double d1;

    double d2;

    char s1[1000];

    char s2[1000];

    switch(GetColumn(j)->type)
    {
    case 0:
        strcpy(s1,(char *)data);
        GetData(i,j,s2);
        if(s1[0]=='\0'&&
            s2[0]=='\0')    
            return 0;       //s1==s2
        if(s1[0]=='\0')
            return 1;       //s1>s2
        if(s2[0]=='\0')
            return -1;      //s1<s2
        return lstrcmp(s1,s2);
    case 1:
        d1=*((double *)data);
        GetData(i,j,&d2);
        if(d1>d2)
        {
            return 1;
        }
        else if(d1<d2)
        {
            return -1;
        }
        else if(d1==d2)
        {
            return 0;
        }
    case 2:
        a1=*((int *)data);
        GetData(i,j,&a2);
        if(a1>a2)
        {
            return 1;
        }
        else if(a1<a2)
        {
            return -1;
        }
        else if(a1==a2)
        {
            return 0;
        }
    }
    return 0;
}

//˳�����ָ����������,��������������
int ZYTable::FindRow1(int j,void *data)
{
    int i;

    for(i=row_cnt-1;i>=0;i--)
    {
        if(CompareRow1(data,i,j)==0)
        {
            return i;
        }
    }

    return -1;
}

//���ֲ���ָ����������,��������������
int ZYTable::FindRow2(int j,void *data)
{
    int low,mid,high;

    low=0;

    high=row_cnt-1;

    while(low<=high)
    {
        mid=(low+high)/2;

        switch(CompareRow1(data,mid,j))
        {
        case 0:
            return mid;
        case -1:
            high=mid-1;
            break;
        case 1:
            low=mid+1;
            break;
        }
    }

    return -1;
}

//����ָ����������,��������������
int ZYTable::FindRow(int j,void *data)
{
    ZYColumn *column=GetColumn(j);

    if(column==NULL)
    {
        return -1;
    }

    if((column->GetAttribute()&COLUMN_ATTR_INDEXEDP)==0)
    {
        return FindRow1(j,data);
    }
    else
    {
        return FindRow2(j,data);
    }
}



//��ձ���������
void ZYTable::ClearRows(void)
{
    //�������
    ZYIndex::IndexUpdate4(this);

    row_cnt=0;

    row_autoid=0;

    tableFile->WriteHeader();

    tableFile->SetEndOfFile();

    tableFile->FlushFile();
}

//��ȡ�����ɵ��Զ����
int ZYTable::GetAutoid(void)
{
    return row_autoid;
}

//������һ���Զ����
int ZYTable::NewAutoid(void)
{
    row_autoid++;

    tableFile->WriteHeader();

    tableFile->FlushFile();

    return row_autoid-1;
}

//���뻺�巽ʽ
void ZYTable::EnterBufferedMode(void)
{
    int i;
    ZYIndex *index;

    bFlush=false;

    if(!IsIndex())
    {
        for(i=0;i<MAX_COLUMN;i++)
        {
            index=indexs[i];

            if(index!=NULL)
            {
                index->EnterBufferedMode();
            }
        }
    }
}

//�뿪���巽ʽ
void ZYTable::LeaveBufferedMode(void)
{
    int i;
    ZYIndex *index;

    bFlush=true;

    tableFile->FlushFile();

    if(!IsIndex())
    {
        for(i=0;i<MAX_COLUMN;i++)
        {
            index=indexs[i];

            if(index!=NULL)
            {
                index->LeaveBufferedMode();
            }
        }
    }
}

//���������е�����
void ZYTable::SwapRow(int i1,int i2)
{
    int i;

    char buf[ROW_LEN];

    tableFile->ReadRow(i1);

    for(i=0;i<row_size;i++)
    {
        buf[i]=row.row_data[i];
    }

    tableFile->ReadRow(i2);

    tableFile->WriteRow(i1);

    for(i=0;i<row_size;i++)
    {
        row.row_data[i]=buf[i];
    }

    tableFile->WriteRow(i2);
}

//�Ƚϱ��������
int ZYTable::CompareRow(int j,int i1,int i2)
{
    char buf[DATA_LEN];

    GetData(i1,j,buf);

    return CompareRow1(buf,j,i2);
}

//��ָ���жԱ������ѡ������
void ZYTable::SelectSort(int j1,ESortOrder sortOrder)
{
    int i;

    int j;

    int k;

    int m;

    m=GetRowCount();

    if(sortOrder==ESortOrder_Asc)
    {
        for(i=0;i<m;i++)
        {
            k=i;

            for(j=i;j<m;j++)
            {
                if(CompareRow(j1,k,j)>0)
                {
                    k=j;
                }
            }

            if(i!=k)
            {
                SwapRow(i,k);
            }
        }
    }
    else //ESortOrder_Desc
    {
        for(i=0;i<m;i++)
        {
            k=i;

            for(j=i;j<m;j++)
            {
                if(CompareRow(j1,k,j)<0)
                {
                    k=j;
                }
            }

            if(i!=k)
            {
                SwapRow(i,k);
            }
        }
    }
}

//��ָ���жԱ������ѡ������
void ZYTable::SelectSort(char *key,ESortOrder sortOrder)
{
    int j1=FindColumn(key);

    SelectSort(j1,sortOrder);
}
