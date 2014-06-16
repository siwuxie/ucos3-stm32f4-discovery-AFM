/*
 * General Atomic Force Microscope (AFM) - Head of Communication
 * =============================================================
 * Date: 11 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * =============================================================
 * 该头文件定义了AFM上下位机交互通信接口。
 */

#include "stm32f4xx.h"

#define COMM_USART			RCC_APB1Periph_USART2
#define COMM_USART_s		USART2
#define COMM_USART_PORT		RCC_AHB1Periph_GPIOA
#define COMM_USART_TX		GPIO_Pin_2
#define COMM_USART_RX		GPIO_Pin_3

#define COMM_RX_BUFF_SIZE		8
#define COMM_CMD_WORD_SIZE		4
#define COMM_EXCUTOR_LIST_SIZE 20

void comm_init();
//void comm_cmd_send(short data, int cmd_word, int cmd_head);


void comm_IC_send(unsigned char data);
void comm_IC_array_send(unsigned char* data, int len);
void comm_IC_init();

void USART2_IRQHandler(void);
