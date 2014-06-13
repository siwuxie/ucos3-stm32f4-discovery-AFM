#include "comm_module.h"
#include "comm.h"

void
comm_module_init()
{
	comm_init();

	struct module_stru temp;
	temp.module_head = MOD_COMM_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = comm_dispatch;
	temp.taks_init[0] = comm_task_init;
	module_addtolist(temp);
}

void
comm_task_init()
{
	CPU_ERR err;
	OSQCreate(&SendDataQ, "SendDataQ", 30, &err);

	OSTaskCreate(
				(OS_TCB	*)&Comm_Send_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_comm_send,
				(void	*)0,
				(OS_PRIO	)1,
				(CPU_STK	*)&Comm_Send_Stk[0],
				(CPU_STK_SIZE)Comm_Send_Stk[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}

void
comm_dispatch(unsigned short *msg)
{
	OS_ERR err;
	comm_render(msg+2,*(msg+4),*(msg+5),0,msg);
	OSQPost(&SendDataQ,msg,sizeof(unsigned short)*4,OS_OPT_POST_FIFO,&err);
}

void
comm_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg)
{
	*msg = des_head;
	*(msg+1) = des_word;
	*(msg+2) = *data;
	*(msg+3) = *(data+1);
	*(msg+4) = MOD_COMM_HEAD;
	*(msg+5) = 0;
}

void
task_comm_send(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;
	unsigned short *msg;
	while (1)
	{
		msg = OSQPend(&SendDataQ,0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);
		comm_IC_array_send((unsigned char*)msg,8);
	}
}
