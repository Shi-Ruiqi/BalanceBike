#include "imu.h"
#include "imath.h"
#include "math.h"
#include "mpu6050.h"

_Matrix Mat = {0};
_Attitude att = {0};
#define MahonyPERIOD        5.0f            //��̬�������ڣ�ms��
#define kp 	    0.5f                        //proportional gain governs rate of convergence to accelerometer/magnetometer 
#define ki 	    0.0001f                     //integral gain governs rate of convergenceof gyroscope biases

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;       //quaternion elements representing theestimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;      //scaled integral error  


/*
 * ��������mahony_update
 * ����  ����̬����
 * ����  ���������������ݣ���λ������/�룩�����ٶ��������ݣ���λ��g��    
 * ����  ��     
 */
//Gyroscope units are radians/second, accelerometer  units are irrelevant as the vector is normalised.
void mahony_update(float gx, float gy, float gz, float ax, float ay, float az) 
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
 
  if(ax*ay*az==0)
      return;
    
    //[ax,ay,az]�ǻ�������ϵ�¼��ٶȼƲ�õ���������(��ֱ����)
	norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	//VectorA = MatrixC * VectorB
	//VectorA ���ο���������ת���ڻ����µ�ֵ
	//MatrixC ����������ϵת��������ϵ����ת����  
	//VectorB ���ο�����������0,0,1��      
    //[vx,vy,vz]�ǵ�������ϵ����������[0,0,1]����DCM��ת����(C(n->b))����õ��Ļ�������ϵ�е���������(��ֱ����)    

  vx = Mat.DCM_T[0][2];
  vy = Mat.DCM_T[1][2];
  vz = Mat.DCM_T[2][2];
    
  //��������ϵ��������˵õ�������������e���ǲ����õ���v����Ԥ��õ��� v^֮��������ת�������v������[ax,ay,az]��,v^����[vx,vy,vz]��
  //����������������DCM�������Ҿ���(����DCM�����е���Ԫ��)�������������þ��ǽ�bϵ��n��ȷ��ת��ֱ���غϡ�
  //ʵ����������������ֻ��bϵ��nϵ��XOYƽ���غ�����������z����ת��ƫ�������ٶȼ��޿��κΣ�
  //���ǣ����ڼ��ٶȼ��޷���֪z���ϵ���ת�˶������Ի���Ҫ�õشż�����һ��������
  //���������Ĳ���õ��Ľ��������������ģ������֮��н����ҵĳ˻�a��v=|a||v|sin��,
  //���ٶȼƲ����õ�������������Ԥ��õ��Ļ������������Ѿ�������λ����������ǵ�ģ��1��
  //Ҳ����˵���������Ĳ���������sin���йأ����ǶȺ�Сʱ�����������Խ����ڽǶȳ����ȡ�

  ex = ay*vz - az*vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;
 
  //������������л���
	exInt = exInt + ex*ki;
	eyInt = eyInt + ey*ki;
	ezInt = ezInt + ez*ki;

  //��̬���������ٶ��ϣ��������ٶȻ���Ư�ƣ�ͨ������Kp��Ki�������������Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶȡ�
	gx = gx + kp*ex + exInt;
	gy = gy + kp*ey + eyInt;
	gz = gz + kp*ez + ezInt;

  //һ�����������������Ԫ�� 
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)* MahonyPERIOD * 0.0005f;
	q1 = q1 + ( q0*gx + q2*gz - q3*gy)* MahonyPERIOD * 0.0005f;
	q2 = q2 + ( q0*gy - q1*gz + q3*gx)* MahonyPERIOD * 0.0005f;
	q3 = q3 + ( q0*gz + q1*gy - q2*gx)* MahonyPERIOD * 0.0005f; 

  //��������������Ԫ�����й�һ�������õ������徭����ת����µ���Ԫ����
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
    
  //��Ԫ��תŷ����
	att.pit =  atan2(2.0f*(q0*q1 + q2*q3),q0*q0 - q1*q1 - q2*q2 + q3*q3) * rad_to_angle;
	att.rol =  asin(2.0f*(q0*q2 - q1*q3)) * rad_to_angle;       
 
  //z����ٶȻ��ֵ�ƫ����
  att.yaw += Mpu.deg_s.z  * MahonyPERIOD * 0.001f;   
}
/*
 * ��������rotation_matrix
 * ����  ����ת���󣺻�������ϵ -> ��������ϵ
 * ����  �� 
 * ����  ��     
 */
void rotation_matrix(void)
{
  Mat.DCM[0][0] = 1.0f - 2.0f * q2*q2 - 2.0f * q3*q3;
  Mat.DCM[0][1] = 2.0f * (q1*q2 -q0*q3);
  Mat.DCM[0][2] = 2.0f * (q1*q3 +q0*q2);

  Mat.DCM[1][0] = 2.0f * (q1*q2 +q0*q3);
  Mat.DCM[1][1] = 1.0f - 2.0f * q1*q1 - 2.0f * q3*q3;
  Mat.DCM[1][2] = 2.0f * (q2*q3 -q0*q1);
   
  Mat.DCM[2][0] = 2.0f * (q1*q3 -q0*q2);
  Mat.DCM[2][1] = 2.0f * (q2*q3 +q0*q1);
  Mat.DCM[2][2] = 1.0f - 2.0f * q1*q1 - 2.0f * q2*q2;   
}
/*
 * ��������rotation_matrix_T
 * ����  ����ת�����ת�þ��󣺵�������ϵ -> ��������ϵ
 * ����  �� 
 * ����  ��     
 */
void rotation_matrix_T(void)
{
  Mat.DCM_T[0][0] = 1.0f - 2.0f * q2*q2 - 2.0f * q3*q3;
  Mat.DCM_T[0][1] = 2.0f * (q1*q2 +q0*q3);    
  Mat.DCM_T[0][2] = 2.0f * (q1*q3 -q0*q2); 
  
  Mat.DCM_T[1][0] = 2.0f * (q1*q2 -q0*q3);
  Mat.DCM_T[1][1] = 1.0f - 2.0f * q1*q1 - 2.0f * q3*q3;  
  Mat.DCM_T[1][2] = 2.0f * (q2*q3 +q0*q1);    
  
  Mat.DCM_T[2][0] = 2.0f * (q1*q3 +q0*q2);
  Mat.DCM_T[2][1] = 2.0f * (q2*q3 -q0*q1);
  Mat.DCM_T[2][2] = 1.0f - 2.0f * q1*q1 - 2.0f * q2*q2;   
}
/*
 * ��������Matrix_ready
 * ����  ���������׼����Ϊ��̬����ʹ��
 * ����  �� 
 * ����  ��     
 */
void Matrix_ready(void)
{
  rotation_matrix();                      //��ת�������
  rotation_matrix_T();                    //��ת�������������
}




