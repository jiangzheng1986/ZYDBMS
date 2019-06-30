//---------------------------------------------------------
//���½�������(ZYGUI) ����:����
//---------------------------------------------------------
#if !defined(ZYGUI1_HPP)        //����ZYGUI_HPP_��
#define ZYGUI1_HPP              //ʹ��ZYGUI.HPP������1��

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

//����洢����(ZYRDB)API�ӿڿ�
#include <ZYRDB.HPP>

//�����ѯ����(ZYSQL)API�ӿڿ�
#include <ZYSQL.HPP>

//�����������(ZYGUI)API�ӿڿ�
#include "ZYGUI.HPP"

//�涨���ı�����󳤶�
#define MAX_TEXT_LEN 100000

//Ӧ��C++��׼�����ƿռ�(std)
using namespace std;

//������״̬
enum EStateScroll
{
    EStateScroll_Start,
    EStateScroll_Scroll
};

//�ؼ�����
class ZYControl;

//λͼͼƬ����
class ZYBitmap;

//��������
class ZYContainer;

//���ڶ���
class ZYFrame:public IZYFrame
{
public:
    //���촰�ڶ���
    ZYFrame(int x,int y,int width,int height,char *title);
    //�������ڶ���
    ~ZYFrame(void);
    //���ز˵�
    HMENU LoadMenu(int rid);
    //�������˵�
    void SetMainMenu(HMENU hMenu);
    //���ӿؼ�
    void AddControl(IZYControl *control);
    //ɾ���ؼ�
    void RemoveControl(IZYControl *control);
    //����λͼͼƬ
    void SetBitmap(IZYBitmap *bitmap);
    //���ñ�����ɫ
    void SetBackColor(COLORREF backColor);
    //��ȡ���ھ��
    HWND GetWindowHandle(void);
    //������Ϣ����
    LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //������Ϣѭ��
    int MsgLoop(void);
    //ˢ�´���
    void Invalidate(void);
    //�رմ���
    void Close(void);
    //��ȡ�����Ƿ��ѹر�
    bool GetClosed(void);
    //�˵�����
    void OnMenuCommand(int mcid);
    //���ò˵��¼���������
    void SetMenuListener(IZYMenuListener *menuListener);
    //���ô����¼���������
    void SetFrameListener(IZYFrameListener *frameListener);
    //��������¼���������
    void SetMouseListener(IZYMouseListener *mouseListener);
    //���������¼���������
    void SetNetworkListener(IZYNetworkListener *networkListener);
    //��ȡ����ؼ�
    IZYControl *GetFocusControl(void);
    //�����Ƿ񿪷�ʽͼ�οⴰ��
    void SetOpenGLWindow(bool openGLWindow);
    //���ô����¼���������1
    void SetFrameListener1(IZYFrameListener1 *frameListener1);
    //���ڶ���
    static vector<ZYFrame *> *frames;
    //���̽���ؼ�
    static ZYControl *focusControl;
    //���������״̬
    static bool caretVisible;
private:
    //ע�ᴰ����
    ATOM RegisterWindowClass(void);
    //��ʼ������
    void InitWindow(void);
    //����˫����
    void EnableDoubleBuffer(void);
    //���ڻ�ͼ��Ϣ����
    void OnWmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //��갴����Ϣ����
    void OnWmLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //����ƶ���Ϣ����
    void OnWmMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //��굯����Ϣ����
    void OnWmLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //�˵�������Ϣ����
    void OnWmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //���̰�����Ϣ����
    void OnWmKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //�ַ�������Ϣ����
    void OnWmChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //��������Ϣ����
    void OnWmMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //���˫����Ϣ����
    void OnWmLButtonDblClk(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //���ڳ�����Ϣ����
    void OnWmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //���ڳ�����Ϣ����
    void OnWmClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //�׽��ֶ���Ϣ����
    void OnWmNetworkEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);
    //���ھ��
    HWND hWnd;
    //�ؼ�����
    vector<ZYControl *> *controls; 
    //��ĻDC
    HDC hDC;                                  
    //˫����λͼ
    HBITMAP hBitmapBuffer;                    
    //˫����λͼ�豸
    HDC hDCBitmapBuffer;   
    //������
    int x;                                  
    //������
    int y;                                  
    //����
    int width;                              
    //�߶�
    int height;
    //���ڱ����ַ���
    char *title;
    //������
    bool mainWindow;
    //�˵��¼���������
    IZYMenuListener *menuListener;
    //�����¼���������
    IZYFrameListener *frameListener;
    //����¼���������
    IZYMouseListener *mouseListener;
    //�����¼���������
    IZYNetworkListener *networkListener;
    //λͼͼƬ����
    ZYBitmap *bitmap;
    //������ɫ
    COLORREF backColor;
    //�����Ƿ��ѹر�
    bool closed;
    //�Ƿ񿪷�ʽͼ�οⴰ��
    bool openGLWindow;
    //�����¼���������1
    IZYFrameListener1 *frameListener1;
};

