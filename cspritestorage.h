#ifndef C_SPRITE_STORAGE_H
#define C_SPRITE_STORAGE_H

//====================================================================================================
//класс хранилища спрайтов
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <memory>
#include <stdint.h>

#include "consts.h"
#include "isprite.h"
#include "ispritestorage.h"

//====================================================================================================
//класс хранилища спрайтов
//====================================================================================================
class CSpriteStorage:public ISpriteStorage
{
 private:
  //-Переменные-----------------------------------------------------------------------------------------
  std::unique_ptr<ISprite> iSprite_Dummy_Ptr;//спрайт заглушки

  std::unique_ptr<ISprite> iSprite_Card_Ptr[NConsts::CARD_UNIT_AMOUNT][NConsts::CARD_COLOR_AMOUNT];//спрайты карт
  std::unique_ptr<ISprite> iSprite_Pointer_Ptr;//спрайт указателя
  std::unique_ptr<ISprite> iSprite_Desktop_Ptr;//спрайт фона
  std::unique_ptr<ISprite> iSprite_Back_Ptr;//спрайт рубашки карт
  std::unique_ptr<ISprite> iSprite_Number_Ptr[NConsts::NUMBER_AMOUNT];//спрайты цифр
  std::unique_ptr<ISprite> iSprite_NumberFrame_Ptr;//спрайт рамки для цифр
  std::unique_ptr<ISprite> iSprite_SaveState_Ptr;//спрайт "сохранить состояние"
  std::unique_ptr<ISprite> iSprite_LoadState_Ptr;//спрайт "загрузить состояние"
  std::unique_ptr<ISprite> iSprite_Start_Ptr;//спрайт "сдать колоду"
  std::unique_ptr<ISprite> iSprite_Win_Ptr;//спрайт "вы победили"
 public:
  //-Конструктор---------------------------------------------------------------------------------------- 
  CSpriteStorage();
  //-Деструктор-----------------------------------------------------------------------------------------
  ~CSpriteStorage();
 public:
  //-Открытые функции класса----------------------------------------------------------------------------  
  ISprite* GetSpriteCardPtr(uint32_t card,uint32_t color) const;//получить указатель на спрайт карты
  ISprite* GetSpritePointerPtr(void) const;//получить указатель на спрайт указателя
  ISprite* GetSpriteDesktopPtr(void) const;//получить указатель на спрайт фона
  ISprite* GetSpriteBackPtr(void) const;//получить указатель на спрайт рубашки карты
  ISprite* GetSpriteNumberPtr(uint32_t number) const;//получить указатель на спрайт цифры
  ISprite* GetSpriteNumberFramePtr(void) const;//получить указатель на спрайт рамки цифр
  ISprite* GetSpriteSaveStatePtr(void) const;//получить указатель на спрайт "сохранить состояние"
  ISprite* GetSpriteLoadStatePtr(void) const;//получить указатель на спрайт "загрузить состояние"
  ISprite* GetSpriteStartPtr(void) const;//получить указатель на спрайт "сдать колоду"
  ISprite* GetSpriteWinPtr(void) const;//получить указатель на спрайт "вы победили"
  //-Закрытые функции класса----------------------------------------------------------------------------
};

#endif