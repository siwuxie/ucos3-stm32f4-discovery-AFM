#include "comm.h"


void
comm_init()
{
	comm_IC_init();
}

//void
//comm_cmd_dispatch(unsigned short* cmd)
//{
//	*cmd=*(cmd+4);
//	*(cmd+1)=*(cmd+5);
//}

void
comm_cmd_send(short data, int cmd_word, int cmd_head)
{
	short cmd[COMM_CMD_WORD_SIZE];
}
