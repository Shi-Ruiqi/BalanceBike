
#include "led.h"
#include "string.h"

led_set_str run_led; //运行指示灯


/*****************************************************************************
** 函数名称: led_ctl 
** 功能描述: led 亮灭控制
** 输入参数: opt led选择
** 输入参数: cmd led亮灭命令
** 输出参数：无
** 返 回 值：无
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
** 函数名称: led_operation(led_set_str * led_data)
** 功能描述: led 运行函数，计数器在一个亮灭周期内累加，根据占空比决定亮灭时间
** 输入参数: led 设置结构体变量
** 输出参数：无
** 返 回 值：无
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
** 函数名称: led_set
** 功能描述: led灯闪烁设置
** 输入参数: uint8_t opt led 选择
** 输入参数: uint16_t period 闪烁周期
** 输入参数: uint16_t duty_cycle 亮度占空比
** 输入参数: uint16_t num 闪烁次数,如果是0则无限闪烁
** 输出参数：无
** 返 回 值：无
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

