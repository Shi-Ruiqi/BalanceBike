#include "pid.h"



_ALL_PID all;

/**
  * @brief   �洢pid���������� �������У�ÿ�д���һ��pid������������ÿ�ж�Ӧ���� 0.kp 1.ki 2.kd 3.�����޷�  4.pid����޷�ֵ
  * @param   x
  * @retval  x
  */
const float  controller_parameter[3][5] =
{
  /* 0.kp 1.ki 2.kd 3.�����޷�  4.pid����޷�ֵ */
    {7.5 , 0.0,  0,  550 , 2000 },                           //rol_angle     �ڻ��ǶȻ�       
    {0.068 , 0.00005,  0.055,  500 , 2000 },                 //vel_encoder   �⻷�ٶȻ�       
    {26.5 ,  0.0,  0,  500 , 2000 },                         //gyro          �ڻ����ٶȻ� 
		
//    {7.5 , 0.0,  0,  550 , 2000 },                           //rol_angle     �ڻ��ǶȻ�       
//    {0.068 , 0.00005,  0.055,  500 , 2000 },                 //vel_encoder   �⻷�ٶȻ�       
//    {26.5 ,  0.0,  0,  500 , 2000 },                         //gyro          �ڻ����ٶȻ� 
};

/**
  * @brief   PID������ʼ������
  * @param   *controller PID������ָ�룬ָ��ͬ�Ŀ�����
  * @param   label PID������ţ�ѡ���Ӧ�������Ĳ���������
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
//PID������ʼ��
void all_pid_init(void)
{
    pid_init(&all.rol_angle,0);
    pid_init(&all.vel_encoder,1);
    pid_init(&all.rol_gyro,2);
} 

/**
  * @brief   PID������
  * @param   *controller PID������ָ�룬ָ��ͬ�Ŀ�����
  * @retval  controller->out �������ƺ�����ֵ
  */
float pid_controller(_PID *controller)
{
    controller->err_last = controller->err;                                                  //�����ϴ�ƫ��
    controller->err = controller->expect - controller->feedback;                            //ƫ�����
    controller->integral += controller->ki * controller->err;                               //����  
    //�����޷�
    if(controller->integral >  controller->integral_max)     controller->integral =  controller->integral_max;
    if(controller->integral < -controller->integral_max)     controller->integral = -controller->integral_max;
    //pid����
    controller->out =  controller->kp*controller->err + controller->integral + controller->kd*(controller->err-controller->err_last);
   
    //����޷�
    if(controller->out >  controller->out_max)   controller->out =  controller->out_max;
    if(controller->out < -controller->out_max)   controller->out = -controller->out_max;
    return controller->out;
}
/**
  * @brief   PID���������������
  * @param   *controller PID������ָ�룬ָ��ͬ�Ŀ�����
  * @retval  x
  */
void clear_integral(_PID *controller)
{
    controller->integral = 0.0f;
}











