//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "ccard.h"
#include "consts.h"
#include "csprite.h"

//====================================================================================================
//конструктор и деструктор класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CCard::CCard(void)
{
 Suit=CARD_SUIT_SPADES;
 Value=2;
 Visible=false;
}

//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CCard::~CCard()
{ 
}

//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//сохранить состояние
//----------------------------------------------------------------------------------------------------
bool CCard::SaveState(std::ofstream &file)
{
 if (file.write(reinterpret_cast<char*>(&Suit),sizeof(CARD_SUIT)).fail()==true) return(false);
 if (file.write(reinterpret_cast<char*>(&Value),sizeof(uint32_t)).fail()==true) return(false);
 if (file.write(reinterpret_cast<char*>(&Visible),sizeof(bool)).fail()==true) return(false);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//загрузить состояние
//----------------------------------------------------------------------------------------------------
bool CCard::LoadState(std::ifstream &file)
{
 if (file.read(reinterpret_cast<char*>(&Suit),sizeof(CARD_SUIT)).fail()==true) return(false);
 if (file.read(reinterpret_cast<char*>(&Value),sizeof(uint32_t)).fail()==true) return(false);
 if (file.read(reinterpret_cast<char*>(&Visible),sizeof(bool)).fail()==true) return(false);
 return(true);
}   
//----------------------------------------------------------------------------------------------------
//отрисовать карту
//----------------------------------------------------------------------------------------------------
void CCard::Draw(const CCoord &cCoord,IVideo *iVideo_Ptr,ISpriteStorage *iSpriteStorage_Ptr)
{
 if (Visible==false)
 {
  iSpriteStorage_Ptr->GetSpriteBackPtr()->Put(iVideo_Ptr,cCoord.X,cCoord.Y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);
  return;
 }

 if (Value<0 || Value>=NConsts::CARD_VALUE_AMOUNT) return;//таких значений нет
 int32_t suit_index=-1;
 if (Suit==CCard::CARD_SUIT_SPADES) suit_index=0;
 if (Suit==CCard::CARD_SUIT_HEARTS) suit_index=1;
 if (Suit==CCard::CARD_SUIT_CLUBS) suit_index=2;
 if (Suit==CCard::CARD_SUIT_DIAMONDS) suit_index=3;
 if (suit_index<0) return;//таких мастей нет
 iSpriteStorage_Ptr->GetSpriteCardPtr(Value,suit_index)->Put(iVideo_Ptr,cCoord.X,cCoord.Y,false,CSprite::ForwardColor,ISprite::COLOR_MODE_RGB);

 uint32_t card_width=iSpriteStorage_Ptr->GetSpriteCardPtr(Value,suit_index)->GetWidth();

 uint32_t width;
 uint32_t height;
 iVideo_Ptr->GetScreenSize(width,height);

 uint32_t *vptr;
 uint32_t linesize;
 iVideo_Ptr->GetVideoPointer(vptr,linesize);

 static const uint32_t color=(0xffUL<<24)|(0x00<<16)|(0x00<<8)|(0x00<<0); //цвет линии

 //рисуем горизонтальную линию
 for(int32_t n=0;n<=card_width;n++)
 {
  uint32_t *v_ptr;
  int32_t x1=cCoord.X+n;
  int32_t y1=cCoord.Y-1;
  if (x1>=0 && x1<width && y1>=0 && y1<height)
  {
   v_ptr=vptr+(x1+y1*linesize);
   *v_ptr=color;
  }
 }
}