#include "stm32f4xx.h"
#include "includes.h"
#include "app_cfg.h"
#include <stdio.h>

#include "bsp_ad9850.h"
#include "cmd_def.h"
#include "usart.h"
#include <stdlib.h>

OS_TCB   DDSTaskStartTCB;
OS_TCB   DDSSweepTCB;
OS_TCB   DDSSendData;
OS_TCB   DDSReciveCmd;

static  CPU_STK  DDSTask_TasksStarts_Stk[APP_CFG_TASK_START_STK_SIZE];
static  CPU_STK  DDSTask_DDSSweep_Stk[APP_CFG_TASK_DDS_SWEEP_STK_SIZE];
static  CPU_STK  DDSTask_SendData_Stk[APP_CFG_TASK_SEND_DATA_STK_SIZE];
static  CPU_STK  DDSTask_ReciveCmd_Stk[APP_CFG_TASK_RECIVE_CMD_STk_SIZE];

/*--------------------------------------Task Difinition--------------------------------------*/
static void DDS_TaskStarts(void * p_arg);
static void DDS_Sweep(void *p_arg);
static void DDS_SendData(void *p_arg);
static void DDS_Recivecmd(void *p_arg);

/*----------------------------Configuration Function-----------------------------*/

#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)
CPU_INT32U ADC_converted_value;
static void DDS_ADC_configuration(void);
static void DDS_Sweep_DeInit(void);

static void delay(CPU_INT32U count);
static void checkCmd(CPU_INT16U *cmd);

/*-----------------------------Frequency sweep configs-----------------------------*/
CPU_FP64 StartFre;
CPU_FP64 EndFre;
CPU_INT32U NumberOfPoints;

/*
 * MSG QUEUE ADN Semaphores
 */
OS_Q  cmd_q;
OS_Q  SendData_q;

