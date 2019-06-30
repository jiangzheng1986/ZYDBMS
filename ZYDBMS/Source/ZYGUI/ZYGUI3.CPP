//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-基础控件 作者:蒋政
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//创建控件对象
IZYControl *IZYControl_Create(void)
{
    return (IZYControl *)new ZYControl();
}

//释放控件对象
void IZYControl_Release(IZYControl *control)
{
    delete (ZYControl *)control;
}

//构造控件对象
ZYControl::ZYControl(void)
{
    this->x=0;
    this->y=0;
    this->width=100;
    this->height=40;
    this->title=NULL;
    this->bitmap=NULL;
    this->frame=NULL;   
    this->mouseListener=NULL;
    this->keyListener=NULL;
    this->container=NULL;
    this->backColor=0x00FFFFFF;
}

//析构控件对象
ZYControl::~ZYControl(void)
{
    if(title!=NULL)
    {
        delete title;
    }
}

//设置控件位置尺寸
void ZYControl::SetLocSize(int x,int y,int width,int height)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
}

//获取控件位置尺寸
void ZYControl::GetLocSize(int *px,int *py,int *pwidth,int *pheight)
{
    (*px)=x;
    (*py)=y;
    (*pwidth)=width;
    (*pheight)=height;
}

//复制一个字符串,带字符串空间分配
char *ZYControl::CopyString(char *s)
{
    char *s1=new char[strlen(s)+1];
    strcpy(s1,s);
    return s1;
}

//获取控件标题文本
void ZYControl::SetTitle(char *title)
{
    this->title=CopyString(title);
}

//获取控件标题文本
char *ZYControl::GetTitle(void)
{
    return title;
}

//设置所在窗口
void ZYControl::SetFrame(IZYFrame *frame)
{
    this->frame=(ZYFrame *)frame;
}

//获取所在窗口
IZYFrame *ZYControl::GetFrame(void)
{
    return (IZYFrame *)frame;
}

//设置所在容器
void ZYControl::SetContainer(ZYContainer *container)
{
    this->container=container;
}

//设置位图图片
void ZYControl::SetBitmap(IZYBitmap *bitmap)
{
    this->bitmap=(ZYBitmap *)bitmap;
}

//设置背景颜色
void ZYControl::SetBackColor(COLORREF backColor)
{
    this->backColor=backColor;
}

//设置控件为键盘焦点控件
void ZYControl::SetFocus(bool focus)
{
    if(focus)
    {
        if(ZYFrame::focusControl!=NULL)
        {
            ZYFrame::focusControl->Invalidate();
            ZYFrame::focusControl->SetFocus(false);
        }
        ZYFrame::focusControl=this;
        ZYFrame::focusControl->Invalidate();
    }
    else
    {
        ZYFrame::focusControl=NULL;
    }
}

//刷新控件
void ZYControl::Invalidate(void)
{
    RECT rect;
    
    int x_base=container?container->GetXBase():0;

    int y_base=container?container->GetYBase():0;
    
    rect.left=x_base+x;

    rect.top=y_base+y;

    rect.right=x_base+x+width+1;

    rect.bottom=y_base+y+height+1;

    InvalidateRect(frame->GetWindowHandle(),&rect,FALSE);
}   

//处理控件重绘
void ZYControl::OnPaint(IZYGraphics *graphics)
{
    if(bitmap!=NULL)
    {
        graphics->DrawBitmap(bitmap,x,y,width,height);

        graphics->DrawRectangle(x,y,width,height);
    }
    else
    {
        graphics->FillRectangle1(x,y,width,height,backColor);
    }
}

//处理鼠标按下
void ZYControl::OnMouseDown(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseDown(this,mx,my);
    }
}

//处理鼠标移动
void ZYControl::OnMouseMove(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseMove(this,mx,my);
    }
}   

//处理鼠标弹起
void ZYControl::OnMouseUp(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseUp(this,mx,my);
    }
}   

//处理鼠标滚动
void ZYControl::OnMouseWheel(int mw)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseWheel(this,mw);
    }
}

//处理鼠标双击
void ZYControl::OnMouseDoubleClick(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseDoubleClick(this,mx,my);
    }
}

//处理键盘按下
void ZYControl::OnKeyDown(int vkey)
{
    if(keyListener!=NULL)
    {
        keyListener->KeyDown(this,vkey);
    }
}

