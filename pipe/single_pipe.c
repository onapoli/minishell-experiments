#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int	launch_process(int pipefd[2], int numchild)
{
	pid_t	child;
	char	c;
	int		ret;

	child = fork();
	if (child == -1)
		return (1);
	if (child == 0)
	{
		if (numchild == 1)
		{
			/*if (write(pipefd[1], "adios\n", 6) == -1)
			{
				printf("Write pipe is closed.\n");
				exit(EXIT_FAILURE);
			}*/
			while (1)
			{
				ret = read(pipefd[0], &c, 1);
				if (ret == -1)
					exit(EXIT_FAILURE);
				if (ret == 0)
					break ;
				putchar(c);
			}
			close(pipefd[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(pipefd[0]);
			if (write(pipefd[1], "hey\n", 4) == -1)
			{
				printf("Could not write to pipe.\n");
				exit(EXIT_FAILURE);
			}
			close(pipefd[1]);
			exit(EXIT_SUCCESS);
		}
	}
	else
		return (0);
}

int	main(void)
{
	int	pipefd[2];
	int	num_child;
	int	status;

	if (pipe(pipefd) == -1)
		return (1);
	num_child = 2;
	while (num_child)
	{
		if (num_child == 1)
			close(pipefd[1]);
		if (launch_process(pipefd, num_child))
			return (1);
		if (wait(&status) == -1)
		{
			printf("Wait failed.\n");
			return (1);
		}
		if (WEXITSTATUS(status))
		{
			printf("Child process encountered an error.\n");
			return (1);
		}
		num_child--;
	}
	close(pipefd[0]);
	return (0);
}
