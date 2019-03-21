#ifndef C_WND_MAIN
#define C_WND_MAIN

//====================================================================================================
//������� ����� ���������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "stdafx.h"
#include <stdint.h>
#include "consts.h"

#include "ivideo.h"
#include "isprite.h"
#include "ispritestorage.h"
#include "cuniquearrayptr.h"

#include <memory>
#include <deque>
#include <algorithm>
#include <fstream>


//����������
class CCoord
{
 public:
  int32_t X;
  int32_t Y;
  CCoord(void)
  {
   X=0;
   Y=0;
   }
  CCoord(int32_t x,int32_t y)
  {
   X=x;
   Y=y;
  }
};


//������������� �������
class CRectangle
{   
 public:
  CCoord cCoord_Position;//��������� �� ������
  CCoord cCoord_Size;//������
 CRectangle(void)
 {
 }
 CRectangle(const CCoord &cCoord_position,const CCoord &cCoord_size)
 {
  Set(cCoord_position,cCoord_size);
 }

  bool IsPointInside(CCoord cCoord)//��������� �� ����� ������
  {
   if (cCoord.X<cCoord_Position.X) return(false);
   if (cCoord.X>=cCoord_Position.X+cCoord_Size.X) return(false);
   if (cCoord.Y<cCoord_Position.Y) return(false);
   if (cCoord.Y>=cCoord_Position.Y+cCoord_Size.Y) return(false);
   return(true);
  }
  void Set(const CCoord &cCoord_position,const CCoord &cCoord_size)
  {
   cCoord_Position=cCoord_position;
   cCoord_Size=cCoord_size;
  }
  int32_t GetLeftX(void)
  {
   return(cCoord_Position.X);
  }
  int32_t GetTopY(void)
  {
   return(cCoord_Position.Y);
  }
  int32_t GetRightX(void)
  {
   return(cCoord_Position.X+cCoord_Size.X);
  }
  int32_t GetBottomY(void)
  {
   return(cCoord_Position.Y+cCoord_Size.Y);
  }
};

  //�����
  enum CARD_SUIT
  {
   //����
   CARD_SUIT_SPADES,
   //�����
   CARD_SUIT_HEARTS,
   //�����
   CARD_SUIT_CLUBS,
   //����
   CARD_SUIT_DIAMONDS
  };


 //�����
 class CCard
 {
  public:
   CARD_SUIT Suit;//�����
   int32_t Value;//�������� ����� �� ������ �� ����
   bool Visible;//true-����� ������
  bool SaveState(std::ofstream &file)
  {
   if (file.write(reinterpret_cast<char*>(&Suit),sizeof(CARD_SUIT)).fail()==true) return(false);
   if (file.write(reinterpret_cast<char*>(&Value),sizeof(uint32_t)).fail()==true) return(false);
   if (file.write(reinterpret_cast<char*>(&Visible),sizeof(bool)).fail()==true) return(false);
   return(true);
  }
  bool LoadState(std::ifstream &file)
  {
   if (file.read(reinterpret_cast<char*>(&Suit),sizeof(CARD_SUIT)).fail()==true) return(false);
   if (file.read(reinterpret_cast<char*>(&Value),sizeof(uint32_t)).fail()==true) return(false);
   if (file.read(reinterpret_cast<char*>(&Visible),sizeof(bool)).fail()==true) return(false);
   return(true);
  }   
 };


//���� ��� ����
class CCardBox
{
 public:
  uint32_t Index;//������ �����
  CRectangle cRectangle;//��������� ����� �� ������
  CCoord cCoord_Offset;//�������� ��� ������ ��������� �����
  std::deque<CCard> deque_CCard;//����� � ������
  bool SaveState(std::ofstream &file)
  {
   size_t size=deque_CCard.size();
   if (file.write(reinterpret_cast<char*>(&size),sizeof(size_t)).fail()==true) return(false);
   for(size_t n=0;n<size;n++)
   {
    if (deque_CCard[n].SaveState(file)==false) return(false);
   };
   return(true);
  }
  bool LoadState(std::ifstream &file)
  {
   size_t size;
   if (file.read(reinterpret_cast<char*>(&size),sizeof(size_t)).fail()==true) return(false);
   deque_CCard.clear();
   for(size_t n=0;n<size;n++)
   {
    CCard cCard;
	if (cCard.LoadState(file)==false) return(false);
    deque_CCard.push_back(cCard);
   }
   return(true);
  }
};

