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

int cmd_sys_date(int argc, char* argv[])
{
	vPrintf("date command\n");

	return 0;
}

int cmd_sys_free(int argc, char* argv[])
{
	vPrintf("free command\n");

	return 0;
}

int cmd_sys_info(int argc, char* argv[])
{
	vPrintf("sysinfo command\n");

	return 0;
}

int cmd_sys_ps(int argc, char* argv[])
{
	vPrintf("ps command\n");

	return 0;
}

#define CMD_SYSTEM_DATE	CMD_TBL_ENTRY(		\
	"date",	4,	cmd_sys_date,	\
	"date		- Show date"	\
),

#define CMD_SYSTEM_FREE	CMD_TBL_ENTRY(		\
	"free",	4,	cmd_sys_free,	\
	"free		- Show free"	\
),

#define CMD_SYSTEM_INFO	CMD_TBL_ENTRY(		\
	"sysinfo",	7,	cmd_sys_info,	\
	"sysinfo		- Show system info"	\
),

#define CMD_SYSTEM_PS	CMD_TBL_ENTRY(		\
	"ps",	2,	cmd_sys_ps,	\
	"ps		- Show ps"	\
),

struct cmd_table cmd_system_tbl[5] = {
	CMD_SYSTEM_DATE
	CMD_SYSTEM_FREE
	CMD_SYSTEM_INFO
	CMD_SYSTEM_PS
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

void system_init()
{
	cmd_register(cmd_trace_tbl);
	cmd_register(cmd_system_tbl);
	cmd_register(cmd_new_tbl);
}
