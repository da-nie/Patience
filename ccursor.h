#ifndef C_CURSOR_H
#define C_CURSOR_H

//====================================================================================================
//����� �������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "ccoord.h"
#include "consts.h"
#include <fstream>
#include <stdint.h>


//====================================================================================================
//����� �������
//====================================================================================================
class CCursor
{
 public:
  //-����������-----------------------------------------------------------------------------------------   
  CCoord cCoord_Position;//������� �������
  bool MoveMode;//���� ������ �����������
  int32_t SelectedBoxIndex;//��������� ����
  int32_t SelectedPositionIndexInBox;//��������� ������ �����
  CCoord cCoord_OffsetWithSelectedPosition;//�������� ������� ������������ ��������� ������ �����
  uint8_t PatienceNumber[NConsts::PATIENCE_NUMBER_AMOUNT];//����� ��������
 public:
  //-����������� ������--------------------------------------------------------------------------------- 
  CCursor(void);
  //-���������� ������----------------------------------------------------------------------------------
  ~CCursor();
  //-�������� ������� ������----------------------------------------------------------------------------
 public:
  void Init(void);//����������������
  void ResetSelected(void);//�������� �����
  bool SaveState(std::ofstream &file);//��������� ���������
  bool LoadState(std::ifstream &file);//��������� ���������
  //-�������� ������� ������----------------------------------------------------------------------------
 protected:
};
#endif