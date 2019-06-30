//-------------------------------------------------------------------
//逐月关系型数据库管理系统 作者:蒋政
//-------------------------------------------------------------------

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

//引入存储引擎(ZYRDB)API库
#include <ZYRDB.HPP>

//引入查询引擎(ZYSQL)API库
#include <ZYSQL.HPP>

//引入界面引擎(ZYGUI)API库
#include <ZYGUI.HPP>

//引入资源头文件
#include "resource.h"

//应用C++标准库名称空间(std)
using namespace std;

//蓝色1
#define BLUE_COLOR_1 0x00E0A0A0

//蓝色2
#define BLUE_COLOR_2 0x00FFD2D2

//蓝色3
#define BLUE_COLOR_3 0x00FFA0A0

//蓝色4
#define BLUE_COLOR_4 0x00FFF5F5

//系统配置数据库
extern IZYDatabase *configration;

//表格图标
extern IZYBitmap *bitmapTable;

//索引图标
extern IZYBitmap *bitmapIndex;

//查询图标
extern IZYBitmap *bitmapQuery;

//新建图标
extern IZYBitmap *bitmapCreateItem;

//水平位图条
extern IZYBitmap *bitmapBarH;

//垂直位图条
extern IZYBitmap *bitmapBarV;

//背景图片1
extern IZYBitmap *bitmapBack1;

//背景图片2
extern IZYBitmap *bitmapBack2;

//加载所有位图
void LoadAllBitmaps(void);

//数据库子窗口
class ZYSubQuery
{
public:
    //析构数据库子窗口
    virtual ~ZYSubQuery(void);
};

//系统主窗口
class ZYQuery1:public IZYMenuListener,public IZYActionListener,public IZYFrameListener
{
public:
    //构造系统主窗口
    ZYQuery1(void);
    //析构系统主窗口
    ~ZYQuery1(void);
    //执行消息循环
    int MsgLoop(void);
    //鼠标命令处理函数
    void MenuCommand(IZYFrame *frame,int mcid);
    //按钮动作处理函数
    void ActionPerformed(IZYControl *control);
    //窗体撤销处理函数
    void OnDestroy(IZYFrame *frame);
    //窗体关闭处理函数
    void OnClose(IZYFrame *frame);
private:
    //窗体-系统主窗口
    IZYFrame *frame;
    //菜单-系统主窗口菜单
    HMENU mainMenu;
    //静态文本框-系统主窗口标题
    IZYStaticText *staticTextCaption;
    //静态文本框-数据库路径
    IZYStaticText *staticTextPath;
    //静态文本框-数据库名称
    IZYStaticText *staticTextName;
    //静态文本框-用户名
    IZYStaticText *staticTextUserid;
    //静态文本框-密码
    IZYStaticText *staticTextPassword;
    //编辑框-数据库路径
    IZYEdit *editPath;
    //编辑框-数据库名称
    IZYEdit *editName;
    //编辑框-用户名
    IZYEdit *editUserid;
    //编辑框-密码
    IZYEdit *editPassword;
    //按钮-登陆
    IZYButton *buttonOK;
    //按钮-取消
    IZYButton *buttonCancel;
    //窗体-子窗口
    vector<ZYSubQuery *> *subQuerys;
};

