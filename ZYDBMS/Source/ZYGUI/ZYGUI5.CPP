//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-滚动条控件 作者:蒋政
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//创建滚动条控件对象
IZYScrollBar *IZYScrollBar_Create(int i_base,int m_display,int m,EScrollType type)
{
    return (IZYScrollBar *)new ZYScrollBar(i_base,m_display,m,type);
}

//释放滚动条控件对象
void IZYScrollBar_Release(IZYScrollBar *scrollBar)
{
    delete (ZYScrollBar *)scrollBar;
}

//构造滚动条控件对象
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

//转换为控件对象
IZYControl *ZYScrollBar::ToControl(void)
{
    return (IZYControl *)this;
}

//设置滚动参数
void ZYScrollBar::SetParam(int i_base,int m_display,int m)
{
    this->i_base=i_base;
    this->m_display=m_display;
    this->m=m;
}

//获取索引偏移
int ZYScrollBar::GetIndexBase(void)
{
    return this->i_base;
}

//设置索引偏移
void ZYScrollBar::SetIndexBase(int i_base)
{
    this->i_base=i_base;

    if(this->container!=NULL)
    {
        this->container->Invalidate();
    }
}

//设置滚动事件监听对象
void ZYScrollBar::SetScrollListener(IZYScrollListener *scrollListener)
{
    this->scrollListener=scrollListener;
}

//设置滚动条位图图片
void ZYScrollBar::SetBarBitmap(IZYBitmap *barBitmap)
{
    this->barBitmap=barBitmap;
}

//处理控件重绘
void ZYScrollBar::OnPaint(IZYGraphics *graphics)
{
    ZYControl::OnPaint(graphics);

    if(type==EScrollType_Vertical)
    {
        //绘制上翻箭头
        this->DrawUpIcon(graphics,x,y,width);

        //绘制下翻箭头
        this->DrawDownIcon(graphics,x,y+height-width,width);

        //绘制滚动条
        this->DrawScrollBarV(graphics,x,y+width,width,m,i_base,m_display);
    }
    else
    {
        //绘制左翻箭头
        this->DrawLeftIcon(graphics,x,y,height);

        //绘制右翻箭头
        this->DrawRightIcon(graphics,x+width-height,y,height);

        //绘制滚动条
        this->DrawScrollBarH(graphics,x+height,y,height,m,i_base,m_display);
    }
}

//绘制上翻箭头
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

//绘制下翻箭头
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

//绘制左翻箭头
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

//绘制右翻箭头
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

//绘制滚动条
void ZYScrollBar::DrawScrollBarV(IZYGraphics *graphics,int x,int y,int w,int m,int i_base,int m_display)
{
    double h_scroll;

    double scroll_bar_len;
    
    double h_scroll_bar;

    //关键:滚动条滑块的中心总是指向i_base

    //计算滚动条的像素长度
    h_scroll=(double)(height-2*w);

    if(m_display<m)
    {
        //计算滚动条滑块的像素长度
        scroll_bar_len=h_scroll*(double)m_display/(double)m;

        //滚动条滑块的像素长度最小也要等于宽度
        if(scroll_bar_len<20)
        {
            scroll_bar_len=w;
        }

        //计算滚动条滑块的位置

        //计算滚动条滑块中心的位置
        h_scroll_bar=scroll_bar_len/2+(h_scroll-scroll_bar_len)*((double)i_base/(double)(m-m_display));

        //计算滚动条滑块上下边的位置
        h_scroll_1=h_scroll_bar-scroll_bar_len/2;
    
        h_scroll_2=h_scroll_bar+scroll_bar_len/2+1;
    }
    else
    {
        h_scroll_bar=h_scroll/2;

        h_scroll_1=0;
        
        h_scroll_2=h_scroll;
    }

    //绘制滚动条
    barBitmap=barBitmap?barBitmap:bitmap;

    graphics->DrawBitmap(barBitmap,x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));

    graphics->FillRectangle1(x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1),0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));
    }

    graphics->DrawRectangle(x,y+(int)h_scroll_1,w,(int)(h_scroll_2-h_scroll_1));
}

//绘制滚动条
void ZYScrollBar::DrawScrollBarH(IZYGraphics *graphics,int x,int y,int h,int m,int i_base,int m_display)
{
    double h_scroll;

    double scroll_bar_len;
    
    double h_scroll_bar;

    //关键:滚动条滑块的中心总是指向i_base

    //计算滚动条的像素长度
    h_scroll=(double)(width-2*h);

    if(m_display<m)
    {
        //计算滚动条滑块的像素长度
        scroll_bar_len=h_scroll*(double)m_display/(double)m;

        //滚动条滑块的像素长度最小也要等于宽度
        if(scroll_bar_len<20)
        {
            scroll_bar_len=h;
        }

        //计算滚动条滑块的位置

        //计算滚动条滑块中心的位置
        h_scroll_bar=scroll_bar_len/2+(h_scroll-scroll_bar_len)*((double)i_base/(double)(m-m_display));

        //计算滚动条滑块上下边的位置
        h_scroll_1=h_scroll_bar-scroll_bar_len/2;
    
        h_scroll_2=h_scroll_bar+scroll_bar_len/2+1;
    }
    else
    {
        h_scroll_bar=h_scroll/2;

        h_scroll_1=0;
        
        h_scroll_2=h_scroll;
    }

    //绘制滚动条
    barBitmap=barBitmap?barBitmap:bitmap;

    graphics->DrawBitmap(barBitmap,x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);

    graphics->FillRectangle1(x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h,0x00FFD2D2);

    if(barBitmap!=NULL)
    {
        graphics->DrawBitmap(barBitmap,x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);
    }

    graphics->DrawRectangle(x+(int)h_scroll_1,y,(int)(h_scroll_2-h_scroll_1),h);
}

//处理鼠标按下
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

//处理鼠标按下
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

//处理鼠标按下
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

//处理鼠标双击
void ZYScrollBar::OnMouseDoubleClick(int mx,int my)
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
    
//减少索引基值
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

//增加索引基值
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

//按页减少索引基值
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

//按页增加索引基值
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

//处理鼠标移动
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

//处理鼠标移动
void ZYScrollBar::OnMouseMoveV(int mx,int my)
{
    double f1,f2,f3,f4,h_scroll,scroll_bar_len;

    ZYControl::OnMouseMove(mx,my);

    if(state_scroll==EStateScroll_Scroll)
    {
        f1=h_scroll_3+(double)my-m_scroll;
        f2=h_scroll_4+(double)my-m_scroll;

        //滚动条长度
        f3=(double)(height-2*width);

        h_scroll=f3;

        //滚动条滑块长度
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

//处理鼠标移动
void ZYScrollBar::OnMouseMoveH(int mx,int my)
{
    double f1,f2,f3,f4,h_scroll,scroll_bar_len;

    ZYControl::OnMouseMove(mx,my);

    if(state_scroll==EStateScroll_Scroll)
    {
        f1=h_scroll_3+(double)mx-m_scroll;
        f2=h_scroll_4+(double)mx-m_scroll;

        //滚动条长度
        f3=(double)(width-2*height);

        h_scroll=f3;

        //滚动条滑块长度
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

//处理鼠标弹起
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

//处理鼠标滚动
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