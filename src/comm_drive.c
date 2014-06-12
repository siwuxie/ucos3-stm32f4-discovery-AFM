#include "comm.h"

static comm_excutor excutor_list[COMM_EXCUTOR_LIST_SIZE];
static unsigned char excutor_count;

void
comm_init()
{
	comm_IC_init();
	excutor_count=0;
}

void
comm_cmd_register(void(*excu)(short para_1, short para_2),void (*sender)(unsigned char* cmd), int cmd_word, int cmd_head)
{
	excutor_list[excutor_count].cmd_head = cmd_head;
	excutor_list[excutor_count].cmd_word = cmd_word;
	excutor_list[excutor_count].excu = excu;
	excutor_list[excutor_count].sender = sender;
	excutor_count++;
}

void
comm_cmd_dispatch(short* cmd)
{
	for (int i=0;i<excutor_count;i++)
	{
		if (*cmd == excutor_list[i].cmd_word)
		{
			if (*(cmd+1) == excutor_list[i].cmd_head)
			{
				(*excutor_list[i].excu)(*(cmd+2),*(cmd+3));
				break;
			}
		}
	}
}

void
comm_cmd_send(short data, int cmd_word, int cmd_head)
{
	short cmd[COMM_CMD_WORD_SIZE];
	for (int i=0;i<excutor_count;i++)
	{
		if ((cmd_word == excutor_list[excutor_count].cmd_word)
				&&(cmd_head == excutor_list[excutor_count].cmd_head))
		{
			excutor_list[excutor_count].sender(cmd);
			comm_IC_array_send((unsigned char*)cmd, COMM_CMD_WORD_SIZE*2);
		}
	}
}