//数据库窗口
class ZYQuery2:public ZYSubQuery,public IZYMenuListener,public IZYMouseListener,public IZYFrameListener
{
public:
    //构造数据库窗口
    ZYQuery2(char *path,IZYDatabase *database);
    //析构数据库窗口
    ~ZYQuery2(void);
    //鼠标命令处理函数
    void MenuCommand(IZYFrame *frame,int mcid);
    //鼠标按下处理函数
    void MouseDown(IZYControl *control,int mx,int my);
    //鼠标移动处理函数
    void MouseMove(IZYControl *control,int mx,int my);
    //鼠标弹起处理函数
    void MouseUp(IZYControl *control,int mx,int my);
    //鼠标滚动处理函数
    void MouseWheel(IZYControl *control,int mw);
    //鼠标双击处理函数
    void MouseDoubleClick(IZYControl *control,int mx,int my);
    //窗体撤销处理函数
    void OnDestroy(IZYFrame *frame);
    //窗体关闭处理函数
    void OnClose(IZYFrame *frame);
    //测试点是否在矩形内
    bool PointInRect(int p_x,int p_y,int x,int y,int width,int height);
    //测试点是否在面板滚动区内
    bool PointInPanelBar(int p_x,int p_y,int loc);
    //测试点是否在项滚动区内
    bool PointInItemBar(int p_x,int p_y,int loc);
    //设置面板滚动条的位置
    void SetPanelBarLoc(IZYStaticPicture *panelBar,int loc);
    //设置项滚动条的位置
    void SetItemBarLoc(IZYStaticPicture *itemBar,int loc);
    //切换面板选项
    void AlterPanelItem(int panelItem);
    //增加项
    void AddItem(char *title,int type);
    //清空项
    void ClearItems(void);
    //新建查询
    ZYSubQuery *NewQuery3(void);
    //新建项
    void NewItem(int type);
    //打开项
    void OpenItem(int type,char *name);
    //增加全部表格
    void AddTables(void);
    //增加全部索引
    void AddIndexs(void);
    //增加全部查询
    void AddQuerys(void);
    //删除查询
    void DeleteQuery(char *name);
    //删除当前选定项
    void DeleteSelectedItem(int type);
private:
    //窗体-数据库窗口
    IZYFrame *frame;
    //菜单-数据库窗口菜单
    HMENU mainMenu;
    //静态图片框-数据库面板
    IZYStaticPicture *staticPicturePanel;
    //静态图片框-面板固定条
    IZYStaticPicture *staticPicturePanelBar1;
    //静态图片框-面板浮动条
    IZYStaticPicture *staticPicturePanelBar2;
    //静态图片框-表格
    IZYStaticPicture *staticPictureTable;
    //静态图片框-索引
    IZYStaticPicture *staticPictureIndex;
    //静态图片框-查询
    IZYStaticPicture *staticPictureQuery;
    //静态图片框-项固定条
    IZYStaticPicture *staticPictureItemBar;
    //静态图片框-创建表格
    IZYStaticPicture *staticPictureCreateItem;
    //静态图片框-项
    vector<IZYStaticPicture *> *staticPictureItems;
    //静态文本框-表格
    IZYStaticText *staticTextTable;
    //静态文本框-索引
    IZYStaticText *staticTextIndex;
    //静态文本框-查询
    IZYStaticText *staticTextQuery;
    //静态文本框-创建项
    IZYStaticText *staticTextCreateItem;
    //静态文本框-项
    vector<IZYStaticText *> *staticTextItems;
    //数据库路径
    char path[1000];
    //数据库对象
    IZYDatabase *database;
    //当前选定项目
    int leftSelect;
    //当前浮动项目
    int leftItem;
    //当前选定项目
    int rightSelect;
    //鼠标按下状态
    bool mouseDown;
    //窗体-子窗口
    vector<ZYSubQuery *> *subQuerys;
};

//查询窗口
class ZYQuery3:public ZYSubQuery,public IZYMenuListener,public IZYFrameListener
{
public:
    //构造查询窗口
    ZYQuery3(ZYQuery2 *query2,char *path,IZYDatabase *database,char *query);
    //析构查询窗口
    ~ZYQuery3(void);
    //鼠标命令处理函数
    void MenuCommand(IZYFrame *frame,int mcid);
    //反转菜单项状态
    void ToggleState(char *name,int mcid);
    //更新菜单项状态
    void UpdateState(char *name,int mcid);
    //获取菜单项状态
    bool GetState(char *name);
    //复制字符串到剪贴板
    void CopyToClipboard(char *buf);
    //执行SQL语句
    void ExecuteSQLString(char *sql);
    //窗体撤销处理函数
    void OnDestroy(IZYFrame *frame);
    //窗体关闭处理函数
    void OnClose(IZYFrame *frame);
private:
    //窗体-查询窗口
    IZYFrame *frame;
    //菜单-查询窗口菜单
    HMENU mainMenu;
    //增强编辑-查询语句
    IZYEditEx *editExQuery;
    //增强编辑-查询结果
    IZYEditEx *editExResult;
    //数据库路径
    char path[1000];
    //查询文件名
    char query[1000];
    //数据库对象
    IZYDatabase *database;
    //数据库窗口
    ZYQuery2 *query2;
    //执行结果
    IZYTable *result;
};

//表格窗口
class ZYQuery4:public ZYSubQuery,public IZYMenuListener
{
public:
    //构造表格窗口
    ZYQuery4(char *path,IZYDatabase *database,char *table_name);
    //析构表格窗口
    ~ZYQuery4(void);
    //鼠标命令处理函数
    void MenuCommand(IZYFrame *frame,int mcid);
private:
    //窗体-表格窗口
    IZYFrame *frame;
    //菜单-表格窗口菜单
    HMENU mainMenu;
    //表格控件-表格
    IZYGrid *gridTable;
    //数据库路径
    char path[1000];
    //表格名称
    char table_name[1000];
    //表格对象
    IZYTable *table;
    //数据库对象
    IZYDatabase *database;
};
