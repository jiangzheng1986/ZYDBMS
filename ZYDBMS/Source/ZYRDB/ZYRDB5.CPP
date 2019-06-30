//---------------------------------------------------------
//逐月存储引擎(ZYRDB)-索引操作 作者:蒋政
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//构造索引声明对象
ZYIndexDecl::ZYIndexDecl(void)
{
}

//构造索引声明对象
ZYIndexDecl::ZYIndexDecl(char *index_name,char *table_name,char *akey,char *pkey)
{
    ZYString::Copy1(this->index_name,index_name,TABLE_TAB_LEN);
    ZYString::Copy1(this->table_name,table_name,TABLE_TAB_LEN);
    ZYString::Copy1(this->akey,akey,COLUMN_NAME_LEN);
    ZYString::Copy1(this->pkey,pkey,COLUMN_NAME_LEN);
}

//构造索引对象(打开)
ZYIndex::ZYIndex(char *filename,ZYTable *table,char *akey,char *pkey):ZYTable(filename)
{
    //在源表被索引的键上设置到索引表的句柄

    int j=table->FindColumn(akey);

    table->indexs[j]=this;
}

//构造索引对象(创建)
ZYIndex::ZYIndex(char *filename,char *name,ZYTable *table,char *akey,char *pkey):ZYTable(filename,name)
{
    int j,attribute;

    ZYColumn *column;

    //对索引表进行标识

    if((table_attr&TABLE_ATTR_INDEX)==0)
    {
        table_attr|=TABLE_ATTR_INDEX;

        tableFile->WriteHeader();

        tableFile->FlushFile();
    }

    //复制索引键

    CopyColumn(table,akey);

    CopyColumn(table,pkey);

    //对源表被索引的主键进行标识

    j=table->FindColumn(pkey);

    column=table->GetColumn(j);

    attribute=column->GetAttribute();

    if((attribute&COLUMN_ATTR_INDEXEDP)==0)
    {
        attribute|=COLUMN_ATTR_INDEXEDP;

        table->SetColumn(j,"属性",&attribute);
    }

    //对源表被索引的辅键进行标识

    j=table->FindColumn(akey);

    column=table->GetColumn(j);

    attribute=column->GetAttribute();

    if((attribute&COLUMN_ATTR_INDEXEDA)==0)
    {
        attribute|=COLUMN_ATTR_INDEXEDA;

        table->SetColumn(j,"属性",&attribute);
    }

    //在源表被索引的键上设置到索引表的句柄

    j=table->FindColumn(akey);

    table->indexs[j]=this;

    //////////////////////////////////////////////////
    //建立初始的索引项
}

//转换为表格对象
IZYTable *ZYIndex::ToTable(void)
{
    return (IZYTable *)this;
}

//查找索引项
int ZYIndex::FindIndex(void *adata)
{
    int low,mid,high;

    low=0;

    mid=0;

    high=GetRowCount()-1;

    while(low<=high)
    {
        mid=(low+high)/2;

        switch(CompareRow1(adata,mid,0))
        {
        case 0:
            //已找到要定位的元素,定位回该元素开始,若大于0且等于则继续找,直到不等于或者等于0
            while(mid-1>=0&&
                  CompareRow1(adata,mid-1,0)==0)
            {
                mid--;
            }
            return mid;
        case -1:
            high=mid-1;
            //如果未找到要定位的元素,插入位置在当前位置上
            if(low>high)
            {
                return mid;
            }
            break;
        case 1:
            low=mid+1;
            //如果未找到要定位的元素,插入位置在当前位置的后面
            if(low>high)
            {
                
                return mid+1;
            }
            break;
        }
    }

    return mid;
}

//查找索引项
int ZYIndex::FindIndex1(void *adata,void *pdata)
{
    int i;

    i=FindIndex(adata);

    if(i==-1)
    {
        return -1;
    }

    for(;i<GetRowCount();i++)
    {
        if(CompareRow1(adata,i,0)!=0)
        {
            return i;
        }

        if(CompareRow1(pdata,i,1)<=0)
        {
            return i;
        }

    }

    return i;
}

//插入索引项
int ZYIndex::InsertIndex(void *adata,void *pdata)
{
    int i;

    i=FindIndex1(adata,pdata);

    i=InsertRow(i);

    SetData(i,0,adata);

    SetData(i,1,pdata);

    return i;
}

//是否辅键值的索引项
bool ZYIndex::IsIndexOf(int i,void *adata)
{
    if(i<0||i>=GetRowCount())
    {
        return false;
    }

    return (CompareRow1(adata,i,0)==0);
}

//获取索引项
void ZYIndex::GetIndex(int i,void *pdata)
{
    GetData(i,1,pdata);
}

