//---------------------------------------------------------
//���½�������(ZYGUI)-���б༭�ؼ� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//������ǿ�ַ�������
ZYStringEx::ZYStringEx(char *s)
{
    this->strEx=NULL;

    this->formatEx=NULL;

    PutString(s);
    
    CheckSql(NULL,NULL);
}

//������ǿ�ַ�������
ZYStringEx::~ZYStringEx(void)
{
    ReleaseStrEx();
}

//�����ַ�������ǿ�ַ���
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

//��ȡ�ַ���
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

//�ͷ���ǿ�ַ���
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

//�ͷŸ�ʽ�ַ���
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

//��ȡ�ַ���
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

//��ȡ�ַ�
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

//��ȡ�ַ��м���
int ZYStringEx::GetLineCount(void)
{
    return strEx->size();
}

//��ȡ���ַ�����
int ZYStringEx::GetCharCount(int lineno)
{
    vector<char> *strLine=(*strEx)[lineno];
    return strLine->size();
}

//��ȡ�ַ������е��ַ���
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

//ɾ���ַ���
void ZYStringEx::DeleteLine(int i)
{
    vector<char> *strLine;
    strLine=(*strEx)[i];
    delete strLine;
    strEx->erase(strEx->begin()+i);
}

//ɾ���ַ�
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
        else if(j==strLine->size()&&i+1<strEx->size())//ɾ������
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

//ɾ���ַ�
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

//��ȡ�ַ���
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

//�����ַ�
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

//���뻻�з�
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

//���SQL���
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

//��ȡ�ַ�����
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

//���Բ�ƥ���־
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

//������ȷ�ϱ�־
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

//������ǿ�༭�ؼ�����
IZYEditEx *IZYEditEx_Create(char *text)
{
    return (IZYEditEx *)new ZYEditEx(text);
}

//�ͷ���ǿ�༭�ؼ�����
void IZYEditEx_Release(IZYEditEx *editEx)
{
    delete (ZYEditEx *)editEx;
}

//������ǿ�༭�ؼ�����
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

//������ǿ�༭�ؼ�����
ZYEditEx::~ZYEditEx(void)
{
    delete this->stringEx;
}

//ת��Ϊ�ؼ�����
IZYControl *ZYEditEx::ToControl(void)
{
    return (IZYControl *)this;
}

//���¹���������
void ZYEditEx::UpdateScrollParam(void)
{
    this->scrollBarV->SetParam(i_base,m_display,stringEx->GetLineCount());

    this->scrollBarH->SetParam(j_base,n_display,stringEx->GetMaxChar());
}

//��ȡ����������
void ZYEditEx::GetScrollBase(void)
{
    i_base=this->scrollBarV->GetIndexBase();

    j_base=this->scrollBarH->GetIndexBase();
}

//���ÿؼ�λ�óߴ�
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

//�����Ƿ�Ϊֻ���༭��
void ZYEditEx::SetReadOnlyEdit(bool readOnlyEdit)
{
    this->readOnlyEdit=readOnlyEdit;
}

//�����Ƿ���SQL���
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

//���ñ༭�ַ���
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

//���ñ༭�ַ���
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

//��ȡ�༭�ַ���
void ZYEditEx::GetEditText(char *text,int bufferLength)
{
    text[0]='\0';

    stringEx->GetString(text,bufferLength);
}

//��ȡѡ�����ַ�
void ZYEditEx::GetSelectedText(char *text,int bufferLength)
{
    text[0]='\0';

    CheckSelectedStartEnd();

    stringEx->GetString(text,i_selectedStart,j_selectedStart,i_selectedEnd,j_selectedEnd,bufferLength);
}

//����������ɫ
void ZYEditEx::SetTextColor(COLORREF color)
{
    this->color=color;
}

//��ȡ��ֱ�������ؼ�
IZYScrollBar *ZYEditEx::GetScrollBarV(void)
{
    return scrollBarV;
}

//��ȡˮƽ�������ؼ�
IZYScrollBar *ZYEditEx::GetScrollBarH(void)
{
    return scrollBarH;
}

//������ص�����༭��
void ZYEditEx::SetRelativeOutput(IZYEditEx *output)
{
    this->output=output;
}

//������ص����ݿ����
void ZYEditEx::SetRelativeDatabase(IZYDatabase *database)
{
    this->database=database;
}

//ʹ�ı���ʼС���ı�����
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

//ʹ�ı���ʼС���ı�����
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

//���ñ��ѡ�����ı�����ɫ
void ZYEditEx::SetMarkSelectedColor(COLORREF colorMarkSelected)
{
    this->colorMarkSelected=colorMarkSelected;
}

//���ѡ�����ı�
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

//���ѡ�����ı�
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

