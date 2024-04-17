#include "controller.h"
#include "imu.h"
#include "pid.h"
#include "encoder.h"
#include "mpu6050.h"
#include "imath.h"
#include "tim.h"
#include "bluetooth.h"
_OUT_Motor Motor1 = {0};
_OUT_Motor Motor2 = {0};



/**
  * @brief   外环速度控制器
  */
static void vel_controller(void)                                       
{                            
  all.vel_encoder.expect = 0.0f;                              //外环输出作为内环期望值
  all.vel_encoder.feedback = encoderINFO.mainNumberValue;    //编码器速度值作为反馈量   此处根据实际情况进行正负选择 +
  pid_controller(&all.vel_encoder);                           //内环pid控制器  
}
/**
  * @brief   内环角度控制器
  */
static void angle_controller(void)                                     
{
  all.rol_angle.expect = all.vel_encoder.out;                 //外环角度期望角度给0
  all.rol_angle.feedback = (-att.rol)+(0);                    //姿态解算的rol角作为反馈量
  pid_controller(&all.rol_angle);                             //外环pid控制器      
}
/**
  * @brief   内环角速度控制器
  */
static void gyro_controller(void)                                      
{                            
  all.rol_gyro.expect = all.rol_angle.out;                    //外环输出作为内环期望值
  all.rol_gyro.feedback = -Mpu.deg_s.y;                       //编码器速度值作为反馈量
  pid_controller(&all.rol_gyro);                              //内环pid控制器  
}
/**
  * @brief   三环串级PID控制器运行
  */
void _controller_perform(void)                                  
{   
  vel_controller();                                           //速度控制器
  angle_controller();                                         //角度控制器
  gyro_controller();                                          //角速度控制器    
}

/**
  * @brief   检测小车是否倒下
  * @param   inAngleData 输入的参考角度值
  * @retval  detectionMark 返回是否倒下标志 1：倒下 0：正常
  */
static uint8_t detectionFallDown(float inAngleData)
{
  uint8_t detectionMark = 0;

  if( f_abs(inAngleData) > 20.0f )                                             
  {
    detectionMark = 1;                                              //关闭输出标志置位                 
  }                                                            
  return detectionMark;     
}
/**
  * @brief   PWM输出
  * @param   pwm1 输入第一路pwm
  * @param   pwm2 输入第二路pwm
  * @retval  x
  */
static void pwmMotorOut(uint16_t pwm1 , uint16_t pwm2 )
{
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, u16RangeLimit(pwm1 , 0 , 2000));	          //控制占空比
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, u16RangeLimit(pwm2 , 0 , 2000));
}

/**
  * @brief   控制器输出
  */
static uint16_t N20_motor_speed; 
void _controller_output(void)                                    
{
  static uint8_t FalldownAndRestart = 0;

  /* 检测是否倒下 */
  if( detectionFallDown(att.rol)==1 )                 
  {
    N20_motor_speed = 2000;                                 //N20速度设为最小
    FalldownAndRestart = 1;                                 //倒下之后需复位重启将其清除才能正常进入到平衡状态
    DisableAuxMotor();                                      //失能关闭前后动力电机
    Motor1.out = 0;                                         //清除电机PWM输出值           
    Motor2.out = 0;                                         //清除电机PWM输出值      
    clear_integral(&all.vel_encoder);                       //清除积分        
    clear_integral(&all.rol_angle);                         //清除积分                
    clear_integral(&all.rol_gyro);                          //清除积分                
  }                                                                   
  else if( (acc_raw.z >= 2500 && acc_raw.z <= 5000) && f_abs(att.rol) <= 20.0f && FalldownAndRestart == 0)                                                               
  {
    N20_motor_speed = 500;
    EnableAuxMotor();                                       //使能前后动力电机
    Motor1.out =  all.rol_gyro.out;                         //正常输出    
  }
  /* 根据输出正负来判断方向 */
  if(Motor1.out>0)  dirAnticlockwise();   
  else  dirClockwise();
 
  TurnLeftOrRight(BluetoothParseMsg.Xrocker);               //左右转向控制
  goForwardOrBackward(BluetoothParseMsg.Yrocker);           //前后方向控制
  
  pwmMotorOut( int_abs(Motor1.out) , N20_motor_speed);                 //电机输出
} 

/**
  * @brief   舵机转向控制 舵机的控制一般需要一个20ms左右的时基脉冲，该脉冲的高电平部分一般为0.5ms-2.5ms范围内的角度控制脉冲部分，总间隔为2ms。
             以180度角度伺服为例，那么对应的控制关系是这样的：
    0.5ms------------0度； 
    1.0ms------------45度；
　　1.5ms------------90度；
　　2.0ms-----------135度；
　　2.5ms-----------180度；
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 5);  //0.5ms
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 10); //1.0ms 右转45度
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 15); //1.5ms 前方向
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 20); //2.0ms 左转45度
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 25); //2.5ms
  * @param   接收到的蓝牙遥控数据（左右摇杆数据）
  * @retval  x
  */
void TurnLeftOrRight(float inXrocker)
{
  static float LastXrocker = 0;
  if(inXrocker != LastXrocker)
  {
    if( inXrocker == 0  )
    {
      __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (dirBASE + DIR_SIZE ) + dirADJUST );// 左转一定角度    
    }
    else if( inXrocker == 5 )
    {
      __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (dirBASE - DIR_SIZE ) + dirADJUST );// 右转一定角度    
    }              
    else if( inXrocker == 2 )
    {
      __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dirBASE + dirADJUST );//1.5ms 前方向
    }    
    LastXrocker = inXrocker;
  }
}
/**
  * @brief   进行前后控制
  * @param   接收到的蓝牙遥控数据（前后摇杆数据）
  * @retval  x
  */
void goForwardOrBackward(float inYrcoker)
{
  if( inYrcoker >= 0 && inYrcoker <= 1 )                    
  {
    EnableAuxMotor();                                       //使能前后动力电机
    CarBackward();                                          //后退    
  }
  else if( inYrcoker >= 4 && inYrcoker <= 5 )
  {
    EnableAuxMotor();                                       //使能前后动力电机
    CargoForward();                                         //前进
  }
  else 
  {
    N20_motor_speed = 2000;                                 //N20电机速度设置为最低
    DisableAuxMotor();                                      //失能关闭前后动力电机
  }
}

/**
 * @brief  小车前进
 */
void CargoForward(void)
{ 
  HAL_GPIO_WritePin(AUX_AIN1_GPIO_Port, AUX_AIN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AUX_AIN2_GPIO_Port, AUX_AIN2_Pin, GPIO_PIN_SET);
}
/**
 * @brief  小车后退
 */
void CarBackward(void)
{
  HAL_GPIO_WritePin(AUX_AIN1_GPIO_Port, AUX_AIN1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(AUX_AIN2_GPIO_Port, AUX_AIN2_Pin, GPIO_PIN_RESET);
}
/**
 * @brief  使能辅助电机
 */
void EnableAuxMotor(void)
{
  HAL_GPIO_WritePin(AUX_STBY_GPIO_Port, AUX_STBY_Pin, GPIO_PIN_SET);
}
/**
 * @brief  失能辅助电机
 */
void DisableAuxMotor(void)
{
  HAL_GPIO_WritePin(AUX_STBY_GPIO_Port, AUX_STBY_Pin, GPIO_PIN_RESET);
}


