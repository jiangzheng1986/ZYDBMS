//---------------------------------------------------------
//���½�������(ZYGUI)-ϵͳ��� ����:����
//---------------------------------------------------------
#include "resource.h"

#include "ZYGUI1.HPP"

//����������
#define ZYWNDCLS "ZYWNDCLS"

//���ڶ���
vector<ZYFrame *> *ZYFrame::frames=NULL;

//���̽���ؼ�
ZYControl *ZYFrame::focusControl=NULL;

//���������״̬
bool ZYFrame::caretVisible=false;

//�������ڶ���
IZYFrame *IZYFrame_Create(int x,int y,int width,int height,char *title)
{
    return (IZYFrame *)new ZYFrame(x,y,width,height,title);
}

//�ͷŴ��ڶ���
void IZYFrame_Release(IZYFrame *frame)
{
    delete (ZYFrame *)frame;
}

//���촰�ڶ���
ZYFrame::ZYFrame(int x,int y,int width,int height,char *title)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->title=title;
    this->bitmap=NULL;
    this->backColor=0x00FFFFFF;
    this->controls=new vector<ZYControl *>();
    this->hDC=(HDC)INVALID_HANDLE_VALUE;
    this->hBitmapBuffer=(HBITMAP)INVALID_HANDLE_VALUE;
    this->hDCBitmapBuffer=(HDC)INVALID_HANDLE_VALUE;
    this->hWnd=(HWND)INVALID_HANDLE_VALUE;
    this->menuListener=NULL;
    this->frameListener=NULL;
    this->mouseListener=NULL;
    this->networkListener=NULL;
    this->mainWindow=false;
    this->closed=false;
    this->openGLWindow=false;
    this->frameListener1=NULL;
    if(frames==NULL)
    {
        frames=new vector<ZYFrame *>();
        RegisterWindowClass();
    }
    frames->push_back(this);
    InitWindow();   
}

//�������ڶ���
ZYFrame::~ZYFrame(void)
{
    vector<ZYControl *>::iterator it;

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->SetFocus(false);
        delete (*it);
    }

    delete controls;

    DeleteObject(hDCBitmapBuffer);

    DeleteObject(hBitmapBuffer);    

    vector<ZYFrame *>::iterator it1;

    for(it1=frames->begin();it1<frames->end();it1++)
    {
        if((*it1)==this)
        {
            frames->erase(it1);

            break;
        }
    }
}

//���ز˵�
HMENU ZYFrame::LoadMenu(int rid)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HMENU hMenu=::LoadMenu(hInstance,(LPSTR)rid);
    return hMenu;
}

//�������˵�
void ZYFrame::SetMainMenu(HMENU hMenu)
{
    SetMenu(hWnd,hMenu);
}

//���ӿؼ�
void ZYFrame::AddControl(IZYControl *control)
{
    controls->push_back((ZYControl *)control);
    control->SetFrame(this);
}

//ɾ���ؼ�
void ZYFrame::RemoveControl(IZYControl *control)
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

//����λͼͼƬ
void ZYFrame::SetBitmap(IZYBitmap *bitmap)
{
    this->bitmap=(ZYBitmap *)bitmap;
}

//���ñ�����ɫ
void ZYFrame::SetBackColor(COLORREF backColor)
{
    this->backColor=backColor;
}

//��ȡ���ھ��
HWND ZYFrame::GetWindowHandle(void)
{
    return hWnd;
}

//ˢ�´���
void ZYFrame::Invalidate(void)
{
    RECT rect;

    GetClientRect(hWnd,&rect);
    
    InvalidateRect(hWnd,&rect,FALSE);
}

//�رմ���
void ZYFrame::Close(void)
{
    if(GetClosed()==false)
    {
        OnWmClose(hWnd,0,0);

        DestroyWindow(GetWindowHandle());
    }
}

//���ڹ���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hWnd1;
    vector<ZYFrame *>::iterator it;

    for(it=ZYFrame::frames->begin();it<ZYFrame::frames->end();it++)
    {
        hWnd1=(*it)->GetWindowHandle();
        if(hWnd1==(HWND)INVALID_HANDLE_VALUE||hWnd==hWnd1)
        {
            return (*it)->MsgProc(hWnd,message,wParam,lParam);
        }
    }

    return 0;
}

//ע�ᴰ����
ATOM ZYFrame::RegisterWindowClass(void)
{
    WNDCLASSEX wcex;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS;
    wcex.lpfnWndProc    = (WNDPROC)WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_ZYGUI);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = (LPCSTR)NULL;
    wcex.lpszClassName  = ZYWNDCLS;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

    return RegisterClassEx(&wcex);
}

