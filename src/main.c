#include "stm32f4xx.h"
#include "includes.h"
#include "app_cfg.h"
#include <stdio.h>
#include "modul.h"

int
main(void)
{
	OS_ERR err;
	OSInit(&err);
	module_init();
}
