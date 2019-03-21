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
//главный класс программы
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-Перечисления---------------------------------------------------------------------------------------
  //режим работы таймера
  enum TIMER_MODE
  {
   //таймер не используется
   TIMER_MODE_NONE,
   //таймер используется для перемещения карты
   TIMER_MODE_MOVE_CARD,
   //таймер используется для вывода поздравления
   TIMER_MODE_CONGRATULATION
  };
  //-Структуры------------------------------------------------------------------------------------------
  //параметры обработки таймера
  struct STimerModeState
  {
   //для режима перемещения карт
   int32_t SourceBoxIndex;
   int32_t SourceCardIndexInBox;
   int32_t DestinationBoxIndex;
   int32_t BeginX;
   int32_t BeginY;
   int32_t EndX;
   int32_t EndY;
   //для режима поздравления
   int32_t Counter;
  };
  //-Переменные класса----------------------------------------------------------------------------------
  RECT Rect_Window;//размер окна
     
  CUniqueArrayPtr<UINT> ScreenBuffer;//указатель на видеобуфер изображения
  std::unique_ptr<IVideo> iVideo_Ptr;//указатель на класс работы с видеобуфером
  std::unique_ptr<ISpriteStorage> iSpriteStorage_Ptr;//указатель на класс хранилища спрайтов

  TIMER_MODE TimerMode;//режим использования таймера
  STimerModeState sTimerModeState;//параметры обработки таймера

  CCardBox cCardBox[NConsts::BOX_AMOUNT];//ящики для карт
  int32_t Magazine_FirstBoxIndex;//индекс первого ящика магазина
  int32_t Magazine_SecondBoxIndex;//индекс второго ящика магазина

  CRectangle cRectangle_NumberFrame;//рамка номера пасьянса
  CRectangle cRectangle_LoadState;//кнопка "загрузить состояние"
  CRectangle cRectangle_SaveState;//кнопка "сохранить состояние"
  CRectangle cRectangle_Start;//кнопка "сдать колоду"
  CRectangle cRectangle_PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//цифры номера расклада  
  CCursor cCursor;//курсор

  //-Функции класса-------------------------------------------------------------------------------------
  //-Прочее---------------------------------------------------------------------------------------------
 public:
  //-Конструктор класса---------------------------------------------------------------------------------
  CWnd_Main(void);
  //-Деструктор класса----------------------------------------------------------------------------------
  ~CWnd_Main();
  //-Функции обработки сообщений класса-----------------------------------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//создание окна
  afx_msg void OnDestroy(void);//уничтожение окна
  afx_msg void OnPaint(void);//перерисовка окна
  afx_msg void OnTimer(UINT nIDEvent);//событие таймера
  afx_msg BOOL OnEraseBkgnd(CDC *pDC);//очистка окна
  afx_msg void OnLButtonDown(UINT nFlags,CPoint point);//нажата левая кнопка мышки
  afx_msg void OnLButtonUp(UINT nFlags,CPoint point);//отпущена левая кнопка мышки
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//нажата правая кнопка мышки
  afx_msg void OnRButtonUp(UINT nFlags,CPoint point);//отпущена правая кнопка мышки
  afx_msg void OnMouseMove(UINT nFlags,CPoint point);//мышка перемещается
  //-Открытые функции класса----------------------------------------------------------------------------
 public:
  //-Закрытые функции класса----------------------------------------------------------------------------
 private: 
  void InitBox(int32_t &right_border,int32_t &bottom_border);//инициализация ящиков
  void InitButton(int32_t &right_border,int32_t &bottom_border);//инициализация кнопок
  void InitScreen(int32_t &right_border,int32_t &bottom_border);//инициализация экрана

  void Draw(void);//отрисовка
  void MouseControl(const CCoord &cCoord,bool left,bool right,bool center);//управление от мышки
  void AutoCollection(void);//автосборка
  void CaptureCard(void);//захват карт
  void ReleaseCard(void);//освобождение карт

  int32_t RND(int32_t max);//случайное число
  void RotatePool(void);//перемещение карт внутри колоды
  void InitGame(void);//инициализировать расклад
  void OnVisibleCard(void);//сделать нижние карты всех рядов видимыми
  void DrawMap(void);//нарисовать поле с картами
  bool GetSelectBoxParam(const CCoord &cCoord,int32_t &box,int32_t &index,bool &empty);//определение что за ящик и номер ячейки в данной позиции экрана
  void DrawDesktop(void);//рисование сукна
  void DrawMenu(void);//рисование меню
  void DrawMoveCard(int32_t source_box_index,int32_t source_card_index_in_box,int32_t destination_box_index);//нарисовать перемещение карт
  void MoveCardWithRules(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//переместить карты с учётом правил
  bool CheckFinish(void);//проверить на собранность пасьянс
  bool CheckMenu(void);//работа с меню
  void DrawFinish(void);//поздравление с победой
  bool LoadState(void);//загрузить состояние
  bool SaveState(void);//сохранить состояние
  //-Прочее------------------------------------------------------------------
};
#endif
