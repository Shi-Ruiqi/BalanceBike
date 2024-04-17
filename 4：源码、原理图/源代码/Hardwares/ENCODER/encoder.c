#include "encoder.h"
#include "tim.h"


_ENCODER_INFO encoderINFO = { 0 } ;

/**
  * @brief   ����ֵ������ԭ����ת������С����������ת��Ϊ��0��ʼ��С����������
  * @param   *ecdData ��������ָ�����
  * @retval  x
  */
static void EncoderManage(int *ecdData)
{
  //����������ʱ��תΪ������ʱ��Ӧ�ĸ���
  if(*ecdData > FULL_ENCODER * 0.5f)
    *ecdData = *ecdData - FULL_ENCODER ; 
  else 
    *ecdData = *ecdData;  
}

/**
  * @brief   ��ȡ������ת��ֵ
  * @param   x
  * @retval  x
  */
uint8_t readEncoderValue(void)
{
  encoderINFO.directionValue = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);      //��ȡ����ת���ķ���(����ת�������ж���ȷ���˴�δʹ�ã����ٻ������������ж�)
  encoderINFO.mainNumberValue = __HAL_TIM_GET_COUNTER(&htim2);              //��ȡ����ֵ
  __HAL_TIM_SET_COUNTER(&htim2,0);                                          //�������ֵ
  
  EncoderManage(&encoderINFO.mainNumberValue) ;                             //���������ת���ı���ֵתΪ��������
  
  return 0;
}





