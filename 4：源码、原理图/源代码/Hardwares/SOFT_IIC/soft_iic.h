#ifndef _SOFT_IIC_h_
#define _SOFT_IIC_h_

//#define SCL_HIGH          MPU_SCL_GPIO_Port->BSRR = MPU_SCL_Pin
//#define SCL_LOW           MPU_SCL_GPIO_Port->BRR  = MPU_SCL_Pin

//#define SDA_HIGH          MPU_SDA_GPIO_Port->BSRR = MPU_SDA_Pin 
//#define SDA_LOW           MPU_SDA_GPIO_Port->BRR  = MPU_SDA_Pin

#define SCL_HIGH  HAL_GPIO_WritePin(MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_SET)
#define SCL_LOW   HAL_GPIO_WritePin(MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_RESET)

#define SDA_HIGH  HAL_GPIO_WritePin(MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_SET)
#define SDA_LOW   HAL_GPIO_WritePin(MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_RESET)

//#define SDA_READ        ((MPU_SDA_GPIO_Port->IDR &  MPU_SDA_Pin)!=0) ? 1 : 0
#define SDA_READ          HAL_GPIO_ReadPin(MPU_SDA_GPIO_Port,MPU_SDA_Pin)

void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Slave_Ack(void);
void IIC_Send_Byte(unsigned char byte);
unsigned char IIC_Read_Byte(void);
void IIC_Write_One_Byte(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char IIC_Read_One_Byte(unsigned char SlaveAddress,unsigned char REG_Address);
unsigned short int IIC_Read_Two_Bytes(unsigned char SlaveAddress,unsigned char REG_Address);

void soft_iic_init(void);

#endif

