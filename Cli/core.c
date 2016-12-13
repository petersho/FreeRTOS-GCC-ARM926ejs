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
#include <FreeRTOS.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <console.h>

#define PROMPT	"pCLI > "

struct cmd_list *topcmd = NULL;
struct cmd_list *currcmd = NULL;

int show_cmd_help()
{
	struct cmd_list *curr = topcmd;

	while (curr->cmd != NULL) {
		vPrintf("%s\n", curr->cmd->usage);
		//vPrintMsg("Help message\n");

		if (curr->next_cmd != NULL) {
			curr = curr->next_cmd;
		} else {
			break;
		}
	}

	return 0;
}

int parse_line()
{
	return 0;
}

int parse_cmd2(char *cmd)
{
	struct cmd_list *curr = topcmd;
	int found = 0;

	if (strlen(cmd) == 0)
		return 0;

	while (curr->cmd != NULL) {
		found = strncmp(curr->cmd->name, cmd, curr->cmd->len_name);
		if (!found) {
			if (curr->cmd->Func != NULL) {
				curr->cmd->Func(0,0);
				break;
			}
		}

		if (curr->next_cmd != NULL)
			curr = curr->next_cmd;
		else
			break;
	}

	return found;
}

#if 0
int cmd_register(struct cmd_table new[])
{
	int i = 0;

	while (new[i].name != NULL) {
		if (topcmd == NULL) {
			topcmd = &new[i];
			currcmd = topcmd;
		} else {
			currcmd->next_cmd = &new[i];
			currcmd = &new[i];
		}
		i++;
	}

	return 0;
}
#endif

void init_cmd_register()
{
	if (topcmd == NULL) {
		//topcmd = malloc(sizeof(struct cmd_list));
		topcmd = (struct cmd_list*)pvPortMalloc(sizeof(struct cmd_list));
		//topcmd->name = NULL;
		topcmd->next_cmd = NULL;
		currcmd = topcmd;
	}
}

int cmd_register(struct cmd_table new[])
{
	int i = 0;

	while (new[i].name != NULL) {
		//memcpy(currcmd, &new[i], sizeof(struct cmd_table));

		currcmd->cmd = &new[i];

		//currcmd->next_cmd = malloc(sizeof(struct cmd_list));
		currcmd->next_cmd = (struct cmd_list *)pvPortMalloc(sizeof(struct cmd_list));
		currcmd = currcmd->next_cmd;
		//currcmd->name = NULL;
		currcmd->next_cmd = NULL;

		i++;
	}

	return 0;
}

#if 0
int main()
{
	int c = 0;
	int i = 0;
	char history_cmd[CMDBUF_SIZE];
	char cmdbuf[CMDBUF_SIZE];
	int ret = 0;

	memset(history_cmd, 0, sizeof(history_cmd));
	memset(cmdbuf, 0, sizeof(cmdbuf));

	//vPrintMsg("%s ", PROMPT);
	vPrintMsg("pCLI > ");

	// Init command list
	init_cmd_register();
	cmd_register(cmd_system_tbl);
	cmd_register(cmd_new_tbl);
	cmd_register(cmd_example_tbl);

	while (1) {
		c = getchar();

		if (c == 0xa) { /* enter key */
			if (i > 0) {
				//vPrintMsg("\n");
				cmdbuf[i] = 0x0;
				ret = parse_cmd2(cmdbuf);
				if (ret != 0) { // Command not found!!
					show_cmd_help();
				}
				memset(cmdbuf, 0, sizeof(cmdbuf));
				i = 0;
			}
			//vPrintMsg("%s ",PROMPT);
			vPrintMsg("pCLI > ");

		} else {
			//vPrintMsg("%c", c);
			cmdbuf[i] = c;
			i++;
                        
			if (i > (CMDBUF_SIZE - 1)) {
				i = 0;
				memset(cmdbuf, 0, sizeof(cmdbuf));
			} 
		}
	}

	return 0;
}
#endif
