//-------------------------------------------------------------------
//���¹�ϵ�����ݿ����ϵͳ ����:����
//-------------------------------------------------------------------

//����32λWindows API��
#include <windows.h>

//����C++���������
#include <iostream>

//����C++�ļ���
#include <fstream>

//����C++�����㷨��
#include <algorithm>

//����C++����������
#include <vector>

//����洢����(ZYRDB)API��
#include <ZYRDB.HPP>

//�����ѯ����(ZYSQL)API��
#include <ZYSQL.HPP>

//�����������(ZYGUI)API��
#include <ZYGUI.HPP>

//������Դͷ�ļ�
#include "resource.h"

//Ӧ��C++��׼�����ƿռ�(std)
using namespace std;

//��ɫ1
#define BLUE_COLOR_1 0x00E0A0A0

//��ɫ2
#define BLUE_COLOR_2 0x00FFD2D2

//��ɫ3
#define BLUE_COLOR_3 0x00FFA0A0

//��ɫ4
#define BLUE_COLOR_4 0x00FFF5F5

//ϵͳ�������ݿ�
extern IZYDatabase *configration;

//����ͼ��
extern IZYBitmap *bitmapTable;

//����ͼ��
extern IZYBitmap *bitmapIndex;

//��ѯͼ��
extern IZYBitmap *bitmapQuery;

//�½�ͼ��
extern IZYBitmap *bitmapCreateItem;

//ˮƽλͼ��
extern IZYBitmap *bitmapBarH;

//��ֱλͼ��
extern IZYBitmap *bitmapBarV;

//����ͼƬ1
extern IZYBitmap *bitmapBack1;

//����ͼƬ2
extern IZYBitmap *bitmapBack2;

//��������λͼ
void LoadAllBitmaps(void);

//���ݿ��Ӵ���
class ZYSubQuery
{
public:
    //�������ݿ��Ӵ���
    virtual ~ZYSubQuery(void);
};

//ϵͳ������
class ZYQuery1:public IZYMenuListener,public IZYActionListener,public IZYFrameListener
{
public:
    //����ϵͳ������
    ZYQuery1(void);
    //����ϵͳ������
    ~ZYQuery1(void);
    //ִ����Ϣѭ��
    int MsgLoop(void);
    //������������
    void MenuCommand(IZYFrame *frame,int mcid);
    //��ť������������
    void ActionPerformed(IZYControl *control);
    //���峷����������
    void OnDestroy(IZYFrame *frame);
    //����رմ�������
    void OnClose(IZYFrame *frame);
private:
    //����-ϵͳ������
    IZYFrame *frame;
    //�˵�-ϵͳ�����ڲ˵�
    HMENU mainMenu;
    //��̬�ı���-ϵͳ�����ڱ���
    IZYStaticText *staticTextCaption;
    //��̬�ı���-���ݿ�·��
    IZYStaticText *staticTextPath;
    //��̬�ı���-���ݿ�����
    IZYStaticText *staticTextName;
    //��̬�ı���-�û���
    IZYStaticText *staticTextUserid;
    //��̬�ı���-����
    IZYStaticText *staticTextPassword;
    //�༭��-���ݿ�·��
    IZYEdit *editPath;
    //�༭��-���ݿ�����
    IZYEdit *editName;
    //�༭��-�û���
    IZYEdit *editUserid;
    //�༭��-����
    IZYEdit *editPassword;
    //��ť-��½
    IZYButton *buttonOK;
    //��ť-ȡ��
    IZYButton *buttonCancel;
    //����-�Ӵ���
    vector<ZYSubQuery *> *subQuerys;
};

