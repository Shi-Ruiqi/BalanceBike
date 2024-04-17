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
  * @brief   三轴数据进行赋值操作
  * @param   *_out_data 输出数据三轴指针变量
  * @param   *_in_data 输入三轴数据指针变量
  * @retval  x
  */
void _set_val(SI_F_XYZ *_out_data,SI_F_XYZ *_in_data)
{
  _out_data->x = _in_data->x;
  _out_data->y = _in_data->y;
  _out_data->z = _in_data->z;
}
/*
 * 函数名：set_value
 * 描述  ：给数据赋值
 * 输入  ：_in_dat输入数据首地址， value所需要赋的值
 * 返回  ：     
 */
void  setFloatValue(SI_F_XYZ *_in_data,float value)
{
    _in_data->x = value;
    _in_data->y = value;
    _in_data->z = value;
}
/*
 * 函数名：f_abs
 * 描述  ：浮点型数据绝对值
 * 输入  ：f浮点数据 
 * 返回  ：绝对值 
 */
float f_abs(float f)
{
	if (f >= 0.0f)
		return f;
	return -f;
}
/**
  * @brief   浮点数据比较判断进行标志返回
  * @param   *_in_data 输入被比较的三轴数据指针变量
  * @param   templt 相比较的参考值
  * @retval  HAL_OK 在参考值范围内返回OK，HAL_ERROR 不在参考范围内返回ERROR
  */
HAL_StatusTypeDef FloatComparison(SI_F_XYZ *_in_data , float templt)
{
  if((f_abs(_in_data->x) > templt) || (f_abs(_in_data->y) > templt) || (f_abs(_in_data->z) > templt))
    return HAL_ERROR;
  return HAL_OK;
}

/**
  * @brief   16位整形数据进行范围限制
  * @param   thr_in 需要被限制的输入数据
  * @param   thr_min 最小值
  * @param   thr_max 最大值
  * @retval  在限制范围内的值
  */
uint16_t u16RangeLimit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max)
{
	if(thr_in < thr_min)	thr_in = thr_min;
	if(thr_in > thr_max)	thr_in = thr_max;
  
	return thr_in;
}
/**
  * @brief   整形数据进行取绝对值
  * @param   f 需要被取绝对值的输入数据
  * @retval  绝对值数据
  */
int int_abs(int f)
{
	if (f >= 0)
		return f;
	return -f;
}
