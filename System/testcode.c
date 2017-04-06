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
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <console.h>

int cmd_testcode_test_memcpy(int argc, char* argv[])
{
	int size_array[4] = {32, 64, 127, 1024};
	char *src, *dst;
	//char src[32], dst[32];
	int i = 0, j = 0;
	int error = 0;

	vPrintf("test code : memcpy : START\n");
	for (i = 0 ; i < (sizeof(size_array) / sizeof(int)) ; i++) {
		src = pvPortMalloc(size_array[i]);
		dst = pvPortMalloc(size_array[i]);

		/* Init src and dst data */
		for (j = 0 ; j < size_array[i] ; j++) {
			src[j] = (j + 1) & 0xFF;
			dst[j] = 0;
		}

		memcpy(dst, src, size_array[i]);

		/* Checking dst data */
		for (j = 0 ; j < size_array[i] ; j++) {
			if (dst[j] != src[j]) {
				vPrintf("data mismatch\n");
					error++;
			}
		}

		vPortFree(src);
		vPortFree(dst);

		if (error == 0) {
			vPrintf("test code : memcpy : PASS : %d bytes\n", size_array[i]);
		}
		error = 0;
	}
	return 0;
}

int cmd_testcode_test_memcmp(int argc, char* argv[])
{
	vPrintf("cmd_testcode_test_memcmp command\n");

	return 0;
}

#define CMD_TESTCODE_MEMCPY	CMD_TBL_ENTRY(		\
	"test-memcpy",	11,	cmd_testcode_test_memcpy,	\
	"test-memcpy	- Test memcpy C library"	\
),

#define CMD_TESTCODE_MEMCMP	CMD_TBL_ENTRY(		\
	"test-memcmp",	11,	cmd_testcode_test_memcmp,	\
	"test-memcmp	- Test memcmp C library"	\
),

struct cmd_table cmd_testcode_tbl[3] = {
	CMD_TESTCODE_MEMCPY
	CMD_TESTCODE_MEMCMP
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

void testcode_init()
{
	cmd_register(cmd_testcode_tbl);
}
