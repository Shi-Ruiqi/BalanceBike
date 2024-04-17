#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "stdint.h"

/* ע�⣺������������������ת�����ǣ����Բ���ǰ����ϸ�ۿ� "���΢����Ƶע�����������¼ǰ�ؿ���"�������Լ����������𻵶�� */
#define dirADJUST 0   /* ����΢��ֵ���޸Ĵ˲�����΢���������ÿ�ν����ڡ�1�����ȣ�����Խ��Խ����ת������ԽС��Խ����ת*/
#define dirBASE 10    /* �����ֵ���޸Ĵ˲����ɵ������� */

/* �Ե���ᳯ���Լ�����Ϊ��׼(���ֳ����Լ�Ϊ׼) */
#define dirClockwise()      HAL_GPIO_WritePin(MAIN_DIR_GPIO_Port, MAIN_DIR_Pin, GPIO_PIN_SET)   //˳ʱ�뷽��
#define dirAnticlockwise()  HAL_GPIO_WritePin(MAIN_DIR_GPIO_Port, MAIN_DIR_Pin, GPIO_PIN_RESET) //��ʱ�뷽��
#define mainMotorEN()       HAL_GPIO_WritePin(MAIN_EN_GPIO_Port, MAIN_EN_Pin, GPIO_PIN_SET)
#define mainMotorStop()     HAL_GPIO_WritePin(MAIN_EN_GPIO_Port, MAIN_EN_Pin, GPIO_PIN_RESET)

/* 
����ת��ĳ߶� 
4:ͬ������ͬ������Ϊת�����
1:������Ϊת�����
*/
#define DIR_SIZE 3    

typedef struct 
{
  int out;
}_OUT_Motor;


void _controller_perform(void) ;
void _controller_output(void)   ;
void CargoForward(void);
void CarBackward(void);
void EnableAuxMotor(void);
void DisableAuxMotor(void);
void TurnLeftOrRight(float inXrocker);
void goForwardOrBackward(float inYrcoker);
extern _OUT_Motor Motor1;

#endif
