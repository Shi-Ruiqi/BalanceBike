#include "flash.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "usart.h"
#include "stdio.h"

#define DEBUG
#define FLASH_SIZE 64                   /* ��ѡMCU��FLASH������С(��λΪK) */

#if FLASH_SIZE < 256                    /* flashС��256K�ֽڵ�оƬ��һ��������ַΪ1K������Ϊ2K */
#define SECTOR_SIZE 1024                /* �ֽ� */
#else
#define SECTOR_SIZE 2048                /* �ֽ� */
#endif

_FLASH_flag flash_flag = {0};


/* Variable used for Erase procedure */
static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t PageError;

/**
  * @brief   ��flashд��������ݣ�16λ��
  * @param   startAddress д�봦����ʼ��ַ
  * @param   *writeData 16λ����ָ�����
  * @param   countToWrite д��İ�����������
  * @retval  x
  */
void FLASH_WriteHalfWordData( uint32_t startAddress, uint16_t *writeData, uint16_t countToWrite )
{
	uint16_t	i;
	uint32_t	offsetAddress;  /* ƫ�Ƶ�ַ */
	uint32_t	sectorPosition; /* ����λ�� */
	uint32_t	sectorStartAddress;
	if ( startAddress < FLASH_BASE || ( (startAddress + countToWrite) >= (FLASH_BASE + 1024 * FLASH_SIZE) ) )
	{
		return;                 /* �Ƿ���ַ */
	}
	/* ����д���� */
	HAL_FLASH_Unlock();

	/* ����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ */
	offsetAddress = startAddress - FLASH_BASE;
	/* ����������ַ */
	sectorPosition = offsetAddress / SECTOR_SIZE;
	/* ��Ӧ�������׵�ַ */
	sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE;

	/* ����������� */
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = sectorStartAddress;
  EraseInitStruct.NbPages = 1;
  printf("����ҳ����%d\r\n",EraseInitStruct.NbPages);
  if ( HAL_FLASHEx_Erase( &EraseInitStruct, &PageError) == HAL_OK )      //д��ǰ�Ƚ���ҳ����
  {
    printf("�����ɹ�\r\n");
  } 
	for ( i = 0; i < countToWrite; i++ )
	{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,startAddress, writeData[i]);
		startAddress = startAddress + 2;
	}
  
	HAL_FLASH_Lock(); /*����д���� */
}
/**
  * @brief   ��flashд�������ݣ�32λ��
  * @param   startAddress д�봦����ʼ��ַ
  * @param   *writeData 32λ����ָ�����
  * @param   countToWrite д��İ�����������
  * @retval  x
  */
void FLASH_WriteWordData( uint32_t startAddress, uint32_t *writeData, uint16_t countToWrite )
{
	uint16_t	i;
	uint32_t	offsetAddress;  /* ƫ�Ƶ�ַ */
	uint32_t	sectorPosition; /* ����λ�� */
	uint32_t	sectorStartAddress;
	if ( startAddress < FLASH_BASE || ( (startAddress + countToWrite) >= (FLASH_BASE + 1024 * FLASH_SIZE) ) )
	{
		return;                 /* �Ƿ���ַ */
	}
	/* ����д���� */
	HAL_FLASH_Unlock();

	/* ����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ */
	offsetAddress = startAddress - FLASH_BASE;
	/* ����������ַ */
	sectorPosition = offsetAddress / SECTOR_SIZE;
	/* ��Ӧ�������׵�ַ */
	sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE;

	/* ����������� */
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = sectorStartAddress;
  EraseInitStruct.NbPages = 1;
  printf("����ҳ����%d\r\n",EraseInitStruct.NbPages);
  if ( HAL_FLASHEx_Erase( &EraseInitStruct, &PageError) == HAL_OK )      //д��ǰ�Ƚ���ҳ����
  {
    printf("�����ɹ�\r\n");
  } 
	for ( i = 0; i < countToWrite; i++ )
	{
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,startAddress, writeData[i]);
		startAddress = startAddress + 4;
	}
  
	HAL_FLASH_Lock(); /*����д���� */
}

/**
  * @brief   ��flashд����������������
  * @param   startAddress д�봦����ʼ��ַ
  * @param   writeData1 ��������1
  * @param   writeData2 ��������2
  * @param   writeData3 ��������3
  * @retval  x
  */
void FLASH_WriteThreeFloatData( uint32_t startAddress,  float writeData1, 
                                                        float writeData2,
                                                        float writeData3)
{
  uint32_t wData[3];
  
  /* ��д��ĸ�������ǿ��תΪ�������ݣ���������д�� */
  wData[0] = *(uint32_t *)(&writeData1);
  wData[1] = *(uint32_t *)(&writeData2);
  wData[2] = *(uint32_t *)(&writeData3);
  
  FLASH_WriteWordData(startAddress,&wData[0],3);
}

/*  δʹ�ã���ʱ����

//��ȡָ����ַ�İ���(16λ����)
static uint16_t FLASH_ReadHalfWord(uint32_t address)
{
  return *(__IO uint16_t*) address; 
}
//��ȡָ����ַ����(32λ����)
static uint32_t FLASH_ReadWord(uint32_t address)
{
  return *(__IO uint32_t*) address; 
}
//��ȡָ����ַ�İ���(32λ����)
static float FLASH_ReadFloatWord(uint32_t address)
{
  return *(__IO float*) address; 
}
*/

/**
  * @brief   ��flash��������������
  * @param   startAddress ����������ʼ��ַ
  * @param   *readData �洢�������ݵ�ָ�����
  * @param   countToRead �������ݳ���
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












