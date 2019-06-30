//-------------------------------------------------------------------
//���¹�ϵ�����ݿ����ϵͳ-��ѯ���� ����:����
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//�����ѯ����
ZYQuery3::ZYQuery3(ZYQuery2 *query2,char *path,IZYDatabase *database,char *query)
{
    //��ѯ���ڱ���
    char title[1000];

    //��ѯ�ļ���
    char filename[1000];

    //Դ�����ı�
    char *source=NULL;

    //��ʼ��:���ݿⴰ��
    this->query2=query2;

    //��ʼ��:���ݿ�·��
    strcpy(this->path,path);

    //��ʼ��:��ѯ�ļ���
    strcpy(this->query,query);

    //��ʼ��:���ݿ����
    this->database=database;

    //��ʼ��:ִ�н��
    this->result=NULL;

    //��ʼ��:���ɴ��ڱ���
    wsprintf(title,"��ѯ:%s",query);

    //��ʼ��:����-��ѯ����
    frame=IZYFrame_Create(180,180,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU3);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);

    //��ʼ��:��ǿ�༭-��ѯ���
    editExQuery=IZYEditEx_Create("");
    editExQuery->ToControl()->SetLocSize(5,5,677,244);
    editExQuery->SetCheckSql(true);
    editExQuery->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    editExQuery->GetScrollBarH()->SetBarBitmap(bitmapBarH);
    frame->AddControl(editExQuery->ToControl());

    //��ʼ��:��ǿ�༭-��ѯ���
    editExResult=IZYEditEx_Create("");
    editExResult->ToControl()->SetLocSize(5,254,677,244);
    editExResult->SetReadOnlyEdit(true);
    editExResult->SetTextColor(0x000000FF);
    editExResult->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    editExResult->GetScrollBarH()->SetBarBitmap(bitmapBarH);
    frame->AddControl(editExResult->ToControl());

    //��ʼ��:���ò�ѯ���Ϊ������
    editExQuery->SetRelativeOutput(editExResult);
    editExQuery->SetRelativeDatabase(database);

    //��ʼ��:����Դ����
    wsprintf(filename,"%s\\%s.sql",path,query);
    source=IZYTextFile_LoadText(filename);
    if(source==NULL)
    {
        source="";
    }

    //��ʼ��:���Դ����
    editExQuery->SetEditText(source);
    editExQuery->SetModified(false);
    editExQuery->ToControl()->SetFocus(true);

    //��ʼ��:�ͷ�Դ����
    if(source!=NULL)
    {
        IZYTextFile_FreeText(source);
    }

    //��ʼ��:���²˵���״̬
    UpdateState("�﷨���",IDM_QUERY3_SYNTAX_CHECK);
    if(GetState("�﷨���")==true)
    {
        editExQuery->SetCheckSql(true);
    }
    else
    {
        editExQuery->SetCheckSql(false);
        editExResult->SetEditText("");
    }
    UpdateState("���ܸ�֪",IDM_QUERY3_SYNTAX_SENSE);
    if(GetState("���ܸ�֪")==true)
    {
        editExQuery->SetRelativeDatabase(database);
    }
    else
    {
        editExQuery->SetRelativeDatabase(NULL);
    }
    UpdateState("Դ����",IDM_QUERY3_OPTION_SOURCE);
    UpdateState("�ʷ����",IDM_QUERY3_OPTION_MATCH);
    UpdateState("�����﷨��",IDM_QUERY3_OPTION_AST);
    UpdateState("�Ż��﷨��",IDM_QUERY3_OPTION_OST);
    UpdateState("Դ����",IDM_QUERY3_OPTION_TABLES);
    UpdateState("��ѯ��ʱ",IDM_QUERY3_OPTION_TIME);
    UpdateState("��ѯ���",IDM_QUERY3_OPTION_RESULT);
}

//������ѯ����
ZYQuery3::~ZYQuery3(void)
{
    frame->Close();

    IZYFrame_Release(frame);
}

//�����ַ�����������
void ZYQuery3::CopyToClipboard(char *buf)
{
    IZYClipboard *clipboard=IZYClipboard_Create(frame);

    clipboard->CopyText(buf);

    IZYClipboard_Release(clipboard);
}

