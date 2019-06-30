//-------------------------------------------------------------------
//逐月关系型数据库管理系统-系统主窗口 作者:蒋政
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//构造数据库窗口
ZYQuery2::ZYQuery2(char *path,IZYDatabase *database)
{
    //标题字符串
    char title[1000];

    //初始化:数据库路径
    strcpy(this->path,path);

    //初始化:数据库对象
    this->database=database;

    //初始化:窗体-数据库窗口
    wsprintf(title,"数据库:%s",path);
    frame=IZYFrame_Create(100,100,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU2);
    frame->SetBitmap(bitmapBack2);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);
    frame->SetMouseListener(this);

    //初始化:当前选定项目
    leftSelect=0;

    //初始化:当前浮动项目
    leftItem=-1;

    //初始化:当前选定项目
    rightSelect=0;

    //初始化:鼠标按下状态
    mouseDown=false;

    //初始化:静态图片框-数据库面板
    staticPicturePanel=IZYStaticPicture_Create(NULL);
    staticPicturePanel->ToControl()->SetLocSize(10,10,142,479);
    staticPicturePanel->ToControl()->SetBackColor(BLUE_COLOR_4);
    frame->AddControl(staticPicturePanel->ToControl());

    //初始化:静态图片框-面板固定条
    staticPicturePanelBar1=IZYStaticPicture_Create(NULL);
    staticPicturePanelBar1->ToControl()->SetLocSize(21,18,120,24);
    staticPicturePanelBar1->ToControl()->SetBackColor(BLUE_COLOR_1);
    frame->AddControl(staticPicturePanelBar1->ToControl());

    //初始化:静态图片框-面板浮动条
    staticPicturePanelBar2=IZYStaticPicture_Create(NULL);
    staticPicturePanelBar2->ToControl()->SetLocSize(21,-100,120,24);
    staticPicturePanelBar2->ToControl()->SetBackColor(BLUE_COLOR_2);
    frame->AddControl(staticPicturePanelBar2->ToControl());

    //初始化:静态图片框-表格
    staticPictureTable=IZYStaticPicture_Create(NULL);
    staticPictureTable->ToControl()->SetLocSize(45,22,16,16);
    staticPictureTable->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureTable->ToControl()->SetBitmap(bitmapTable);
    frame->AddControl(staticPictureTable->ToControl());

    //初始化:静态图片框-索引
    staticPictureIndex=IZYStaticPicture_Create(NULL);
    staticPictureIndex->ToControl()->SetLocSize(45,52,16,16);
    staticPictureIndex->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureIndex->ToControl()->SetBitmap(bitmapIndex);
    frame->AddControl(staticPictureIndex->ToControl());

    //初始化:静态图片框-查询
    staticPictureQuery=IZYStaticPicture_Create(NULL);
    staticPictureQuery->ToControl()->SetLocSize(45,82,16,16);
    staticPictureQuery->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureQuery->ToControl()->SetBitmap(bitmapQuery);
    frame->AddControl(staticPictureQuery->ToControl());

    //初始化:静态图片框-项固定条
    staticPictureItemBar=IZYStaticPicture_Create(NULL);
    staticPictureItemBar->ToControl()->SetLocSize(168,18,200,24);
    frame->AddControl(staticPictureItemBar->ToControl());
    
    //初始化:静态图片框-创建项
    staticPictureCreateItem=IZYStaticPicture_Create(NULL);
    staticPictureCreateItem->ToControl()->SetLocSize(172,22,16,16);
    staticPictureCreateItem->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureCreateItem->ToControl()->SetBitmap(bitmapCreateItem);
    frame->AddControl(staticPictureCreateItem->ToControl());

    //初始化:静态图片框-项
    staticPictureItems=new vector<IZYStaticPicture *>();

    //初始化:静态文本框-表格
    staticTextTable=IZYStaticText_Create("表格");
    staticTextTable->ToControl()->SetLocSize(72,20,100,22);
    frame->AddControl(staticTextTable->ToControl());

    //初始化:静态文本框-索引
    staticTextIndex=IZYStaticText_Create("索引");
    staticTextIndex->ToControl()->SetLocSize(72,50,100,22);
    frame->AddControl(staticTextIndex->ToControl());

    //初始化:静态文本框-查询
    staticTextQuery=IZYStaticText_Create("查询");
    staticTextQuery->ToControl()->SetLocSize(72,80,100,22);
    frame->AddControl(staticTextQuery->ToControl());

    //初始化:静态文本框-创建项
    staticTextCreateItem=IZYStaticText_Create("新建表格");
    staticTextCreateItem->ToControl()->SetLocSize(197,20,100,22);
    frame->AddControl(staticTextCreateItem->ToControl());

    //初始化:静态文本框-项
    staticTextItems=new vector<IZYStaticText *>();

    //初始化:窗体-子窗口
    subQuerys=new vector<ZYSubQuery *>();

    //初始化:切换到表格面板滚动区
    AlterPanelItem(0);
}

