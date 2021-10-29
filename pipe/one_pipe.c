#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int	launch_process(pid_t *child, int pipefd[2])
{
	unsigned char	res;

	*child = fork();
	if (*child == -1)
		return (1);
	if (*child == 0)
	{
		close(pipefd[1]);
		while (1)
		{
			if (read(pipefd[0], &res, 1) <= 0)
			{
				close(pipefd[0]);
				exit(EXIT_SUCCESS);
			}
			if (write(STDOUT_FILENO, &res, 1) == -1)
			{
				printf("Write error.");
				exit(EXIT_FAILURE);
			}
		}		
	}
	else
	{
		close(pipefd[1]);
		close(pipefd[0]);
		return (0);
	}
}

int	main(void)
{
	int		pipefd[2];
	pid_t	child;
	int		status;

	if (pipe(pipefd) == -1)
		return (1);
	child = 0;
	status = 0;
	write(pipefd[1], "hola\n", 5);
	if (launch_process(&child, pipefd))
		return (1);
	if (waitpid(child, &status, 0) == -1)
		return (1);
	if (WEXITSTATUS(status))
	{
		printf("Child process encountered an error.\n");
		return (1);
	}
	return (0);
}