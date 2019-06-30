//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-多行编辑控件 作者:蒋政
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//构造增强字符串对象
ZYStringEx::ZYStringEx(char *s)
{
    this->strEx=NULL;

    this->formatEx=NULL;

    PutString(s);
    
    CheckSql(NULL,NULL);
}

//析构增强字符串对象
ZYStringEx::~ZYStringEx(void)
{
    ReleaseStrEx();
}

//解析字符串到增强字符串
void ZYStringEx::PutString(char *s)
{
    int i=0;
    int lineno=0;

    vector<char> *strLine;

    ReleaseStrEx();

    this->strEx=new vector<vector<char> *>();

    strLine=new vector<char>();
    strEx->push_back(strLine);

    if(s!=NULL)
    {
        while(s[i]!='\0')
        {
            if(s[i]=='\n')
            {
                strLine=new vector<char>();
                strEx->push_back(strLine);
                lineno++;
            }
            else
            {
                (*strEx)[lineno]->push_back(s[i]);
            }
            i++;
        }
    }
}

//获取字符串
void ZYStringEx::GetString(char *s,int len)
{
    int i,j,k;
    vector<char> *strLine;

    for(i=0,k=0;i<strEx->size();i++)
    {
        strLine=(*strEx)[i];
        for(j=0;j<strLine->size();j++)
        {
            s[k++]=(*strLine)[j];
            if(k==len)
            {
                s[k]='\0';
                return;
            }
        }
        if(i!=strEx->size()-1)
        {
            s[k++]='\n';
        }
        if(k==len)
        {
            s[k]='\0';
            return;
        }
    }
    s[k]='\0';
}

//释放增强字符串
void ZYStringEx::ReleaseStrEx(void)
{
    int i;

    if(strEx!=NULL)
    {
        for(i=0;i<strEx->size();i++)
        {
            delete (*strEx)[i];
        }

        delete strEx;
    }
}

//释放格式字符串
void ZYStringEx::ReleaseFormatEx(void)
{
    int i;

    if(formatEx!=NULL)
    {
        for(i=0;i<formatEx->size();i++)
        {
            delete (*formatEx)[i];
        }

        delete formatEx;
    }
}

//获取字符行
void ZYStringEx::GetLine(int lineno,char *buf)
{
    vector<char> *strLine;
    if(lineno>=0&&lineno<strEx->size())
    {
        strLine=(*strEx)[lineno];
        strncpy(buf,strLine->begin(),strLine->size());
        buf[strLine->size()]='\0';
    }
    else
    {
        buf[0]='\0';
    }
}

//获取字符
char ZYStringEx::GetChar(int i,int j)
{
    vector<char> *strLine;
    if(i>=0&&i<strEx->size())
    {
        strLine=(*strEx)[i];
        if(j>=0&&j<strLine->size())
        {
            return (*strLine)[j];
        }
    }
    return ' ';
}

//获取字符行计数
int ZYStringEx::GetLineCount(void)
{
    return strEx->size();
}

//获取行字符计数
int ZYStringEx::GetCharCount(int lineno)
{
    vector<char> *strLine=(*strEx)[lineno];
    return strLine->size();
}

//获取字符最多的行的字符数
int ZYStringEx::GetMaxChar(void)
{
    int i;
    int maxChar;
    vector<char> *strLine;

    for(i=0,maxChar=0;i<strEx->size();i++)
    {
        strLine=(*strEx)[i];
        if(strLine->size()>maxChar)
        {
            maxChar=strLine->size();
        }
    }

    return maxChar;
}

//删除字符行
void ZYStringEx::DeleteLine(int i)
{
    vector<char> *strLine;
    strLine=(*strEx)[i];
    delete strLine;
    strEx->erase(strEx->begin()+i);
}

//删除字符
void ZYStringEx::DeleteChar(int i,int j)
{
    int j1;
    vector<char> *strLine;
    vector<char> *strLine1;
    if(i>=0&&i<strEx->size())
    {
        strLine=(*strEx)[i];
        if(j>=0&&j<strLine->size())
        {
            strLine->erase(strLine->begin()+j);
        }
        else if(j==strLine->size()&&i+1<strEx->size())//删除换行
        {
            strLine1=(*strEx)[i+1];
            for(j1=0;j1<strLine1->size();j1++)
            {
                strLine->push_back((*strLine1)[j1]);
            }
            delete strLine1;
            strEx->erase(strEx->begin()+i+1);
        }
    }
}

//删除字符
void ZYStringEx::DeleteChars(int i1,int j1,int i2,int j2)
{
    int i;
    int j;
    int n;

    if(i1==i2)
    {
        for(j=0;j1+j<j2;j++)
        {
            DeleteChar(i1,j1);
        }

        return;
    }

    for(i=i1+1;i<i2;i++)
    {
        DeleteLine(i1+1);
    }

    n=GetCharCount(i1);
    for(j=0;j<n+1-j1;j++)
    {
        DeleteChar(i1,j1);
    }

    for(j=0;j<j2;j++)
    {
        DeleteChar(i1,j1);
    }
}

