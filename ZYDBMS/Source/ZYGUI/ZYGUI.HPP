//---------------------------------------------------------
//���½�������(ZYGUI) ����:����
//---------------------------------------------------------
#if !defined(ZYGUI_HPP)        //����ZYGUI_HPP_��
#define ZYGUI_HPP              //ʹ��ZYGUI.HPP������1��

#include <ZYRDB.HPP>

//�׽��ֶ���Ϣ
#define WM_NETWORK_EVENT WM_USER+500

//Windows������ڵ�궨��
#define Main(PARAM) APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)

//������
enum EFrameStyle
{
    EFrameStyle_StartUp,        //��������
    EFrameStyle_Normal          //��������
};

//����������
enum EScrollType
{
    EScrollType_Horizontal,     //ˮƽ������
    EScrollType_Vertical        //��ֱ������
};

//���ڶ���
class IZYFrame;

//ͼ�ζ���
class IZYGraphics;

//λͼͼƬ����
class IZYBitmap;

//���������
class IZYClipboard;

//�ؼ�����
class IZYControl;

//��̬�ı��ؼ�����
class IZYStaticText;

//��̬ͼƬ�ؼ�����
class IZYStaticPicture;

//��ť�ؼ�����
class IZYButton;

//��ѡ��ؼ�����
class IZYCheckBox;

//�༭�ؼ�����
class IZYEdit;

//����ؼ�����
class IZYGrid;

//�������ؼ�����
class IZYScrollBar;

//��ǿ�༭����
class IZYEditEx;

//SQL�༭�ؼ�����
class IZYSqlEdit;

//�����¼������ӿ�
class IZYFrameListener
{
public:
    //���峷����������
    virtual void OnDestroy(IZYFrame *frame)=0;
    //����رմ�������
    virtual void OnClose(IZYFrame *frame)=0;
};

//�����¼������ӿ�1
class IZYFrameListener1
{
public:
    //������Ϣ��������
    virtual void OnWindowsMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)=0;
};

//�˵��¼������ӿ�
class IZYMenuListener
{
public:
    //�˵����������
    virtual void MenuCommand(IZYFrame *frame,int mcid)=0;
};

//�����¼������ӿ�
class IZYNetworkListener
{
public:
    //�����¼���������
    virtual void NetworkEvent(IZYFrame *frame,int error,int event,SOCKET socket)=0;
};

//��ť���������ӿ�
class IZYActionListener
{
public:
    //��ť������������
    virtual void ActionPerformed(IZYControl *control)=0;
};

//�������ӿ�
class IZYMouseListener
{
public:
    //��갴�´�������
    virtual void MouseDown(IZYControl *control,int mx,int my)=0;
    //����ƶ���������
    virtual void MouseMove(IZYControl *control,int mx,int my)=0;
    //��굯��������
    virtual void MouseUp(IZYControl *control,int mx,int my)=0;
    //��������������
    virtual void MouseWheel(IZYControl *control,int mw)=0;
    //���˫����������
    virtual void MouseDoubleClick(IZYControl *control,int mx,int my)=0;
};

//���̼����ӿ�
class IZYKeyListener
{
public:
    //���̰��´�������
    virtual void KeyDown(IZYControl *control,int vkey)=0;
    //�ַ����봦������
    virtual void CharInput(IZYControl *control,char ch)=0;
};

//���������ӿ�
class IZYScrollListener
{
public:
    //������������
    virtual void BarScroll(IZYControl *control,int i_base)=0;
};

//�������ڶ���
IZYFrame *IZYFrame_Create(int x,int y,int width,int height,char *title);

//�ͷŴ��ڶ���
void IZYFrame_Release(IZYFrame *frame);

//���ڶ���
class IZYFrame
{
public: 
    //���ز˵�
    virtual HMENU LoadMenu(int rid)=0;
    //�������˵�
    virtual void SetMainMenu(HMENU hMenu)=0;
    //���ӿؼ�
    virtual void AddControl(IZYControl *control)=0;
    //ɾ���ؼ�
    virtual void RemoveControl(IZYControl *control)=0;
    //����λͼͼƬ
    virtual void SetBitmap(IZYBitmap *bitmap)=0;
    //���ñ�����ɫ
    virtual void SetBackColor(COLORREF backColor)=0;
    //��ȡ���ھ��
    virtual HWND GetWindowHandle(void)=0;
    //������Ϣ����
    virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)=0;
    //������Ϣѭ��
    virtual int MsgLoop(void)=0;
    //ˢ�´���
    virtual void Invalidate(void)=0;
    //�˵�����
    virtual void OnMenuCommand(int mcid)=0;
    //���ò˵��¼���������
    virtual void SetMenuListener(IZYMenuListener *menuListener)=0;
    //���ô����¼���������
    virtual void SetFrameListener(IZYFrameListener *frameListener)=0;
    //��������¼���������
    virtual void SetMouseListener(IZYMouseListener *mouseListener)=0;
    //�رմ���
    virtual void Close(void)=0;
    //��ȡ�����Ƿ��ѹر�
    virtual bool GetClosed(void)=0;
    //��ȡ����ؼ�
    virtual IZYControl *GetFocusControl(void)=0;
    //���������¼���������
    virtual void SetNetworkListener(IZYNetworkListener *networkListener)=0;
    //�����Ƿ񿪷�ʽͼ�οⴰ��
    virtual void SetOpenGLWindow(bool openGLWindow)=0;
    //���ô����¼���������1
    virtual void SetFrameListener1(IZYFrameListener1 *frameListener1)=0;
};

//����ͼ�ζ���
IZYGraphics *IZYGraphics_Create(HDC hdc);

//�ͷ�ͼ�ζ���
void IZYGraphics_Release(IZYGraphics *graphics);

//ͼ�ζ���
class IZYGraphics
{
public:
    //��������ƫ��
    virtual void SetXYBase(int x_base,int y_base)=0;
    //����һ����
    virtual void DrawLine(int x1,int y1,int x2,int y2)=0;
    //����һ������
    virtual void DrawRectangle(int x,int y,int w,int h)=0;
    //������
    virtual void FillRectangle(int x,int y,int w,int h,int brush)=0;
    //������1
    virtual void FillRectangle1(int x,int y,int w,int h,COLORREF color)=0;
    //����ı�
    virtual void OutputText(char *buf,int x,int y,int w,int h,int align)=0;
    //���ͼ��
    virtual void DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h)=0;
    //�����ı���ɫ
    virtual void SetTextColor(COLORREF color)=0;
    //����������ɫ
    virtual void SetPenColor(COLORREF color)=0;
    //���û�����ɫ
    virtual void ResetPenColor(void)=0;
};

//����λͼͼƬ����
IZYBitmap *IZYBitmap_Create(int rid);

//�ͷ�λͼͼƬ����
void IZYBitmap_Release(IZYBitmap *bitmap);

//λͼͼƬ����
class IZYBitmap
{
public:
    //��ȡλͼͼƬ�豸���
    virtual HDC GetHdcBitmap(void)=0;
};

//�������������
IZYClipboard *IZYClipboard_Create(IZYFrame *frame);

//�ͷż��������
void IZYClipboard_Release(IZYClipboard *clipboard);

//���������
class IZYClipboard
{
public:
    //��ȡ�������е��ı�
    virtual char *PasteText(void)=0;
    //���ü������е��ı�
    virtual void CopyText(char *text)=0;
};

//�����ؼ�����
IZYControl *IZYControl_Create(void);

//�ͷſؼ�����
void IZYControl_Release(IZYControl *control);

//�ؼ�����
class IZYControl
{
public:
    //���ÿؼ�λ�óߴ�
    virtual void SetLocSize(int x,int y,int width,int height)=0;
    //��ȡ�ؼ�λ�óߴ�
    virtual void GetLocSize(int *px,int *py,int *pwidth,int *pheight)=0;
    //��ȡ�ؼ������ı�
    virtual void SetTitle(char *title)=0;
    //��ȡ�ؼ������ı�
    virtual char *GetTitle(void)=0;
    //�������ڴ���
    virtual void SetFrame(IZYFrame *frame)=0;
    //��ȡ���ڴ���
    virtual IZYFrame *GetFrame(void)=0;
    //����λͼͼƬ
    virtual void SetBitmap(IZYBitmap *bitmap)=0;
    //���ñ�����ɫ
    virtual void SetBackColor(COLORREF backColor)=0;
    //���ü��̽���
    virtual void SetFocus(bool focus)=0;
    //ˢ�¿ؼ�
    virtual void Invalidate(void)=0;
    //�����ؼ��ػ�
    virtual void OnPaint(IZYGraphics *graphics)=0;
    //������갴��
    virtual void OnMouseDown(int mx,int my)=0;
    //��������ƶ�
    virtual void OnMouseMove(int mx,int my)=0;
    //������굯��
    virtual void OnMouseUp(int mx,int my)=0;
    //����������
    virtual void OnMouseWheel(int mw)=0;
    //�������˫��
    virtual void OnMouseDoubleClick(int mx,int my)=0;
    //�������̰���
    virtual void OnKeyDown(int vkey)=0;
    //�����ַ�����
    virtual void OnCharInput(char ch)=0;
    //��������¼���������
    virtual void SetMouseListener(IZYMouseListener *mouseListener)=0;
    //���ü����¼���������
    virtual void SetKeyListener(IZYKeyListener *keyListener)=0;
};

//������̬�ı��ؼ�����
IZYStaticText *IZYStaticText_Create(char *title);

//�ͷž�̬�ı��ؼ�����
void IZYStaticText_Release(IZYStaticText *staticText);

//��̬�ı��ؼ�����
class IZYStaticText
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
};

//������̬ͼƬ�ؼ�����
IZYStaticPicture *IZYStaticPicture_Create(IZYBitmap *bitmap);

//�ͷž�̬ͼƬ�ؼ�����
void IZYStaticPicture_Release(IZYStaticPicture *staticPicture);

//��̬ͼƬ�ؼ�����
class IZYStaticPicture
{   
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
};

//������ť�ؼ�����
IZYButton *IZYButton_Create(char *title);

