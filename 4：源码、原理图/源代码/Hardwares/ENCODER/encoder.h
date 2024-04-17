#ifndef _ENCODER_h_
#define _ENCODER_h_


#include "stdint.h"
#include "main.h"

#define FULL_ENCODER 50000


typedef struct 
{
  int mainNumberValue;
  uint8_t directionValue;  
  
}_ENCODER_INFO;

uint8_t readEncoderValue(void);

extern _ENCODER_INFO encoderINFO ;

#endif
