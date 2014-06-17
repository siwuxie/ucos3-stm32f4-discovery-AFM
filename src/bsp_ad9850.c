/*
*********************************************************************************************************
*
*	ģ����� : AD9850��ģ��
*	�ļ���� : bsp_ad9850.c
*	��    �� : V2.0
*	˵    �� : ��DDSоƬAD9850���������Ҳ��ͷ���
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*       v1.0    2012-07-05 armfly  �װ档ST�̼���V3.5.0�汾��
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

#include "bsp_ad9850.h"
#define AD9850_OSC_FREQ		125000000u

#define DDS_RESET_PORT	GPIOA
#define DDS_RESET_PIN	GPIO_Pin_6
#define DDS_RESET_CLK	RCC_AHB1Periph_GPIOA

#define DDS_W_CLK_PORT	GPIOA
#define DDS_W_CLK_PIN	GPIO_Pin_5
#define DDS_W_CLK_CLK	RCC_AHB1Periph_GPIOA

#define DDS_FQ_UD_PORT	GPIOA
#define DDS_FQ_UD_PIN	GPIO_Pin_4
#define DDS_FQ_UD_CLK	RCC_AHB1Periph_GPIOA

#define DDS_D7_PORT	GPIOA
#define DDS_D7_PIN		GPIO_Pin_7
#define DDS_D7_CLK		RCC_AHB1Periph_GPIOA

#define DDS_RESET_1()	GPIO_SetBits(DDS_RESET_PORT,(DDS_RESET_PIN))
#define DDS_RESET_0()	GPIO_ResetBits(DDS_RESET_PORT,(DDS_RESET_PIN))

#define DDS_W_CLK_1()	GPIO_SetBits(DDS_W_CLK_PORT,(DDS_W_CLK_PIN))
#define DDS_W_CLK_0()	GPIO_ResetBits(DDS_W_CLK_PORT,(DDS_W_CLK_PIN))

#define DDS_FQ_UD_1()	GPIO_SetBits(DDS_FQ_UD_PORT,(DDS_FQ_UD_PIN))
#define DDS_FQ_UD_0()	GPIO_ResetBits(DDS_FQ_UD_PORT,(DDS_FQ_UD_PIN))

#define DDS_D7_1()		GPIO_SetBits(DDS_D7_PORT,(DDS_D7_PIN))
#define DDS_D7_0()		GPIO_ResetBits(DDS_D7_PORT,(DDS_D7_PIN))

static void AD9850_ResetToSerial(void);

/*
*********************************************************************************************************
*	�� �� ��: AD9850_InitHard
*	����˵��: ��ʼ��AD9850�����ӵ�CPU GPIO�˿�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
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

	AD9850_ResetToSerial();		/* ��λ���봮��ģʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: AD9850_ResetToSerial
*	����˵��: ��λAD9850��֮��Ϊ����ģʽ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AD9850_ResetToSerial(void)
{
	DDS_W_CLK_0();
	DDS_FQ_UD_0();
	DDS_RESET_0();

	DDS_RESET_1();
	DDS_RESET_0();

	/* ����һ��W_CLK���� */
	DDS_W_CLK_1();
	DDS_W_CLK_0();

	/* ����һ��FQ_UP���� */
	DDS_FQ_UD_1();
	DDS_FQ_UD_0();

	/* ֮��AD9850 ���봮��ģʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: AD9850_WriteCmd
*	����˵��: ������Э�飬����40bit��������
*	��    �Σ�_ucPhase ����λ����(һ����0����_dOutFreq ��Ƶ�ʲ���(������)����λHz���������� 0.01Hz
*	�� �� ֵ: �������
*********************************************************************************************************
*/
void AD9850_WriteCmd(uint8_t _ucPhase, double _dOutFreq)
{
	uint32_t ulFreqWord;
	uint8_t i;

	/* ����ź�Ƶ�ʼ��㹫ʽ��
		fOUT = (�� Phase �� CLKIN) / (2��32�η�))
			 �� Phase = value of 32-bit tuning word
			CLKIN     = input reference clock frequency in MHz
			fOUT      = frequency of the output signal in MHz
		����Ϊ��
		�� Phase = (fOUT * ��2��32�η�) / CLKIN
				 = (fOUT * 4294967296) / CLKIN
	*/

	/* AD9850 ������ݸ�ʽ

		����40��W_CLK������40��Bit ��W0~W39)��Ҳ����5���ֽ�

		ǰ4���ֽڣ���32bit����λ�ȴ������� Freq
	*/


	/* ��ݲ���Ƶ�ʼ���32λ��Ƶ�ʲ��� */
	ulFreqWord =  (uint32_t)((_dOutFreq * 4294967295u) / AD9850_OSC_FREQ);

	/* д32bit Ƶ���� */
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

	/* ���͵�5���ֽڣ���λ���� һ���� 0 */
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

	/* ֮ǰ�������д��AD9850�Ļ��������ڸ�һ��FQ_UD���壬�����¼Ĵ���Ӱ����� */
	DDS_FQ_UD_1();
	DDS_FQ_UD_0();
}

