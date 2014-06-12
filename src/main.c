#include "stm32f4xx.h"
#include "includes.h"
#include "app_cfg.h"
#include <stdio.h>
#include "comm.h"

void
sender(unsigned char* cmd)
{
	*cmd=0x01;
	*(cmd+1)=0x02;
	*(cmd+2)=0x03;
	*(cmd+3)=0x04;
}
void
excu(short para_1, short para_2)
{

}

/*
 * STM32F4 led blink sample (retargetted to SWO).
 *
 * In debug configurations, demonstrate how to print a greeting message
 * on the standard output. In release configurations the message is
 * simply discarded. By default the trace messages are forwarded to the SWO,
 * but can be rerouted to semihosting or completely suppressed by changing
 * the definitions in misc/include/trace_impl.h.
 *
 * Then demonstrates how to blink a led with 1Hz, using a
 * continuous loop and SysTick delays.
 *
 * On DEBUG, the uptime in seconds is also displayed on the standard output.
 *
 * The external clock frequency is specified as a preprocessor definition
 * passed to the compiler via a command line option (see the 'C/C++ General' ->
 * 'Paths and Symbols' -> the 'Symbols' tab, if you want to change it).
 * The value selected during project creation was HSE_VALUE=168000000.
 *
 * Note1: The default clock settings assume that the HSE_VALUE is a multiple
 * of 1MHz, and try to reach the maximum speed available for the
 * board. It does NOT guarantee that the required USB clock of 48MHz is
 * available. If you need this, please update the settings of PLL_M, PLL_N,
 * PLL_P, PLL_Q in libs/CMSIS/src/system_stm32f4xx.c to match your needs.
 *
 * Note2: The external memory controllers are not enabled. If needed, you
 * have to define DATA_IN_ExtSRAM or DATA_IN_ExtSDRAM and to configure
 * the memory banks in libs/CMSIS/src/system_stm32f4xx.c to match your needs.
 *
 * The build does not use startup files, and on Release it does not even use
 * any standard library function (on Debug the printf() brings lots of
 * functions; removing it should also use no other standard lib functions).
 *
 * If the application requires special initialisation code present
 * in some other libraries (for example librdimon.a, for semihosting),
 * define USE_STARTUP_FILES and uncheck the corresponding option in the
 * linker configuration.
 *
 */

static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static void ledbling(void * p_arg);

// ----------------------------------------------------------------------------

static void
Delay(__IO uint32_t nTime);

static void
TimingDelay_Decrement(void);

void
SysTick_Handler(void);

/* ----- SysTick definitions ----------------------------------------------- */

#define SYSTICK_FREQUENCY_HZ       1000

/* ----- LED definitions --------------------------------------------------- */

/* Adjust them for your own board. */

#if defined(BOARD_OLIMEX_STM32_E407)

/* STM32-E407 definitions (the GREEN LED) */

#define BLINK_PORT      GPIOC
#define BLINK_PIN       13
#define BLINK_RCC_BIT   RCC_AHB1Periph_GPIOC

#else

/* STM32F4DISCOVERY definitions (the GREEN LED) */

#define BLINK_PORT      GPIOD
#define BLINK_PIN       12
#define BLINK_RCC_BIT   RCC_AHB1Periph_GPIOD

#endif

#define BLINK_TICKS     SYSTICK_FREQUENCY_HZ/10

// ----------------------------------------------------------------------------

int
main(void)
{
	OS_ERR err;
	OSInit(&err);

	OSTaskCreate(
			(OS_TCB	*)&AppTaskStartTCB,
			(CPU_CHAR	*)"Led Blink",
			(OS_TASK_PTR)ledbling,
			(void	*)0,
			(OS_PRIO	)1,
			(CPU_STK	*)&AppTaskStartStk[0],
			(CPU_STK_SIZE)AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10],
			(CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,
			(OS_MSG_QTY	)0,
			(OS_TICK	)0,
			(void	*)0,
			(OS_OPT)(OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR),
			(OS_ERR *)&err
			);



  /*
   * At this stage the microcontroller clock setting is already configured,
   * this is done through SystemInit() function which is called from startup
   * files (startup_cm.c) before to branch to the
   * application main. To reconfigure the default setting of SystemInit()
   * function, refer to system_stm32f4xx.c file.
   */

  RCC_ClocksTypeDef RCC_Clocks;

  /* Use SysTick as reference for the timer */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / SYSTICK_FREQUENCY_HZ);

  /* GPIO Periph clock enable */
  RCC_AHB1PeriphClockCmd(BLINK_RCC_BIT, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure pin in output push/pull mode */
  GPIO_InitStructure.GPIO_Pin = (1 << BLINK_PIN);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(BLINK_PORT, &GPIO_InitStructure);

  comm_init();
  comm_cmd_register(excu,sender,0x0001,0x0001);

  OSStart(&err);

}

void
ledbling(void *p_arg)
{


	while (1)
	  {
	    GPIO_ResetBits(BLINK_PORT, (1 << BLINK_PIN));
	    Delay(BLINK_TICKS);
	    GPIO_SetBits(BLINK_PORT, (1 << BLINK_PIN));
	    Delay(BLINK_TICKS);
	    short a[4];
	    a[0]=0x0001;
	    a[1]=0x0203;
	    a[2]=0x0405;
	    a[3]=0x0607;
	    comm_IC_array_send((unsigned char*)a, 8);
	  }
}

// ----------------------------------------------------------------------------

static __IO uint32_t uwTimingDelay;

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in SysTick ticks.
 * @retval None
 */
void
Delay(__IO uint32_t nTime)
{
  uwTimingDelay = nTime;

  while (uwTimingDelay != 0)
    ;
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void
TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
    {
      uwTimingDelay--;
    }
}

// ----------------------------------------------------------------------------

/**
 * @brief  This function is the SysTick Handler.
 * @param  None
 * @retval None
 */
void
SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

// ----------------------------------------------------------------------------

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif
