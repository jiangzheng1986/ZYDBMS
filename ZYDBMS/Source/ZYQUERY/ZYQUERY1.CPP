//-------------------------------------------------------------------
//逐月关系型数据库管理系统-系统登陆窗口 作者:蒋政
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//表格图标
IZYBitmap *bitmapTable;

//索引图标
IZYBitmap *bitmapIndex;

//查询图标
IZYBitmap *bitmapQuery;

//新建图标
IZYBitmap *bitmapCreateItem;

//水平位图条
IZYBitmap *bitmapBarH;

//垂直位图条
IZYBitmap *bitmapBarV;

//图片-finger.bmp
IZYBitmap *bitmapFinger;

//图片-jazz.bmp
IZYBitmap *bitmapJazz;

//背景图片1
IZYBitmap *bitmapBack1;

//背景图片2
IZYBitmap *bitmapBack2;

//系统配置数据库
IZYDatabase *configration=NULL;

//系统入口点
int Main(void)
{
    int ret;

    LoadAllBitmaps();

    configration=IZYDatabase_Open("系统数据库","","","");

    ZYQuery1 *query1=new ZYQuery1();
    
    ret=query1->MsgLoop();

    delete query1;

    IZYDatabase_Close(configration);

    return ret;
}

//加载所有位图
void LoadAllBitmaps(void)
{
    bitmapFinger=IZYBitmap_Create(IDB_BITMAP_FINGER);

    bitmapJazz=IZYBitmap_Create(IDB_BITMAP_JAZZ);

    bitmapTable=IZYBitmap_Create(IDB_BITMAP1);

    bitmapIndex=IZYBitmap_Create(IDB_BITMAP2);

    bitmapQuery=IZYBitmap_Create(IDB_BITMAP3);

    bitmapCreateItem=IZYBitmap_Create(IDB_BITMAP4);

    bitmapBarH=NULL;//IZYBitmap_Create(IDB_BITMAP5);

    bitmapBarV=NULL;//IZYBitmap_Create(IDB_BITMAP6);

    bitmapBack1=bitmapFinger;

    bitmapBack2=bitmapJazz;
}

//构造系统主窗口
ZYQuery1::ZYQuery1(void)
{
    int offset_x=100,offset_y=50;

    //初始化:窗体-系统主窗口
    frame=IZYFrame_Create(20,20,698,558,"逐月关系型数据库管理系统 V1.0");
    mainMenu=frame->LoadMenu(IDR_MENU1);
    frame->SetBitmap(bitmapBack1);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);

    //初始化:静态文本框-系统主窗口标题
    staticTextCaption=IZYStaticText_Create("逐月关系型数据库管理系统 V1.0");
    staticTextCaption->ToControl()->SetLocSize(118+offset_x,44+offset_y,300,22);
    frame->AddControl(staticTextCaption->ToControl());

    //初始化:静态文本框-数据库路径
    staticTextPath=IZYStaticText_Create("数据库:");
    staticTextPath->ToControl()->SetLocSize(47+offset_x,80+offset_y,100,22);
    frame->AddControl(staticTextPath->ToControl());

    //初始化:静态文本框-数据库名称
    staticTextName=IZYStaticText_Create("  名称:");
    staticTextName->ToControl()->SetLocSize(47+offset_x,110+offset_y,100,22);
    frame->AddControl(staticTextName->ToControl());

    //初始化:静态文本框-用户名
    staticTextUserid=IZYStaticText_Create("用户名:");
    staticTextUserid->ToControl()->SetLocSize(47+offset_x,140+offset_y,100,22);
    frame->AddControl(staticTextUserid->ToControl());

    //初始化:静态文本框-密码
    staticTextPassword=IZYStaticText_Create("  密码:");
    staticTextPassword->ToControl()->SetLocSize(47+offset_x,170+offset_y,100,22);
    frame->AddControl(staticTextPassword->ToControl());

    //初始化:编辑框-数据库路径
    editPath=IZYEdit_Create("中文方式测试数据库");
    editPath->ToControl()->SetLocSize(118+offset_x,80+offset_y,267,22);
    frame->AddControl(editPath->ToControl());

    //初始化:编辑框-数据库名称
    editName=IZYEdit_Create("学生选修数据库");
    editName->ToControl()->SetLocSize(118+offset_x,110+offset_y,267,22);
    frame->AddControl(editName->ToControl());

    //初始化:编辑框-用户名
    editUserid=IZYEdit_Create("系统管理员");
    editUserid->ToControl()->SetLocSize(118+offset_x,140+offset_y,267,22);
    frame->AddControl(editUserid->ToControl());

    //初始化:编辑框-密码
    editPassword=IZYEdit_Create("672250695");
    editPassword->SetPasswordEdit(true);
    editPassword->ToControl()->SetLocSize(118+offset_x,170+offset_y,267,22);
    frame->AddControl(editPassword->ToControl());

    //初始化:按钮-登陆
    buttonOK=IZYButton_Create("登陆");
    buttonOK->ToControl()->SetLocSize(118+offset_x,210+offset_y,88,22);
    buttonOK->ToControl()->SetBackColor(BLUE_COLOR_2);
    buttonOK->SetActionListener(this);
    frame->AddControl(buttonOK->ToControl());

    //初始化:按钮-取消
    buttonCancel=IZYButton_Create("取消");
    buttonCancel->ToControl()->SetLocSize(216+offset_x,210+offset_y,88,22);
    buttonCancel->ToControl()->SetBackColor(BLUE_COLOR_2);
    buttonCancel->SetActionListener(this);
    frame->AddControl(buttonCancel->ToControl());

    //初始化:窗体-子窗口
    subQuerys=new vector<ZYSubQuery *>();
}

