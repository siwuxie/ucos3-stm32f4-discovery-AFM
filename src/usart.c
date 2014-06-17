//#include "usart.h"
//#include "cmd_def.h"
//
//CPU_INT08U rxbuff[RX_BUFF_SIZE];
//CPU_INT08U rxbuff_count;
//
//void
//DDS_USART2_configuration()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
//
//	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART2, &USART_InitStructure);
//	USART_Cmd(USART2, ENABLE);
//
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	NVIC_EnableIRQ(USART2_IRQn);
//
//	rxbuff_count = 0;
//
//#ifdef USART_SEND_TEST
//	while (1)
//	{
//		USART_SendData(USART2, 0xE1);
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
//	}
//#endif
//
//}
//
//void
//USART2_puts(char* s)
//{
//	while (*s)
//	{
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
//		USART_SendData(USART2, *s);
//		s++;
//	}
//}
//
//
//
//void
//USART2_IRQHandler(void)
//{
//	CPU_ERR err;
//
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		CPU_INT08U c = USART_ReceiveData(USART2);
//		rxbuff[rxbuff_count]=c;
//		rxbuff_count++;
//
//		if ((rxbuff_count%2)==0)
//		{
//			CPU_INT08U temp;
//			temp = rxbuff[rxbuff_count-1];
//			rxbuff[rxbuff_count-1]=rxbuff[rxbuff_count-2];
//			rxbuff[rxbuff_count-2]=temp;
//		}
//
//		if (rxbuff_count == 8)
//		{
//			OSQPost(
//					(OS_Q *)&cmd_q,
//					(void *)rxbuff,
//					(OS_MSG_SIZE)sizeof(rxbuff),
//					(OS_OPT)OS_OPT_POST_FIFO,
//					(OS_ERR *)&err
//					);
//			rxbuff_count = 0;
//			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//		}
//	}
//}
