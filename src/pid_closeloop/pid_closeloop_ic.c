#include "pid_clooseloop.h"

void pid_IC_init()
{
	pid_dac_config();
	pid_adc_config();
}

void pid_dac_config()
{
	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(PID_OUTPUT_PORT, ENABLE);
	RCC_APB1PeriphClockCmd(PID_OUTPUT_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PID_OUTPUT_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(PID_OUTPUT_PORT_s, &GPIO_InitStructure);

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;

	DAC_Init(PID_OUTPUT_PORT_CHANNEL, &DAC_InitStructure);
	DAC_Cmd(PID_OUTPUT_PORT_CHANNEL, ENABLE);
}

void
pid_adc_config()
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	ADC_InitTypeDef 		ADC_InitStructure;
	ADC_CommonInitTypeDef	ADC_CommonInitStructure;

	GPIO_InitStructure.GPIO_Pin = PID_INPUT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PID_INPUT_PORT_s, &GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_Interl;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(PID_INPUT_ADC, &ADC_InitStructure);
	ADC_RegularChannelConfig(PID_INPUT_ADC, PID_INPUT_PORT_CHANNEL, 1, ADC_SampleTime_144Cycles);
	ADC_ITConfig(PID_INPUT_ADC, ADC_IT_EOC, ENABLE);
	ADC_Cmd(PID_INPUT_ADC, ENABLE);
}

void
pid_IC_outputz(unsigned char channel, int value)
{
	if (channel == DAC_Channel_1)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R, value & 0xfff);
	}
	else
	{
		DAC_SetChannel2Data(DAC_Align_12b_R, value & 0xfff);
	}
}

void
pid_value_signal(unsigned short *signal)
{
	*signal = ADC_GetConversionValue(PID_INPUT_ADC);
}
