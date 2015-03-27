/*
 * General Atomic Force Microscope (AFM) - Header of Motor
 * ===========================================================
 * Date: 09 June 2014
 * Co-Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 璇ュご鏂囦欢瀹氫箟浜咥FM涓杩涚數鏈洪┍鍔ㄦ帴鍙�
 */
#ifndef MOTOR
#define MOTOR

/*
 * 鐢垫満宸ヤ綔GPIO绠¤剼瀹氫箟
 * ====================
 * 璇ュ畾涔夊熀浜嶴TM32F407 - Discovery 璇勪及鏉匡紝濡備笉鍚屽垯闇�淇敼銆�
 */
#include "stm32f4xx.h"
#define MOTOR_PORT		RCC_AHB1Periph_GPIOE
#define MOTOR_PORT_s	GPIOE
#define MOTOR_DIR		GPIO_Pin_11
#define MOTOR_WAKEUP	GPIO_Pin_13
#define MOTOR_SQUARE	GPIO_Pin_15

void motor_init();

/*=============End 绠¤剼瀹氫箟================*/

/*
 * 鐢垫満宸ヤ綔搴曞眰椹卞姩
 * ================
 * 浠ヤ笅鎺ュ彛鐢ㄤ簬搴曞眰椹卞姩涓嶢3967鑺墖浜や簰銆傝嫢鑺墖涓嶅悓鍒欓渶瑕佷慨鏀瑰叿浣撳疄鐜般�
 */

#define MOTOR_DIR_FORWARD		1
#define MOTOR_DIR_BACKWARD		2
#define MOTOR_STEP_DELAY		20000		//璇ュ欢杩熷緢閲嶈锛屽鏋滃欢杩熶笉澶燂紝鐢垫満涓嶄細杞姩銆�

void motor_IC_one_step();
void motor_IC_dir(int dir);
void motor_IC_sleep();
void motor_IC_wakeup();
void motor_IC_init();

/*==============End 搴曞眰椹卞姩=================*/

void motor_init();
void motor_auto_forward();
void motor_auto_backward();
void motor_stop();
unsigned char motor_check_stop();
void motor_step_forward(int step);
void motor_step_backward(int step);

void motor_originate();
void motor_origin_set();
int motor_getorigin();

/*
 * 鏍囧噯鍗曟闀�
 * ==========
 * 鏍囧噯涓�釜姝ラ暱鐨勮繘閽堥噺
 */
#define MOTOR_SINGLE_STEP		2

/*
 * 鑷姩杩涢拡鍒ゆ柇鏍囧噯
 * ================
 * 杩斿洖:
 * 		STOP: 鍋滄杩涢拡/閫�拡
 * 		GOON: 缁х画杩涢拡/閫�拡
 */
short motor_continue_check();
#define MOTOR_STOP 1
#define MOTOR_GOON 0

#endif
