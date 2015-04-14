/*
 * General Atomic Force Microscope (AFM) - Module of PID Closeloop
 * ===============================================================
 * Date: 11 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===============================================================
 * 璇ュご鏂囦欢瀹氫箟浜哖ID鎺у埗闂幆鐨勭浉鍏虫帴鍙�
 */

#include "module.h"
#include "pid_clooseloop.h"

#ifndef MODULE_PID
#define MODULE_PID

#define MOD_PID_HEAD														0x0003

#define MOD_PID_TASK_SET												0x00
#define MOD_PID_TASK_RUN											0x01
#define MOD_PID_TASK_REQUEST									0x02

#define MOD_PID_CMD_ENABLE										0x00
#define MOD_PID_CMD_SETP											0x01
#define MOD_PID_CMD_SETI												0x02
#define MOD_PID_CMD_SETD											0x03
#define MOD_PID_CMD_SETDELAY									0x04
#define MOD_PID_CMD_SETPOINT									0x05
#define MOD_PID_CMD_ASK_REPORT_ERR					0x06
#define MOD_PID_CMD_ASK_REPORT_Z						0x07
#define MOD_PID_CMD_MOTOR_STOP							0x08
#define MOD_PID_CMD_DISABLE									0x09

#define MOD_PID_REPORT_Z												0x00
#define MOD_PID_REPORT_ERR										0x01
#define MOD_PID_REPORT_INT										0x02

#define MOD_PID_REQUEST_Z											0x00
#define MOD_PID_REQUEST_ERR										0x01
#define MOD_PID_REQUEST_INT										0x03


OS_Q SETQ;
OS_Q REQUESTQ;

 OS_TCB Pid_Set_TCB, Pid_Run_TCB, Pid_Request_TCB;
 CPU_STK Pid_Set_Stk[64], Pid_Run_Stk[1024], Pid_Request_Stk[64];

void task_pid_set(void *p_arg);
void task_pid_run(void *p_arg);
void task_pid_request(void *p_arg);

void pid_module_init();
void pid_task_init();
void pid_dispatch(void *msg);
void pid_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg);

#endif



