//-------------------------------------------------------------------
//���¹�ϵ�����ݿ����ϵͳ-ϵͳ������ ����:����
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//�������ݿⴰ��
ZYQuery2::ZYQuery2(char *path,IZYDatabase *database)
{
    //�����ַ���
    char title[1000];

    //��ʼ��:���ݿ�·��
    strcpy(this->path,path);

    //��ʼ��:���ݿ����
    this->database=database;

    //��ʼ��:����-���ݿⴰ��
    wsprintf(title,"���ݿ�:%s",path);
    frame=IZYFrame_Create(100,100,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU2);
    frame->SetBitmap(bitmapBack2);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);
    frame->SetMouseListener(this);

    //��ʼ��:��ǰѡ����Ŀ
    leftSelect=0;

    //��ʼ��:��ǰ������Ŀ
    leftItem=-1;

    //��ʼ��:��ǰѡ����Ŀ
    rightSelect=0;

    //��ʼ��:��갴��״̬
    mouseDown=false;

    //��ʼ��:��̬ͼƬ��-���ݿ����
    staticPicturePanel=IZYStaticPicture_Create(NULL);
    staticPicturePanel->ToControl()->SetLocSize(10,10,142,479);
    staticPicturePanel->ToControl()->SetBackColor(BLUE_COLOR_4);
    frame->AddControl(staticPicturePanel->ToControl());

    //��ʼ��:��̬ͼƬ��-���̶���
    staticPicturePanelBar1=IZYStaticPicture_Create(NULL);
    staticPicturePanelBar1->ToControl()->SetLocSize(21,18,120,24);
    staticPicturePanelBar1->ToControl()->SetBackColor(BLUE_COLOR_1);
    frame->AddControl(staticPicturePanelBar1->ToControl());

    //��ʼ��:��̬ͼƬ��-��帡����
    staticPicturePanelBar2=IZYStaticPicture_Create(NULL);
    staticPicturePanelBar2->ToControl()->SetLocSize(21,-100,120,24);
    staticPicturePanelBar2->ToControl()->SetBackColor(BLUE_COLOR_2);
    frame->AddControl(staticPicturePanelBar2->ToControl());

    //��ʼ��:��̬ͼƬ��-����
    staticPictureTable=IZYStaticPicture_Create(NULL);
    staticPictureTable->ToControl()->SetLocSize(45,22,16,16);
    staticPictureTable->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureTable->ToControl()->SetBitmap(bitmapTable);
    frame->AddControl(staticPictureTable->ToControl());

    //��ʼ��:��̬ͼƬ��-����
    staticPictureIndex=IZYStaticPicture_Create(NULL);
    staticPictureIndex->ToControl()->SetLocSize(45,52,16,16);
    staticPictureIndex->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureIndex->ToControl()->SetBitmap(bitmapIndex);
    frame->AddControl(staticPictureIndex->ToControl());

    //��ʼ��:��̬ͼƬ��-��ѯ
    staticPictureQuery=IZYStaticPicture_Create(NULL);
    staticPictureQuery->ToControl()->SetLocSize(45,82,16,16);
    staticPictureQuery->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureQuery->ToControl()->SetBitmap(bitmapQuery);
    frame->AddControl(staticPictureQuery->ToControl());

    //��ʼ��:��̬ͼƬ��-��̶���
    staticPictureItemBar=IZYStaticPicture_Create(NULL);
    staticPictureItemBar->ToControl()->SetLocSize(168,18,200,24);
    frame->AddControl(staticPictureItemBar->ToControl());
    
    //��ʼ��:��̬ͼƬ��-������
    staticPictureCreateItem=IZYStaticPicture_Create(NULL);
    staticPictureCreateItem->ToControl()->SetLocSize(172,22,16,16);
    staticPictureCreateItem->ToControl()->SetBackColor(BLUE_COLOR_3);
    staticPictureCreateItem->ToControl()->SetBitmap(bitmapCreateItem);
    frame->AddControl(staticPictureCreateItem->ToControl());

    //��ʼ��:��̬ͼƬ��-��
    staticPictureItems=new vector<IZYStaticPicture *>();

    //��ʼ��:��̬�ı���-����
    staticTextTable=IZYStaticText_Create("����");
    staticTextTable->ToControl()->SetLocSize(72,20,100,22);
    frame->AddControl(staticTextTable->ToControl());

    //��ʼ��:��̬�ı���-����
    staticTextIndex=IZYStaticText_Create("����");
    staticTextIndex->ToControl()->SetLocSize(72,50,100,22);
    frame->AddControl(staticTextIndex->ToControl());

    //��ʼ��:��̬�ı���-��ѯ
    staticTextQuery=IZYStaticText_Create("��ѯ");
    staticTextQuery->ToControl()->SetLocSize(72,80,100,22);
    frame->AddControl(staticTextQuery->ToControl());

    //��ʼ��:��̬�ı���-������
    staticTextCreateItem=IZYStaticText_Create("�½�����");
    staticTextCreateItem->ToControl()->SetLocSize(197,20,100,22);
    frame->AddControl(staticTextCreateItem->ToControl());

    //��ʼ��:��̬�ı���-��
    staticTextItems=new vector<IZYStaticText *>();

    //��ʼ��:����-�Ӵ���
    subQuerys=new vector<ZYSubQuery *>();

    //��ʼ��:�л���������������
    AlterPanelItem(0);
}

