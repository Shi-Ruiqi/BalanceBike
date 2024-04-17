#include "display.h"
#include "displayfont.h"

/**********************************************
//IIC Start
**********************************************/
void DISplayIIC_Start(void)
{
	DISplay_SCLK_Set() ;
	DISplay_SDIN_Set();
	DISplay_SDIN_Clr();
	DISplay_SCLK_Clr();
}
/**********************************************
//IIC Stop
**********************************************/
void DISplayIIC_Stop(void)
{
  DISplay_SCLK_Set() ;
//	DISplay_SCLK_Clr();
	DISplay_SDIN_Clr();
	DISplay_SDIN_Set();
}

void DISplayIIC_Wait_Ack(void)
{
	DISplay_SCLK_Set() ;
	DISplay_SCLK_Clr();
}
/**********************************************
// DISplayIIC Write byte
**********************************************/
void DISplayWrite_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	DISplay_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
    m=da;
		//  DISplay_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
      {DISplay_SDIN_Set();} 
		else 
      DISplay_SDIN_Clr();
    da=da<<1;
		DISplay_SCLK_Set();
		DISplay_SCLK_Clr();
  }
}
/**********************************************
// DISplayIIC Write Command
**********************************************/
void DISplayWrite_IIC_Command(unsigned char IIC_Command)
{
  DISplayIIC_Start();
  DISplayWrite_IIC_Byte(0x78);            //Slave address,SA0=0
  DISplayIIC_Wait_Ack();	
  DISplayWrite_IIC_Byte(0x00);			      //write command
  DISplayIIC_Wait_Ack();	
  DISplayWrite_IIC_Byte(IIC_Command); 
  DISplayIIC_Wait_Ack();	 
  DISplayIIC_Stop();
}
/**********************************************
// DISplayIIC Write Data
**********************************************/
void DISplayWrite_IIC_Data(unsigned char IIC_Data)
{
  DISplayIIC_Start();
  DISplayWrite_IIC_Byte(0x78);			//D/C#=0; R/W#=0
  DISplayIIC_Wait_Ack();	
  DISplayWrite_IIC_Byte(0x40);			//write data
  DISplayIIC_Wait_Ack();	
  DISplayWrite_IIC_Byte(IIC_Data);
  DISplayIIC_Wait_Ack();	
  DISplayIIC_Stop();
}
void DISplay_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
  {
    DISplayWrite_IIC_Data(dat);
  }
	else 
  {
    DISplayWrite_IIC_Command(dat);
	}
}

/********************************************
// fill_Picture
********************************************/
void DISplayfill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		DISplay_WR_Byte(0xb0+m,0);		//page0-page1
		DISplay_WR_Byte(0x00,0);		//low column start address
		DISplay_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
    {
      DISplay_WR_Byte(fill_Data,1);
    }
	}
}

void DISplay_Set_Pos(unsigned char x, unsigned char y) 
{ 	
  DISplay_WR_Byte(0xb0+y,OLED_CMD);
	DISplay_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	DISplay_WR_Byte((x&0x0f),OLED_CMD); 
}   	  

//开启OLED显示    
void DISplay_Display_On(void)
{
	DISplay_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	DISplay_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	DISplay_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void DISplay_Display_Off(void)
{
	DISplay_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	DISplay_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	DISplay_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void DISplay_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		DISplay_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		DISplay_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		DISplay_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
      DISplay_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void DISplay_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		DISplay_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		DISplay_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		DISplay_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
      DISplay_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void DISplay_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
  unsigned char c=0,i=0;	
  c=chr-' ';//得到偏移后的值			
  if(x>Max_Column-1) {x=0;y=y+2;}
  if(Char_Size ==16)
  {
    DISplay_Set_Pos(x,y);	
    for(i=0;i<8;i++)
      DISplay_WR_Byte(F8X16[c*16+i],OLED_DATA);
    DISplay_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
      DISplay_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
  }
  else 
  {	
    DISplay_Set_Pos(x,y);
    for(i=0;i<6;i++)
      DISplay_WR_Byte(F6x8[c][i],OLED_DATA);
  }
}
//m^n函数
uint32_t DISplay_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)
    result *= m;    
	return result;
}				  
//显示数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);	 		  
void DISplay_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size2)
{         	
  uint8_t t,temp;
  uint8_t enshow = 0;						   
  for(t=0;t<len;t++)
  {
    temp = (num / DISplay_pow(10,len-t-1) ) %10;
    if(enshow==0 && t<(len-1))
    {
      if(temp==0)
      {
        DISplay_ShowChar(x+(size2/2)*t,y,' ',size2);
          continue;
      }
      else 
        enshow=1; 
    }
    DISplay_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
  }
} 

