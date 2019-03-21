#ifndef C_RECTANGLE_H
#define C_RECTANGLE_H

//====================================================================================================
//����� ������������� �������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "ccoord.h"
#include <stdint.h>

//====================================================================================================
//����� ������������� �������
//====================================================================================================
class CRectangle
{
 public:
  //-����������----------------------------------------------------------------------------------------- 
  CCoord cCoord_Position;//��������� �� ������
  CCoord cCoord_Size;//������
 public:
  //-����������� ������--------------------------------------------------------------------------------- 
  CRectangle(void);
  CRectangle(const CCoord &cCoord_position,const CCoord &cCoord_size);
  //-���������� ������----------------------------------------------------------------------------------
  ~CRectangle();
  //-�������� ������� ������----------------------------------------------------------------------------
 public:
  bool IsPointInside(CCoord cCoord);//��������� �� ����� ������
  void Set(const CCoord &cCoord_position,const CCoord &cCoord_size);//������ ����������
  int32_t GetLeftX(void);//�������� ����� �������� ���������� X
  int32_t GetTopY(void);//�������� ������� �������� ���������� Y
  int32_t GetRightX(void);//�������� ������ �������� ���������� X
  int32_t GetBottomY(void);//�������� ������ �������� ���������� Y
  //-�������� ������� ������----------------------------------------------------------------------------
 protected:
};
#endif