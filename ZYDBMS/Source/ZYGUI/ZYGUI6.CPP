//---------------------------------------------------------
//���½�������(ZYGUI)-����ؼ� ����:����
//---------------------------------------------------------
#include "ZYGUI1.HPP"

//������ɫ1
#define GRID_COLOR1 0x00FFE0E0

//������ɫ2
#define GRID_COLOR2 0x00FFD2D2

//��������ؼ�����
IZYGrid *IZYGrid_Create(IZYTable *table)
{
    return (IZYGrid *)new ZYGrid(table);
}

//�ͷű���ؼ�����
void IZYGrid_Release(IZYGrid *grid)
{
    delete (ZYGrid *)grid;
}

//�������ؼ�����
ZYGrid::ZYGrid(IZYTable *table)
{
    this->table=table;
    this->item=0;
    this->column=0;
    this->i_input=-1;
    this->j_input=-1;
    this->unit_width=120;
    this->unit_height=22;
    this->editData=NULL;
    this->titleBitmap=NULL;
    this->scrollBar=IZYScrollBar_Create(0,10,table->GetRowCount(),EScrollType_Vertical);
    this->AddControl(scrollBar->ToControl());
}

//ת��Ϊ�ؼ�����
IZYControl *ZYGrid::ToControl(void)
{
    return (IZYControl *)this;
}

//������ʾ�ı���
void ZYGrid::SetTable(IZYTable *table)
{
    this->table=table;

    SetLocSize(x,y,m_display,unit_width,unit_height);

    frame->Invalidate();
}

//���ñ���ͼƬ
void ZYGrid::SetTitleBitmap(IZYBitmap *titleBitmap)
{
    this->titleBitmap=titleBitmap;
}

//��ȡ��ֱ�������ؼ�
IZYScrollBar *ZYGrid::GetScrollBarV(void)
{
    return this->scrollBar;
}

//���ÿؼ�λ�óߴ�
void ZYGrid::SetLocSize(int x,int y,int m_display,int unit_width,int unit_height)
{
    int n=table->GetColumnCount();

    int m=table->GetRowCount();

    this->x=x;

    this->y=y;

    this->m_display=m_display;

    this->scrollBar->SetParam(0,m_display,m);

    this->unit_width=unit_width;

    this->unit_height=unit_height;

    this->width=n*unit_width+unit_height;

    this->height=2*unit_height+m_display*unit_height;

    this->scrollBar->ToControl()->SetLocSize(n*unit_width,2*unit_height,unit_height,m_display*unit_height);
}

//���¹�������
void ZYGrid::UpdateScrollParam(int i_base)
{
    int m=table->GetRowCount();
    scrollBar->SetParam(i_base,m_display,m);
}

//�����ؼ��ػ�
void ZYGrid::OnPaint(IZYGraphics *graphics)
{
    int j,k;

    int n=table->GetColumnCount();

    int m=table->GetRowCount();

    int w=unit_width;

    int h=unit_height;

    int i_base=scrollBar->GetIndexBase();

    //�ؼ��ػ�
    ZYControl::OnPaint(graphics);

    //�������
    OutputTableName(graphics,x,y,width,h);

    //�����б�����
    DrawGirds(graphics,x,y+h,w,h,1,n);

    //�����½���ť
    DrawCrossIcon(graphics,x+n*unit_width,y+unit_height,unit_height);

    //���Ƶ�ǰѡ�������
    DrawItemBar(graphics);

    //�������ݱ�����
    DrawGirds(graphics,x,y+2*h,w,h,m>m_display?m_display:m,n);

    //����û��ʵ��ĸ���
    if(m<m_display)
    {
        graphics->FillRectangle1(x,y+(2+m)*h,n*w,(m_display-m)*h,GRID_COLOR1);
    }

    //�����������
    for(j=0;j<n;j++)
    {
        //�������
        OutputColumnName(graphics,j,x+j*w,y+h,w,h);

        for(k=0;k+i_base<m&&k<m_display;k++)
        {
            //�������
            OutputData(graphics,k+i_base,j,x+j*w,y+(k+2)*h,w,h);
        }
    }

    //�����ػ�
    ZYContainer::OnPaint(graphics);
}

