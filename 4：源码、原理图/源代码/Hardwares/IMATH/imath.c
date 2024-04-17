#include "imath.h"


// Fast inverse square-root
float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}
/**
  * @brief   �������ݽ��и�ֵ����
  * @param   *_out_data �����������ָ�����
  * @param   *_in_data ������������ָ�����
  * @retval  x
  */
void _set_val(SI_F_XYZ *_out_data,SI_F_XYZ *_in_data)
{
  _out_data->x = _in_data->x;
  _out_data->y = _in_data->y;
  _out_data->z = _in_data->z;
}
/*
 * ��������set_value
 * ����  �������ݸ�ֵ
 * ����  ��_in_dat���������׵�ַ�� value����Ҫ����ֵ
 * ����  ��     
 */
void  setFloatValue(SI_F_XYZ *_in_data,float value)
{
    _in_data->x = value;
    _in_data->y = value;
    _in_data->z = value;
}
/*
 * ��������f_abs
 * ����  �����������ݾ���ֵ
 * ����  ��f�������� 
 * ����  ������ֵ 
 */
float f_abs(float f)
{
	if (f >= 0.0f)
		return f;
	return -f;
}
/**
  * @brief   �������ݱȽ��жϽ��б�־����
  * @param   *_in_data ���뱻�Ƚϵ���������ָ�����
  * @param   templt ��ȽϵĲο�ֵ
  * @retval  HAL_OK �ڲο�ֵ��Χ�ڷ���OK��HAL_ERROR ���ڲο���Χ�ڷ���ERROR
  */
HAL_StatusTypeDef FloatComparison(SI_F_XYZ *_in_data , float templt)
{
  if((f_abs(_in_data->x) > templt) || (f_abs(_in_data->y) > templt) || (f_abs(_in_data->z) > templt))
    return HAL_ERROR;
  return HAL_OK;
}

/**
  * @brief   16λ�������ݽ��з�Χ����
  * @param   thr_in ��Ҫ�����Ƶ���������
  * @param   thr_min ��Сֵ
  * @param   thr_max ���ֵ
  * @retval  �����Ʒ�Χ�ڵ�ֵ
  */
uint16_t u16RangeLimit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max)
{
	if(thr_in < thr_min)	thr_in = thr_min;
	if(thr_in > thr_max)	thr_in = thr_max;
  
	return thr_in;
}
/**
  * @brief   �������ݽ���ȡ����ֵ
  * @param   f ��Ҫ��ȡ����ֵ����������
  * @retval  ����ֵ����
  */
int int_abs(int f)
{
	if (f >= 0)
		return f;
	return -f;
}
