#include "dds_module.h"

void
dds_module_init()
{
	AD9850_InitHard();

	struct module_stru temp;
	temp.module_head = MOD_DDS_HEAD;
	temp.count_tasks = 1;
	temp.dispatch = dds_dispatch;
	temp.taks_init[0] = dds_task_init;
	module_addtolist(temp, MOD_DDS_HEAD);
}


void
dds_task_init()
{
	OS_ERR err;
	OSQCreate(&FREQ, "Freq", 10, &err);

	OSTaskCreate(
				(OS_TCB	*)&DDS_Frequency_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_dds_frequency,
				(void	*)0,
				(OS_PRIO	)5,
				(CPU_STK	*)&DDS_Frequency_STK[0],
				(CPU_STK_SIZE)DDS_Frequency_STK[128 / 10],
				(CPU_STK_SIZE)128,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);

}

void
dds_dispatch(unsigned short *msg)
{
	OS_ERR err;
	unsigned short temp_task = *(msg+1)>>8;
	switch (temp_task)
	{
	case MOD_DDS_TASK_FREQUENCE:
		OSQPost(&FREQ, msg+1, sizeof(unsigned short)*3, OS_OPT_POST_FIFO, &err);
		break;
	}
}

void
dds_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg)
{
	*msg = des_head;
	*(msg+1) = des_word;
	*(msg+2) = *data;
	*(msg+3) = *(data+1);
	*(msg+4) = MOD_DDS_HEAD;
	*(msg+5) = ori_task_interface;
}

void
task_dds_frequency(void *p_arg)
{
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;

	unsigned short *msg;
	double frequency;

	while (1)
	{
		msg = OSQPend(&FREQ, 0, OS_OPT_PEND_BLOCKING, &size, &ts, &err);
		switch(*msg & 0xFF)
		{
		case MOD_DDS_CMD_SET_FREQUENCY:
			frequency = (double)(*(msg+1)+*(msg+2));
			AD9850_WriteCmd(0, frequency);
		}
	}
}