//������
class CCursor
{
 public:
  CCoord cCoord_Position;//������� �������
  bool MoveMode;//���� ������ �����������
  int32_t SelectedBoxIndex;//��������� ����
  int32_t SelectedPositionIndexInBox;//��������� ������ �����
  CCoord cCoord_OffsetWithSelectedPosition;//�������� ������� ������������ ��������� ������ �����
  uint8_t PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//����� ��������
  CCursor(void)
  {
   Init();
  }
  void Init(void)
  {
   cCoord_Position.X=0;
   cCoord_Position.Y=0;
   MoveMode=false;
   ResetSelected();
  }
  void ResetSelected(void)
  {
   SelectedBoxIndex=-1;
   SelectedPositionIndexInBox=-1;
   cCoord_OffsetWithSelectedPosition.X=-1;
   cCoord_OffsetWithSelectedPosition.Y=-1;
  }
  bool SaveState(std::ofstream &file)
  {
   if (file.write(reinterpret_cast<char*>(&PatienceNumber),sizeof(uint8_t)*NConsts::PATIENCE_NUMBER_AMOUNT).fail()==true) return(false);
   return(true);
  }
  bool LoadState(std::ifstream &file)
  {
   if (file.read(reinterpret_cast<char*>(&PatienceNumber),sizeof(uint8_t)*NConsts::PATIENCE_NUMBER_AMOUNT).fail()==true) return(false);
   return(true);
  }
};



//====================================================================================================
//������� ����� ���������
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-���������� ������----------------------------------------------------------------------------------
  RECT Rect_Window;//������ ����
  //����� ������ �������
  enum TIMER_MODE
  {
   //������ �� ������������
   TIMER_MODE_NONE,
   //������ ������������ ��� ����������� �����
   TIMER_MODE_MOVE_CARD,
   //������ ������������ ��� ����������� ���� ��� ��������
   TIMER_MODE_CONGRATULATION
  };

   //�����
  CUniqueArrayPtr<UINT> ScreenBuffer;//��������� �� ���������� �����������
  std::unique_ptr<IVideo> iVideo_Ptr;//��������� �� ����� ������ � ������������
  std::unique_ptr<ISpriteStorage> iSpriteStorage_Ptr;//��������� �� ����� ��������� ��������

  TIMER_MODE TimerMode;//����� ������������� �������
  
  struct STimerModeState
  {
   //��� ������ ����������� ����
   int32_t s_box;
   int32_t s_index;
   int32_t d_box;
   int32_t xb;
   int32_t yb;
   int32_t xe;
   int32_t ye;
   //��� ������ ������������
   int32_t Counter;
  } sTimerModeState;


  CCardBox cCardBox[NConsts::BOX_AMOUNT];//����� ��� ����

  CRectangle cRectangle_NumberFrame;//����� ������ ��������
  CRectangle cRectangle_LoadState;//������ "��������� ���������"
  CRectangle cRectangle_SaveState;//������ "��������� ���������"
  CRectangle cRectangle_Start;//������ "����� ������"
  CRectangle cRectangle_PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//����� ������ ��������
  
  CCursor cCursor;

  //-������� ������-------------------------------------------------------------------------------------
  //-������---------------------------------------------------------------------------------------------
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CWnd_Main(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CWnd_Main();
  //-������� ��������� ��������� ������-----------------------------------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//�������� ����
  afx_msg void OnDestroy(void);//����������� ����
  afx_msg void OnPaint(void);//����������� ����
  afx_msg void OnTimer(UINT nIDEvent);//������� �������
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);//������� ����
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);//������ ����� ������ �����
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);//�������� ����� ������ �����
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//������ ������ ������ �����
  afx_msg void OnRButtonUp(UINT nFlags,CPoint point);//�������� ������ ������ �����
  afx_msg void OnMouseMove(UINT nFlags,CPoint point);//����� ������������
  //-�������� ������� ������----------------------------------------------------------------------------
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
 private: 
  void InitBox(int32_t &right_border,int32_t &bottom_border);//������������� ������
  void InitButton(int32_t &right_border,int32_t &bottom_border);//������������� ������
  void InitScreen(int32_t &right_border,int32_t &bottom_border);//������������� ������

  void Draw(void);//���������
  void MouseControl(const CCoord &cCoord,bool left,bool right,bool center);//���������� �� �����
  void AutoCollection(void);//����������
  void CaptureCard(void);//������ ����
  void ReleaseCard(void);//������������ ����

  int32_t RND(int32_t max);//��������� �����
  void DrawCard(const CCoord &cCoord,int32_t value,CARD_SUIT suit);//��������� �����
  bool MoveCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination);//����������� ����� �� ������ � ������
  void RotatePool(void);//����������� ���� ������ ������
  void InitGame(void);//���������������� �������
  void OnVisibleCard(void);//������� ������ ����� ���� ����� ��������
  void DrawMap(void);//���������� ���� � �������
  bool GetSelectBoxParam(const CCoord &cCoord,int32_t &box,int32_t &index,bool &empty);//����������� ��� �� ���� � ����� ������ � ������ ������� ������
  void ChangeBox(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//����������� ��������� ���� �� ����� ������ � ������
  void DrawDesktop(void);//��������� �����
  void DrawMenu(void);//��������� ����
  void DrawMoveCard(int32_t s_box,int32_t s_index,int32_t d_box);//���������� ����������� ����
  void ChangeCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//����������� ����� � ������ ������
  bool CheckFinish(void);//��������� �� ����������� �������
  bool CheckMenu(void);//������ � ����
  void DrawFinish(void);//������������ � �������
  bool LoadState(void);//��������� ���������
  bool SaveState(void);//��������� ���������
  //-������------------------------------------------------------------------
};
#endif