//获取字符串
void ZYStringEx::GetString(char *s,int i1,int j1,int i2,int j2,int len)
{
    int i;
    int j;
    int k;
    int n;

    k=0;

    if(i1==i2)
    {
        for(j=0;j1+j<j2;j++)
        {
            s[k++]=GetChar(i1,j1+j);
        }

        s[k++]='\0';

        return;
    }

    n=GetCharCount(i1);
    for(j=0;j<n-j1;j++)
    {
        s[k++]=GetChar(i1,j1+j);
    }

    s[k++]='\n';

    for(i=i1+1;i<i2;i++)
    {
        n=GetCharCount(i);
        for(j=0;j<n;j++)
        {
            s[k++]=GetChar(i,j);
        }

        s[k++]='\n';
    }

    for(j=0;j<j2;j++)
    {
        s[k++]=GetChar(i2,j);
    }

    s[k++]='\0';
}

//插入字符
void ZYStringEx::InsertChar(int i,int j,char ch)
{
    vector<char> *strLine;
    if(i>=0&&i<strEx->size())
    {
        strLine=(*strEx)[i];
        if(j>=0&&j<=strLine->size())
        {
            strLine->insert(strLine->begin()+j,ch);
        }
    }
}

//插入换行符
void ZYStringEx::InsertNewLine(int i,int j)
{
    vector<char> *strLine1;
    vector<char> *strLine2;
    if(i>=0&&i<strEx->size())
    {
        strLine1=(*strEx)[i];
        strLine2=new vector<char>();

        strEx->insert(strEx->begin()+i+1,strLine2);

        while(j<strLine1->size())
        {
            strLine2->push_back((*strLine1)[j]);
            strLine1->erase(strLine1->begin()+j);
        }
    }
}

//检查SQL语句
void ZYStringEx::CheckSql(IZYDatabase *database,IZYEditEx *output)
{
    char sql[10000];
    char format[10000];
    int i,j,k;
    vector<char> *strLine;
    vector<char> *formatLine;
    char *checking;
    
    GetString(sql,10000);

    IZYCompiler *compiler=IZYCompiler_Create("checking.lst",database);
    
    compiler->CheckSQL(sql,format);

    IZYCompiler_Release(compiler);

    ReleaseFormatEx();

    this->formatEx=new vector<vector<char> *>();

    for(i=0,k=0;i<strEx->size();i++)
    {
        strLine=(*strEx)[i];

        formatLine=new vector<char>();
        formatEx->push_back(formatLine);

        for(j=0;j<strLine->size();j++)
        {
            (*formatEx)[i]->push_back(format[k++]);
        }

        k++;
    }

    if(output!=NULL)
    {
        checking=IZYTextFile_LoadText("checking.lst");

        output->SetEditText(checking);

        if(checking!=NULL)
        {
            IZYTextFile_FreeText(checking);
        }

        IZYTextFile_DeleteText("checking.lst");
    }
}

//获取字符类型
ESourceCharType ZYStringEx::GetCharType(int i,int j)
{
    char type;
    vector<char> *formatLine;
    if(i>=0&&i<formatEx->size())
    {
        formatLine=(*formatEx)[i];
        if(j>=0&&j<formatLine->size())
        {
            type=(*formatLine)[j];
            type&=0x0F;
            return (ESourceCharType)type;
        }
    }
    return ESourceCharType_Unknown;
}

//测试不匹配标志
bool ZYStringEx::TestDismatchFlag(int i,int j)
{
    ESourceCharType type;
    vector<char> *formatLine;
    if(i>=0&&i<formatEx->size())
    {
        formatLine=(*formatEx)[i];
        if(j>=0&&j<formatLine->size())
        {
            type=(ESourceCharType)(*formatLine)[j];
            return (type&DISMATCH_FLAG)!=0;
        }
    }
    return false;
}

//测试已确认标志
bool ZYStringEx::TestAcknowledgedFlag(int i,int j)
{
    ESourceCharType type;
    vector<char> *formatLine;
    if(i>=0&&i<formatEx->size())
    {
        formatLine=(*formatEx)[i];
        if(j>=0&&j<formatLine->size())
        {
            type=(ESourceCharType)(*formatLine)[j];
            return (type&ACKNOWLEDGED_FLAG)!=0;
        }
    }
    return false;
}

//创建增强编辑控件对象
IZYEditEx *IZYEditEx_Create(char *text)
{
    return (IZYEditEx *)new ZYEditEx(text);
}

//释放增强编辑控件对象
void IZYEditEx_Release(IZYEditEx *editEx)
{
    delete (ZYEditEx *)editEx;
}