//λͼͼƬ����
class ZYBitmap:public IZYBitmap
{
public:
    //����λͼͼƬ����
    ZYBitmap(int rid);
    //��ȡλͼͼƬ�豸���
    HDC GetHdcBitmap(void);
private:
    //λͼͼƬ���
    HBITMAP hBitmap;
    //λͼͼƬ�豸���
    HDC hdcBitmap;
};

//ͼ�ζ���
class ZYGraphics:public IZYGraphics
{
public:
    //����ͼ�ζ���
    ZYGraphics(HDC hdc);
    //��������ƫ��
    void SetXYBase(int x_base,int y_base);
    //����һ����
    void DrawLine(int x1,int y1,int x2,int y2);
    //����һ������
    void DrawRectangle(int x,int y,int w,int h);
    //������
    void FillRectangle(int x,int y,int w,int h,int brush);
    //������1
    void FillRectangle1(int x,int y,int w,int h,COLORREF color);
    //����ı�
    void OutputText(char *buf,int x,int y,int w,int h,int align);
    //���ͼ��
    void DrawBitmap(IZYBitmap *bitmap,int x,int y,int w,int h);
    //�����ı���ɫ
    void SetTextColor(COLORREF color);
    //���û�����ɫ
    void SetPenColor(COLORREF color);
    //���û�����ɫ
    void ResetPenColor(void);
private:
    //ͼ���豸���
    HDC hdc;
    //���ʶ�����
    HANDLE hPen;
    //Xƫ��
    int x_base;
    //Yƫ��
    int y_base;
};

//���������
class ZYClipboard:public IZYClipboard
{
public:
    //������������
    ZYClipboard(IZYFrame *frame);
    //��ȡ�������е��ı�
    char *PasteText(void);
    //���ü������е��ı�
    void CopyText(char *text);
private:
    //���ھ��
    HWND hWnd;
};

//�ؼ�����
class ZYControl:public IZYControl
{
public:
    //����ؼ�����
    ZYControl(void);
    //�����ؼ�����
    ~ZYControl(void);
    //���ÿؼ�λ�óߴ�
    void SetLocSize(int x,int y,int width,int height);
    //��ȡ�ؼ�λ�óߴ�
    void GetLocSize(int *px,int *py,int *pwidth,int *pheight);
    //��ȡ�ؼ������ı�
    void SetTitle(char *title);
    //��ȡ�ؼ������ı�
    char *GetTitle(void);
    //�������ڴ���
    void SetFrame(IZYFrame *frame);
    //��ȡ���ڴ���
    IZYFrame *GetFrame(void);
    //������������
    void SetContainer(ZYContainer *container);
    //����λͼͼƬ
    void SetBitmap(IZYBitmap *bitmap);
    //���ñ�����ɫ
    void SetBackColor(COLORREF backColor);
    //���ü��̽���
    void SetFocus(bool focus);
    //ˢ�¿ؼ�
    void Invalidate(void);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //��������ƶ�
    void OnMouseMove(int mx,int my);
    //������굯��
    void OnMouseUp(int mx,int my);
    //����������
    void OnMouseWheel(int mw);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
    //�������̰���
    void OnKeyDown(int vkey);
    //�����ַ�����
    void OnCharInput(char ch);
    //��������¼���������
    void SetMouseListener(IZYMouseListener *mouseListener);
    //���ü����¼���������
    void SetKeyListener(IZYKeyListener *keyListener);
    //����һ���ַ���,���ַ����ռ����
    char *CopyString(char *s);
protected:
    //������
    int x;                                  
    //������
    int y;                                  
    //����
    int width;                              
    //�߶�
    int height;
    //�����ı�
    char *title;
    //λͼͼƬ����
    ZYBitmap *bitmap;
    //���ڴ���
    ZYFrame *frame;
    //��������
    ZYContainer *container;
    //����¼���������
    IZYMouseListener *mouseListener;
    //�����¼���������
    IZYKeyListener *keyListener;
    //������ɫ
    COLORREF backColor;
};

