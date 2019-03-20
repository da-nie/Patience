#ifndef C_WND_MAIN
#define C_WND_MAIN

//====================================================================================================
//главный класс программы
//====================================================================================================


//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "stdafx.h"
#include <stdint.h>

#include "ivideo.h"
#include "isprite.h"
#include "ispritestorage.h"
#include "cuniquearrayptr.h"
#include <memory>

//====================================================================================================
//главный класс программы
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-Переменные класса----------------------------------------------------------------------------------
  //масти
  enum CARD_SUIT
  {
   //пики
   CARD_SUIT_SPADES,
   //червы
   CARD_SUIT_HEARTS,
   //трефы
   CARD_SUIT_CLUBS,
   //буби
   CARD_SUIT_DIAMONDS
  };

  //режим работы таймера
  enum TIMER_MODE
  {
   //таймер не используется
   TIMER_MODE_NONE,
   //таймер используется для перемещения карты
   TIMER_MODE_MOVE_CARD,
   //таймер используется для перемещения карт при выигрыше
   TIMER_MODE_CONGRATULATION
  };

   //экран
  CUniqueArrayPtr<UINT> ScreenBuffer;//указатель на видеобуфер изображения
  std::unique_ptr<IVideo> iVideo_Ptr;//указатель на класс работы с видеобуфером
  std::unique_ptr<ISpriteStorage> iSpriteStorage_Ptr;//указатель на класс хранилища спрайтов

  TIMER_MODE TimerMode;//режим использования таймера
  
  struct STimerModeState
  {
   //для режима перемещения карт
   int32_t s_box;
   int32_t s_index;
   int32_t d_box;
   int32_t xb;
   int32_t yb;
   int32_t xe;
   int32_t ye;
   //для режима поздравления
   int32_t Counter;
  } sTimerModeState;
 
  struct SCard
  {
   CARD_SUIT Suit;//масть
   int32_t Value;//значение карты от двойки до туза
   bool Visible;//true-карта видима
  } sCard_Box[13][53];//тринадцать ящиков по 52 карты в каждой максимум

  //координаты расположения ячеек карт
  int32_t BoxXPos[13][53];
  int32_t BoxYPos[13][53];

  //данные курсора
  struct SCursor
  {
   int32_t X;
   int32_t Y;
   bool Cross;//флаг нажатия крестика
   int32_t SelectBox;//выбранный ящик
   int32_t SelectIndex;//выбранная ячейка
   int32_t OffsetSelectX;//смещение курсора для выбранных ячеек
   int32_t OffsetSelectY;
   uint8_t Number[5];//номер пасьянса
  } sCursor;
  //-Функции класса-------------------------------------------------------------------------------------
  //-Прочее---------------------------------------------------------------------------------------------
 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CWnd_Main(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CWnd_Main();
  //-Функции обработки сообщений класса-----------------------------------------------------------------
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
  //-Открытые функции класса----------------------------------------------------------------------------
 public:
  //-Закрытые функции класса----------------------------------------------------------------------------
 private: 
  void Draw(void);//отрисовка
  void MouseControl(int32_t x,int32_t y,bool left,bool right,bool center);//управление от мышки
  int32_t RND(int32_t max);//случайное число
  void DrawCard(int32_t x,int32_t y,int32_t value,CARD_SUIT suit);//отрисовка карты
  bool MoveCard(int32_t s,int32_t d);//переместить карту из ящика s в ячейку d
  int32_t GetCardInBox(int32_t box);//получить количество карт в ящике
  void RotatePool(void);//перемещение карт внутри колоды
  void InitGame(void);//инициализировать расклад
  void OnVisibleCard(void);//сделать нижние карты всех рядов видимыми
  void DrawMap(void);//нарисовать поле с картами
  bool GetSelectBoxParam(int32_t x,int32_t y,int32_t *box,int32_t *index);//определение что за ящик и номер ячейки в данной позиции экрана
  void ChangeBox(int32_t s_box,int32_t s_index,int32_t d_box);//переместить карты из одного ящика в другой
  void DrawDesktop(void);//рисование сукна
  void DrawMenu(void);//рисование меню
  void DrawMoveCard(int32_t s_box,int32_t s_index,int32_t d_box);//нарисовать перемещение карт
  void ChangeCard(int32_t s_box,int32_t s_index,int32_t d_box,int32_t d_index);//переместить карты с учётом правил
  bool CheckFinish(void);//проверить на собранность пасьянс
  bool CheckMenu(void);//работа с меню
  void DrawFinish(void);//поздравление с победой
  //-Прочее------------------------------------------------------------------
};
#endif
