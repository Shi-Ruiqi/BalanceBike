#include "ano.h"
#include "stdint.h"
#include "mpu6050.h"
#include "imu.h"
#include "usart.h"
#include "encoder.h"
#include "adc.h"
#include "controller.h"
dt_flag_t f;					                      //��Ҫ�������ݵı�־ 
uint8_t data_to_send[100];                  //�������ݻ���
uint8_t USART_RX_DATA[USART_RX_LEN];        //�������ݻ���

/* ������һ���ֽڵ����ݲ�ֳɶ���ֽڷ��� */
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

/* ��������λ��������̬�ǣ�����״̬ */
void ANO_DT_Send_Status(void)
{
	uint8_t _cnt=0;
	volatile int16_t _temp;
	volatile int32_t _temp2;
	uint8_t sum = 0;
	uint8_t i;
  
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;

	_temp = (int)(att.rol*100);                     //�����
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
    
	_temp = (int)(att.pit*100);                     //������    
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
    
	_temp = (int)(att.yaw*100);                     //ƫ����
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

	_temp2 = (int32_t)(100*99.99);       		        //�߶� 
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);

  data_to_send[_cnt++]=0x01;  					//����ģʽ    01����̬  02������  03������
  data_to_send[_cnt++]=0x01;                      //����״̬

	data_to_send[3] = _cnt-4;
	sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
    
  Usart1_DMA_Sent(data_to_send,_cnt);   //����
}
/* ��������λ�����ʹ�����ԭʼ���� */
 void ANO_DT_Send_Senser(int16_t a_x,int16_t a_y,int16_t a_z,int16_t g_x,int16_t g_y,int16_t g_z,int16_t m_x,int16_t m_y,int16_t m_z)
{
  uint8_t _cnt=0;
  volatile int16_t _temp;
  uint8_t sum = 0;
  uint8_t i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x02;
  data_to_send[_cnt++]=0;

  _temp = a_x;    
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);

  _temp = g_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);

  _temp = m_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);

  data_to_send[3] = _cnt-4;

  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++] = sum;
    
  Usart1_DMA_Sent(data_to_send,_cnt);   //����
}
/* ң����ͨ������ */
void ANO_DT_Send_RCData(uint16_t thr,uint16_t yaw,uint16_t rol,uint16_t pit,uint16_t aux1,uint16_t aux2,uint16_t aux3,uint16_t aux4,uint16_t aux5,uint16_t aux6)
{
  uint8_t _cnt=0;
  uint8_t i=0;
  uint8_t sum = 0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x03;
  data_to_send[_cnt++]=0;

  data_to_send[_cnt++]=BYTE1(thr);
  data_to_send[_cnt++]=BYTE0(thr);
  data_to_send[_cnt++]=BYTE1(yaw);
  data_to_send[_cnt++]=BYTE0(yaw);
  data_to_send[_cnt++]=BYTE1(rol);
  data_to_send[_cnt++]=BYTE0(rol);
  data_to_send[_cnt++]=BYTE1(pit);
  data_to_send[_cnt++]=BYTE0(pit);

  data_to_send[_cnt++]=BYTE1(aux1);
  data_to_send[_cnt++]=BYTE0(aux1);
  data_to_send[_cnt++]=BYTE1(aux2);
  data_to_send[_cnt++]=BYTE0(aux2);
  data_to_send[_cnt++]=BYTE1(aux3);
  data_to_send[_cnt++]=BYTE0(aux3);
  data_to_send[_cnt++]=BYTE1(aux4);
  data_to_send[_cnt++]=BYTE0(aux4);
  data_to_send[_cnt++]=BYTE1(aux5);
  data_to_send[_cnt++]=BYTE0(aux5);
  data_to_send[_cnt++]=BYTE1(aux6);
  data_to_send[_cnt++]=BYTE0(aux6);

  data_to_send[3] = _cnt-4;

  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;

  Usart1_DMA_Sent(data_to_send,_cnt);   //����
}
/* ��ѹ�������ݷ�����������λ�� */ 
void ANO_DT_Send_Power(float votage, float current)
{
	uint8_t _cnt=0;
	uint16_t temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = (uint16_t)100*votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = (uint16_t)100*current;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
  Usart1_DMA_Sent(data_to_send,_cnt);   //����
}
//�û��Զ������ݷ��ͣ�1-5��int16t��������     6-10��float��������
void ANO_DT_Send_User(int16_t user1,int16_t user2,int16_t user3,int16_t user4,int16_t user5,
                      float user6, float user7, float user8, float user9, float user10,
                      float user11,float user12,float user13,float user14,float user15)
{
  uint8_t _cnt=0;
  volatile int16_t _temp;
  float _temp_f;
  
  uint8_t sum = 0;
  uint8_t i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xF1;
  data_to_send[_cnt++]=0;
  
  //1-5  int16t��������
  _temp = user1;    
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = user2;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = user3;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = user4;    
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = user5;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);

  //6-10 ��float��������
  _temp_f = user6;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user7;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user8;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user9;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user10;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);

  _temp_f = user11;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user12;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user13;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user14;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);
  _temp_f = user15;
  data_to_send[_cnt++]=BYTE3(_temp_f);
  data_to_send[_cnt++]=BYTE2(_temp_f);
  data_to_send[_cnt++]=BYTE1(_temp_f);
  data_to_send[_cnt++]=BYTE0(_temp_f);

  data_to_send[3] = _cnt-4;

  sum = 0;
  for(i=0;i<_cnt;i++)
      sum += data_to_send[i];
  data_to_send[_cnt++] = sum;
  
  Usart1_DMA_Sent(data_to_send,_cnt);   //����
}
/*
����λ������λ�����в���ʱ����λ��������Ӧ�ķ�����Ϣ
 MSG_ID:                 MSG_DATA:
01:���ٶ�               01:У׼�ɹ�
02:������               E1:У׼ʧ��
03:����                 31:���óɹ�
30:���߶�λģ��         32:���óɹ�2
40:��������             A1:�ָ�Ĭ�ϳɹ�
 */
static void ANO_DT_Send_MSG(uint8_t MSG_ID, uint8_t MSG_DATA)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEE;
	data_to_send[3]=2;                      //����
	data_to_send[4]=MSG_ID;                 //������
	data_to_send[5]=MSG_DATA;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

  Usart1_DMA_Sent(data_to_send,7);   //����
}

/* �������� */
void ANO_DMA_SEND_DATA(void)
{
  static uint8_t ANO_debug_cnt = 0;
  ANO_debug_cnt++;
  if(ANO_debug_cnt==1)
  {
    ANO_DT_Send_Status();
  }
  else if(ANO_debug_cnt==2)
  { 
    ANO_DT_Send_Senser((int16_t)acc_raw.x,(int16_t)acc_raw.y,(int16_t)acc_raw.z, 
                       (int16_t)gyro_raw.x,(int16_t)gyro_raw.y,(int16_t)gyro_raw.z,
                       (int16_t)0,(int16_t)0,(int16_t)0);
  }
  else if(ANO_debug_cnt==3)
  {
    ANO_DT_Send_RCData(1100,1200,1300,1400,1500,1600,1700,1800,1900,1100);
  }
  else if(ANO_debug_cnt==4)
  {
    ANO_DT_Send_Power((float)adcINFO.VbatOrg,56.78);
  }
  else if(ANO_debug_cnt==5)
  {
    ANO_DT_Send_User((int16_t)encoderINFO.mainNumberValue,Motor1.out,0,0,0,
                    0,(float)encoderINFO.mainNumberValue,0,0,0,
                    0,0,0,0,0);
  }
  else if(ANO_debug_cnt==6)
  {
//    if(f.send_pid1)
//    {
//      f.send_pid1 = 0;
//      //����λ�������ڻ��ٶȻ�PID����ֵ
//      ANO_DT_Send_PID(1,  vel.kp*0.1f,
//                          vel.ki,
//                          vel.kd,
//                          1.5f,
//                          2.5f,
//                          1.5f,
//                          2.5f,
//                          1.5f,
//                          2.5f);        
//    }
//    if(f.send_pid2)
//    {
//      f.send_pid2 = 0;
//      //����λ�������⻷�ǶȻ�PID����ֵ
//      ANO_DT_Send_PID(2,  bal.kp*0.1f,
//                          bal.ki,
//                          bal.kd,
//                          2.6f,
//                          1.5f,
//                          2.4f,
//                          1.1f,
//                          2.2f,
//                          1.1f);   
//    }
    if(CalGyro.success==1)                          //����У׼�ɹ���Ϣ����λ��
    {
      CalGyro.success = 0;
      ANO_DT_Send_MSG(SENSOR_GYRO,CAL_SUCCESS);
    }
    else if(CalGyro.success==2)                     //����У׼ʧ����Ϣ����λ��
    {
      CalGyro.success = 0;
      ANO_DT_Send_MSG(SENSOR_GYRO,CAL_SUCCESS);
    }
  }
  else if(ANO_debug_cnt==7)
  {
    ANO_debug_cnt = 0;
  }
}


