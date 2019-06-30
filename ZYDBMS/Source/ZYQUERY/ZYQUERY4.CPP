//-------------------------------------------------------------------
//逐月关系型数据库管理系统-表格窗口 作者:蒋政
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//构造表格窗口
ZYQuery4::ZYQuery4(char *path,IZYDatabase *database,char *table_name)
{
    //表格窗口标题
    char title[1000];

    //初始化:数据库路径
    strcpy(this->path,path);

    //初始化:表格文件名
    strcpy(this->table_name,table_name);

    //初始化:数据库对象
    this->database=database;

    //初始化:表格对象
    table=database->GetTableI(table_name);

    //初始化:生成窗口标题
    wsprintf(title,"表格:%s",table_name);

    //初始化:窗体-表格窗口
    frame=IZYFrame_Create(180,180,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU4);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);

    //初始化:表格控件-表格
    gridTable=IZYGrid_Create(table);
    gridTable->SetLocSize(5,5,12,120,22);
    frame->AddControl(gridTable->ToControl());
    gridTable->SetTitleBitmap(bitmapBarH);
    gridTable->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    gridTable->ToControl()->SetFocus(true);
}

//析构表格窗口
ZYQuery4::~ZYQuery4(void)
{
    frame->Close();

    IZYFrame_Release(frame);
}

//鼠标命令处理函数
void ZYQuery4::MenuCommand(IZYFrame *frame,int mcid)
{
    //菜单:文件-关闭
    if(mcid==IDM_QUERY4_FILE_CLOSE)
    {
        frame->Close();
    }
    //菜单:行-新增
    else if(mcid==IDM_QUERY4_ROW_APPEND)
    {
        gridTable->AppendItem();
    }
    //菜单:行-删除
    else if(mcid==IDM_QUERY4_ROW_DELETE)
    {
        gridTable->DeleteSelectedItem();
    }
}
