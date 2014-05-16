#ifndef CMD_DEF
#define CMD_DEF


#define DDS_CMD_WORD   0x0001
#define DDS_DATA_WORD  0x0002

#define DDS_CMD_SET_FREQUENCY 0x0001
#define DDS_CMD_SET_START_FREQUENCY 0x0002
#define DDS_CMD_SET_END_FREQUENCY 0x0003

#define DDS_CMD_SET_NUMBER_OF_POINTS 0x0004

#define DDS_CMD_FREQUENCY_SWEEP_START 0x0005

#define NOT_A_CMD 0u



/*-----------------Debug-----------------*/

#define DEBUG_FREQUENCY_TASK
#define DEBUG_RECIVECMD

#define DEBUG_ADC
//#define TEST_CASE1_ADC

//#define USART_SEND_TEST
#define ENDING_WRONG

/*---------------------------------------*/

#endif
