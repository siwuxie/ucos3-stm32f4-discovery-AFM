#include "motor_module.h"

void
motor_module_init()
{
	motor_init();

	struct module_stru temp;
	temp.module_head = MOD_MOTOR_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = motor_dispatch;
	temp.taks_init[0] = motor_task_init;
	module_addtolist(temp);
}

void
motor_task_init()
{
	OS_ERR err;
	OSQCreate(&MoveQ, "MoveQ", 10, &err);

	OSTaskCreate(
				(OS_TCB	*)&Motor_Move_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_motor_move,
				(void	*)0,
				(OS_PRIO	)1,
				(CPU_STK	*)&Motor_Move_Stk[0],
				(CPU_STK_SIZE)Motor_Move_Stk[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}

void
motor_dispatch(unsigned short *msg)
{
	OS_ERR err;
//	unsigned short temp_word = *(msg+1)&0x00FF;
	unsigned short temp_task = *(msg+1)>>8;
	switch (temp_task)
	{
	case MOD_MOTOR_TASK_MOVE:
		OSQPost(&MoveQ, msg+1, sizeof(unsigned short), OS_OPT_POST_FIFO, &err);
		break;
	}
}

void
motor_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg)
{
	*msg = des_head;
	*(msg+1) = des_word;
	*(msg+2) = *data;
	*(msg+3) = *(data+1);
	*(msg+4) = MOD_MOTOR_HEAD;
	*(msg+5) = ori_task_interface;
}

void
task_motor_move(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	unsigned short *msg;

	while (1)
	{
		msg = OSQPend(&MoveQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch (*msg)
		{
		case MOD_MOTOR_CMD_SET_ORIGIN:
			motor_origin_set();

			break;
		case MOD_MOTOR_CMD_STEP_FORWARD:
			motor_step_forward(*(msg+1));
			break;
		case MOD_MOTOR_CMD_STEP_BACKWARD:
			motor_step_backward(*(msg+1));
			break;
		case MOD_MOTOR_CMD_AUTO_FORWARD:
			motor_auto_forward();
			break;
		case MOD_MOTOR_CMD_AUTO_BACKWARD:
			motor_auto_backward();
			break;
		case MOD_MOTOR_CMD_STOP:
			motor_stop();
			break;
		case MOD_MOTOR_CMD_ORIGINATE:
			motor_originate();
			break;
		}
	}

}