static void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;                  //����
	data_to_send[4]=head;               //������
	data_to_send[5]=check_sum;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

  Usart1_DMA_Sent(data_to_send,7);   //����
}

//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{  
	uint8_t sum = 0;
	for(uint8_t i=0;i<(num-1);i++)
  {
    sum += *(data_buf+i);        
  }
	if(!(sum==*(data_buf+num-1)))	                //�ж�sum	
    return;		
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))	//�ж�֡ͷ	
    return;		
	
	if(*(data_buf+2)==0X01)                         //������01
	{
//        pu8printf("buf:",data_buf,num,1);
		if(*(data_buf+4)==0X01)                     //ACCУ׼
    {
    }            
//			mpu6050.Acc_CALIBRATE = 1;
		if(*(data_buf+4)==0X02)                     //GYROУ׼
    {
      CalGyro.flag = 1;                       //���յ�У׼ָ�����Ϊ1    
//            _led.sta = 2;                           //Ĭ���޲���
    }
		if(*(data_buf+4)==0X03)
		{
//			mpu6050.Acc_CALIBRATE = 1;		
//			mpu6050.Gyro_CALIBRATE = 1;			
		}
	}
	if(*(data_buf+2)==0X02)                         //������02
	{
		if(*(data_buf+4)==0X01)                     //��λ����ȡpid����
		{
			f.send_pid1 = 1;
			f.send_pid2 = 1;
			f.send_pid3 = 1;
			f.send_pid4 = 1;
			f.send_pid5 = 1;
			f.send_pid6 = 1;
//            ANO_DT_Send_Check(*(data_buf+2),sum);
		}
		if(*(data_buf+4)==0X02)     //��ȡ����ģʽ��������
		{
		}
		if(*(data_buf+4)==0XA0)		//��ȡ�汾��Ϣ
		{
			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//�ָ�Ĭ�ϲ���
		{
//            PidToFactorySetup();    //������ֵ
      ANO_DT_Send_Check(*(data_buf+2),sum);
            /* �������ڻ�PID����д��flash */
//            FlashWriteNineFloat(PID_Group1_ADDRESS, WriteFlashVel.kp,WriteFlashVel.ki,WriteFlashVel.kd,
//                                                    WriteFlashBal.kp,WriteFlashBal.ki,WriteFlashBal.kd,
//                                                    WriteFlashTur.kp,WriteFlashTur.ki,WriteFlashTur.kd);
            /* �������⻷PID����д��flash */
//            FlashWriteNineFloat(PID_Group2_ADDRESS, all.rol_angle.kp,all.rol_angle.ki,all.rol_angle.kd,
//                                                    all.pit_angle.kp,all.pit_angle.ki,all.pit_angle.kd,
//                                                    all.yaw_angle.kp,all.yaw_angle.ki,all.yaw_angle.kd);
//            flash_flag.pid = 0;
//            FlashWriteByte(PID_FLAG_ADDRESS,flash_flag.pid);
		}
	}
	if(*(data_buf+2)==0X10)         //����PID��1
  {
        /* PID�ڻ��ٶȻ�������ֵ */
//        all.rol_gyro.kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        all.rol_gyro.ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        all.rol_gyro.kd = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        all.pit_gyro.kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        all.pit_gyro.ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        all.pit_gyro.kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        all.yaw_gyro.kp	= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        all.yaw_gyro.ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        all.yaw_gyro.kd = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    ANO_DT_Send_Check(*(data_buf+2),sum);
        /* ������д��flash�д洢 */
//        FlashWriteNineFloat(PID_Group1_ADDRESS, all.rol_gyro.kp,all.rol_gyro.ki,all.rol_gyro.kd,
//                                                all.pit_gyro.kp,all.pit_gyro.ki,all.pit_gyro.kd,
//                                                all.yaw_gyro.kp,all.yaw_gyro.ki,all.yaw_gyro.kd);
//        flash_flag.pid = 1;
//        /* ����־λд���ڴ� */
//        FlashWriteByte(PID_FLAG_ADDRESS,flash_flag.pid);
  }
  if(*(data_buf+2)==0X11)         //����PID��2
  {
        /* PID�⻷�ǶȻ�������ֵ */
//        all.rol_angle.kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//        all.rol_angle.ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//        all.rol_angle.kd = 0.001*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//        all.pit_angle.kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//        all.pit_angle.ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//        all.pit_angle.kd = 0.001*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//        all.yaw_angle.kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//        all.yaw_angle.ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//        all.yaw_angle.kd = 0.001*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
        ANO_DT_Send_Check(*(data_buf+2),sum);
        /* ������д��flash�д洢 */
//        FlashWriteNineFloat(PID_Group2_ADDRESS, all.rol_angle.kp,all.rol_angle.ki,all.rol_angle.kd,
//                                                all.pit_angle.kp,all.pit_angle.ki,all.pit_angle.kd,
//                                                all.yaw_angle.kp,all.yaw_angle.ki,all.yaw_angle.kd);
//        flash_flag.pid = 1;
//        /* ����־λд���ڴ� */
//        FlashWriteByte(PID_FLAG_ADDRESS,flash_flag.pid);
  }
  if(*(data_buf+2)==0X12)         //����PID��3
  {	
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
	if(*(data_buf+2)==0X13)         //����PID��4
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)         //����PID��5
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)         //����PID��6
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
}

