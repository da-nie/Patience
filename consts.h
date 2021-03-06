#ifndef CONSTS_H
#define CONSTS_H

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//������������ ����������
//====================================================================================================

//====================================================================================================
//���������
//====================================================================================================
namespace NConsts
{
 static const int32_t BOX_AMOUNT=13;//���������� ������
 static const int32_t WIN_COUNTER_MAX_VALUE=5*50;//����� ������ ������������

 static const int32_t NUMBER_AMOUNT=10;//���������� ����
 static const int32_t CARD_SUIT_AMOUNT=4;//���������� ������
 static const int32_t CARD_VALUE_AMOUNT=13;//���������� ����� ����

 static const int32_t BOX_WIDTH_OFFSET=10;//�������� ���� � ������� �� X
 static const int32_t BOX_HEIGHT_OFFSET=10;//�������� ���� � ������� �� Y

 static const int32_t BUTTON_HEIGHT_GAP=20;//���������� ����� �������� �� ������

 static const int32_t PATIENCE_NUMBER_AMOUNT=8;//���������� �������� � ������ ���������

 static const int32_t CARD_AMOUNT=52;//���������� ����
 static const int32_t CARD_AMOUNT_OFFSET=1;//�������� ������� ����

 static const int32_t BOX_GAP=10;//���������� ����� ������� ����

 static const int32_t CARD_STACK_AMOUNT_IN_SCREEN=15;//���������� ���� � ������ ��� ������� ������ ����

 static const char STATE_FILE_NAME[]="state.bin";//��� ����� ��������� ����
}

#endif
