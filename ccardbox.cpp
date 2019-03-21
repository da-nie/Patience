//====================================================================================================
//������������ ����������
//====================================================================================================
#include "ccardbox.h"

//====================================================================================================
//����������� � ���������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CCardBox::CCardBox(void)
{ 
}

//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CCardBox::~CCardBox()
{
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������,���� �� ����
//----------------------------------------------------------------------------------------------------
bool CCardBox::IsEmpty(void)
{
 return(deque_CCard.empty());
}
//----------------------------------------------------------------------------------------------------
//�������� ���������� ���� � �����
//----------------------------------------------------------------------------------------------------
size_t CCardBox::GetCardAmount(void)
{
 return(deque_CCard.size());
}

//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CCardBox::SaveState(std::ofstream &file)
{
 size_t size=deque_CCard.size();
 if (file.write(reinterpret_cast<char*>(&size),sizeof(size_t)).fail()==true) return(false);
 for(size_t n=0;n<size;n++)
 {
  if (deque_CCard[n].SaveState(file)==false) return(false);
 };
 return(true);
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
bool CCardBox::LoadState(std::ifstream &file)
{
 size_t size;
 if (file.read(reinterpret_cast<char*>(&size),sizeof(size_t)).fail()==true) return(false);
 deque_CCard.clear();
 for(size_t n=0;n<size;n++)
 {
  CCard cCard;
  if (cCard.LoadState(file)==false) return(false);
  deque_CCard.push_back(cCard);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ��������� �� ������ ����� � ����� � ������� card_index
//----------------------------------------------------------------------------------------------------
CRectangle CCardBox::GetCardRectangle(int32_t card_index)
{
 int32_t x=cRectangle_Position.cCoord_Position.X+cCoord_CardOffset.X*card_index;
 int32_t y=cRectangle_Position.cCoord_Position.Y+cCoord_CardOffset.Y*card_index;
 return(CRectangle(CCoord(x,y),cRectangle_Position.cCoord_Size));
}

//----------------------------------------------------------------------------------------------------
//����������� ���� ����� �� ����� � ������ ����
//----------------------------------------------------------------------------------------------------
bool CCardBox::MoveOneCard(CCardBox &cCardBox_Source)
{
 if (cCardBox_Source.IsEmpty()==true) return(false);//�������� ���� ����
 deque_CCard.push_back(cCardBox_Source.deque_CCard.back());
 cCardBox_Source.deque_CCard.pop_back();
 return(true);
}
//----------------------------------------------------------------------------------------------------
//����������� ����� � ������ ����
//----------------------------------------------------------------------------------------------------

#include <windows.h>

void CCardBox::MoveCard(CCardBox &cCardBox_Source,size_t card_amount)
{
 size_t size=cCardBox_Source.GetCardAmount();
 if (size<card_amount)
 {
  MessageBox(NULL,"���� ����!","������",MB_OK);//������ ��� ������� - ������ � ������!
  return;
 } 
 size_t position=size-card_amount;
 while(position<size)
 {  
  deque_CCard.push_back(cCardBox_Source.deque_CCard[position]);
  position++;
 }
 //������� ����� �� ��������� �����
 while(card_amount>0)
 {
  if (cCardBox_Source.IsEmpty()==true) break;//�������� ���� ����
  cCardBox_Source.deque_CCard.pop_back();
  card_amount--;
 }
}