#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "stdint.h"
#include "main.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define DISplay_SCLK_Clr() HAL_GPIO_WritePin(DIS_SCL_GPIO_Port, DIS_SCL_Pin, GPIO_PIN_RESET)//SCL
#define DISplay_SCLK_Set() HAL_GPIO_WritePin(DIS_SCL_GPIO_Port, DIS_SCL_Pin, GPIO_PIN_SET)

#define DISplay_SDIN_Clr() HAL_GPIO_WritePin(DIS_SDA_GPIO_Port, DIS_SDA_Pin, GPIO_PIN_RESET)//SDA
#define DISplay_SDIN_Set() HAL_GPIO_WritePin(DIS_SDA_GPIO_Port, DIS_SDA_Pin, GPIO_PIN_SET)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void DISplay_WR_Byte(unsigned dat,unsigned cmd);  
void DISplay_Display_On(void);
void DISplay_Display_Off(void);	   							   		    
void DISplay_Init(void);
void DISplay_Clear(void);
void DISplay_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void DISplay_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void DISplay_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void DISplay_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void DISplay_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t Char_Size);	 
void DISplay_Set_Pos(unsigned char x, unsigned char y);
void DISplay_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void DISplay_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void DISplayfill_picture(unsigned char fill_Data);
void DISplayIIC_Start(void);
void DISplayIIC_Stop(void);
void DISplayWrite_IIC_Command(unsigned char IIC_Command);
void DISplayWrite_IIC_Data(unsigned char IIC_Data);
void DISplayWrite_IIC_Byte(unsigned char IIC_Byte);
void DISplayIIC_Wait_Ack(void);

void DISplay_ShowFolatNum(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size2); //len:带小数点、负号数据个数

#endif

