/*
 * General Atomic Force Microscope (AFM) - Modules Definitions
 * ===========================================================
 * Date: 09 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 璇ュご鏂囦欢瀹氫箟浜嗗祵鍏ュ紡绯荤粺鑷姩杞藉叆鍚勪釜妯″潡鐨勬帴鍙ｃ�璇ユ枃浠跺彲浠�
 * 瑙嗕负OS鐨勪竴閮ㄥ垎
 */

#include "includes.h"
#include "stm32f4xx.h"

#ifndef MODULE
#define MODULE

#include "test_module.h"
#include "comm_module.h"
#include "motor_module.h"
#include "pid_closeloop_modul.h"
#include "dds_module.h"

#define MODULE_MAX_INTERFACES					20
#define MODULE_MAX_MODULES						0xFF
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
 * Module commands/data Description Structure
 * ==========================================
 */

typedef struct msg_stru
{
	unsigned short para1;
	unsigned short para2;
	unsigned short para3;
	unsigned short para4;
	unsigned short para5;
} MSG_STRU;

typedef struct cmd_stru
{
	unsigned short cmd_head;
	unsigned short cmd_word;
	unsigned short para1;
	unsigned short para2;
	unsigned short para3;
} CMD_STRU;

typedef struct data_stru
{
	unsigned short data_head;
	unsigned short data1;
	unsigned short data2;
	unsigned short data3;
	unsigned short data4;
} DATA_STRU;


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
//unsigned char module_check();

//Modules Call
void module_msg_dispatch(CMD_STRU *msg);
void module_addtolist(struct module_stru temp, unsigned short head);
void module_msg_render(MSG_STRU *msg,
		unsigned short para1,
		unsigned short para2,
		unsigned short para3,
		unsigned short para4,
		unsigned short para5);

#endif
