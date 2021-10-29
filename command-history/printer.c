#include "history.h"

int	print_prompt()
{
	if (write(1, "my_prompt:$ ", 12) == -1)
		return (1);
	return (0);
}

int	print_command(char *command)
{
	/*
	**	NO USO puts AQUÍ PORQUE AÑADE UN '\n' A command,
	**	QUE YA CONTIENE UNO.
	**	
	**	if (puts(command) == EOF)
	**	return (1);
	*/

	// strlen(command) - 1 PARA NO IMPRIMIR EL '\n' DEL FINAL
	if (write(1, command, strlen(command) - 1) == -1)
		return (1);
	return (0);
}
