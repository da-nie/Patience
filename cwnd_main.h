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

#include "ccoord.h"
#include "ccard.h"
#include "crectangle.h"
#include "ccardbox.h"
#include "ccursor.h"

#include <memory>
#include <fstream>

//====================================================================================================
//������� ����� ���������
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-������������---------------------------------------------------------------------------------------
  //����� ������ �������
  enum TIMER_MODE
  {
   //������ �� ������������
   TIMER_MODE_NONE,
   //������ ������������ ��� ����������� �����
   TIMER_MODE_MOVE_CARD,
   //������ ������������ ��� ������ ������������
   TIMER_MODE_CONGRATULATION
  };
  //-���������------------------------------------------------------------------------------------------
  //��������� ��������� �������
  struct STimerModeState
  {
   //��� ������ ����������� ����
   int32_t SourceBoxIndex;
   int32_t SourceCardIndexInBox;
   int32_t DestinationBoxIndex;
   int32_t BeginX;
   int32_t BeginY;
   int32_t EndX;
   int32_t EndY;
   //��� ������ ������������
   int32_t Counter;
  };
  //-���������� ������----------------------------------------------------------------------------------
  RECT Rect_Window;//������ ����
     
  CUniqueArrayPtr<UINT> ScreenBuffer;//��������� �� ���������� �����������
  std::unique_ptr<IVideo> iVideo_Ptr;//��������� �� ����� ������ � ������������
  std::unique_ptr<ISpriteStorage> iSpriteStorage_Ptr;//��������� �� ����� ��������� ��������

  TIMER_MODE TimerMode;//����� ������������� �������
  STimerModeState sTimerModeState;//��������� ��������� �������

  CCardBox cCardBox[NConsts::BOX_AMOUNT];//����� ��� ����
  int32_t Magazine_FirstBoxIndex;//������ ������� ����� ��������
  int32_t Magazine_SecondBoxIndex;//������ ������� ����� ��������

  CRectangle cRectangle_NumberFrame;//����� ������ ��������
  CRectangle cRectangle_LoadState;//������ "��������� ���������"
  CRectangle cRectangle_SaveState;//������ "��������� ���������"
  CRectangle cRectangle_Start;//������ "����� ������"
  CRectangle cRectangle_PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//����� ������ ��������  
  CCursor cCursor;//������

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
  void RotatePool(void);//����������� ���� ������ ������
  void InitGame(void);//���������������� �������
  void OnVisibleCard(void);//������� ������ ����� ���� ����� ��������
  void DrawMap(void);//���������� ���� � �������
  bool GetSelectBoxParam(const CCoord &cCoord,int32_t &box,int32_t &index,bool &empty);//����������� ��� �� ���� � ����� ������ � ������ ������� ������
  void DrawDesktop(void);//��������� �����
  void DrawMenu(void);//��������� ����
  void DrawMoveCard(int32_t source_box_index,int32_t source_card_index_in_box,int32_t destination_box_index);//���������� ����������� ����
  void MoveCardWithRules(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//����������� ����� � ������ ������
  bool CheckFinish(void);//��������� �� ����������� �������
  bool CheckMenu(void);//������ � ����
  void DrawFinish(void);//������������ � �������
  bool LoadState(void);//��������� ���������
  bool SaveState(void);//��������� ���������
  //-������------------------------------------------------------------------
};
#endif
