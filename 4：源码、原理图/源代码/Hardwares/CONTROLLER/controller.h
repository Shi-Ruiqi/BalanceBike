#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "stdint.h"

/* 注意：以下两个参数均存在转向死角，调试参数前请仔细观看 "舵机微调视频注意事项（代码烧录前必看）"，否则自己调很容易损坏舵机 */
#define dirADJUST 0   /* 方向微调值：修改此参数可微调舵机方向，每次仅调节±1的量度，数据越大，越往左转；数据越小，越往右转*/
#define dirBASE 10    /* 方向基值：修改此参数可调整方向 */

/* 以电机轴朝向自己本人为基准(飞轮朝向自己为准) */
#define dirClockwise()      HAL_GPIO_WritePin(MAIN_DIR_GPIO_Port, MAIN_DIR_Pin, GPIO_PIN_SET)   //顺时针方向
#define dirAnticlockwise()  HAL_GPIO_WritePin(MAIN_DIR_GPIO_Port, MAIN_DIR_Pin, GPIO_PIN_RESET) //逆时针方向
#define mainMotorEN()       HAL_GPIO_WritePin(MAIN_EN_GPIO_Port, MAIN_EN_Pin, GPIO_PIN_SET)
#define mainMotorStop()     HAL_GPIO_WritePin(MAIN_EN_GPIO_Port, MAIN_EN_Pin, GPIO_PIN_RESET)

/* 
左右转向的尺度 
4:同步轮与同步带作为转向机构
1:舵盘作为转向机构
*/
#define DIR_SIZE 3    

typedef struct 
{
  int out;
}_OUT_Motor;


void _controller_perform(void) ;
void _controller_output(void)   ;
void CargoForward(void);
void CarBackward(void);
void EnableAuxMotor(void);
void DisableAuxMotor(void);
void TurnLeftOrRight(float inXrocker);
void goForwardOrBackward(float inYrcoker);
extern _OUT_Motor Motor1;

#endif