//������������
void ZYQuery3::MenuCommand(IZYFrame *frame,int mcid)
{
    char buf[100000];   
    char filename[1000];
    char resultName[1000];

    //�˵�:�ļ�-�ر�
    if(mcid==IDM_QUERY3_FILE_CLOSE)
    {
        OnClose(frame);
        frame->Close();
    }
    //�˵�:ִ��-ִ��
    else if(mcid==IDM_QUERY3_EXECUTE_EXECUTE)
    {
        buf[0]='\0';

        editExQuery->GetEditText(buf,100000);

        ExecuteSQLString(buf);
    }
    //�˵�:ִ��-���
    else if(mcid==IDM_QUERY3_EXECUTE_RESULT)
    {
        if(result!=NULL)
        {
            result->GetName(resultName);

            query2->OpenItem(0,resultName);
        }
    }           
    //�˵�:�ļ�-����
    else if(mcid==IDM_QUERY3_FILE_SAVE)
    {
        wsprintf(filename,"%s\\%s.sql",path,query);

        editExQuery->GetEditText(buf,100000);

        IZYTextFile_SaveText(filename,buf);

        editExQuery->SetModified(false);
    }
    //�˵�:�༭-����
    else if(mcid==IDM_QUERY3_EDIT_CUT)
    {
        editExQuery->GetSelectedText(buf,100000);

        CopyToClipboard(buf);

        editExQuery->SetSelectedText("");

        editExQuery->ToControl()->SetFocus(true);
    }
    //�˵�:�༭-����
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
    //�˵�:�༭-ճ��
    else if(mcid==IDM_QUERY3_EDIT_PASTE)
    {
        IZYClipboard *clipboard=IZYClipboard_Create(frame);

        char *s=clipboard->PasteText();

        IZYClipboard_Release(clipboard);

        editExQuery->SetSelectedText(s);

        editExQuery->ToControl()->SetFocus(true);
    }
    //�˵�:�﷨-�﷨���
    else if(mcid==IDM_QUERY3_SYNTAX_CHECK)
    {
        ToggleState("�﷨���",mcid);
        if(GetState("�﷨���")==true)
        {
            editExQuery->SetCheckSql(true);
        }
        else
        {
            editExQuery->SetCheckSql(false);
            editExResult->SetEditText("�﷨��鹦�ܹر�...");
        }
        frame->Invalidate();
    }
    //�˵�:�﷨-���ܸ�֪
    else if(mcid==IDM_QUERY3_SYNTAX_SENSE)
    {
        ToggleState("���ܸ�֪",mcid);
        if(GetState("���ܸ�֪")==true)
        {
            editExQuery->SetRelativeDatabase(database);
            editExResult->SetEditText("���ܸ�֪���ܿ���...");
        }
        else
        {
            editExQuery->SetRelativeDatabase(NULL);
            editExResult->SetEditText("���ܸ�֪���ܹر�...");
        }
        frame->Invalidate();
    }
    //�˵�:ѡ��-Դ����
    else if(mcid==IDM_QUERY3_OPTION_SOURCE)
    {
        ToggleState("Դ����",mcid);
    }
    //�˵�:ѡ��-�ʷ����
    else if(mcid==IDM_QUERY3_OPTION_MATCH)
    {
        ToggleState("�ʷ����",mcid);
    }
    //�˵�:ѡ��-�����﷨��
    else if(mcid==IDM_QUERY3_OPTION_AST)
    {
        ToggleState("�����﷨��",mcid);
    }
    //�˵�:ѡ��-�Ż��﷨��
    else if(mcid==IDM_QUERY3_OPTION_OST)
    {
        ToggleState("�Ż��﷨��",mcid);
    }
    //�˵�:ѡ��-Դ����
    else if(mcid==IDM_QUERY3_OPTION_TABLES)
    {
        ToggleState("Դ����",mcid);
    }
    //�˵�:ѡ��-��ѯ��ʱ
    else if(mcid==IDM_QUERY3_OPTION_TIME)
    {
        ToggleState("��ѯ��ʱ",mcid);
    }
    //�˵�:ѡ��-��ѯ���
    else if(mcid==IDM_QUERY3_OPTION_RESULT)
    {
        ToggleState("��ѯ���",mcid);
    }
}

//��ת�˵���״̬
void ZYQuery3::ToggleState(char *name,int mcid)
{
    bool state=true;
    IZYTable *table=configration->GetTableI("���ѡ��");
    int j1=table->FindColumn("����");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"״̬",&state);
    state=!state;
    table->SetData(i1,"״̬",&state);
    if(state==true)
    {
        ::CheckMenuItem(mainMenu,mcid,MF_CHECKED);
    }
    else
    {
        ::CheckMenuItem(mainMenu,mcid,MF_UNCHECKED);
    }
}

//���²˵���״̬
void ZYQuery3::UpdateState(char *name,int mcid)
{
    bool state=false;
    IZYTable *table=configration->GetTableI("���ѡ��");
    int j1=table->FindColumn("����");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"״̬",&state);
    if(state==true)
    {
        ::CheckMenuItem(mainMenu,mcid,MF_CHECKED);
    }
    else
    {
        ::CheckMenuItem(mainMenu,mcid,MF_UNCHECKED);
    }
}

//��ȡ�˵���״̬
bool ZYQuery3::GetState(char *name)
{
    bool state=false;
    IZYTable *table=configration->GetTableI("���ѡ��");
    int j1=table->FindColumn("����");
    int i1=table->FindRow(j1,name);
    table->GetData(i1,"״̬",&state);
    return state;
}

//ִ��SQL���
void ZYQuery3::ExecuteSQLString(char *sql)
{
    char listingFile[1000];
   
    if(database!=NULL)
    {
        wsprintf(listingFile,"%s\\listing.lst",path);

        IZYCompiler *compiler=IZYCompiler_Create(listingFile,database);

        compiler->SetDisplaySource(GetState("Դ����"));

        compiler->SetDisplayMatch(GetState("�ʷ����"));

        compiler->SetDisplayAST(GetState("�����﷨��"));

        compiler->SetDisplayTables(GetState("Դ����"));

        compiler->SetDisplayOST(GetState("�Ż��﷨��"));

        compiler->SetDisplayTime(GetState("��ѯ��ʱ"));

        compiler->SetDisplayResult(GetState("��ѯ���"));

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

//���峷����������
void ZYQuery3::OnDestroy(IZYFrame *frame)
{
}

//����رմ�������
void ZYQuery3::OnClose(IZYFrame *frame)
{
    char tip[1000];
    int mb_ret;
    char buf[100000];
    char filename[1000];

    if(editExQuery->GetModified()==true)
    {
        wsprintf(tip,"��ѯ\'%s\'���޸�,�Ƿ񱣴棿",query);
        mb_ret=MessageBox(frame->GetWindowHandle(),tip,"��ʾ",MB_YESNO|MB_ICONQUESTION);
        if(mb_ret==6)
        {
            wsprintf(filename,"%s\\%s.sql",path,query);

            editExQuery->GetEditText(buf,100000);

            IZYTextFile_SaveText(filename,buf);
        }
    }
}