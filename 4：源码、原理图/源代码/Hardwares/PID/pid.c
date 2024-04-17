#include "pid.h"



_ALL_PID all;

/**
  * @brief   存储pid控制器参数 三行五列，每行代表一个pid控制器参数，每列对应参数 0.kp 1.ki 2.kd 3.积分限幅  4.pid输出限幅值
  * @param   x
  * @retval  x
  */
const float  controller_parameter[3][5] =
{
  /* 0.kp 1.ki 2.kd 3.积分限幅  4.pid输出限幅值 */
    {7.5 , 0.0,  0,  550 , 2000 },                           //rol_angle     内环角度环       
    {0.068 , 0.00005,  0.055,  500 , 2000 },                 //vel_encoder   外环速度环       
    {26.5 ,  0.0,  0,  500 , 2000 },                         //gyro          内环角速度环 
		
//    {7.5 , 0.0,  0,  550 , 2000 },                           //rol_angle     内环角度环       
//    {0.068 , 0.00005,  0.055,  500 , 2000 },                 //vel_encoder   外环速度环       
//    {26.5 ,  0.0,  0,  500 , 2000 },                         //gyro          内环角速度环 
};

/**
  * @brief   PID参数初始化配置
  * @param   *controller PID控制器指针，指向不同的控制器
  * @param   label PID参数标号，选择对应控制器的参数数组标号
  * @retval  x
  */
void pid_init(_PID *controller,uint8_t label)
{
    controller->kp              = controller_parameter[label][0];         
    controller->ki              = controller_parameter[label][1];         
    controller->kd              = controller_parameter[label][2];         
    controller->integral_max    = controller_parameter[label][3];         
    controller->out_max         = controller_parameter[label][4];               
}
//PID参数初始化
void all_pid_init(void)
{
    pid_init(&all.rol_angle,0);
    pid_init(&all.vel_encoder,1);
    pid_init(&all.rol_gyro,2);
} 

/**
  * @brief   PID控制器
  * @param   *controller PID控制器指针，指向不同的控制器
  * @retval  controller->out 经过控制后的输出值
  */
float pid_controller(_PID *controller)
{
    controller->err_last = controller->err;                                                  //保存上次偏差
    controller->err = controller->expect - controller->feedback;                            //偏差计算
    controller->integral += controller->ki * controller->err;                               //积分  
    //积分限幅
    if(controller->integral >  controller->integral_max)     controller->integral =  controller->integral_max;
    if(controller->integral < -controller->integral_max)     controller->integral = -controller->integral_max;
    //pid运算
    controller->out =  controller->kp*controller->err + controller->integral + controller->kd*(controller->err-controller->err_last);
   
    //输出限幅
    if(controller->out >  controller->out_max)   controller->out =  controller->out_max;
    if(controller->out < -controller->out_max)   controller->out = -controller->out_max;
    return controller->out;
}
/**
  * @brief   PID控制器积分项清除
  * @param   *controller PID控制器指针，指向不同的控制器
  * @retval  x
  */
void clear_integral(_PID *controller)
{
    controller->integral = 0.0f;
}











