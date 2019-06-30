//---------------------------------------------------------
//逐月存储引擎(ZYRDB)-列操作 作者:蒋政
//---------------------------------------------------------
#include "ZYRDB1.HPP"

//获取列名称
void ZYColumn::GetName(char *name)
{
    ZYString::Copy2(name,this->name,COLUMN_NAME_LEN);
}

//获取列类型
EColumnType ZYColumn::GetType(void)
{
    return this->type;
}

//获取列长度
int ZYColumn::GetLength(void)
{
    return this->length;
}

//获取列属性
int ZYColumn::GetAttribute(void)
{
    return this->attribute;
}

//获取指定表格的列数
int ZYTable::GetColumnCount(void)
{
    return column_cnt;
}

//获取指定的表格列
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

//获取指定的表格列(用于InsertColumn)
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

//获取指定的表格列
ZYColumn *ZYTable::GetColumn(char *name)
{
    int j=FindColumn(name);

    return GetColumn(j);
}

//查找指定表的指定列索引,未找到返回-1
int ZYTable::FindColumn(char *name)
{
    int j;

    //依次检查内存中保存的属性项,查找指定名称的属性

    for(j=0;j<column_cnt;j++)
    {
        if(ZYString::Compare((columns+j)->name,name,COLUMN_NAME_LEN)==1)
        {
            //找到指定名称的属性
            return j;
        }
    }

    //未找到
    return -1;
}

//将表格复制到临时表格
ZYTable *ZYTable::CopyToTemp(void)
{
    char temp_file[MAX_PATH+100];

    //禁用索引自动更新
    ZYIndex::DisableIndexUpdate();
    
    sprintf(temp_file,"%s.temp",filename);

    tableFile->CloseFile();

    CopyFile(filename,temp_file,0);

    tableFile->OpenFile();

    ZYTable *temp=new ZYTable(temp_file);

    return temp;
}

//计算指定表格各属性的偏移
void ZYTable::CalcOffset(void)
{
    ZYColumn *column;
    int j;
    int offset=0;

    //字符串按指定长度,浮点型8字节,按整型按4字节,依次计算偏移
    
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

    //更新行长度

    row_size=offset;

    //更新表格文件头
    tableFile->WriteHeader();
    tableFile->WriteColumns();
    tableFile->SetEndOfFile();
    tableFile->FlushFile();
}

//从临时表格中重新装入表格数据
void ZYTable::TransferBack(ZYTable *temp)
{
    int i,a;
    double d;
    int j1,j2;
    ZYColumn *column1,*column2;
    char name[COLUMN_NAME_LEN];
    char data[DATA_LEN];
    
    //重新计算偏移
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

                //如果是改变过类型的属性或者新属性
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
                //如果是只改变了长度的属性或者原有的属性
                else
                {
                    temp->GetData(i,j2,data);
                    this->SetData(i,j1,data);
                }
            }
        }
        LeaveBufferedMode();
    }

    //重新启用索引自动更新
    ZYIndex::EnableIndexUpdate();
}

//删除表格列
void ZYTable::DeleteColumn(int j)
{
    char *p1,*p2;
    int i;

    ZYColumn *column=GetColumn(j);

    //如果是无效列,则直接返回
    if(column==NULL)
    {
        return;
    }

    //将表格复制到临时表格
    ZYTable *temp=CopyToTemp();

    p1=(char *)column;
    p2=p1+sizeof(ZYColumn);

    //前移被删除列后的所有列

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

    //清除原来的最后一个列

    for(i=0;i<sizeof(ZYColumn);i++)
    {
        p1[i]=0;
    }

    indexs[j]=NULL;

    //列计数减少
    column_cnt--;

    //从隐藏表重新装入表格数据
    TransferBack(temp);

    //删除临时表格
    temp->Drop();
    delete temp;
}

//删除表格列
void ZYTable::DeleteColumn(char *name)
{
    int j=FindColumn(name);

    DeleteColumn(j);
}

//插入表格列
int ZYTable::InsertColumn1(int j1,char name[],EColumnType type,int length,int attribute)
{
    int i;
    int j;
    char *p1,*p2;

    //如果表格的列已满,返回-1
    if(column_cnt>=MAX_COLUMN)
    {
        return -1;
    }   

    ZYColumn *column=GetColumn1(j1);

    //如果是无效列,返回-1
    if(column==NULL)
    {
        return -1;
    }

    //将表格复制到临时表格
    ZYTable *temp=CopyToTemp();

    column_cnt++;

    ZYColumn *column1=GetColumn(column_cnt-1);

    p1=(char *)column1;
    p2=p1-sizeof(ZYColumn);

    //后移插入列后的所有列

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

    //从隐藏表重新装入表格数据
    TransferBack(temp);

    //删除临时表格
    temp->Drop();
    delete temp;

    return j;
}

