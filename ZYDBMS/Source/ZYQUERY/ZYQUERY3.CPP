//-------------------------------------------------------------------
//逐月关系型数据库管理系统-查询窗口 作者:蒋政
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//构造查询窗口
ZYQuery3::ZYQuery3(ZYQuery2 *query2,char *path,IZYDatabase *database,char *query)
{
    //查询窗口标题
    char title[1000];

    //查询文件名
    char filename[1000];

    //源程序文本
    char *source=NULL;

    //初始化:数据库窗口
    this->query2=query2;

    //初始化:数据库路径
    strcpy(this->path,path);

    //初始化:查询文件名
    strcpy(this->query,query);

    //初始化:数据库对象
    this->database=database;

    //初始化:执行结果
    this->result=NULL;

    //初始化:生成窗口标题
    wsprintf(title,"查询:%s",query);

    //初始化:窗体-查询窗口
    frame=IZYFrame_Create(180,180,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU3);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);

    //初始化:增强编辑-查询语句
    editExQuery=IZYEditEx_Create("");
    editExQuery->ToControl()->SetLocSize(5,5,677,244);
    editExQuery->SetCheckSql(true);
    editExQuery->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    editExQuery->GetScrollBarH()->SetBarBitmap(bitmapBarH);
    frame->AddControl(editExQuery->ToControl());

    //初始化:增强编辑-查询结果
    editExResult=IZYEditEx_Create("");
    editExResult->ToControl()->SetLocSize(5,254,677,244);
    editExResult->SetReadOnlyEdit(true);
    editExResult->SetTextColor(0x000000FF);
    editExResult->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    editExResult->GetScrollBarH()->SetBarBitmap(bitmapBarH);
    frame->AddControl(editExResult->ToControl());

    //初始化:设置查询结果为相关输出
    editExQuery->SetRelativeOutput(editExResult);
    editExQuery->SetRelativeDatabase(database);

    //初始化:加载源程序
    wsprintf(filename,"%s\\%s.sql",path,query);
    source=IZYTextFile_LoadText(filename);
    if(source==NULL)
    {
        source="";
    }

    //初始化:检查源程序
    editExQuery->SetEditText(source);
    editExQuery->SetModified(false);
    editExQuery->ToControl()->SetFocus(true);

    //初始化:释放源程序
    if(source!=NULL)
    {
        IZYTextFile_FreeText(source);
    }

    //初始化:更新菜单项状态
    UpdateState("语法检查",IDM_QUERY3_SYNTAX_CHECK);
    if(GetState("语法检查")==true)
    {
        editExQuery->SetCheckSql(true);
    }
    else
    {
        editExQuery->SetCheckSql(false);
        editExResult->SetEditText("");
    }
    UpdateState("智能感知",IDM_QUERY3_SYNTAX_SENSE);
    if(GetState("智能感知")==true)
    {
        editExQuery->SetRelativeDatabase(database);
    }
    else
    {
        editExQuery->SetRelativeDatabase(NULL);
    }
    UpdateState("源程序",IDM_QUERY3_OPTION_SOURCE);
    UpdateState("词法标记",IDM_QUERY3_OPTION_MATCH);
    UpdateState("抽象语法树",IDM_QUERY3_OPTION_AST);
    UpdateState("优化语法树",IDM_QUERY3_OPTION_OST);
    UpdateState("源表格",IDM_QUERY3_OPTION_TABLES);
    UpdateState("查询计时",IDM_QUERY3_OPTION_TIME);
    UpdateState("查询结果",IDM_QUERY3_OPTION_RESULT);
}

//析构查询窗口
ZYQuery3::~ZYQuery3(void)
{
    frame->Close();

    IZYFrame_Release(frame);
}

//复制字符串到剪贴板
void ZYQuery3::CopyToClipboard(char *buf)
{
    IZYClipboard *clipboard=IZYClipboard_Create(frame);

    clipboard->CopyText(buf);

    IZYClipboard_Release(clipboard);
}

