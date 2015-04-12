#include "motor.h"

void motor_IC_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MOTOR_PORT, ENABLE);
	GPIO_InitStructure.GPIO_Pin = MOTOR_DIR | MOTOR_WAKEUP | MOTOR_SQUARE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTOR_PORT_s, &GPIO_InitStructure);
	motor_IC_sleep();
}

void motor_IC_sleep()
{
	GPIO_ResetBits(MOTOR_PORT_s, MOTOR_WAKEUP);
}

void motor_IC_wakeup()
{
	GPIO_SetBits(MOTOR_PORT_s, MOTOR_WAKEUP);
}

void motor_IC_one_step()
{
	static unsigned char i = 0;
	GPIO_SetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	for (long i=0;i<MOTOR_STEP_DELAY;i++);
	GPIO_ResetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	for (long i=0;i<MOTOR_STEP_DELAY;i++);
}

void motor_IC_dir(int dir)
{
	if (dir == MOTOR_DIR_FORWARD)
	{
		GPIO_SetBits(MOTOR_PORT_s, MOTOR_DIR);
	}
	else
	{
		GPIO_ResetBits(MOTOR_PORT_s, MOTOR_DIR);
	}
}
