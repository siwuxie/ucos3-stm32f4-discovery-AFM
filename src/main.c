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
	OSStart(&err);
}


//static void
//DDS_ADC_configuration(void)
//{
//	ADC_InitTypeDef       ADC_InitStructure;
//	  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	  DMA_InitTypeDef       DMA_InitStructure;
//	  GPIO_InitTypeDef      GPIO_InitStructure;
//
//	  /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
//
//	  /* DMA2 Stream0 channel0 configuration **************************************/
//	  DMA_InitStructure.DMA_Channel = DMA_Channel_2;
//	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
//	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC_converted_value;
//	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	  DMA_InitStructure.DMA_BufferSize = 1;
//	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
//	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//	  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//	  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
//	  DMA_Cmd(DMA2_Stream0, ENABLE);
//
//	  /* Configure ADC3 Channel12 pin as analog input ******************************/
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//	  GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	  /* ADC Common Init **********************************************************/
//	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//	  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
//	  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//	  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;
//	  ADC_CommonInit(&ADC_CommonInitStructure);
//
//	  /* ADC3 Init ****************************************************************/
//	  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//	  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//	  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	  ADC_InitStructure.ADC_NbrOfConversion = 1;
//	  ADC_Init(ADC3, &ADC_InitStructure);
//
//	  /* ADC3 regular channel12 configuration *************************************/
//	  ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
//
//	 /* Enable DMA request after last transfer (Single-ADC mode) */
//	  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
//
//	  /* Enable ADC3 DMA */
//	  ADC_DMACmd(ADC3, ENABLE);
//
//	  /* Enable ADC3 */
//	  ADC_Cmd(ADC3, ENABLE);
//}
//
//static void
//DDS_Sweep_DeInit(void)
//{
//	ADC_DeInit();
//	DMA_DeInit(DMA2_Stream0);
//}
//
//static void
//delay(CPU_INT32U counts)
//{
//	CPU_INT32U temp=0;
//	while (temp<counts)
//	{
//		temp++;
//	}
//}
//#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
//CPU_TS_TMR  CPU_TS_TmrRd (void)
//{
//    CPU_TS_TMR  ts_tmr_cnts;
//
//    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;
//
//    return (ts_tmr_cnts);
//}
//#endif
//
