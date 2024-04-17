
#include "led.h"
#include "string.h"

led_set_str run_led; //����ָʾ��


/*****************************************************************************
** ��������: led_ctl 
** ��������: led �������
** �������: opt ledѡ��
** �������: cmd led��������
** �����������
** �� �� ֵ����
*****************************************************************************/
static void led_ctl(uint8_t opt,uint8_t cmd)
{
	switch(opt)
	{
		case RUN_LED:
			 RUN_LED_RUN(cmd);
		break;
//		case CAN_LED:
//			 CAN_LED_RUN(cmd);
//		break;
		default:break;
	}
}

/*****************************************************************************
** ��������: led_operation(led_set_str * led_data)
** ��������: led ���к�������������һ�������������ۼӣ�����ռ�ձȾ�������ʱ��
** �������: led ���ýṹ�����
** �����������
** �� �� ֵ����
*****************************************************************************/
void led_operation(led_set_str * led_data)
{
	led_data->heart++;
	if(led_data->heart> 0 && led_data->heart < led_data->pulse)
	{
		led_ctl(led_data->led_opt,ON);
	}
	else if(led_data->heart >= led_data->pulse && led_data->heart < led_data->period)
	{
		led_ctl(led_data->led_opt,OFF);
	}
	else if(led_data->heart >= led_data->period)
	{
		led_data->heart = 0;
		
		if(led_data->num != 0)
		{
			if(++led_data->count >= led_data->num)
			{
				memset(led_data , 0 , sizeof(led_set_str));
			}
		}
	}
}

/*****************************************************************************
** ��������: led_set
** ��������: led����˸����
** �������: uint8_t opt led ѡ��
** �������: uint16_t period ��˸����
** �������: uint16_t duty_cycle ����ռ�ձ�
** �������: uint16_t num ��˸����,�����0��������˸
** �����������
** �� �� ֵ����
*****************************************************************************/
void led_set(uint8_t opt,uint16_t period, uint16_t pulse,uint16_t num)
{
	led_set_str led_data;
	led_data.led_opt = opt;
	led_data.period = period;
	led_data.pulse = pulse;
	led_data.num = num;
	led_data.heart = 0;
	led_data.count = 0;
	switch(opt)
	{
		case RUN_LED:
			memcpy(&run_led,&led_data,sizeof(led_set_str));
		break;
//		case CAN_LED:
//			memcpy(&can_led,&led_data,sizeof(led_set_str));
//		break;
		
		default:break;
	}
}