//追加表格列
int ZYTable::AppendColumn1(char name[],EColumnType type,int length,int attribute)
{
    int j1=GetColumnCount();

    return InsertColumn1(j1,name,type,length,attribute);
}

//获取表格列
void ZYTable::GetColumn(int j,char *aspect,void *data)
{
    ZYColumn *column=GetColumn(j);

    //如果是无效列,则直接返回
    if(column==NULL)
    {
        return;
    }

    if(strcmp(aspect,"名称")==0)
    {
        column->GetName((char *)data);
    }
    else if(strcmp(aspect,"类型")==0)
    {
        *((int *)data)=column->GetType();
    }
    else if(strcmp(aspect,"长度")==0)
    {
        *((int *)data)=column->GetLength();
    }
    else if(strcmp(aspect,"属性")==0)
    {
        *((int *)data)=column->GetAttribute();
    }
}

//获取表格列
void ZYTable::GetColumn(char *name,char *aspect,void *data)
{
    int j=FindColumn(name);

    GetColumn(j,aspect,data);
}

//修改表格列
void ZYTable::SetColumn(int j,char *aspect,void *data)
{
    ZYColumn *column=GetColumn(j);

    //如果是无效属性,则直接返回
    if(column==NULL)
    {
        return;
    }

    if(strcmp(aspect,"名称")==0)
    {
        if(ZYString::Compare(column->name,(char *)data,COLUMN_NAME_LEN)==0)
        {
            ZYString::Copy1(column->name,(char *)data,COLUMN_NAME_LEN);

            //保存表格文件头
            tableFile->WriteColumn(j);
            tableFile->FlushFile();
        }
    }
    else if(strcmp(aspect,"类型")==0)
    {
        if( column->type!=*((int *)data)&&
            *((int *)data)>=EColumnType_String&&
            *((int *)data)<=EColumnType_Integer)
        {
            //将表格复制到临时表格
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

            //从隐藏表重新装入表格数据
            TransferBack(temp);

            //删除临时表格
            temp->Drop();
            delete temp;
        }
    }
    else if(strcmp(aspect,"长度")==0)
    {
        if(column->type==EColumnType_String&&
           column->length!=*((int *)data)&&
           *((int *)data)>=0)
        {
            //将表格复制到临时表格
            ZYTable *temp=CopyToTemp();

            column->length=*((int *)data);

            //从隐藏表重新装入表格数据
            TransferBack(temp);

            //删除临时表格
            temp->Drop();
            delete temp;
        }
    }
    else if(strcmp(aspect,"属性")==0)
    {
        if(column->attribute!=*((int *)data))
        {
            column->attribute=*((int *)data);

            //保存表格文件头
            tableFile->WriteColumn(j);
            tableFile->FlushFile();
        }
    }
}

//修改表格列
void ZYTable::SetColumn(char *name,char *aspect,void *data)
{
    int j=FindColumn(name);

    SetColumn(j,aspect,data);
}

//插入表格列
int ZYTable::InsertColumn(int j1,char name[],EColumnType type,int length,int attribute)
{
    int j=FindColumn(name);

    //如果列不存在,则插入列
    if(j==-1)
    {
        return InsertColumn1(j1,name,type,length,attribute);
    }

    ZYColumn *column=GetColumn(j);

    //确保类型相同

    if(column->GetType()!=type)
    {
        SetColumn(j,"类型",&type);
    }

    //确保长度相同

    if(column->GetType()==0)
    {
        if(column->GetLength()!=length)
        {
            SetColumn(j,"长度",&length);
        }
    }

    return j;
}

//追加表格列
int ZYTable::AppendColumn(char name[],EColumnType type,int length,int attribute)
{
    int j1=GetColumnCount();

    return InsertColumn(j1,name,type,length,attribute);
}

//复制表格列
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

//复制表格列
int ZYTable::CopyColumn(IZYTable *source,char *column_name)
{
    return CopyColumn((ZYTable *)source,column_name);
}

//清空表格列
void ZYTable::ClearColumns(void)
{
    this->column_cnt=0;
    tableFile->WriteHeader();
    tableFile->SetEndOfFile();
    tableFile->FlushFile();
}

//设置主键
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

        SetColumn(j,"属性",&attribute);
    }
}

//设置主键
void ZYTable::SetPrimaryKey(char *name)
{
    int j;

    j=FindColumn(name);
    SetPrimaryKey(j);
}

//获取主键
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