#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stdint.h"
typedef struct BluetoothParseMsg
{
  float Xrocker;
  float Yrocker;
  uint8_t ModeRocker;
  uint8_t OpenCloseStatus;
}BluetoothParse;

void Bluetooth_RecvData(void);
uint8_t ParseBluetoothMessage(char *pInput ,uint16_t rcLens , BluetoothParse *blueParseMsg );

void UploadMsg_MpuData(void);
void UploadMsg_Gyro(void);

extern BluetoothParse BluetoothParseMsg ;
#endif
