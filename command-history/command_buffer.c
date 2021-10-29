#include "history.h"

int	init_comm_buff(t_command_buff **command_buff)
{
	*command_buff = malloc(sizeof(t_command_buff));
	if (!(*command_buff))
		return (1);
	(*command_buff)->command = 0;
	(*command_buff)->next = 0;
	return (0);
}

int	add_command(t_command_buff **head, char	*command)
{
	t_command_buff	*aux;

	if (!(*head))
	{
		if (init_comm_buff(head))
			return (1);
		(*head)->command = strdup(command);
		return (0);
	}
	aux = malloc(sizeof(t_command_buff));
	if (!aux)
		return (1);
	aux->command = (*head)->command;
	aux->next = (*head)->next;
	(*head)->command = strdup(command);
	(*head)->next = aux;
	return (0);
}
