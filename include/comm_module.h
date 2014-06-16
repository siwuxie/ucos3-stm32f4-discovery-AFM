/*
 * General Atomic Force Microscope (AFM) - Module of Communication
 * ===============================================================
 * Date: 11 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===============================================================
 * 该头文件定义了AFM通信模块相关接口。
 */
#include "comm.h"
#include "modul.h"

#ifndef MODULE_COMM
#define MODULE_COMM
#define MOD_COMM_HEAD					0x0001

#define MOD_COMM_TASK_SEND				0x0000

#define MOD_COMM_CMD_SEND_INT			0x00
#define MOD_COMM_CMD_SEND_FP			0x01

OS_Q	SendDataQ;

static OS_TCB Comm_Send_TCB;
static CPU_STK Comm_Send_Stk[256];

/*
 * Send Task of Comm
 * =================
 */
void task_comm_send(void *p_arg);

void comm_module_init();
void comm_task_init();
void comm_dispatch(unsigned short *msg);
void comm_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg);

#endif



