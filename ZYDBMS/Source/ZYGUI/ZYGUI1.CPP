//---------------------------------------------------------
//逐月界面引擎(ZYGUI)-系统框架 作者:蒋政
//---------------------------------------------------------
#include "resource.h"

#include "ZYGUI1.HPP"

//窗口类名称
#define ZYWNDCLS "ZYWNDCLS"

//窗口对象集
vector<ZYFrame *> *ZYFrame::frames=NULL;

//键盘焦点控件
ZYControl *ZYFrame::focusControl=NULL;

//插入符可视状态
bool ZYFrame::caretVisible=false;

//创建窗口对象
IZYFrame *IZYFrame_Create(int x,int y,int width,int height,char *title)
{
    return (IZYFrame *)new ZYFrame(x,y,width,height,title);
}

//释放窗口对象
void IZYFrame_Release(IZYFrame *frame)
{
    delete (ZYFrame *)frame;
}

//构造窗口对象
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

//析构窗口对象
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

//加载菜单
HMENU ZYFrame::LoadMenu(int rid)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HMENU hMenu=::LoadMenu(hInstance,(LPSTR)rid);
    return hMenu;
}

//设置主菜单
void ZYFrame::SetMainMenu(HMENU hMenu)
{
    SetMenu(hWnd,hMenu);
}

//增加控件
void ZYFrame::AddControl(IZYControl *control)
{
    controls->push_back((ZYControl *)control);
    control->SetFrame(this);
}

//删除控件
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

//设置位图图片
void ZYFrame::SetBitmap(IZYBitmap *bitmap)
{
    this->bitmap=(ZYBitmap *)bitmap;
}

//设置背景颜色
void ZYFrame::SetBackColor(COLORREF backColor)
{
    this->backColor=backColor;
}

//获取窗口句柄
HWND ZYFrame::GetWindowHandle(void)
{
    return hWnd;
}

//刷新窗口
void ZYFrame::Invalidate(void)
{
    RECT rect;

    GetClientRect(hWnd,&rect);
    
    InvalidateRect(hWnd,&rect,FALSE);
}

//关闭窗口
void ZYFrame::Close(void)
{
    if(GetClosed()==false)
    {
        OnWmClose(hWnd,0,0);

        DestroyWindow(GetWindowHandle());
    }
}

//窗口过程
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

//注册窗口类
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

//初始化窗口
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

//启动双缓冲
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

//窗口消息处理
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

//窗口消息循环
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

//菜单命令
void ZYFrame::OnMenuCommand(int mcid)
{
    if(menuListener!=NULL)
    {
        menuListener->MenuCommand(this,mcid);
    }
}

//设置菜单事件监听对象
void ZYFrame::SetMenuListener(IZYMenuListener *menuListener)
{
    this->menuListener=menuListener;
}

//设置窗体事件监听对象
void ZYFrame::SetFrameListener(IZYFrameListener *frameListener)
{
    this->frameListener=frameListener;
}

//设置鼠标事件监听对象
void ZYFrame::SetMouseListener(IZYMouseListener *mouseListener)
{
    this->mouseListener=mouseListener;
}

//设置网络事件监听对象
void ZYFrame::SetNetworkListener(IZYNetworkListener *networkListener)
{
    this->networkListener=networkListener;
}

//获取焦点控件
IZYControl *ZYFrame::GetFocusControl(void)
{
    return (IZYControl *)ZYFrame::focusControl;
}

//设置是否开放式图形库窗口
void ZYFrame::SetOpenGLWindow(bool openGLWindow)
{
    this->openGLWindow=openGLWindow;
}

//设置窗体事件监听对象1
void ZYFrame::SetFrameListener1(IZYFrameListener1 *frameListener1)
{
    this->frameListener1=frameListener1;
}

//窗口绘图消息处理
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

//鼠标按下消息处理
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

//鼠标移动消息处理
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

//鼠标弹起消息处理
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

//鼠标双击消息处理
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

//菜单命令消息处理
void ZYFrame::OnWmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    this->OnMenuCommand((int)LOWORD(wParam));
}

//键盘按下消息处理
void ZYFrame::OnWmKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(focusControl!=NULL)
    {
        focusControl->OnKeyDown((int)wParam);
    }
}

//字符输入消息处理
void ZYFrame::OnWmChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(focusControl!=NULL)
    {
        focusControl->OnCharInput((char)wParam);
    }
}

//鼠标滚动消息处理
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

//窗口撤销消息处理
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

//窗口撤销消息处理
void ZYFrame::OnWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    this->closed=true;

    if(this->frameListener!=NULL)
    {
        this->frameListener->OnClose(this);
    }
}

//获取窗口是否已关闭
bool ZYFrame::GetClosed(void)
{
    return closed;
}

//套接字读消息处理
void ZYFrame::OnWmNetworkEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if(this->networkListener!=NULL)
    {
        this->networkListener->NetworkEvent(this,WSAGETSELECTERROR(lParam),WSAGETSELECTEVENT(lParam),(SOCKET)wParam);
    }
}