//�������
void ZYGrid::OutputTableName(IZYGraphics *graphics,int x,int y,int w,int h)
{
    char table_name[100];

    graphics->FillRectangle1(x,y,width,unit_height,GRID_COLOR1);
    
    if(titleBitmap!=NULL)
    {
        graphics->DrawBitmap(titleBitmap,x,y,w,h);
    }

    graphics->DrawRectangle(x,y,w,h);

    table->GetName(table_name);

    graphics->OutputText(table_name,x,y,w,h,DT_LEFT);
}

//�������
void ZYGrid::OutputColumnName(IZYGraphics *graphics,int j,int x,int y,int w,int h)
{
    char name[1000];

    table->GetColumn(j,"����",name);

    graphics->FillRectangle1(x,y,w,h,GRID_COLOR1);

    if(titleBitmap!=NULL)
    {
        graphics->DrawBitmap(titleBitmap,x,y,w,h);
    }

    graphics->DrawRectangle(x,y,w,h);
        
    graphics->OutputText(name,x,y,w,h,DT_LEFT);
}

//��6��С���ض�Ϊ2��С��
void ZYGrid::ProcessDouble(char *str_double)
{
    int i=0;
    
    while(str_double[i]!='.')
    {
        i++;
    }
    
    str_double[i+3]='\0';
}

//�������
void ZYGrid::OutputData(IZYGraphics *graphics,int i,int j,int x,int y,int w,int h)
{
    EColumnType type;
    int a;
    double d;
    char s[1000];
    char buf[1000];

    table->GetColumn(j,"����",&type);

    switch(type)
    {
    case 0:
        table->GetData(i,j,s);
        wsprintf(buf,"%s",s);
        break;
    case 1:
        table->GetData(i,j,&d);
        sprintf(buf,"%f",d);
        ProcessDouble(buf);
        break;
    case 2:
        table->GetData(i,j,&a);
        wsprintf(buf,"%d",a);
        break;
    }

    if(type==EColumnType_String)
    {
        graphics->OutputText(buf,x,y,w,h,DT_LEFT);
    }
    else if(type==EColumnType_Double)
    {
        graphics->OutputText(buf,x,y,w,h,DT_RIGHT);
    }
    else if(type==EColumnType_Integer)
    {
        graphics->OutputText(buf,x,y,w,h,DT_RIGHT);
    }
}

//���Ʊ�����
void ZYGrid::DrawGirds(IZYGraphics *graphics,int x,int y,int w,int h,int m,int n)
{
    int i;

    for(i=0;i<=m;i++)
    {
        graphics->DrawLine(x,y+i*h,x+n*w,y+i*h);
    }

    for(i=0;i<=n;i++)
    {
        graphics->DrawLine(x+i*w,y,x+i*w,y+m*h+1);
    }
}

// ����ʮ�ּ�ͷ
void ZYGrid::DrawCrossIcon(IZYGraphics *graphics,int x,int y,int h)
{
    graphics->FillRectangle1(x,y,h,h,GRID_COLOR1);

    if(titleBitmap!=NULL)
    {
        graphics->DrawBitmap(titleBitmap,x,y,h,h);
    }   

    graphics->DrawRectangle(x,y,h,h);

    graphics->DrawLine(x+h/6,y+h/2,x+h-h/6,y+h/2);

    graphics->DrawLine(x+h/2,y+h/6,x+h/2,y+h-h/6);
}

//���Ƶ�ǰѡ���и�����
void ZYGrid::DrawItemBar(IZYGraphics *graphics)
{
    int i_base=scrollBar->GetIndexBase();

    int n=table->GetColumnCount();

    //��ǳ��ɫ����ѡ������

    if(ZYFrame::focusControl!=this->ToControl())
    {
        if(this->editData==NULL)
        {
            return;
        }

        if(ZYFrame::focusControl!=this->editData->ToControl())
        {
            return;
        }
    }

    if(item>=i_base&&item<i_base+m_display)
    {
        graphics->FillRectangle1(x,y+(item-i_base+2)*unit_height,unit_width*n,unit_height,GRID_COLOR2);
    }
}

