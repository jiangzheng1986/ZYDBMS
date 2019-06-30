//---------------------------------------------------------
//逐月界面引擎(ZYGUI) 作者:蒋政
//---------------------------------------------------------
#if !defined(ZYGUI1_HPP)        //测试ZYGUI_HPP_宏
#define ZYGUI1_HPP              //使得ZYGUI.HPP仅包含1次

//引入32位Windows API库
#include <windows.h>

//引入C++输入输出库
#include <iostream>

//引入C++文件库
#include <fstream>

//引入C++泛型算法库
#include <algorithm>

//引入C++泛型向量库
#include <vector>

//引入存储引擎(ZYRDB)API接口库
#include <ZYRDB.HPP>

//引入查询引擎(ZYSQL)API接口库
#include <ZYSQL.HPP>

//引入界面引擎(ZYGUI)API接口库
#include "ZYGUI.HPP"

//规定的文本的最大长度
#define MAX_TEXT_LEN 100000

//应用C++标准库名称空间(std)
using namespace std;

//滚动条状态
enum EStateScroll
{
    EStateScroll_Start,
    EStateScroll_Scroll
};

//控件对象
class ZYControl;

//位图图片对象
class ZYBitmap;

//容器对象
class ZYContainer;

//窗口对象
class ZYFrame:public IZYFrame
{
public:
    //构造窗口对象
    ZYFrame(int x,int y,int width,int height,char *title);
    //析构窗口对象
    ~ZYFrame(void);
    //加载菜单
    HMENU LoadMenu(int rid);
    //设置主菜单
    void SetMainMenu(HMENU hMenu);
    //增加控件
    void AddControl(IZYControl *control);
    //删除控件
    void RemoveControl(IZYControl *control);
    //设置位图图片
    void SetBitmap(IZYBitmap *bitmap);
    //设置背景颜色
    void SetBackColor(COLORREF backColor);
    //获取窗口句柄
    HWND GetWindowHandle(void);
    //窗口消息处理
    LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //窗口消息循环
    int MsgLoop(void);
    //刷新窗口
    void Invalidate(void);
    //关闭窗口
    void Close(void);
    //获取窗口是否已关闭
    bool GetClosed(void);
    //菜单命令
    void OnMenuCommand(int mcid);
    //设置菜单事件监听对象
    void SetMenuListener(IZYMenuListener *menuListener);
    //设置窗体事件监听对象
    void SetFrameListener(IZYFrameListener *frameListener);
    //设置鼠标事件监听对象
    void SetMouseListener(IZYMouseListener *mouseListener);
    //设置网络事件监听对象
    void SetNetworkListener(IZYNetworkListener *networkListener);
    //获取焦点控件
    IZYControl *GetFocusControl(void);
    //设置是否开放式图形库窗口
    void SetOpenGLWindow(bool openGLWindow);
    //设置窗体事件监听对象1
    void SetFrameListener1(IZYFrameListener1 *frameListener1);
    //窗口对象集
    static vector<ZYFrame *> *frames;
    //键盘焦点控件
    static ZYControl *focusControl;
    //插入符可视状态
    static bool caretVisible;
private:
    //注册窗口类
    ATOM RegisterWindowClass(void);
    //初始化窗口
    void InitWindow(void);
    //启动双缓冲
    void EnableDoubleBuffer(void);
    //窗口绘图消息处理
    void OnWmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //鼠标按下消息处理
    void OnWmLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //鼠标移动消息处理
    void OnWmMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //鼠标弹起消息处理
    void OnWmLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //菜单命令消息处理
    void OnWmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //键盘按下消息处理
    void OnWmKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //字符输入消息处理
    void OnWmChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //鼠标滚动消息处理
    void OnWmMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //鼠标双击消息处理
    void OnWmLButtonDblClk(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //窗口撤销消息处理
    void OnWmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //窗口撤销消息处理
    void OnWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //套接字读消息处理
    void OnWmNetworkEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //窗口句柄
    HWND hWnd;
    //控件集合
    vector<ZYControl *> *controls; 
    //屏幕DC
    HDC hDC;                                  
    //双缓冲位图
    HBITMAP hBitmapBuffer;                    
    //双缓冲位图设备
    HDC hDCBitmapBuffer;   
    //横坐标
    int x;                                  
    //竖坐标
    int y;                                  
    //宽度
    int width;                              
    //高度
    int height;
    //窗口标题字符串
    char *title;
    //主窗口
    bool mainWindow;
    //菜单事件监听对象
    IZYMenuListener *menuListener;
    //窗体事件监听对象
    IZYFrameListener *frameListener;
    //鼠标事件监听对象
    IZYMouseListener *mouseListener;
    //网络事件监听对象
    IZYNetworkListener *networkListener;
    //位图图片对象
    ZYBitmap *bitmap;
    //背景颜色
    COLORREF backColor;
    //窗口是否已关闭
    bool closed;
    //是否开放式图形库窗口
    bool openGLWindow;
    //窗体事件监听对象1
    IZYFrameListener1 *frameListener1;
};

//位图图片对象
class ZYBitmap:public IZYBitmap
{
public:
    //构造位图图片对象
    ZYBitmap(int rid);
    //获取位图图片设备句柄
    HDC GetHdcBitmap(void);
private:
    //位图图片句柄
    HBITMAP hBitmap;
    //位图图片设备句柄
    HDC hdcBitmap;
};

//图形对象
class ZYGraphics:public IZYGraphics
{
public:
    //构造图形对象
    ZYGraphics(HDC hdc);
    //设置坐标偏移
    void SetXYBase(int x_base,int y_base);
    //绘制一条线
    void DrawLine(int x1,int y1,int x2,int y2);
    //绘制一个矩形
    void DrawRectangle(int x,int y,int w,int h);
    //填充矩形
    void FillRectangle(int x,int y,int w,int h,int brush);
    //填充矩形1
    void FillRectangle1(int x,int y,int w,int h,COLORREF color);
    //输出文本
    void OutputText(char *buf,int x,int y,int w,int h,int align);
    //输出图象
    void DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h);
    //设置文本颜色
    void SetTextColor(COLORREF color);
    //设置画笔颜色
    void SetPenColor(COLORREF color);
    //重置画笔颜色
    void ResetPenColor(void);
private:
    //图形设备句柄
    HDC hdc;
    //画笔对象句柄
    HANDLE hPen;
    //X偏移
    int x_base;
    //Y偏移
    int y_base;
};