//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
void ANO_DT_Data_Receive_Prepare(uint8_t data)
{
	static uint8_t RxBuffer[50];
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	
	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}

//void ANO_DT_Data_Receive_Prepare(uint8_t *data)
//{
//	static uint8_t RxBuffer[50];
//	static uint8_t _data_len = 0,_data_cnt = 0;
//	static uint8_t state = 0;
//	
//	if(state==0&&data[0]==0xAA)
//	{
//		state=1;
//		RxBuffer[0] = *(data+0);
//	}
//	else if(state==1&&data[1]==0xAF)
//	{
//		state=2;
//		RxBuffer[1]=*(data+1);
//	}
//	else if(state==2&&data[2]<0XF1)
//	{
//		state=3;
//		RxBuffer[2]=*(data+2);
//	}
//	else if(state==3&&data[3]<50)
//	{
//		state = 4;
//		RxBuffer[3]=*(data+3);
//		_data_len = *(data+3);
//		_data_cnt = 0;
//	}
//	else if(state==4&&_data_len>0)
//	{
//		_data_len--;
//		RxBuffer[4+_data_cnt++]=*(data+4);
//		if(_data_len==0)
//			state = 5;
//	}
//	else if(state==5)
//	{
//		state = 0;
//		RxBuffer[4+_data_cnt]=*(data+5);
//		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
//	}
//	else
//		state = 0;
//}


