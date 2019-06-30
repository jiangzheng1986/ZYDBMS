//---------------------------------------------------------
//���½�������(ZYGUI)-�����ؼ� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//�����ؼ�����
IZYControl *IZYControl_Create(void)
{
    return (IZYControl *)new ZYControl();
}

//�ͷſؼ�����
void IZYControl_Release(IZYControl *control)
{
    delete (ZYControl *)control;
}

//����ؼ�����
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

//�����ؼ�����
ZYControl::~ZYControl(void)
{
    if(title!=NULL)
    {
        delete title;
    }
}

//���ÿؼ�λ�óߴ�
void ZYControl::SetLocSize(int x,int y,int width,int height)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
}

//��ȡ�ؼ�λ�óߴ�
void ZYControl::GetLocSize(int *px,int *py,int *pwidth,int *pheight)
{
    (*px)=x;
    (*py)=y;
    (*pwidth)=width;
    (*pheight)=height;
}

//����һ���ַ���,���ַ����ռ����
char *ZYControl::CopyString(char *s)
{
    char *s1=new char[strlen(s)+1];
    strcpy(s1,s);
    return s1;
}

//��ȡ�ؼ������ı�
void ZYControl::SetTitle(char *title)
{
    this->title=CopyString(title);
}

//��ȡ�ؼ������ı�
char *ZYControl::GetTitle(void)
{
    return title;
}

//�������ڴ���
void ZYControl::SetFrame(IZYFrame *frame)
{
    this->frame=(ZYFrame *)frame;
}

//��ȡ���ڴ���
IZYFrame *ZYControl::GetFrame(void)
{
    return (IZYFrame *)frame;
}

//������������
void ZYControl::SetContainer(ZYContainer *container)
{
    this->container=container;
}

//����λͼͼƬ
void ZYControl::SetBitmap(IZYBitmap *bitmap)
{
    this->bitmap=(ZYBitmap *)bitmap;
}

//���ñ�����ɫ
void ZYControl::SetBackColor(COLORREF backColor)
{
    this->backColor=backColor;
}

//���ÿؼ�Ϊ���̽���ؼ�
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

//ˢ�¿ؼ�
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

//�����ؼ��ػ�
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

//������갴��
void ZYControl::OnMouseDown(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseDown(this,mx,my);
    }
}

//��������ƶ�
void ZYControl::OnMouseMove(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseMove(this,mx,my);
    }
}   

//������굯��
void ZYControl::OnMouseUp(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseUp(this,mx,my);
    }
}   

//����������
void ZYControl::OnMouseWheel(int mw)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseWheel(this,mw);
    }
}

//�������˫��
void ZYControl::OnMouseDoubleClick(int mx,int my)
{
    if(mouseListener!=NULL)
    {
        mouseListener->MouseDoubleClick(this,mx,my);
    }
}

//�������̰���
void ZYControl::OnKeyDown(int vkey)
{
    if(keyListener!=NULL)
    {
        keyListener->KeyDown(this,vkey);
    }
}

//�����ַ�����
void ZYControl::OnCharInput(char ch)
{
    if(keyListener!=NULL)
    {
        keyListener->CharInput(this,ch);
    }
}

//��������¼���������
void ZYControl::SetMouseListener(IZYMouseListener *mouseListener)
{
    this->mouseListener=mouseListener;
}

//���ü����¼���������
void ZYControl::SetKeyListener(IZYKeyListener *keyListener)
{
    this->keyListener=keyListener;
}

//������������
ZYContainer::ZYContainer(void)
{
    this->controls=new vector<ZYControl *>();
}

//������������
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

//��ȡXƫ��
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

//��ȡYƫ��
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

//���������ػ�
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

//������갴��
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

//�������˫��
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

//��������ƶ�
void ZYContainer::OnMouseMove(int mx,int my)
{
    vector<ZYControl *>::iterator it;

    ZYControl::OnMouseMove(mx,my);

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseMove(mx-GetXBase(),my-GetYBase());
    }
}

//������굯��
void ZYContainer::OnMouseUp(int mx,int my)
{
    vector<ZYControl *>::iterator it;

    ZYControl::OnMouseUp(mx,my);

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseUp(mx-GetXBase(),my-GetYBase());
    }
}

//�����ӿؼ�
void ZYContainer::AddControl(IZYControl *control)
{
    this->controls->push_back((ZYControl *)control);
    ((ZYControl *)control)->SetContainer(this);
    ((ZYControl *)control)->SetFrame(this->frame);
}

