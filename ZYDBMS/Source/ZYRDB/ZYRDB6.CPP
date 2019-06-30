//--------------------------------------------------------------
//���´洢����(ZYRDB)-�������� ����:����
//--------------------------------------------------------------
#include "ZYRDB1.HPP"

//�������������
static int day_month1[12]={31,29,31,30,31,30,31,31,30,31,30,31};

//���������������
static int day_month2[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//��������ʱ�����
IZYDateTime *IZYDateTime_Create(int year,int month,int day)
{
    return (IZYDateTime *)new ZYDateTime(year,month,day);
}

//��ȡ����ʱ�����
IZYDateTime *IZYDateTime_Today(void)
{
    return (IZYDateTime *)new ZYDateTime();
}

//�ͷ�����ʱ�����
void IZYDateTime_Release(IZYDateTime *datetime)
{
    delete (ZYDateTime *)datetime;
}       

//��������ʱ�����
ZYDateTime::ZYDateTime(void)
{
    GetLocalTime(&datetime);
}

//��������ʱ�����
ZYDateTime::ZYDateTime(int year,int month,int day)
{
    datetime.wYear=year;

    datetime.wMonth=month;

    datetime.wDay=day;
}

//��ȡ����Ϣ
int ZYDateTime::GetYear(void)
{
    return datetime.wYear;
}

//��ȡ����Ϣ
int ZYDateTime::GetMonth(void)
{
    return datetime.wMonth;
}

//��ȡ����Ϣ
int ZYDateTime::GetDay(void)
{
    return datetime.wDay;
}
//��ȡʱ��Ϣ
int ZYDateTime::GetHour(void)
{
    return datetime.wHour;
}

//��ȡ����Ϣ
int ZYDateTime::GetMinute(void)
{
    return datetime.wMinute;
}

//��ȡ����Ϣ
int ZYDateTime::GetSecond(void)
{
    return datetime.wSecond;
}

//�Ƿ�������
bool ZYDateTime::IsRunNian(int year)
{
    if(year%400==0)        //����400,������
    {
        return 1;
    }
    else if(year%100==0)   //������400,����100,��������
    {
        return 0;     
    }
    else if(year%4==0)     //������400,������100,����4,������
    {
        return 1;
    }
    else                   //������400,������100,������4,��������
    {
        return 0;
    }
}

//���㵽1980.01.01������
int ZYDateTime::Substract1980(void)
{
    int year;

    int month;

    int day;

    int n=0;             //��1980.01.01������

    int i;

    year=datetime.wYear;

    month=datetime.wMonth;

    day=datetime.wDay;

    for(i=1980;i<year;i++)
    {
        if(IsRunNian(i))
        {
            n+=366;
        }
        else
        {
            n+=365;
        }
    }

    for(i=1-1;i<month-1;i++)
    {
        if(IsRunNian(year))
        {
            n+=day_month1[i];
        }
        else
        {
            n+=day_month2[i];
        }
    }

    n+=day-1;

    return n;
}

//��������֮��ļ��
int ZYDateTime::Substract(IZYDateTime *datetime)
{
    int n1;

    int n2;

    n1=this->Substract1980();

    n2=((ZYDateTime *)datetime)->Substract1980();

    return n1-n2;
}

//�������������������
IZYRandom *IZYRandom_Create(void)
{
    return (IZYRandom *)new ZYRandom();
}

//�ͷ����������������
void IZYRandom_Release(IZYRandom *random)
{
    delete (ZYRandom *)random;
}

//�������������������
ZYRandom::ZYRandom(void)
{
    SYSTEMTIME datetime; 

    GetLocalTime(&datetime);

    srand(datetime.wMilliseconds);
}

//��������[min,max]������������
int ZYRandom::RandInt(int min,int max)
{
    return min+rand()%(max-min+1);
}

//��������[min,max]��������С��
double ZYRandom::RandDouble(double min,double max)
{
    return (double)(RandInt((int)(min*100),(int)(max*100)))/100.0f;
}

//����һ���䳤�ַ����������ַ�����
void ZYString::Copy1(char *dest,char *src,int cnt)
{
    int i;
    for(i=0;i<cnt;i++)
    {
        dest[i]=src[i];
        if(src[i]==0)
        {
            break;
        }
    }
}

//����һ�������ַ������䳤�ַ�����
void ZYString::Copy2(char *dest,char *src,int cnt)
{
    int i;
    for(i=0;i<cnt;i++)
    {
        dest[i]=src[i];
        if(src[i]==0)
        {
            break;
        }
    }
    dest[i]='\0';
}

//��һ�������ַ�����䳤�ַ����Ƚ�,�ж��Ƿ����
int ZYString::Compare(char *sa,char *sb,int cnt)
{
    int i;
    for(i=0;i<cnt;i++)
    {
        if(sa[i]=='\0'&&sb[i]=='\0')
            return 1;
        if(sa[i]!='\0'&&sb[i]=='\0')
            return 0;
        if(sa[i]=='\0'&&sb[i]!='\0')
            return 0;
        if(sa[i]!=sb[i])
            return 0;
    }
    return 1;
}

//����һ���ַ���
char *ZYString::Copy(char *s)
{
    char *s1=(char *)malloc(strlen(s)+1);
    strcpy(s1,s);
    return s1;
}

//�ͷŸ��Ƶ��ַ���
void ZYString::Free(char *s)
{
    free(s);
}

//�����ڴ�ռ����
IZYMemory *IZYMemory_Create(int size)
{
    return (IZYMemory *)new ZYMemory(size);
}

//�ͷ��ڴ�ռ����
void IZYMemory_Release(IZYMemory *memory)
{
    delete (ZYMemory *)memory;
}

//�����ڴ�ռ����
ZYMemory::ZYMemory(int size)
{
    this->lp=ApplyMemory(size);
    this->size=size;
}

//�����ڴ�ռ����
ZYMemory::~ZYMemory(void)
{
    FreeMemory(lp);
}

//��ȡ�ڴ�ռ�ָ��
void *ZYMemory::GetPointer(void)
{
    return lp;
}

//��ȡ�ڴ�ռ��С
int ZYMemory::GetSize(void)
{
    return size;
}

//�����ڴ�ռ��С
void *ZYMemory::Resize(int size)
{
    return lp=ResizeMemory(lp,size);
}

//�����ڴ�ռ�
void *ZYMemory::ApplyMemory(int byte_count)
{
    HGLOBAL h;

    void *lp;

    h=GlobalAlloc(0,byte_count);

    lp=GlobalLock(h);

    return lp;
}

//��ȡ�ڴ�ռ��С
int ZYMemory::SizeOfMemory(void *lp)
{
    HGLOBAL h;

    if(lp==NULL)
    {
        return 0;
    }
    else
    {
        h=GlobalHandle(lp);

        return GlobalSize(h);
    }
}

//�����ڴ�ռ��С
void *ZYMemory::ResizeMemory(void *lp,int byte_count)
{
    HGLOBAL h1;

    HGLOBAL h2;

    char *lp1;

    char *lp2;

    int c;

    lp1=(char *)lp;

    h1=GlobalHandle(lp);

    c=GlobalSize(h1);

    h2=GlobalAlloc(0,byte_count);

    lp2=(char *)GlobalLock(h2);

    memcpy(lp2,lp1,c);

    GlobalUnlock(h1);

    GlobalFree(h1);

    return lp2;
}

//�ͷ��ڴ�ռ�
void ZYMemory::FreeMemory(void *lp)
{
    HGLOBAL h;

    h=GlobalHandle(lp);

    GlobalUnlock(h);

    GlobalFree(h);
}
