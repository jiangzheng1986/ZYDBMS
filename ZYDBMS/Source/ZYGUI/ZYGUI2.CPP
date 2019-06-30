//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-图形操作 作者:蒋政
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//创建位图图片对象
IZYBitmap *IZYBitmap_Create(int rid)
{
    return (IZYBitmap *)new ZYBitmap(rid);
}

//释放位图图片对象
void IZYBitmap_Release(IZYBitmap *bitmap)
{
    delete (ZYBitmap *)bitmap;
}       

//构造位图图片对象
ZYBitmap::ZYBitmap(int rid)
{
    static HDC hDC=(HDC)INVALID_HANDLE_VALUE;

    HINSTANCE hInstance=GetModuleHandle(NULL);

    if(hDC==INVALID_HANDLE_VALUE)
    {
        hDC=CreateDC("DISPLAY",(LPCSTR)NULL,(LPCSTR)NULL,(CONST DEVMODE*)NULL);
    }

    hBitmap=LoadBitmap(hInstance,(LPCSTR)rid);

    hdcBitmap=CreateCompatibleDC(hDC);

    SelectObject(hdcBitmap,hBitmap);
}

//获取位图图片设备句柄
HDC ZYBitmap::GetHdcBitmap(void)
{
    return hdcBitmap;
}

//创建图形对象
IZYGraphics *IZYGraphics_Create(HDC hdc)
{
    return (IZYGraphics *)new ZYGraphics(hdc);
}

//释放图形对象
void IZYGraphics_Release(IZYGraphics *graphics)
{
    delete (ZYGraphics *)graphics;
}

//构造图形对象
ZYGraphics::ZYGraphics(HDC hdc)
{
    this->hdc=hdc;
    this->hPen=INVALID_HANDLE_VALUE;
    this->x_base=0;
    this->y_base=0;
}

//设置坐标偏移
void ZYGraphics::SetXYBase(int x_base,int y_base)
{
    this->x_base=x_base;
    this->y_base=y_base;
}

//绘制一条线
void ZYGraphics::DrawLine(int x1,int y1,int x2,int y2)
{
    MoveToEx(hdc,x_base+x1,y_base+y1,NULL);
    
    LineTo(hdc,x_base+x2,y_base+y2);
}

//绘制一个矩形
void ZYGraphics::DrawRectangle(int x,int y,int w,int h)
{
    DrawLine(x,y,x+w,y);
    
    DrawLine(x+w,y,x+w,y+h);
    
    DrawLine(x+w,y+h,x,y+h);
    
    DrawLine(x,y+h,x,y);
}

//填充矩形
void ZYGraphics::FillRectangle(int x,int y,int w,int h,int brush)
{
    SelectObject(hdc,GetStockObject(brush));
    
    Rectangle(hdc,x_base+x,y_base+y,x_base+x+w+1,y_base+y+h+1);
}

//填充矩形1
void ZYGraphics::FillRectangle1(int x,int y,int w,int h,COLORREF color)
{
    HANDLE hBrush=CreateSolidBrush(color);

    SelectObject(hdc,hBrush);
    
    Rectangle(hdc,x_base+x,y_base+y,x_base+x+w+1,y_base+y+h+1);

    DeleteObject(hBrush);

    SelectObject(hdc,GetStockObject(BLACK_BRUSH));
}

//输出文本
void ZYGraphics::OutputText(char *buf,int x,int y,int w,int h,int align)
{
    RECT rt;

    rt.left=x_base+x+3;

    rt.right=x_base+x+w-3;
    
    rt.top=y_base+y+3;
    
    rt.bottom=y_base+y+h-3;

    DrawText(hdc, buf, strlen(buf), &rt, align);
}

//输出图象
void ZYGraphics::DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h)
{
    if(bitmap!=NULL)
    {
        BitBlt(hdc,x_base+x,y_base+y,w,h,bitmap->GetHdcBitmap(),0,0,SRCCOPY);
    }
}

//设置文本颜色
void ZYGraphics::SetTextColor(COLORREF color)
{
    ::SetTextColor(hdc,color);
}

//设置画笔颜色
void ZYGraphics::SetPenColor(COLORREF color)
{
    hPen=CreatePen(PS_SOLID,1,color);
    
    SelectObject(hdc,hPen);
}

//重置画笔颜色
void ZYGraphics::ResetPenColor(void)
{
    SelectObject(hdc,GetStockObject(BLACK_PEN));

    DeleteObject(hPen);
}

//创建剪贴板对象
IZYClipboard *IZYClipboard_Create(IZYFrame *frame)
{
    return (IZYClipboard *)new ZYClipboard(frame);
}

//释放剪贴板对象
void IZYClipboard_Release(IZYClipboard *clipboard)
{
    delete (ZYClipboard *)clipboard;
}

//构造剪贴板对象
ZYClipboard::ZYClipboard(IZYFrame *frame)
{
    hWnd=frame->GetWindowHandle();
}

//获取剪贴板中的文本
char *ZYClipboard::PasteText(void)
{
    int b;

    int k1;
    
    int k2;

    HGLOBAL h1;

    HGLOBAL h2;

    char *lp1;

    char *lp2;

    b=OpenClipboard(hWnd);

    h1=GetClipboardData(CF_TEXT);

    lp1=(char *)GlobalLock(h1);

    b=lstrlen(lp1);

    h2=GlobalAlloc(0,b);

    lp2=(char *)GlobalLock(h2);

    for(k1=0,k2=0;k1<b;k1++)
    {
        if(lp1[k1]!='\x0d')
        {
            lp2[k2++]=lp1[k1];
        }
    }   

    lp2[k2]='\0';

    GlobalUnlock(h1);

    b=CloseClipboard();

    return lp2;
}

//设置剪贴板中的文本
void ZYClipboard::CopyText(char *text)
{
    int b;

    int k1;
    
    int k2;
    
    HGLOBAL h;
    
    char *lp;

    b=OpenClipboard(hWnd);

    b=EmptyClipboard();

    b=lstrlen(text);

    h=GlobalAlloc(0,2*b+1);

    lp=(char *)GlobalLock(h);

    for(k1=0,k2=0;k1<b;k1++)
    {
        if(text[k1]=='\n')
        {
            lp[k2++]='\x0d';
            lp[k2++]='\x0a';
        }
        else
        {
            lp[k2++]=text[k1];
        }
    }   

    lp[k2]='\0';

    GlobalUnlock(h);

    h=SetClipboardData(CF_TEXT,h);

    b=CloseClipboard();
}

