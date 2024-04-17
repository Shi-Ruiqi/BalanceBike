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

//����OLED��ʾ    
void DISplay_Display_On(void)
{
	DISplay_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	DISplay_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	DISplay_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void DISplay_Display_Off(void)
{
	DISplay_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	DISplay_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	DISplay_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void DISplay_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		DISplay_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		DISplay_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		DISplay_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
      DISplay_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}
void DISplay_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		DISplay_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		DISplay_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		DISplay_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
      DISplay_WR_Byte(1,OLED_DATA); 
	} //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void DISplay_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
  unsigned char c=0,i=0;	
  c=chr-' ';//�õ�ƫ�ƺ��ֵ			
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
//m^n����
uint32_t DISplay_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)
    result *= m;    
	return result;
}				  
//��ʾ����
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//num:��ֵ(0~4294967295);	 		  
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

//��ʾС��
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//num:��ֵ
void DISplay_ShowFolatNum(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size2)//len:��С���㡢�������ݸ���
{         	
  uint8_t t,temp,i=0;
  uint8_t enshow=0;
  uint16_t k;
  
  if(num<0)
  {
    num = -num;
    i = 1;     //������־	
  }	
  k = num * 10; //�˴�Ϊ��ʾһλС��*10ת��Ϊ����
  for(t = 0 ; t < len ; t++)
  {
    temp = (k / DISplay_pow(10,len-t-1)) % 10;
    if(enshow==0 && t<(len-1))
    {
      if(temp==0)
      {
        if(( (k / DISplay_pow(10,len-t-2)%10) != 0) && (i==1))//�ж��Ƿ�Ϊ�����������λǰһλ
        {
          DISplay_ShowChar(x+(size2/2)*t,y,'-',size2);
          i=0;	                                    //����жϺ�һλ�ı�־
        }
        else
          DISplay_ShowChar(x+(size2/2)*t,y,' ',size2);   //���û�����־���ʾ�ո�
        continue;
      }
      else 
        enshow = 1;		//�˴����ж��Ƿ�Ҫ��ʾ����	
    }
    if(t==len-1)
    {
      DISplay_ShowChar(x+(size2/2)*t,y,'.',size2);
      //�ж��Ƿ�Ϊ���һλ��ǰһλ����ʾһλС����
      t++;
    }
    DISplay_ShowChar(x+(size2/2)*t,y,temp+'0',size2); //һλһλ��ʾ��ȥ
  }
}

//��ʾһ���ַ��Ŵ�
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
//��ʾ����
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
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
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




//��ʼ��SSD1306					    
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
