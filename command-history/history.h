#ifndef _HISTORY_
# define _HISTORY_

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>

# include <termios.h>
# include <termcap.h>
# include <curses.h> //PARA OBTENER VALOR DE "ERR"

#define BUFF_SIZE 10

struct termios	init_term_attr; //VARIABLE GLOBAL

typedef struct	s_comm_var
{
	char	*command;
	int		len;
	int		cap;
	char	key[6];
	int		key_len;
}				t_comm_var;

typedef struct	s_key_code
{
	char	*arrow_up;
	char	*arrow_down;
	char	*arrow_left;
	char	*arrow_right;
	char	*backspace;
	char	*del;
}				t_key_code;

typedef struct	s_command_buff
{
	char					*command;
	struct s_command_buff	*next;
}				t_command_buff;

/*
**	termcap_config.c
*/

int				tputs_write(int c);
int				init_termcap();
int				setup_key_read(t_key_code *key_code);
int				end_termcap();

/*
**	terminal_capabilities.c
*/

int				execute_capability(char *target_key, t_key_code key_code,
t_command_buff *command_buff);

/*
**	command_buffer.c
*/

int				init_comm_buff(t_command_buff **command_buff);
int				add_command(t_command_buff **head, char	*command);

/*
**	reader.c
*/

void			reader(t_key_code const keycode);

/*
**	printer.c
*/

int				print_prompt();
int				print_command(char *command);

/*
**	ending.c
*/

void			clean_exit(int exit_code);

#endif
