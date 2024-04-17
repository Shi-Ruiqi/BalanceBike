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
�������ܣ�����2DMA���ݷ���
�����βΣ�Sendbuff ����������
          Bufflens �����ݳ���
��������ֵ�����ݳ���
��ע����
*/
uint16_t Usart2_DMA_Sent(uint8_t * Sendbuff, uint16_t Bufflens)
{
	uint16_t l_val = Bufflens > USART_BUFFSIZE ? USART_BUFFSIZE : Bufflens;
	if(Bufflens <= 0)
	{
		return 0;
	}
//	while(__HAL_DMA_GET_COUNTER(&hdma_usart2_tx));  //���DMA����ͨ���ڻ���û������
	if(Sendbuff)
	{
		memcpy( ( void * ) Usart2_Str.Usart_SentBuff, ( const void * )Sendbuff, l_val);
	}
	HAL_UART_Transmit_DMA(&huart2, Usart2_Str.Usart_SentBuff, l_val);
  return l_val;
}

/*
 * ��������ParseBluetoothMessage
 * ����  ����������app���͹��������ݰ�
 * ����  ��pInput�յ�������ң�������׵�ַ , blueParseMsg�����󱣴������������Ч��Ϣ
 * ����  ��err
 */
/*
 * Э����ʽ���ַ���������Э��ɲ���app inventor�ļ������Э�����ݣ�
 * ֡ͷ��# ֡β��*
 * ���磺#9,1,1,2,2,1*  
 * ÿλ�����壺
 * 9�����ݳ���(�̶�) 1��ģʽ 1���ر�/��ʼ״̬ 2������ҡ��X������ 2������ҡ��Y������ 1��У��ֵ(�ۼӺͼ�ȥ7)
*/
uint8_t ParseBluetoothMessage(char *pInput ,uint16_t rcLens , BluetoothParse *blueParseMsg )
{
  unsigned char plen,sum,check;
  ErrorStatus err;

  if(*pInput == '#')//  #9,1,1,2,2,1*                                               //���յ����ݵİ�ͷ#
  {
    plen = (unsigned char)atof(strtok(pInput + 1, ","));                               //��������length
    if(plen == 9)
    {
      /* �����������ݽ����ۼ�У�� */ 
      sum = (unsigned char)int_abs( ((int)atof(strtok(pInput+3, ",")) + 
            (int)atof(strtok(NULL, ",")) +
            (int)atof(strtok(NULL, ",")) + 
            (int)atof(strtok(NULL, ","))) - 7 );
      /* �����ۼ�У������ */
      check = (unsigned char)atof(strtok(NULL, ",")) ;                                //�ۼ�У������
      if(sum == check)                                                                //У��ƥ��ɹ��Ž������ݽ���     
      {
        blueParseMsg->ModeRocker = (unsigned char)atof(strtok(pInput+3, ","));        //ģʽ����
        blueParseMsg->OpenCloseStatus = (unsigned char)atof(strtok(pInput+5, ","));   //�ر�/��ʼ״̬����
        blueParseMsg->Xrocker = (unsigned char)atof(strtok(pInput+7, ","));           //ҡ��X����
        blueParseMsg->Yrocker = (unsigned char)atof(strtok(pInput+9, ","));           //ҡ��Y����
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
�������ܣ��������ݴ�����
�����βΣ�
��������ֵ��
��ע����
*/
void Bluetooth_RecvData(void)
{
  if(Usart2_Str.RecvFlags == 1) 
  { 
    ParseBluetoothMessage( (char *)Usart2_Str.Usart_RecvBuff , Usart2_Str.Usart_RecvLens , &BluetoothParseMsg);   //�������ݽ���
    memset (Usart2_Str.Usart_RecvBuff , 0,Usart2_Str.Usart_RecvLens);                 //�����������
    Usart2_Str.Usart_RecvLens = 0;                                                    //���յ����ݳ������
    Usart2_Str.RecvFlags = 0;                                                         //��־λ���
  }
}

/**
  * @brief   �ϴ��汾��Ϣ
  */
void UploadMsg_Version(void)
{
  char Msg[32];
	sprintf( Msg, "%s,%s\r\n", FirmwareVersion , ChipVersion );
  Usart2_DMA_Sent( (uint8_t*)Msg , sizeof(Msg));
}

/**
  * @brief   �ϴ��Ƕ�������Ϣ
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
  * @brief   �ϴ�������������Ϣ
  */
void UploadMsg_Gyro(void)
{
  char Msg[64];
	sprintf( Msg, "#GY%d*GY #GX%d*GX #GZ%d*GZ \r\n", gyro_raw.y, gyro_raw.x , gyro_raw.z);
  
  Usart2_DMA_Sent( (uint8_t*)Msg , sizeof(Msg));
}

