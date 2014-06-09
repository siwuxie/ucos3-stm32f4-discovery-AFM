#include "motor.h"
#include "cmd_def.h"

void motor_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MOTOR_PORT, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MOTOR_DIR | MOTOR_WAKEUP | MOTOR_SQUARE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(MOTOR_PORT_s, &GPIO_InitStructure);
}

void motor_sleep()
{
//	MOTOR_PORT_s->BSRRL=0x0020;
	GPIO_SetBits(MOTOR_PORT_s, MOTOR_WAKEUP);
}

void motor_IC_forward_one_step()
{
	static unsigned char i = 0;
//	MOTOR_PORT_s->BSRRH = 0x0020;
	GPIO_ResetBits(MOTOR_PORT_s, MOTOR_WAKEUP);
	if (i%2)
	{
//		MOTOR_PORT_s->BSRRL = 0x0080;
		GPIO_SetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	}
	else
	{
//		MOTOR_PORT_s->BSRRH = 0x0080;
		GPIO_ResetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	}
	i++;
}

void motor_IC_backward_one_step()
{
	static unsigned char i = 0;
//	MOTOR_PORT_s->BSRRH = 0x0028;
	GPIO_ResetBits(MOTOR_PORT_s, MOTOR_WAKEUP);
	GPIO_SetBits(MOTOR_PORT_s, MOTOR_DIR);
	if (i%2)
	{
//		MOTOR_PORT_s->BSRRH = 0x0080;
		GPIO_SetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	}
	else
	{
//		MOTOR_PORT_s->BSRRL = 0x0080;
		GPIO_ResetBits(MOTOR_PORT_s, MOTOR_SQUARE);
	}
	i++;
}

void motor_auto_forward()
{
	while (motor_auto_forward_check() == MOTOR_GOON)
	{
		motor_step_forward(MOTOR_SINGLE_STEP);
	}
	/*
	 * need informations
	 */
}

void motor_auto_backward()
{
	while (motor_auto_backward_check() == MOTOR_GOON)
	{
		motor_step_backward(MOTOR_SINGLE_STEP);
	}
	/*
	 * need informations
	 */
}

void motor_step_forward(int step)
{
	int temp = step;
	while (temp>0)
	{
		motor_IC_forward_one_step();
		temp--;
	}
}

void motor_step_backward(int step)
{
	int temp = step;
	while (temp>0)
	{
		motor_IC_forward_one_step();
		temp--;
	}
}
