#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define CMDBUF_SIZE	128

typedef struct cmd_table {
	char	*name;
	int	len_name;
	int 	(*Func)(int argc, char* argv[]);
	char 	*usage;
	//struct	cmd_table 	*next_cmd;
//	struct	cmd_table 	*prev_lv;
} CMD_TABLE;

struct cmd_list {
	CMD_TABLE		*cmd;
	struct cmd_list		*next_cmd;
};
//int cmd_help(int argc, char* argv[]);
//int cmd_back(int argc, char* argv[]);
//int cmd_quit(int argc, char* argv[]);

#define CMD_TBL_ENTRY(name, len_name, cmd, usage)  {name, len_name, cmd, usage}

void init_cmd_register(void);
int cmd_register(struct cmd_table new[]);
int parse_cmd2(char *cmd);

#endif