//剪贴板对象
class ZYClipboard:public IZYClipboard
{
public:
    //构造剪贴板对象
    ZYClipboard(IZYFrame *frame);
    //获取剪贴板中的文本
    char *PasteText(void);
    //设置剪贴板中的文本
    void CopyText(char *text);
private:
    //窗口句柄
    HWND hWnd;
};

//控件对象
class ZYControl:public IZYControl
{
public:
    //构造控件对象
    ZYControl(void);
    //析构控件对象
    ~ZYControl(void);
    //设置控件位置尺寸
    void SetLocSize(int x,int y,int width,int height);
    //获取控件位置尺寸
    void GetLocSize(int *px,int *py,int *pwidth,int *pheight);
    //获取控件标题文本
    void SetTitle(char *title);
    //获取控件标题文本
    char *GetTitle(void);
    //设置所在窗口
    void SetFrame(IZYFrame *frame);
    //获取所在窗口
    IZYFrame *GetFrame(void);
    //设置所在容器
    void SetContainer(ZYContainer *container);
    //设置位图图片
    void SetBitmap(IZYBitmap *bitmap);
    //设置背景颜色
    void SetBackColor(COLORREF backColor);
    //设置键盘焦点
    void SetFocus(bool focus);
    //刷新控件
    void Invalidate(void);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标移动
    void OnMouseMove(int mx,int my);
    //处理鼠标弹起
    void OnMouseUp(int mx,int my);
    //处理鼠标滚动
    void OnMouseWheel(int mw);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
    //处理键盘按下
    void OnKeyDown(int vkey);
    //处理字符输入
    void OnCharInput(char ch);
    //设置鼠标事件监听对象
    void SetMouseListener(IZYMouseListener *mouseListener);
    //设置键盘事件监听对象
    void SetKeyListener(IZYKeyListener *keyListener);
    //复制一个字符串,带字符串空间分配
    char *CopyString(char *s);
protected:
    //横坐标
    int x;                                  
    //竖坐标
    int y;                                  
    //宽度
    int width;                              
    //高度
    int height;
    //标题文本
    char *title;
    //位图图片对象
    ZYBitmap *bitmap;
    //所在窗口
    ZYFrame *frame;
    //所在容器
    ZYContainer *container;
    //鼠标事件监听对象
    IZYMouseListener *mouseListener;
    //键盘事件监听对象
    IZYKeyListener *keyListener;
    //背景颜色
    COLORREF backColor;
};

