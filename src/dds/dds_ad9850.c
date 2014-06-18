#include "dds.h"

void AD9850_ResetToSerial(void)
{
	DDS_W_CLK_0();
	DDS_FQ_UD_0();
	DDS_RESET_0();
	DDS_RESET_1();
	DDS_RESET_0();
	DDS_W_CLK_1();
	DDS_W_CLK_0();
	DDS_FQ_UD_1();
	DDS_FQ_UD_0();
}

void AD9850_InitHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(DDS_RESET_CLK | DDS_W_CLK_CLK | DDS_FQ_UD_CLK | DDS_D7_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DDS_RESET_PIN;
	GPIO_Init(DDS_RESET_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DDS_W_CLK_PIN;
	GPIO_Init(DDS_W_CLK_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DDS_FQ_UD_PIN;
	GPIO_Init(DDS_FQ_UD_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DDS_D7_PIN;
	GPIO_Init(DDS_D7_PORT, &GPIO_InitStructure);
	DDS_RESET_0();
	DDS_W_CLK_0();
	DDS_FQ_UD_0();
	DDS_D7_0();
	AD9850_ResetToSerial();
}

void AD9850_WriteCmd(uint8_t _ucPhase, double _dOutFreq)
{
	uint32_t ulFreqWord;
	uint8_t i;
	ulFreqWord =  (uint32_t)((_dOutFreq * 4294967295u) / AD9850_OSC_FREQ);
	for (i = 0; i < 32; i++)
	{
		if (ulFreqWord & 0x00000001)
		{
			DDS_D7_1();
		}
		else
		{
			DDS_D7_0();
		}
		ulFreqWord >>= 1;
		DDS_W_CLK_1();
		DDS_W_CLK_0();
	}
	for (i = 0; i < 8; i++)
	{
		if (_ucPhase & 0x00000001)
		{
			DDS_D7_1();
		}
		else
		{
			DDS_D7_0();
		}
		_ucPhase >>= 1;
		DDS_W_CLK_1();
		DDS_W_CLK_0();
	}
	DDS_FQ_UD_1();
	DDS_FQ_UD_0();
}
