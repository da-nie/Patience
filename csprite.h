#ifndef C_SPRITE_H
#define C_SPRITE_H

//====================================================================================================
//����� �������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <stdint.h>
#include <windows.h>

#include "ivideo.h"
#include "isprite.h"

//====================================================================================================
//����� �������
//====================================================================================================
class CSprite:public ISprite
{
 private:
  //-����������-----------------------------------------------------------------------------------------
  int32_t Width;//������
  int32_t Height;//������

  uint32_t *Data;//��������� �� ������ �������    
 public:
  //-����������� ������---------------------------------------------------------------------------------
  CSprite(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CSprite();
 public: 
  //-�������� ������� ������----------------------------------------------------------------------------
  bool Load(const char *file_name);//��������� ������ �� �����
  bool Load(HMODULE hModule,int32_t id);//��������� ������ �� ��������
  void Put(IVideo *iVideo_Ptr,int32_t x,int32_t y,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//���������� ������
  void PutSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,bool alpha,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//���������� ����� �������
  void PutSpriteItemMask(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,uint8_t *Mask,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//���������� ����� ������� � ������
  void PutBlendSpriteItem(IVideo *iVideo_Ptr,int32_t x,int32_t y,int32_t offsetx,int32_t offsety,int32_t width,int32_t height,color_func_ptr_t color_func_ptr,COLOR_MODE color_mode) const;//���������� ����� ������� �� ����������� ������
  void SetAlpha(uint8_t alpha,uint8_t r,uint8_t g,uint8_t b);//���������� �������� ������������
  void Release(void);//������� ������
  int32_t GetWidth(void) const;//�������� ������ �������
  int32_t GetHeight(void) const;//�������� ������ �������
  void Normalize(void);//����������� ����������� �������
  bool Save(const char *file_name) const;//��������� ����������� �������
 private:
  //-�������� ������� ������----------------------------------------------------------------------------
 public:
  //-����������� ������� ������-------------------------------------------------------------------------
  static uint32_t ConvertColor(COLOR_MODE color_mode,uint32_t color);//������������� �����
  static uint32_t ForwardColor(COLOR_MODE color_mode,uint32_t color);//������ ����������� �����
};
#endif