//---------------------------------------------------------
//���½�������(ZYGUI)-�༭�ؼ� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//�����༭�ؼ�����
IZYEdit *IZYEdit_Create(char *text)
{
    return (IZYEdit *)new ZYEdit(text);
}

//�ͷű༭�ؼ�����
void IZYEdit_Release(IZYEdit *edit)
{
    delete (ZYEdit *)edit;
}

//����༭�ؼ�����
ZYEdit::ZYEdit(char *text)
{
    lstrcpy(this->text,text);

    n=lstrlen(text);
    
    i=n;

    n_display=width/8-1;

    i_base=0;

    passwordEdit=false;

    readOnlyEdit=false;

    colorMarkSelected=0x00E0E0E0;

    selectedStart=0;

    selectedEnd=0;

    stateSelect=false;

    if(n>n_display)
    {
        i_base=n-n_display;
    }
}

//ת��Ϊ�ؼ�����
IZYControl *ZYEdit::ToControl(void)
{
    return (IZYControl *)this;
}

//���ÿؼ�λ�óߴ�
void ZYEdit::SetLocSize(int x,int y,int width,int height)
{
    ZYControl::SetLocSize(x,y,width,height);

    n_display=width/8-1;

    if(n>n_display)
    {
        i_base=n-n_display;
    }
    else
    {
        i_base=0;
    }
}

//�����Ƿ�Ϊ����༭��
void ZYEdit::SetPasswordEdit(bool passwordEdit)
{
    this->passwordEdit=passwordEdit;
}

//�����Ƿ�Ϊֻ���༭��
void ZYEdit::SetReadOnlyEdit(bool readOnlyEdit)
{
    this->readOnlyEdit=readOnlyEdit;
}

//���ñ༭�ַ���
void ZYEdit::SetEditText(char *text)
{
    lstrcpyn(this->text,text,MAX_TEXT_LEN);
        
    n=lstrlen(text);

    if(n>MAX_TEXT_LEN)
        n=MAX_TEXT_LEN;
    
    i=n;

    i_base=0;

    if(ZYFrame::focusControl==this)
    {
        ShowCaret();
    }

    frame->Invalidate();
}

//����ѡ�����ַ���
void ZYEdit::SetSelectedText(char *text)
{
    int j;
    CheckSelectedStartEnd();
    if( stateSelect==false&&
        selectedStart!=selectedEnd)
    {
        DeleteChar2();
    }
    for(j=0;text[j]!=0;j++)
    {
        InsertChar(i,text[j]);
        IncEditIndex();
    }
    ShowCaret();
    Invalidate();
}

//��ȡ�༭�ַ���
void ZYEdit::GetEditText(char *text,int bufferLength)
{
    lstrcpyn(text,this->text,bufferLength);
}

//��ȡѡ�����ַ���
void ZYEdit::GetSelectedText(char *text,int bufferLength)
{
    CheckSelectedStartEnd();
    lstrcpyn(text,this->text+selectedStart,min(bufferLength,selectedEnd-selectedStart+1));
}

//���ü��̽���
void ZYEdit::SetFocus(bool focus)
{
    ZYControl::SetFocus(focus);

    if(focus)
    {
        if(!readOnlyEdit)
        {
            ShowCaret();
        }
    }
    else
    {
        HideCaret();
    }
}

//���ñ��ѡ�����ı�����ɫ
void ZYEdit::SetMarkSelectedColor(COLORREF colorMarkSelected)
{
    this->colorMarkSelected=colorMarkSelected;
}

//���ѡ�����ı�
void ZYEdit::MarkSelectedText1(IZYGraphics *graphics,int start,int end)
{
    if(start!=end)
    {
        graphics->SetPenColor(colorMarkSelected);
        graphics->FillRectangle1(x+3+start*8,y+3,8*(end-start),16,colorMarkSelected);
        graphics->DrawRectangle(x+3+start*8,y+3,8*(end-start),16);
        graphics->ResetPenColor();
    }
}

//���ѡ�����ı�
void ZYEdit::MarkSelectedText(IZYGraphics *graphics)
{
    int selectedStart1=min(selectedStart,selectedEnd);
    int selectedEnd1=max(selectedStart,selectedEnd);
    MarkSelectedText1(graphics,selectedStart1-i_base,selectedEnd1-i_base);
}