//构造增强编辑控件对象
ZYEditEx::ZYEditEx(char *text)
{
    this->stringEx=new ZYStringEx(text);
    this->scrollBarV=IZYScrollBar_Create(0,10,stringEx->GetLineCount(),EScrollType_Vertical);
    this->scrollBarV->SetScrollListener(this);
    this->scrollBarH=IZYScrollBar_Create(0,10,stringEx->GetMaxChar(),EScrollType_Horizontal);
    this->scrollBarH->SetScrollListener(this);
    this->AddControl(scrollBarV->ToControl());
    this->AddControl(scrollBarH->ToControl());
    this->readOnlyEdit=false;
    this->checkSql=false;
    this->i=0;
    this->j=0;
    this->i_base=0;
    this->j_base=0;
    this->color=0x00000000;
    colorMarkSelected=0x00E0E0E0;
    i_selectedStart=0;
    i_selectedEnd=0;
    j_selectedStart=0;
    j_selectedEnd=0;
    stateSelect=false;
    this->output=NULL;
    this->database=NULL;
    CheckSql();
    this->modified=false;
}

//析构增强编辑控件对象
ZYEditEx::~ZYEditEx(void)
{
    delete this->stringEx;
}

//转换为控件对象
IZYControl *ZYEditEx::ToControl(void)
{
    return (IZYControl *)this;
}

//更新滚动条参数
void ZYEditEx::UpdateScrollParam(void)
{
    this->scrollBarV->SetParam(i_base,m_display,stringEx->GetLineCount());

    this->scrollBarH->SetParam(j_base,n_display,stringEx->GetMaxChar());
}

//获取滚动条基数
void ZYEditEx::GetScrollBase(void)
{
    i_base=this->scrollBarV->GetIndexBase();

    j_base=this->scrollBarH->GetIndexBase();
}

//设置控件位置尺寸
void ZYEditEx::SetLocSize(int x,int y,int width,int height)
{
    int h=22;

    this->line_height=22;

    this->x=x;

    this->y=y;

    this->width=width;

    this->height=height;
    
    this->m_display=(height-h)/(line_height-3);

    this->n_display=(width-h)/8-1;

    UpdateScrollParam();

    this->scrollBarV->ToControl()->SetLocSize(width-h,0,h,height-h);

    this->scrollBarH->ToControl()->SetLocSize(0,height-h,width-h,h);
}

//设置是否为只读编辑框
void ZYEditEx::SetReadOnlyEdit(bool readOnlyEdit)
{
    this->readOnlyEdit=readOnlyEdit;
}

//设置是否检查SQL语句
void ZYEditEx::SetCheckSql(bool checkSql)
{
    bool saved_modified;

    this->checkSql=checkSql;

    if(checkSql)
    {
        saved_modified=this->modified;
        
        CheckSql();
        
        modified=saved_modified;
    }
}

//设置编辑字符串
void ZYEditEx::SetEditText(char *text)
{
    stringEx->PutString(text);

    i=0;

    j=0;

    i_base=0;

    j_base=0;

    i_selectedStart=0;
    
    i_selectedEnd=0;
    
    j_selectedStart=0;
    
    j_selectedEnd=0;

    UpdateScrollParam();

    if(ZYFrame::focusControl==this->ToControl())
    {
        ShowCaret();
    }
    
    CheckSql();

    Invalidate();
}

//设置编辑字符串
void ZYEditEx::SetSelectedText(char *text)
{
    if( (i_selectedStart!=i_selectedEnd||
         j_selectedStart!=j_selectedEnd))
    {
        stateSelect=false;
        DeleteChar();
    }

    int n=lstrlen(text);
    int j1;

    for(j1=0;j1<n;j1++)
    {
        char ch=text[j1];
        if(ch=='\n')
        {
            InsertNewLine();
        }
        else
        {
            stringEx->InsertChar(i,j,ch);
            IncEditIndex();
        }
    }

    CheckSql();    

    i_selectedStart=0;
    
    i_selectedEnd=0;
    
    j_selectedStart=0;
    
    j_selectedEnd=0;
    
    ShowCaret();
    
    Invalidate();
}

//获取编辑字符串
void ZYEditEx::GetEditText(char *text,int bufferLength)
{
    text[0]='\0';

    stringEx->GetString(text,bufferLength);
}

//获取选定的字符
void ZYEditEx::GetSelectedText(char *text,int bufferLength)
{
    text[0]='\0';

    CheckSelectedStartEnd();

    stringEx->GetString(text,i_selectedStart,j_selectedStart,i_selectedEnd,j_selectedEnd,bufferLength);
}

//设置文字颜色
void ZYEditEx::SetTextColor(COLORREF color)
{
    this->color=color;
}

//获取垂直滚动条控件
IZYScrollBar *ZYEditEx::GetScrollBarV(void)
{
    return scrollBarV;
}

//获取水平滚动条控件
IZYScrollBar *ZYEditEx::GetScrollBarH(void)
{
    return scrollBarH;
}

//设置相关的输出编辑框
void ZYEditEx::SetRelativeOutput(IZYEditEx *output)
{
    this->output=output;
}

//设置相关的数据库对象
void ZYEditEx::SetRelativeDatabase(IZYDatabase *database)
{
    this->database=database;
}

//使文本开始小于文本结束
void ZYEditEx::CheckSelectedStartEnd(void)
{
    int temp;

    if(i_selectedEnd>i_selectedStart)
    {
        return;
    }

    if(i_selectedEnd==i_selectedStart&&
        j_selectedEnd>=j_selectedStart)
    {
        return;
    }

    temp=i_selectedStart;
    i_selectedStart=i_selectedEnd;
    i_selectedEnd=temp;

    temp=j_selectedStart;
    j_selectedStart=j_selectedEnd;
    j_selectedEnd=temp;
}

