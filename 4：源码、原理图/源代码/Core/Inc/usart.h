/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN Private defines */
#define USART_BUFFSIZE    200   // 定义缓冲区的大小
typedef struct
{
	uint16_t Usart_SendLens;  //待发送数据长度
	uint16_t Usart_RecvLens;  //接收到的数据长度
	uint16_t RecvQue_Head;    //新接收数据环形队列头指针
	uint16_t RecvQue_Tail;    //新接收数据环形队列尾指针
	uint8_t Usart_SentBuff[USART_BUFFSIZE];
	uint8_t Usart_RecvBuff[USART_BUFFSIZE];
  uint8_t RecvFlags;
}USART_STR;

extern USART_STR Usart1_Str ;
extern USART_STR Usart2_Str ;

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */

uint16_t Usart1_DMA_Sent(uint8_t * Sendbuff, uint16_t Bufflens);
uint16_t Uart_Receive_Data(USART_STR * Uart_Str, uint8_t * RcvBuff, uint16_t RevLen);
void pu8printf(char* name, uint8_t* pbuf, uint32_t count, uint8_t Mode);
void Usart_RecvData(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