//�����ؼ��ػ�
void ZYEdit::OnPaint(IZYGraphics *graphics)
{
    char buf[MAX_TEXT_LEN];

    int j;

    char *text1;

    ZYControl::OnPaint(graphics);

    if(passwordEdit)
    {
        for(j=0;j<n;j++)
        {
            if(text[j]>=0)
            {       //Ӣ�ı�*
                buf[j]='*';
            }
            else    //���ı䣪
            {
                buf[j]=(char)0xA3;
        
                j++;
                
                buf[j]=(char)0xAA;
            }
        }

        buf[j]='\0';

        text1=buf;
    }
    else
    {
        text1=text;
    }

    if(ZYFrame::focusControl==this)
    {
        MarkSelectedText(graphics);

        switch(TestChar(text1,i_base))
        {
        case 0:
        case 1:
            graphics->OutputText(text1+i_base,x,y,width,height,DT_LEFT);
            break;
        case 2:
            graphics->OutputText(text1+i_base+1,x+8,y,width,height,DT_LEFT);
            break;
        }
    }
    else
    {
        graphics->OutputText(text1,x,y,width,height,DT_LEFT);
    }
}

//������갴��
void ZYEdit::OnMouseDown(int mx,int my)
{
    ZYControl::OnMouseDown(mx,my);

    i=i_base+(mx-x+1)/8;

    if(i<i_base)
    {
        i=i_base;
    }

    if(i==i_base)
    {
        if(TestChar(text,i)==2)
        {
            i++;
        }
    }

    if(i>n)
    {
        i=n;
    }

    if(TestChar(text,i)==2)
    {
        i--;
    }

    selectedStart=i;

    selectedEnd=i;

    stateSelect=true;

    SetCapture(frame->GetWindowHandle());

    if(!readOnlyEdit)
    {
        this->SetFocus(true);
    }

    Invalidate();
}

//��������ƶ�
void ZYEdit::OnMouseMove(int mx,int my)
{
    ZYControl::OnMouseMove(mx,my);

    if(stateSelect)
    {
        i=i_base+(mx-x+1)/8;

        if(i<i_base)
        {
            i=i_base;
        }

        if(i==i_base)
        {
            if(TestChar(text,i)==2)
            {
                i++;
            }
        }

        if(i>n)
        {
            i=n;
        }

        if(TestChar(text,i)==2)
        {
            i--;
        }

        if(selectedEnd!=i)
        {
            selectedEnd=i;

            if(!readOnlyEdit)
            {
                this->SetFocus(true);
            }

            Invalidate();
        }
    }
}

//ʹ�ı���ʼС���ı�����
void ZYEdit::CheckSelectedStartEnd(void)
{
    int temp;

    if(selectedEnd<selectedStart)
    {
        temp=selectedStart;
        selectedStart=selectedEnd;
        selectedEnd=temp;
    }
}

//������굯��
void ZYEdit::OnMouseUp(int mx,int my)
{
    ZYControl::OnMouseUp(mx,my);

    stateSelect=false;

    ReleaseCapture();

    CheckSelectedStartEnd();
}

//�������̰���
void ZYEdit::OnKeyDown(int vkey)
{
    ZYControl::OnKeyDown(vkey);

    //������ͬ�Ĺ���
    switch(vkey)
    {
    case VK_LEFT:
        LeftKey();
        break;
    case VK_RIGHT:
        RightKey();
        break;
    case VK_HOME:
        HomeEditIndex();
        break;
    case VK_END:
        EndEditIndex();
        break;
    case VK_DELETE:
        CheckSelectedStartEnd();
        DeleteChar2();
        break;
    case VK_BACK:
        CheckSelectedStartEnd();
        BackspaceChar();
        break;
    case VK_RETURN:
        CheckSelectedStartEnd();
        if(this->container!=NULL)
        {
            container->OnKeyDown(VK_RETURN);
            return;
        }
        break;
    default:
        return;
    }

    //���²����λ��
    ShowCaret();

    Invalidate();
}

//�����ַ�����
void ZYEdit::OnCharInput(char ch)
{
    //һ���ַ�
    if(ch>=0x20||ch<0)
    {
        CheckSelectedStartEnd();
        if( stateSelect==false&&
            selectedStart!=selectedEnd)
        {
            DeleteChar2();
        }
        InsertChar(i,ch);
        IncEditIndex();
        ShowCaret();
        Invalidate();
    }
}

//��ʾ�����
void ZYEdit::ShowCaret(void)
{
    HWND hWnd=GetFrame()->GetWindowHandle();

    CreateCaret(hWnd,NULL,2,16);

    if(container!=NULL)
    {
        SetCaretPos(container->GetXBase()+x+3+(i-i_base)*8,container->GetYBase()+y+3);
    }
    else
    {
        SetCaretPos(x+3+(i-i_base)*8,y+3);
    }

    ::ShowCaret(hWnd);

    ZYFrame::caretVisible=1;
}

//���ز����
void ZYEdit::HideCaret(void)
{
    if(ZYFrame::caretVisible==1)
    {
        DestroyCaret();

        ZYFrame::caretVisible=0;
    }
}

