//-------------------------------------------------------------------
//���¹�ϵ�����ݿ����ϵͳ-ϵͳ��½���� ����:����
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//����ͼ��
IZYBitmap *bitmapTable;

//����ͼ��
IZYBitmap *bitmapIndex;

//��ѯͼ��
IZYBitmap *bitmapQuery;

//�½�ͼ��
IZYBitmap *bitmapCreateItem;

//ˮƽλͼ��
IZYBitmap *bitmapBarH;

//��ֱλͼ��
IZYBitmap *bitmapBarV;

//ͼƬ-finger.bmp
IZYBitmap *bitmapFinger;

//ͼƬ-jazz.bmp
IZYBitmap *bitmapJazz;

//����ͼƬ1
IZYBitmap *bitmapBack1;

//����ͼƬ2
IZYBitmap *bitmapBack2;

//ϵͳ�������ݿ�
IZYDatabase *configration=NULL;

//ϵͳ��ڵ�
int Main(void)
{
    int ret;

    LoadAllBitmaps();

    configration=IZYDatabase_Open("ϵͳ���ݿ�","","","");

    ZYQuery1 *query1=new ZYQuery1();
    
    ret=query1->MsgLoop();

    delete query1;

    IZYDatabase_Close(configration);

    return ret;
}

//��������λͼ
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

//����ϵͳ������
ZYQuery1::ZYQuery1(void)
{
    int offset_x=100,offset_y=50;

    //��ʼ��:����-ϵͳ������
    frame=IZYFrame_Create(20,20,698,558,"���¹�ϵ�����ݿ����ϵͳ V1.0");
    mainMenu=frame->LoadMenu(IDR_MENU1);
    frame->SetBitmap(bitmapBack1);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);
    frame->SetFrameListener(this);

    //��ʼ��:��̬�ı���-ϵͳ�����ڱ���
    staticTextCaption=IZYStaticText_Create("���¹�ϵ�����ݿ����ϵͳ V1.0");
    staticTextCaption->ToControl()->SetLocSize(118+offset_x,44+offset_y,300,22);
    frame->AddControl(staticTextCaption->ToControl());

    //��ʼ��:��̬�ı���-���ݿ�·��
    staticTextPath=IZYStaticText_Create("���ݿ�:");
    staticTextPath->ToControl()->SetLocSize(47+offset_x,80+offset_y,100,22);
    frame->AddControl(staticTextPath->ToControl());

    //��ʼ��:��̬�ı���-���ݿ�����
    staticTextName=IZYStaticText_Create("  ����:");
    staticTextName->ToControl()->SetLocSize(47+offset_x,110+offset_y,100,22);
    frame->AddControl(staticTextName->ToControl());

    //��ʼ��:��̬�ı���-�û���
    staticTextUserid=IZYStaticText_Create("�û���:");
    staticTextUserid->ToControl()->SetLocSize(47+offset_x,140+offset_y,100,22);
    frame->AddControl(staticTextUserid->ToControl());

    //��ʼ��:��̬�ı���-����
    staticTextPassword=IZYStaticText_Create("  ����:");
    staticTextPassword->ToControl()->SetLocSize(47+offset_x,170+offset_y,100,22);
    frame->AddControl(staticTextPassword->ToControl());

    //��ʼ��:�༭��-���ݿ�·��
    editPath=IZYEdit_Create("���ķ�ʽ�������ݿ�");
    editPath->ToControl()->SetLocSize(118+offset_x,80+offset_y,267,22);
    frame->AddControl(editPath->ToControl());

    //��ʼ��:�༭��-���ݿ�����
    editName=IZYEdit_Create("ѧ��ѡ�����ݿ�");
    editName->ToControl()->SetLocSize(118+offset_x,110+offset_y,267,22);
    frame->AddControl(editName->ToControl());

    //��ʼ��:�༭��-�û���
    editUserid=IZYEdit_Create("ϵͳ����Ա");
    editUserid->ToControl()->SetLocSize(118+offset_x,140+offset_y,267,22);
    frame->AddControl(editUserid->ToControl());

    //��ʼ��:�༭��-����
    editPassword=IZYEdit_Create("672250695");
    editPassword->SetPasswordEdit(true);
    editPassword->ToControl()->SetLocSize(118+offset_x,170+offset_y,267,22);
    frame->AddControl(editPassword->ToControl());

    //��ʼ��:��ť-��½
    buttonOK=IZYButton_Create("��½");
    buttonOK->ToControl()->SetLocSize(118+offset_x,210+offset_y,88,22);
    buttonOK->ToControl()->SetBackColor(BLUE_COLOR_2);
    buttonOK->SetActionListener(this);
    frame->AddControl(buttonOK->ToControl());

    //��ʼ��:��ť-ȡ��
    buttonCancel=IZYButton_Create("ȡ��");
    buttonCancel->ToControl()->SetLocSize(216+offset_x,210+offset_y,88,22);
    buttonCancel->ToControl()->SetBackColor(BLUE_COLOR_2);
    buttonCancel->SetActionListener(this);
    frame->AddControl(buttonCancel->ToControl());

    //��ʼ��:����-�Ӵ���
    subQuerys=new vector<ZYSubQuery *>();
}

//����ϵͳ������
ZYQuery1::~ZYQuery1(void)
{
    IZYFrame_Release(frame);
}

//ִ����Ϣѭ��
int ZYQuery1::MsgLoop(void)
{
    return frame->MsgLoop();
}

//������������
void ZYQuery1::MenuCommand(IZYFrame *frame,int mcid)
{
    //�˵�:�ļ�-�ر�
    if(mcid==IDM_QUERY1_FILE_EXIT)
    {
        frame->Close();
    }
}

//��ť������������
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
            wsprintf(tip,"���ݿ�:\'%s\'������,�Ƿ񴴽���",path);
            mb_ret=MessageBox(frame->GetWindowHandle(),tip,"��ʾ",MB_YESNO|MB_ICONQUESTION);
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
            wsprintf(tip,"���ݿ�:\'%s\'�Ѵ򿪡�",path);
            MessageBox(frame->GetWindowHandle(),tip,"��ʾ",MB_OK|MB_ICONINFORMATION);
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
            wsprintf(tip,"�������ݿ����ơ��û����������Ƿ���ȷ��");
            MessageBox(frame->GetWindowHandle(),tip,"��ʾ",MB_OK|MB_ICONINFORMATION);
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

//���峷����������
void ZYQuery1::OnDestroy(IZYFrame *frame)
{
}

//����رմ�������
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