//使文本开始小于文本结束
void ZYEditEx::CheckSelectedStartEnd1(int *p_i_selectedStart,
                                      int *p_i_selectedEnd,
                                      int *p_j_selectedStart,
                                      int *p_j_selectedEnd)
{
    if((i_selectedEnd>i_selectedStart)||
       (i_selectedEnd==i_selectedStart&&
        j_selectedEnd>=j_selectedStart))
    {
        *p_i_selectedStart=i_selectedStart;
        *p_i_selectedEnd=i_selectedEnd;
        *p_j_selectedStart=j_selectedStart;
        *p_j_selectedEnd=j_selectedEnd;
    }
    else
    {
        *p_i_selectedStart=i_selectedEnd;
        *p_i_selectedEnd=i_selectedStart;
        *p_j_selectedStart=j_selectedEnd;
        *p_j_selectedEnd=j_selectedStart;
    }
}

//设置标记选定的文本的颜色
void ZYEditEx::SetMarkSelectedColor(COLORREF colorMarkSelected)
{
    this->colorMarkSelected=colorMarkSelected;
}

//标记选定的文本
void ZYEditEx::MarkSelectedText1(IZYGraphics *graphics,int i,int start,int end)
{
    if(i>=0&&i<m_display&&start!=end)
    {
        if(start<0)
        {
            start=0;
        }
        if(end<=0)
        {
            return;
        }
        if(start==0&&end==0)
        {
            return;
        }
        graphics->SetPenColor(colorMarkSelected);
        graphics->FillRectangle1(x+3+start*8,y+2+19*i,8*(end-start),18,colorMarkSelected);
        graphics->DrawRectangle(x+3+start*8,y+2+19*i,8*(end-start),18);
        graphics->ResetPenColor();
    }
}

//标记选定的文本
void ZYEditEx::MarkSelectedText(IZYGraphics *graphics)
{
    int n,i1;
    int i_selectedStart;
    int i_selectedEnd;
    int j_selectedStart;
    int j_selectedEnd;

    CheckSelectedStartEnd1(&i_selectedStart,&i_selectedEnd,&j_selectedStart,&j_selectedEnd);

    if(i_selectedStart==i_selectedEnd)
    {
        MarkSelectedText1(graphics,i_selectedStart-i_base,j_selectedStart-j_base,j_selectedEnd-j_base);

        return;
    }

    n=stringEx->GetCharCount(i_selectedStart);
    MarkSelectedText1(graphics,i_selectedStart-i_base,j_selectedStart-j_base,n-j_base);

    for(i1=i_selectedStart+1;i1<i_selectedEnd;i1++)
    {
        n=stringEx->GetCharCount(i1);
        MarkSelectedText1(graphics,i1-i_base,0-j_base,n-j_base);
    }

    MarkSelectedText1(graphics,i_selectedEnd-i_base,0-j_base,j_selectedEnd-j_base);
}

//处理控件重绘
void ZYEditEx::OnPaint(IZYGraphics *graphics)
{
    char buf[1000];
    int i;

    int h=22;//滚动条尺寸

    int m=stringEx->GetLineCount();

    if(checkSql)
    {
        OnPaintSql(graphics);
        return;
    }

    GetScrollBase();

    ZYControl::OnPaint(graphics);

    if(ZYFrame::focusControl==this)
    {
        MarkSelectedText(graphics);
    }

    graphics->SetTextColor(color);

    for(i=0;i+i_base<m&&i<m_display;i++)
    {
        stringEx->GetLine(i+i_base,buf);

        if(j_base<strlen(buf))
        {
            switch(TestChar(buf,j_base))
            {
            case 0:
            case 1:
                graphics->OutputText(buf+j_base,x,y+i*(line_height-3),width-h,line_height,DT_LEFT);
                break;
            case 2:
                graphics->OutputText(buf+j_base+1,x+8,y+i*(line_height-3),width-h-8,line_height,DT_LEFT);
                break;
            }
        }
    }

    graphics->SetTextColor(0x00000000);

    ZYContainer::OnPaint(graphics);
}