//�������ݿⴰ��
ZYQuery2::~ZYQuery2(void)
{
    frame->Close();
}

//���Ե��Ƿ��ھ�����
bool ZYQuery2::PointInRect(int p_x,int p_y,int x,int y,int width,int height)
{
    return (p_x>=x&&p_x<=x+width&&p_y>=y&&p_y<=y+height);
}

//���Ե��Ƿ�������������
bool ZYQuery2::PointInPanelBar(int p_x,int p_y,int loc)
{
    return PointInRect(p_x,p_y,21,18+loc*30,120,24);
}

//���Ե��Ƿ������������
bool ZYQuery2::PointInItemBar(int p_x,int p_y,int loc)
{
    return PointInRect(p_x,p_y,168,18+loc*30,200,24);
}

//��������������λ��
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

//�������������λ��
void ZYQuery2::SetItemBarLoc(IZYStaticPicture *itemBar,int loc)
{
    itemBar->ToControl()->SetLocSize(168,18+loc*30,200,24);
}

//������
void ZYQuery2::AddItem(char *title,int type)
{
    int loc=staticPictureItems->size();

    //��̬ͼƬ��-��
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

    //��̬�ı���-��
    IZYStaticText *staticTextItem=IZYStaticText_Create(title);
    staticTextItems->push_back(staticTextItem);
    staticTextItem->ToControl()->SetLocSize(197,80+(loc-1)*30,100,22);
    frame->AddControl(staticTextItem->ToControl());
}

//�����
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

//����ȫ������
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

//����ȫ������
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

//����ȫ����ѯ
void ZYQuery2::AddQuerys(void)
{
    int k=0;
    char pattern[MAX_PATH+1];
    char filename[1000];

    HANDLE hFindFile;

    WIN32_FIND_DATA wfd;

    //����ϵͳ����FindFirstFile��FindNextFile�����ļ����е��ļ�

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

//�л����ѡ��
void ZYQuery2::AlterPanelItem(int panelItem)
{
    switch(panelItem)
    {
    case 0:
        staticTextCreateItem->ToControl()->SetTitle("�½�����");
        ClearItems();
        AddTables();
        break;
    case 1:
        staticTextCreateItem->ToControl()->SetTitle("�½�����");
        ClearItems();
        AddIndexs();
        break;
    case 2:
        staticTextCreateItem->ToControl()->SetTitle("�½���ѯ");
        ClearItems();
        AddQuerys();
        break;
    }
}

//��갴�´�������
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

//����ƶ���������
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

//��굯��������
void ZYQuery2::MouseUp(IZYControl *control,int mx,int my)
{
    mouseDown=false;
    MouseMove(control,mx,my);
}

//��������������
void ZYQuery2::MouseWheel(IZYControl *control,int mw)
{
}

//�½���ѯ
ZYSubQuery *ZYQuery2::NewQuery3(void)
{
    int i;
    char filename[1000];
    char queryName[1000];
    ZYSubQuery *subQuery=NULL;

    i=1;
    
    while(true)
    {
        wsprintf(filename,"%s\\��ѯ%02d.sql",path,i);
    
        if(!IZYTextFile_Exists(filename))
        {
            break;
        }
        
        i++;
    }
    
    IZYTextFile_SaveText(filename,"");
    
    wsprintf(queryName,"��ѯ%02d",i);
    
    subQuery=new ZYQuery3(this,path,database,queryName);

    //ˢ�����
    AlterPanelItem(2);

    i=staticTextItems->size();
    SetItemBarLoc(staticPictureItemBar,i);
    rightSelect=i;

    frame->Invalidate();

    return subQuery;
}

//�½���
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

//����
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

//ɾ����ѯ
void ZYQuery2::DeleteQuery(char *name)
{
    char tip[1000];
    int mb_ret;

    //��ѯ�ļ���
    char filename[1000];

    wsprintf(tip,"ȷ��Ҫɾ����ѯ\'%s\'��",name);
    mb_ret=MessageBox(frame->GetWindowHandle(),tip,"��ʾ",MB_YESNO|MB_ICONQUESTION);
    if(mb_ret==6)
    {
        //��ʼ��:����Դ����
        wsprintf(filename,"%s\\%s.sql",path,name);
        IZYTextFile_DeleteText(filename);
    }
}

//ɾ����ǰѡ����
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

//���˫����������
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

//���峷����������
void ZYQuery2::OnDestroy(IZYFrame *frame)
{
}

//����رմ�������
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

//������������
void ZYQuery2::MenuCommand(IZYFrame *frame,int mcid)
{
    //�˵�:�ļ�-�ر�
    if(mcid==IDM_QUERY2_FILE_CLOSE)
    {
        frame->Close();
    }
    //�˵�:��ѯ-�½�
    else if(mcid==IDM_QUERY2_QUERY_NEW)
    {
        NewItem(2);
    }
    //�˵�:��ѯ-ɾ��
    else if(mcid==IDM_QUERY2_QUERY_DELETE)
    {
        DeleteSelectedItem(leftSelect);
    }
}

//�������ݿ��Ӵ���
ZYSubQuery::~ZYSubQuery(void)
{
}
    