//显示小数
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//num:数值
void DISplay_ShowFolatNum(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size2)//len:带小数点、负号数据个数
{         	
  uint8_t t,temp,i=0;
  uint8_t enshow=0;
  uint16_t k;
  
  if(num<0)
  {
    num = -num;
    i = 1;     //负数标志	
  }	
  k = num * 10; //此处为显示一位小数*10转化为整数
  for(t = 0 ; t < len ; t++)
  {
    temp = (k / DISplay_pow(10,len-t-1)) % 10;
    if(enshow==0 && t<(len-1))
    {
      if(temp==0)
      {
        if(( (k / DISplay_pow(10,len-t-2)%10) != 0) && (i==1))//判断是否为负数且在最高位前一位
        {
          DISplay_ShowChar(x+(size2/2)*t,y,'-',size2);
          i=0;	                                    //清除判断后一位的标志
        }
        else
          DISplay_ShowChar(x+(size2/2)*t,y,' ',size2);   //如果没到数字就显示空格
        continue;
      }
      else 
        enshow = 1;		//此处是判断是否要显示数字	
    }
    if(t==len-1)
    {
      DISplay_ShowChar(x+(size2/2)*t,y,'.',size2);
      //判断是否为最后一位的前一位（显示一位小数）
      t++;
    }
    DISplay_ShowChar(x+(size2/2)*t,y,temp+'0',size2); //一位一位显示下去
  }
}

//显示一个字符号串
void DISplay_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
    DISplay_ShowChar(x,y,chr[j],Char_Size);
    x+=8;
		if(x>120)
    {x=0;y+=2;}
    j++;
	}
}
//显示汉字
void DISplay_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	DISplay_Set_Pos(x,y);	
  for(t=0;t<16;t++)
  {
    DISplay_WR_Byte(Hzk[2*no][t],OLED_DATA);
    adder+=1;
  }	
  DISplay_Set_Pos(x,y+1);	
  for(t=0;t<16;t++)
  {	
    DISplay_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
    adder+=1;
  }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void DISplay_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) 
    y=y1/8;      
  else 
    y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		DISplay_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
    {      
      DISplay_WR_Byte(BMP[j++],OLED_DATA);	    	
    }
	}
} 




//初始化SSD1306					    
void DISplay_Init(void)
{ 	
  HAL_Delay(800);
  
  DISplay_WR_Byte(0xAE,OLED_CMD);//--display off
	DISplay_WR_Byte(0x00,OLED_CMD);//---set low column address
	DISplay_WR_Byte(0x10,OLED_CMD);//---set high column address
	DISplay_WR_Byte(0x40,OLED_CMD);//--set start line address  
	DISplay_WR_Byte(0xB0,OLED_CMD);//--set page address
	DISplay_WR_Byte(0x81,OLED_CMD); // contract control
	DISplay_WR_Byte(0xFF,OLED_CMD);//--128   
	DISplay_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	DISplay_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	DISplay_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	DISplay_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	DISplay_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	DISplay_WR_Byte(0xD3,OLED_CMD);//-set display offset
	DISplay_WR_Byte(0x00,OLED_CMD);//
	
	DISplay_WR_Byte(0xD5,OLED_CMD);//set osc division
	DISplay_WR_Byte(0x80,OLED_CMD);//
	
	DISplay_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	DISplay_WR_Byte(0x05,OLED_CMD);//
	
	DISplay_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	DISplay_WR_Byte(0xF1,OLED_CMD);//
	
	DISplay_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	DISplay_WR_Byte(0x12,OLED_CMD);//
	
	DISplay_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	DISplay_WR_Byte(0x30,OLED_CMD);//
	
	DISplay_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	DISplay_WR_Byte(0x14,OLED_CMD);//
	
	DISplay_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  
