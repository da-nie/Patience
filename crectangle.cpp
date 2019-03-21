//====================================================================================================
//������������ ����������
//====================================================================================================
#include "crectangle.h"


//====================================================================================================
//����������� � ���������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�����������
//----------------------------------------------------------------------------------------------------
CRectangle::CRectangle(void)
{ 
}

CRectangle::CRectangle(const CCoord &cCoord_position,const CCoord &cCoord_size)
{
 Set(cCoord_position,cCoord_size);
}


//----------------------------------------------------------------------------------------------------
//����������
//----------------------------------------------------------------------------------------------------
CRectangle::~CRectangle()
{ 
}

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//====================================================================================================
//�������� ������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������� �� ����� ������
//----------------------------------------------------------------------------------------------------
bool CRectangle::IsPointInside(CCoord cCoord)
{
 if (cCoord.X<cCoord_Position.X) return(false);
 if (cCoord.X>=cCoord_Position.X+cCoord_Size.X) return(false);
 if (cCoord.Y<cCoord_Position.Y) return(false);
 if (cCoord.Y>=cCoord_Position.Y+cCoord_Size.Y) return(false);
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������ ����������
//----------------------------------------------------------------------------------------------------
void CRectangle::Set(const CCoord &cCoord_position,const CCoord &cCoord_size)
{
 cCoord_Position=cCoord_position;
 cCoord_Size=cCoord_size;
}
//----------------------------------------------------------------------------------------------------
//�������� ����� �������� ���������� X
//----------------------------------------------------------------------------------------------------
int32_t CRectangle::GetLeftX(void)
{
 return(cCoord_Position.X);
}
//----------------------------------------------------------------------------------------------------
//�������� ������� �������� ���������� Y
//----------------------------------------------------------------------------------------------------
int32_t CRectangle::GetTopY(void)
{
 return(cCoord_Position.Y);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ �������� ���������� X
//----------------------------------------------------------------------------------------------------
int32_t CRectangle::GetRightX(void)
{
 return(cCoord_Position.X+cCoord_Size.X);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ �������� ���������� Y
//----------------------------------------------------------------------------------------------------
int32_t CRectangle::GetBottomY(void)
{
 return(cCoord_Position.Y+cCoord_Size.Y);
}