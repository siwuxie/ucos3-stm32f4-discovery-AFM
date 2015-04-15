#include "scan_control_module.h"

void scan_module_init()
{
	scan_init();

	struct module_stru temp;
	temp.module_head = MOD_SCAN_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = scan_dispatch;
	temp.taks_init[0] = scan_task_init;
	module_addtolist(temp, MOD_SCAN_HEAD);
}

void scan_task_init()
{
	OS_ERR err;
	OSQCreate(&ScanQ, "ScanQ", 10, &err);

	OSTaskCreate(
				(OS_TCB	*)&Scan_TCB,
				(CPU_CHAR	*)"Scan Task",
				(OS_TASK_PTR)task_scan,
				(void	*)0,
				(OS_PRIO	)2,
				(CPU_STK	*)&Scan_Stk[0],
				(CPU_STK_SIZE)16,
				(CPU_STK_SIZE)128,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}

void scan_dispatch(void  *msg)
{
	OS_ERR err;
	CMD_STRU* temp = (CMD_STRU*)msg;

	switch (temp->cmd_word>>8)
	{
	case MOD_SCAN_TASK_SCAN:
		OSQPost(&MoveQ, msg, sizeof(unsigned short)*5, OS_OPT_POST_FIFO, &err);
		break;
	}
}

void task_scan(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	unsigned short *msg;
	unsigned short msg_send[6];
	unsigned short data[2];

	while (1)
	{
	}

}
