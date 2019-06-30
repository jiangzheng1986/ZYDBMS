//--------------------------------------------------------------
//逐月存储引擎(ZYRDB)-其他功能 作者:蒋政
//--------------------------------------------------------------
#include "ZYRDB1.HPP"

//闰年各月天数表
static int day_month1[12]={31,29,31,30,31,30,31,31,30,31,30,31};

//非闰年各月天数表
static int day_month2[12]={31,28,31,30,31,30,31,31,30,31,30,31};

//创建日期时间对象
IZYDateTime *IZYDateTime_Create(int year,int month,int day)
{
    return (IZYDateTime *)new ZYDateTime(year,month,day);
}

//获取日期时间对象
IZYDateTime *IZYDateTime_Today(void)
{
    return (IZYDateTime *)new ZYDateTime();
}

//释放日期时间对象
void IZYDateTime_Release(IZYDateTime *datetime)
{
    delete (ZYDateTime *)datetime;
}       

//构造日期时间对象
ZYDateTime::ZYDateTime(void)
{
    GetLocalTime(&datetime);
}

//构造日期时间对象
ZYDateTime::ZYDateTime(int year,int month,int day)
{
    datetime.wYear=year;

    datetime.wMonth=month;

    datetime.wDay=day;
}

//获取年信息
int ZYDateTime::GetYear(void)
{
    return datetime.wYear;
}

//获取月信息
int ZYDateTime::GetMonth(void)
{
    return datetime.wMonth;
}

//获取日信息
int ZYDateTime::GetDay(void)
{
    return datetime.wDay;
}
//获取时信息
int ZYDateTime::GetHour(void)
{
    return datetime.wHour;
}

//获取分信息
int ZYDateTime::GetMinute(void)
{
    return datetime.wMinute;
}

//获取秒信息
int ZYDateTime::GetSecond(void)
{
    return datetime.wSecond;
}

//是否是闰年
bool ZYDateTime::IsRunNian(int year)
{
    if(year%400==0)        //整除400,是闰年
    {
        return 1;
    }
    else if(year%100==0)   //不整除400,整除100,不是闰年
    {
        return 0;     
    }
    else if(year%4==0)     //不整除400,不整除100,整除4,是闰年
    {
        return 1;
    }
    else                   //不整除400,不整除100,不整除4,不是闰年
    {
        return 0;
    }
}

//计算到1980.01.01的天数
int ZYDateTime::Substract1980(void)
{
    int year;

    int month;

    int day;

    int n=0;             //距1980.01.01的天数

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

//计算日期之间的间隔
int ZYDateTime::Substract(IZYDateTime *datetime)
{
    int n1;

    int n2;

    n1=this->Substract1980();

    n2=((ZYDateTime *)datetime)->Substract1980();

    return n1-n2;
}

//创建随机数发生器对象
IZYRandom *IZYRandom_Create(void)
{
    return (IZYRandom *)new ZYRandom();
}

//释放随机数发生器对象
void IZYRandom_Release(IZYRandom *random)
{
    delete (ZYRandom *)random;
}

//构造随机数发生器对象
ZYRandom::ZYRandom(void)
{
    SYSTEMTIME datetime; 

    GetLocalTime(&datetime);

    srand(datetime.wMilliseconds);
}

//生成属于[min,max]区间的随机整数
int ZYRandom::RandInt(int min,int max)
{
    return min+rand()%(max-min+1);
}

//生成属于[min,max]区间的随机小数
double ZYRandom::RandDouble(double min,double max)
{
    return (double)(RandInt((int)(min*100),(int)(max*100)))/100.0f;
}

//拷贝一个变长字符串到定长字符串中
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

//拷贝一个定长字符串到变长字符串中
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

//将一个定长字符串与变长字符串比较,判断是否相等
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

//复制一个字符串
char *ZYString::Copy(char *s)
{
    char *s1=(char *)malloc(strlen(s)+1);
    strcpy(s1,s);
    return s1;
}

//释放复制的字符串
void ZYString::Free(char *s)
{
    free(s);
}

//创建内存空间对象
IZYMemory *IZYMemory_Create(int size)
{
    return (IZYMemory *)new ZYMemory(size);
}

//释放内存空间对象
void IZYMemory_Release(IZYMemory *memory)
{
    delete (ZYMemory *)memory;
}

//构造内存空间对象
ZYMemory::ZYMemory(int size)
{
    this->lp=ApplyMemory(size);
    this->size=size;
}

//析构内存空间对象
ZYMemory::~ZYMemory(void)
{
    FreeMemory(lp);
}

//获取内存空间指针
void *ZYMemory::GetPointer(void)
{
    return lp;
}

//获取内存空间大小
int ZYMemory::GetSize(void)
{
    return size;
}

//调整内存空间大小
void *ZYMemory::Resize(int size)
{
    return lp=ResizeMemory(lp,size);
}

//申请内存空间
void *ZYMemory::ApplyMemory(int byte_count)
{
    HGLOBAL h;

    void *lp;

    h=GlobalAlloc(0,byte_count);

    lp=GlobalLock(h);

    return lp;
}

//获取内存空间大小
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

//调整内存空间大小
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

//释放内存空间
void ZYMemory::FreeMemory(void *lp)
{
    HGLOBAL h;

    h=GlobalHandle(lp);

    GlobalUnlock(h);

    GlobalFree(h);
}

