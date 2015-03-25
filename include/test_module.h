#include "module.h"

#define MOD_TEST_HEAD						0x0000

#define MOD_TEST_TASK_BLINK					0x00

#define MOD_TEST_CMD_BLINK					0x00
#define MOD_TEST_CMD_SET_BLINK				0x01

OS_Q	BlinkQ;

static OS_TCB Test_led_blink_TCB;
static CPU_STK Test_led_blink_STK[256];

void task_led_blink(void *p_arg);

#define BLINK_PORT      GPIOD
#define BLINK_PIN       12
#define BLINK_RCC_BIT   RCC_AHB1Periph_GPIOD

void test_module_init();
void test_task_init();
void interface_blink();
void interface_set_blink();
void test_dispatch(void *msg);

