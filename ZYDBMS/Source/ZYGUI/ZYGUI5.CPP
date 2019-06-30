//---------------------------------------------------------
//���½�������(ZYGUI)-�������ؼ� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//�����������ؼ�����
IZYScrollBar *IZYScrollBar_Create(int i_base,int m_display,int m,EScrollType type)
{
    return (IZYScrollBar *)new ZYScrollBar(i_base,m_display,m,type);
}

//�ͷŹ������ؼ�����
void IZYScrollBar_Release(IZYScrollBar *scrollBar)
{
    delete (ZYScrollBar *)scrollBar;
}

//����������ؼ�����
ZYScrollBar::ZYScrollBar(int i_base,int m_display,int m,EScrollType type)
{
    this->i_base=i_base;
    this->m_display=m_display;
    this->m=m;
    this->type=type;
    this->h_scroll_1=0;
    this->h_scroll_2=0;
    this->h_scroll_3=0;
    this->h_scroll_4=0;
    this->m_scroll=0;
    this->state_scroll=EStateScroll_Start;
    this->scrollListener=NULL;
    this->barBitmap=NULL;
}

//ת��Ϊ�ؼ�����
IZYControl *ZYScrollBar::ToControl(void)
{
    return (IZYControl *)this;
}

//���ù�������
void ZYScrollBar::SetParam(int i_base,int m_display,int m)
{
    this->i_base=i_base;
    this->m_display=m_display;
    this->m=m;
}

//��ȡ����ƫ��
int ZYScrollBar::GetIndexBase(void)
{
    return this->i_base;
}

//��������ƫ��
void ZYScrollBar::SetIndexBase(int i_base)
{
    this->i_base=i_base;

    if(this->container!=NULL)
    {
        this->container->Invalidate();
    }
}

//���ù����¼���������
void ZYScrollBar::SetScrollListener(IZYScrollListener *scrollListener)
{
    this->scrollListener=scrollListener;
}

//���ù�����λͼͼƬ
void ZYScrollBar::SetBarBitmap(IZYBitmap *barBitmap)
{
    this->barBitmap=barBitmap;
}

//�����ؼ��ػ�
void ZYScrollBar::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);

    if(type==EScrollType_Vertical)
    {
        //�����Ϸ���ͷ
        this->DrawUpIcon(graphics,x,y,width);

        //�����·���ͷ
        this->DrawDownIcon(graphics,x,y+height-width,width);

        //���ƹ�����
        this->DrawScrollBarV(graphics,x,y+width,width,m,i_base,m_display);
    }
    else
    {
        //�����󷭼�ͷ
        this->DrawLeftIcon(graphics,x,y,height);

        //�����ҷ���ͷ
        this->DrawRightIcon(graphics,x+width-height,y,height);

        //���ƹ�����
        this->DrawScrollBarH(graphics,x+height,y,height,m,i_base,m_display);
    }
}

//�����Ϸ���ͷ
void ZYScrollBar::DrawUpIcon(IZYGraphics *graphics,int x,int y,int h)
{
    graphics->FillRectangle1(x,y,h,h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y,h,h);
    }

    graphics->DrawRectangle(x,y,h,h);

    graphics->DrawLine(x+h/6,y+5*h/6,x+h/2,y+h/6);

    graphics->DrawLine(x+h/2,y+h/6,x+h-h/6,y+5*h/6);
}

//�����·���ͷ
void ZYScrollBar::DrawDownIcon(IZYGraphics *graphics,int x,int y,int h)
{
    graphics->FillRectangle1(x,y,h,h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y,h,h);
    }

    graphics->DrawRectangle(x,y,h,h);

    graphics->DrawLine(x+h/6,y+h-5*h/6,x+h/2,y+h-h/6);

    graphics->DrawLine(x+h/2,y+h-h/6,x+h-h/6,y+h-5*h/6);
}

//�����󷭼�ͷ
void ZYScrollBar::DrawLeftIcon(IZYGraphics *graphics,int x,int y,int h)
{
    graphics->FillRectangle1(x,y,h,h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y,h,h);
    }

    graphics->DrawRectangle(x,y,h,h);

    graphics->DrawLine(x+5*h/6,y+5*h/6,x+h/6,y+h/2);

    graphics->DrawLine(x+h/6,y+h/2,x+5*h/6,y+h/6);
}

//�����ҷ���ͷ
void ZYScrollBar::DrawRightIcon(IZYGraphics *graphics,int x,int y,int h)
{
    graphics->FillRectangle1(x,y,h,h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y,h,h);
    }

    graphics->DrawRectangle(x,y,h,h);

    graphics->DrawLine(x+h/6,y+h/6,x+5*h/6,y+h/2);

    graphics->DrawLine(x+5*h/6,y+h/2,x+h/6,y+5*h/6);
}

