#include "bluetooth.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "imath.h"
#include "imu.h"
#include "mpu6050.h"

BluetoothParse BluetoothParseMsg = {2,2,0,0};

const char FirmwareVersion[] = "BK20211221";
const char ChipVersion[] = "STM32F103C8T6";

/*
函数功能：串口2DMA数据发送
函数形参：Sendbuff ：缓冲数据
          Bufflens ：数据长度
函数返回值：数据长度
备注：无
*/
uint16_t Usart2_DMA_Sent(uint8_t * Sendbuff, uint16_t Bufflens)
{
	uint16_t l_val = Bufflens > USART_BUFFSIZE ? USART_BUFFSIZE : Bufflens;
	if(Bufflens <= 0)
	{
		return 0;
	}
//	while(__HAL_DMA_GET_COUNTER(&hdma_usart2_tx));  //检测DMA发送通道内还有没有数据
	if(Sendbuff)
	{
		memcpy( ( void * ) Usart2_Str.Usart_SentBuff, ( const void * )Sendbuff, l_val);
	}
	HAL_UART_Transmit_DMA(&huart2, Usart2_Str.Usart_SentBuff, l_val);
  return l_val;
}

/*
 * 函数名：ParseBluetoothMessage
 * 描述  ：解析蓝牙app发送过来的数据包
 * 输入  ：pInput收到的蓝牙遥控数据首地址 , blueParseMsg解析后保存的蓝牙数据有效信息
 * 返回  ：err
 */
/*
 * 协议形式：字符串（具体协议可参照app inventor文件里具体协议内容）
 * 帧头：# 帧尾：*
 * 例如：#9,1,1,2,2,1*  
 * 每位代表含义：
 * 9：数据长度(固定) 1：模式 1：关闭/开始状态 2：蓝牙摇杆X轴数据 2：蓝牙摇杆Y轴数据 1：校验值(累加和减去7)
*/
uint8_t ParseBluetoothMessage(char *pInput ,uint16_t rcLens , BluetoothParse *blueParseMsg )
{
  unsigned char plen,sum,check;
  ErrorStatus err;

  if(*pInput == '#')//  #9,1,1,2,2,1*                                               //接收到数据的包头#
  {
    plen = (unsigned char)atof(strtok(pInput + 1, ","));                               //解析长度length
    if(plen == 9)
    {
      /* 将读出的数据进行累加校验 */ 
      sum = (unsigned char)int_abs( ((int)atof(strtok(pInput+3, ",")) + 
            (int)atof(strtok(NULL, ",")) +
            (int)atof(strtok(NULL, ",")) + 
            (int)atof(strtok(NULL, ","))) - 7 );
      /* 读出累加校验数据 */
      check = (unsigned char)atof(strtok(NULL, ",")) ;                                //累加校验数据
      if(sum == check)                                                                //校验匹配成功才进行数据解析     
      {
        blueParseMsg->ModeRocker = (unsigned char)atof(strtok(pInput+3, ","));        //模式数据
        blueParseMsg->OpenCloseStatus = (unsigned char)atof(strtok(pInput+5, ","));   //关闭/开始状态数据
        blueParseMsg->Xrocker = (unsigned char)atof(strtok(pInput+7, ","));           //摇杆X数据
        blueParseMsg->Yrocker = (unsigned char)atof(strtok(pInput+9, ","));           //摇杆Y数据
        err = SUCCESS;
      }
      else
      {
        err = ERROR;  
      }
    }
    else 
    {
      err = ERROR;
    }
  }
  else 
  {
    err = ERROR;
  }

  return err ;
}

/*
函数功能：接收数据处理函数
函数形参：
函数返回值：
备注：无
*/
void Bluetooth_RecvData(void)
{
  if(Usart2_Str.RecvFlags == 1) 
  { 
    ParseBluetoothMessage( (char *)Usart2_Str.Usart_RecvBuff , Usart2_Str.Usart_RecvLens , &BluetoothParseMsg);   //进行数据解析
    memset (Usart2_Str.Usart_RecvBuff , 0,Usart2_Str.Usart_RecvLens);                 //缓存数据清除
    Usart2_Str.Usart_RecvLens = 0;                                                    //接收的数据长度清除
    Usart2_Str.RecvFlags = 0;                                                         //标志位清除
  }
}

/**
  * @brief   上传版本信息
  */
void UploadMsg_Version(void)
{
  char Msg[32];
	sprintf( Msg, "%s,%s\r\n", FirmwareVersion , ChipVersion );
  Usart2_DMA_Sent( (uint8_t*)Msg , sizeof(Msg));
}

/**
  * @brief   上传角度数据信息
  */
void UploadMsg_MpuData(void)
{
  char Msg[108];
	sprintf( Msg, "#R%.1f*R#P%.1f*P#Y%.1f*Y#GY%d*GY#GX%d*GX#GZ%d*GZ", 
                  att.rol , att.pit , att.yaw,
                  gyro_raw.y, gyro_raw.x , gyro_raw.z);
  
  Usart2_DMA_Sent( (uint8_t*)Msg , sizeof(Msg));
}

/**
  * @brief   上传陀螺仪数据信息
  */
void UploadMsg_Gyro(void)
{
  char Msg[64];
	sprintf( Msg, "#GY%d*GY #GX%d*GX #GZ%d*GZ \r\n", gyro_raw.y, gyro_raw.x , gyro_raw.z);
  
  Usart2_DMA_Sent( (uint8_t*)Msg , sizeof(Msg));
}

