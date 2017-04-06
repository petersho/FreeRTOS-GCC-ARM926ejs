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

#define SYS_INFO_BUF_SIZE	1024

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
typedef struct BLOCK_LINK
{
	struct BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	struct BLOCK_LINK *pxNextAllocBlock;
	size_t xBlockSize;						/*<< The size of the free block. */
	TaskHandle_t xOwner;
} MemBlockLink_t;

int cmd_test2(int argc, char* argv[])
{
	char *ptr1;
	char *ptr2;
	char *ptr3;
	MemBlockLink_t *ptr;


	vPrintf("test2 command\n");
	ptr1 = pvPortMalloc(1024);
	vPortFree(ptr1);
	ptr2 = pvPortMalloc(1024 * 2);
	ptr3 = pvPortMalloc(1024 * 4);

	for (ptr = getxStartTaskOwnerPtr() ; ptr->pxNextAllocBlock != NULL ; ptr = ptr->pxNextAllocBlock) {
		vPrintf("handle = 0x%x\n", ptr->xOwner);
		if (ptr->xOwner == NULL) {
			vPrintf("System default\n");
		} else {
			vPrintf("name = %s\n", pcTaskGetName(ptr->xOwner));
		}
		vPrintf("size = %d\n", (ptr->xBlockSize & (0x7fffffff)) - portBYTE_ALIGNMENT);
		vPrintf("========\n");
	}
	vPrintf("handle = 0x%x\n", ptr->xOwner);
	vPrintf("name = %s\n", pcTaskGetName(ptr->xOwner));
	vPrintf("size = %d\n", (ptr->xBlockSize & (0x7fffffff)) - portBYTE_ALIGNMENT);

	return 0;
}

int cmd_test3(int argc, char* argv[])
{
	pthread_mutex_t lock;

	//xSemaphoreCreateMutex();
	vPrintf("test3 command\n");

	return 0;
}

int cmd_test4(int argc, char* argv[])
{

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
#define CMD_TBL_TEST4	CMD_TBL_ENTRY(		\
	"test4",	5,	cmd_test4,	\
	"test4		- test4 test command"	\
),

struct cmd_table cmd_new_tbl[5] = {
	CMD_TBL_TEST1
	CMD_TBL_TEST2
	CMD_TBL_TEST3
	CMD_TBL_TEST4
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
	char *ptr = NULL;
	int i = 0;

	ptr = pvPortMalloc(SYS_INFO_BUF_SIZE);
	if (ptr == NULL)
		return 0;

	memset(ptr, 0, SYS_INFO_BUF_SIZE);

	vTaskList(ptr);
	vPrintf("Task\t\tState\tPri\tStack\tHeap\tNum\n");
	vPrintf("----------------------------------------------------\n");

	for (i = 0 ; i < SYS_INFO_BUF_SIZE ; i++) {
		if (*(ptr + i) != 0) {
			vPrintf("%c", *(ptr + i));
		}
	}

	vPortFree(ptr);

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

	testcode_init();
}