//0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
int ZYEdit::TestChar(char *text,int i)
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
int ZYEdit::IsChineseChar(int i)
{
    if(i+1<n && text[i]<0 && text[i+1]<0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//�����ַ�����
void ZYEdit::IncEditIndex(void)
{
    i++;
    
    if(i>i_base+n_display)
    {
        i_base++;
    }
}

//�ݼ��ַ�����
void ZYEdit::DecEditIndex(void)
{
    i--;
    
    if(i<i_base)
    {
        i_base--;
    }
}

//�ַ������ص���ʼ
void ZYEdit::HomeEditIndex(void)
{
    i=0;
    
    i_base=0;

    selectedStart=0;
        
    selectedEnd=0;
}

//�ַ������ص�����
void ZYEdit::EndEditIndex(void)
{
    i=n;
    
    if(n>n_display)
    {
        i_base=n-n_display;
    }

    selectedStart=0;
        
    selectedEnd=0;
}

//�ݼ��ַ�����,���ڻ����ַ�
void ZYEdit::DecEditIndex2(void)
{
    i--;
    
    if(i_base>0)
    {
        i_base--;
    }
}

//�����ַ����༭�ַ���
void ZYEdit::InsertChar(int i,char c)
{
    int j;

    if(n<MAX_TEXT_LEN-2)
    {
        for(j=n;j>i;j--)
        {
            text[j]=text[j-1];
        }
    
        text[i]=c;
        
        n++;
        
        text[n]='\0';
    }
}

//�ӱ༭�ַ���ɾ���ַ�
void ZYEdit::DeleteChar(int i,int c)
{
    for(;i+c<=n;i++)
    {
        text[i]=text[i+c];
    }
    
    n-=c;
    
    text[n]='\0';
}

//�ӱ༭�ַ���ɾ���ַ�,����������
void ZYEdit::DeleteChar2(void)
{
    int j;

    if( stateSelect==false&&
        selectedStart!=selectedEnd)
    {
        for(j=0;selectedStart+j<selectedEnd;j++)
        {
            DeleteChar(selectedStart,1);
        }

        i=selectedStart;

        if(!readOnlyEdit)
        {
            this->SetFocus(true);
        }

        selectedStart=0;
        selectedEnd=0;

        return;
    }

    if(i<n)
    {
        if(IsChineseChar(i))       //�����ַ�
        {
            DeleteChar(i,2);
        }
        else                       //Ӣ���ַ�
        {
            DeleteChar(i,1);
        }
    }
}

//�ڱ༭�ַ����л����ַ�,����������
void ZYEdit::BackspaceChar(void)
{
    if( stateSelect==false&&
        selectedStart!=selectedEnd)
    {
        DeleteChar2();

        return;
    }

    if(i>0)
    {
        if(IsChineseChar(i-2))     //�����ַ�
        {
            DecEditIndex2();
    
            DecEditIndex2();
            
            DeleteChar(i,2);
        }
        else                       //Ӣ���ַ�
        {
            DecEditIndex2();
            
            DeleteChar(i,1);
        }
    }
}

//�ݼ��ַ�����,����������
void ZYEdit::LeftKey(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //���SHIFTδ����
    if(shift==false)
    {
        if( stateSelect==false&&
            selectedStart!=selectedEnd )
        {
            CheckSelectedStartEnd();
            i=selectedStart;
            selectedStart=0;
            selectedEnd=0;
            return;
        }
    }
        
    //���SHIFT�Ѱ���
    if(shift==true)
    {
        if(selectedStart==0&&
           selectedEnd==0)
        {
            selectedStart=i;
            selectedEnd=i;
        }
    }

    if(i>0)
    {
        if(IsChineseChar(i-2))     //�����ַ�
        {
            DecEditIndex();
            
            DecEditIndex();
        }
        else                       //Ӣ���ַ�
        {
            DecEditIndex();
        }
    }

    //���SHIFT�Ѱ���
    if(shift==true)
    {
        selectedEnd=i;
    }
}

//�����ַ�����,����������
void ZYEdit::RightKey(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //���SHIFTδ����
    if(shift==false)
    {
        if( stateSelect==false&&
            selectedStart!=selectedEnd )
        {
            CheckSelectedStartEnd();
            i=selectedEnd;
            selectedStart=0;
            selectedEnd=0;
            return;
        }
    }
    
    //���SHIFT�Ѱ���
    if(shift==true)
    {
        if(selectedStart==0&&
           selectedEnd==0)
        {
            selectedStart=i;
            selectedEnd=i;
        }
    }

    if(i<n)
    {
        if(IsChineseChar(i))       //�����ַ�
        {
            IncEditIndex();
            
            IncEditIndex();
        }
        else                       //Ӣ���ַ�
        {
            IncEditIndex();
        }
    }

    //���SHIFT�Ѱ���
    if(shift==true)
    {
        selectedEnd=i;
    }
}