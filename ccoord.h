#ifndef C_COORD_H
#define C_COORD_H

//====================================================================================================
//����� ���������� �����
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include <stdint.h>

//====================================================================================================
//����� ���������� �����
//====================================================================================================
class CCoord
{
 public:
  //-����������----------------------------------------------------------------------------------------- 
  int32_t X;
  int32_t Y;
 public:
  //-����������� ������--------------------------------------------------------------------------------- 
  CCoord(void);
  CCoord(int32_t x,int32_t y);
  //-���������� ������----------------------------------------------------------------------------------
  ~CCoord();
  //-�������� ������� ������----------------------------------------------------------------------------
 public:  
  //-�������� ������� ������----------------------------------------------------------------------------
 protected:
};
#endif