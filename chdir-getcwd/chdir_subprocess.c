#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
**	EL WORKING DIRECTORY ESTÁ LIGADO AL PROCESO DESDE EL QUE SE MODIFICA SU
**	VALOR.
**	LOS SUBPROCESOS HEREDAN EL WORKING DIRECTORY DEL PROCESO "PADRE". LAS
**	MODIFICACIONES QUE SE HAGAN EN EL WORKING DIRECTORY DE UN SUBPROCESO
**	SE APLICAN SÓLO AL WORKING DIRECTORY DE ESE SUBPROCESO, Y NO AFECTA
**	AL DEL PROCESO "PADRE".
*/

#define PATH_BIN "/bin"
#define START_SIZE_WDIR 255

static void	clean_exit(char **wdir, size_t exit_code)
{
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

static int	prnt_wdir(char *wdir, size_t wdir_size)
{
	if (!getcwd(wdir, wdir_size))
	{
		printf("Could not get current working directory.\n");
		printf("errno: %s\n", strerror(errno));
		return (1);
	}
	printf("CURRENT WORKING DIRECTORY IN PROCESS: %s\n", wdir);
	return (0);
}

static int	child_process(pid_t pchild, char **wdir, size_t wdir_size)
{
	if ((pchild = fork()) == -1)
	{
		printf("Process creation failed.\n");
		return (1);
	}
	if (pchild == 0)
	{
		if (chdir(PATH_BIN))
		{
			printf("Invalid path for chdir.\n");
			printf("errno: %s\n", strerror(errno));
			exit (EXIT_FAILURE);
		}
		if (prnt_wdir(*wdir, wdir_size))
			exit (EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else
		return (0);
}

int			main(void)
{
	pid_t	pchild;
	int		status;
	char	*wdir;
	size_t	wdir_size;

	pchild = 0;
	status = 0;
	wdir_size = START_SIZE_WDIR;
	if (char_malloc(&wdir, wdir_size))
		return (1);
	if (prnt_wdir(wdir, wdir_size))
		clean_exit(&wdir, EXIT_FAILURE);
	child_process(pchild, &wdir, wdir_size);
	if (wait(&status) == -1)
	{
		printf("Wait failed.\n");
		clean_exit(&wdir, EXIT_FAILURE);
	}
	if (WEXITSTATUS(status))
	{
		printf("Child process encountered an error.\n");
		clean_exit(&wdir, EXIT_FAILURE);
	}
	if (prnt_wdir(wdir, wdir_size))
		clean_exit(&wdir, EXIT_FAILURE);
	free(wdir);
	return (0);
}
