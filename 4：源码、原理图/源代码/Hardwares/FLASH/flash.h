#ifndef __FLASH_H_
#define __FLASH_H_

#include "stdint.h"

typedef struct
{
  uint16_t sensor;
  uint8_t pid;
//    uint16_t acc_offset;
//    uint8_t acc_scale;
//    uint8_t mag_offset;
//	uint8_t gyro_offset;
}_FLASH_flag;


#define START_ADDRESS  (0x08000000 + 1024*40)
#define SENSOR_CAL_ADDRESS    START_ADDRESS + 36
#define PID_Group1_ADDRESS    SENSOR_CAL_ADDRESS + 36
#define PID_Group2_ADDRESS    PID_Group1_ADDRESS + 36
#define PID_Group3_ADDRESS    PID_Group2_ADDRESS + 36
#define PID_FLAG_ADDRESS      PID_Group3_ADDRESS + 4  

/* Private define ------------------------------------------------------------*/
#define FLASH_USER_START_ADDR   START_ADDRESS                     /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (START_ADDRESS + FLASH_PAGE_SIZE)   /* End @ of user Flash area */
                                       
extern _FLASH_flag flash_flag;

void FLASH_WriteHalfWordData( uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite );
void FLASH_WriteWordData( uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite );
void FLASH_ReadFloatData(uint32_t startAddress,float *readData,uint16_t countToRead);
void FLASH_WriteFloatData( uint32_t startAddress, float writeData[], uint16_t countToWrite );
void FLASH_WriteThreeFloatData( uint32_t startAddress,  float writeData1, float writeData2, float writeData3);


#endif