//删除索引项
void ZYIndex::DeleteIndex(void *adata,void *pdata)
{
    int i=FindIndex1(adata,pdata);

    if( CompareRow1(adata,i,0)==0&&
        CompareRow1(pdata,i,1)==0 )
    {
        DeleteRow(i);
    }
}

//修改索引项(AKEY变化)
void ZYIndex::SetIndex(void *adata1,void *adata2,void *pdata)
{
    DeleteIndex(adata1,pdata);

    InsertIndex(adata2,pdata);
}

//修改索引项(PKEY变化)
void ZYIndex::SetIndex1(void *adata,void *pdata1,void *pdata2)
{
    DeleteIndex(adata,pdata1);

    InsertIndex(adata,pdata2);
}

//索引是否自动更新
bool ZYIndex::index_update=1;

//索引是否自动更新
bool ZYIndex::IfIndexUpdate(void)
{
    return index_update;
}

//启用索引自动更新
void ZYIndex::EnableIndexUpdate(void)
{
    index_update=1;
}

//禁用索引自动更新
void ZYIndex::DisableIndexUpdate(void)
{
    index_update=0;
}

//更新索引,用于AppendRow
void ZYIndex::IndexUpdate1(ZYTable *table,int i)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j;

    //如果不允许索引自动更新,则直接返回
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //如果是索引表,则不执行操作
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //索引更新:插入akey,pkey的索引项
    for(j=0;j<table->GetColumnCount();j++)
    {
        column=table->GetColumn(j);

        if((column->GetAttribute()&COLUMN_ATTR_INDEXEDA)!=0)
        {
            index=table->indexs[j];

            column1=index->GetColumn(0);

            column1->GetName(name);
    
            table->GetData(i,name,adata);

            column1=index->GetColumn(1);

            column1->GetName(name);

            table->GetData(i,name,pdata);

            index->InsertIndex(adata,pdata);
        }
    }
}

//更新索引,用于DeleteRow
void ZYIndex::IndexUpdate2(ZYTable *table,int i)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j;

    //如果不允许索引自动更新,则直接返回
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //如果是索引表,则不执行操作
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //索引更新:插入akey,pkey的索引项
    for(j=0;j<table->GetColumnCount();j++)
    {
        column=table->GetColumn(j);

        if((column->GetAttribute()&COLUMN_ATTR_INDEXEDA)!=0)
        {
            index=table->indexs[j];

            column1=index->GetColumn(0);

            column1->GetName(name);
    
            table->GetData(i,name,adata);

            column1=index->GetColumn(1);

            column1->GetName(name);

            table->GetData(i,name,pdata);

            index->DeleteIndex(adata,pdata);
        }
    }
}

//更新索引,用于SetData
void ZYIndex::IndexUpdate3(ZYTable *table,int i,int j,void *data)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j2,j3;

    //如果不允许索引自动更新,则直接返回
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //如果是索引表,则不执行操作
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    column=table->GetColumn(j);
    
    //A索引更新:将akey修改成新的akey值
    if((column->GetAttribute()&COLUMN_ATTR_INDEXEDA)!=0)
    {
        index=table->indexs[j];

        column1=index->GetColumn(0);

        column1->GetName(name);

        table->GetData(i,name,adata);

        column1=index->GetColumn(1);

        column1->GetName(name);

        table->GetData(i,name,pdata);

        index->SetIndex(adata,data,pdata);
    }

    //P索引更新:将pkey修改成新的pkey值
    if((column->GetAttribute()&COLUMN_ATTR_INDEXEDP)!=0)
    {
        for(j2=0;j2<table->GetColumnCount();j2++)
        {
            column=table->GetColumn(j2);

            if((column->GetAttribute()&COLUMN_ATTR_INDEXEDA)!=0)
            {
                index=table->indexs[j2];

                column1=index->GetColumn(1);

                column1->GetName(name);

                j3=table->FindColumn(name);

                if(j==j3)
                {
                    table->GetData(i,j3,pdata);

                    column1=index->GetColumn(0);

                    column1->GetName(name);

                    j3=table->FindColumn(name);

                    table->GetData(i,j3,adata);

                    index->SetIndex1(adata,pdata,data);
                }
            }
        }
    }
}

//更新索引,用于ClearRows
void ZYIndex::IndexUpdate4(ZYTable *table)
{
    ZYColumn *column;

    ZYIndex *index;

    int j;

    //如果不允许索引自动更新,则直接返回
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //如果是索引表,则不执行操作
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //索引更新:清空索引
    for(j=0;j<table->GetColumnCount();j++)
    {
        column=table->GetColumn(j);

        if((column->GetAttribute()&COLUMN_ATTR_INDEXEDA)!=0)
        {
            index=table->indexs[j];

            index->ClearRows();
        }
    }
}