//���ݿⴰ��
class ZYQuery2:public ZYSubQuery,public IZYMenuListener,public IZYMouseListener,public IZYFrameListener
{
public:
    //�������ݿⴰ��
    ZYQuery2(char *path,IZYDatabase *database);
    //�������ݿⴰ��
    ~ZYQuery2(void);
    //������������
    void MenuCommand(IZYFrame *frame,int mcid);
    //��갴�´�������
    void MouseDown(IZYControl *control,int mx,int my);
    //����ƶ���������
    void MouseMove(IZYControl *control,int mx,int my);
    //��굯��������
    void MouseUp(IZYControl *control,int mx,int my);
    //��������������
    void MouseWheel(IZYControl *control,int mw);
    //���˫����������
    void MouseDoubleClick(IZYControl *control,int mx,int my);
    //���峷����������
    void OnDestroy(IZYFrame *frame);
    //����رմ�������
    void OnClose(IZYFrame *frame);
    //���Ե��Ƿ��ھ�����
    bool PointInRect(int p_x,int p_y,int x,int y,int width,int height);
    //���Ե��Ƿ�������������
    bool PointInPanelBar(int p_x,int p_y,int loc);
    //���Ե��Ƿ������������
    bool PointInItemBar(int p_x,int p_y,int loc);
    //��������������λ��
    void SetPanelBarLoc(IZYStaticPicture *panelBar,int loc);
    //�������������λ��
    void SetItemBarLoc(IZYStaticPicture *itemBar,int loc);
    //�л����ѡ��
    void AlterPanelItem(int panelItem);
    //������
    void AddItem(char *title,int type);
    //�����
    void ClearItems(void);
    //�½���ѯ
    ZYSubQuery *NewQuery3(void);
    //�½���
    void NewItem(int type);
    //����
    void OpenItem(int type,char *name);
    //����ȫ������
    void AddTables(void);
    //����ȫ������
    void AddIndexs(void);
    //����ȫ����ѯ
    void AddQuerys(void);
    //ɾ����ѯ
    void DeleteQuery(char *name);
    //ɾ����ǰѡ����
    void DeleteSelectedItem(int type);
private:
    //����-���ݿⴰ��
    IZYFrame *frame;
    //�˵�-���ݿⴰ�ڲ˵�
    HMENU mainMenu;
    //��̬ͼƬ��-���ݿ����
    IZYStaticPicture *staticPicturePanel;
    //��̬ͼƬ��-���̶���
    IZYStaticPicture *staticPicturePanelBar1;
    //��̬ͼƬ��-��帡����
    IZYStaticPicture *staticPicturePanelBar2;
    //��̬ͼƬ��-����
    IZYStaticPicture *staticPictureTable;
    //��̬ͼƬ��-����
    IZYStaticPicture *staticPictureIndex;
    //��̬ͼƬ��-��ѯ
    IZYStaticPicture *staticPictureQuery;
    //��̬ͼƬ��-��̶���
    IZYStaticPicture *staticPictureItemBar;
    //��̬ͼƬ��-��������
    IZYStaticPicture *staticPictureCreateItem;
    //��̬ͼƬ��-��
    vector<IZYStaticPicture *> *staticPictureItems;
    //��̬�ı���-����
    IZYStaticText *staticTextTable;
    //��̬�ı���-����
    IZYStaticText *staticTextIndex;
    //��̬�ı���-��ѯ
    IZYStaticText *staticTextQuery;
    //��̬�ı���-������
    IZYStaticText *staticTextCreateItem;
    //��̬�ı���-��
    vector<IZYStaticText *> *staticTextItems;
    //���ݿ�·��
    char path[1000];
    //���ݿ����
    IZYDatabase *database;
    //��ǰѡ����Ŀ
    int leftSelect;
    //��ǰ������Ŀ
    int leftItem;
    //��ǰѡ����Ŀ
    int rightSelect;
    //��갴��״̬
    bool mouseDown;
    //����-�Ӵ���
    vector<ZYSubQuery *> *subQuerys;
};

//��ѯ����
class ZYQuery3:public ZYSubQuery,public IZYMenuListener,public IZYFrameListener
{
public:
    //�����ѯ����
    ZYQuery3(ZYQuery2 *query2,char *path,IZYDatabase *database,char *query);
    //������ѯ����
    ~ZYQuery3(void);
    //������������
    void MenuCommand(IZYFrame *frame,int mcid);
    //��ת�˵���״̬
    void ToggleState(char *name,int mcid);
    //���²˵���״̬
    void UpdateState(char *name,int mcid);
    //��ȡ�˵���״̬
    bool GetState(char *name);
    //�����ַ�����������
    void CopyToClipboard(char *buf);
    //ִ��SQL���
    void ExecuteSQLString(char *sql);
    //���峷����������
    void OnDestroy(IZYFrame *frame);
    //����رմ�������
    void OnClose(IZYFrame *frame);
private:
    //����-��ѯ����
    IZYFrame *frame;
    //�˵�-��ѯ���ڲ˵�
    HMENU mainMenu;
    //��ǿ�༭-��ѯ���
    IZYEditEx *editExQuery;
    //��ǿ�༭-��ѯ���
    IZYEditEx *editExResult;
    //���ݿ�·��
    char path[1000];
    //��ѯ�ļ���
    char query[1000];
    //���ݿ����
    IZYDatabase *database;
    //���ݿⴰ��
    ZYQuery2 *query2;
    //ִ�н��
    IZYTable *result;
};

//���񴰿�
class ZYQuery4:public ZYSubQuery,public IZYMenuListener
{
public:
    //������񴰿�
    ZYQuery4(char *path,IZYDatabase *database,char *table_name);
    //�������񴰿�
    ~ZYQuery4(void);
    //������������
    void MenuCommand(IZYFrame *frame,int mcid);
private:
    //����-���񴰿�
    IZYFrame *frame;
    //�˵�-���񴰿ڲ˵�
    HMENU mainMenu;
    //����ؼ�-����
    IZYGrid *gridTable;
    //���ݿ�·��
    char path[1000];
    //��������
    char table_name[1000];
    //�������
    IZYTable *table;
    //���ݿ����
    IZYDatabase *database;
};