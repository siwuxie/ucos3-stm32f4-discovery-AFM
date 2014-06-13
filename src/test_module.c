#include "test_module.h"
#include "stm32f4xx.h"

void task_led_blink(void *p_arg)
{
	unsigned short *cmd;
	OS_MSG_SIZE size;

	CPU_TS ts;

	RCC_AHB1PeriphClockCmd(BLINK_RCC_BIT, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	  /* Configure pin in output push/pull mode */
	GPIO_InitStructure.GPIO_Pin = (1 << BLINK_PIN);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BLINK_PORT, &GPIO_InitStructure);
	unsigned short temp;
	temp=MOD_TEST_CMD_BLINK;
	OS_ERR err;
	OSQPost(&BlinkQ, &temp, sizeof(unsigned short), OS_OPT_POST_FIFO, &err);
	while (1)
	{
		OS_ERR err;
		unsigned short cmd_new[6];
		unsigned short data[2];
		data[0]=0x1001;
		data[1]=0x0110;
		cmd = (unsigned short*)OSQPend(&BlinkQ,0,OS_OPT_PEND_BLOCKING,&size,&ts,&err);
		if (*cmd == MOD_TEST_CMD_BLINK)
		{
			GPIO_ResetBits(BLINK_PORT, (1 << BLINK_PIN));
			for (long i=0;i<50000;i++);
			GPIO_SetBits(BLINK_PORT, (1 << BLINK_PIN));
			for (long i=0;i<50000;i++);
			test_render(data,MOD_TEST_HEAD,(MOD_TEST_TASK_BLINK<<8) + MOD_TEST_CMD_SET_BLINK, (MOD_TEST_TASK_BLINK<<8) + MOD_TEST_CMD_BLINK,cmd_new);
			module_msg_dispatch(cmd_new);
		}
		else if (*cmd == MOD_TEST_CMD_SET_BLINK)
		{
			test_render(data,MOD_TEST_HEAD,(MOD_TEST_TASK_BLINK<<8) + MOD_TEST_CMD_BLINK, (MOD_TEST_TASK_BLINK<<8) + MOD_TEST_CMD_SET_BLINK,cmd_new);
			module_msg_dispatch(cmd_new);
			test_render(data,
						MOD_COMM_HEAD,
						(MOD_COMM_TASK_SEND<<8) + MOD_COMM_CMD_SEND_INT,
						(MOD_TEST_TASK_BLINK<<8) + MOD_TEST_CMD_SET_BLINK,
						cmd_new);
			module_msg_dispatch(cmd_new);
		}
	}
}

void test_module_init()
{
	struct module_stru temp;
	temp.module_head=MOD_TEST_HEAD;
	temp.count_tasks=1;
	temp.dispatch=test_dispatch;
	temp.taks_init[0]=test_task_init;
	module_addtolist(temp);
}
void test_task_init()
{
	CPU_ERR err;
	OSQCreate(&BlinkQ,"BlinkQ",10,&err);

	OSTaskCreate(
				(OS_TCB	*)&Test_led_blink_TCB,
				(CPU_CHAR	*)"Led Blink",
				(OS_TASK_PTR)task_led_blink,
				(void	*)0,
				(OS_PRIO	)2,
				(CPU_STK	*)&Test_led_blink_STK[0],
				(CPU_STK_SIZE)Test_led_blink_STK[256 / 10],
				(CPU_STK_SIZE)256,
				(OS_MSG_QTY	)0,
				(OS_TICK	)0,
				(void	*)0,
				(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
				(OS_ERR *)&err
				);
}

void test_dispatch(unsigned short *msg)
{
	unsigned short cmd_word = *(msg+1)&0x00FF;
	unsigned short cmd_head = *(msg+1)>>8;
	OS_ERR err;

	switch (*(msg+1)>>8)
	{
	case MOD_TEST_TASK_BLINK:
	{
		switch (*(msg+1)&0x00FF)
		{
		case MOD_TEST_CMD_BLINK:
			OSQPost(&BlinkQ, &cmd_word, sizeof(unsigned short), OS_OPT_POST_FIFO, &err);
			break;
		case MOD_TEST_CMD_SET_BLINK:
			OSQPost(&BlinkQ, &cmd_word, sizeof(unsigned short), OS_OPT_POST_FIFO, &err);
			break;
		}
		break;
	}
	}
}

void test_render(unsigned short *data, unsigned short des_head, unsigned short des_word,
				 unsigned short ori_task_interface, unsigned short *msg)
{
	*msg = des_head;
	*(msg+1) = des_word;
	*(msg+2) = *data;
	*(msg+3) = *(data+1);
	*(msg+4) = MOD_TEST_HEAD;
	*(msg+5) = ori_task_interface;
}
