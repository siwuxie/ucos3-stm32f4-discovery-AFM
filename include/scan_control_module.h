/*
 * scan_module.h
 *
 *  Created on: 2014年6月17日
 *      Author: baby
 */

#include "modul.h"
#include "scan_control.h"

#ifndef SCAN_MODULE
#define SCAN_MODULE


#define MOD_SCAN_HEAD						0x0010

#define MOD_SCAN_TASK_MOVE					0x00

//#define MOD_MOTOR_CMD_SET_ORIGIN			0x00
//#define MOD_MOTOR_CMD_AUTO_FORWARD			0x01
//#define MOD_MOTOR_CMD_AUTO_BACKWARD			0x02
//#define MOD_MOTOR_CMD_STOP					0x03
//#define MOD_MOTOR_CMD_STEP_FORWARD			0x04
//#define MOD_MOTOR_CMD_STEP_BACKWARD			0x05
//#define MOD_MOTOR_CMD_ORIGINATE				0x06

/*
 * Send From AFM
 * =============
 */
//#define MOD_MOTOR_REPORT_STEPS				0x00
//#define MOD_MOTOR_REPORT_STOP				0x01
//#define MOD_MOTOR_REPORT_ORIGINATE			0x02
//#define MOD_MOTOR_REPORT_MOVEMENT			0x03

OS_Q ScanQ;

static OS_TCB Scan_TCB;
static CPU_STK Scan_Stk[256];

/*
 * Move Task of Motor
 * ==================
 */
void task_motor_move(void *p_arg);

void motor_module_init();
void motor_task_init();
void motor_dispatch(unsigned short *msg);
void motor_render(unsigned short *data, unsigned short des_head, unsigned short des_word, unsigned short ori_task_interface, unsigned short *msg);


#endif /* SCAN_MODULE_H_ */