//���ƹ�����
void ZYScrollBar::DrawScrollBarV(IZYGraphics *graphics,int x,int y,int w,int m,int i_base,int m_display)
{
    double h_scroll;

    double scroll_bar_len;
    
    double h_scroll_bar;

    //�ؼ�:�������������������ָ��i_base

    //��������������س���
    h_scroll=(double)(height-2*w);

    if(m_display<m)
    {
        //�����������������س���
        scroll_bar_len=h_scroll*(double)m_display/(double)m;

        //��������������س�����СҲҪ���ڿ���
        if(scroll_bar_len<20)
        {
            scroll_bar_len=w;
        }

        //��������������λ��

        //����������������ĵ�λ��
        h_scroll_bar=scroll_bar_len/2+(h_scroll-scroll_bar_len)*((double)i_base/(double)(m-m_display));

        //����������������±ߵ�λ��
        h_scroll_1=h_scroll_bar-scroll_bar_len/2;
    
        h_scroll_2=h_scroll_bar+scroll_bar_len/2+1;
    }
    else
    {
        h_scroll_bar=h_scroll/2;

        h_scroll_1=0;
        
        h_scroll_2=h_scroll;
    }

    //���ƹ�����
    barBitmap=barBitmap?barBitmap:bitmap;

    graphics->DrawBitmap(barBitmap,x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));

    graphics->FillRectangle1(x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1),0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));
    }

    graphics->DrawRectangle(x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));
}

//���ƹ�����
void ZYScrollBar::DrawScrollBarH(IZYGraphics *graphics,int x,int y,int h,int m,int i_base,int m_display)
{
    double h_scroll;

    double scroll_bar_len;
    
    double h_scroll_bar;

    //�ؼ�:�������������������ָ��i_base

    //��������������س���
    h_scroll=(double)(width-2*h);

    if(m_display<m)
    {
        //�����������������س���
        scroll_bar_len=h_scroll*(double)m_display/(double)m;

        //��������������س�����СҲҪ���ڿ���
        if(scroll_bar_len<20)
        {
            scroll_bar_len=h;
        }

        //��������������λ��

        //����������������ĵ�λ��
        h_scroll_bar=scroll_bar_len/2+(h_scroll-scroll_bar_len)*((double)i_base/(double)(m-m_display));

        //����������������±ߵ�λ��
        h_scroll_1=h_scroll_bar-scroll_bar_len/2;
    
        h_scroll_2=h_scroll_bar+scroll_bar_len/2+1;
    }
    else
    {
        h_scroll_bar=h_scroll/2;

        h_scroll_1=0;
        
        h_scroll_2=h_scroll;
    }

    //���ƹ�����
    barBitmap=barBitmap?barBitmap:bitmap;

    graphics->DrawBitmap(barBitmap,x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);

    graphics->FillRectangle1(x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);
    }

    graphics->DrawRectangle(x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);
}

//������갴��
void ZYScrollBar::OnMouseDown(int mx,int my)
{
    if(type==EScrollType_Vertical)
    {
        OnMouseDownV(mx,my);
    }
    else
    {
        OnMouseDownH(mx,my);
    }
}

//������갴��
void ZYScrollBar::OnMouseDownV(int mx,int my)
{
    ZYControl::OnMouseDown(mx,my);

    if(my<=y+width)
    {
        DecIndexBase();
    }
    else if(my>=y+height-width)
    {
        IncIndexBase();
    }
    else if(my>y+width&&my<y+width+(int)h_scroll_1)
    {
        DecIndexBasePage();
    }
    else if(my>y+width+(int)h_scroll_2&&my<y+height-width)
    {
        IncIndexBasePage();
    }
    else if(my>=y+width+(int)h_scroll_1&&my<=y+width+(int)h_scroll_2)
    {
        if(m>m_display)
        {
            state_scroll=EStateScroll_Scroll;

            SetCapture(frame->GetWindowHandle());

            h_scroll_3=h_scroll_1;
            h_scroll_4=h_scroll_2;

            m_scroll=(double)my;

            return;
        }
    }

    if(this->scrollListener!=NULL)
    {
        scrollListener->BarScroll(this,i_base);
    }

    if(this->container!=NULL)
    {
        this->container->Invalidate();
    }
}

//������갴��
void ZYScrollBar::OnMouseDownH(int mx,int my)
{
    ZYControl::OnMouseDown(mx,my);

    if(mx<=x+height)
    {
        DecIndexBase();
    }
    else if(mx>=x+width-height)
    {
        IncIndexBase();
    }
    else if(mx>x+height&&mx<x+height+(int)h_scroll_1)
    {
        DecIndexBasePage();
    }
    else if(mx>x+height+(int)h_scroll_2&&mx<x+width-height)
    {
        IncIndexBasePage();
    }
    else if(mx>=x+height+(int)h_scroll_1&&mx<=x+height+(int)h_scroll_2)
    {
        if(m>m_display)
        {
            state_scroll=EStateScroll_Scroll;

            SetCapture(frame->GetWindowHandle());

            h_scroll_3=h_scroll_1;
            h_scroll_4=h_scroll_2;

            m_scroll=(double)mx;

            return;
        }
    }

    if(this->scrollListener!=NULL)
    {
        scrollListener->BarScroll(this,i_base);
    }

    if(this->container!=NULL)
    {
        this->container->Invalidate();
    }
}

