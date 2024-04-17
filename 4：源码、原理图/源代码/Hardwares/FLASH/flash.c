#include "flash.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "usart.h"
#include "stdio.h"

#define DEBUG
#define FLASH_SIZE 64                   /* 所选MCU的FLASH容量大小(单位为K) */

#if FLASH_SIZE < 256                    /* flash小于256K字节的芯片的一个扇区地址为1K，否则为2K */
#define SECTOR_SIZE 1024                /* 字节 */
#else
#define SECTOR_SIZE 2048                /* 字节 */
#endif

_FLASH_flag flash_flag = {0};


/* Variable used for Erase procedure */
static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t PageError;

/**
  * @brief   向flash写入半字数据（16位）
  * @param   startAddress 写入处的起始地址
  * @param   *writeData 16位数据指针变量
  * @param   countToWrite 写入的半字数据数量
  * @retval  x
  */
void FLASH_WriteHalfWordData( uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite )
{
	uint16_t	i;
	uint32_t	offsetAddress;  /* 偏移地址 */
	uint32_t	sectorPosition; /* 扇区位置 */
	uint32_t	sectorStartAddress;
	if ( startAddress < FLASH_BASE || ( (startAddress + countToWrite) >= (FLASH_BASE + 1024 * FLASH_SIZE) ) )
	{
		return;                 /* 非法地址 */
	}
	/* 解锁写保护 */
	HAL_FLASH_Unlock();

	/* 计算去掉0X08000000后的实际偏移地址 */
	offsetAddress = startAddress - FLASH_BASE;
	/* 计算扇区地址 */
	sectorPosition = offsetAddress / SECTOR_SIZE;
	/* 对应扇区的首地址 */
	sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE;

	/* 擦除这个扇区 */
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = sectorStartAddress;
  EraseInitStruct.NbPages = 1;
  printf("擦除页数：%d\r\n",EraseInitStruct.NbPages);
  if ( HAL_FLASHEx_Erase( &EraseInitStruct, &PageError) == HAL_OK )      //写入前先进行页擦除
  {
    printf("擦除成功\r\n");
  } 
	for ( i = 0; i < countToWrite; i++ )
	{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,startAddress, writeData[i]);
		startAddress = startAddress + 2;
	}
  
	HAL_FLASH_Lock(); /*上锁写保护 */
}
/**
  * @brief   向flash写入字数据（32位）
  * @param   startAddress 写入处的起始地址
  * @param   *writeData 32位数据指针变量
  * @param   countToWrite 写入的半字数据数量
  * @retval  x
  */
void FLASH_WriteWordData( uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite )
{
	uint16_t	i;
	uint32_t	offsetAddress;  /* 偏移地址 */
	uint32_t	sectorPosition; /* 扇区位置 */
	uint32_t	sectorStartAddress;
	if ( startAddress < FLASH_BASE || ( (startAddress + countToWrite) >= (FLASH_BASE + 1024 * FLASH_SIZE) ) )
	{
		return;                 /* 非法地址 */
	}
	/* 解锁写保护 */
	HAL_FLASH_Unlock();

	/* 计算去掉0X08000000后的实际偏移地址 */
	offsetAddress = startAddress - FLASH_BASE;
	/* 计算扇区地址 */
	sectorPosition = offsetAddress / SECTOR_SIZE;
	/* 对应扇区的首地址 */
	sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE;

	/* 擦除这个扇区 */
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = sectorStartAddress;
  EraseInitStruct.NbPages = 1;
  printf("擦除页数：%d\r\n",EraseInitStruct.NbPages);
  if ( HAL_FLASHEx_Erase( &EraseInitStruct, &PageError) == HAL_OK )      //写入前先进行页擦除
  {
    printf("擦除成功\r\n");
  } 
	for ( i = 0; i < countToWrite; i++ )
	{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,startAddress, writeData[i]);
		startAddress = startAddress + 4;
	}
  
	HAL_FLASH_Lock(); /*上锁写保护 */
}

/**
  * @brief   向flash写入三个浮点型数据
  * @param   startAddress 写入处的起始地址
  * @param   writeData1 浮点数据1
  * @param   writeData2 浮点数据2
  * @param   writeData3 浮点数据3
  * @retval  x
  */
void FLASH_WriteThreeFloatData( uint32_t startAddress,  float writeData1, 
                                                        float writeData2,
                                                        float writeData3)
{
  uint32_t wData[3];
  
  /* 待写入的浮点数据强制转为整形数据，方便数据写入 */
  wData[0] = *(uint32_t *)(&writeData1);
  wData[1] = *(uint32_t *)(&writeData2);
  wData[2] = *(uint32_t *)(&writeData3);
  
  FLASH_WriteWordData(startAddress,&wData[0],3);
}

/*  未使用，暂时屏蔽

//读取指定地址的半字(16位数据)
static uint16_t FLASH_ReadHalfWord(uint32_t address)
{
  return *(__IO uint16_t*) address; 
}
//读取指定地址的字(32位数据)
static uint32_t FLASH_ReadWord(uint32_t address)
{
  return *(__IO uint32_t*) address; 
}
//读取指定地址的半字(32位数据)
static float FLASH_ReadFloatWord(uint32_t address)
{
  return *(__IO float*) address; 
}
*/

/**
  * @brief   从flash读出浮点型数据
  * @param   startAddress 读出处的起始地址
  * @param   *readData 存储读出数据的指针变量
  * @param   countToRead 读出数据长度
  * @retval  x
  */
void FLASH_ReadFloatData(uint32_t startAddress,float *readData,uint16_t countToRead)
{
  uint16_t dataIndex;
  for( dataIndex = 0 ; dataIndex < countToRead ; dataIndex++ )
  {
    readData[dataIndex] = *(__IO float*) (startAddress + dataIndex * 4);
  }
}