//鼠标命令处理函数
void ZYQuery3::MenuCommand(IZYFrame *frame,int mcid)
{
    char buf[100000];   
    char filename[1000];
    char resultName[1000];

    //菜单:文件-关闭
    if(mcid==IDM_QUERY3_FILE_CLOSE)
    {
        OnClose(frame);
        frame->Close();
    }
    //菜单:执行-执行
    else if(mcid==IDM_QUERY3_EXECUTE_EXECUTE)
    {
        buf[0]='\0';

        editExQuery->GetEditText(buf,100000);

        ExecuteSQLString(buf);
    }
    //菜单:执行-结果
    else if(mcid==IDM_QUERY3_EXECUTE_RESULT)
    {
        if(result!=NULL)
        {
            result->GetName(resultName);

            query2->OpenItem(0,resultName);
        }
    }           
    //菜单:文件-保存
    else if(mcid==IDM_QUERY3_FILE_SAVE)
    {
        wsprintf(filename,"%s\\%s.sql",path,query);

        editExQuery->GetEditText(buf,100000);

        IZYTextFile_SaveText(filename,buf);

        editExQuery->SetModified(false);
    }
    //菜单:编辑-剪切
    else if(mcid==IDM_QUERY3_EDIT_CUT)
    {
        editExQuery->GetSelectedText(buf,100000);

        CopyToClipboard(buf);

        editExQuery->SetSelectedText("");

        editExQuery->ToControl()->SetFocus(true);
    }
    //菜单:编辑-复制
    else if(mcid==IDM_QUERY3_EDIT_COPY)
    {
        if(frame->GetFocusControl()==editExResult->ToControl())
        {
            editExResult->GetEditText(buf,100000);
        }
        else
        {
            editExQuery->GetSelectedText(buf,100000);
        }

        CopyToClipboard(buf);
    }
    //菜单:编辑-粘贴
    else if(mcid==IDM_QUERY3_EDIT_PASTE)
    {
        IZYClipboard *clipboard=IZYClipboard_Create(frame);

        char *s=clipboard->PasteText();

        IZYClipboard_Release(clipboard);

        editExQuery->SetSelectedText(s);

        editExQuery->ToControl()->SetFocus(true);
    }
    //菜单:语法-语法检查
    else if(mcid==IDM_QUERY3_SYNTAX_CHECK)
    {
        ToggleState("语法检查",mcid);
        if(GetState("语法检查")==true)
        {
            editExQuery->SetCheckSql(true);
        }
        else
        {
            editExQuery->SetCheckSql(false);
            editExResult->SetEditText("语法检查功能关闭...");
        }
        frame->Invalidate();
    }
    //菜单:语法-智能感知
    else if(mcid==IDM_QUERY3_SYNTAX_SENSE)
    {
        ToggleState("智能感知",mcid);
        if(GetState("智能感知")==true)
        {
            editExQuery->SetRelativeDatabase(database);
            editExResult->SetEditText("智能感知功能开启...");
        }
        else
        {
            editExQuery->SetRelativeDatabase(NULL);
            editExResult->SetEditText("智能感知功能关闭...");
        }
        frame->Invalidate();
    }
    //菜单:选项-源程序
    else if(mcid==IDM_QUERY3_OPTION_SOURCE)
    {
        ToggleState("源程序",mcid);
    }
    //菜单:选项-词法标记
    else if(mcid==IDM_QUERY3_OPTION_MATCH)
    {
        ToggleState("词法标记",mcid);
    }
    //菜单:选项-抽象语法树
    else if(mcid==IDM_QUERY3_OPTION_AST)
    {
        ToggleState("抽象语法树",mcid);
    }
    //菜单:选项-优化语法树
    else if(mcid==IDM_QUERY3_OPTION_OST)
    {
        ToggleState("优化语法树",mcid);
    }
    //菜单:选项-源表格
    else if(mcid==IDM_QUERY3_OPTION_TABLES)
    {
        ToggleState("源表格",mcid);
    }
    //菜单:选项-查询计时
    else if(mcid==IDM_QUERY3_OPTION_TIME)
    {
        ToggleState("查询计时",mcid);
    }
    //菜单:选项-查询结果
    else if(mcid==IDM_QUERY3_OPTION_RESULT)
    {
        ToggleState("查询结果",mcid);
    }
}

//反转菜单项状态
void ZYQuery3::ToggleState(char *name,int mcid)
{
    bool state=true;
    IZYTable *table=configration->GetTableI("输出选项");
    int j1=table->FindColumn("名称");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"状态",&state);
    state=!state;
    table->SetData(i1,"状态",&state);
    if(state==true)
    {
        ::CheckMenuItem(mainMenu,mcid,MF_CHECKED);
    }
    else
    {
        ::CheckMenuItem(mainMenu,mcid,MF_UNCHECKED);
    }
}

//更新菜单项状态
void ZYQuery3::UpdateState(char *name,int mcid)
{
    bool state=false;
    IZYTable *table=configration->GetTableI("输出选项");
    int j1=table->FindColumn("名称");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"状态",&state);
    if(state==true)
    {
        ::CheckMenuItem(mainMenu,mcid,MF_CHECKED);
    }
    else
    {
        ::CheckMenuItem(mainMenu,mcid,MF_UNCHECKED);
    }
}

//获取菜单项状态
bool ZYQuery3::GetState(char *name)
{
    bool state=false;
    IZYTable *table=configration->GetTableI("输出选项");
    int j1=table->FindColumn("名称");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"状态",&state);
    return state;
}

//执行SQL语句
void ZYQuery3::ExecuteSQLString(char *sql)
{
    char listingFile[1000];
   
    if(database!=NULL)
    {
        wsprintf(listingFile,"%s\\listing.lst",path);

        IZYCompiler *compiler=IZYCompiler_Create(listingFile,database);

        compiler->SetDisplaySource(GetState("源程序"));

        compiler->SetDisplayMatch(GetState("词法标记"));

        compiler->SetDisplayAST(GetState("抽象语法树"));

        compiler->SetDisplayTables(GetState("源表格"));

        compiler->SetDisplayOST(GetState("优化语法树"));

        compiler->SetDisplayTime(GetState("查询计时"));

        compiler->SetDisplayResult(GetState("查询结果"));

        compiler->ExecuteSQL(sql);

        result=compiler->GetResultTable();

        IZYCompiler_Release(compiler);

        char *listing=IZYTextFile_LoadText(listingFile);

        listing[lstrlen(listing)-2]='\0';

        editExResult->SetEditText(listing);

        if(listing!=NULL)
        {
            IZYTextFile_FreeText(listing);
        }

        IZYTextFile_DeleteText(listingFile);
    }
}

//窗体撤销处理函数
void ZYQuery3::OnDestroy(IZYFrame *frame)
{
}

//窗体关闭处理函数
void ZYQuery3::OnClose(IZYFrame *frame)
{
    char tip[1000];
    int mb_ret;
    char buf[100000];
    char filename[1000];

    if(editExQuery->GetModified()==true)
    {
        wsprintf(tip,"查询\'%s\'已修改,是否保存？",query);
        mb_ret=MessageBox(frame->GetWindowHandle(),tip,"提示",MB_YESNO|MB_ICONQUESTION);
        if(mb_ret==6)
        {
            wsprintf(filename,"%s\\%s.sql",path,query);

            editExQuery->GetEditText(buf,100000);

            IZYTextFile_SaveText(filename,buf);
        }
    }
}
