//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cwnd_main.h"

#include "cvideo.h"
#include "csprite.h"
#include "cspritestorage.h"
#include "consts.h"

//====================================================================================================
//����� ��������� ������
//====================================================================================================
BEGIN_MESSAGE_MAP(CWnd_Main,CWnd)
 ON_WM_PAINT()
 ON_WM_CREATE()
 ON_WM_DESTROY()
 ON_WM_TIMER() 
 ON_WM_LBUTTONDOWN()
 ON_WM_RBUTTONDOWN()
 ON_WM_RBUTTONUP()
 ON_WM_LBUTTONUP()
 ON_WM_MOUSEMOVE()
 ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//====================================================================================================
//����������� � ���������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CWnd_Main::CWnd_Main(void)
{
 int32_t n,m;
 //������������� ��������� ������
 iVideo_Ptr.reset(new CVideo(300*NConsts::SIZE_SCALE,200*NConsts::SIZE_SCALE));
 ScreenBuffer.Set(new uint32_t[300*NConsts::SIZE_SCALE*200*NConsts::SIZE_SCALE]);
 iVideo_Ptr->SetVideoPointer(ScreenBuffer.Get(),300*NConsts::SIZE_SCALE);
 //������ ��������� ��������
 iSpriteStorage_Ptr.reset(new CSpriteStorage());

 sCursor.X=0;
 sCursor.Y=0;
 sCursor.Cross=false;
 sCursor.SelectBox=-1;
 sCursor.SelectIndex=-1;
 sCursor.OffsetSelectX=-1;
 sCursor.OffsetSelectY=-1;

 for(n=0;n<13;n++)
 {
  int32_t xl=0;
  int32_t yl=0;
  int32_t dx=0;
  int32_t dy=0;
  if (n<2)
  {
   xl=NConsts::BOX_0_1_OFFSET_X+NConsts::BOX_WIDTH*n;
   yl=NConsts::BOX_0_1_OFFSET_Y;
   xl*=NConsts::SIZE_SCALE;
   yl*=NConsts::SIZE_SCALE;
   dx=0;
   dy=0;
  }
  if (n>=2 && n<9)
  {
   xl=NConsts::BOX_2_8_OFFSET_X+NConsts::BOX_WIDTH*(n-2);
   yl=NConsts::BOX_2_8_OFFSET_Y;
   xl*=NConsts::SIZE_SCALE;
   yl*=NConsts::SIZE_SCALE;
   dx=0;
   dy=NConsts::CARD_DX_OFFSET*NConsts::SIZE_SCALE;
  }
  if (n>=9 && n<13)
  {
   xl=NConsts::BOX_9_12_OFFSET_X+(n-9)*NConsts::BOX_WIDTH;
   yl=NConsts::BOX_9_12_OFFSET_Y;
   xl*=NConsts::SIZE_SCALE;
   yl*=NConsts::SIZE_SCALE;
   dx=0;
   dy=0;
  }
  for(m=0;m<53;m++)
  {
   BoxXPos[n][m]=xl+dx*m;
   BoxYPos[n][m]=yl+dy*m;
  }
 }

 sCursor.Number[0]=0;
 sCursor.Number[1]=0;
 sCursor.Number[2]=0;
 sCursor.Number[3]=0;
 sCursor.Number[4]=0;
 
 TimerMode=TIMER_MODE_NONE;

 InitGame();
}
//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CWnd_Main::~CWnd_Main()
{ 
}
//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================
afx_msg int CWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 int32_t ret=CWnd::OnCreate(lpCreateStruct);
 Draw();
 SetTimer(ID_TIMER_MAIN,20,NULL);
 return(ret);
}
afx_msg void CWnd_Main::OnDestroy(void)
{
 KillTimer(ID_TIMER_MAIN);
 CWnd::OnDestroy();
}
afx_msg void CWnd_Main::OnPaint(void)
{ 
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);

 CPaintDC dc(this);
 BITMAPINFOHEADER bmih;
 bmih.biSize=sizeof(BITMAPINFOHEADER);
 bmih.biWidth=width;
 bmih.biHeight=-height;
 bmih.biPlanes=1;
 bmih.biBitCount=32;
 bmih.biCompression=BI_RGB;
 bmih.biSizeImage=0;
 bmih.biXPelsPerMeter=300;
 bmih.biYPelsPerMeter=300;
 bmih.biClrUsed=0;
 bmih.biClrImportant=0;
 RGBQUAD rgbq;
 BITMAPINFO info;
 rgbq.rgbBlue=1;
 rgbq.rgbGreen=0;
 rgbq.rgbRed=0;
 rgbq.rgbReserved=0;
 info.bmiHeader=bmih;
 info.bmiColors[0]=rgbq;

 StretchDIBits(dc.m_hDC,0,0,width,height,0,0,width,height,vptr,&info,DIB_RGB_COLORS,SRCCOPY);
 CWnd::OnPaint();
}
afx_msg void CWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_MAIN) 
 {
  CWnd::OnTimer(nIDEvent);
  return;
 }
 if (TimerMode==TIMER_MODE_NONE)
 {
  int32_t n;
  int32_t m;
  bool done=true;
  for(n=0;n<9;n++)
  {
   int32_t card=GetCardInBox(n);
   for(m=0;m<card;m++)
   {
    if (sCard_Box[n][m].Visible==false)
    {
     done=false;
     break;    	
    }
   }
  }
  //��������� �������������� ����
  if (done==true)
  {
   for(n=0;n<9;n++)
   {
    int32_t card=GetCardInBox(n);
    for(int32_t b=9;b<13;b++)
    {          	
     ChangeCard(n,card-1,b,GetCardInBox(b)-1);
     if (TimerMode!=TIMER_MODE_NONE) return;
    }    	
   }
  }
  return;
 }	
	
 if (TimerMode==TIMER_MODE_MOVE_CARD)
 {
  int32_t s_box=sTimerModeState.s_box;
  int32_t d_box=sTimerModeState.d_box;
  int32_t s_index=sTimerModeState.s_index;
  if ((sTimerModeState.xb!=sTimerModeState.xe || sTimerModeState.yb!=sTimerModeState.ye))
  {
   for(int32_t n=0;n<10;n++)
   {
    if (sTimerModeState.xb<sTimerModeState.xe) sTimerModeState.xb++;
    if (sTimerModeState.xb>sTimerModeState.xe) sTimerModeState.xb--;
    if (sTimerModeState.yb<sTimerModeState.ye) sTimerModeState.yb++;
    if (sTimerModeState.yb>sTimerModeState.ye) sTimerModeState.yb--;
   }
   //������ ����� ����������
   SCursor sCursorCopy=sCursor;
   sCursor.SelectBox=s_box;
   sCursor.SelectIndex=s_index;
   sCursor.OffsetSelectX=0;
   sCursor.OffsetSelectY=0;  	
   sCursor.X=sTimerModeState.xb;
   sCursor.Y=sTimerModeState.yb;
   Draw();
   sCursor=sCursorCopy;   
  }
  else 
  {
   ChangeBox(s_box,s_index,d_box);
   TimerMode=TIMER_MODE_NONE;
   Draw();
   if (CheckFinish()==true) DrawFinish();
  }
  return;
 }	
 
 if (TimerMode==TIMER_MODE_CONGRATULATION)
 {
  if (sTimerModeState.Counter>0) sTimerModeState.Counter--; 
  else
  {
   TimerMode=TIMER_MODE_NONE;
   InitGame();
  }
  Draw();
  return;
 }  
}
afx_msg BOOL CWnd_Main::OnEraseBkgnd(CDC *pDC)
{
 return(TRUE);
}
afx_msg void CWnd_Main::OnLButtonDown(UINT nFlags,CPoint point)
{
 bool left=false;
 bool right=false;
 bool center=false;
 if (nFlags&MK_MBUTTON) center=true;
 if (nFlags&MK_LBUTTON) left=true;
 if (nFlags&MK_RBUTTON) right=true;
 int32_t x=point.x;
 int32_t y=point.y;
 MouseControl(x,y,left,right,center);
}
afx_msg void CWnd_Main::OnLButtonUp(UINT nFlags,CPoint point)
{
 bool left=false;
 bool right=false;
 bool center=false;
 if (nFlags&MK_MBUTTON) center=true;
 if (nFlags&MK_LBUTTON) left=true;
 if (nFlags&MK_RBUTTON) right=true;
 int32_t x=point.x;
 int32_t y=point.y;
 MouseControl(x,y,left,right,center);
}
afx_msg void CWnd_Main::OnRButtonDown(UINT nFlags,CPoint point)
{
 bool left=false;
 bool right=false;
 bool center=false;
 if (nFlags&MK_MBUTTON) center=true;
 if (nFlags&MK_LBUTTON) left=true;
 if (nFlags&MK_RBUTTON) right=true;
 int32_t x=point.x;
 int32_t y=point.y;
 MouseControl(x,y,left,right,center);
}
afx_msg void CWnd_Main::OnRButtonUp(UINT nFlags,CPoint point)
{
 bool left=false;
 bool right=false;
 bool center=false;
 if (nFlags&MK_MBUTTON) center=true;
 if (nFlags&MK_LBUTTON) left=true;
 if (nFlags&MK_RBUTTON) right=true;
 int32_t x=point.x;
 int32_t y=point.y;
 MouseControl(x,y,left,right,center);
}
afx_msg void CWnd_Main::OnMouseMove(UINT nFlags,CPoint point)
{
 bool left=false;
 bool right=false;
 bool center=false;
 if (nFlags&MK_MBUTTON) center=true;
 if (nFlags&MK_LBUTTON) left=true;
 if (nFlags&MK_RBUTTON) right=true;
 int32_t x=point.x;
 int32_t y=point.y;
 MouseControl(x,y,left,right,center);
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//���������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::Draw(void)
{
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);
  
 DrawDesktop();
 DrawMenu();
 DrawMap();

 if (TimerMode==TIMER_MODE_CONGRATULATION)
 {
  int32_t x=(width-iSpriteStorage_Ptr->GetSpriteWinPtr()->GetWidth())/2;
  int32_t y=(height-iSpriteStorage_Ptr->GetSpriteWinPtr()->GetHeight())/2;
  iSpriteStorage_Ptr->GetSpriteWinPtr()->Put(iVideo_Ptr.get(),x,y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 }
 InvalidateRect(NULL,FALSE);
}
//----------------------------------------------------------------------------------------------------
//���������� �� �����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::MouseControl(int32_t x,int32_t y,bool left,bool right,bool center)
{ 
 if (TimerMode!=TIMER_MODE_NONE) return;

 sCursor.X=x;
 sCursor.Y=y;  
 
 //������� �� ������� ������ ������
 if (right==true)//������ ������ ������ �����
 {
  if (sCursor.Cross==false)
  {
   //����������, ��� �� �������
   if (GetSelectBoxParam(sCursor.X,sCursor.Y,&sCursor.SelectBox,&sCursor.SelectIndex)==true)//���� ������� ������
   {
    if (sCursor.SelectBox<9 && sCursor.SelectIndex>=0)//��������� �����
    {
     int32_t b;
     for(b=9;b<13;b++) ChangeCard(sCursor.SelectBox,sCursor.SelectIndex,b,GetCardInBox(b)-1);
     sCursor.SelectBox=-1;
     sCursor.SelectIndex=-1;
     sCursor.OffsetSelectX=-1;
     sCursor.OffsetSelectY=-1;
     Draw();
     return;
    }
   }
  }
 }
 if (left==true)//������ ����� ������ �����
 {
  if (sCursor.Cross==false)//���� ����� ������ ����� �� ���� ������ �� �����
  {
   sCursor.Cross=true;//����������, ��� �� ������ ����� ������ �����
   if (CheckMenu()==true) return;
   //����������, ��� �� �������
   if (GetSelectBoxParam(sCursor.X,sCursor.Y,&sCursor.SelectBox,&sCursor.SelectIndex)==true)//���� ������� ������
   {
    if (sCursor.SelectIndex>=0)//� ������� �����
    {
     if (sCard_Box[sCursor.SelectBox][sCursor.SelectIndex].Visible==true)
     {
      sCursor.OffsetSelectX=sCursor.X-BoxXPos[sCursor.SelectBox][sCursor.SelectIndex];
      sCursor.OffsetSelectY=sCursor.Y-BoxYPos[sCursor.SelectBox][sCursor.SelectIndex];
     }
     else
     {
      sCursor.SelectBox=-1;
      sCursor.SelectIndex=-1;
      sCursor.OffsetSelectX=-1;
      sCursor.OffsetSelectY=-1;
     }
    }
   }
  }
 }
 if (left==false)//��������� ����� ������ �����
 {
  if (sCursor.Cross==true)//���� ����� ������ ����� ���� ������, � ������ �������� - ����� ��������� �����-�� ��������
  {
   sCursor.Cross=false;
   //��������, ��� �� ��������� ����� ������ �����
   int32_t box;
   int32_t index;
   if (GetSelectBoxParam(sCursor.X,sCursor.Y,&box,&index)==true)
   {
    //���� ���� ��������� �����
    if (sCursor.SelectBox==box)//����� ������� ���� ��, ��� ��� � ����
    {
     if (box==1 || box==0) RotatePool();
     sCursor.SelectBox=-1;
     sCursor.SelectIndex=-1;
     sCursor.OffsetSelectX=-1;
     sCursor.OffsetSelectY=-1;
     Draw();
     return;
    }
    if (sCursor.SelectBox<2 && box<0)//����� ������ �������� � �������
    {
     sCursor.SelectBox=-1;
     sCursor.SelectIndex=-1;
     sCursor.OffsetSelectX=-1;
     sCursor.OffsetSelectY=-1;
     Draw();
     return;
    }
    if (sCursor.SelectBox>=0 && sCursor.SelectIndex>=0)//��������� �����
    {
     ChangeCard(sCursor.SelectBox,sCursor.SelectIndex,box,index);
     sCursor.SelectBox=-1;
     sCursor.SelectIndex=-1;
     sCursor.OffsetSelectX=-1;
     sCursor.OffsetSelectY=-1;
     Draw();
     return;
    }
   }
  }
  sCursor.SelectBox=-1;
  sCursor.SelectIndex=-1;
  sCursor.OffsetSelectX=-1;
  sCursor.OffsetSelectY=-1;
 }
 Draw();
}
//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
int32_t CWnd_Main::RND(int32_t max)
{
 return((int32_t)((float)(max)*((float)rand())/((float)RAND_MAX)));
}
//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawCard(int32_t x,int32_t y,int32_t value,CARD_SUIT suit)
{
 int32_t n;	
 if (value<1 || value>=14) return;//����� �������� ���
 int32_t suit_index=-1;
 if (suit==CARD_SUIT_SPADES) suit_index=0;
 if (suit==CARD_SUIT_HEARTS) suit_index=1;
 if (suit==CARD_SUIT_CLUBS) suit_index=2;
 if (suit==CARD_SUIT_DIAMONDS) suit_index=3;
 if (suit_index<0) return;//����� ������ ���
 iSpriteStorage_Ptr->GetSpriteCardPtr(value,suit_index)->Put(iVideo_Ptr.get(),x,y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);

 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);

 //������ �������������� ������ �����
 for(n=0;n<=NConsts::CARD_WIDTH*NConsts::SIZE_SCALE;n++)
 {
  uint32_t *v_ptr;
  int32_t x1=x+n;
  int32_t y1=y-1;
  if (x1>=0 && x1<width && y1>=0 && y1<height)
  {
   v_ptr=vptr+(x1+y1*linesize);
   *v_ptr=0xffUL<<24;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//����������� ����� �� ����� s � ������ d
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::MoveCard(int32_t s,int32_t d)
{
 int32_t n;
 int32_t s_end=0;
 int32_t d_end=0;
 //���� ������ ��������� ����� � ������
 for(n=0;n<53;n++)
 {
  s_end=n;
  if (sCard_Box[s][n].Value<0) break;
 }
 for(n=0;n<53;n++)
 {
  d_end=n;
  if (sCard_Box[d][n].Value<0) break;
 }
 if (s_end==0) return(false);//��������� ���� ����
 //����� ��������� �����
 sCard_Box[d][d_end]=sCard_Box[s][s_end-1];
 sCard_Box[s][s_end-1].Value=-1;//����� ��� ������ ���
 return(true);
}

//----------------------------------------------------------------------------------------------------
//�������� ���������� ���� � �����
//----------------------------------------------------------------------------------------------------
int32_t CWnd_Main::GetCardInBox(int32_t box)
{
 int32_t n;
 int32_t amount=0;
 for(n=0;n<53;n++)
 {
  if (sCard_Box[box][n].Value<0) break;
  amount++;
 }
 return(amount);
}
//----------------------------------------------------------------------------------------------------
//����������� ���� ������ ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::RotatePool(void)
{
 bool r=MoveCard(0,1);//���������� ����� �� �������� ����� � ������
 if (r==false)//���� ���
 {
  //���������� �������
  while(MoveCard(1,0)==true);
 }
}
//----------------------------------------------------------------------------------------------------
//���������������� �������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::InitGame(void)
{
 TimerMode=TIMER_MODE_NONE;
 int32_t value=sCursor.Number[0]+10*sCursor.Number[1]+100*sCursor.Number[2]+1000*sCursor.Number[3]+10000*sCursor.Number[4];
 srand(value);
 int32_t n,m,s;
 //���������� ��� ��������� ������ � �������� ���������
 for(s=0;s<13;s++)
  for(n=0;n<53;n++) sCard_Box[s][n].Value=-1;
 //�������� � �������� ���� �����
 int32_t index=0;
 CARD_SUIT suit[4]={CARD_SUIT_SPADES,CARD_SUIT_HEARTS,CARD_SUIT_CLUBS,CARD_SUIT_DIAMONDS};
 for(s=0;s<4;s++)
 {
  for(n=0;n<13;n++,index++)
  {
   sCard_Box[0][index].Value=n;//������ �����
   sCard_Box[0][index].Suit=suit[s];
   sCard_Box[0][index].Visible=true;
  }
 }
 //������ ������������ ����� �� ������
 for(n=0;n<7;n++)
 {
  for(m=0;m<=n;m++)
  {
   int32_t change=RND(100);
   for(s=0;s<=change;s++) RotatePool();//���������� �����
   //���������� �����
   if (MoveCard(0,n+2)==false)//���� ����� � ����� 0 - ������ ������
   {
    m--;
    continue;
   }
   int32_t amount=GetCardInBox(n+2);
   if (amount>0) sCard_Box[n+2][amount-1].Visible=false;//����� ��������
  }
 }
 //�������� ������� � �������� ���������
 while(1)
 {
  if (GetCardInBox(1)==0) break;//���� ����� � ����� 1
  RotatePool();//���������� �����
 }
}
//----------------------------------------------------------------------------------------------------
//������� ������ ����� ���� ����� ��������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::OnVisibleCard(void)
{
 int32_t n;
 for(n=2;n<9;n++)
 {
  int32_t amount=GetCardInBox(n);
  if (amount>0) sCard_Box[n][amount-1].Visible=true;
 }
}
//----------------------------------------------------------------------------------------------------
//���������� ���� � �������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawMap(void)
{
 int32_t n,m;
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);


 //������ ������� ������ ���
 OnVisibleCard();
 //������ ������ ��� ����
 for(int32_t s=0;s<13;s++)
 {
  int32_t x=BoxXPos[s][0];
  int32_t y=BoxYPos[s][0];
  for(n=x;n<=x+NConsts::CARD_WIDTH*NConsts::SIZE_SCALE;n++)
  {
   for(m=y;m<=y+NConsts::CARD_HEIGHT*NConsts::SIZE_SCALE;m++)
   {
    if (n>=0 && n<width && m>=0 && m<height)
    {
     uint32_t *v_ptr=vptr+(n+m*linesize);
	 *v_ptr=(0xff<<24)|(0x00<<16)|(0x40<<8)|(0x00);
    }
   }
  }
 }
 //������ �����
 for(n=0;n<13;n++)
 {
  int32_t amount;
  amount=GetCardInBox(n);
  for(m=0;m<amount;m++)
  {
   if (sCursor.SelectBox==n && sCursor.SelectIndex>=0 && sCursor.SelectIndex<=m) continue;
   //��� ����� �� �������
   if (sCard_Box[n][m].Visible==false) iSpriteStorage_Ptr->GetSpriteBackPtr()->Put(iVideo_Ptr.get(),BoxXPos[n][m],BoxYPos[n][m],false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
                                  else DrawCard(BoxXPos[n][m],BoxYPos[n][m],sCard_Box[n][m].Value+1,sCard_Box[n][m].Suit);
  }
 }
 //������ ��������� �����
 for(n=0;n<13;n++)
 {
  int32_t amount;
  amount=GetCardInBox(n);
  for(m=0;m<amount;m++)
  {
   if (sCursor.SelectBox==n && sCursor.SelectIndex<=m && sCursor.SelectIndex>=0)//��� ����� �������, �� ����� ����� ����������
   {
    int32_t offsetx=BoxXPos[sCursor.SelectBox][sCursor.SelectIndex]-sCursor.X+sCursor.OffsetSelectX;
    int32_t offsety=BoxYPos[sCursor.SelectBox][sCursor.SelectIndex]-sCursor.Y+sCursor.OffsetSelectY;
    DrawCard(BoxXPos[n][m]-offsetx,BoxYPos[n][m]-offsety,sCard_Box[n][m].Value+1,sCard_Box[n][m].Suit);//������ ������ �����
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//����������� ��� �� ���� � ����� ������ � ������ ������� ������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::GetSelectBoxParam(int32_t x,int32_t y,int32_t *box,int32_t *index)
{
 *box=-1;
 *index=-1;
 int32_t n,m;
 //�������� �� ������� "��������"
 for(n=0;n<13;n++)
 {
  int32_t amount;
  amount=GetCardInBox(n);
  for(m=0;m<=amount;m++)//���� m<=amount ������� 53-� ������ (����� ������� �� ������ �������)
  {
   int32_t xl=BoxXPos[n][m];
   int32_t yl=BoxYPos[n][m];
   int32_t xr=xl+NConsts::CARD_WIDTH*NConsts::SIZE_SCALE;
   int32_t yr=yl+NConsts::CARD_HEIGHT*NConsts::SIZE_SCALE;
   if (x>=xl && x<=xr && y>=yl && y<=yr)
   {
    *box=n;
    if (m<amount) *index=m;
   }
  }
 }
 if (*box<0) return(false);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//����������� ����� �� ������ ����� � ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::ChangeBox(int32_t s_box,int32_t s_index,int32_t d_box)
{
 int32_t n;
 int32_t d_end=0;
 //���� ������ ��������� ����� � ����� ����������
 for(n=0;n<52;n++)
 {
  d_end=n;
  if (sCard_Box[d_box][n].Value<0) break;
 }
 //���������� ���� ����� �� ���������� �����
 for(n=s_index;n<52;n++,d_end++)
 {
  if (sCard_Box[s_box][n].Value<0) break;
  sCard_Box[d_box][d_end]=sCard_Box[s_box][n];
  sCard_Box[s_box][n].Value=-1;//����� ��� ������ ���
 }
}
//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawDesktop(void)
{
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 int32_t y=0;
 while(y<height)
 {
  int32_t x=0;
  while(x<width)
  {
   iSpriteStorage_Ptr->GetSpriteDesktopPtr()->Put(iVideo_Ptr.get(),x,y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
   x+=iSpriteStorage_Ptr->GetSpriteDesktopPtr()->GetWidth();
  }
  y+=iSpriteStorage_Ptr->GetSpriteDesktopPtr()->GetHeight();
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawMenu(void)
{
 int32_t n;
 iSpriteStorage_Ptr->GetSpriteNumberFramePtr()->Put(iVideo_Ptr.get(),230*NConsts::SIZE_SCALE,0,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 iSpriteStorage_Ptr->GetSpriteStartPtr()->Put(iVideo_Ptr.get(),230*NConsts::SIZE_SCALE,40,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);

 iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->Put(iVideo_Ptr.get(),230*NConsts::SIZE_SCALE,108,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->Put(iVideo_Ptr.get(),230*NConsts::SIZE_SCALE,200,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 for(n=0;n<5;n++) iSpriteStorage_Ptr->GetSpriteNumberPtr(sCursor.Number[n])->Put(iVideo_Ptr.get(),240*2+n*20,13,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
}
//----------------------------------------------------------------------------------------------------
//���������� ����������� ����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawMoveCard(int32_t s_box,int32_t s_index,int32_t d_box)
{
 TimerMode=TIMER_MODE_MOVE_CARD;
 sTimerModeState.s_box=s_box;
 sTimerModeState.d_box=d_box;
 sTimerModeState.s_index=s_index;
 sTimerModeState.xb=BoxXPos[s_box][s_index];
 sTimerModeState.yb=BoxYPos[s_box][s_index];
 //����� �������� ����������
 int32_t d_index=GetCardInBox(d_box);
 sTimerModeState.xe=BoxXPos[d_box][d_index];
 sTimerModeState.ye=BoxYPos[d_box][d_index];
}
//----------------------------------------------------------------------------------------------------
//����������� ����� � ������ ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::ChangeCard(int32_t s_box,int32_t s_index,int32_t d_box,int32_t d_index)
{
 if (d_box>=2 && d_box<9)//���� ���� �� ������� ����
 {
  //���� �� ����, �� ������ ���� ����� ������ ������
  if (d_index<0)
  {
   if (sCard_Box[s_box][s_index].Value==12) ChangeBox(s_box,s_index,d_box);//���� ����� - ������, ���������� �
   return;
  }
  //�����, ������ ����� � ������� �������� � ������ �������� ������
  if (sCard_Box[d_box][d_index].Value<=sCard_Box[s_box][s_index].Value) return;//�������� ����� ������, ��� ��, ��� ���� � ������ �����
  if (sCard_Box[d_box][d_index].Value>sCard_Box[s_box][s_index].Value+1) return;//����� ������ ������ �����, ������������ �� �������� �� 1
  CARD_SUIT md=sCard_Box[d_box][d_index].Suit;
  CARD_SUIT ms=sCard_Box[s_box][s_index].Suit;
  if ((md==CARD_SUIT_SPADES || md==CARD_SUIT_CLUBS) && (ms==CARD_SUIT_SPADES || ms==CARD_SUIT_CLUBS)) return;//����� ����� ���������
  if ((md==CARD_SUIT_HEARTS || md==CARD_SUIT_DIAMONDS) && (ms==CARD_SUIT_HEARTS || ms==CARD_SUIT_DIAMONDS)) return;//����� ����� ���������
  ChangeBox(s_box,s_index,d_box);//�������� �����
  return;
 }
 if (d_box>=9 && d_box<13)//���� ���� �� ���� ������
 {
  //���� ������� ��������� ����, �� ��� ���������� ����� ������ - ������ �� �����
  if (GetCardInBox(s_box)>s_index+1) return;
  //���� ���� ����, �� ������ ���� ����� ������ ����
  if (d_index<0)
  {
   if (sCard_Box[s_box][s_index].Value==0)//���� ����� - ���, ���������� �
   {
    DrawMoveCard(s_box,s_index,d_box);   
   }
   return;
  }
  //�����, ������ ����� � ������� ����������� � ���������� �������� ������
  if (sCard_Box[d_box][d_index].Value>sCard_Box[s_box][s_index].Value) return;//�������� ����� ������, ��� ��, ��� ���� � ������ �����
  if (sCard_Box[d_box][d_index].Value+1<sCard_Box[s_box][s_index].Value) return;//����� ������ ������ �����, ������������ �� �������� �� 1
  CARD_SUIT md=sCard_Box[d_box][d_index].Suit;
  CARD_SUIT ms=sCard_Box[s_box][s_index].Suit;
  if (ms!=md) return;//����� �� ���������
  DrawMoveCard(s_box,s_index,d_box);
  return;
 }
}
//----------------------------------------------------------------------------------------------------
//��������� �� ����������� �������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::CheckFinish(void)
{
 int32_t n;
 for(n=9;n<13;n++)
 {
  if (GetCardInBox(n)!=13) return(false);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������ � ����
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::CheckMenu(void)
{
 int32_t n,m;
 //��������, �� ������ �� �� ������� �� ������� ����
 if (sCursor.Y>=13 && sCursor.Y<=13+iSpriteStorage_Ptr->GetSpriteNumberPtr(0)->GetHeight())
 {
  if (sCursor.X>=240*NConsts::SIZE_SCALE && sCursor.X<=240*NConsts::SIZE_SCALE+20*5)
  {
   int32_t index=(sCursor.X-240*NConsts::SIZE_SCALE)/(20);
   sCursor.Number[index]++;
   sCursor.Number[index]%=10;
   return(true);
  }
 }
 if (sCursor.Y>=40 && sCursor.Y<=40+iSpriteStorage_Ptr->GetSpriteStartPtr()->GetHeight())
 {
  if (sCursor.X>=230*NConsts::SIZE_SCALE && sCursor.X<=230*NConsts::SIZE_SCALE+iSpriteStorage_Ptr->GetSpriteStartPtr()->GetWidth())
  {
   InitGame();
   OnPaint();
   return(true);
  }
 }
 if (sCursor.Y>=108 && sCursor.Y<=108+iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->GetHeight())
 {
  if (sCursor.X>=230*NConsts::SIZE_SCALE && sCursor.X<=230*NConsts::SIZE_SCALE+iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->GetWidth())
  {
   //��������� ���������
   FILE *file=fopen("state.bin","rb");
   if (file==NULL) return(true);//������
   //��������� ��������� ����
   for(n=0;n<5;n++) fread(&sCursor.Number[n],sizeof(uint8_t),1,file);
   for(n=0;n<13;n++)
   {
    for(m=0;m<53;m++)
    {
     fread(&sCard_Box[n][m],sizeof(struct SCard),1,file);
    }
   }
   fclose(file);
   OnPaint();
   return(true);
  }
 }
 if (sCursor.Y>=200 && sCursor.Y<=200+iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->GetHeight())
 {
  if (sCursor.X>=230*NConsts::SIZE_SCALE && sCursor.X<=230*NConsts::SIZE_SCALE+iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->GetWidth())
  {
   //��������� ���������
   FILE *file=fopen("state.bin","wb");
   if (file==NULL) return(true);//������
   //��������� ��������� ����
   for(n=0;n<5;n++) fwrite(&sCursor.Number[n],sizeof(uint8_t),1,file);
   for(n=0;n<13;n++)
   {
    for(m=0;m<53;m++)
    {
     fwrite(&sCard_Box[n][m],sizeof(struct SCard),1,file);
    }
   }
   fclose(file);
   OnPaint();
   return(true);
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//������������ � �������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawFinish(void)
{
 TimerMode=TIMER_MODE_CONGRATULATION;
 sTimerModeState.Counter=5*50;
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================
