#include <stdio.h>
#include <stdlib.h>
#include <console.h>
#include <trcUser.h>

int cmd_trace_info(int argc, char* argv[])
{
	//vPrintMsg("test1 command\n");
	vPrintf("Trace data addr = 0x%x\n", vTraceGetTraceBuffer());
	vPrintf("Trace data size = %d\n", uiTraceGetTraceBufferSize());

	return 0;
}
#define CMD_TBL_TRCINFO	CMD_TBL_ENTRY(		\
	"trcinfo",		7,	cmd_trace_info,	\
	"trcinfo		- Show Trace Data and Address"		\
),

struct cmd_table cmd_trace_tbl[2] = {
	CMD_TBL_TRCINFO
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

int cmd_test1(int argc, char* argv[])
{
	vPrintf("test1 command\n");

	return 0;
}
int cmd_test2(int argc, char* argv[])
{
	vPrintf("test2 command\n");

	return 0;
}
int cmd_test3(int argc, char* argv[])
{
	vPrintf("test3 command\n");

	return 0;
}

int cmd_example1(int argc, char* argv[])
{
	vPrintf("cmd_example1 command\n");

	return 0;
}

#define CMD_TBL_TEST1	CMD_TBL_ENTRY(		\
	"test1",	5,	cmd_test1,	\
	"test1		- test1 test command"	\
),
#define CMD_TBL_TEST2	CMD_TBL_ENTRY(		\
	"test2",	5,	cmd_test2,	\
	"test2		- test2 test command"	\
),
#define CMD_TBL_TEST3	CMD_TBL_ENTRY(		\
	"test3",	5,	cmd_test3,	\
	"test3		- test3 test command"	\
),

struct cmd_table cmd_new_tbl[4] = {
	CMD_TBL_TEST1
	CMD_TBL_TEST2
	CMD_TBL_TEST3
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

void system_init()
{
	cmd_register(cmd_trace_tbl);
}