//容器对象
class ZYContainer:public ZYControl
{
public:
    //构造容器对象
    ZYContainer(void);
    //构造容器对象
    virtual ~ZYContainer(void);
    //获取X偏移
    int GetXBase(void);
    //获取Y偏移
    int GetYBase(void);
    //设置所在窗口
    void SetFrame(IZYFrame *frame);
    //设置所在窗口
    void SetBitmap(IZYBitmap *bitmap);
    //处理容器重绘
    void OnPaint(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标移动
    void OnMouseMove(int mx,int my);
    //处理鼠标弹起
    void OnMouseUp(int mx,int my);
    //增加子控件
    void AddControl(IZYControl *control);
    //删除控件
    void RemoveControl(IZYControl *control);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
private:
    //控件集合
    vector<ZYControl *> *controls;
};

//静态文本控件对象                                                                                                                                                                                                                                                                 
class ZYStaticText:public ZYControl,public IZYStaticText
{
public:
    //构造静态文本控件对象
    ZYStaticText(char *title);
    //转换为控件对象
    IZYControl *ToControl(void);
    //绘制静态文本
    void OnPaint(IZYGraphics *graphics);
};

//静态图片控件对象
class ZYStaticPicture:public ZYControl,public IZYStaticPicture
{   
public:
    //构造静态图片控件对象
    ZYStaticPicture(ZYBitmap *bitmap);
    //转换为控件对象
    IZYControl *ToControl(void);
    //绘制静态图片
    void OnPaint(IZYGraphics *graphics);
};

//按钮控件对象
class ZYButton:public ZYControl,public IZYButton
{
public:
    //构造按钮控件对象
    ZYButton(char *title);
    //转换为控件对象
    IZYControl *ToControl(void);
    //设置按钮动作监听对象
    void SetActionListener(IZYActionListener *actionListener);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
private:
    //按钮动作监听对象
    IZYActionListener *actionListener;
};

//单选框控件对象
class ZYCheckBox:public ZYControl,public IZYCheckBox
{
public:
    //构造单选框控件对象
    ZYCheckBox(bool check);
    //转换为控件对象
    IZYControl *ToControl(void);
    //获取选择状态
    bool GetCheckState(void);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
private:
    //选择状态
    bool check;          
};

//编辑控件对象
class ZYEdit:public ZYControl,public IZYEdit
{
public:
    //构造编辑控件对象
    ZYEdit(char *text);
    //转换为控件对象
    IZYControl *ToControl(void);
    //设置控件位置尺寸
    void SetLocSize(int x,int y,int width,int height);
    //设置是否为密码编辑框
    void SetPasswordEdit(bool passwordEdit);
    //设置是否为只读编辑框
    void SetReadOnlyEdit(bool readOnlyEdit);
    //设置编辑字符串
    void SetEditText(char *text);
    //获取编辑字符串
    void GetEditText(char *text,int bufferLength);
    //设置键盘焦点
    void SetFocus(bool focus);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标移动
    void OnMouseMove(int mx,int my);
    //处理鼠标弹起
    void OnMouseUp(int mx,int my);
    //处理键盘按下
    void OnKeyDown(int vkey);
    //处理字符输入
    void OnCharInput(char ch);
    //设置标记选定的文本的颜色
    void SetMarkSelectedColor(COLORREF colorMarkSelected);
    //获取选定的字符串
    void GetSelectedText(char *text,int bufferLength);
    //设置选定的字符串
    void SetSelectedText(char *text);
private:
    //显示插入符
    void ShowCaret(void);
    //隐藏插入符
    void HideCaret(void);
    //0:英文,1:中文首字节,2:中文次字节
    int TestChar(char *text,int i);
    //是否中文字符
    int IsChineseChar(int i);
    //递增字符索引
    void IncEditIndex(void);
    //递减字符索引
    void DecEditIndex(void);
    //字符索引回到开始
    void HomeEditIndex(void);
    //字符索引回到结束
    void EndEditIndex(void);
    //递减字符索引,用于回退字符
    void DecEditIndex2(void);
    //插入字符到编辑字符串
    void InsertChar(int i,char c);
    //从编辑字符串删除字符
    void DeleteChar(int i,int c);
    //从编辑字符串删除字符,考虑了中文
    void DeleteChar2(void);
    //在编辑字符串中回退字符,考虑了中文
    void BackspaceChar(void);
    //递减字符索引,考虑了中文
    void LeftKey(void);
    //递增字符索引,考虑了中文
    void RightKey(void);
    //标记选定的文本
    void MarkSelectedText(IZYGraphics *graphics);
    //标记选定的文本
    void MarkSelectedText1(IZYGraphics *graphics,int start,int end);
    //使文本开始小于文本结束
    void CheckSelectedStartEnd(void);
    //编辑字符串
    char text[MAX_TEXT_LEN];
    //字符数
    int n;                                  
    //编辑位置
    int i;
    //编辑位置基值
    int i_base;
    //编辑显示字符数
    int n_display;                          
    //是否为密码编辑框
    bool passwordEdit;
    //是否为只读编辑框
    bool readOnlyEdit;
    //标记选定的文本的颜色
    COLORREF colorMarkSelected;
    //选定文本开始
    int selectedStart;
    //选定文本结束
    int selectedEnd;
    //选定状态
    bool stateSelect;
};

//滚动条控件对象
class ZYScrollBar:public ZYControl,public IZYScrollBar
{
public:
    //构造滚动条控件对象
    ZYScrollBar(int i_base,int m_display,int m,EScrollType type);
    //转换为控件对象
    IZYControl *ToControl(void);
    //设置滚动参数
    void SetParam(int i_base,int m_display,int m);
    //获取索引偏移
    int GetIndexBase(void);
    //设置索引偏移
    void SetIndexBase(int i_base);
    //设置滚动事件监听对象
    void SetScrollListener(IZYScrollListener *scrollListener);
    //设置滚动条位图图片
    void SetBarBitmap(IZYBitmap *barBitmap);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标移动
    void OnMouseMove(int mx,int my);
    //处理鼠标弹起
    void OnMouseUp(int mx,int my);
    //处理鼠标滚动
    void OnMouseWheel(int mw);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
private:
    //绘制下翻箭头
    void DrawDownIcon(IZYGraphics *graphics,int x,int y,int h);
    //绘制上翻箭头
    void DrawUpIcon(IZYGraphics *graphics,int x,int y,int h);
    //绘制左翻箭头
    void DrawLeftIcon(IZYGraphics *graphics,int x,int y,int h);
    //绘制右翻箭头
    void DrawRightIcon(IZYGraphics *graphics,int x,int y,int h);
    //绘制滚动条
    void DrawScrollBarV(IZYGraphics *graphics,int x,int y,int w,int m,int i_base,int m_display);
    //绘制滚动条
    void DrawScrollBarH(IZYGraphics *graphics,int x,int y,int h,int m,int i_base,int m_display);
    //处理鼠标按下
    void OnMouseDownV(int mx,int my);
    //处理鼠标按下
    void OnMouseDownH(int mx,int my);
    //处理鼠标移动
    void OnMouseMoveV(int mx,int my);
    //处理鼠标移动
    void OnMouseMoveH(int mx,int my);
    //减少索引基值
    void DecIndexBase(void);
    //增加索引基值
    void IncIndexBase(void);
    //按页减少索引基值
    void DecIndexBasePage(void);
    //按页增加索引基值
    void IncIndexBasePage(void);
    //索引偏移
    int i_base;                             
    //显示行数
    int m_display;
    //总计行数
    int m;
    //滚动条滑块的上边相对于滚动条的位置
    double h_scroll_1;
    //滚动条滑块的下边相对于滚动条的位置
    double h_scroll_2;  
    //保存的滚动条上边位置
    double h_scroll_3;                  
    //保存的滚动条下边位置
    double h_scroll_4;                  
    //滚动时鼠标位置
    double m_scroll;
    //滚动状态
    EStateScroll state_scroll;  
    //滚动条类型
    EScrollType type;
    //滚动事件监听对象
    IZYScrollListener *scrollListener;
    //滚动条位图图片
    IZYBitmap *barBitmap;
};

//表格控件对象
class ZYGrid:public ZYContainer,public IZYGrid
{
public:
    //构造表格控件对象
    ZYGrid(IZYTable *table);
    //转换为控件对象
    IZYControl *ToControl(void);
    //设置显示的表格
    void SetTable(IZYTable *table);
    //设置控件位置尺寸
    void SetLocSize(int x,int y,int m_display,int unit_width,int unit_height);  
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //测试点是否在矩形内
    bool PointInRect(int p_x,int p_y,int x,int y,int width,int height);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标滚动
    void OnMouseWheel(int mw);
    //处理鼠标双击
    void OnMouseDoubleClick(int mx,int my);
    //处理键盘按下
    void OnKeyDown(int vkey);
    //上移当前选中行
    void DecSelectedItem(void);
    //下移当前选中行
    void IncSelectedItem(void);
    //按页上移当前选中行
    void DecSelectedItemPage(void);
    //按页下移当前选中行
    void IncSelectedItemPage(void);
    //删除当前选中行
    bool DeleteSelectedItem(void);
    //新增行
    void AppendItem(void);
    //当前选中行回到开始
    void HomeSelectedItem(void);
    //当前选中行去到结束
    void EndSelectedItem(void);
    //设置标题图片
    void SetTitleBitmap(IZYBitmap *titleBitmap);
    //获取垂直滚动条控件
    IZYScrollBar *GetScrollBarV(void);
private:
    //输出表名
    void OutputTableName(IZYGraphics *graphics,int x,int y,int w,int h);
    //输出列名
    void OutputColumnName(IZYGraphics *graphics,int j,int x,int y,int w,int h);
    //将6个小数截断为2个小数
    void ProcessDouble(char *str_double);
    //输出数据
    void OutputData(IZYGraphics *graphics,int i,int j,int x,int y,int w,int h);
    //绘制表格线
    void DrawGirds(IZYGraphics *graphics,int x,int y,int w,int h,int m,int n);
    // 绘制十字箭头
    void DrawCrossIcon(IZYGraphics *graphics,int x,int y,int h);
    //绘制当前选中行浮动条
    void DrawItemBar(IZYGraphics *graphics);
    //更新滚动参数
    void UpdateScrollParam(int i_base);
    //获取并格式化表格数据
    void GetTableData(char *s,IZYTable *table,int i,int j);
    //格式化并设置表格数据
    void PutTableData(char *s,IZYTable *table,int i,int j);
    //离开编辑模式
    void LeaveEditMode(void);
    //表格对象
    IZYTable *table;
    //滚动条
    IZYScrollBar *scrollBar;
    //单元格宽度
    int unit_width;
    //单元格高度
    int unit_height;
    //显示行数
    int m_display;
    //表的当前项
    int item;
    //表的当前列
    int column;
    //输入的行
    int i_input;
    //输入的列
    int j_input;
    //输入编辑框
    IZYEdit *editData;
    //标题图片
    IZYBitmap *titleBitmap;
};

//增强字符串对象
class ZYStringEx
{
public:
    //构造增强字符串对象
    ZYStringEx(char *s);
    //析构增强字符串对象
    ~ZYStringEx(void);
    //解析字符串
    void PutString(char *s);
    //获取字符串
    void GetString(char *s,int len);
    //获取字符行
    void GetLine(int lineno,char *buf);
    //获取字符
    char GetChar(int i,int j);
    //获取字符行计数
    int GetLineCount(void);
    //获取行字符计数
    int GetCharCount(int lineno);
    //获取字符最多的行的字符数
    int GetMaxChar(void);
    //删除字符
    void DeleteChar(int i,int j);
    //插入字符
    void InsertChar(int i,int j,char ch);
    //插入换行符
    void InsertNewLine(int i,int j);
    //检查SQL语句
    void CheckSql(IZYDatabase *database,IZYEditEx *output);
    //获取字符类型
    ESourceCharType GetCharType(int i,int j);
    //测试不匹配标志
    bool TestDismatchFlag(int i,int j);
    //测试已确认标志
    bool TestAcknowledgedFlag(int i,int j);
    //删除字符
    void DeleteChars(int i1,int j1,int i2,int j2);
    //删除字符行
    void DeleteLine(int i);
    //获取字符串
    void GetString(char *s,int i1,int j1,int i2,int j2,int len);
private:
    //释放增强字符串
    void ReleaseStrEx(void);
    //释放格式字符串
    void ReleaseFormatEx(void);
    //增强字符串数据结构
    vector<vector<char> *> *strEx;
    //格式字符串数据结构
    vector<vector<char> *> *formatEx;
};

//增强编辑控件对象
class ZYEditEx:public ZYContainer,public IZYEditEx,public IZYScrollListener
{
public:
    //构造增强编辑控件对象
    ZYEditEx(char *text);
    //析构增强编辑控件对象
    ~ZYEditEx(void);
    //转换为控件对象
    IZYControl *ToControl(void);
    //设置是否为只读编辑框
    void SetReadOnlyEdit(bool readOnlyEdit);
    //设置是否检查SQL语句
    void SetCheckSql(bool checkSql);
    //设置控件位置尺寸
    void SetLocSize(int x,int y,int width,int height);
    //设置编辑字符串
    void SetEditText(char *text);
    //获取编辑字符串
    void GetEditText(char *text,int bufferLength);
    //设置文字颜色
    void SetTextColor(COLORREF color);
    //获取垂直滚动条控件
    IZYScrollBar *GetScrollBarV(void);
    //获取水平滚动条控件
    IZYScrollBar *GetScrollBarH(void);
    //设置相关的输出编辑框
    void SetRelativeOutput(IZYEditEx *output);
    //设置相关的数据库对象
    void SetRelativeDatabase(IZYDatabase *database);
    //处理控件重绘
    void OnPaint(IZYGraphics *graphics);
    //处理控件重绘
    void OnPaintSql(IZYGraphics *graphics);
    //处理鼠标按下
    void OnMouseDown(int mx,int my);
    //处理鼠标滚动
    void OnMouseWheel(int mw);
    //处理键盘按下
    void OnKeyDown(int vkey);
    //处理字符输入
    void OnCharInput(char ch);
    //滚动处理函数
    void BarScroll(IZYControl *control,int i_base);
    //获取已修改标志
    bool GetModified(void);
    //设置已修改标志
    void SetModified(bool modified);
    //页面回到开始
    void LocateToHome(void);
    //页面回到结束
    void LocateToEnd(void);
    //处理鼠标移动
    void OnMouseMove(int mx,int my);
    //处理鼠标弹起
    void OnMouseUp(int mx,int my);
    //获取选定的字符
    void GetSelectedText(char *text,int bufferLength);
    //设置选定的字符
    void SetSelectedText(char *text);
protected:
    //设置键盘焦点
    void SetFocus(bool focus);
    //显示插入符
    void ShowCaret(void);
    //隐藏插入符
    void HideCaret(void);
    //更新滚动条参数
    void UpdateScrollParam(void);
    //获取滚动条基数
    void GetScrollBase(void);
    //0:英文,1:中文首字节,2:中文次字节
    int TestChar(char *text,int i);
    //0:英文,1:中文首字节,2:中文次字节
    int TestChar1(int i,int j);
    //是否中文字符
    bool IsChineseChar(int i,int j);
    //递增字符索引
    void IncEditIndex(void);
    //递减字符索引
    void DecEditIndex(void);
    //字符索引回到开始
    void HomeEditIndex(void);
    //字符索引回到结束
    void EndEditIndex(void);
    //递减字符索引,考虑了中文
    void LeftKey(void);
    //递增字符索引,考虑了中文
    void RightKey(void);
    //递减行索引,考虑了中文
    void UpKey(void);
    //递增行索引,考虑了中文
    void DownKey(void);
    //按页递减行索引,考虑了中文
    void PageUp(void);
    //按页递增行索引,考虑了中文
    void PageDown(void);
    //递减字符索引,用于回退字符
    void DecEditIndex2(void);
    //从编辑字符串删除字符,考虑了中文
    void DeleteChar(void);
    //在编辑字符串中回退字符,考虑了中文
    void BackspaceChar(void);
    //插入换行符
    void InsertNewLine(void);
    //检查SQL语法
    void CheckSql(void);
    //智能感知
    void IntelligentSense(void);
    //使文本开始小于文本结束
    void CheckSelectedStartEnd(void);
    //使文本开始小于文本结束
    void ZYEditEx::CheckSelectedStartEnd1(int *p_i_selectedStart,
                                          int *p_i_selectedEnd,
                                          int *p_j_selectedStart,
                                          int *p_j_selectedEnd);
    //设置标记选定的文本的颜色
    void SetMarkSelectedColor(COLORREF colorMarkSelected);
    //标记选定的文本
    void MarkSelectedText(IZYGraphics *graphics);
    //标记选定的文本
    void MarkSelectedText1(IZYGraphics *graphics,int i,int start,int end);
    //移动键处理函数开始处理
    bool MoveKeyStart(int type);
    //移动键处理函数结束处理
    void MoveKeyEnd(void);
    //删除当前选定的字符
    void DeleteSelectedText(void);
    //增强字符串对象
    ZYStringEx *stringEx;
    //竖直滚动条
    IZYScrollBar *scrollBarV;
    //水平滚动条
    IZYScrollBar *scrollBarH;
    //字符行高
    int line_height;
    //显示字符行数
    int m_display;
    //每行显示字符数
    int n_display;
    //是否为只读编辑框
    bool readOnlyEdit;
    //是否检查SQL语句
    bool checkSql;
    //行编辑位置
    int i;
    //行编辑位置基值
    int i_base;
    //列编辑位置
    int j;
    //列编辑位置基值
    int j_base;
    //文字颜色
    COLORREF color;
    //相关的输出编辑框
    IZYEditEx *output;
    //相关的数据库对象
    IZYDatabase *database;
    //已修改标志
    bool modified;
    //标记选定的文本的颜色
    COLORREF colorMarkSelected;
    //选定文本开始行
    int i_selectedStart;
    //选定文本结束行
    int i_selectedEnd;
    //选定文本开始列
    int j_selectedStart;
    //选定文本结束列
    int j_selectedEnd;
    //选定状态
    bool stateSelect;
};

#endif