//ɾ���ؼ�
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

//�������ڴ���
void ZYContainer::SetFrame(IZYFrame *frame)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->SetFrame(frame);
    }

    this->frame=(ZYFrame *)frame;
}

//�������ڴ���
void ZYContainer::SetBitmap(IZYBitmap *bitmap)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->SetBitmap(bitmap);
    }

    this->bitmap=(ZYBitmap *)bitmap;
}

//������̬�ı��ؼ�����
IZYStaticText *IZYStaticText_Create(char *title)
{
    return (IZYStaticText *)new ZYStaticText(title);
}

//�ͷž�̬�ı��ؼ�����
void IZYStaticText_Release(IZYStaticText *staticText)
{
    delete (ZYStaticText *)staticText;
}

//���쾲̬�ı��ؼ�����
ZYStaticText::ZYStaticText(char *title)
{
    SetTitle(title);
}

//ת��Ϊ�ؼ�����
IZYControl *ZYStaticText::ToControl(void)
{
    return (IZYControl *)this;
}

//���ƾ�̬�ı�
void ZYStaticText::OnPaint(IZYGraphics *graphics)
{
    graphics->OutputText(title,x,y,width,height,DT_LEFT);
}

//������̬ͼƬ�ؼ�����
IZYStaticPicture *IZYStaticPicture_Create(IZYBitmap *bitmap)
{
    return (IZYStaticPicture *)new ZYStaticPicture((ZYBitmap *)bitmap);
}

//�ͷž�̬ͼƬ�ؼ�����
void IZYStaticPicture_Release(IZYStaticPicture *staticPicture)
{
    delete (ZYStaticPicture *)staticPicture;
}

//���쾲̬ͼƬ�ؼ�����
ZYStaticPicture::ZYStaticPicture(ZYBitmap *bitmap)
{
    this->bitmap=bitmap;
}

//ת��Ϊ�ؼ�����
IZYControl *ZYStaticPicture::ToControl(void)
{
    return (IZYControl *)this;
}

//���ƾ�̬ͼƬ
void ZYStaticPicture::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);
}

//������ť�ؼ�����
IZYButton *IZYButton_Create(char *title)
{
    return (IZYButton *)new ZYButton(title);
}

//�ͷŰ�ť�ؼ�����
void IZYButton_Release(IZYButton *button)
{
    delete (ZYButton *)button;
}

//���찴ť�ؼ�����
ZYButton::ZYButton(char *title)
{
    SetTitle(title);
    this->actionListener=NULL;
}

//ת��Ϊ�ؼ�����
IZYControl *ZYButton::ToControl(void)
{
    return (IZYControl *)this;
}

//���ð�ť������������
void ZYButton::SetActionListener(IZYActionListener *actionListener)
{
    this->actionListener=actionListener;
}

//�����ؼ��ػ�
void ZYButton::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);

    graphics->OutputText(title,x,y,width,height,DT_CENTER);
}

//������갴��
void ZYButton::OnMouseDown(int mx,int my)
{
    ZYControl::OnMouseDown(mx,my);

    if(this->actionListener!=NULL)
    {
        actionListener->ActionPerformed(this);
    }

    Invalidate();
}

//�������˫��
void ZYButton::OnMouseDoubleClick(int mx,int my)
{
    if(this->actionListener!=NULL)
    {
        actionListener->ActionPerformed(this);
    }

    Invalidate();
}

//������ѡ��ؼ�����
IZYCheckBox *IZYCheckBox_Create(bool check)
{
    return (IZYCheckBox *)new ZYCheckBox(check);
}

//�ͷŵ�ѡ��ؼ�����
void IZYCheckBox_Release(IZYCheckBox *checkBox)
{
    delete (ZYCheckBox *)checkBox;
}

//���쵥ѡ��ؼ�����
ZYCheckBox::ZYCheckBox(bool check)
{
    this->check=check;
}

//ת��Ϊ�ؼ�����
IZYControl *ZYCheckBox::ToControl(void)
{
    return (IZYControl *)this;
}

//��ȡѡ��״̬
bool ZYCheckBox::GetCheckState(void)
{
    return this->check;
}

//�����ؼ��ػ�
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

//������갴��
void ZYCheckBox::OnMouseDown(int mx,int my)
{
    this->check^=true;

    Invalidate();
}

//�������˫��
void ZYCheckBox::OnMouseDoubleClick(int mx,int my)
{
    HWND hWnd;
    WPARAM wParam;
    LPARAM lParam;

    //ģ��һ����굥��
    
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