//�����ؼ��ػ�
void ZYEditEx::OnPaint(IZYGraphics *graphics)
{
    char buf[1000];
    int i;

    int h=22;//�������ߴ�

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

//�����ؼ��ػ�
void ZYEditEx::OnPaintSql(IZYGraphics *graphics)
{
    char buf[1000];

    int i,j,j1,c;

    ESourceCharType type;

    bool dismatch;

    bool acknowledged;

    int h=22;//�������ߴ�

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
                    graphics->SetTextColor(0x00FF0080);//��
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

//������갴��
void ZYEditEx::OnMouseDown(int mx,int my)
{
    int h=22;//�������ߴ�

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
    if(TestChar1(i,j)==2)     //���Ĵ��ֽ�
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

//��������ƶ�
void ZYEditEx::OnMouseMove(int mx,int my)
{
    int h=22;//�������ߴ�

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
        if(TestChar1(i,j)==2)     //���Ĵ��ֽ�
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

//������굯��
void ZYEditEx::OnMouseUp(int mx,int my)
{
    ZYContainer::OnMouseUp(mx,my);

    ReleaseCapture();

    stateSelect=false;

    CheckSelectedStartEnd();
}

//����������
void ZYEditEx::OnMouseWheel(int mw)
{
    ZYControl::OnMouseWheel(mw);

    scrollBarV->ToControl()->OnMouseWheel(mw);
}

//�������̰���
void ZYEditEx::OnKeyDown(int vkey)
{
    ZYControl::OnKeyDown(vkey);

    GetScrollBase();

    //������ͬ�Ĺ���
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

    //���²����λ��
    ShowCaret();

    Invalidate();
}

//������������
void ZYEditEx::BarScroll(IZYControl *control,int i_base)
{
    //���²����λ��
    ShowCaret();
}

//0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
int ZYEditEx::TestChar1(int i,int j)
{
    char buf[1000];

    stringEx->GetLine(i,buf);

    return TestChar(buf,j);
}

//0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
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

//�Ƿ������ַ�
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

//�����ַ�����
void ZYEditEx::IncEditIndex(void)
{
    j++;
    
    if(j>j_base+n_display)
    {
        j_base++;
        UpdateScrollParam();
    }
}

//�ݼ��ַ�����
void ZYEditEx::DecEditIndex(void)
{
    j--;
    
    if(j<j_base)
    {
        j_base--;
        UpdateScrollParam();
    }
}

//�ƶ�������������ʼ����
bool ZYEditEx::MoveKeyStart(int type)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    if(stateSelect)
    {
        return true;
    }

    //���SHIFTδ����
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

    //���SHIFT�Ѱ���
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

//�ƶ�������������������
void ZYEditEx::MoveKeyEnd(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //���SHIFT�Ѱ���
    if(shift==true)
    {
        i_selectedEnd=i;
        j_selectedEnd=j;
    }
}

//�ַ������ص���ʼ
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

//�ַ������ص�����
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

//�ݼ��ַ�����,����������
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
        if(IsChineseChar(i,j-2))     //�����ַ�
        {
            DecEditIndex();
            
            DecEditIndex();
        }
        else                       //Ӣ���ַ�
        {
            DecEditIndex();
        }
    }

    MoveKeyEnd();
}

//�����ַ�����,����������
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
        if(IsChineseChar(i,j))//�����ַ�
        {
            IncEditIndex();
            
            IncEditIndex();
        }
        else                       //Ӣ���ַ�
        {
            IncEditIndex();
        }
    }

    MoveKeyEnd();
}

//�ݼ�������,����������
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
        if(TestChar1(i,j)==2)     //���Ĵ��ֽ�
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

//����������,����������
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
        if(TestChar1(i,j)==2)     //���Ĵ��ֽ�
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

//��ҳ�ݼ�������,����������
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

//��ҳ����������,����������
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

//ҳ��ص���ʼ
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

//ҳ��ص�����
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

//���ü��̽���
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

//��ʾ�����
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

//���ز����
void ZYEditEx::HideCaret(void)
{
    if(ZYFrame::caretVisible==1)
    {
        DestroyCaret();

        ZYFrame::caretVisible=0;
    }
}

//�ݼ��ַ�����,���ڻ����ַ�
void ZYEditEx::DecEditIndex2(void)
{
    j--;
    
    if(j_base>0)
    {
        j_base--;
    }
}

//ɾ����ǰѡ�����ַ�
void ZYEditEx::DeleteSelectedText(void)
{
    stringEx->DeleteChars(i_selectedStart,j_selectedStart,i_selectedEnd,j_selectedEnd);
}

//�ӱ༭�ַ���ɾ���ַ�,����������
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
        if(IsChineseChar(i,j))       //�����ַ�
        {
            stringEx->DeleteChar(i,j);
            stringEx->DeleteChar(i,j);
        }
        else                       //Ӣ���ַ�
        {
            stringEx->DeleteChar(i,j);
        }

        UpdateScrollParam();

        CheckSql();
    }
}

//�ڱ༭�ַ����л����ַ�,����������
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

//���뻻�з�
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

//�����ַ�����
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

    //Ӣ���ַ�
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

    //�����ַ�
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

//��ȡ���޸ı�־
bool ZYEditEx::GetModified(void)
{
    return modified;
}

//�������޸ı�־
void ZYEditEx::SetModified(bool modified)
{
    this->modified=modified;
}

//���SQL�﷨
void ZYEditEx::CheckSql(void)
{
    modified=true;

    if(checkSql)
    {
        stringEx->CheckSql(database,output);
    }
}

//���ܸ�֪
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

        lstrcat(output_info,"���ܸ�֪:\n");

        IZYTable *table=database->GetTableI(table_name);

        if(table!=NULL)
        {
            lstrcat(output_info,"����:");

            lstrcat(output_info,table_name);

            lstrcat(output_info,"\n����:");

            for(j2=0;j2<table->GetColumnCount();j2++)
            {
                table->GetColumn(j2,"����",attr_name);

                lstrcat(output_info,attr_name);

                if(j2!=table->GetColumnCount()-1)
                {
                    lstrcat(output_info,",");
                }
            }
        }
        else
        {
            lstrcat(output_info,"����:");

            lstrcat(output_info,"(δ֪����:");

            lstrcat(output_info,table_name);

            lstrcat(output_info,")\n����:(��)");
        }

        output->SetEditText(output_info);
    }
}