//���Ƶ�ǰѡ����
void ZYGrid::DecSelectedItem(void)
{
    int i_base=scrollBar->GetIndexBase();

    int m=table->GetRowCount();

    if(m>0)
    {
        if(item==-1)
        {
            item=0;
        }
        if(item>0)
        {   
            item--;
        }
        if(i_base>item)
        {
            i_base=item;
        }
        if(i_base+m_display<item+1)
        {
            i_base=item-m_display+1;
        }

        UpdateScrollParam(i_base);

        Invalidate();
    }
}

//���Ƶ�ǰѡ����
void ZYGrid::IncSelectedItem(void)
{
    int i_base=scrollBar->GetIndexBase();

    int m=table->GetRowCount();

    if(m>0)
    {
        if(item<m-1)
        {   
            item++;
        }
        if(i_base+m_display<item+1)
        {
            i_base=item-m_display+1;
        }
        if(i_base>item)
        {
            i_base=item;
        }

        UpdateScrollParam(i_base);

        Invalidate();
    }
}

//������
void ZYGrid::AppendItem(void)
{
    table->AppendRow();

    EndSelectedItem();

    Invalidate();
}

//ɾ����ǰѡ����
bool ZYGrid::DeleteSelectedItem(void)
{
    int i_base=scrollBar->GetIndexBase();

    int m=table->GetRowCount();

    if(item>=0&&item<m)
    {
        table->DeleteRow(item);

        m=table->GetRowCount();

        if(item>=m)
        {
            item--;
        }

        if(i_base+m_display>m)
        {
            i_base--;
        }
        
        if(m_display>m)
        {
            i_base=0;
        }

        if(m==0)
        {
            item=0;
        }

        UpdateScrollParam(i_base);

        Invalidate();

        return true;
    }

    return false;
}

//��ҳ���Ƶ�ǰѡ����
void ZYGrid::DecSelectedItemPage(void)
{
    int i;
    for(i=0;i<m_display;i++)
    {
        DecSelectedItem();
    }
}

//��ҳ���Ƶ�ǰѡ����
void ZYGrid::IncSelectedItemPage(void)
{
    int i;
    for(i=0;i<m_display;i++)
    {
        IncSelectedItem();
    }
}

//��ǰѡ���лص���ʼ
void ZYGrid::HomeSelectedItem(void)
{
    item=0;
    UpdateScrollParam(0);
    Invalidate();
}

//��ǰѡ����ȥ������
void ZYGrid::EndSelectedItem(void)
{
    int m=table->GetRowCount();
    item=m-1;
    if(m>m_display)
    {
        UpdateScrollParam(m-m_display);
    }
    else
    {
        UpdateScrollParam(0);
    }
    Invalidate();
}

//�������̰���
void ZYGrid::OnKeyDown(int vkey)
{
    ZYControl::OnKeyDown(vkey);

    //������ͬ�Ĺ���
    switch(vkey)
    {
    case VK_UP:
        DecSelectedItem();
        break;
    case VK_DOWN:
        IncSelectedItem();
        break;
    case VK_PRIOR:
        DecSelectedItemPage();
        break;
    case VK_NEXT:
        IncSelectedItemPage();
        break;
    case VK_HOME:
        HomeSelectedItem();
        break;
    case VK_END:
        EndSelectedItem();
        break;
    case VK_DELETE:
        DeleteSelectedItem();
        break;
    case VK_RETURN:
        //������ڱ༭״̬��
        if(editData!=NULL)
        {
            LeaveEditMode();
            this->SetFocus(true);
        }
        break;
    }

    Invalidate();
}

//���Ե��Ƿ��ھ�����
bool ZYGrid::PointInRect(int p_x,int p_y,int x,int y,int width,int height)
{
    return (p_x>=x&&p_x<=x+width&&p_y>=y&&p_y<=y+height);
}