int
main(void)
{
	OS_ERR err;
	OSInit(&err);

	OSQCreate(
			(OS_Q *)&cmd_q,
			(CPU_CHAR *)"Cmd_queue",
			(OS_MSG_QTY)10,
			(OS_ERR *)&err
			);
	OSQCreate(
			(OS_Q *)&SendData_q,
			(CPU_CHAR *)"Send Data queue",
			(OS_MSG_QTY)10,
			(OS_ERR *)&err
			);

	OSTaskCreate(
			(OS_TCB	*)&DDSTaskStartTCB,
			(CPU_CHAR	*)"TaskStarts",
			(OS_TASK_PTR)DDS_TaskStarts,
			(void	*)0,
			(OS_PRIO	)1,
			(CPU_STK	*)&DDSTask_TasksStarts_Stk[0],
			(CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE / 2,
			(CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,
			(OS_MSG_QTY	)0,
			(OS_TICK	)0,
			(void	*)0,
			(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
			(OS_ERR *)&err
			);

  AD9850_InitHard();
  DDS_USART2_configuration();
  USART2_puts("DDS_Sweep_program!\n");
//  AD9850_WriteCmd(0,40000000);
  OSStart(&err);
}

void
DDS_TaskStarts(void *p_arg)
{
	OS_ERR err;
	/*------------------Frequency Sweep Task---------------------*/
#ifdef DEBUG_FREQUENCY_TASK
	OSTaskCreate(
			(OS_TCB	*)&DDSSweepTCB,
			(CPU_CHAR	*)"Sweep",
			(OS_TASK_PTR)DDS_Sweep,
			(void	*)0,
			(OS_PRIO	)5,
			(CPU_STK	*)&DDSTask_DDSSweep_Stk[0],
			(CPU_STK_SIZE)DDSTask_DDSSweep_Stk[APP_CFG_TASK_DDS_SWEEP_STK_SIZE / 10],
			(CPU_STK_SIZE)APP_CFG_TASK_DDS_SWEEP_STK_SIZE,
			(OS_MSG_QTY	)0,
			(OS_TICK	)0,
			(void	*)0,
			(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
			(OS_ERR *)&err
			);
#endif

	/*-------------------------Send Data-----------------------------*/
#ifdef DEBUG_SENDDATA
	OSTaskCreate(
			(OS_TCB	*)&DDSSendData,
			(CPU_CHAR	*)"SendData",
			(OS_TASK_PTR)DDS_SendData,
			(void	*)0,
			(OS_PRIO	)2,
			(CPU_STK	*)&DDSTask_SendData_Stk[0],
			(CPU_STK_SIZE)DDSTask_SendData_Stk[APP_CFG_TASK_SEND_DATA_STK_SIZE / 10],
			(CPU_STK_SIZE)APP_CFG_TASK_SEND_DATA_STK_SIZE,
			(OS_MSG_QTY	)0,
			(OS_TICK	)0,
			(void	*)0,
			(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
			(OS_ERR *)&err
			);
#endif

	/*------------------------ReciveCmd------------------------------*/
#ifdef DEBUG_RECIVECMD
	OSTaskCreate(
			(OS_TCB	*)&DDSReciveCmd,
			(CPU_CHAR	*)"ReciveCmd",
			(OS_TASK_PTR)DDS_Recivecmd,
			(void	*)0,
			(OS_PRIO	)10,
			(CPU_STK	*)&DDSTask_ReciveCmd_Stk[0],
			(CPU_STK_SIZE)APP_CFG_TASK_RECIVE_CMD_STk_SIZE / 2,
			(CPU_STK_SIZE)APP_CFG_TASK_RECIVE_CMD_STk_SIZE,
			(OS_MSG_QTY	)0,
			(OS_TICK	)0,
			(void	*)0,
			(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
			(OS_ERR *)&err
			);
#endif

	OSTaskDel((OS_TCB *)0,&err);
}

static void
DDS_Sweep(void *p_arg)
{
	CPU_FP64 step;
	CPU_FP64 frequency_temp;
	CPU_INT32U num_temp;
	CPU_INT32U result[NumberOfPoints];
	OS_ERR err;
	CPU_TS ts;
	CPU_INT32U count_temp;
	CPU_CHAR s[10];

#ifdef DEBUG_FREQUENCY_SWEEP
	StartFre = 0;
	EndFre = 60000;
	NumberOfPoints = 60000;
#endif

	while (1)
	{
		OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, &ts, &err);

		num_temp = 0;
		step =(EndFre - StartFre)/(NumberOfPoints*1.0);
		frequency_temp = StartFre;

#ifdef DEBUG_ADC
		DDS_ADC_configuration();
		ADC_SoftwareStartConv(ADC3);
#endif

		while (num_temp <= NumberOfPoints)
		{
			AD9850_WriteCmd(0,frequency_temp+step*num_temp);

//			delay(1000);

			result[num_temp]=ADC_converted_value;

			sprintf(s, "%d\n", ADC_converted_value);
			USART2_puts(s);

//			USART_SendData(USART2,(CPU_INT08U)ADC_converted_value&0xFF);
//			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
//			USART_SendData(USART2,(CPU_INT08U)(ADC_converted_value>>8)&0xFF);
//			while (USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);

#ifdef TEST_CASE1_ADC
			AD9850_WriteCmd(0,100000);
#endif
//			frequency_temp+=step;
			num_temp++;
		}
#ifdef DEBUG_ADC
		DDS_Sweep_DeInit();
#endif

#ifdef DEBUG_SEND_TASK
		OSQPost(
				(OS_Q *)&SendData_q,
				(void *)result,
				(OS_MSG_SIZE)sizeof(result),
				(OS_OPT )OS_OPT_POST_FIFO,
				(OS_ERR *)&err
				);
#endif
	}
}

static void DDS_SendData(void *p_arg)
{
	CPU_INT32U *data;
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;

	while (1)
	{
		data=OSQPend(
				(OS_Q *)&SendData_q,
				(OS_TICK )0,
				(OS_OPT)OS_OPT_PEND_BLOCKING,
				(OS_MSG_SIZE *)&size,
				(CPU_TS *)&ts,
				(OS_ERR *)&err
				);
	}
}
static void DDS_Recivecmd(void *p_arg)
{
	CPU_INT16U* cmd;
	OS_ERR err;
	OS_MSG_SIZE size;
	CPU_TS ts;

	while (1)
	{
		cmd=(CPU_INT16U*)OSQPend(
				(OS_Q *)&cmd_q,
				(OS_TICK )0,
				(OS_OPT)OS_OPT_PEND_BLOCKING,
				(OS_MSG_SIZE *)&size,
				(CPU_TS *)&ts,
				(OS_ERR *)&err
				);

		checkCmd((CPU_INT16U*)cmd);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	}
}

static void
checkCmd(CPU_INT16U *cmd)
{
	CPU_ERR err;
	if (*cmd == DDS_CMD_WORD)
	{
		switch (*(cmd+1))
		{
		case DDS_CMD_SET_FREQUENCY:

			break;
		case DDS_CMD_SET_START_FREQUENCY:
			StartFre = *(cmd+2)*4369+*(cmd+3);
			break;
		case DDS_CMD_SET_END_FREQUENCY:
			EndFre = *(cmd+2)*4369+*(cmd+3);
			break;
		case DDS_CMD_SET_NUMBER_OF_POINTS:
			NumberOfPoints = *(cmd+2)*4369+*(cmd+3);
			break;
		case DDS_CMD_FREQUENCY_SWEEP_START:
			OSTaskSemPost(
				&DDSSweepTCB,
				OS_OPT_POST_NONE,
				&err
				);
			break;
		}
	}
}

static void
DDS_ADC_configuration(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	  DMA_InitTypeDef       DMA_InitStructure;
	  GPIO_InitTypeDef      GPIO_InitStructure;

	  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	  /* DMA2 Stream0 channel0 configuration **************************************/
	  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_converted_value;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	  DMA_InitStructure.DMA_BufferSize = 1;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	  DMA_Cmd(DMA2_Stream0, ENABLE);

	  /* Configure ADC3 Channel12 pin as analog input ******************************/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  /* ADC Common Init **********************************************************/
	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	  ADC_CommonInit(&ADC_CommonInitStructure);

	  /* ADC3 Init ****************************************************************/
	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfConversion = 1;
	  ADC_Init(ADC3, &ADC_InitStructure);

	  /* ADC3 regular channel12 configuration *************************************/
	  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

	 /* Enable DMA request after last transfer (Single-ADC mode) */
	  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	  /* Enable ADC3 DMA */
	  ADC_DMACmd(ADC3, ENABLE);

	  /* Enable ADC3 */
	  ADC_Cmd(ADC3, ENABLE);
}

static void
DDS_Sweep_DeInit(void)
{
	ADC_DeInit();
	DMA_DeInit(DMA2_Stream0);
}

static void
delay(CPU_INT32U counts)
{
	CPU_INT32U temp=0;
	while (temp<counts)
	{
		temp++;
	}
}
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif
