#include "history.h"

static int	init_command(t_comm_var *comm_var)
{
	comm_var->len = 0;
	comm_var->cap = BUFF_SIZE;
	comm_var->key[0] = 0;
	comm_var->key_len = 0;
	comm_var->command = malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!comm_var->command)
		return (1);
	comm_var->command[BUFF_SIZE] = 0;
	return (0);
}

static void	handle_command_confirm(char real_time_buff, t_comm_var *comm_var,
t_command_buff **command_buff)
{
	if (real_time_buff == '\n') // ENTER (CR)
	{
		if (add_command(command_buff, comm_var->command))
			clean_exit(EXIT_FAILURE);
		free(comm_var->command);
		init_command(comm_var);
		if (print_prompt())
			clean_exit(EXIT_FAILURE);
		/*INIT PROCESSING AND EXECUTION OF THE CONFIRMED COMMAND*/
	}
}

static int	handle_keycode(char real_time_buff, t_comm_var *comm_var,
t_key_code const key_code, t_command_buff *command_buff)
{
	comm_var->key[comm_var->key_len] = real_time_buff;
	comm_var->key[comm_var->key_len + 1] = 0;
	comm_var->key_len++;
	if (comm_var->key_len == 5)
		comm_var->key[0] = 0;
	else
	{
		if (!execute_capability(comm_var->key, key_code, command_buff))
		{
			comm_var->key[0] = 0;
			comm_var->key_len = 0;
			return (1);
		}
	}
	return (0);
}

static int	inc_comm_cap(char **command, int *comm_cap, int comm_len)
{
	char	*aux;
	size_t	i;

	*comm_cap = comm_len + BUFF_SIZE;
	aux = malloc(sizeof(char) * (*comm_cap + 1));
	if (!aux)
		return (1);
	aux[*comm_cap] = 0;
	i = 0;
	while (*command[i])
	{
		aux[i] = *command[i];
		++i;
	}
	aux[i] = 0;
	free(*command);
	*command = strdup(aux);
	free(aux);
	return (0);
}

void		reader(t_key_code const key_code)
{
	t_command_buff	*command_buff;
	t_comm_var		comm_var;
	char			real_time_buff;

	if (init_command(&comm_var))
		clean_exit(EXIT_FAILURE);
	command_buff = 0;
	while (read(1, &real_time_buff, 1) != -1)
	{
		if (real_time_buff == '\004') // Ctrl + D
			clean_exit(EXIT_SUCCESS);
		else if (comm_var.key[0] || key_code.arrow_up[0] == real_time_buff ||
		key_code.backspace[0] == real_time_buff ||
		key_code.del[0] == real_time_buff)
		{
			if (handle_keycode(real_time_buff, &comm_var, key_code,
			command_buff))
				continue ;
		}
		if (comm_var.key[0])
			continue ;
		write(1, &real_time_buff, 1);
		if (comm_var.len == comm_var.cap)
		{
			if (inc_comm_cap(&comm_var.command, &comm_var.cap, comm_var.len))
				clean_exit(EXIT_FAILURE);
		}
		comm_var.command[comm_var.len] = real_time_buff;
		comm_var.command[comm_var.len + 1] = '\0';
		comm_var.len++;
		handle_command_confirm(real_time_buff, &comm_var, &command_buff);
	}
	clean_exit(EXIT_FAILURE);
}
