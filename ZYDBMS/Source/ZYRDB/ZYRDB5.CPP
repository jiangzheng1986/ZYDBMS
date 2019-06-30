//---------------------------------------------------------
//���´洢����(ZYRDB)-�������� ����:����
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//����������������
ZYIndexDecl::ZYIndexDecl(void)
{
}

//����������������
ZYIndexDecl::ZYIndexDecl(char *index_name,char *table_name,char *akey,char *pkey)
{
    ZYString::Copy1(this->index_name,index_name,TABLE_TAB_LEN);
    ZYString::Copy1(this->table_name,table_name,TABLE_TAB_LEN);
    ZYString::Copy1(this->akey,akey,COLUMN_NAME_LEN);
    ZYString::Copy1(this->pkey,pkey,COLUMN_NAME_LEN);
}

//������������(��)
ZYIndex::ZYIndex(char *filename,ZYTable *table,char *akey,char *pkey):ZYTable(filename)
{
    //��Դ���������ļ������õ��������ľ��

    int j=table->FindColumn(akey);

    table->indexs[j]=this;
}

//������������(����)
ZYIndex::ZYIndex(char *filename,char *name,ZYTable *table,char *akey,char *pkey):ZYTable(filename,name)
{
    int j,attribute;

    ZYColumn *column;

    //�����������б�ʶ

    if((table_attr&TABLE_ATTR_INDEX)==0)
    {
        table_attr|=TABLE_ATTR_INDEX;

        tableFile->WriteHeader();

        tableFile->FlushFile();
    }

    //����������

    CopyColumn(table,akey);

    CopyColumn(table,pkey);

    //��Դ�����������������б�ʶ

    j=table->FindColumn(pkey);

    column=table->GetColumn(j);

    attribute=column->GetAttribute();

    if((attribute&COLUMN_ATTR_INDEXEDP)==0)
    {
        attribute|=COLUMN_ATTR_INDEXEDP;

        table->SetColumn(j,"����",&attribute);
    }

    //��Դ���������ĸ������б�ʶ

    j=table->FindColumn(akey);

    column=table->GetColumn(j);

    attribute=column->GetAttribute();

    if((attribute&COLUMN_ATTR_INDEXEDA)==0)
    {
        attribute|=COLUMN_ATTR_INDEXEDA;

        table->SetColumn(j,"����",&attribute);
    }

    //��Դ���������ļ������õ��������ľ��

    j=table->FindColumn(akey);

    table->indexs[j]=this;

    //////////////////////////////////////////////////
    //������ʼ��������
}

//ת��Ϊ�������
IZYTable *ZYIndex::ToTable(void)
{
    return (IZYTable *)this;
}

//����������
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
            //���ҵ�Ҫ��λ��Ԫ��,��λ�ظ�Ԫ�ؿ�ʼ,������0�ҵ����������,ֱ�������ڻ��ߵ���0
            while(mid-1>=0&&
                  CompareRow1(adata,mid-1,0)==0)
            {
                mid--;
            }
            return mid;
        case -1:
            high=mid-1;
            //���δ�ҵ�Ҫ��λ��Ԫ��,����λ���ڵ�ǰλ����
            if(low>high)
            {
                return mid;
            }
            break;
        case 1:
            low=mid+1;
            //���δ�ҵ�Ҫ��λ��Ԫ��,����λ���ڵ�ǰλ�õĺ���
            if(low>high)
            {
                
                return mid+1;
            }
            break;
        }
    }

    return mid;
}

//����������
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

//����������
int ZYIndex::InsertIndex(void *adata,void *pdata)
{
    int i;

    i=FindIndex1(adata,pdata);

    i=InsertRow(i);

    SetData(i,0,adata);

    SetData(i,1,pdata);

    return i;
}

//�Ƿ񸨼�ֵ��������
bool ZYIndex::IsIndexOf(int i,void *adata)
{
    if(i<0||i>=GetRowCount())
    {
        return false;
    }

    return (CompareRow1(adata,i,0)==0);
}

//��ȡ������
void ZYIndex::GetIndex(int i,void *pdata)
{
    GetData(i,1,pdata);
}

//ɾ��������
void ZYIndex::DeleteIndex(void *adata,void *pdata)
{
    int i=FindIndex1(adata,pdata);

    if( CompareRow1(adata,i,0)==0&&
        CompareRow1(pdata,i,1)==0 )
    {
        DeleteRow(i);
    }
}

//�޸�������(AKEY�仯)
void ZYIndex::SetIndex(void *adata1,void *adata2,void *pdata)
{
    DeleteIndex(adata1,pdata);

    InsertIndex(adata2,pdata);
}

//�޸�������(PKEY�仯)
void ZYIndex::SetIndex1(void *adata,void *pdata1,void *pdata2)
{
    DeleteIndex(adata,pdata1);

    InsertIndex(adata,pdata2);
}

//�����Ƿ��Զ�����
bool ZYIndex::index_update=1;

//�����Ƿ��Զ�����
bool ZYIndex::IfIndexUpdate(void)
{
    return index_update;
}

//���������Զ�����
void ZYIndex::EnableIndexUpdate(void)
{
    index_update=1;
}

//���������Զ�����
void ZYIndex::DisableIndexUpdate(void)
{
    index_update=0;
}

//��������,����AppendRow
void ZYIndex::IndexUpdate1(ZYTable *table,int i)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j;

    //��������������Զ�����,��ֱ�ӷ���
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //�����������,��ִ�в���
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //��������:����akey,pkey��������
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

//��������,����DeleteRow
void ZYIndex::IndexUpdate2(ZYTable *table,int i)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j;

    //��������������Զ�����,��ֱ�ӷ���
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //�����������,��ִ�в���
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //��������:����akey,pkey��������
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

//��������,����SetData
void ZYIndex::IndexUpdate3(ZYTable *table,int i,int j,void *data)
{
    ZYColumn *column,*column1;

    ZYIndex *index;

    char name[COLUMN_NAME_LEN+1];

    char adata[DATA_LEN];

    char pdata[DATA_LEN];

    int j2,j3;

    //��������������Զ�����,��ֱ�ӷ���
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //�����������,��ִ�в���
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    column=table->GetColumn(j);
    
    //A��������:��akey�޸ĳ��µ�akeyֵ
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

    //P��������:��pkey�޸ĳ��µ�pkeyֵ
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

//��������,����ClearRows
void ZYIndex::IndexUpdate4(ZYTable *table)
{
    ZYColumn *column;

    ZYIndex *index;

    int j;

    //��������������Զ�����,��ֱ�ӷ���
    if(IfIndexUpdate()==false)
    {
        return;
    }

    //�����������,��ִ�в���
    if((table->GetAttr()&TABLE_ATTR_INDEX)!=0)
    {
        return;
    }

    //��������:�������
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