//析构数据库窗口
ZYQuery2::~ZYQuery2(void)
{
    frame->Close();
}

//测试点是否在矩形内
bool ZYQuery2::PointInRect(int p_x,int p_y,int x,int y,int width,int height)
{
    return (p_x>=x&&p_x<=x+width&&p_y>=y&&p_y<=y+height);
}

//测试点是否在面板滚动区内
bool ZYQuery2::PointInPanelBar(int p_x,int p_y,int loc)
{
    return PointInRect(p_x,p_y,21,18+loc*30,120,24);
}

//测试点是否在项滚动区内
bool ZYQuery2::PointInItemBar(int p_x,int p_y,int loc)
{
    return PointInRect(p_x,p_y,168,18+loc*30,200,24);
}

//设置面板滚动条的位置
void ZYQuery2::SetPanelBarLoc(IZYStaticPicture *panelBar,int loc)
{
    if(loc==-1)
    {
        panelBar->ToControl()->SetLocSize(21,-100,120,24);
    }
    else
    {
        panelBar->ToControl()->SetLocSize(21,18+loc*30,120,24);
    }
}

//设置项滚动条的位置
void ZYQuery2::SetItemBarLoc(IZYStaticPicture *itemBar,int loc)
{
    itemBar->ToControl()->SetLocSize(168,18+loc*30,200,24);
}

//增加项
void ZYQuery2::AddItem(char *title,int type)
{
    int loc=staticPictureItems->size();

    //静态图片框-项
    IZYStaticPicture *staticPictureItem=IZYStaticPicture_Create(NULL);
    staticPictureItems->push_back(staticPictureItem);
    staticPictureItem->ToControl()->SetLocSize(172,82+(loc-1)*30,16,16);
    staticPictureItem->ToControl()->SetBackColor(BLUE_COLOR_3);
    switch(type)
    {
    case 0:
        staticPictureItem->ToControl()->SetBitmap(bitmapTable);
        break;
    case 1:
        staticPictureItem->ToControl()->SetBitmap(bitmapIndex);
        break;
    case 2:
        staticPictureItem->ToControl()->SetBitmap(bitmapQuery);
        break;
    }
    frame->AddControl(staticPictureItem->ToControl());

    //静态文本框-项
    IZYStaticText *staticTextItem=IZYStaticText_Create(title);
    staticTextItems->push_back(staticTextItem);
    staticTextItem->ToControl()->SetLocSize(197,80+(loc-1)*30,100,22);
    frame->AddControl(staticTextItem->ToControl());
}

//清空项
void ZYQuery2::ClearItems(void)
{
    int i;

    for(i=0;i<staticPictureItems->size();i++)
    {
        frame->RemoveControl((*staticPictureItems)[i]->ToControl());
    }

    staticPictureItems->clear();
    
    for(i=0;i<staticTextItems->size();i++)
    {
        frame->RemoveControl((*staticTextItems)[i]->ToControl());
    }

    staticTextItems->clear();
}

//增加全部表格
void ZYQuery2::AddTables(void)
{
    int i;
    char name[1000];
    IZYTable *table;
    
    for(i=0;i<database->GetTableCount();i++)
    {
        table=database->GetTableI(i);

        if( table->IsView()==false&&
            table->IsIndex()==false )
        {
            table->GetName(name);

            AddItem(name,0);
        }
    }
}

//增加全部索引
void ZYQuery2::AddIndexs(void)
{
    int i;
    char name[1000];
    IZYTable *table;
    
    for(i=0;i<database->GetTableCount();i++)
    {
        table=database->GetTableI(i);

        if(table->IsIndex()==true)
        {
            table->GetName(name);

            AddItem(name,1);
        }
    }
}