//��ʼ������
void ZYFrame::InitWindow(void)
{
   HINSTANCE hInstance=GetModuleHandle(NULL);

   hWnd = CreateWindow(ZYWNDCLS, title, WS_OVERLAPPEDWINDOW,
      x, y, width, height, NULL, NULL, hInstance, NULL);

   EnableDoubleBuffer();

   if (!hWnd)
   {
      return;
   }

   ShowWindow(hWnd, SW_SHOW);
   
   Invalidate();
}

//����˫����
void ZYFrame::EnableDoubleBuffer(void)
{
    if(hDC==INVALID_HANDLE_VALUE)
    {
        hDC=CreateDC("DISPLAY",(LPCSTR)NULL,(LPCSTR)NULL,(CONST DEVMODE*)NULL);
    }

    if(hDCBitmapBuffer==INVALID_HANDLE_VALUE)
    {
        hDCBitmapBuffer=CreateCompatibleDC(hDC);

        hBitmapBuffer=CreateCompatibleBitmap(hDC,1024,768);

        SelectObject(hDCBitmapBuffer,hBitmapBuffer);
    }
}

//������Ϣ����
LRESULT ZYFrame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(this->frameListener1)
    {
        frameListener1->OnWindowsMessage(hWnd,message,wParam,lParam);
    }

    switch (message) 
    {
    case WM_MOUSEMOVE:
        OnWmMouseMove(hWnd,wParam,lParam);
        break;
    case WM_PAINT:
        OnWmPaint(hWnd,wParam,lParam);
        break;
    case WM_COMMAND:
        OnWmCommand(hWnd,wParam,lParam);
        break;
    case WM_LBUTTONDOWN:
        OnWmLButtonDown(hWnd,wParam,lParam);
        break;
    case WM_LBUTTONUP:
        OnWmLButtonUp(hWnd,wParam,lParam);
        break;
    case WM_LBUTTONDBLCLK:
        OnWmLButtonDblClk(hWnd,wParam,lParam);
        break;
    case WM_KEYDOWN:
        OnWmKeyDown(hWnd,wParam,lParam);
        break;
    case WM_CHAR:
        OnWmChar(hWnd,wParam,lParam);
        break;
    case 0x020a://WM_MOUSEWHEEL
        OnWmMouseWheel(hWnd,wParam,lParam);
        break;
    case WM_DESTROY:
        OnWmDestroy(hWnd,wParam,lParam);
        break;
    case WM_NETWORK_EVENT:
        OnWmNetworkEvent(hWnd,wParam,lParam);
        break;
    case WM_CLOSE:
        OnWmClose(hWnd,wParam,lParam);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    } 
    return 0;
}

//������Ϣѭ��
int ZYFrame::MsgLoop(void)
{
    MSG msg;

    HINSTANCE hInstance=GetModuleHandle(NULL);

    HACCEL hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ZYGUI);

    this->mainWindow=true;

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

//�˵�����
void ZYFrame::OnMenuCommand(int mcid)
{
    if(menuListener!=NULL)
    {
        menuListener->MenuCommand(this,mcid);
    }
}

//���ò˵��¼���������
void ZYFrame::SetMenuListener(IZYMenuListener *menuListener)
{
    this->menuListener=menuListener;
}

//���ô����¼���������
void ZYFrame::SetFrameListener(IZYFrameListener *frameListener)
{
    this->frameListener=frameListener;
}

//��������¼���������
void ZYFrame::SetMouseListener(IZYMouseListener *mouseListener)
{
    this->mouseListener=mouseListener;
}

//���������¼���������
void ZYFrame::SetNetworkListener(IZYNetworkListener *networkListener)
{
    this->networkListener=networkListener;
}

//��ȡ����ؼ�
IZYControl *ZYFrame::GetFocusControl(void)
{
    return (IZYControl *)ZYFrame::focusControl;
}

//�����Ƿ񿪷�ʽͼ�οⴰ��
void ZYFrame::SetOpenGLWindow(bool openGLWindow)
{
    this->openGLWindow=openGLWindow;
}

//���ô����¼���������1
void ZYFrame::SetFrameListener1(IZYFrameListener1 *frameListener1)
{
    this->frameListener1=frameListener1;
}

