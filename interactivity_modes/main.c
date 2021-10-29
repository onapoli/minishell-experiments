#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
**	BUSCAR ARCHIVO EN CURRENT DIRECTORY, Y SI NO EXISTE,
**	BUSCAR EN TODOS LOS DIRECTORIOS DE $PATH.
*/

static int	get_fd_script(char const *name, int *fd_script)
{
	return (0);
}

int	non_interact_reader(char const *script_name)
{
	int	fd_script;

	if (get_fd_script(script_name, &fd_script))
		return (1);
	return (0);
}

int	interact_reader()
{
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		printf("Too many arguments");
		return (1);
	}
	if (argc == 2)
	{
		//NON-INTERACTIVE MODE
		if (non_interact_reader(argv[1]))
			return (1);
	}
	else
	{
		//INTERACTIVE MODE
		if (interact_reader())
			return (1);
	}
	return (0);
}