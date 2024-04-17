#ifndef _ANO_H_
#define _ANO_H_

#include "stdint.h"

#define SENSOR_GYRO 02
#define USART_RX_LEN 100 

#define CAL_SUCCESS             0x01
#define SETUP_SUCCESS           0x31
#define RESTORE_DEFAULT_SUCCESS 0xA1


typedef struct 
{
  uint8_t send_version;
  uint8_t send_status;
  uint8_t send_senser;
  uint8_t send_pid1;
  uint8_t send_pid2;
  uint8_t send_pid3;
  uint8_t send_pid4;
  uint8_t send_pid5;
  uint8_t send_pid6;
  uint8_t send_rcdata;
  uint8_t send_offset;
  uint8_t send_motopwm;
  uint8_t send_power;

}dt_flag_t;

void ANO_DMA_SEND_DATA(void);

void ANO_DT_Data_Receive_Prepare(uint8_t data);
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num);

#endif

