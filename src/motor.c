#include "motor.h"
#include "cmd_def.h"

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
	motor_wakeup();
	motor_IC_dir(MOTOR_DIR_FORWARD);
	while (temp>0)
	{
		motor_IC_one_step();
		temp--;
	}
	motor_sleep();
}

void motor_step_backward(int step)
{
	int temp = step;
	motor_wakeup();
	motor_IC_dir(MOTOR_DIR_BACKWARD);
	while (temp>0)
	{
		motor_IC_one_step();
		temp--;
	}
	motor_sleep();
}