//处理字符输入
void ZYControl::OnCharInput(char ch)
{
    if(keyListener!=NULL)
    {
        keyListener->CharInput(this,ch);
    }
}

//设置鼠标事件监听对象
void ZYControl::SetMouseListener(IZYMouseListener *mouseListener)
{
    this->mouseListener=mouseListener;
}

//设置键盘事件监听对象
void ZYControl::SetKeyListener(IZYKeyListener *keyListener)
{
    this->keyListener=keyListener;
}

//构造容器对象
ZYContainer::ZYContainer(void)
{
    this->controls=new vector<ZYControl *>();
}

//构造容器对象
ZYContainer::~ZYContainer(void)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        if((*it)!=NULL)
        {
            delete (*it);
        }
    }

    delete controls;
}

//获取X偏移
int ZYContainer::GetXBase(void)
{
    if(this->container!=NULL)
    {
        return this->container->GetXBase()+x;
    }
    else
    {
        return x;
    }
}

//获取Y偏移
int ZYContainer::GetYBase(void)
{
    if(this->container!=NULL)
    {
        return this->container->GetYBase()+y;
    }
    else
    {
        return y;
    }
}

//处理容器重绘
void ZYContainer::OnPaint(IZYGraphics *graphics)
{
    vector<ZYControl *>::iterator it;

    graphics->SetXYBase(GetXBase(),GetYBase());

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnPaint(graphics);
    }

    if(container==NULL)
    {
        graphics->SetXYBase(0,0);
    }
}

//处理鼠标按下
void ZYContainer::OnMouseDown(int mx,int my)
{
    vector<ZYControl *>::reverse_iterator it;

    int x,y,width,height;

    ZYControl::OnMouseDown(mx,my);

    for(it=controls->rbegin();it<controls->rend();it++)
    {
        (*it)->GetLocSize(&x,&y,&width,&height);

        if(mx>GetXBase()+x&&mx<GetXBase()+x+width&&my>GetYBase()+y&&my<GetYBase()+y+height)
        {
            (*it)->OnMouseDown(mx-GetXBase(),my-GetYBase());

            return;
        }
    }
}

//处理鼠标双击
void ZYContainer::OnMouseDoubleClick(int mx,int my)
{
    vector<ZYControl *>::reverse_iterator it;

    int x,y,width,height;

    ZYControl::OnMouseDoubleClick(mx,my);

    for(it=controls->rbegin();it<controls->rend();it++)
    {
        (*it)->GetLocSize(&x,&y,&width,&height);

        if(mx>GetXBase()+x&&mx<GetXBase()+x+width&&my>GetYBase()+y&&my<GetYBase()+y+height)
        {
            (*it)->OnMouseDoubleClick(mx-GetXBase(),my-GetYBase());

            return;
        }
    }
}

//处理鼠标移动
void ZYContainer::OnMouseMove(int mx,int my)
{
    vector<ZYControl *>::iterator it;

    ZYControl::OnMouseMove(mx,my);

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseMove(mx-GetXBase(),my-GetYBase());
    }
}

//处理鼠标弹起
void ZYContainer::OnMouseUp(int mx,int my)
{
    vector<ZYControl *>::iterator it;

    ZYControl::OnMouseUp(mx,my);

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseUp(mx-GetXBase(),my-GetYBase());
    }
}

//增加子控件
void ZYContainer::AddControl(IZYControl *control)
{
    this->controls->push_back((ZYControl *)control);
    ((ZYControl *)control)->SetContainer(this);
    ((ZYControl *)control)->SetFrame(this->frame);
}

//删除控件
void ZYContainer::RemoveControl(IZYControl *control)
{
    vector<ZYControl *>::iterator it;
    
    for(it=controls->begin();it<controls->end();it++)
    {
        if((*it)==(ZYControl *)control)
        {
            (*it)->SetFocus(false);

            delete (*it);
            
            controls->erase(it);

            break;
        }
    }
}

//设置所在窗口
void ZYContainer::SetFrame(IZYFrame *frame)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->SetFrame(frame);
    }

    this->frame=(ZYFrame *)frame;
}

//设置所在窗口
void ZYContainer::SetBitmap(IZYBitmap *bitmap)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->SetBitmap(bitmap);
    }

    this->bitmap=(ZYBitmap *)bitmap;
}

//创建静态文本控件对象
IZYStaticText *IZYStaticText_Create(char *title)
{
    return (IZYStaticText *)new ZYStaticText(title);
}

