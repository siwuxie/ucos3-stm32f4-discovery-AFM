/*
 * General Atomic Force Microscope (AFM) - Header of Motor
 * ===========================================================
 * Date: 09 June 2014
 * Co-Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 该头文件定义了AFM中步进电机驱动接口
 */
#ifndef MOTOR
#define MOTOR

/*
 * 电机工作GPIO管脚定义
 * ====================
 * 该定义基于STM32F407 - Discovery 评估板，如不同则需要修改。
 */
#include "stm32f4xx.h"
#define MOTOR_PORT		RCC_AHB1Periph_GPIOD
#define MOTOR_PORT_s	GPIOD
#define MOTOR_DIR		GPIO_Pin_3
#define MOTOR_WAKEUP	GPIO_Pin_5
#define MOTOR_SQUARE	GPIO_Pin_7

void motor_init();

/*=============End 管脚定义================*/

/*
 * 电机工作底层驱动
 * ================
 * 以下接口用于底层驱动与A3967芯片交互。若芯片不同则需要修改具体实现。
 */

void motor_IC_forward_one_step();
void motor_IC_backward_one_step();
void motor_sleep();

/*==============End 底层驱动=================*/


void motor_auto_forward();
void motor_auto_backward();
void motor_stop();
void motor_step_forward(int step);
void motor_step_backward(int step);

void motor_originate();
void motor_origin_set();

/*
 * 标准单步长
 * ==========
 * 标准一个步长的进针量
 */
#define MOTOR_SINGLE_STEP		2

/*
 * 自动进针判断标准
 * ================
 * 返回:
 * 		STOP: 停止进针/退针
 * 		GOON: 继续进针/退针
 *
 * 	该函数需要在特定的地方定义，供该驱动调用
 *
 */
short motor_auto_forward_check();
short motor_auto_backward_check();
#define MOTOR_STOP 1
#define MOTOR_GOON 0


#endif