//处理控件重绘
void ZYEditEx::OnPaintSql(IZYGraphics *graphics)
{
    char buf[1000];

    int i,j,j1,c;

    ESourceCharType type;

    bool dismatch;

    bool acknowledged;

    int h=22;//滚动条尺寸

    int m=stringEx->GetLineCount();

    int n=stringEx->GetMaxChar();

    GetScrollBase();

    ZYControl::OnPaint(graphics);

    if(ZYFrame::focusControl==this)
    {
        MarkSelectedText(graphics);
    }

    graphics->SetPenColor(0x000000FF);

    for(i=0;i+i_base<m&&i<m_display;i++)
    {
        for(j=0;j+j_base<n&&j<n_display;j++)
        {
            if(TestChar1(i+i_base,j+j_base)==2)
            {
                j++;
            }

            j1=j;
            buf[0]=stringEx->GetChar(i+i_base,j+j_base);
            buf[1]='\0';
            c=1;

            type=stringEx->GetCharType(i+i_base,j+j_base);
            dismatch=stringEx->TestDismatchFlag(i+i_base,j+j_base);
            acknowledged=stringEx->TestAcknowledgedFlag(i+i_base,j+j_base);

            if(buf[0]<0)
            {
                buf[1]=stringEx->GetChar(i+i_base,j+1+j_base);
                buf[2]='\0';
                j++;
                c=2;
            }

            switch(type)
            {
            case ESourceCharType_Comment:
                graphics->SetTextColor(0x0060A00A);
                break;
            case ESourceCharType_Keyword:
            case ESourceCharType_Punctuation:
                graphics->SetTextColor(0x00FF3E3E);
                break;
            case ESourceCharType_String:
            case ESourceCharType_Num:
            case ESourceCharType_Float:
                graphics->SetTextColor(0x004080FF);
                break;
            case ESourceCharType_Identifier:
                if(acknowledged)
                {
                    graphics->SetTextColor(0x00FF0080);//紫
                }
                else
                {
                    graphics->SetTextColor(0x00000000);
                }
                break;
            default:
                graphics->SetTextColor(0x00000000);
            }

            graphics->OutputText(buf,x+j1*8,y+i*(line_height-3),100,line_height,DT_LEFT);

            if(dismatch)
            {
                graphics->DrawLine(x+j1*8+2,y+i*(line_height-3)+line_height-2,x+j1*8+c*8+3,y+i*(line_height-3)+line_height-2);
            }
        }
    }

    graphics->SetTextColor(0x00000000);

    graphics->ResetPenColor();

    ZYContainer::OnPaint(graphics);
}

//处理鼠标按下
void ZYEditEx::OnMouseDown(int mx,int my)
{
    int h=22;//滚动条尺寸

    ZYContainer::OnMouseDown(mx,my);

    this->SetFocus(true);

    if(my>=y+height-h||mx>=x+width-h)
    {
        return;
    }

    GetScrollBase();

    i=i_base+(my-y)/(line_height-3);

    if(i>=stringEx->GetLineCount())
    {
        i=stringEx->GetLineCount()-1;
    }

    j=j_base+(mx-x)/8;

    if(j>stringEx->GetCharCount(i))
    {
        j=stringEx->GetCharCount(i);
    }

    if(TestChar1(i,j)==2)
    {
        j--;
    }

    if(j>stringEx->GetCharCount(i))
    {
        j=stringEx->GetCharCount(i);
    }
    if(TestChar1(i,j)==2)     //中文次字节
    {
        j--;
    }
    if(j<j_base)
    {
        j_base=j-n_display;
        if(j_base<0)
        {
            j_base=0;
        }
    }
    if(i<i_base)
    {
        i_base--;
    }
    
    UpdateScrollParam();

    i_selectedStart=i;
    j_selectedStart=j;
    i_selectedEnd=i;
    j_selectedEnd=j;
    stateSelect=true;

    SetCapture(frame->GetWindowHandle());
    
    ShowCaret();
}

//处理鼠标移动
void ZYEditEx::OnMouseMove(int mx,int my)
{
    int h=22;//滚动条尺寸

    ZYContainer::OnMouseMove(mx,my);

    if(stateSelect)
    {
        if(my>=y+height-h||mx>=x+width-h)
        {
            return;
        }

        GetScrollBase();

        i=i_base+(my-y)/(line_height-3);

        if(i>=stringEx->GetLineCount())
        {
            i=stringEx->GetLineCount()-1;
        }

        j=j_base+(mx-x)/8;

        if(j>stringEx->GetCharCount(i))
        {
            j=stringEx->GetCharCount(i);
        }

        if(TestChar1(i,j)==2)
        {
            j--;
        }

        if(j>stringEx->GetCharCount(i))
        {
            j=stringEx->GetCharCount(i);
        }
        if(TestChar1(i,j)==2)     //中文次字节
        {
            j--;
        }
        if(j<j_base)
        {
            j_base=j-n_display;
            if(j_base<0)
            {
                j_base=0;
            }
        }
        if(i<i_base)
        {
            i_base--;
        }

        if( i_selectedEnd!=i||
            j_selectedEnd!=j )
        {
            i_selectedEnd=i;
            j_selectedEnd=j;

            ShowCaret();

            Invalidate();
        }
    }
}

//处理鼠标弹起
void ZYEditEx::OnMouseUp(int mx,int my)
{
    ZYContainer::OnMouseUp(mx,my);

    ReleaseCapture();

    stateSelect=false;

    CheckSelectedStartEnd();
}

//处理鼠标滚动
void ZYEditEx::OnMouseWheel(int mw)
{
    ZYControl::OnMouseWheel(mw);

    scrollBarV->ToControl()->OnMouseWheel(mw);
}