//���ڻ�ͼ��Ϣ����
void ZYFrame::OnWmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    vector<ZYControl *>::iterator it;

    PAINTSTRUCT ps;

    HDC hdcClient;
    
    HDC hdc;

    if(openGLWindow)
    {       
        return;
    }

    memset(&ps,0,sizeof(ps));

    ps.fErase=false;
    
    hdcClient = BeginPaint(hWnd, &ps);

    hdc=hDCBitmapBuffer;

    SetBkMode(hdc,TRANSPARENT);

    SelectObject(hdc,GetStockObject(OEM_FIXED_FONT));

    ZYGraphics *graphics=new ZYGraphics(hdc);

    if(bitmap!=NULL)
    {
        graphics->FillRectangle(-1,-1,1026,770,WHITE_BRUSH);

        graphics->DrawBitmap(bitmap,-1,-1,1026,770);
    }
    else
    {
        graphics->FillRectangle1(-1,-1,1026,770,backColor);
    }

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnPaint(graphics);
    }

    delete graphics;

    BitBlt(hdcClient,0,0,1024,768,hdc,0,0,SRCCOPY);

    EndPaint(hWnd, &ps);
}

//��갴����Ϣ����
void ZYFrame::OnWmLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    vector<ZYControl *>::reverse_iterator it;

    int mx,my,x,y,width,height;

    mx=LOWORD(lParam);
    my=HIWORD(lParam);

    if(this->mouseListener!=NULL)
    {
        this->mouseListener->MouseDown(NULL,mx,my);
    }

    for(it=controls->rbegin();it<controls->rend();it++)
    {
        (*it)->GetLocSize(&x,&y,&width,&height);

        if(mx>x&&mx<x+width&&my>y&&my<y+height)
        {
            (*it)->OnMouseDown(mx,my);

            return;
        }
    }
}

//����ƶ���Ϣ����
void ZYFrame::OnWmMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    vector<ZYControl *>::iterator it;

    int mx,my;

    mx=LOWORD(lParam);
    my=HIWORD(lParam);

    if(this->mouseListener!=NULL)
    {
        this->mouseListener->MouseMove(NULL,mx,my);
    }

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseMove(mx,my);
    }
}

//��굯����Ϣ����
void ZYFrame::OnWmLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    vector<ZYControl *>::iterator it;

    int mx,my;

    mx=LOWORD(lParam);
    my=HIWORD(lParam);

    if(this->mouseListener!=NULL)
    {
        this->mouseListener->MouseUp(NULL,mx,my);
    }

    for(it=controls->begin();it<controls->end();it++)
    {
        (*it)->OnMouseUp(mx,my);
    }
}

//���˫����Ϣ����
void ZYFrame::OnWmLButtonDblClk(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    vector<ZYControl *>::reverse_iterator it;

    int mx,my,x,y,width,height;

    mx=LOWORD(lParam);
    my=HIWORD(lParam);

    if(this->mouseListener!=NULL)
    {
        this->mouseListener->MouseDoubleClick(NULL,mx,my);
    }

    for(it=controls->rbegin();it<controls->rend();it++)
    {
        (*it)->GetLocSize(&x,&y,&width,&height);

        if(mx>x&&mx<x+width&&my>y&&my<y+height)
        {
            (*it)->OnMouseDoubleClick(mx,my);

            return;
        }
    }
}

//�˵�������Ϣ����
void ZYFrame::OnWmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    this->OnMenuCommand((int)LOWORD(wParam));
}

//���̰�����Ϣ����
void ZYFrame::OnWmKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(focusControl!=NULL)
    {
        focusControl->OnKeyDown((int)wParam);
    }
}

//�ַ�������Ϣ����
void ZYFrame::OnWmChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(focusControl!=NULL)
    {
        focusControl->OnCharInput((char)wParam);
    }
}

//��������Ϣ����
void ZYFrame::OnWmMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(this->mouseListener!=NULL)
    {
        this->mouseListener->MouseWheel(NULL,(int)wParam);
    }

    if(focusControl!=NULL)
    {
        focusControl->OnMouseWheel((int)wParam);
    }
}

//���ڳ�����Ϣ����
void ZYFrame::OnWmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(this->frameListener!=NULL)
    {
        this->frameListener->OnDestroy(this);
    }

    if(this->mainWindow==true)
    {
        PostQuitMessage(0);
    }
}

//���ڳ�����Ϣ����
void ZYFrame::OnWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    this->closed=true;

    if(this->frameListener!=NULL)
    {
        this->frameListener->OnClose(this);
    }
}

//��ȡ�����Ƿ��ѹر�
bool ZYFrame::GetClosed(void)
{
    return closed;
}

//�׽��ֶ���Ϣ����
void ZYFrame::OnWmNetworkEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(this->networkListener!=NULL)
    {
        this->networkListener->NetworkEvent(this,WSAGETSELECTERROR(lParam),WSAGETSELECTEVENT(lParam),(SOCKET)wParam);
    }
}