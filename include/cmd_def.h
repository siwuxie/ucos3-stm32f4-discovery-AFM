/*
 * General Atomic Force Microscope (AFM) - Command Definitions
 * ===========================================================
 * Date: 09 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 该头文件定义了AFM上下位机交互命令。
 */

/*
 * Command Head
 * ============
 */

#define PC_PROCESS_CMD 		0x0001
#define PC_SET_CMD			0x0000
#define AFM_ERR_MSG 		0x0003
#define AFM_DATA_MSG		0x0006
#define AFM_SET_ENABLE		0xFFFF

/*=====End of Command Head=====*/


/*
 * Motor Command Definitions
 * =========================
 */

#define MOTOR_CMD_ORIGIN_SET		0x0000
#define MOTOR_CMD_AUTO_FOR			0x0001
#define MOTOR_CMD_AUTO_BACK			0x0002
#define MOTOR_CMD_STOP				0x0003
#define MOTOR_CMD_STEP_FOR			0x0004
#define MOTOR_CMD_STEP_BACK			0x0005
#define MOTOR_CMD_ADAPT_ORI			0x0006

/*=========End of Motor=====*/
