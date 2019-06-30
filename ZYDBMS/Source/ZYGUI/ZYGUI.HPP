//---------------------------------------------------------
//逐月界面引擎(ZYGUI) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYGUI_HPP)        //测试ZYGUI_HPP_宏
#define ZYGUI_HPP              //使得ZYGUI.HPP仅包含1次

#include <ZYRDB.HPP>

//套接字读消息
#define WM_NETWORK_EVENT WM_USER+500

//Windows函数入口点宏定义
#define Main(PARAM) APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)

//窗体风格
enum EFrameStyle
{
    EFrameStyle_StartUp,        //启动窗体
    EFrameStyle_Normal          //正常窗体
};

//滚动条类型
enum EScrollType
{
    EScrollType_Horizontal,     //水平滚动条
    EScrollType_Vertical        //垂直滚动条
};

//窗口对象
class IZYFrame;

//图形对象
class IZYGraphics;

//位图图片对象
class IZYBitmap;

//剪贴板对象
class IZYClipboard;

//控件对象
class IZYControl;

//静态文本控件对象
class IZYStaticText;

//静态图片控件对象
class IZYStaticPicture;

//按钮控件对象
class IZYButton;

//单选框控件对象
class IZYCheckBox;

//编辑控件对象
class IZYEdit;

//表格控件对象
class IZYGrid;

//滚动条控件对象
class IZYScrollBar;

//增强编辑对象
class IZYEditEx;

//SQL编辑控件对象
class IZYSqlEdit;

//窗体事件监听接口
class IZYFrameListener
{
public:
    //窗体撤销处理函数
    virtual void OnDestroy(IZYFrame *frame)=0;
    //窗体关闭处理函数
    virtual void OnClose(IZYFrame *frame)=0;
};

//窗体事件监听接口1
class IZYFrameListener1
{
public:
    //窗体消息处理函数
    virtual void OnWindowsMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)=0;
};

//菜单事件监听接口
class IZYMenuListener
{
public:
    //菜单命令处理函数
    virtual void MenuCommand(IZYFrame *frame,int mcid)=0;
};

//网络事件监听接口
class IZYNetworkListener
{
public:
    //网络事件处理函数
    virtual void NetworkEvent(IZYFrame *frame,int error,int event,SOCKET socket)=0;
};

//按钮动作监听接口
class IZYActionListener
{
public:
    //按钮动作处理函数
    virtual void ActionPerformed(IZYControl *control)=0;
};

//鼠标监听接口
class IZYMouseListener
{
public:
    //鼠标按下处理函数
    virtual void MouseDown(IZYControl *control,int mx,int my)=0;
    //鼠标移动处理函数
    virtual void MouseMove(IZYControl *control,int mx,int my)=0;
    //鼠标弹起处理函数
    virtual void MouseUp(IZYControl *control,int mx,int my)=0;
    //鼠标滚动处理函数
    virtual void MouseWheel(IZYControl *control,int mw)=0;
    //鼠标双击处理函数
    virtual void MouseDoubleClick(IZYControl *control,int mx,int my)=0;
};

//键盘监听接口
class IZYKeyListener
{
public:
    //键盘按下处理函数
    virtual void KeyDown(IZYControl *control,int vkey)=0;
    //字符输入处理函数
    virtual void CharInput(IZYControl *control,char ch)=0;
};

//滚动监听接口
class IZYScrollListener
{
public:
    //滚动处理函数
    virtual void BarScroll(IZYControl *control,int i_base)=0;
};

//创建窗口对象
IZYFrame *IZYFrame_Create(int x,int y,int width,int height,char *title);

//释放窗口对象
void IZYFrame_Release(IZYFrame *frame);

//窗口对象
class IZYFrame
{
public: 
    //加载菜单
    virtual HMENU LoadMenu(int rid)=0;
    //设置主菜单
    virtual void SetMainMenu(HMENU hMenu)=0;
    //增加控件
    virtual void AddControl(IZYControl *control)=0;
    //删除控件
    virtual void RemoveControl(IZYControl *control)=0;
    //设置位图图片
    virtual void SetBitmap(IZYBitmap *bitmap)=0;
    //设置背景颜色
    virtual void SetBackColor(COLORREF backColor)=0;
    //获取窗口句柄
    virtual HWND GetWindowHandle(void)=0;
    //窗口消息处理
    virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)=0;
    //窗口消息循环
    virtual int MsgLoop(void)=0;
    //刷新窗口
    virtual void Invalidate(void)=0;
    //菜单命令
    virtual void OnMenuCommand(int mcid)=0;
    //设置菜单事件监听对象
    virtual void SetMenuListener(IZYMenuListener *menuListener)=0;
    //设置窗体事件监听对象
    virtual void SetFrameListener(IZYFrameListener *frameListener)=0;
    //设置鼠标事件监听对象
    virtual void SetMouseListener(IZYMouseListener *mouseListener)=0;
    //关闭窗口
    virtual void Close(void)=0;
    //获取窗口是否已关闭
    virtual bool GetClosed(void)=0;
    //获取焦点控件
    virtual IZYControl *GetFocusControl(void)=0;
    //设置网络事件监听对象
    virtual void SetNetworkListener(IZYNetworkListener *networkListener)=0;
    //设置是否开放式图形库窗口
    virtual void SetOpenGLWindow(bool openGLWindow)=0;
    //设置窗体事件监听对象1
    virtual void SetFrameListener1(IZYFrameListener1 *frameListener1)=0;
};

