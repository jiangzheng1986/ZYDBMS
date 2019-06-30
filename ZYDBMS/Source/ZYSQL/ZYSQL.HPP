//---------------------------------------------------------
//���²�ѯ����(ZYSQL) ����:����
//---------------------------------------------------------
#if !defined(ZYSQL_HPP)        //����ZYSQL_HPP_��
#define ZYSQL_HPP              //ʹ��ZYSQL.HPP������1��

//Դ�����ַ�����
typedef enum
{
    ESourceCharType_Unknown,         //δ֪����
    ESourceCharType_Comment,         //ע��
    ESourceCharType_Identifier,      //��ʶ��
    ESourceCharType_Keyword,         //�ؼ���
    ESourceCharType_Punctuation,     //������
    ESourceCharType_String,          //�ַ���
    ESourceCharType_Num,             //����
    ESourceCharType_Float,           //С��
    ESourceCharType_Eof              //�ļ�����
}ESourceCharType;

//��ƥ���־
#define DISMATCH_FLAG   0x80

//��ȷ�ϱ�־
#define ACKNOWLEDGED_FLAG 0x40

//���²�ѯ����������
class IZYCompiler;

//���ܼ��Ӷ���
class IZYProfiler;

//�������²�ѯ����������
IZYCompiler *IZYCompiler_Create(char *listingfile,IZYDatabase *database);

//�ͷ����²�ѯ����������
void IZYCompiler_Release(IZYCompiler *compiler);

//���²�ѯ����������
class IZYCompiler
{
public:
    //������½ṹ����ѯ����(ZYSQL)���
    virtual void CheckSQL(char *sql,char *format)=0;
    //ִ�����½ṹ����ѯ����(ZYSQL)���
    virtual void ExecuteSQL(char *sql)=0;
    //��ȡ��ѯִ�н������
    virtual IZYTable *GetResultTable(void)=0;
    //�����Ƿ���ʾԴ����
    virtual void SetDisplaySource(bool displaySource)=0;
    //�����Ƿ���ʾƥ�����
    virtual void SetDisplayMatch(bool displayMatch)=0;
    //�����Ƿ���ʾ�����﷨��
    virtual void SetDisplayAST(bool displayAST)=0;
    //�����Ƿ���ʾ�Ż��﷨��
    virtual void SetDisplayOST(bool displayOST)=0;
    //�����Ƿ���ʾ���ݱ���
    virtual void SetDisplayTables(bool displayTables)=0;
    //�����Ƿ���ʾ��ѯ��ʱ
    virtual void SetDisplayTime(bool displayTime)=0;
    //�����Ƿ���ʾ�������
    virtual void SetDisplayResult(bool displayResult)=0;
};

//�����ı��ļ��ı�������
bool IZYTextFile_Exists(char *filename);

//�����ı��ļ��ı�
char *IZYTextFile_LoadText(char *filename);

//�����ı��ļ��ı�
void IZYTextFile_SaveText(char *filename,char *s);

//�ͷ��ı��ļ��ı�
void IZYTextFile_FreeText(char *s);

//ɾ���ı��ļ��ı�
void IZYTextFile_DeleteText(char *filename);

//�������ܼ��Ӷ���
IZYProfiler *IZYProfiler_Create(void);

//�ͷ����ܼ��Ӷ���
void IZYProfiler_Release(IZYProfiler *profiler);

//���ܼ��Ӷ���
class IZYProfiler
{
public:
    //���ܼ��Ӷ������ܼ��ӿ�ʼ
    void Start(void);
    //���ܼ��Ӷ������ܼ��ӽ���
    void End(void);
    //��ȡ���ܼ��Ӷ�����ӵ���ִ��ʱ��(��)
    float GetTime(void);
    //��ȡ���ܼ��Ӷ�����ӵ���ִ��ʱ��(����)
    int GetTimeMs(void);
};

#endif