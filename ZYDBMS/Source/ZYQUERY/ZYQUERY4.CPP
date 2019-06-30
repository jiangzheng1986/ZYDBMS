//-------------------------------------------------------------------
//���¹�ϵ�����ݿ����ϵͳ-���񴰿� ����:����
//-------------------------------------------------------------------
#include "ZYQUERY1.HPP"

//������񴰿�
ZYQuery4::ZYQuery4(char *path,IZYDatabase *database,char *table_name)
{
    //���񴰿ڱ���
    char title[1000];

    //��ʼ��:���ݿ�·��
    strcpy(this->path,path);

    //��ʼ��:�����ļ���
    strcpy(this->table_name,table_name);

    //��ʼ��:���ݿ����
    this->database=database;

    //��ʼ��:�������
    table=database->GetTableI(table_name);

    //��ʼ��:���ɴ��ڱ���
    wsprintf(title,"����:%s",table_name);

    //��ʼ��:����-���񴰿�
    frame=IZYFrame_Create(180,180,698,558,title);
    mainMenu=frame->LoadMenu(IDR_MENU4);
    frame->SetMainMenu(mainMenu);
    frame->SetMenuListener(this);

    //��ʼ��:����ؼ�-����
    gridTable=IZYGrid_Create(table);
    gridTable->SetLocSize(5,5,12,120,22);
    frame->AddControl(gridTable->ToControl());
    gridTable->SetTitleBitmap(bitmapBarH);
    gridTable->GetScrollBarV()->SetBarBitmap(bitmapBarV);
    gridTable->ToControl()->SetFocus(true);
}

//�������񴰿�
ZYQuery4::~ZYQuery4(void)
{
    frame->Close();

    IZYFrame_Release(frame);
}

//������������
void ZYQuery4::MenuCommand(IZYFrame *frame,int mcid)
{
    //�˵�:�ļ�-�ر�
    if(mcid==IDM_QUERY4_FILE_CLOSE)
    {
        frame->Close();
    }
    //�˵�:��-����
    else if(mcid==IDM_QUERY4_ROW_APPEND)
    {
        gridTable->AppendItem();
    }
    //�˵�:��-ɾ��
    else if(mcid==IDM_QUERY4_ROW_DELETE)
    {
        gridTable->DeleteSelectedItem();
    }
}