//创建图形对象
IZYGraphics *IZYGraphics_Create(HDC hdc);

//释放图形对象
void IZYGraphics_Release(IZYGraphics *graphics);

//图形对象
class IZYGraphics
{
public:
    //设置坐标偏移
    virtual void SetXYBase(int x_base,int y_base)=0;
    //绘制一条线
    virtual void DrawLine(int x1,int y1,int x2,int y2)=0;
    //绘制一个矩形
    virtual void DrawRectangle(int x,int y,int w,int h)=0;
    //填充矩形
    virtual void FillRectangle(int x,int y,int w,int h,int brush)=0;
    //填充矩形1
    virtual void FillRectangle1(int x,int y,int w,int h,COLORREF color)=0;
    //输出文本
    virtual void OutputText(char *buf,int x,int y,int w,int h,int align)=0;
    //输出图象
    virtual void DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h)=0;
    //设置文本颜色
    virtual void SetTextColor(COLORREF color)=0;
    //设置线条颜色
    virtual void SetPenColor(COLORREF color)=0;
    //重置画笔颜色
    virtual void ResetPenColor(void)=0;
};

//创建位图图片对象
IZYBitmap *IZYBitmap_Create(int rid);

//释放位图图片对象
void IZYBitmap_Release(IZYBitmap *bitmap);

//位图图片对象
class IZYBitmap
{
public:
    //获取位图图片设备句柄
    virtual HDC GetHdcBitmap(void)=0;
};

//创建剪贴板对象
IZYClipboard *IZYClipboard_Create(IZYFrame *frame);

//释放剪贴板对象
void IZYClipboard_Release(IZYClipboard *clipboard);

//剪贴板对象
class IZYClipboard
{
public:
    //获取剪贴板中的文本
    virtual char *PasteText(void)=0;
    //设置剪贴板中的文本
    virtual void CopyText(char *text)=0;
};

//创建控件对象
IZYControl *IZYControl_Create(void);

//释放控件对象
void IZYControl_Release(IZYControl *control);

//控件对象
class IZYControl
{
public:
    //设置控件位置尺寸
    virtual void SetLocSize(int x,int y,int width,int height)=0;
    //获取控件位置尺寸
    virtual void GetLocSize(int *px,int *py,int *pwidth,int *pheight)=0;
    //获取控件标题文本
    virtual void SetTitle(char *title)=0;
    //获取控件标题文本
    virtual char *GetTitle(void)=0;
    //设置所在窗口
    virtual void SetFrame(IZYFrame *frame)=0;
    //获取所在窗口
    virtual IZYFrame *GetFrame(void)=0;
    //设置位图图片
    virtual void SetBitmap(IZYBitmap *bitmap)=0;
    //设置背景颜色
    virtual void SetBackColor(COLORREF backColor)=0;
    //设置键盘焦点
    virtual void SetFocus(bool focus)=0;
    //刷新控件
    virtual void Invalidate(void)=0;
    //处理控件重绘
    virtual void OnPaint(IZYGraphics *graphics)=0;
    //处理鼠标按下
    virtual void OnMouseDown(int mx,int my)=0;
    //处理鼠标移动
    virtual void OnMouseMove(int mx,int my)=0;
    //处理鼠标弹起
    virtual void OnMouseUp(int mx,int my)=0;
    //处理鼠标滚动
    virtual void OnMouseWheel(int mw)=0;
    //处理鼠标双击
    virtual void OnMouseDoubleClick(int mx,int my)=0;
    //处理键盘按下
    virtual void OnKeyDown(int vkey)=0;
    //处理字符输入
    virtual void OnCharInput(char ch)=0;
    //设置鼠标事件监听对象
    virtual void SetMouseListener(IZYMouseListener *mouseListener)=0;
    //设置键盘事件监听对象
    virtual void SetKeyListener(IZYKeyListener *keyListener)=0;
};

//创建静态文本控件对象
IZYStaticText *IZYStaticText_Create(char *title);

//释放静态文本控件对象
void IZYStaticText_Release(IZYStaticText *staticText);

//静态文本控件对象
class IZYStaticText
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
};

//创建静态图片控件对象
IZYStaticPicture *IZYStaticPicture_Create(IZYBitmap *bitmap);

