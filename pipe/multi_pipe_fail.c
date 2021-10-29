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

void	child_func(int pipefd[2], int mode)
{
	char	c;
	int		ret;

	if (mode == 1) //SÓLO LEER DEL PIPE
	{
		close(pipefd[1]);
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
	if (mode == 2) //SÓLO ESCRIBIR EN EL PIPE
	{
		close(pipefd[0]);
		if (write(pipefd[1], "hola\n", 5) == -1)
		{
			printf("Could not write to first pipe.\n");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	// LEER Y ESCRIBIR EN EL PIPE
	while (1)
	{
		ret = read(pipefd[0], &c, 1);
		if (ret == -1)
			exit(EXIT_FAILURE);
		if (ret == 0)
			break ;
		if (write(pipefd[1], &c, 1) == -1)
		{
			printf("Could not write to second pipe.\n");
			exit(EXIT_FAILURE);
		}
	}
	close(pipefd[0]);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

int	launch_process(int pipefd[2], int mode)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return (1);
	if (child == 0)
	{
		child_func(pipefd, mode);
	}
	return (0);
}

int	main(void)
{
	int	a_pipefd[2];
	//int	b_pipefd[2];
	//int	mode;

	if (pipe(a_pipefd) == -1)
		return (1);
	if (launch_process(a_pipefd, 2))
		return (1);
	if (waiter())
		return (1);
	if (launch_process(a_pipefd, 3))
		return (1);
	if (waiter())
		return (1);
	if (launch_process(a_pipefd, 1))
		return (1);
	if (waiter())
		return (1);
	return (0);
}
