#ifndef __FILTER_H__
#define __FILTER_H__

#include "stdint.h"  

typedef struct
{
  float input_data[3];
  float output_data[3];
}_Butterworth_data;

typedef struct
{
  const float a[3];
  const float b[3];
}_Butterworth_parameter;

float butterworth_lpf(float now_input,_Butterworth_data *buffer, _Butterworth_parameter *parameter);







#endif

