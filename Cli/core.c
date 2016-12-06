#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"

#define PROMPT	"pCLI > "

int cmd_test1(int argc, char* argv[])
{
	vPrintMsg("test1 command\n");

	return 0;
}
int cmd_test2(int argc, char* argv[])
{
	vPrintMsg("test2 command\n");

	return 0;
}
int cmd_test3(int argc, char* argv[])
{
	vPrintMsg("test3 command\n");

	return 0;
}

int cmd_example1(int argc, char* argv[])
{
	vPrintMsg("cmd_example1 command\n");

	return 0;
}

int cmd_example2(int argc, char* argv[])
{
	vPrintMsg("cmd_example2 command\n");

	return 0;
}

int cmd_quit(int argc, char* argv[])
{
	//exit(0);
}
#define CMD_TBL_QUIT	CMD_TBL_ENTRY(		\
	"quit",		4,	cmd_quit,	\
	"quit		- Exit feicli"		\
),


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

#define CMD_TBL_EXAMPLE1	CMD_TBL_ENTRY(		\
	"example1",	8,	cmd_example1,	\
	"example1	- example1 command"	\
),
#define CMD_TBL_EXAMPLE2	CMD_TBL_ENTRY(		\
	"example2",	8,	cmd_example2,	\
	"example2	- example2 command"	\
),

struct cmd_table cmd_new_tbl[4] = {
	CMD_TBL_TEST1
	CMD_TBL_TEST2
	CMD_TBL_TEST3
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

struct cmd_table cmd_example_tbl[3] = {
	CMD_TBL_EXAMPLE1
	CMD_TBL_EXAMPLE2
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

struct cmd_table cmd_system_tbl[2] = {
	CMD_TBL_QUIT
	CMD_TBL_ENTRY(NULL, 0, NULL, NULL)
};

//struct cmd_table *topcmd = NULL;
//struct cmd_table *currcmd = NULL;

struct cmd_list *topcmd = NULL;
struct cmd_list *currcmd = NULL;

int show_cmd_help()
{
	struct cmd_list *curr = topcmd;

	while (curr->cmd != NULL) {
		print_msg("%s\n", curr->cmd->usage);
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
		topcmd = pvPortMalloc(sizeof(struct cmd_list));
		//topcmd->name = NULL;
		topcmd->next_cmd = NULL;
		currcmd = topcmd;
	}
}

void add_command_test()
{
	cmd_register(cmd_new_tbl);
}

int cmd_register(struct cmd_table new[])
{
	int i = 0;

	while (new[i].name != NULL) {
		//memcpy(currcmd, &new[i], sizeof(struct cmd_table));

		currcmd->cmd = &new[i];

		//currcmd->next_cmd = malloc(sizeof(struct cmd_list));
		currcmd->next_cmd = pvPortMalloc(sizeof(struct cmd_list));
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
