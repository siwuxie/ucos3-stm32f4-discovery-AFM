#include "pid_clooseloop.h"

void pid_IC_init()
{
	pid_dac_config();
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

void pid_IC_outputz(unsigned char channel, int value)
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
