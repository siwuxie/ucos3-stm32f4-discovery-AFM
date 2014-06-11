/*
 * General Atomic Force Microscope (AFM) - Head of Communication
 * =============================================================
 * Date: 11 June 2014
 * Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * =============================================================
 * 该头文件定义了AFM上下位机交互通信接口。
 */

#include "cmd_def.h"
#include "stm32f4xx.h"

#define COMM_USART			RCC_APB1Periph_USART2
#define COMM_USART_s		USART2
#define COMM_USART_PORT		RCC_AHB1Periph_GPIOA
#define COMM_USART_TX		GPIO_Pin_2
#define COMM_USART_RX		GPIO_Pin_3

#define COMM_RX_BUFF_SIZE		8

#define COMM_CMD_WORD_SIZE		4

#define COMM_EXCUTOR_LIST_SIZE 20

struct excutor_list
{
	void (*excu)();
	void (*sender)();
	int cmd_word;
	int cmd_head;
};
typedef struct excutor_list comm_excutor;

void comm_cmd_register(void (*func)(), void (*sender)(), int cmd_word, int cmd_head);
void comm_cmd_dispatch(short* cmd);

void comm_init();

void comm_sing_send(int data);
void comm_array_send(int* data, int len);
void comm_sing_send_f(double data);
void comm_array_send_f(double* data);


void comm_IC_send(unsigned char data);
void comm_IC_array_send(unsigned char* data);
void comm_IC_init();

//void comm_IC_recieve(short data);
void USART_IRQHandler(void);