//�������˫��
void ZYScrollBar::OnMouseDoubleClick(int mx,int my)
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
    
//����������ֵ
void ZYScrollBar::DecIndexBase(void)
{
    //int m;
    
    //m=getItemCount(t[0]->table);

    if(m>0)
    {
        if(i_base>0)
        {   
            i_base--;
        }
    }
}

//����������ֵ
void ZYScrollBar::IncIndexBase(void)
{
    //int m;
    
    //m=getItemCount(t[0]->table);

    if(m>0)
    {
        if(i_base+m_display<m)
        {
            i_base++;
        }
    }
}

//��ҳ����������ֵ
void ZYScrollBar::DecIndexBasePage(void)
{
    int i;

    //int m;
    
    //m=getItemCount(t[0]->table);

    if(m>0)
    {
        for(i=0;i<m_display;i++)
        {
            if(i_base>0)
            {   
                i_base--;
            }
        }
    }
}

//��ҳ����������ֵ
void ZYScrollBar::IncIndexBasePage(void)
{
    int i;
    
    //int m;
    
    //m=getItemCount(t[0]->table);
    if(m>0)
    {
        for(i=0;i<m_display;i++)
        {
            if(i_base+m_display<m)
            {
                i_base++;
            }
        }
    }
}

//��������ƶ�
void ZYScrollBar::OnMouseMove(int mx,int my)
{
    if(type==EScrollType_Vertical)
    {
        OnMouseMoveV(mx,my);
    }
    else
    {
        OnMouseMoveH(mx,my);
    }
}

//��������ƶ�
void ZYScrollBar::OnMouseMoveV(int mx,int my)
{
    double f1,f2,f3,f4,h_scroll,scroll_bar_len;

    ZYControl::OnMouseMove(mx,my);

    if(state_scroll==EStateScroll_Scroll)
    {
        f1=h_scroll_3+(double)my-m_scroll;
        f2=h_scroll_4+(double)my-m_scroll;

        //����������
        f3=(double)(height-2*width);

        h_scroll=f3;

        //���������鳤��
        f4=h_scroll_4-h_scroll_3;

        scroll_bar_len=f4;

        if(f1>0&&f2<f3)
        {
            h_scroll_1=f1;
            h_scroll_2=f2;

            i_base=(int)(f1/(f3-f4)*(m-m_display));
        }
        else if(f1<=0)
        {
            h_scroll_1=0;
            h_scroll_2=f4;

            i_base=0;
        }
        else if(f2>=f3)
        {
            h_scroll_1=f3-f4;
            h_scroll_2=f3;

            i_base=m-m_display;
        }

        if(this->scrollListener!=NULL)
        {
            scrollListener->BarScroll(this,i_base);
        }

        if(this->container!=NULL)
        {
            this->container->Invalidate();
        }
    }
}

//��������ƶ�
void ZYScrollBar::OnMouseMoveH(int mx,int my)
{
    double f1,f2,f3,f4,h_scroll,scroll_bar_len;

    ZYControl::OnMouseMove(mx,my);

    if(state_scroll==EStateScroll_Scroll)
    {
        f1=h_scroll_3+(double)mx-m_scroll;
        f2=h_scroll_4+(double)mx-m_scroll;

        //����������
        f3=(double)(width-2*height);

        h_scroll=f3;

        //���������鳤��
        f4=h_scroll_4-h_scroll_3;

        scroll_bar_len=f4;

        if(f1>0&&f2<f3)
        {
            h_scroll_1=f1;
            h_scroll_2=f2;

            i_base=(int)(f1/(f3-f4)*(m-m_display));
        }
        else if(f1<=0)
        {
            h_scroll_1=0;
            h_scroll_2=f4;

            i_base=0;
        }
        else if(f2>=f3)
        {
            h_scroll_1=f3-f4;
            h_scroll_2=f3;

            i_base=m-m_display;
        }

        if(this->scrollListener!=NULL)
        {
            scrollListener->BarScroll(this,i_base);
        }

        if(this->container!=NULL)
        {
            this->container->Invalidate();
        }
    }
}

//������굯��
void ZYScrollBar::OnMouseUp(int mx,int my)
{
    ZYControl::OnMouseUp(mx,my);

    ReleaseCapture();

    if(state_scroll==EStateScroll_Scroll)
    {
        state_scroll=EStateScroll_Start;

        if(this->container!=NULL)
        {
            this->container->Invalidate();
        }
    }
}

//����������
void ZYScrollBar::OnMouseWheel(int mw)
{
    if(mw>0)
    {
        DecIndexBase();
    }
    else
    {
        IncIndexBase();
    }

    if(this->scrollListener!=NULL)
    {
        scrollListener->BarScroll(this,i_base);
    }

    if(this->container!=NULL)
    {
        this->container->Invalidate();
    }
}