#ifndef __LED_H__
#define __LED_H__

#include "stdint.h"
#include "main.h"
typedef struct
{
	uint8_t  led_opt;	//led��ѡ��
	uint16_t period;	//����(��λ10mS)
	uint16_t pulse;		//�������(��λ10mS)
	uint16_t num;		//��˸����
	uint16_t count;		//��˸��������
	uint16_t heart;		//��������
}led_set_str; 

typedef enum{
	RUN_LED=0,
	CAN_LED,
}LED_OPT;	//ledָʾ��ѡ��

#define ON  1
#define OFF 0

#define RUN_LED_RUN(a)		if(a){ HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);}\
							else{ HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); }
							
void led_operation(led_set_str * led_data);
void led_set(uint8_t opt,uint16_t period, uint16_t pulse,uint16_t num);
extern led_set_str run_led; //����ָʾ��
              
#endif

