#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int	waiter()
{
	int status;

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
	return (0);
}

void	child_func(int rd, int wt)
{
	char	c;
	int		ret;

	if (rd != -1)
	{
		while (1)
		{
			ret = read(rd, &c, 1);
			if (ret == -1)
				exit(EXIT_FAILURE);
			if (ret == 0)
				break ;
			if (wt != -1)
			{
				if (write(wt, &c, 1) == -1)
				{
					printf("Could not write to second pipe.\n");
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				if (putchar(c) == EOF)
					exit(EXIT_FAILURE);
			}
		}
		close(rd);
		if (wt != -1)
			close(wt);
		exit(EXIT_SUCCESS);
	}
	if (wt != -1)
	{
		if (write(wt, "hola\n", 5) == -1)
		{
			printf("Could not write to first pipe.\n");
			exit(EXIT_FAILURE);
		}
		close(wt);
		exit(EXIT_SUCCESS);
	}
	if (puts("adios") == EOF)
	{
		printf("Could not write to no pipe.\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

int	launch_process(int rd, int wt)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return (1);
	if (child == 0)
	{
		child_func(rd, wt);
	}
	return (0);
}

int	main(void)
{
	int	a_pipefd[2];
	int	b_pipefd[2];

	if (pipe(a_pipefd) == -1)
		return (1);
	if (launch_process(-1, a_pipefd[1]))
		return (1);
	if (waiter())
		return (1);
	close(a_pipefd[1]);
	if (pipe(b_pipefd) == -1)
		return (1);
	if (launch_process(a_pipefd[0], b_pipefd[1]))
		return (1);
	if (waiter())
		return (1);
	close(a_pipefd[0]);
	close(b_pipefd[1]);
	if (launch_process(b_pipefd[0], -1))
		return (1);
	if (waiter())
		return (1);
	close(b_pipefd[0]);
	return (0);
}