//�ͷŰ�ť�ؼ�����
void IZYButton_Release(IZYButton *button);

//��ť�ؼ�����
class IZYButton
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //���ð�ť������������
    virtual void SetActionListener(IZYActionListener *actionListener)=0;
};

//������ѡ��ؼ�����
IZYCheckBox *IZYCheckBox_Create(bool check);

//�ͷŵ�ѡ��ؼ�����
void IZYCheckBox_Release(IZYCheckBox *button);

//��ѡ��ؼ�����
class IZYCheckBox
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //��ȡѡ��״̬
    virtual bool GetCheckState(void)=0;
};

//�����༭�ؼ�����
IZYEdit *IZYEdit_Create(char *text);

//�ͷű༭�ؼ�����
void IZYEdit_Release(IZYEdit *edit);

//�༭�ؼ�����
class IZYEdit
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //�����Ƿ�Ϊ����༭��
    virtual void SetPasswordEdit(bool passwordEdit)=0;
    //�����Ƿ�Ϊֻ���༭��
    virtual void SetReadOnlyEdit(bool readOnlyEdit)=0;
    //���ñ༭�ַ���
    virtual void SetEditText(char *text)=0;
    //��ȡ�༭�ַ���
    virtual void GetEditText(char *text,int bufferLength)=0;
    //���ñ��ѡ�����ı�����ɫ
    virtual void SetMarkSelectedColor(COLORREF colorMarkSelected)=0;
    //��ȡѡ�����ַ���
    virtual void GetSelectedText(char *text,int bufferLength)=0;
    //����ѡ�����ַ���
    virtual void SetSelectedText(char *text)=0;
};

//�����������ؼ�����
IZYScrollBar *IZYScrollBar_Create(int i_base,int m_display,int m,EScrollType type);

//�ͷŹ������ؼ�����
void IZYScrollBar_Release(IZYScrollBar *scrollBar);

//�������ؼ�����
class IZYScrollBar
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //���ù�������
    virtual void SetParam(int i_base,int m_display,int m)=0;
    //��ȡ����ƫ��
    virtual int GetIndexBase(void)=0;
    //��������ƫ��
    virtual void SetIndexBase(int i_base)=0;
    //���ù����¼���������
    virtual void SetScrollListener(IZYScrollListener *scrollListener)=0;
    //���ù�����λͼͼƬ
    virtual void SetBarBitmap(IZYBitmap *barBitmap)=0;
};

//��������ؼ�����
IZYGrid *IZYGrid_Create(IZYTable *table);

//�ͷű���ؼ�����
void IZYGrid_Release(IZYGrid *grid);

//����ؼ�����
class IZYGrid
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //������ʾ�ı���
    virtual void SetTable(IZYTable *table)=0;
    //���ÿؼ�λ�óߴ�
    virtual void SetLocSize(int x,int y,int m_display,int unit_width,int unit_height)=0;  
    //ɾ����ǰѡ����
    virtual bool DeleteSelectedItem(void)=0;
    //������
    virtual void AppendItem(void)=0;
    //���ñ���ͼƬ
    virtual void SetTitleBitmap(IZYBitmap *titleBitmap)=0;
    //��ȡ��ֱ�������ؼ�
    virtual IZYScrollBar *GetScrollBarV(void)=0;
};

//������ǿ�༭�ؼ�����
IZYEditEx *IZYEditEx_Create(char *text);

//�ͷ���ǿ�༭�ؼ�����
void IZYEditEx_Release(IZYEditEx *editEx);

//��ǿ�༭�ؼ�����
class IZYEditEx
{
public:
    //ת��Ϊ�ؼ�����
    virtual IZYControl *ToControl(void)=0;
    //�����Ƿ�Ϊֻ���༭��
    virtual void SetReadOnlyEdit(bool readOnlyEdit)=0;
    //�����Ƿ���SQL���
    virtual void SetCheckSql(bool checkSql)=0;
    //���ñ༭�ַ���
    virtual void SetEditText(char *text)=0;
    //��ȡ�༭�ַ���
    virtual void GetEditText(char *text,int bufferLength)=0;
    //����������ɫ
    virtual void SetTextColor(COLORREF color)=0;
    //��ȡ��ֱ�������ؼ�
    virtual IZYScrollBar *GetScrollBarV(void)=0;
    //��ȡˮƽ�������ؼ�
    virtual IZYScrollBar *GetScrollBarH(void)=0;
    //������ص�����༭��
    virtual void SetRelativeOutput(IZYEditEx *output)=0;
    //������ص����ݿ����
    virtual void SetRelativeDatabase(IZYDatabase *database)=0;
    //��ȡ���޸ı�־
    virtual bool GetModified(void)=0;
    //�������޸ı�־
    virtual void SetModified(bool modified)=0;
    //ҳ��ص���ʼ
    virtual void LocateToHome(void)=0;
    //ҳ��ص�����
    virtual void LocateToEnd(void)=0;
    //��ȡѡ�����ַ�
    virtual void GetSelectedText(char *text,int bufferLength)=0;
    //����ѡ�����ַ�
    virtual void SetSelectedText(char *text)=0;
};

#endif