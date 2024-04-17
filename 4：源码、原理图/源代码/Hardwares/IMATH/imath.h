#ifndef _IMATH_H_
#define _IMATH_H_
#include "main.h"
#include "mpu6050.h"

float invSqrt(float x);

void _set_val(SI_F_XYZ *_out_data,SI_F_XYZ *_in_data);


HAL_StatusTypeDef FloatComparison(SI_F_XYZ *_in_data , float templt);

void  setFloatValue(SI_F_XYZ *_in_data,float value);

float f_abs(float f);

uint16_t u16RangeLimit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max);
int int_abs(int f);


#endif