//释放静态文本控件对象
void IZYStaticText_Release(IZYStaticText *staticText)
{
    delete (ZYStaticText *)staticText;
}

//构造静态文本控件对象
ZYStaticText::ZYStaticText(char *title)
{
    SetTitle(title);
}

//转换为控件对象
IZYControl *ZYStaticText::ToControl(void)
{
    return (IZYControl *)this;
}

//绘制静态文本
void ZYStaticText::OnPaint(IZYGraphics *graphics)
{
    graphics->OutputText(title,x,y,width,height,DT_LEFT);
}

//创建静态图片控件对象
IZYStaticPicture *IZYStaticPicture_Create(IZYBitmap *bitmap)
{
    return (IZYStaticPicture *)new ZYStaticPicture((ZYBitmap *)bitmap);
}

//释放静态图片控件对象
void IZYStaticPicture_Release(IZYStaticPicture *staticPicture)
{
    delete (ZYStaticPicture *)staticPicture;
}

//构造静态图片控件对象
ZYStaticPicture::ZYStaticPicture(ZYBitmap *bitmap)
{
    this->bitmap=bitmap;
}

//转换为控件对象
IZYControl *ZYStaticPicture::ToControl(void)
{
    return (IZYControl *)this;
}

//绘制静态图片
void ZYStaticPicture::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);
}

//创建按钮控件对象
IZYButton *IZYButton_Create(char *title)
{
    return (IZYButton *)new ZYButton(title);
}

//释放按钮控件对象
void IZYButton_Release(IZYButton *button)
{
    delete (ZYButton *)button;
}

//构造按钮控件对象
ZYButton::ZYButton(char *title)
{
    SetTitle(title);
    this->actionListener=NULL;
}

//转换为控件对象
IZYControl *ZYButton::ToControl(void)
{
    return (IZYControl *)this;
}

//设置按钮动作监听对象
void ZYButton::SetActionListener(IZYActionListener *actionListener)
{
    this->actionListener=actionListener;
}

//处理控件重绘
void ZYButton::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);

    graphics->OutputText(title,x,y,width,height,DT_CENTER);
}

//处理鼠标按下
void ZYButton::OnMouseDown(int mx,int my)
{
    ZYControl::OnMouseDown(mx,my);

    if(this->actionListener!=NULL)
    {
        actionListener->ActionPerformed(this);
    }

    Invalidate();
}

//处理鼠标双击
void ZYButton::OnMouseDoubleClick(int mx,int my)
{
    if(this->actionListener!=NULL)
    {
        actionListener->ActionPerformed(this);
    }

    Invalidate();
}

//创建单选框控件对象
IZYCheckBox *IZYCheckBox_Create(bool check)
{
    return (IZYCheckBox *)new ZYCheckBox(check);
}

//释放单选框控件对象
void IZYCheckBox_Release(IZYCheckBox *checkBox)
{
    delete (ZYCheckBox *)checkBox;
}

//构造单选框控件对象
ZYCheckBox::ZYCheckBox(bool check)
{
    this->check=check;
}

//转换为控件对象
IZYControl *ZYCheckBox::ToControl(void)
{
    return (IZYControl *)this;
}

//获取选择状态
bool ZYCheckBox::GetCheckState(void)
{
    return this->check;
}

//处理控件重绘
void ZYCheckBox::OnPaint(IZYGraphics *graphics)
{
    int h;
    
    h=width<height?width:height;

    ZYControl::OnPaint(graphics);

    if(check==true)
    {
        graphics->DrawLine(x+h/4,y+h/4,x+h/2,y+h-h/5);
        graphics->DrawLine(x+h/2,y+h-h/5,x+h-h/6,y+h/6);            
    }
}

//处理鼠标按下
void ZYCheckBox::OnMouseDown(int mx,int my)
{
    this->check^=true;

    Invalidate();
}

//处理鼠标双击
void ZYCheckBox::OnMouseDoubleClick(int mx,int my)
{
    HWND hWnd;
    WPARAM wParam;
    LPARAM lParam;

    //模拟一次鼠标单击
    
    hWnd=frame->GetWindowHandle();

    if(this->container!=NULL)
    {
        mx+=this->container->GetXBase();
        my+=this->container->GetYBase();
    }

    wParam=0;

    lParam=MAKELONG(mx,my);

    PostMessage(hWnd,WM_LBUTTONDOWN,wParam,lParam);

    Invalidate();
}