//处理键盘按下
void ZYEditEx::OnKeyDown(int vkey)
{
    ZYControl::OnKeyDown(vkey);

    GetScrollBase();

    //处理不同的功能
    switch(vkey)
    {
    case VK_UP:
        UpKey();
        break;
    case VK_DOWN:
        DownKey();
        break;
    case VK_LEFT:
        LeftKey();
        break;
    case VK_RIGHT:
        RightKey();
        break;
    case VK_PRIOR:
        PageUp();
        break;
    case VK_NEXT:
        PageDown();
        break;
    case VK_HOME:
        HomeEditIndex();
        break;
    case VK_END:
        EndEditIndex();
        break;
    case VK_DELETE:
        DeleteChar();
        break;
    case VK_BACK:
        BackspaceChar();
        break;
    case VK_RETURN:
        InsertNewLine();
        break;
    default:
        return;
    }

    //更新插入符位置
    ShowCaret();

    Invalidate();
}

//滚动处理函数
void ZYEditEx::BarScroll(IZYControl *control,int i_base)
{
    //更新插入符位置
    ShowCaret();
}

//0:英文,1:中文首字节,2:中文次字节
int ZYEditEx::TestChar1(int i,int j)
{
    char buf[1000];

    stringEx->GetLine(i,buf);

    return TestChar(buf,j);
}

//0:英文,1:中文首字节,2:中文次字节
int ZYEditEx::TestChar(char *text,int i)
{
    int j;

    int s;

    s=0;

    for(j=0;j<=i;j++)
    {
        if(text[j]>0)
        {
            if(i==j)
            {
                return 0;
            }
        }
        if(text[j]<0)
        {
            if(s==0)
            {
                if(i==j)
                {
                    return 1;
                }
                s=1;
            }
            else
            {
                if(i==j)
                {
                    return 2;
                }
                s=0;
            }
        }
    }
    return 0;
}

