#ifndef C_SPRITE_H
#define C_SPRITE_H

//====================================================================================================
//класс спрайта
//====================================================================================================

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include <stdint.h>
#include <windows.h>

#include "ivideo.h"
#include "isprite.h"

//====================================================================================================
//класс спрайта
//====================================================================================================
class CSprite:public ISprite
{
 private:
  //-ѕеременные-----------------------------------------------------------------------------------------
  int32_t Width;//ширина
  int32_t Height;//высота

  uint32_t *Data;//указатель на данные спрайта    
 public:
  //- онструктор класса---------------------------------------------------------------------------------
  CSprite(void);
  //-ƒеструктор класса----------------------------------------------------------------------------------
  ~CSprite();
 public: 
  //-ќткрытые функции класса----------------------------------------------------------------------------
  bool Load(const char *file_name);//загрузить спрайт из файла
  bool Load(HMODULE hModule,int32_t id);//загрузить спрайт из ресурсов
  void Put(IVideo *iVideo_Ptr,int32_t x,int32_t y,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//отобразить спрайт
  void PutSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//отобразить часть спрайта
  void PutSpriteItemMask(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,uint8_t *Mask,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//отобразить часть спрайта с маской
  void PutBlendSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//отобразить часть спрайта со смешиванием цветов
  void SetAlpha(uint8_t alpha,uint8_t r,uint8_t g,uint8_t b);//установить значение програчности
  void Release(void);//удалить спрайт
  int32_t GetWidth(void) const;//получить ширину спрайта
  int32_t GetHeight(void) const;//получить высоту спрайта
  void Normalize(void);//нормировать изображение спрайта
  bool Save(const char *file_name) const;//сохранить изображение спрайта
 private:
  //-«акрытые функции класса----------------------------------------------------------------------------
 public:
  //-—татические функции класса-------------------------------------------------------------------------
  static uint32_t ConvertColor(COLOR_MODE color_mode,uint32_t color);//перекодировка цвета
  static uint32_t ForwardColor(COLOR_MODE color_mode,uint32_t color);//пр€мое отображение цвета
};
#endif