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

#include "ivideo.h"
#include "isprite.h"
#include "ispritestorage.h"
#include "cuniquearrayptr.h"
#include <memory>

//====================================================================================================
//������� ����� ���������
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-���������� ������----------------------------------------------------------------------------------
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
 
  struct SCard
  {
   CARD_SUIT Suit;//�����
   int32_t Value;//�������� ����� �� ������ �� ����
   bool Visible;//true-����� ������
  } sCard_Box[13][53];//���������� ������ �� 52 ����� � ������ ��������

  //���������� ������������ ����� ����
  int32_t BoxXPos[13][53];
  int32_t BoxYPos[13][53];

  //������ �������
  struct SCursor
  {
   int32_t X;
   int32_t Y;
   bool Cross;//���� ������� ��������
   int32_t SelectBox;//��������� ����
   int32_t SelectIndex;//��������� ������
   int32_t OffsetSelectX;//�������� ������� ��� ��������� �����
   int32_t OffsetSelectY;
   uint8_t Number[5];//����� ��������
  } sCursor;
  //-������� ������-------------------------------------------------------------------------------------
  //-������---------------------------------------------------------------------------------------------
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CWnd_Main(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CWnd_Main();
  //-������� ��������� ��������� ������-----------------------------------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy(void);
  afx_msg void OnPaint(void);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags,CPoint point);
  afx_msg void OnMouseMove(UINT nFlags,CPoint point);
  //-�������� ������� ������----------------------------------------------------------------------------
 public:
  //-�������� ������� ������----------------------------------------------------------------------------
 private: 
  void Draw(void);//���������
  void MouseControl(int32_t x,int32_t y,bool left,bool right,bool center);//���������� �� �����
  int32_t RND(int32_t max);//��������� �����
  void DrawCard(int32_t x,int32_t y,int32_t value,CARD_SUIT suit);//��������� �����
  bool MoveCard(int32_t s,int32_t d);//����������� ����� �� ����� s � ������ d
  int32_t GetCardInBox(int32_t box);//�������� ���������� ���� � �����
  void RotatePool(void);//����������� ���� ������ ������
  void InitGame(void);//���������������� �������
  void OnVisibleCard(void);//������� ������ ����� ���� ����� ��������
  void DrawMap(void);//���������� ���� � �������
  bool GetSelectBoxParam(int32_t x,int32_t y,int32_t *box,int32_t *index);//����������� ��� �� ���� � ����� ������ � ������ ������� ������
  void ChangeBox(int32_t s_box,int32_t s_index,int32_t d_box);//����������� ����� �� ������ ����� � ������
  void DrawDesktop(void);//��������� �����
  void DrawMenu(void);//��������� ����
  void DrawMoveCard(int32_t s_box,int32_t s_index,int32_t d_box);//���������� ����������� ����
  void ChangeCard(int32_t s_box,int32_t s_index,int32_t d_box,int32_t d_index);//����������� ����� � ������ ������
  bool CheckFinish(void);//��������� �� ����������� �������
  bool CheckMenu(void);//������ � ����
  void DrawFinish(void);//������������ � �������
  //-������------------------------------------------------------------------
};
#endif
