#include "motor.h"
#include "cmd_def.h"

int motor_origin_point;
int motor_stop_flag;

void motor_init()
{
	motor_IC_init();
	motor_origin_point = 0;
}

void motor_auto_forward()
{
	while (motor_continue_check() == MOTOR_GOON)
	{
		motor_step_forward(MOTOR_SINGLE_STEP);
	}
	/*
	 * need informations
	 */
}

void motor_auto_backward()
{
	while (motor_continue_check() == MOTOR_GOON)
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
	motor_IC_wakeup();
	motor_IC_dir(MOTOR_DIR_FORWARD);
	while (temp>0)
	{
		motor_IC_one_step();
		temp--;
	}
	motor_IC_sleep();
}

void motor_step_backward(int step)
{
	int temp = step;
	motor_IC_wakeup();
	motor_IC_dir(MOTOR_DIR_BACKWARD);
	while (temp>0)
	{
		motor_IC_one_step();
		temp--;
	}
	motor_IC_sleep();
}

void motor_origin_set()
{
	motor_origin_point=0;
	/*
	 * need informations
	 */
}

void motor_originate()
{
	if (motor_origin_point>0)
	{
		motor_step_backward(motor_origin_point);
	}
	else if (motor_origin_point<0)
	{
		motor_step_forward(motor_origin_point);
	}
	/*
	 * need informations
	 */
}

void motor_stop()
{
	motor_stop_flag = MOTOR_STOP;
}

short motor_continue_check()
{
	if (motor_stop_flag == MOTOR_STOP)
		return MOTOR_STOP;
	return MOTOR_GOON;
}
