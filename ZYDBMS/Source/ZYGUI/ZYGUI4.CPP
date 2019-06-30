//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-编辑控件 作者:蒋政
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//创建编辑控件对象
IZYEdit *IZYEdit_Create(char *text)
{
    return (IZYEdit *)new ZYEdit(text);
}

//释放编辑控件对象
void IZYEdit_Release(IZYEdit *edit)
{
    delete (ZYEdit *)edit;
}

//构造编辑控件对象
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

//转换为控件对象
IZYControl *ZYEdit::ToControl(void)
{
    return (IZYControl *)this;
}

//设置控件位置尺寸
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

//设置是否为密码编辑框
void ZYEdit::SetPasswordEdit(bool passwordEdit)
{
    this->passwordEdit=passwordEdit;
}

//设置是否为只读编辑框
void ZYEdit::SetReadOnlyEdit(bool readOnlyEdit)
{
    this->readOnlyEdit=readOnlyEdit;
}

//设置编辑字符串
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

//设置选定的字符串
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

//获取编辑字符串
void ZYEdit::GetEditText(char *text,int bufferLength)
{
    lstrcpyn(text,this->text,bufferLength);
}

//获取选定的字符串
void ZYEdit::GetSelectedText(char *text,int bufferLength)
{
    CheckSelectedStartEnd();
    lstrcpyn(text,this->text+selectedStart,min(bufferLength,selectedEnd-selectedStart+1));
}

//设置键盘焦点
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

//设置标记选定的文本的颜色
void ZYEdit::SetMarkSelectedColor(COLORREF colorMarkSelected)
{
    this->colorMarkSelected=colorMarkSelected;
}

//标记选定的文本
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

//标记选定的文本
void ZYEdit::MarkSelectedText(IZYGraphics *graphics)
{
    int selectedStart1=min(selectedStart,selectedEnd);
    int selectedEnd1=max(selectedStart,selectedEnd);
    MarkSelectedText1(graphics,selectedStart1-i_base,selectedEnd1-i_base);
}

//处理控件重绘
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
            {       //英文变*
                buf[j]='*';
            }
            else    //中文变＊
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

//处理鼠标按下
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

//处理鼠标移动
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

//使文本开始小于文本结束
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

//处理鼠标弹起
void ZYEdit::OnMouseUp(int mx,int my)
{
    ZYControl::OnMouseUp(mx,my);

    stateSelect=false;

    ReleaseCapture();

    CheckSelectedStartEnd();
}

//处理键盘按下
void ZYEdit::OnKeyDown(int vkey)
{
    ZYControl::OnKeyDown(vkey);

    //处理不同的功能
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

    //更新插入符位置
    ShowCaret();

    Invalidate();
}

//处理字符输入
void ZYEdit::OnCharInput(char ch)
{
    //一般字符
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

//显示插入符
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

//隐藏插入符
void ZYEdit::HideCaret(void)
{
    if(ZYFrame::caretVisible==1)
    {
        DestroyCaret();

        ZYFrame::caretVisible=0;
    }
}

//0:英文,1:中文首字节,2:中文次字节
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

//是否中文字符
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

//递增字符索引
void ZYEdit::IncEditIndex(void)
{
    i++;
    
    if(i>i_base+n_display)
    {
        i_base++;
    }
}

//递减字符索引
void ZYEdit::DecEditIndex(void)
{
    i--;
    
    if(i<i_base)
    {
        i_base--;
    }
}

//字符索引回到开始
void ZYEdit::HomeEditIndex(void)
{
    i=0;
    
    i_base=0;

    selectedStart=0;
        
    selectedEnd=0;
}

//字符索引回到结束
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

//递减字符索引,用于回退字符
void ZYEdit::DecEditIndex2(void)
{
    i--;
    
    if(i_base>0)
    {
        i_base--;
    }
}

//插入字符到编辑字符串
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

//从编辑字符串删除字符
void ZYEdit::DeleteChar(int i,int c)
{
    for(;i+c<=n;i++)
    {
        text[i]=text[i+c];
    }
    
    n-=c;
    
    text[n]='\0';
}

//从编辑字符串删除字符,考虑了中文
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
        if(IsChineseChar(i))       //中文字符
        {
            DeleteChar(i,2);
        }
        else                       //英文字符
        {
            DeleteChar(i,1);
        }
    }
}

//在编辑字符串中回退字符,考虑了中文
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
        if(IsChineseChar(i-2))     //中文字符
        {
            DecEditIndex2();
    
            DecEditIndex2();
            
            DeleteChar(i,2);
        }
        else                       //英文字符
        {
            DecEditIndex2();
            
            DeleteChar(i,1);
        }
    }
}

//递减字符索引,考虑了中文
void ZYEdit::LeftKey(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //如果SHIFT未按下
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
        
    //如果SHIFT已按下
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
        if(IsChineseChar(i-2))     //中文字符
        {
            DecEditIndex();
            
            DecEditIndex();
        }
        else                       //英文字符
        {
            DecEditIndex();
        }
    }

    //如果SHIFT已按下
    if(shift==true)
    {
        selectedEnd=i;
    }
}

//递增字符索引,考虑了中文
void ZYEdit::RightKey(void)
{
    bool shift;
    shift=GetKeyState(VK_SHIFT)<0;

    //如果SHIFT未按下
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
    
    //如果SHIFT已按下
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
        if(IsChineseChar(i))       //中文字符
        {
            IncEditIndex();
            
            IncEditIndex();
        }
        else                       //英文字符
        {
            IncEditIndex();
        }
    }

    //如果SHIFT已按下
    if(shift==true)
    {
        selectedEnd=i;
    }
}