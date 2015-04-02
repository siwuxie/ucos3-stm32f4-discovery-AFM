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
	module_addtolist(temp, MOD_MOTOR_HEAD);
}

void
motor_task_init()
{
	OS_ERR err;
	OSQCreate(&MoveQ, "MoveQ", 10, &err);
	OSQCreate(&StopQ, "StopQ", 10, &err);

	OSTaskCreate(
				(OS_TCB	*)&Motor_Move_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_motor_move,
				(void	*)0,
				(OS_PRIO	)3,
				(CPU_STK	*)&Motor_Move_Stk[0],
				(CPU_STK_SIZE)Motor_Move_Stk[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

	OSTaskCreate(
				(OS_TCB	*)&Motor_Stop_TCB,
				(CPU_CHAR	*)"Led Blink11",
				(OS_TASK_PTR)task_motor_stop,
				(void	*)0,
				(OS_PRIO	)2,
				(CPU_STK	*)&Motor_Stop_Stk[0],
				(CPU_STK_SIZE)Motor_Stop_Stk[256 / 10],
				(CPU_STK_SIZE)64,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}



void
motor_dispatch(void *msg)
{
	OS_ERR err;

	switch ( (((CMD_STRU*)msg)->cmd_head) >> 8)
	{
	case MOD_MOTOR_TASK_MOVE:
		OSQPost(&MoveQ, msg, sizeof(unsigned short)*5, OS_OPT_POST_FIFO, &err);
		break;
	case MOD_MOTOR_TASK_STOP:
		OSQPost(&StopQ, msg, sizeof(unsigned short)*5, OS_OPT_POST_FIFO, &err);
	}
}

void
task_motor_stop(void *p_arg)
{
	OS_ERR err;

	OS_MSG_SIZE size;
	CPU_TS ts;

	CMD_STRU *msg;
	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&StopQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		motor_stop();
	}
}

void
task_motor_move(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	CMD_STRU *msg;
	CMD_STRU *send_msg = (CMD_STRU*)malloc(sizeof(CMD_STRU));
	unsigned short msg_send[6];
	unsigned short report_data[3];

	report_data[0] = 0;
	report_data[1] = 0;
	report_data[2] = 0;

	while (1)
	{
		msg = (CMD_STRU*)OSQPend(&MoveQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch (msg->cmd_word & 0x00ff)
		{
		case MOD_MOTOR_CMD_SET_ORIGIN:
			motor_origin_set();
			break;

		case MOD_MOTOR_CMD_STEP_FORWARD:

			report_data[0] = 1;
			report_data[1] = msg->para1;
			motor_report(msg_send, report_data);

			motor_step_forward(msg->para1);

			report_data[0] = 0;
			report_data[1] = 0;
			motor_report(msg_send, report_data);

			break;

		case MOD_MOTOR_CMD_STEP_BACKWARD:
			report_data[0] = 2;
			report_data[1] = msg->para1;
			motor_report(msg_send, report_data);

			motor_step_backward(msg->para1);

			report_data[0] = 0;
			report_data[1] = 0;
			motor_report(msg_send,  report_data);
			break;

		case MOD_MOTOR_CMD_AUTO_FORWARD:
			report_data[0] = 1;
			report_data[1] = 0xffff;
			motor_report(msg_send, report_data);
			while (motor_continue_check() == MOTOR_GOON)
			{
				motor_step_forward(MOTOR_SINGLE_STEP);
				if (motor_check_stop()==MOTOR_STOP)
				{
					motor_reset_stop();
					report_data[0] = 0;
					report_data[1] = 0;
					motor_report(msg_send, report_data);
					break;
				}
				for (int i=0;i<MOTOR_STEP_DELAY;i++);
			}
			break;

		case MOD_MOTOR_CMD_AUTO_BACKWARD:
			report_data[0] = 2;
			report_data[1] = 0xffff;
			motor_report(msg_send, report_data);
			while (motor_continue_check() == MOTOR_GOON)
			{
				motor_step_backward(MOTOR_SINGLE_STEP);
				if (motor_check_stop()==MOTOR_STOP)
				{
					report_data[0] = 0;
					report_data[1] = 0;
					motor_report(msg_send, report_data);
					motor_reset_stop();
					break;
				}
				for (int i=0;i<MOTOR_STEP_DELAY;i++);
			}
			break;

		case MOD_MOTOR_CMD_ORIGINATE:
			report_data[0] = 3;
			report_data[1] = motor_getorigin();
			motor_report(msg_send, report_data);
			motor_originate();
			break;
		}
	}
}

void
motor_report(void* msg, unsigned short *data)
{
	module_msg_render((MSG_STRU*)msg,
			 MOD_COMM_HEAD,
			 MOD_COMM_TASK_REPORT,
			 *data,
			 *(data+1),
			 *(data+2));
	module_msg_dispatch((CMD_STRU*)msg);
}