//析构系统主窗口
ZYQuery1::~ZYQuery1(void)
{
    IZYFrame_Release(frame);
}

//执行消息循环
int ZYQuery1::MsgLoop(void)
{
    return frame->MsgLoop();
}

//鼠标命令处理函数
void ZYQuery1::MenuCommand(IZYFrame *frame,int mcid)
{
    //菜单:文件-关闭
    if(mcid==IDM_QUERY1_FILE_EXIT)
    {
        frame->Close();
    }
}

//按钮动作处理函数
void ZYQuery1::ActionPerformed(IZYControl *control)
{
    char path[1000];
    char name[1000];
    char userid[1000];
    char password[1000];
    IZYDatabase *database;
    char tip[1000];
    int mb_ret;

    if(control==buttonOK->ToControl())
    {
        editPath->GetEditText(path,1000);
        editName->GetEditText(name,1000);
        editUserid->GetEditText(userid,1000);
        editPassword->GetEditText(password,1000);

        if(lstrcmp(path,"")==0)
        {
            return;
        }
        
        if(!IZYDatabase_Exists(path))
        {
            wsprintf(tip,"数据库:\'%s\'不存在,是否创建？",path);
            mb_ret=MessageBox(frame->GetWindowHandle(),tip,"提示",MB_YESNO|MB_ICONQUESTION);
            if(mb_ret==6)
            {
                database=IZYDatabase_Create(path,name,userid,password);

                if(database!=NULL)
                {
                    ZYQuery2 *query2=new ZYQuery2(path,database);
                    subQuerys->push_back(query2);
                }
            }
            return;
        }

        if(IZYDatabase_Opened(path))
        {
            wsprintf(tip,"数据库:\'%s\'已打开。",path);
            MessageBox(frame->GetWindowHandle(),tip,"提示",MB_OK|MB_ICONINFORMATION);
            return;
        }

        database=IZYDatabase_Open(path,name,userid,password);
        
        if(database!=NULL)
        {
            ZYQuery2 *query2=new ZYQuery2(path,database);
            subQuerys->push_back(query2);
        }
        else
        {
            wsprintf(tip,"请检查数据库名称、用户名、密码是否正确。");
            MessageBox(frame->GetWindowHandle(),tip,"提示",MB_OK|MB_ICONINFORMATION);
        }
    }
    else if(control==buttonCancel->ToControl())
    {
        editPath->SetEditText("");
        editName->SetEditText("");
        editUserid->SetEditText("");
        editPassword->SetEditText("");
    }
}

//窗体撤销处理函数
void ZYQuery1::OnDestroy(IZYFrame *frame)
{
}

//窗体关闭处理函数
void ZYQuery1::OnClose(IZYFrame *frame)
{
    int i;

    if(subQuerys!=NULL)
    {
        for(i=0;i<this->subQuerys->size();i++)
        {
            delete (*subQuerys)[i];
        }

        delete subQuerys;

        subQuerys=NULL;
    }
}