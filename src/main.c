#include "stm32f4xx.h"
#include "includes.h"
#include "motor.h"
#include "app_cfg.h"
#include "modul.h"


int
main(void)
{
	OS_ERR err;
	OSInit(&err);
	module_init();
	comm_board_IC_array_send("hello",5);
	OSStart(&err);
}
