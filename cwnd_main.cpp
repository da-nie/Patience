//====================================================================================================
//������������ ����������
//====================================================================================================
#include "cwnd_main.h"

#include "cvideo.h"
#include "csprite.h"
#include "cspritestorage.h"
#include "consts.h"

#include "craiifilein.h"
#include "craiifileout.h"

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
 int32_t right_border=0;//������ ������� ������
 int32_t bottom_border=0;//������ ������� ������

 //������ ��������� ��������
 iSpriteStorage_Ptr.reset(new CSpriteStorage());
 //�������������� ������
 cCursor.Init();
 //�������������� ����� ��� ����
 InitBox(right_border,bottom_border);
 //�������������� ������
 InitButton(right_border,bottom_border);
 //��������� �������
 right_border+=NConsts::BOX_WIDTH_OFFSET;
 bottom_border+=NConsts::BOX_HEIGHT_OFFSET;
 //�������������� �����
 InitScreen(right_border,bottom_border);
 //�������������� ����� ������ �������
 TimerMode=TIMER_MODE_NONE;
 //�������������� ����
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

//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
afx_msg int CWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 int32_t ret=CWnd::OnCreate(lpCreateStruct);
 //������������� ��������� ����
 SetWindowPos(NULL,Rect_Window.left,Rect_Window.right,Rect_Window.right-Rect_Window.left,Rect_Window.bottom-Rect_Window.top,SWP_NOMOVE);
 
 Draw();
 SetTimer(ID_TIMER_MAIN,20,NULL);
 return(ret);
}
//----------------------------------------------------------------------------------------------------
//����������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Main::OnDestroy(void)
{
 KillTimer(ID_TIMER_MAIN);
 CWnd::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//����������� ����
//----------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
afx_msg void CWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent!=ID_TIMER_MAIN) 
 {
  CWnd::OnTimer(nIDEvent);
  return;
 }

 if (TimerMode==TIMER_MODE_NONE)
 {
  size_t n;
  size_t m;
  bool done=true;
  for(n=0;n<9;n++)
  {
   size_t card=cCardBox[n].deque_CCard.size();
   for(m=0;m<card;m++)
   {
    if (cCardBox[n].deque_CCard[m].Visible==false)
    {
     done=false;
     break;    	
    }
   }
  }
  //��������� �������������� ����, ���� ��� ����� �������
  if (done==true)
  {
   for(n=0;n<9;n++)
   {
    //�������� ���������� ����� � ������ ������� ����
    int32_t card=cCardBox[n].deque_CCard.size();
    for(int32_t b=9;b<13;b++)
    {          
     ChangeCard(cCardBox[n],cCardBox[b],1);
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
   for(int32_t n=0;n<NConsts::NUMBER_AMOUNT;n++)
   {
    if (sTimerModeState.xb<sTimerModeState.xe) sTimerModeState.xb++;
    if (sTimerModeState.xb>sTimerModeState.xe) sTimerModeState.xb--;
    if (sTimerModeState.yb<sTimerModeState.ye) sTimerModeState.yb++;
    if (sTimerModeState.yb>sTimerModeState.ye) sTimerModeState.yb--;
   }
   //������ ����� ����������
   CCursor cCursorCopy=cCursor;
   cCursor.SelectedBoxIndex=s_box;
   cCursor.SelectedPositionIndexInBox=s_index;
   cCursor.cCoord_OffsetWithSelectedPosition.X=0;
   cCursor.cCoord_OffsetWithSelectedPosition.Y=0;  	
   cCursor.cCoord_Position.X=sTimerModeState.xb;
   cCursor.cCoord_Position.Y=sTimerModeState.yb;
   Draw();
   cCursor=cCursorCopy;   
  }
  else 
  {
   int32_t card_amount=cCardBox[s_box].deque_CCard.size()-s_index;
   ChangeBox(cCardBox[s_box],cCardBox[d_box],card_amount);
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
//----------------------------------------------------------------------------------------------------
//������� �������� ����
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CWnd_Main::OnEraseBkgnd(CDC *pDC)
{
 return(TRUE);
}
//----------------------------------------------------------------------------------------------------
//������ ����� ������ �����
//----------------------------------------------------------------------------------------------------
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
 MouseControl(CCoord(x,y),left,right,center);
}
//----------------------------------------------------------------------------------------------------
//�������� ����� ������ �����
//----------------------------------------------------------------------------------------------------
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
 MouseControl(CCoord(x,y),left,right,center);
}
//----------------------------------------------------------------------------------------------------
//������ ������ ������ �����
//----------------------------------------------------------------------------------------------------
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
 MouseControl(CCoord(x,y),left,right,center);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ������ �����
//----------------------------------------------------------------------------------------------------
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
 MouseControl(CCoord(x,y),left,right,center);
}
//----------------------------------------------------------------------------------------------------
//����� ������������
//----------------------------------------------------------------------------------------------------
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
 MouseControl(CCoord(x,y),left,right,center);
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������������� ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::InitBox(int32_t &right_border,int32_t &bottom_border)
{
 //�������������� ������ ��� ����
 uint32_t card_width=iSpriteStorage_Ptr->GetSpriteCardPtr(1,0)->GetWidth();
 uint32_t card_height=iSpriteStorage_Ptr->GetSpriteCardPtr(1,0)->GetHeight();
 uint32_t box_space_width=card_width+NConsts::BOX_GAP;
 uint32_t box_space_height=card_height+NConsts::BOX_GAP;
 uint32_t card_dy_offset=(card_height>>2);//�������� ��� ������ ����� ����
 //������� ������
 right_border=0;
 bottom_border=0;

 for(uint32_t n=0;n<NConsts::BOX_AMOUNT;n++)
 {
  int32_t xl=0;
  int32_t yt=0;
  int32_t dx=0;
  int32_t dy=0;
  if (n<2)//�������
  {
   xl=NConsts::BOX_WIDTH_OFFSET+box_space_width*n;
   yt=NConsts::BOX_HEIGHT_OFFSET;
   dx=0;
   dy=0;
  }
  if (n>=2 && n<9)//�������
  {
   xl=NConsts::BOX_WIDTH_OFFSET+box_space_width*(n-2);
   yt=NConsts::BOX_HEIGHT_OFFSET+card_height+NConsts::BOX_GAP;
   dx=0;
   dy=card_dy_offset;
  }
  if (n>=9 && n<13)//���� ������
  {
   xl=NConsts::BOX_WIDTH_OFFSET+(n-9+3)*box_space_width;
   yt=NConsts::BOX_HEIGHT_OFFSET;
   dx=0;
   dy=0;
  }
  cCardBox[n].Index=n;
  cCardBox[n].cRectangle.Set(CCoord(xl,yt),CCoord(card_width,card_height));
  cCardBox[n].cCoord_Offset.X=dx;
  cCardBox[n].cCoord_Offset.Y=dy;

  if (right_border<cCardBox[n].cRectangle.GetRightX()) right_border=cCardBox[n].cRectangle.GetRightX();
  if (bottom_border<cCardBox[n].cRectangle.GetBottomY()) bottom_border=cCardBox[n].cRectangle.GetBottomY();
 }
 right_border+=NConsts::BOX_WIDTH_OFFSET;
}

//----------------------------------------------------------------------------------------------------
//������������� ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::InitButton(int32_t &right_border,int32_t &bottom_border)
{
 int32_t control_left_border=right_border;
 int32_t control_top_border=0;

 //����� ������� ��������
 int32_t frame_width=iSpriteStorage_Ptr->GetSpriteNumberFramePtr()->GetWidth();
 int32_t frame_height=iSpriteStorage_Ptr->GetSpriteNumberFramePtr()->GetHeight();

 int32_t number_width=iSpriteStorage_Ptr->GetSpriteNumberPtr(0)->GetWidth();
 int32_t number_height=iSpriteStorage_Ptr->GetSpriteNumberPtr(0)->GetHeight();

 int32_t number_offset_x=control_left_border+(frame_width-number_width*NConsts::PATIENCE_NUMBER_AMOUNT)/2;
 int32_t number_offset_y=NConsts::BOX_HEIGHT_OFFSET+(frame_height-number_height)/2;

 cRectangle_NumberFrame.Set(CCoord(control_left_border,NConsts::BOX_HEIGHT_OFFSET),CCoord(frame_width,frame_height));
 right_border=cRectangle_NumberFrame.GetRightX();
 control_top_border=cRectangle_NumberFrame.GetBottomY();

 for(uint32_t n=0;n<NConsts::PATIENCE_NUMBER_AMOUNT;n++)
 {
  cCursor.PatienceNumber[n]=0;
  cRectangle_PatienceNumber[n].Set(CCoord(number_offset_x+number_width*n,number_offset_y),CCoord(number_width,number_height));
  if (right_border<cRectangle_PatienceNumber[n].GetRightX()) right_border=cRectangle_PatienceNumber[n].GetRightX();
  if (control_top_border<cRectangle_PatienceNumber[n].GetBottomY()) control_top_border=cRectangle_PatienceNumber[n].GetBottomY();
 }
 control_top_border+=NConsts::BUTTON_HEIGHT_GAP;

 int32_t save_width=iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->GetWidth();
 int32_t save_height=iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->GetHeight();
 int32_t load_width=iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->GetWidth();
 int32_t load_height=iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->GetHeight();
 int32_t start_width=iSpriteStorage_Ptr->GetSpriteStartPtr()->GetWidth();
 int32_t start_height=iSpriteStorage_Ptr->GetSpriteStartPtr()->GetHeight();
  
 cRectangle_Start.Set(CCoord(control_left_border,control_top_border),CCoord(start_width,start_height));
 control_top_border+=NConsts::BUTTON_HEIGHT_GAP+start_height;
 if (right_border<cRectangle_Start.GetRightX()) right_border=cRectangle_Start.GetRightX();
 cRectangle_LoadState.Set(CCoord(control_left_border,control_top_border),CCoord(load_width,load_height));
 control_top_border+=NConsts::BUTTON_HEIGHT_GAP+load_height;
 if (right_border<cRectangle_LoadState.GetRightX()) right_border=cRectangle_LoadState.GetRightX();
 cRectangle_SaveState.Set(CCoord(control_left_border,control_top_border),CCoord(save_width,save_height));
 control_top_border+=NConsts::BUTTON_HEIGHT_GAP+save_height;
 if (right_border<cRectangle_SaveState.GetRightX()) right_border=cRectangle_SaveState.GetRightX();
 
 uint32_t card_height=iSpriteStorage_Ptr->GetSpriteCardPtr(1,0)->GetHeight();
 uint32_t card_dy_offset=(card_height>>2);//�������� ��� ������ ����� ����

 bottom_border+=card_dy_offset*NConsts::CARD_STACK_AMOUNT_IN_SCREEN;
 if (bottom_border<control_top_border)  bottom_border=control_top_border;
}

//----------------------------------------------------------------------------------------------------
//������������� ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::InitScreen(int32_t &right_border,int32_t &bottom_border)
{
 //������������� ��������� ������
 uint32_t screen_width=right_border;
 uint32_t screen_height=bottom_border;

 iVideo_Ptr.reset(new CVideo(screen_width,screen_height));
 ScreenBuffer.Set(new uint32_t[screen_width*screen_height]);
 iVideo_Ptr->SetVideoPointer(ScreenBuffer.Get(),screen_width);
  
 //��������� �� ��� ��������� ����
 Rect_Window.left=0;
 Rect_Window.right=screen_width;
 Rect_Window.top=0;
 Rect_Window.bottom=screen_height;
 AdjustWindowRect(&Rect_Window,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,FALSE);
}

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
void CWnd_Main::MouseControl(const CCoord &cCoord,bool left,bool right,bool center)
{ 
 if (TimerMode!=TIMER_MODE_NONE) return;
 cCursor.cCoord_Position=cCoord;
 //������� �� ������� ������ ������
 if (right==true)//������ ������ ������ ����� (����������)
 {
  AutoCollection();
  return;
 }
 if (left==true)//������ ����� ������ ����� (������ ���� � ����� ������ ����)
 {
  if (CheckMenu()==true) return;
  CaptureCard();
 }
 if (left==false)//��������� ����� ������ ����� (������������ ����)
 {
  ReleaseCard();
 }
 Draw();
}

//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::AutoCollection(void)
{
 if (cCursor.MoveMode==false)
 {
  //����������, ��� �� �������
  bool empty;
  if (GetSelectBoxParam(cCursor.cCoord_Position,cCursor.SelectedBoxIndex,cCursor.SelectedPositionIndexInBox,empty)==true)//���� ������� ������
  {
   if (cCursor.SelectedBoxIndex<9 && empty==false && cCursor.SelectedPositionIndexInBox>=0)//��������� �����
   {
    int32_t b;
    for(b=9;b<NConsts::BOX_AMOUNT;b++) 
    {
     if (cCursor.SelectedPositionIndexInBox==cCardBox[cCursor.SelectedBoxIndex].deque_CCard.size()-1) ChangeCard(cCardBox[cCursor.SelectedBoxIndex],cCardBox[b],1);
    }
	cCursor.ResetSelected();
    Draw();
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//������ ����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::CaptureCard(void)
{
 if (cCursor.MoveMode==false)//���� ����� ������ ����� �� ���� ������ �� �����
 {
  cCursor.MoveMode=true;//����������, ��� �� ������ ����� ������ �����  
  //����������, ��� �� �������
  bool empty;
  if (GetSelectBoxParam(cCursor.cCoord_Position,cCursor.SelectedBoxIndex,cCursor.SelectedPositionIndexInBox,empty)==true)//���� ������� ������
  {
   if (empty==false)//� ������� �����
   {
    if (cCardBox[cCursor.SelectedBoxIndex].deque_CCard[cCursor.SelectedPositionIndexInBox].Visible==true)
    {
     //��������� �������� ������� ������������ ��������� �����
     cCursor.cCoord_OffsetWithSelectedPosition.X=cCursor.cCoord_Position.X-(cCardBox[cCursor.SelectedBoxIndex].cRectangle.cCoord_Position.X+cCardBox[cCursor.SelectedBoxIndex].cCoord_Offset.X*cCursor.SelectedPositionIndexInBox);
	 cCursor.cCoord_OffsetWithSelectedPosition.Y=cCursor.cCoord_Position.Y-(cCardBox[cCursor.SelectedBoxIndex].cRectangle.cCoord_Position.Y+cCardBox[cCursor.SelectedBoxIndex].cCoord_Offset.Y*cCursor.SelectedPositionIndexInBox);
    }
    else
    {
	 cCursor.ResetSelected();
    }
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//������������ ����
//----------------------------------------------------------------------------------------------------
void CWnd_Main::ReleaseCard(void)
{
 if (cCursor.MoveMode==true)//���� ����� ������ ����� ���� ������, � ������ �������� - ����� ��������� �����-�� ��������
 {
  cCursor.MoveMode=false;
  //��������, ��� �� ��������� ����� ������ �����
  int32_t box;
  int32_t index;
  bool empty;
  if (GetSelectBoxParam(cCursor.cCoord_Position,box,index,empty)==true)
  {
   //���� ���� ��������� �����
   if (cCursor.SelectedBoxIndex==box)//����� ������� ���� ��, ��� ��� � ����
   {
    if (box==1 || box==0) RotatePool();
    cCursor.ResetSelected();
    Draw();
    return;
   }
   if (cCursor.SelectedBoxIndex<2 && box<0)//����� ������ �������� � �������
   {
    cCursor.ResetSelected();
    Draw();
    return;
   }
   if (cCursor.SelectedBoxIndex>=0 && cCursor.SelectedPositionIndexInBox>=0)//��������� �����
   {
    int32_t card_amount=cCardBox[cCursor.SelectedBoxIndex].deque_CCard.size()-cCursor.SelectedPositionIndexInBox;
    ChangeCard(cCardBox[cCursor.SelectedBoxIndex],cCardBox[box],card_amount);
    cCursor.ResetSelected();
    Draw();
    return;
   }
  }
 }
 cCursor.ResetSelected();
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
void CWnd_Main::DrawCard(const CCoord &cCoord,int32_t value,CARD_SUIT suit)
{
 int32_t n;	
 if (value<0 || value>=NConsts::CARD_UNIT_AMOUNT) return;//����� �������� ���
 int32_t suit_index=-1;
 if (suit==CARD_SUIT_SPADES) suit_index=0;
 if (suit==CARD_SUIT_HEARTS) suit_index=1;
 if (suit==CARD_SUIT_CLUBS) suit_index=2;
 if (suit==CARD_SUIT_DIAMONDS) suit_index=3;
 if (suit_index<0) return;//����� ������ ���
 iSpriteStorage_Ptr->GetSpriteCardPtr(value,suit_index)->Put(iVideo_Ptr.get(),cCoord.X,cCoord.Y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);

 uint32_t card_width=iSpriteStorage_Ptr->GetSpriteCardPtr(value,suit_index)->GetWidth();

 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);

 //������ �������������� ������ �����
 for(n=0;n<=card_width;n++)
 {
  uint32_t *v_ptr;
  int32_t x1=cCoord.X+n;
  int32_t y1=cCoord.Y-1;
  if (x1>=0 && x1<width && y1>=0 && y1<height)
  {
   v_ptr=vptr+(x1+y1*linesize);
   *v_ptr=(0xffUL<<24)|(0x00<<16)|(0x00<<8)|(0x00<<0);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//����������� ����� �� ������ � ������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::MoveCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination)
{
 if (cCardBox_Source.deque_CCard.empty()==true) return(false);//�������� ���� ����
 cCardBox_Destination.deque_CCard.push_back(cCardBox_Source.deque_CCard.back());
 cCardBox_Source.deque_CCard.pop_back();
 return(true);
}
//----------------------------------------------------------------------------------------------------
//����������� ���� ������ ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::RotatePool(void)
{
 bool empty=MoveCard(cCardBox[0],cCardBox[1]);//���������� ����� �� �������� ����� � ������
 if (empty==false)//���� ���
 {
  //���������� �������
  while(MoveCard(cCardBox[1],cCardBox[0])==true);
 }
}
//----------------------------------------------------------------------------------------------------
//���������������� �������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::InitGame(void)
{
 TimerMode=TIMER_MODE_NONE;
 int32_t value=cCursor.PatienceNumber[0]+10*cCursor.PatienceNumber[1]+100*cCursor.PatienceNumber[2]+1000*cCursor.PatienceNumber[3]+10000*cCursor.PatienceNumber[4];
 srand(value);
 //���������� ��� ��������� ������ � �������� ���������
 for(int32_t s=0;s<NConsts::CARD_UNIT_AMOUNT;s++) cCardBox[s].deque_CCard.clear();
 //�������� � �������� ���� �����
 int32_t index=0;
 CARD_SUIT suit[NConsts::CARD_COLOR_AMOUNT]={CARD_SUIT_SPADES,CARD_SUIT_HEARTS,CARD_SUIT_CLUBS,CARD_SUIT_DIAMONDS};
 for(int32_t s=0;s<NConsts::CARD_COLOR_AMOUNT;s++)
 {
  for(int32_t n=0;n<NConsts::CARD_UNIT_AMOUNT;n++,index++)
  {
   CCard cCard;
   cCard.Suit=suit[s];
   cCard.Value=n;
   cCard.Visible=true;
   cCardBox[0].deque_CCard.push_back(cCard);
  }
 }
 
 //������ ������������ ����� �� ������
 for(int32_t n=0;n<7;n++)
 {
  for(int32_t m=0;m<=n;m++)
  {
   int32_t change=RND(100);
   for(int32_t s=0;s<=change;s++) RotatePool();//���������� �����
   //���������� �����
   if (MoveCard(cCardBox[0],cCardBox[n+2])==false)//���� ����� � ����� 0 - ������ ������
   {
    m--;
    continue;
   }
   //������ ����� ���������
   if (cCardBox[n+2].deque_CCard.empty()==false) cCardBox[n+2].deque_CCard.back().Visible=false;
  }
 }
 //�������� ������� � �������� ���������
 while(1)
 {
  if (cCardBox[1].deque_CCard.empty()==true) break;//���� ����� � ����� 1
  RotatePool();//���������� �����
 }
}
//----------------------------------------------------------------------------------------------------
//������� ������ ����� ���� ����� ��������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::OnVisibleCard(void)
{
 for(int32_t n=2;n<9;n++)
 {
  if (cCardBox[n].deque_CCard.empty()==false) cCardBox[n].deque_CCard.back().Visible=true;
 }
}
//----------------------------------------------------------------------------------------------------
//���������� ���� � �������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::DrawMap(void)
{
 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);

 //������ ������� ������ ���
 OnVisibleCard();
 //������ ������ ��� ����
 for(int32_t s=0;s<NConsts::BOX_AMOUNT;s++)
 {
  int32_t xl=cCardBox[s].cRectangle.GetLeftX();
  int32_t yt=cCardBox[s].cRectangle.GetTopY();
  int32_t xr=cCardBox[s].cRectangle.GetRightX();
  int32_t yb=cCardBox[s].cRectangle.GetBottomY();
  for(int32_t n=xl;n<xr;n++)
  {
   for(int32_t m=yt;m<yb;m++)
   {
    if (n>=0 && n<width && m>=0 && m<height)
    {
     uint32_t *v_ptr=vptr+(n+m*linesize);
	 *v_ptr=(0xff<<24)|(0x00<<16)|(0x40<<8)|(0x00<<0);
    }
   }
  }
 }
 //������ �����
 for(int32_t n=0;n<NConsts::BOX_AMOUNT;n++)
 {
  size_t size=cCardBox[n].deque_CCard.size();
  for(size_t m=0;m<size;m++)
  {
   if (cCursor.SelectedBoxIndex==n && cCursor.SelectedPositionIndexInBox>=0 && cCursor.SelectedPositionIndexInBox<=m) continue;
   //��� ����� �� �������
   int32_t x=cCardBox[n].cRectangle.cCoord_Position.X+cCardBox[n].cCoord_Offset.X*m;
   int32_t y=cCardBox[n].cRectangle.cCoord_Position.Y+cCardBox[n].cCoord_Offset.Y*m;

   if (cCardBox[n].deque_CCard[m].Visible==false) iSpriteStorage_Ptr->GetSpriteBackPtr()->Put(iVideo_Ptr.get(),x,y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
                                             else DrawCard(CCoord(x,y),cCardBox[n].deque_CCard[m].Value,cCardBox[n].deque_CCard[m].Suit);
  }
 }
 //������ ��������� �����
 for(int32_t n=0;n<NConsts::BOX_AMOUNT;n++)
 {
  size_t size=cCardBox[n].deque_CCard.size();
  for(size_t m=0;m<size;m++)
  {
   if (cCursor.SelectedBoxIndex==n && cCursor.SelectedPositionIndexInBox<=m && cCursor.SelectedPositionIndexInBox>=0)//��� ����� �������, �� ����� ����� ����������
   {
    //�������� ��������� ���� � ������
    int32_t x=cCardBox[cCursor.SelectedBoxIndex].cRectangle.cCoord_Position.X+cCardBox[cCursor.SelectedBoxIndex].cCoord_Offset.X*cCursor.SelectedPositionIndexInBox;
    int32_t y=cCardBox[cCursor.SelectedBoxIndex].cRectangle.cCoord_Position.Y+cCardBox[cCursor.SelectedBoxIndex].cCoord_Offset.Y*cCursor.SelectedPositionIndexInBox;

    int32_t offsetx=x-cCursor.cCoord_Position.X+cCursor.cCoord_OffsetWithSelectedPosition.X;
    int32_t offsety=y-cCursor.cCoord_Position.Y+cCursor.cCoord_OffsetWithSelectedPosition.Y;

    x=cCardBox[n].cRectangle.cCoord_Position.X+cCardBox[n].cCoord_Offset.X*m;
    y=cCardBox[n].cRectangle.cCoord_Position.Y+cCardBox[n].cCoord_Offset.Y*m;
    DrawCard(CCoord(x-offsetx,y-offsety),cCardBox[n].deque_CCard[m].Value,cCardBox[n].deque_CCard[m].Suit);//������ ������ �����
   }
  }
 }
}
//----------------------------------------------------------------------------------------------------
//����������� ��� �� ���� � ����� ������ � ������ ������� ������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::GetSelectBoxParam(const CCoord &cCoord,int32_t &box,int32_t &index,bool &empty)
{
 box=-1;
 index=-1;
 empty=true;
 //�������� �� �������
 for(int32_t n=0;n<NConsts::BOX_AMOUNT;n++)
 {
  size_t size=cCardBox[n].deque_CCard.size();
  bool is_empty=false;
  if (size==0)
  {
   size=1;
   is_empty=true;
  }
  for(size_t m=0;m<size;m++)
  {
   int32_t xl=cCardBox[n].cRectangle.cCoord_Position.X+cCardBox[n].cCoord_Offset.X*m;
   int32_t yt=cCardBox[n].cRectangle.cCoord_Position.Y+cCardBox[n].cCoord_Offset.Y*m;
   int32_t width=cCardBox[n].cRectangle.cCoord_Size.X;
   int32_t height=cCardBox[n].cRectangle.cCoord_Size.Y;

   CRectangle cRectangle(CCoord(xl,yt),CCoord(width,height));

   if (cRectangle.IsPointInside(cCoord)==true)
   {
    box=n;
	empty=is_empty;
    if (m<size) index=m;
   }
  }
 }
 if (box<0) return(false);//� ������ �� ������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//����������� ��������� ���� �� ����� ������ � ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::ChangeBox(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount)
{
 size_t size=cCardBox_Source.deque_CCard.size();
 if (size<card_amount)
 {
  MessageBox("���� ����!","������",MB_OK);
  return;
 } 
 size_t position=size-card_amount;
 while(position<size)
 {  
  cCardBox_Destination.deque_CCard.push_back(cCardBox_Source.deque_CCard[position]);
  position++;
 }
 while(card_amount>0)
 {
  if (cCardBox_Source.deque_CCard.empty()==true) break;//�������� ���� ����
  cCardBox_Source.deque_CCard.pop_back();
  card_amount--;
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
 iSpriteStorage_Ptr->GetSpriteNumberFramePtr()->Put(iVideo_Ptr.get(),cRectangle_NumberFrame.GetLeftX(),cRectangle_NumberFrame.GetTopY(),false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 for(uint32_t n=0;n<NConsts::PATIENCE_NUMBER_AMOUNT;n++) 
 {
  int32_t x=cRectangle_PatienceNumber[n].GetLeftX();
  int32_t y=cRectangle_PatienceNumber[n].GetTopY();
  iSpriteStorage_Ptr->GetSpriteNumberPtr(cCursor.PatienceNumber[n])->Put(iVideo_Ptr.get(),x,y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 }

 iSpriteStorage_Ptr->GetSpriteStartPtr()->Put(iVideo_Ptr.get(),cRectangle_Start.GetLeftX(),cRectangle_Start.GetTopY(),false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 iSpriteStorage_Ptr->GetSpriteLoadStatePtr()->Put(iVideo_Ptr.get(),cRectangle_LoadState.GetLeftX(),cRectangle_LoadState.GetTopY(),false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
 iSpriteStorage_Ptr->GetSpriteSaveStatePtr()->Put(iVideo_Ptr.get(),cRectangle_SaveState.GetLeftX(),cRectangle_SaveState.GetTopY(),false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
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
 sTimerModeState.xb=cCardBox[s_box].cRectangle.cCoord_Position.X+cCardBox[s_box].cCoord_Offset.X*s_index;
 sTimerModeState.yb=cCardBox[s_box].cRectangle.cCoord_Position.Y+cCardBox[s_box].cCoord_Offset.Y*s_index;
 //����� �������� ����������
 size_t size=cCardBox[s_box].deque_CCard.size();
 sTimerModeState.xe=cCardBox[d_box].cRectangle.cCoord_Position.X+cCardBox[d_box].cCoord_Offset.X*size;
 sTimerModeState.ye=cCardBox[d_box].cRectangle.cCoord_Position.Y+cCardBox[d_box].cCoord_Offset.Y*size;
}
//----------------------------------------------------------------------------------------------------
//����������� ����� � ������ ������
//----------------------------------------------------------------------------------------------------
void CWnd_Main::ChangeCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount)
{
 size_t source_size=cCardBox_Source.deque_CCard.size();
 if (source_size<card_amount) return;
 int32_t selected_card_index=source_size-card_amount;
 CCard &cCard_SourceFirst=cCardBox_Source.deque_CCard[selected_card_index];

 if (cCardBox_Destination.Index>=2 && cCardBox_Destination.Index<9)//���� ���� �� ������� ����
 {
  //���� �� ����, �� ������ ���� ����� ������ ������
  if (cCardBox_Destination.deque_CCard.empty()==true)
  {
   if (cCard_SourceFirst.Value==12) ChangeBox(cCardBox_Source,cCardBox_Destination,card_amount);//���� ����� - ������, ���������� �
   return;
  }
  //�����, ������ ����� � ������� �������� � ������ �������� ������
  CCard &cCard_DestinationLast=cCardBox_Destination.deque_CCard.back();

  if (cCard_DestinationLast.Value<=cCard_SourceFirst.Value) return;//�������� ����� ������, ��� ��, ��� ���� � ������ �����
  if (cCard_DestinationLast.Value>cCard_SourceFirst.Value+1) return;//����� ������ ������ �����, ������������ �� �������� �� 1
  CARD_SUIT md=cCard_DestinationLast.Suit;
  CARD_SUIT ms=cCard_SourceFirst.Suit;
  if ((md==CARD_SUIT_SPADES || md==CARD_SUIT_CLUBS) && (ms==CARD_SUIT_SPADES || ms==CARD_SUIT_CLUBS)) return;//����� ����� ���������
  if ((md==CARD_SUIT_HEARTS || md==CARD_SUIT_DIAMONDS) && (ms==CARD_SUIT_HEARTS || ms==CARD_SUIT_DIAMONDS)) return;//����� ����� ���������
  ChangeBox(cCardBox_Source,cCardBox_Destination,card_amount);//�������� �����
  return;
 }
 if (cCardBox_Destination.Index>=9 && cCardBox_Destination.Index<13)//���� ���� �� ���� ������
 {
  //���� ������� ��������� ����, �� ��� ���������� ����� ������ - ������ �� �����
  if (card_amount>1) return;
  //���� ���� ����, �� ������ ���� ����� ������ ����
  if (cCardBox_Destination.deque_CCard.empty()==true)
  {
   if (cCard_SourceFirst.Value==0)//���� ����� - ���, ���������� �
   {
	DrawMoveCard(cCardBox_Source.Index,selected_card_index,cCardBox_Destination.Index);   
   }
   return;
  }
  //�����, ������ ����� � ������� ����������� � ���������� �������� ������
  CCard &cCard_DestinationLast=cCardBox_Destination.deque_CCard.back();

  if (cCard_DestinationLast.Value>cCard_SourceFirst.Value) return;//�������� ����� ������, ��� ��, ��� ���� � ������ �����
  if (cCard_DestinationLast.Value+1<cCard_SourceFirst.Value) return;//����� ������ ������ �����, ������������ �� �������� �� 1
  CARD_SUIT md=cCard_DestinationLast.Suit;
  CARD_SUIT ms=cCard_SourceFirst.Suit;
  if (ms!=md) return;//����� �� ���������
  DrawMoveCard(cCardBox_Source.Index,selected_card_index,cCardBox_Destination.Index);   
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
  if (cCardBox[n].deque_CCard.size()!=13) return(false);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������ � ����
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::CheckMenu(void)
{
 //��������, �� ������ �� �� ����� ��������
 for(uint32_t n=0;n<NConsts::PATIENCE_NUMBER_AMOUNT;n++) 
 {
  if (cRectangle_PatienceNumber[n].IsPointInside(cCursor.cCoord_Position)==true)
  {
   cCursor.PatienceNumber[n]++;
   cCursor.PatienceNumber[n]%=10;
   return(true);
  }
 }
 //��������, �� ������� �� �� ������������� ��������
 if (cRectangle_Start.IsPointInside(cCursor.cCoord_Position)==true)
 {
  InitGame();
  OnPaint();
  return(true);
 }
 //��������, �� ������� �� �� ���������� �������
 if (cRectangle_SaveState.IsPointInside(cCursor.cCoord_Position)==true)
 {
  if (SaveState()==false) MessageBox("���������� �� �������!","������",MB_OK);
  OnPaint();
  return(true);
 }
 //��������, �� ������� �� �� �������� �������
 if (cRectangle_LoadState.IsPointInside(cCursor.cCoord_Position)==true)
 {
  if (LoadState()==false)
  {
   MessageBox("�������� �� �������! ������� �����������.","������",MB_OK);
   InitGame();
  }
  OnPaint();
  return(true);
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

//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::LoadState(void)
{
 CRAIIFileIn cRAIIFileIn(NConsts::STATE_FILE_NAME,std::ios_base::in|std::ios_base::binary);
 if (cRAIIFileIn.IsOpened()==false) return(false);
 cCursor.LoadState(cRAIIFileIn.GetHandle());
 for(int32_t n=0;n<NConsts::BOX_AMOUNT;n++) cCardBox[n].LoadState(cRAIIFileIn.GetHandle());
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CWnd_Main::SaveState(void)
{
 CRAIIFileOut cRAIIFileOut(NConsts::STATE_FILE_NAME,std::ios_base::out|std::ios_base::binary);
 if (cRAIIFileOut.IsOpened()==false) return(false);
 cCursor.SaveState(cRAIIFileOut.GetHandle());
 for(int32_t n=0;n<NConsts::BOX_AMOUNT;n++) cCardBox[n].SaveState(cRAIIFileOut.GetHandle());
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================