//��������
class ZYContainer:public ZYControl
{
public:
    //������������
    ZYContainer(void);
    //������������
    virtual ~ZYContainer(void);
    //��ȡXƫ��
    int GetXBase(void);
    //��ȡYƫ��
    int GetYBase(void);
    //�������ڴ���
    void SetFrame(IZYFrame *frame);
    //�������ڴ���
    void SetBitmap(IZYBitmap *bitmap);
    //���������ػ�
    void OnPaint(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //��������ƶ�
    void OnMouseMove(int mx,int my);
    //������굯��
    void OnMouseUp(int mx,int my);
    //�����ӿؼ�
    void AddControl(IZYControl *control);
    //ɾ���ؼ�
    void RemoveControl(IZYControl *control);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
private:
    //�ؼ�����
    vector<ZYControl *> *controls;
};

//��̬�ı��ؼ�����                                                                                                                                                                                                                                                                 
class ZYStaticText:public ZYControl,public IZYStaticText
{
public:
    //���쾲̬�ı��ؼ�����
    ZYStaticText(char *title);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //���ƾ�̬�ı�
    void OnPaint(IZYGraphics *graphics);
};

//��̬ͼƬ�ؼ�����
class ZYStaticPicture:public ZYControl,public IZYStaticPicture
{   
public:
    //���쾲̬ͼƬ�ؼ�����
    ZYStaticPicture(ZYBitmap *bitmap);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //���ƾ�̬ͼƬ
    void OnPaint(IZYGraphics *graphics);
};

//��ť�ؼ�����
class ZYButton:public ZYControl,public IZYButton
{
public:
    //���찴ť�ؼ�����
    ZYButton(char *title);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //���ð�ť������������
    void SetActionListener(IZYActionListener *actionListener);
    //������갴��
    void OnMouseDown(int mx,int my);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
private:
    //��ť������������
    IZYActionListener *actionListener;
};

//��ѡ��ؼ�����
class ZYCheckBox:public ZYControl,public IZYCheckBox
{
public:
    //���쵥ѡ��ؼ�����
    ZYCheckBox(bool check);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //��ȡѡ��״̬
    bool GetCheckState(void);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
private:
    //ѡ��״̬
    bool check;          
};

//�༭�ؼ�����
class ZYEdit:public ZYControl,public IZYEdit
{
public:
    //����༭�ؼ�����
    ZYEdit(char *text);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //���ÿؼ�λ�óߴ�
    void SetLocSize(int x,int y,int width,int height);
    //�����Ƿ�Ϊ����༭��
    void SetPasswordEdit(bool passwordEdit);
    //�����Ƿ�Ϊֻ���༭��
    void SetReadOnlyEdit(bool readOnlyEdit);
    //���ñ༭�ַ���
    void SetEditText(char *text);
    //��ȡ�༭�ַ���
    void GetEditText(char *text,int bufferLength);
    //���ü��̽���
    void SetFocus(bool focus);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //��������ƶ�
    void OnMouseMove(int mx,int my);
    //������굯��
    void OnMouseUp(int mx,int my);
    //�������̰���
    void OnKeyDown(int vkey);
    //�����ַ�����
    void OnCharInput(char ch);
    //���ñ��ѡ�����ı�����ɫ
    void SetMarkSelectedColor(COLORREF colorMarkSelected);
    //��ȡѡ�����ַ���
    void GetSelectedText(char *text,int bufferLength);
    //����ѡ�����ַ���
    void SetSelectedText(char *text);
private:
    //��ʾ�����
    void ShowCaret(void);
    //���ز����
    void HideCaret(void);
    //0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
    int TestChar(char *text,int i);
    //�Ƿ������ַ�
    int IsChineseChar(int i);
    //�����ַ�����
    void IncEditIndex(void);
    //�ݼ��ַ�����
    void DecEditIndex(void);
    //�ַ������ص���ʼ
    void HomeEditIndex(void);
    //�ַ������ص�����
    void EndEditIndex(void);
    //�ݼ��ַ�����,���ڻ����ַ�
    void DecEditIndex2(void);
    //�����ַ����༭�ַ���
    void InsertChar(int i,char c);
    //�ӱ༭�ַ���ɾ���ַ�
    void DeleteChar(int i,int c);
    //�ӱ༭�ַ���ɾ���ַ�,����������
    void DeleteChar2(void);
    //�ڱ༭�ַ����л����ַ�,����������
    void BackspaceChar(void);
    //�ݼ��ַ�����,����������
    void LeftKey(void);
    //�����ַ�����,����������
    void RightKey(void);
    //���ѡ�����ı�
    void MarkSelectedText(IZYGraphics *graphics);
    //���ѡ�����ı�
    void MarkSelectedText1(IZYGraphics *graphics,int start,int end);
    //ʹ�ı���ʼС���ı�����
    void CheckSelectedStartEnd(void);
    //�༭�ַ���
    char text[MAX_TEXT_LEN];
    //�ַ���
    int n;                                  
    //�༭λ��
    int i;
    //�༭λ�û�ֵ
    int i_base;
    //�༭��ʾ�ַ���
    int n_display;                          
    //�Ƿ�Ϊ����༭��
    bool passwordEdit;
    //�Ƿ�Ϊֻ���༭��
    bool readOnlyEdit;
    //���ѡ�����ı�����ɫ
    COLORREF colorMarkSelected;
    //ѡ���ı���ʼ
    int selectedStart;
    //ѡ���ı�����
    int selectedEnd;
    //ѡ��״̬
    bool stateSelect;
};

//�������ؼ�����
class ZYScrollBar:public ZYControl,public IZYScrollBar
{
public:
    //����������ؼ�����
    ZYScrollBar(int i_base,int m_display,int m,EScrollType type);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //���ù�������
    void SetParam(int i_base,int m_display,int m);
    //��ȡ����ƫ��
    int GetIndexBase(void);
    //��������ƫ��
    void SetIndexBase(int i_base);
    //���ù����¼���������
    void SetScrollListener(IZYScrollListener *scrollListener);
    //���ù�����λͼͼƬ
    void SetBarBitmap(IZYBitmap *barBitmap);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //��������ƶ�
    void OnMouseMove(int mx,int my);
    //������굯��
    void OnMouseUp(int mx,int my);
    //����������
    void OnMouseWheel(int mw);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
private:
    //�����·���ͷ
    void DrawDownIcon(IZYGraphics *graphics,int x,int y,int h);
    //�����Ϸ���ͷ
    void DrawUpIcon(IZYGraphics *graphics,int x,int y,int h);
    //�����󷭼�ͷ
    void DrawLeftIcon(IZYGraphics *graphics,int x,int y,int h);
    //�����ҷ���ͷ
    void DrawRightIcon(IZYGraphics *graphics,int x,int y,int h);
    //���ƹ�����
    void DrawScrollBarV(IZYGraphics *graphics,int x,int y,int w,int m,int i_base,int m_display);
    //���ƹ�����
    void DrawScrollBarH(IZYGraphics *graphics,int x,int y,int h,int m,int i_base,int m_display);
    //������갴��
    void OnMouseDownV(int mx,int my);
    //������갴��
    void OnMouseDownH(int mx,int my);
    //��������ƶ�
    void OnMouseMoveV(int mx,int my);
    //��������ƶ�
    void OnMouseMoveH(int mx,int my);
    //����������ֵ
    void DecIndexBase(void);
    //����������ֵ
    void IncIndexBase(void);
    //��ҳ����������ֵ
    void DecIndexBasePage(void);
    //��ҳ����������ֵ
    void IncIndexBasePage(void);
    //����ƫ��
    int i_base;                             
    //��ʾ����
    int m_display;
    //�ܼ�����
    int m;
    //������������ϱ�����ڹ�������λ��
    double h_scroll_1;
    //������������±�����ڹ�������λ��
    double h_scroll_2;  
    //����Ĺ������ϱ�λ��
    double h_scroll_3;                  
    //����Ĺ������±�λ��
    double h_scroll_4;                  
    //����ʱ���λ��
    double m_scroll;
    //����״̬
    EStateScroll state_scroll;  
    //����������
    EScrollType type;
    //�����¼���������
    IZYScrollListener *scrollListener;
    //������λͼͼƬ
    IZYBitmap *barBitmap;
};

//����ؼ�����
class ZYGrid:public ZYContainer,public IZYGrid
{
public:
    //�������ؼ�����
    ZYGrid(IZYTable *table);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //������ʾ�ı���
    void SetTable(IZYTable *table);
    //���ÿؼ�λ�óߴ�
    void SetLocSize(int x,int y,int m_display,int unit_width,int unit_height);  
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //���Ե��Ƿ��ھ�����
    bool PointInRect(int p_x,int p_y,int x,int y,int width,int height);
    //������갴��
    void OnMouseDown(int mx,int my);
    //����������
    void OnMouseWheel(int mw);
    //�������˫��
    void OnMouseDoubleClick(int mx,int my);
    //�������̰���
    void OnKeyDown(int vkey);
    //���Ƶ�ǰѡ����
    void DecSelectedItem(void);
    //���Ƶ�ǰѡ����
    void IncSelectedItem(void);
    //��ҳ���Ƶ�ǰѡ����
    void DecSelectedItemPage(void);
    //��ҳ���Ƶ�ǰѡ����
    void IncSelectedItemPage(void);
    //ɾ����ǰѡ����
    bool DeleteSelectedItem(void);
    //������
    void AppendItem(void);
    //��ǰѡ���лص���ʼ
    void HomeSelectedItem(void);
    //��ǰѡ����ȥ������
    void EndSelectedItem(void);
    //���ñ���ͼƬ
    void SetTitleBitmap(IZYBitmap *titleBitmap);
    //��ȡ��ֱ�������ؼ�
    IZYScrollBar *GetScrollBarV(void);
private:
    //�������
    void OutputTableName(IZYGraphics *graphics,int x,int y,int w,int h);
    //�������
    void OutputColumnName(IZYGraphics *graphics,int j,int x,int y,int w,int h);
    //��6��С���ض�Ϊ2��С��
    void ProcessDouble(char *str_double);
    //�������
    void OutputData(IZYGraphics *graphics,int i,int j,int x,int y,int w,int h);
    //���Ʊ�����
    void DrawGirds(IZYGraphics *graphics,int x,int y,int w,int h,int m,int n);
    // ����ʮ�ּ�ͷ
    void DrawCrossIcon(IZYGraphics *graphics,int x,int y,int h);
    //���Ƶ�ǰѡ���и�����
    void DrawItemBar(IZYGraphics *graphics);
    //���¹�������
    void UpdateScrollParam(int i_base);
    //��ȡ����ʽ����������
    void GetTableData(char *s,IZYTable *table,int i,int j);
    //��ʽ�������ñ�������
    void PutTableData(char *s,IZYTable *table,int i,int j);
    //�뿪�༭ģʽ
    void LeaveEditMode(void);
    //�������
    IZYTable *table;
    //������
    IZYScrollBar *scrollBar;
    //��Ԫ�����
    int unit_width;
    //��Ԫ��߶�
    int unit_height;
    //��ʾ����
    int m_display;
    //���ĵ�ǰ��
    int item;
    //���ĵ�ǰ��
    int column;
    //�������
    int i_input;
    //�������
    int j_input;
    //����༭��
    IZYEdit *editData;
    //����ͼƬ
    IZYBitmap *titleBitmap;
};

//��ǿ�ַ�������
class ZYStringEx
{
public:
    //������ǿ�ַ�������
    ZYStringEx(char *s);
    //������ǿ�ַ�������
    ~ZYStringEx(void);
    //�����ַ���
    void PutString(char *s);
    //��ȡ�ַ���
    void GetString(char *s,int len);
    //��ȡ�ַ���
    void GetLine(int lineno,char *buf);
    //��ȡ�ַ�
    char GetChar(int i,int j);
    //��ȡ�ַ��м���
    int GetLineCount(void);
    //��ȡ���ַ�����
    int GetCharCount(int lineno);
    //��ȡ�ַ������е��ַ���
    int GetMaxChar(void);
    //ɾ���ַ�
    void DeleteChar(int i,int j);
    //�����ַ�
    void InsertChar(int i,int j,char ch);
    //���뻻�з�
    void InsertNewLine(int i,int j);
    //���SQL���
    void CheckSql(IZYDatabase *database,IZYEditEx *output);
    //��ȡ�ַ�����
    ESourceCharType GetCharType(int i,int j);
    //���Բ�ƥ���־
    bool TestDismatchFlag(int i,int j);
    //������ȷ�ϱ�־
    bool TestAcknowledgedFlag(int i,int j);
    //ɾ���ַ�
    void DeleteChars(int i1,int j1,int i2,int j2);
    //ɾ���ַ���
    void DeleteLine(int i);
    //��ȡ�ַ���
    void GetString(char *s,int i1,int j1,int i2,int j2,int len);
private:
    //�ͷ���ǿ�ַ���
    void ReleaseStrEx(void);
    //�ͷŸ�ʽ�ַ���
    void ReleaseFormatEx(void);
    //��ǿ�ַ������ݽṹ
    vector<vector<char> *> *strEx;
    //��ʽ�ַ������ݽṹ
    vector<vector<char> *> *formatEx;
};

//��ǿ�༭�ؼ�����
class ZYEditEx:public ZYContainer,public IZYEditEx,public IZYScrollListener
{
public:
    //������ǿ�༭�ؼ�����
    ZYEditEx(char *text);
    //������ǿ�༭�ؼ�����
    ~ZYEditEx(void);
    //ת��Ϊ�ؼ�����
    IZYControl *ToControl(void);
    //�����Ƿ�Ϊֻ���༭��
    void SetReadOnlyEdit(bool readOnlyEdit);
    //�����Ƿ���SQL���
    void SetCheckSql(bool checkSql);
    //���ÿؼ�λ�óߴ�
    void SetLocSize(int x,int y,int width,int height);
    //���ñ༭�ַ���
    void SetEditText(char *text);
    //��ȡ�༭�ַ���
    void GetEditText(char *text,int bufferLength);
    //����������ɫ
    void SetTextColor(COLORREF color);
    //��ȡ��ֱ�������ؼ�
    IZYScrollBar *GetScrollBarV(void);
    //��ȡˮƽ�������ؼ�
    IZYScrollBar *GetScrollBarH(void);
    //������ص�����༭��
    void SetRelativeOutput(IZYEditEx *output);
    //������ص����ݿ����
    void SetRelativeDatabase(IZYDatabase *database);
    //�����ؼ��ػ�
    void OnPaint(IZYGraphics *graphics);
    //�����ؼ��ػ�
    void OnPaintSql(IZYGraphics *graphics);
    //������갴��
    void OnMouseDown(int mx,int my);
    //����������
    void OnMouseWheel(int mw);
    //�������̰���
    void OnKeyDown(int vkey);
    //�����ַ�����
    void OnCharInput(char ch);
    //������������
    void BarScroll(IZYControl *control,int i_base);
    //��ȡ���޸ı�־
    bool GetModified(void);
    //�������޸ı�־
    void SetModified(bool modified);
    //ҳ��ص���ʼ
    void LocateToHome(void);
    //ҳ��ص�����
    void LocateToEnd(void);
    //��������ƶ�
    void OnMouseMove(int mx,int my);
    //������굯��
    void OnMouseUp(int mx,int my);
    //��ȡѡ�����ַ�
    void GetSelectedText(char *text,int bufferLength);
    //����ѡ�����ַ�
    void SetSelectedText(char *text);
protected:
    //���ü��̽���
    void SetFocus(bool focus);
    //��ʾ�����
    void ShowCaret(void);
    //���ز����
    void HideCaret(void);
    //���¹���������
    void UpdateScrollParam(void);
    //��ȡ����������
    void GetScrollBase(void);
    //0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
    int TestChar(char *text,int i);
    //0:Ӣ��,1:�������ֽ�,2:���Ĵ��ֽ�
    int TestChar1(int i,int j);
    //�Ƿ������ַ�
    bool IsChineseChar(int i,int j);
    //�����ַ�����
    void IncEditIndex(void);
    //�ݼ��ַ�����
    void DecEditIndex(void);
    //�ַ������ص���ʼ
    void HomeEditIndex(void);
    //�ַ������ص�����
    void EndEditIndex(void);
    //�ݼ��ַ�����,����������
    void LeftKey(void);
    //�����ַ�����,����������
    void RightKey(void);
    //�ݼ�������,����������
    void UpKey(void);
    //����������,����������
    void DownKey(void);
    //��ҳ�ݼ�������,����������
    void PageUp(void);
    //��ҳ����������,����������
    void PageDown(void);
    //�ݼ��ַ�����,���ڻ����ַ�
    void DecEditIndex2(void);
    //�ӱ༭�ַ���ɾ���ַ�,����������
    void DeleteChar(void);
    //�ڱ༭�ַ����л����ַ�,����������
    void BackspaceChar(void);
    //���뻻�з�
    void InsertNewLine(void);
    //���SQL�﷨
    void CheckSql(void);
    //���ܸ�֪
    void IntelligentSense(void);
    //ʹ�ı���ʼС���ı�����
    void CheckSelectedStartEnd(void);
    //ʹ�ı���ʼС���ı�����
    void ZYEditEx::CheckSelectedStartEnd1(int *p_i_selectedStart,
                                          int *p_i_selectedEnd,
                                          int *p_j_selectedStart,
                                          int *p_j_selectedEnd);
    //���ñ��ѡ�����ı�����ɫ
    void SetMarkSelectedColor(COLORREF colorMarkSelected);
    //���ѡ�����ı�
    void MarkSelectedText(IZYGraphics *graphics);
    //���ѡ�����ı�
    void MarkSelectedText1(IZYGraphics *graphics,int i,int start,int end);
    //�ƶ�������������ʼ����
    bool MoveKeyStart(int type);
    //�ƶ�������������������
    void MoveKeyEnd(void);
    //ɾ����ǰѡ�����ַ�
    void DeleteSelectedText(void);
    //��ǿ�ַ�������
    ZYStringEx *stringEx;
    //��ֱ������
    IZYScrollBar *scrollBarV;
    //ˮƽ������
    IZYScrollBar *scrollBarH;
    //�ַ��и�
    int line_height;
    //��ʾ�ַ�����
    int m_display;
    //ÿ����ʾ�ַ���
    int n_display;
    //�Ƿ�Ϊֻ���༭��
    bool readOnlyEdit;
    //�Ƿ���SQL���
    bool checkSql;
    //�б༭λ��
    int i;
    //�б༭λ�û�ֵ
    int i_base;
    //�б༭λ��
    int j;
    //�б༭λ�û�ֵ
    int j_base;
    //������ɫ
    COLORREF color;
    //��ص�����༭��
    IZYEditEx *output;
    //��ص����ݿ����
    IZYDatabase *database;
    //���޸ı�־
    bool modified;
    //���ѡ�����ı�����ɫ
    COLORREF colorMarkSelected;
    //ѡ���ı���ʼ��
    int i_selectedStart;
    //ѡ���ı�������
    int i_selectedEnd;
    //ѡ���ı���ʼ��
    int j_selectedStart;
    //ѡ���ı�������
    int j_selectedEnd;
    //ѡ��״̬
    bool stateSelect;
};

#endif