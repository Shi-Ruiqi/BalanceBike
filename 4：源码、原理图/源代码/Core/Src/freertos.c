/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "mpu6050.h"
#include "stdio.h"
#include "imath.h"
#include "imu.h"
#include "ano.h"
#include "flash.h"
#include "adc.h"
#include "tim.h"
#include "led.h"
#include "display.h"
#include "encoder.h"
#include "pid.h"
#include "bluetooth.h"
#include "controller.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId SensorSampleTaskHandle;
osThreadId ScatteredTaskHandle;
osThreadId DebugTransferTaskHandle;
osThreadId BluetoothTransferTaskHandle;
osThreadId ControllerTaskHandle;
osMessageQId MpuSensorQueueHandle;
osMessageQId EncoderQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartSensorSampleTask(void const * argument);
void StartScatteredTask(void const * argument);
void StartDebugTransferTask(void const * argument);
void StartBluetoothTransferTask(void const * argument);
void StartControllerTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of MpuSensorQueue */
  osMessageQDef(MpuSensorQueue, 1, uint32_t);
  MpuSensorQueueHandle = osMessageCreate(osMessageQ(MpuSensorQueue), NULL);

  /* definition and creation of EncoderQueue */
  osMessageQDef(EncoderQueue, 16, uint16_t);
  EncoderQueueHandle = osMessageCreate(osMessageQ(EncoderQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of SensorSampleTask */
  osThreadDef(SensorSampleTask, StartSensorSampleTask, osPriorityHigh, 0, 128);
  SensorSampleTaskHandle = osThreadCreate(osThread(SensorSampleTask), NULL);

  /* definition and creation of ScatteredTask */
  osThreadDef(ScatteredTask, StartScatteredTask, osPriorityBelowNormal, 0, 64);
  ScatteredTaskHandle = osThreadCreate(osThread(ScatteredTask), NULL);

  /* definition and creation of DebugTransferTask */
  osThreadDef(DebugTransferTask, StartDebugTransferTask, osPriorityBelowNormal, 0, 128);
  DebugTransferTaskHandle = osThreadCreate(osThread(DebugTransferTask), NULL);

  /* definition and creation of BluetoothTransferTask */
  osThreadDef(BluetoothTransferTask, StartBluetoothTransferTask, osPriorityNormal, 0, 128);
  BluetoothTransferTaskHandle = osThreadCreate(osThread(BluetoothTransferTask), NULL);

  /* definition and creation of ControllerTask */
  osThreadDef(ControllerTask, StartControllerTask, osPriorityAboveNormal, 0, 64);
  ControllerTaskHandle = osThreadCreate(osThread(ControllerTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  led_set(RUN_LED,100, 50,0); 

  /* Infinite loop */
  for(;;)
  {
    led_operation(&run_led);
    adcINFO.VbatOrg = read_ADC_voltagedata();                           //获取原始ADC值
    adcINFO.Voltage = (float)adcINFO.VbatOrg * 3.3f / 4096.0f * 11.0f ; //11.0为实际电路中电阻分压比1:10之和
   
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartSensorSampleTask */
/**
* @brief Function implementing the SensorSampleTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorSampleTask */
void StartSensorSampleTask(void const * argument)
{
  /* USER CODE BEGIN StartSensorSampleTask */

  /* Infinite loop */
  for(;;)
  {
    get_acc_raw();                                                //读取加速度原始数据
    get_gyro_raw();                                               //读取陀螺仪原始数据
    get_deg_s(&gyro_raw_f,&Mpu.deg_s);                            //陀螺仪原始数据转为度为单位的速率    
    get_rad_s(&gyro_raw_f,&Mpu.rad_s);                            //陀螺仪原始数据转为弧度为单位的速率
    get_acc_raw();                                                //加速度数据
    acc_iir_lpf(&acc_raw_f,&acc_att_lpf,Mpu.att_acc_factor);      //姿态解算时加速度低通滤波 
    get_acc_g(&acc_att_lpf,&Mpu.acc_g);                           //将加速度数据转为以g为单位的加速度数据
    mahony_update(Mpu.rad_s.x,Mpu.rad_s.y,Mpu.rad_s.z,Mpu.acc_g.x,Mpu.acc_g.y,Mpu.acc_g.z); 
    Matrix_ready();                                               //姿态解算相关矩阵更新

    readEncoderValue();                                           //飞轮电机编码器值读取及处理
    
    _controller_perform();                                        //三环串级PID控制器
    _controller_output();                                         //控制器输出配置
    
    gyro_cal(&gyro_raw_cal);                                      //陀螺仪零偏校准
    osDelay(5);
  }
  /* USER CODE END StartSensorSampleTask */
}

/* USER CODE BEGIN Header_StartScatteredTask */
/**
* @brief Function implementing the ScatteredTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartScatteredTask */
void StartScatteredTask(void const * argument)
{
  /* USER CODE BEGIN StartScatteredTask */

  DISplay_ShowString(0,0,"rol:",12);                              //字符显示
  DISplay_ShowString(0,1,"des:",12);                              //字符显示
  DISplay_ShowString(0,2,"dirBASE:",12);                          //字符显示
  DISplay_ShowString(0,3,"dirADJUST:",12);                        //字符显示
  DISplay_ShowString(0,4,"encoder:",12);                          //字符显示
  /* Infinite loop */
  for(;;)
  { 
    DISplay_ShowFolatNum(80,0,att.rol,5,12);                      //横滚角度rol显示
    DISplay_ShowFolatNum(80,1,Mpu.deg_s.y,5,12);                  //y轴角速度显示
    DISplay_ShowFolatNum(80,2,dirBASE,5,12);                      //舵机方向基值
    DISplay_ShowFolatNum(80,3,dirADJUST,5,12);                    //舵机方向微调值
    DISplay_ShowFolatNum(80,4,encoderINFO.mainNumberValue,5,12);  //编码速度值
    
    osDelay(40);
  }
  /* USER CODE END StartScatteredTask */
}

/* USER CODE BEGIN Header_StartDebugTransferTask */
/**
* @brief Function implementing the DebugTransferTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugTransferTask */
void StartDebugTransferTask(void const * argument)
{
  /* USER CODE BEGIN StartDebugTransferTask */
  /* Infinite loop */
  for(;;)
  {
    Usart_RecvData();                                               //接收上位机数据处理
    ANO_DMA_SEND_DATA();                                            //向上位机发送数据
    osDelay(10);
  }
  /* USER CODE END StartDebugTransferTask */
}

/* USER CODE BEGIN Header_StartBluetoothTransferTask */
/**
* @brief Function implementing the BluetoothTransf thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBluetoothTransferTask */
void StartBluetoothTransferTask(void const * argument)
{
  /* USER CODE BEGIN StartBluetoothTransferTask */
  /* Infinite loop */
  for(;;)
  {
    Bluetooth_RecvData();                                             //接收蓝牙数据处理
    UploadMsg_MpuData();                                              //向蓝牙app上传数据  
    osDelay(80);
  }
  /* USER CODE END StartBluetoothTransferTask */
}

/* USER CODE BEGIN Header_StartControllerTas
k */
/**
* @brief Function implementing the ControllerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControllerTask */
void StartControllerTask(void const * argument)
{
  /* USER CODE BEGIN StartControllerTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(20);
  }
  /* USER CODE END StartControllerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

