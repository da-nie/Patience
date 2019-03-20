#ifndef C_SPRITE_STORAGE_H
#define C_SPRITE_STORAGE_H

//====================================================================================================
//����� ��������� ��������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <memory>
#include <stdint.h>

#include "consts.h"
#include "isprite.h"
#include "ispritestorage.h"

//====================================================================================================
//����� ��������� ��������
//====================================================================================================
class CSpriteStorage:public ISpriteStorage
{
 private:
  //-����������-----------------------------------------------------------------------------------------
  std::unique_ptr<ISprite> iSprite_Dummy_Ptr;//������ ��������

  std::unique_ptr<ISprite> iSprite_Card_Ptr[NConsts::CARD_UNIT_AMOUNT][NConsts::CARD_COLOR_AMOUNT];//������� ����
  std::unique_ptr<ISprite> iSprite_Pointer_Ptr;//������ ���������
  std::unique_ptr<ISprite> iSprite_Desktop_Ptr;//������ ����
  std::unique_ptr<ISprite> iSprite_Back_Ptr;//������ ������� ����
  std::unique_ptr<ISprite> iSprite_Number_Ptr[NConsts::NUMBER_AMOUNT];//������� ����
  std::unique_ptr<ISprite> iSprite_NumberFrame_Ptr;//������ ����� ��� ����
  std::unique_ptr<ISprite> iSprite_SaveState_Ptr;//������ "��������� ���������"
  std::unique_ptr<ISprite> iSprite_LoadState_Ptr;//������ "��������� ���������"
  std::unique_ptr<ISprite> iSprite_Start_Ptr;//������ "����� ������"
  std::unique_ptr<ISprite> iSprite_Win_Ptr;//������ "�� ��������"
 public:
  //-�����������---------------------------------------------------------------------------------------- 
  CSpriteStorage();
  //-����������-----------------------------------------------------------------------------------------
  ~CSpriteStorage();
 public:
  //-�������� ������� ������----------------------------------------------------------------------------  
  ISprite* GetSpriteCardPtr(uint32_t card,uint32_t color) const;//�������� ��������� �� ������ �����
  ISprite* GetSpritePointerPtr(void) const;//�������� ��������� �� ������ ���������
  ISprite* GetSpriteDesktopPtr(void) const;//�������� ��������� �� ������ ����
  ISprite* GetSpriteBackPtr(void) const;//�������� ��������� �� ������ ������� �����
  ISprite* GetSpriteNumberPtr(uint32_t number) const;//�������� ��������� �� ������ �����
  ISprite* GetSpriteNumberFramePtr(void) const;//�������� ��������� �� ������ ����� ����
  ISprite* GetSpriteSaveStatePtr(void) const;//�������� ��������� �� ������ "��������� ���������"
  ISprite* GetSpriteLoadStatePtr(void) const;//�������� ��������� �� ������ "��������� ���������"
  ISprite* GetSpriteStartPtr(void) const;//�������� ��������� �� ������ "����� ������"
  ISprite* GetSpriteWinPtr(void) const;//�������� ��������� �� ������ "�� ��������"
  //-�������� ������� ������----------------------------------------------------------------------------
};

#endif