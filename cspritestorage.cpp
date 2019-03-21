//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "cspritestorage.h"
#include "csprite.h"

//====================================================================================================
//конструктор и деструктор класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CSpriteStorage::CSpriteStorage()
{
 iSprite_Dummy_Ptr.reset(new CSprite());

 static const int32_t MAX_STRING_SIZE=255;//максимальный размер строки

 char filename[MAX_STRING_SIZE];
 //загружаем спрайты 
 for(uint32_t n=0;n<NConsts::CARD_SUIT_AMOUNT;n++)
 {
  for(uint32_t m=0;m<NConsts::CARD_VALUE_AMOUNT;m++) iSprite_Card_Ptr[m][n].reset(new CSprite());
  //туз
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_A_%i.tga",n);
  iSprite_Card_Ptr[0][n]->Load(filename);
  //2
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_02_%i.tga",n);
  iSprite_Card_Ptr[1][n]->Load(filename);
  //3
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_03_%i.tga",n);
  iSprite_Card_Ptr[2][n]->Load(filename);
  //4
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_04_%i.tga",n);
  iSprite_Card_Ptr[3][n]->Load(filename);
  //5
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_05_%i.tga",n);
  iSprite_Card_Ptr[4][n]->Load(filename);
  //6
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_06_%i.tga",n);
  iSprite_Card_Ptr[5][n]->Load(filename);
  //7
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_07_%i.tga",n);
  iSprite_Card_Ptr[6][n]->Load(filename);
  //8
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_08_%i.tga",n);
  iSprite_Card_Ptr[7][n]->Load(filename);
  //9
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_09_%i.tga",n);
  iSprite_Card_Ptr[8][n]->Load(filename);
  //10
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_10_%i.tga",n);
  iSprite_Card_Ptr[9][n]->Load(filename);
  //валет
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_J_%i.tga",n);
  iSprite_Card_Ptr[10][n]->Load(filename);
  //дама
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_Q_%i.tga",n);
  iSprite_Card_Ptr[11][n]->Load(filename);
  //король
  sprintf_s(filename,MAX_STRING_SIZE,"Sprites\\card_K_%i.tga",n);
  iSprite_Card_Ptr[12][n]->Load(filename);
 }

 for(uint32_t m=0;m<NConsts::NUMBER_AMOUNT;m++) iSprite_Number_Ptr[m].reset(new CSprite());

 iSprite_Number_Ptr[0]->Load("Sprites\\n0.tga");
 iSprite_Number_Ptr[1]->Load("Sprites\\n1.tga");
 iSprite_Number_Ptr[2]->Load("Sprites\\n2.tga");
 iSprite_Number_Ptr[3]->Load("Sprites\\n3.tga");
 iSprite_Number_Ptr[4]->Load("Sprites\\n4.tga");
 iSprite_Number_Ptr[5]->Load("Sprites\\n5.tga");
 iSprite_Number_Ptr[6]->Load("Sprites\\n6.tga");
 iSprite_Number_Ptr[7]->Load("Sprites\\n7.tga");
 iSprite_Number_Ptr[8]->Load("Sprites\\n8.tga");
 iSprite_Number_Ptr[9]->Load("Sprites\\n9.tga");

 iSprite_NumberFrame_Ptr.reset(new CSprite());
 iSprite_Start_Ptr.reset(new CSprite());
 iSprite_SaveState_Ptr.reset(new CSprite());
 iSprite_LoadState_Ptr.reset(new CSprite());
 iSprite_Desktop_Ptr.reset(new CSprite());
 iSprite_Back_Ptr.reset(new CSprite());
 iSprite_Win_Ptr.reset(new CSprite());

 iSprite_NumberFrame_Ptr->Load("Sprites\\number.tga");
 iSprite_Start_Ptr->Load("Sprites\\start.tga");
 iSprite_SaveState_Ptr->Load("Sprites\\save_state.tga");
 iSprite_LoadState_Ptr->Load("Sprites\\load_state.tga");
 iSprite_Desktop_Ptr->Load("Sprites\\desktop.tga");
 iSprite_Back_Ptr->Load("Sprites\\back.tga");
 iSprite_Win_Ptr->Load("Sprites\\win.tga");
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CSpriteStorage::~CSpriteStorage()
{  
}
//====================================================================================================
//закрытые функции класса
//====================================================================================================

//====================================================================================================
//открытые функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт карты
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteCardPtr(uint32_t card,uint32_t color) const
{
 if (card>=NConsts::CARD_VALUE_AMOUNT) return(iSprite_Dummy_Ptr.get());
 if (color>=NConsts::CARD_SUIT_AMOUNT) return(iSprite_Dummy_Ptr.get());
 return(iSprite_Card_Ptr[card][color].get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт указателя
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpritePointerPtr(void) const
{
 return(iSprite_Pointer_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт фона
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteDesktopPtr(void) const
{
 return(iSprite_Desktop_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт рубашки карты
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteBackPtr(void) const
{
 return(iSprite_Back_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт цифры
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteNumberPtr(uint32_t number) const
{
 if (number>=NConsts::NUMBER_AMOUNT) return(iSprite_Dummy_Ptr.get());
 return(iSprite_Number_Ptr[number].get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт рамки цифр
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteNumberFramePtr(void) const
{
 return(iSprite_NumberFrame_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт "сохранить состояние"
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteSaveStatePtr(void) const
{
 return(iSprite_SaveState_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт "загрузить состояние"
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteLoadStatePtr(void) const
{
 return(iSprite_LoadState_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт "сдать колоду"
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteStartPtr(void) const
{
 return(iSprite_Start_Ptr.get());
}
//----------------------------------------------------------------------------------------------------
//получить указатель на спрайт "вы победили"
//----------------------------------------------------------------------------------------------------
ISprite* CSpriteStorage::GetSpriteWinPtr(void) const
{
 return(iSprite_Win_Ptr.get());
}