//������갴��
void ZYGrid::OnMouseDown(int mx,int my)
{
    int edit_x,edit_y,edit_width,edit_height;

    ZYContainer::OnMouseDown(mx,my);

    int i_base=scrollBar->GetIndexBase();

    int n=table->GetColumnCount();

    int m=table->GetRowCount();

    //������ڱ༭״̬��
    if(editData!=NULL)
    {
        editData->ToControl()->GetLocSize(&edit_x,&edit_y,&edit_width,&edit_height);

        if(!PointInRect(mx,my,x+edit_x,y+edit_y,edit_width,edit_height))
        {
            LeaveEditMode();
        }
        else
        {
            return;
        }
    }

    //�½���ť
    if(PointInRect(mx,my,x+n*unit_width,y+unit_height,unit_height,unit_height))
    {
        AppendItem();
    }
    //����������
    else if(PointInRect(mx,my,x,y+2*unit_height,unit_width*n,m_display*unit_height))
    {
        int item=i_base+(my-y-2*unit_height)/unit_height;

        if(item<m)
        {
            this->item=item;

            column=(mx-x)/unit_width;

            Invalidate();
        }
    }

    this->SetFocus(true);
}

//��ȡ����ʽ����������
void ZYGrid::GetTableData(char *s,IZYTable *table,int i,int j)
{
    double d;

    int a;

    EColumnType type;

    table->GetColumn(j,"����",&type);

    s[0]='\0';

    switch(type)
    {
    case EColumnType_String:
        table->GetData(i,j,s);
        break;
    case EColumnType_Double:
        table->GetData(i,j,&d);
        if(d!=0.0)
        {
            sprintf(s,"%f",d);
            ProcessDouble(s);
        }
        else
        {
            s[0]='\0';
        }
        break;
    case EColumnType_Integer:
        table->GetData(i,j,&a);
        if(a!=0)
        {
            wsprintf(s,"%d",a);
        }
        else
        {
            s[0]='\0';
        }
        break;
    }
}

//��ʽ�������ñ�������
void ZYGrid::PutTableData(char *s,IZYTable *table,int i,int j)
{
    double d;

    int a;

    EColumnType type;

    table->GetColumn(j,"����",&type);

    switch(type)
    {
    case EColumnType_String:
        table->SetData(i,j,s);
        break;
    case EColumnType_Double:
        d=atof(s);
        table->SetData(i,j,&d);
        break;
    case EColumnType_Integer:
        a=atoi(s);
        table->SetData(i,j,&a);
        break;
    }
}

//�뿪�༭ģʽ
void ZYGrid::LeaveEditMode(void)
{
    char buf_input[1000];

    editData->GetEditText(buf_input,1000);

    PutTableData(buf_input,table,i_input,j_input);

    this->RemoveControl(editData->ToControl());

    editData=NULL;
}

//�������˫��
void ZYGrid::OnMouseDoubleClick(int mx,int my)
{
    HWND hWnd;
    WPARAM wParam;
    LPARAM lParam;

    char buf_input[1000];

    int x_edit,y_edit;

    int i_base=scrollBar->GetIndexBase();

    int n=table->GetColumnCount();

    int m=table->GetRowCount();

    if(editData==NULL&&PointInRect(mx,my,x,y+2*unit_height,unit_width*n,(m<m_display?m:m_display)*unit_height))
    {
        i_input=(my-y)/unit_height-2+i_base;

        j_input=(mx-x)/unit_width;

        GetTableData(buf_input,table,i_input,j_input);

        x_edit=j_input*unit_width;

        y_edit=(i_input+2-i_base)*unit_height;

        //���ӱ༭�ӿؼ�
        editData=IZYEdit_Create("");

        AddControl(editData->ToControl());
        
        editData->SetEditText(buf_input);

        editData->ToControl()->SetLocSize(x_edit,y_edit,unit_width,unit_height);

        editData->ToControl()->SetFocus(true);

        return;
    }

    //ģ��һ����굥��
    
    hWnd=frame->GetWindowHandle();

    if(this->container!=NULL)
    {
        mx+=this->container->GetXBase();
        my+=this->container->GetYBase();
    }

    wParam=0;

    lParam=MAKELONG(mx,my);

    PostMessage(hWnd,WM_LBUTTONDOWN,wParam,lParam);

    Invalidate();
}

//����������
void ZYGrid::OnMouseWheel(int mw)
{
    ZYControl::OnMouseWheel(mw);

    //������ڱ༭״̬��
    if(editData!=NULL)
    {
        LeaveEditMode();
    }

    if(mw>0)
    {
        DecSelectedItem();  
    }
    else
    {
        IncSelectedItem();
    }
}