//---------------------------------------------------------
//���½�������(ZYGUI)-ͼ�β��� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//����λͼͼƬ����
IZYBitmap *IZYBitmap_Create(int rid)
{
    return (IZYBitmap *)new ZYBitmap(rid);
}

//�ͷ�λͼͼƬ����
void IZYBitmap_Release(IZYBitmap *bitmap)
{
    delete (ZYBitmap *)bitmap;
}       

//����λͼͼƬ����
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

//��ȡλͼͼƬ�豸���
HDC ZYBitmap::GetHdcBitmap(void)
{
    return hdcBitmap;
}

//����ͼ�ζ���
IZYGraphics *IZYGraphics_Create(HDC hdc)
{
    return (IZYGraphics *)new ZYGraphics(hdc);
}

//�ͷ�ͼ�ζ���
void IZYGraphics_Release(IZYGraphics *graphics)
{
    delete (ZYGraphics *)graphics;
}

//����ͼ�ζ���
ZYGraphics::ZYGraphics(HDC hdc)
{
    this->hdc=hdc;
    this->hPen=INVALID_HANDLE_VALUE;
    this->x_base=0;
    this->y_base=0;
}

//��������ƫ��
void ZYGraphics::SetXYBase(int x_base,int y_base)
{
    this->x_base=x_base;
    this->y_base=y_base;
}

//����һ����
void ZYGraphics::DrawLine(int x1,int y1,int x2,int y2)
{
    MoveToEx(hdc,x_base+x1,y_base+y1,NULL);
    
    LineTo(hdc,x_base+x2,y_base+y2);
}

//����һ������
void ZYGraphics::DrawRectangle(int x,int y,int w,int h)
{
    DrawLine(x,y,x+w,y);
    
    DrawLine(x+w,y,x+w,y+h);
    
    DrawLine(x+w,y+h,x,y+h);
    
    DrawLine(x,y+h,x,y);
}

//������
void ZYGraphics::FillRectangle(int x,int y,int w,int h,int brush)
{
    SelectObject(hdc,GetStockObject(brush));
    
    Rectangle(hdc,x_base+x,y_base+y,x_base+x+w+1,y_base+y+h+1);
}

//������1
void ZYGraphics::FillRectangle1(int x,int y,int w,int h,COLORREF color)
{
    HANDLE hBrush=CreateSolidBrush(color);

    SelectObject(hdc,hBrush);
    
    Rectangle(hdc,x_base+x,y_base+y,x_base+x+w+1,y_base+y+h+1);

    DeleteObject(hBrush);

    SelectObject(hdc,GetStockObject(BLACK_BRUSH));
}

//����ı�
void ZYGraphics::OutputText(char *buf,int x,int y,int w,int h,int align)
{
    RECT rt;

    rt.left=x_base+x+3;

    rt.right=x_base+x+w-3;
    
    rt.top=y_base+y+3;
    
    rt.bottom=y_base+y+h-3;

    DrawText(hdc, buf, strlen(buf), &rt, align);
}

//���ͼ��
void ZYGraphics::DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h)
{
    if(bitmap!=NULL)
    {
        BitBlt(hdc,x_base+x,y_base+y,w,h,bitmap->GetHdcBitmap(),0,0,SRCCOPY);
    }
}

//�����ı���ɫ
void ZYGraphics::SetTextColor(COLORREF color)
{
    ::SetTextColor(hdc,color);
}

//���û�����ɫ
void ZYGraphics::SetPenColor(COLORREF color)
{
    hPen=CreatePen(PS_SOLID,1,color);
    
    SelectObject(hdc,hPen);
}

//���û�����ɫ
void ZYGraphics::ResetPenColor(void)
{
    SelectObject(hdc,GetStockObject(BLACK_PEN));

    DeleteObject(hPen);
}

//�������������
IZYClipboard *IZYClipboard_Create(IZYFrame *frame)
{
    return (IZYClipboard *)new ZYClipboard(frame);
}

//�ͷż��������
void IZYClipboard_Release(IZYClipboard *clipboard)
{
    delete (ZYClipboard *)clipboard;
}

//������������
ZYClipboard::ZYClipboard(IZYFrame *frame)
{
    hWnd=frame->GetWindowHandle();
}

//��ȡ�������е��ı�
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

//���ü������е��ı�
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
