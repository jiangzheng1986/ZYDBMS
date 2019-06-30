//---------------------------------------------------------
//逐月存储引擎(ZYRDB)-行操作 作者:蒋政
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//获取指定的表格数据项指针
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

//获取指定的表格数据项指针(用于InsertRow)
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

//获取指定表指定数据项,使用列索引
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

//获取指定表指定数据项,使用列名称
void ZYTable::GetData(int i,char *key,void *data)
{
    int j=FindColumn(key);

    GetData(i,j,data);
}

//二分查找指定的数据行,返回数据项索引(用于SetData)
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

//设置指定表指定数据项,使用列索引
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

    //设置索引项
    ZYIndex::IndexUpdate3(this,i,j,data);

    tableFile->ReadRow(i);
    
    //优化:相等则不进行后续操作

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

//设置指定表指定数据项,使用列名称
void ZYTable::SetData(int i,char *key,void *data)
{
    int j=FindColumn(key);

    SetData(i,j,data);
}

//获取指定表格的行数
int ZYTable::GetRowCount(void)
{
    return row_cnt;
}

//追加表格数据行
int ZYTable::AppendRow(void)
{
    int i,j;

    void *p,*p1;

    ZYColumn *column;
    
    i=row_cnt;

    row.i=row_cnt;

    row_cnt++;

    p1=GetDataPointer(i,0);

    //数据行初始化
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
            //注意:这里为了优化磁盘性能,将被索引编号初始化为MAX_ROW
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

    //插入新索引项
    ZYIndex::IndexUpdate1(this,i);
    
    return i;
}

//插入表格数据行
int ZYTable::InsertRow(int i)
{
    int j;

    void *p,*p1;

    ZYColumn *column;
    
    row.i=i;

    p1=GetDataPointer1(i,0);

    //数据行初始化
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
            //注意:这里为了优化磁盘性能,将被索引编号初始化为MAX_ROW
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

    //插入新索引项
    ZYIndex::IndexUpdate1(this,i);

    return i;
}

//删除表格数据行
void ZYTable::DeleteRow(int i)
{
    if(i<0)
    {
        return;
    }

    //删除索引项
    ZYIndex::IndexUpdate2(this,i);

    tableFile->DeleteRow(i);

    tableFile->WriteHeader();

    tableFile->SetEndOfFile();

    if(bFlush)
    {
        tableFile->FlushFile(); 
    }
}

//比较表中属性与指定值
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

//顺序查找指定的数据行,返回数据项索引
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

//二分查找指定的数据行,返回数据项索引
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

//查找指定的数据行,返回数据项索引
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



//清空表格数据行
void ZYTable::ClearRows(void)
{
    //清空索引
    ZYIndex::IndexUpdate4(this);

    row_cnt=0;

    row_autoid=0;

    tableFile->WriteHeader();

    tableFile->SetEndOfFile();

    tableFile->FlushFile();
}

//获取将生成的自动编号
int ZYTable::GetAutoid(void)
{
    return row_autoid;
}

//新生成一个自动编号
int ZYTable::NewAutoid(void)
{
    row_autoid++;

    tableFile->WriteHeader();

    tableFile->FlushFile();

    return row_autoid-1;
}

//进入缓冲方式
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

//离开缓冲方式
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

//交换表格中的两行
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

//比较表格的两行
int ZYTable::CompareRow(int j,int i1,int i2)
{
    char buf[DATA_LEN];

    GetData(i1,j,buf);

    return CompareRow1(buf,j,i2);
}

//按指定列对表格进行选择排序
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

//按指定列对表格进行选择排序
void ZYTable::SelectSort(char *key,ESortOrder sortOrder)
{
    int j1=FindColumn(key);

    SelectSort(j1,sortOrder);
}