//增加全部查询
void ZYQuery2::AddQuerys(void)
{
    int k=0;
    char pattern[MAX_PATH+1];
    char filename[1000];

    HANDLE hFindFile;

    WIN32_FIND_DATA wfd;

    //调用系统函数FindFirstFile和FindNextFile遍历文件夹中的文件

    sprintf(pattern,"%s\\*.sql",path);

    hFindFile=FindFirstFile(pattern,&wfd);

    if(hFindFile==INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        if((wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)
        {
            for(k=0;wfd.cFileName[k]!='.'&&wfd.cFileName[k]!='\0';k++)
            {
                filename[k]=wfd.cFileName[k];
            }
            filename[k]='\0';
            AddItem(filename,2);
        }
    }
    while(FindNextFile(hFindFile,&wfd));

    FindClose(hFindFile);
}

//切换面板选项
void ZYQuery2::AlterPanelItem(int panelItem)
{
    switch(panelItem)
    {
    case 0:
        staticTextCreateItem->ToControl()->SetTitle("新建表格");
        ClearItems();
        AddTables();
        break;
    case 1:
        staticTextCreateItem->ToControl()->SetTitle("新建索引");
        ClearItems();
        AddIndexs();
        break;
    case 2:
        staticTextCreateItem->ToControl()->SetTitle("新建查询");
        ClearItems();
        AddQuerys();
        break;
    }
}

//鼠标按下处理函数
void ZYQuery2::MouseDown(IZYControl *control,int mx,int my)
{
    int i;

    if(PointInPanelBar(mx,my,0))
    {
        AlterPanelItem(0);
        SetPanelBarLoc(staticPicturePanelBar1,0);
        SetPanelBarLoc(staticPicturePanelBar2,-1);
        SetItemBarLoc(staticPictureItemBar,0);
        leftSelect=0;
        leftItem=-1;
        mouseDown=true;
        frame->Invalidate();
    }
    else if(PointInPanelBar(mx,my,1))
    {
        AlterPanelItem(1);
        SetPanelBarLoc(staticPicturePanelBar1,1);
        SetPanelBarLoc(staticPicturePanelBar2,-1);
        SetItemBarLoc(staticPictureItemBar,0);
        leftSelect=1;
        leftItem=-1;
        mouseDown=true;
        frame->Invalidate();
    }
    else if(PointInPanelBar(mx,my,2))
    {
        AlterPanelItem(2);
        SetPanelBarLoc(staticPicturePanelBar1,2);
        SetPanelBarLoc(staticPicturePanelBar2,-1);
        SetItemBarLoc(staticPictureItemBar,0);
        leftSelect=2;
        leftItem=-1;
        mouseDown=true;
        frame->Invalidate();
    }
    else
    {
        for(i=0;i<=staticPictureItems->size();i++)
        {
            if(PointInItemBar(mx,my,i))
            {
                SetItemBarLoc(staticPictureItemBar,i);
                rightSelect=i;
                mouseDown=true;
                frame->Invalidate();
            }
        }
    }
}

//鼠标移动处理函数
void ZYQuery2::MouseMove(IZYControl *control,int mx,int my)
{
    if(mouseDown)
    {
        SetPanelBarLoc(staticPicturePanelBar2,-1);
        leftItem=-1;
        frame->Invalidate();
        return;
    }

    if(PointInPanelBar(mx,my,0))
    {
        if(leftItem!=0)
        {
            SetPanelBarLoc(staticPicturePanelBar2,0);
            leftItem=0;
            frame->Invalidate();
        }
    }
    else if(PointInPanelBar(mx,my,1))
    {
        if(leftItem!=1)
        {
            SetPanelBarLoc(staticPicturePanelBar2,1);
            leftItem=1;
            frame->Invalidate();
        }
    }
    else if(PointInPanelBar(mx,my,2))
    {
        if(leftItem!=2)
        {
            SetPanelBarLoc(staticPicturePanelBar2,2);
            leftItem=2;
            frame->Invalidate();
        }
    }
    else if(leftItem!=-1)
    {
        SetPanelBarLoc(staticPicturePanelBar2,-1);
        leftItem=-1;
        frame->Invalidate();
    }
}

//鼠标弹起处理函数
void ZYQuery2::MouseUp(IZYControl *control,int mx,int my)
{
    mouseDown=false;
    MouseMove(control,mx,my);
}

//鼠标滚动处理函数
void ZYQuery2::MouseWheel(IZYControl *control,int mw)
{
}

//新建查询
ZYSubQuery *ZYQuery2::NewQuery3(void)
{
    int i;
    char filename[1000];
    char queryName[1000];
    ZYSubQuery *subQuery=NULL;

    i=1;
    
    while(true)
    {
        wsprintf(filename,"%s\\查询%02d.sql",path,i);
    
        if(!IZYTextFile_Exists(filename))
        {
            break;
        }
        
        i++;
    }
    
    IZYTextFile_SaveText(filename,"");
    
    wsprintf(queryName,"查询%02d",i);
    
    subQuery=new ZYQuery3(this,path,database,queryName);

    //刷新面板
    AlterPanelItem(2);

    i=staticTextItems->size();
    SetItemBarLoc(staticPictureItemBar,i);
    rightSelect=i;

    frame->Invalidate();

    return subQuery;
}

//新建项
void ZYQuery2::NewItem(int type)
{
    ZYSubQuery *subQuery=NULL;
    switch(type)
    {
    case 0:
    case 1:
        return;
    case 2:
        subQuery=NewQuery3();
        break;
    }
    subQuerys->push_back(subQuery);
}

//打开项
void ZYQuery2::OpenItem(int type,char *name)
{
    ZYSubQuery *subQuery=NULL;
    switch(type)
    {
    case 0:
    case 1:
        subQuery=new ZYQuery4(path,database,name);
        break;
    case 2:
        subQuery=new ZYQuery3(this,path,database,name);
        break;
    }
    subQuerys->push_back(subQuery);
}

//删除查询
void ZYQuery2::DeleteQuery(char *name)
{
    char tip[1000];
    int mb_ret;

    //查询文件名
    char filename[1000];

    wsprintf(tip,"确定要删除查询\'%s\'吗？",name);
    mb_ret=MessageBox(frame->GetWindowHandle(),tip,"提示",MB_YESNO|MB_ICONQUESTION);
    if(mb_ret==6)
    {
        //初始化:加载源程序
        wsprintf(filename,"%s\\%s.sql",path,name);
        IZYTextFile_DeleteText(filename);
    }
}

//删除当前选定项
void ZYQuery2::DeleteSelectedItem(int type)
{
    char *title;
    int i=rightSelect-1;
    if(i<0||leftSelect!=2)
    {
        return;
    }
    title=(*staticTextItems)[i]->ToControl()->GetTitle();
    switch(type)
    {
    case 0:
    case 1:
        break;
    case 2:
        DeleteQuery(title);
        break;
    }
    SetItemBarLoc(staticPictureItemBar,i);
    rightSelect=i;
    AlterPanelItem(leftSelect);
    frame->Invalidate();
}

//鼠标双击处理函数
void ZYQuery2::MouseDoubleClick(IZYControl *control,int mx,int my)
{
    int i;
    char *title;

    if(PointInItemBar(mx,my,0))
    {
        NewItem(leftSelect);
        return;
    }

    for(i=0;i<staticPictureItems->size();i++)
    {
        if(PointInItemBar(mx,my,i+1))
        {
            title=(*staticTextItems)[i]->ToControl()->GetTitle();
            OpenItem(leftSelect,title);
            return;
        }
    }

    MouseDown(control,mx,my);
}

//窗体撤销处理函数
void ZYQuery2::OnDestroy(IZYFrame *frame)
{
}

//窗体关闭处理函数
void ZYQuery2::OnClose(IZYFrame *frame)
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

    IZYDatabase_Close(database);
}

//鼠标命令处理函数
void ZYQuery2::MenuCommand(IZYFrame *frame,int mcid)
{
    //菜单:文件-关闭
    if(mcid==IDM_QUERY2_FILE_CLOSE)
    {
        frame->Close();
    }
    //菜单:查询-新建
    else if(mcid==IDM_QUERY2_QUERY_NEW)
    {
        NewItem(2);
    }
    //菜单:查询-删除
    else if(mcid==IDM_QUERY2_QUERY_DELETE)
    {
        DeleteSelectedItem(leftSelect);
    }
}

//析构数据库子窗口
ZYSubQuery::~ZYSubQuery(void)
{
}
    