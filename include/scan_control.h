/*
 * scan_control.h
 *
 *  Created on: 2014年6月17日
 *      Author: baby
 */

#ifndef SCAN_CONTROL_H_
#define SCAN_CONTROL_H_

#include "stm32f4xx.h"

#define	SCAN_OUTPUT_PORT					GPIOA
#define SCAN_OUTPUT_PORT_RCC			RCC_AHB1Periph_GPIOA
#define	SCAN_OUTPUT_DAC_RCC			RCC_APB1Periph_DAC

#define SCAN_OUTPUT_PIN							GPIO_Pin_4 | GPIO_Pin_5
#define SCAN_OUTPUT_X								DAC_Channel_1
#define SCAN_OUTPUT_Y								DAC_Channel_2

typedef enum
{
	SCAN_X=SCAN_OUTPUT_X,
	SCAN_Y=SCAN_OUTPUT_Y
}SCAN_DIR;


#endif /* SCAN_CONTROL_H_ */
