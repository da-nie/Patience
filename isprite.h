#ifndef I_SPRITE_H
#define I_SPRITE_H

//====================================================================================================
//������������ ����� �������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <stdint.h>
#include <windows.h>

#include "ivideo.h"

//====================================================================================================
//������������ ����� �������
//====================================================================================================
class ISprite
{
 public:
  //-������������---------------------------------------------------------------------------------------
  enum COLOR_MODE
  {
   COLOR_MODE_RGB=0,
   COLOR_MODE_GBR,
   COLOR_MODE_GRB,
   COLOR_MODE_RBG,
   COLOR_MODE_BRG,
   COLOR_MODE_BGR,
   COLOR_MODE_AMOUNT//���������� �������
  };
 public:
  //-����-----------------------------------------------------------------------------------------------
  typedef uint32_t (*color_func_ptr_t)(COLOR_MODE,uint32_t);//��� ��������� �� ������� �������������� ������
 public:
  //-���������� ������----------------------------------------------------------------------------------
  virtual ~ISprite() {};
 public: 
  //-�������� ������� ������----------------------------------------------------------------------------
  virtual bool Load(const char *file_name)=0;//��������� ������ �� �����
  virtual bool Load(HMODULE hModule,int32_t id)=0;//��������� ������ �� ��������
  virtual void Put(IVideo *iVideo_Ptr,int32_t x,int32_t y,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//���������� ������
  virtual void PutSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//���������� ����� �������
  virtual void PutSpriteItemMask(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,uint8_t *Mask,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//���������� ����� ������� � ������
  virtual void PutBlendSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const =0;//���������� ����� ������� �� ����������� ������
  virtual void SetAlpha(uint8_t alpha,uint8_t r,uint8_t g,uint8_t b)=0;//���������� �������� ������������
  virtual void Release(void)=0;//������� ������
  virtual int32_t GetWidth(void) const =0;//�������� ������ �������
  virtual int32_t GetHeight(void) const =0;//�������� ������ �������
  virtual void Normalize(void)=0;//����������� ����������� �������
  virtual bool Save(const char *file_name) const =0;//��������� ����������� �������
  //-�������� ������� ������----------------------------------------------------------------------------
};
#endif