//是否中文字符
bool ZYEditEx::IsChineseChar(int i,int j)
{
    char ch1,ch2;

    if(j+1<stringEx->GetCharCount(i))
    {
        ch1=stringEx->GetChar(i,j);
        ch2=stringEx->GetChar(i,j+1);
        
        if(ch1<0&&ch2<0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

//递增字符索引
void ZYEditEx::IncEditIndex(void)
{
    j++;
    
    if(j>j_base+n_display)
    {
        j_base++;
        UpdateScrollParam();
    }
}

//递减字符索引
void ZYEditEx::DecEditIndex(void)
{
    j--;
    
    if(j<j_base)
    {
        j_base--;
        UpdateScrollParam();
    }
}

//移动键处理函数开始处理
bool ZYEditEx::MoveKeyStart(int type)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    if(stateSelect)
    {
        return true;
    }

    //如果SHIFT未按下
    if(shift==false)
    {
        if(i_selectedStart!=i_selectedEnd||
           j_selectedStart!=j_selectedEnd)
        {
            CheckSelectedStartEnd();
            if(type==0)
            {
                i=i_selectedStart;
                j=j_selectedStart;
            }
            else if(type==1)
            {
                i=i_selectedEnd;
                j=j_selectedEnd;
            }
            i_selectedStart=0;
            i_selectedEnd=0;
            j_selectedStart=0;
            j_selectedEnd=0;
            Invalidate();
            return true;
        }
    }

    //如果SHIFT已按下
    if(shift==true)
    {
        if(i_selectedStart==0&&
           i_selectedEnd==0&&
           j_selectedStart==0&&
           j_selectedEnd==0)
        {
            i_selectedStart=i;
            i_selectedEnd=i;
            j_selectedStart=j;
            j_selectedEnd=j;
        }
    }

    return false;
}

//移动键处理函数结束处理
void ZYEditEx::MoveKeyEnd(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //如果SHIFT已按下
    if(shift==true)
    {
        i_selectedEnd=i;
        j_selectedEnd=j;
    }
}

//字符索引回到开始
void ZYEditEx::HomeEditIndex(void)
{
    if(MoveKeyStart(0))
    {
        return;
    }

    j=0;
    
    j_base=0;

    UpdateScrollParam();

    MoveKeyEnd();
}

//字符索引回到结束
void ZYEditEx::EndEditIndex(void)
{
    if(MoveKeyStart(1))
    {
        return;
    }

    int n=stringEx->GetCharCount(i);

    j=n;
    
    if(n>n_display)
    {
        j_base=n-n_display;

        UpdateScrollParam();
    }

    MoveKeyEnd();
}

//递减字符索引,考虑了中文
void ZYEditEx::LeftKey(void)
{
    if(MoveKeyStart(0))
    {
        return;
    }

    if(i==0&&j==0)
    {
        return;
    }

    if(j==0)
    {
        j=stringEx->GetCharCount(i-1);
        UpKey();
    }
    else if(j>0)
    {
        if(IsChineseChar(i,j-2))     //中文字符
        {
            DecEditIndex();
            
            DecEditIndex();
        }
        else                       //英文字符
        {
            DecEditIndex();
        }
    }

    MoveKeyEnd();
}

//递增字符索引,考虑了中文
void ZYEditEx::RightKey(void)
{
    if(MoveKeyStart(1))
    {
        return;
    }

    int n=stringEx->GetCharCount(i);

    int m=stringEx->GetLineCount();

    if(i==m-1&&j==n)
    {
        return;
    }

    if(j==n)
    {
        j=0;
        DownKey();
    }
    else if(j<n)
    {
        if(IsChineseChar(i,j))//中文字符
        {
            IncEditIndex();
            
            IncEditIndex();
        }
        else                       //英文字符
        {
            IncEditIndex();
        }
    }

    MoveKeyEnd();
}

//递减行索引,考虑了中文
void ZYEditEx::UpKey(void)
{
    if(MoveKeyStart(0))
    {
        return;
    }
    
    if(i>0)
    {
        i--;
        if(j>stringEx->GetCharCount(i))
        {
            j=stringEx->GetCharCount(i);
        }
        if(TestChar1(i,j)==2)     //中文次字节
        {
            j--;
        }
        if(j<j_base)
        {
            j_base=j-n_display;
            if(j_base<0)
            {
                j_base=0;
            }
        }
        if(i<i_base)
        {
            i_base--;
        }
        UpdateScrollParam();
    }

    MoveKeyEnd();
}

//递增行索引,考虑了中文
void ZYEditEx::DownKey(void)
{
    if(MoveKeyStart(1))
    {
        return;
    }

    int m=stringEx->GetLineCount();

    if(i<m-1)
    {
        i++;
        if(j>stringEx->GetCharCount(i))
        {
            j=stringEx->GetCharCount(i);
        }
        if(TestChar1(i,j)==2)     //中文次字节
        {
            j--;
        }
        if(j<j_base)
        {
            j_base=j-n_display;
            if(j_base<0)
            {
                j_base=0;
            }
        }
        if(i>i_base+m_display-1)
        {
            i_base++;
        }
        UpdateScrollParam();
    }

    MoveKeyEnd();
}

//按页递减行索引,考虑了中文
void ZYEditEx::PageUp(void)
{
    int i;

    if(MoveKeyStart(0))
    {
        return;
    }

    if(readOnlyEdit)
    {
        i_base-=m_display;
        if(i_base<0)
        {
            i_base=0;
        }
        UpdateScrollParam();
        MoveKeyEnd();
        return;
    }

    j=0;
    j_base=0;
    for(i=0;i<m_display;i++)
    {
        UpKey();
    }

    MoveKeyEnd();
}

//按页递增行索引,考虑了中文
void ZYEditEx::PageDown(void)
{
    int i;

    if(MoveKeyStart(1))
    {
        return;
    }

    int m=stringEx->GetLineCount();
    
    if(readOnlyEdit)
    {
        i_base+=m_display;
        if(i_base+m_display>m)
        {
            i_base=m-m_display;
        }
        UpdateScrollParam();
        MoveKeyEnd();
        return;
    }

    j=0;
    j_base=0;
    for(i=0;i<m_display;i++)
    {
        DownKey();
    }

    MoveKeyEnd();
}

//页面回到开始
void ZYEditEx::LocateToHome(void)
{
    i=0;

    j=0;

    i_base=0;

    j_base=0;

    UpdateScrollParam();

    if(ZYFrame::focusControl==this->ToControl())
    {
        ShowCaret();
    }

    Invalidate();
}

//页面回到结束
void ZYEditEx::LocateToEnd(void)
{
    int m=stringEx->GetLineCount();

    i=m-1;

    j=stringEx->GetCharCount(i);

    if(m>m_display)
    {
        i_base=m-m_display;
    }

    j_base=0;
    
    UpdateScrollParam();

    if(ZYFrame::focusControl==this->ToControl())
    {
        ShowCaret();
    }

    Invalidate();
}

//设置键盘焦点
void ZYEditEx::SetFocus(bool focus)
{
    ZYControl::SetFocus(focus);

    if(focus)
    {
        ShowCaret();
    }
    else
    {
        HideCaret();
    }
}

//显示插入符
void ZYEditEx::ShowCaret(void)
{
    if(readOnlyEdit)
    {
        return;
    }

    HWND hWnd=GetFrame()->GetWindowHandle();

    i_base=this->scrollBarV->GetIndexBase();

    j_base=this->scrollBarH->GetIndexBase();

    if( i-i_base<0||i-i_base>m_display-1||
        j-j_base<0||j-j_base>n_display)
    {
        HideCaret();
        return;
    }

    CreateCaret(hWnd,NULL,2,line_height-6);

    SetCaretPos(x+(j-j_base)*8+3,y+(i-i_base)*(line_height-3)+3);

    ::ShowCaret(hWnd);

    ZYFrame::caretVisible=1;
}

//隐藏插入符
void ZYEditEx::HideCaret(void)
{
    if(ZYFrame::caretVisible==1)
    {
        DestroyCaret();

        ZYFrame::caretVisible=0;
    }
}

//递减字符索引,用于回退字符
void ZYEditEx::DecEditIndex2(void)
{
    j--;
    
    if(j_base>0)
    {
        j_base--;
    }
}

//删除当前选定的字符
void ZYEditEx::DeleteSelectedText(void)
{
    stringEx->DeleteChars(i_selectedStart,j_selectedStart,i_selectedEnd,j_selectedEnd);
}

//从编辑字符串删除字符,考虑了中文
void ZYEditEx::DeleteChar(void)
{
    if(stateSelect)
    {
        return;
    }

    if(i_selectedStart!=i_selectedEnd||
       j_selectedStart!=j_selectedEnd)
    {
        CheckSelectedStartEnd();
        DeleteSelectedText();

        i=i_selectedStart;
        j=j_selectedStart;

        if(!readOnlyEdit)
        {
            this->SetFocus(true);
        }

        i_selectedStart=0;
        i_selectedEnd=0;
        j_selectedStart=0;
        j_selectedEnd=0;
        
        UpdateScrollParam();

        CheckSql();

        return;
    }

    int n=stringEx->GetCharCount(i);

    if(readOnlyEdit)
    {
        return;
    }

    if(j<n+1)
    {
        if(IsChineseChar(i,j))       //中文字符
        {
            stringEx->DeleteChar(i,j);
            stringEx->DeleteChar(i,j);
        }
        else                       //英文字符
        {
            stringEx->DeleteChar(i,j);
        }

        UpdateScrollParam();

        CheckSql();
    }
}

//在编辑字符串中回退字符,考虑了中文
void ZYEditEx::BackspaceChar(void)
{
    if(readOnlyEdit)
    {
        return;
    }

    if(stateSelect)
    {
        return;
    }

    if(i==0&&j==0)
    {
        return;
    }

    if(i_selectedStart!=i_selectedEnd||
       j_selectedStart!=j_selectedEnd)
    {
        DeleteChar();

        return;
    }

    LeftKey();

    DeleteChar();
}

//插入换行符
void ZYEditEx::InsertNewLine(void)
{
    if(readOnlyEdit)
    {
        return;
    }

    if(stateSelect)
    {
        return;
    }

    if(i_selectedStart!=i_selectedEnd||
       j_selectedStart!=j_selectedEnd)
    {
        DeleteChar();
    }

    stringEx->InsertNewLine(i,j);

    CheckSql();
        
    j=0;
    DownKey();
        
    ShowCaret();
        
    Invalidate();
}

//处理字符输入
void ZYEditEx::OnCharInput(char ch)
{
    static int p=0;

    if(readOnlyEdit)
    {
        return;
    }

    if(stateSelect)
    {
        return;
    }

    //英文字符
    if(ch>=0x20)
    {
        if(i_selectedStart!=i_selectedEnd||
           j_selectedStart!=j_selectedEnd)
        {
            DeleteChar();
        }

        stringEx->InsertChar(i,j,ch);

        CheckSql();

        if(ch=='.')
        {
            IntelligentSense();
        }

        IncEditIndex();
        
        ShowCaret();
        
        Invalidate();
    }

    //中文字符
    if(ch<0)
    {
        if(i_selectedStart!=i_selectedEnd||
           j_selectedStart!=j_selectedEnd)
        {
            DeleteChar();
        }

        stringEx->InsertChar(i,j,ch);

        if(p==0)
        {
            IncEditIndex();

            p=1;
        }
        else
        {
            CheckSql();

            IncEditIndex();

            ShowCaret();
        
            Invalidate();

            p=0;
        }
    }
}

//获取已修改标志
bool ZYEditEx::GetModified(void)
{
    return modified;
}

//设置已修改标志
void ZYEditEx::SetModified(bool modified)
{
    this->modified=modified;
}

//检查SQL语法
void ZYEditEx::CheckSql(void)
{
    modified=true;

    if(checkSql)
    {
        stringEx->CheckSql(database,output);
    }
}

//智能感知
void ZYEditEx::IntelligentSense(void)
{
    int k=0;
    
    int j1=j-1;

    int j2=0;

    char table_name[1000];

    char attr_name[1000];

    char output_info[10000];
    
    if(checkSql==true&&output!=NULL&&database!=NULL)
    {
        while(stringEx->GetCharType(i+i_base,j1+j_base)==ESourceCharType_Identifier)
        {
            j1--;
        }

        j1++;
        
        while(stringEx->GetCharType(i+i_base,j1+j_base)==ESourceCharType_Identifier)
        {
            table_name[k++]=stringEx->GetChar(i+i_base,j1+j_base);
            j1++;
        }
        
        table_name[k]='\0';

        output_info[0]='\0';

        lstrcat(output_info,"智能感知:\n");

        IZYTable *table=database->GetTableI(table_name);

        if(table!=NULL)
        {
            lstrcat(output_info,"表格:");

            lstrcat(output_info,table_name);

            lstrcat(output_info,"\n属性:");

            for(j2=0;j2<table->GetColumnCount();j2++)
            {
                table->GetColumn(j2,"名称",attr_name);

                lstrcat(output_info,attr_name);

                if(j2!=table->GetColumnCount()-1)
                {
                    lstrcat(output_info,",");
                }
            }
        }
        else
        {
            lstrcat(output_info,"表格:");

            lstrcat(output_info,"(未知表格:");

            lstrcat(output_info,table_name);

            lstrcat(output_info,")\n属性:(空)");
        }

        output->SetEditText(output_info);
    }
}