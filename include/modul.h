/*
 * General Atomic Force Microscope (AFM) - Modules Definitions
 * ===========================================================
 * Date: 09 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 该头文件定义了嵌入式系统自动载入各个模块的接口。该文件可以
 * 视为OS的一部分
 */

#include "includes.h"
#include "stm32f4xx.h""

#ifndef MODULE
#define MODULE

#include "test_module.h"
#include "comm_module.h"

#define MODULE_MAX_INTERFACES					20
#define MODULE_MAX_MODULES						10
#define MODULE_MAX_TASKS						1
#define MODULE_MSG_LEN							6

/*
 * Modules Description Structure
 * =============================
 */
struct module_stru
{
	unsigned short module_head;
	void (*dispatch)(unsigned short *msg);
	void (*taks_init[MODULE_MAX_TASKS])();
	int  count_tasks;
	unsigned char priors[MODULE_MAX_TASKS];
};
/*
 * Modules Description List
 * ========================
 */
struct module_stru modules_list[MODULE_MAX_MODULES];

/*
 * Interfaces Provides to OS
 * =========================
 */
//OS Call
void module_init();
void module_task_init();
unsigned char module_check();

//Modules Call
void module_msg_dispatch(unsigned short *msg);
void module_addtolist(struct module_stru temp);

#endif
