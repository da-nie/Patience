#ifndef I_SPRITE_H
#define I_SPRITE_H

//====================================================================================================
//интерфейсный класс спрайта
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include <stdint.h>
#include <windows.h>

#include "ivideo.h"

//====================================================================================================
//интерфейсный класс спрайта
//====================================================================================================
class ISprite
{
 public:
  //-Перечисления---------------------------------------------------------------------------------------
  enum COLOR_MODE
  {
   COLOR_MODE_RGB=0,
   COLOR_MODE_GBR,
   COLOR_MODE_GRB,
   COLOR_MODE_RBG,
   COLOR_MODE_BRG,
   COLOR_MODE_BGR,
   COLOR_MODE_AMOUNT//количество режимов
  };
 public:
  //-Типы-----------------------------------------------------------------------------------------------
  typedef uint32_t (*color_func_ptr_t)(COLOR_MODE,uint32_t);//тип указателя на функцию преобразования цветов
 public:
  //-Деструктор класса----------------------------------------------------------------------------------
  virtual ~ISprite() {};
 public: 
  //-Открытые функции класса----------------------------------------------------------------------------
  virtual bool Load(const char *file_name)=0;//загрузить спрайт из файла
  virtual bool Load(HMODULE hModule,int32_t id)=0;//загрузить спрайт из ресурсов
  virtual void Put(IVideo *iVideo_Ptr,int32_t x,int32_t y,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//отобразить спрайт
  virtual void PutSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//отобразить часть спрайта
  virtual void PutSpriteItemMask(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,uint8_t *Mask,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//отобразить часть спрайта с маской
  virtual void PutBlendSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//отобразить часть спрайта со смешиванием цветов
  virtual void SetAlpha(uint8_t alpha,uint8_t r,uint8_t g,uint8_t b)=0;//установить значение програчности
  virtual void Release(void)=0;//удалить спрайт
  virtual int32_t GetWidth(void) const =0;//получить ширину спрайта
  virtual int32_t GetHeight(void) const =0;//получить высоту спрайта
  virtual void Normalize(void)=0;//нормировать изображение спрайта
  virtual bool Save(const char *file_name) const =0;//сохранить изображение спрайта
  //-Закрытые функции класса----------------------------------------------------------------------------
};
#endif