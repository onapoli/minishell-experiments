#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
**	NO PARECE QUE ESTÉ MUY CLARO CUÁL ES EL LÍMITE DE CARACTERES EN UN PATH
**	DE UNIX/LINUX. EN ALGUNOS CASOS SE DICE QUE ES 4096, PERO NO ES SEGURO.
**	EN LOS FILENAMES EL LÍMITE PARECE QUE ESTÁ FIJADO EN 255, PERO NO SÉ SI
**	ES SEGURO.
*/

/*
**	PRUEBA DE wdir DINÁMICO, AMPLIÁNDO SU MEMORIA ASIGNADA CUANDO EL LENGTH
**	DEL PATH ES MAYOR QUE LA CAPACIDAD DE wdir.
*/

#define PATH_BIN "/etc/networkd-dispatcher"
#define START_SIZE_WDIR 10

static void	clean_exit(char **wdir, size_t exit_code)
{
	if (*wdir)
		free(*wdir);
	exit(exit_code);
}

static int	char_malloc(char **target, size_t target_size)
{
	if (!(*target = malloc(sizeof(char) * (target_size + 1))))
	{
		printf("Not enough memory for char_malloc.\n");
		return (1);
	}
	return (0);
}

static int	prnt_wdir(char **wdir, size_t *wdir_size)
{
	while (!getcwd(*wdir, *wdir_size))
	{
		if (errno == ERANGE)
		{
			free(*wdir);
			*wdir = 0;
			*wdir_size += 10;
			if (char_malloc(wdir, *wdir_size))
				return (1);
			printf("Increased wdir length.\n");
		}
		else
		{
			printf("Could not get current working directory.\n");
			printf("errno: %s\n", strerror(errno));
			return (1);
		}
	}
	printf("CURRENT WORKING DIRECTORY IN MAIN PROCESS: %s\n", *wdir);
	return (0);
}

int			main(void)
{
	char	*wdir;
	size_t	wdir_size;

	wdir_size = START_SIZE_WDIR;
	if (char_malloc(&wdir, wdir_size))
		return (1);
	if (prnt_wdir(&wdir, &wdir_size))
		clean_exit(&wdir, EXIT_FAILURE);
	if (chdir(PATH_BIN))
	{
		printf("Invalid path for chdir.\n");
		printf("errno: %s\n", strerror(errno));
		clean_exit(&wdir, EXIT_FAILURE);
	}
	if (prnt_wdir(&wdir, &wdir_size))
		clean_exit(&wdir, EXIT_FAILURE);
	free(wdir);
	return (0);
}
