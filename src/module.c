#include "modul.h"

void (*init_function[10])()={test_module_init, comm_module_init, motor_module_init};
unsigned short head_list[10] = {MOD_TEST_HEAD, MOD_COMM_HEAD, MOD_MOTOR_HEAD};
int count_modules = 3;



void module_init()
{
	OS_ERR err;
	for (int i=0 ;i<count_modules; i++)
	{
		(*init_function[i])();
	}
	module_task_init();
	OSStart(&err);
}

void module_task_init()
{
	for (int i =0;i<count_modules;i++)
	{
		struct module_stru temp = modules_list[head_list[i]];
		for (int j = 0; j<temp.count_tasks; j++)
		{
			(*temp.taks_init[j])();
		}
	}
}

void module_msg_dispatch(unsigned short *msg)
{
	(*modules_list[*msg].dispatch)(msg);
}

unsigned char module_check()
{
	int check[MODULE_MAX_TASKS*MODULE_MAX_MODULES];
	for (int i = 0; i<MODULE_MAX_TASKS*MODULE_MAX_MODULES; i++)
	{
		check[i]=0;
	}
	for (int i = 0; i<MODULE_MAX_MODULES*MODULE_MAX_TASKS; i++)
	{
		if (check[i]==0)
		{
			check[i]=1;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

void module_addtolist(struct module_stru temp, unsigned short head)
{
	modules_list[head]=temp;
}
