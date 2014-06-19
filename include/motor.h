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
#define MOTOR_PORT		RCC_AHB1Periph_GPIOE
#define MOTOR_PORT_s	GPIOE
#define MOTOR_DIR		GPIO_Pin_11
#define MOTOR_WAKEUP	GPIO_Pin_13
#define MOTOR_SQUARE	GPIO_Pin_15

void motor_init();

/*=============End 管脚定义================*/

/*
 * 电机工作底层驱动
 * ================
 * 以下接口用于底层驱动与A3967芯片交互。若芯片不同则需要修改具体实现。
 */

#define MOTOR_DIR_FORWARD		1
#define MOTOR_DIR_BACKWARD		2
#define MOTOR_STEP_DELAY		4000		//该延迟很重要，如果延迟不够，电机不会转动。

void motor_IC_one_step();
void motor_IC_dir(int dir);
void motor_IC_sleep();
void motor_IC_wakeup();
void motor_IC_init();

/*==============End 底层驱动=================*/

void motor_init();
void motor_auto_forward();
void motor_auto_backward();
void motor_stop();
unsigned char motor_check_stop();
void motor_step_forward(int step);
void motor_step_backward(int step);

void motor_originate();
void motor_origin_set();
int motor_getorigin();

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
 */
short motor_continue_check();
#define MOTOR_STOP 1
#define MOTOR_GOON 0

#endif
