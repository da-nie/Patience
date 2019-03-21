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

#include <memory>
#include <deque>
#include <algorithm>
#include <fstream>


//координата
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


//прямоугольная область
class CRectangle
{   
 public:
  CCoord cCoord_Position;//положение на экране
  CCoord cCoord_Size;//размер
 CRectangle(void)
 {
 }
 CRectangle(const CCoord &cCoord_position,const CCoord &cCoord_size)
 {
  Set(cCoord_position,cCoord_size);
 }

  bool IsPointInside(CCoord cCoord)//находится ли точка внутри
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


 //карта
 class CCard
 {
  public:
   CARD_SUIT Suit;//масть
   int32_t Value;//значение карты от двойки до туза
   bool Visible;//true-карта видима
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


//ящик для карт
class CCardBox
{
 public:
  uint32_t Index;//индекс ящика
  CRectangle cRectangle;//положения ящика на экране
  CCoord cCoord_Offset;//смещение для каждой следующей карты
  std::deque<CCard> deque_CCard;//карты в ячейке
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

//курсор
class CCursor
{
 public:
  CCoord cCoord_Position;//позиция курсора
  bool MoveMode;//флаг режима перемещения
  int32_t SelectedBoxIndex;//выбранный ящик
  int32_t SelectedPositionIndexInBox;//выбранная ячейка ящика
  CCoord cCoord_OffsetWithSelectedPosition;//смещение курсора относительно выбранной ячейки ящика
  uint8_t PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//номер пасьянса
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
//главный класс программы
//====================================================================================================
class CWnd_Main:public CWnd
{
 private:
  //-Переменные класса----------------------------------------------------------------------------------
  RECT Rect_Window;//размер окна
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


  CCardBox cCardBox[NConsts::BOX_AMOUNT];//ящики для карт

  CRectangle cRectangle_NumberFrame;//рамка номера пасьянса
  CRectangle cRectangle_LoadState;//кнопка "загрузить состояние"
  CRectangle cRectangle_SaveState;//кнопка "сохранить состояние"
  CRectangle cRectangle_Start;//кнопка "сдать колоду"
  CRectangle cRectangle_PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//цифры номера расклада
  
  CCursor cCursor;

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
  void DrawCard(const CCoord &cCoord,int32_t value,CARD_SUIT suit);//отрисовка карты
  bool MoveCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination);//переместить карту из ячейки в ячейку
  void RotatePool(void);//перемещение карт внутри колоды
  void InitGame(void);//инициализировать расклад
  void OnVisibleCard(void);//сделать нижние карты всех рядов видимыми
  void DrawMap(void);//нарисовать поле с картами
  bool GetSelectBoxParam(const CCoord &cCoord,int32_t &box,int32_t &index,bool &empty);//определение что за ящик и номер ячейки в данной позиции экрана
  void ChangeBox(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//переместить несколько карт из одной ячейки в другую
  void DrawDesktop(void);//рисование сукна
  void DrawMenu(void);//рисование меню
  void DrawMoveCard(int32_t s_box,int32_t s_index,int32_t d_box);//нарисовать перемещение карт
  void ChangeCard(CCardBox &cCardBox_Source,CCardBox &cCardBox_Destination,size_t card_amount);//переместить карты с учётом правил
  bool CheckFinish(void);//проверить на собранность пасьянс
  bool CheckMenu(void);//работа с меню
  void DrawFinish(void);//поздравление с победой
  bool LoadState(void);//загрузить состояние
  bool SaveState(void);//сохранить состояние
  //-Прочее------------------------------------------------------------------
};
#endif