//释放静态图片控件对象
void IZYStaticPicture_Release(IZYStaticPicture *staticPicture);

//静态图片控件对象
class IZYStaticPicture
{   
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
};

//创建按钮控件对象
IZYButton *IZYButton_Create(char *title);

//释放按钮控件对象
void IZYButton_Release(IZYButton *button);

//按钮控件对象
class IZYButton
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //设置按钮动作监听对象
    virtual void SetActionListener(IZYActionListener *actionListener)=0;
};

//创建单选框控件对象
IZYCheckBox *IZYCheckBox_Create(bool check);

//释放单选框控件对象
void IZYCheckBox_Release(IZYCheckBox *button);

//单选框控件对象
class IZYCheckBox
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //获取选择状态
    virtual bool GetCheckState(void)=0;
};

//创建编辑控件对象
IZYEdit *IZYEdit_Create(char *text);

//释放编辑控件对象
void IZYEdit_Release(IZYEdit *edit);

//编辑控件对象
class IZYEdit
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //设置是否为密码编辑框
    virtual void SetPasswordEdit(bool passwordEdit)=0;
    //设置是否为只读编辑框
    virtual void SetReadOnlyEdit(bool readOnlyEdit)=0;
    //设置编辑字符串
    virtual void SetEditText(char *text)=0;
    //获取编辑字符串
    virtual void GetEditText(char *text,int bufferLength)=0;
    //设置标记选定的文本的颜色
    virtual void SetMarkSelectedColor(COLORREF colorMarkSelected)=0;
    //获取选定的字符串
    virtual void GetSelectedText(char *text,int bufferLength)=0;
    //设置选定的字符串
    virtual void SetSelectedText(char *text)=0;
};

//创建滚动条控件对象
IZYScrollBar *IZYScrollBar_Create(int i_base,int m_display,int m,EScrollType type);

//释放滚动条控件对象
void IZYScrollBar_Release(IZYScrollBar *scrollBar);

//滚动条控件对象
class IZYScrollBar
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //设置滚动参数
    virtual void SetParam(int i_base,int m_display,int m)=0;
    //获取索引偏移
    virtual int GetIndexBase(void)=0;
    //设置索引偏移
    virtual void SetIndexBase(int i_base)=0;
    //设置滚动事件监听对象
    virtual void SetScrollListener(IZYScrollListener *scrollListener)=0;
    //设置滚动条位图图片
    virtual void SetBarBitmap(IZYBitmap *barBitmap)=0;
};

//创建表格控件对象
IZYGrid *IZYGrid_Create(IZYTable *table);

//释放表格控件对象
void IZYGrid_Release(IZYGrid *grid);

//表格控件对象
class IZYGrid
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //设置显示的表格
    virtual void SetTable(IZYTable *table)=0;
    //设置控件位置尺寸
    virtual void SetLocSize(int x,int y,int m_display,int unit_width,int unit_height)=0;  
    //删除当前选中行
    virtual bool DeleteSelectedItem(void)=0;
    //新增行
    virtual void AppendItem(void)=0;
    //设置标题图片
    virtual void SetTitleBitmap(IZYBitmap *titleBitmap)=0;
    //获取垂直滚动条控件
    virtual IZYScrollBar *GetScrollBarV(void)=0;
};

//创建增强编辑控件对象
IZYEditEx *IZYEditEx_Create(char *text);

//释放增强编辑控件对象
void IZYEditEx_Release(IZYEditEx *editEx);

//增强编辑控件对象
class IZYEditEx
{
public:
    //转换为控件对象
    virtual IZYControl *ToControl(void)=0;
    //设置是否为只读编辑框
    virtual void SetReadOnlyEdit(bool readOnlyEdit)=0;
    //设置是否检查SQL语句
    virtual void SetCheckSql(bool checkSql)=0;
    //设置编辑字符串
    virtual void SetEditText(char *text)=0;
    //获取编辑字符串
    virtual void GetEditText(char *text,int bufferLength)=0;
    //设置文字颜色
    virtual void SetTextColor(COLORREF color)=0;
    //获取垂直滚动条控件
    virtual IZYScrollBar *GetScrollBarV(void)=0;
    //获取水平滚动条控件
    virtual IZYScrollBar *GetScrollBarH(void)=0;
    //设置相关的输出编辑框
    virtual void SetRelativeOutput(IZYEditEx *output)=0;
    //设置相关的数据库对象
    virtual void SetRelativeDatabase(IZYDatabase *database)=0;
    //获取已修改标志
    virtual bool GetModified(void)=0;
    //设置已修改标志
    virtual void SetModified(bool modified)=0;
    //页面回到开始
    virtual void LocateToHome(void)=0;
    //页面回到结束
    virtual void LocateToEnd(void)=0;
    //获取选定的字符
    virtual void GetSelectedText(char *text,int bufferLength)=0;
    //设置选定的字符
    virtual void SetSelectedText(char *text)=0;
};

#endif