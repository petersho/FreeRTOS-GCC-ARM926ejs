/*
    Copyright (C) 2016 <peterkoug@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <console.h>
#include <trcUser.h>

#include <pthread.h>

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

void lloop()
{
	int i = 0;

	for (i = 0 ; i < 5 ; i++) {
		vPrintf("pthread created teak\n");
		vTaskDelay(1000);
	}

	vTaskDelete(NULL);
}

int cmd_test1(int argc, char* argv[])
{
	pthread_t id;
	int ret = 0;

	ret = pthread_create(&id,NULL,(void *) lloop, NULL);

	pthread_join(id, NULL);

	vPrintf("test1 pthread command\n");
	//vTaskDelay(5000);
	//vTaskDelete((TaskHandle_t)id);

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
