#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct s_pipe
{
	int	a_pipefd[2];
	int	b_pipefd[2];
	int	mode; // 1: SÓLO LEER; 2: SÓLO ESCRIBIR; 3: LEER Y ESCRIBIR
	int order; // 0: PRIMERA ACCIÓN EN PIPE a_pipefd; 1: PRIMERA ACCIÓN EN PIPE b_pipefd.
}				t_pipe;

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

void	child_func(t_pipe pipes)
{
	char	c;
	int		ret;

	if (pipes.mode == 1) //SÓLO LEER DEL PIPE
	{
		if (pipes.order == 0)
		{
			close(pipes.b_pipefd[0]);
			close(pipes.b_pipefd[1]);
			close(pipes.a_pipefd[1]);
		}
		else
		{
			close(pipes.a_pipefd[0]);
			close(pipes.a_pipefd[1]);
			close(pipes.b_pipefd[1]);
		}
		while (1)
		{
			if (pipes.order == 0)
				ret = read(pipes.a_pipefd[0], &c, 1);
			else
				ret = read(pipes.b_pipefd[0], &c, 1);
			if (ret == -1)
				exit(EXIT_FAILURE);
			if (ret == 0)
				break ;
			putchar(c);
		}
		if (pipes.order == 0)
			close(pipes.a_pipefd[0]);
		else
			close(pipes.b_pipefd[0]);
		exit(EXIT_SUCCESS);
	}
	if (pipes.mode == 2) //SÓLO ESCRIBIR EN EL PIPE
	{
		if (pipes.order == 0)
		{
			close(pipes.b_pipefd[0]);
			close(pipes.b_pipefd[1]);
			close(pipes.a_pipefd[0]);
		}
		else
		{
			close(pipes.a_pipefd[0]);
			close(pipes.a_pipefd[1]);
			close(pipes.b_pipefd[0]);
		}
		if (pipes.order == 0)
		{
			if (write(pipes.a_pipefd[1], "hola\n", 5) == -1)
			{
				printf("Could not write to first pipe.\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			if (write(pipes.b_pipefd[1], "hola\n", 5) == -1)
			{
				printf("Could not write to first pipe.\n");
				exit(EXIT_FAILURE);
			}
		}
		if (pipes.order == 0)
			close(pipes.a_pipefd[1]);
		else
			close(pipes.b_pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	// LEER Y ESCRIBIR EN EL PIPE
	if (pipes.order == 0)
	{
		close(pipes.a_pipefd[1]);
		close(pipes.b_pipefd[0]);
	}
	else
	{
		close(pipes.b_pipefd[1]);
		close(pipes.a_pipefd[0]);
	}
	if (pipes.order == 0)
	{
		while (1)
		{
			ret = read(pipes.a_pipefd[0], &c, 1);
			if (ret == -1)
				exit(EXIT_FAILURE);
			if (ret == 0)
				break ;
			if (write(pipes.b_pipefd[1], &c, 1) == -1)
			{
				printf("Could not write to second pipe.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		while (1)
		{
			ret = read(pipes.b_pipefd[0], &c, 1);
			if (ret == -1)
				exit(EXIT_FAILURE);
			if (ret == 0)
				break ;
			if (write(pipes.a_pipefd[1], &c, 1) == -1)
			{
				printf("Could not write to second pipe.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	if (pipes.order == 0)
	{
		close(pipes.a_pipefd[0]);
		close(pipes.b_pipefd[1]);
	}
	else
	{
		close(pipes.b_pipefd[0]);
		close(pipes.a_pipefd[1]);
	}
	exit(EXIT_SUCCESS);
}

int	launch_process(t_pipe pipes)
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return (1);
	if (child == 0)
		child_func(pipes);
	return (0);
}

int	main(void)
{
	t_pipe	pipes;

	if (pipe(pipes.a_pipefd) == -1)
		return (1);
	if (pipe(pipes.b_pipefd) == -1)
		return (1);
	pipes.mode = 2;
	pipes.order = 0;
	if (launch_process(pipes))
		return (1);
	if (waiter())
		return (1);
	close(pipes.a_pipefd[1]);
	pipes.mode = 3;
	pipes.order = 0;
	if (launch_process(pipes))
		return (1);
	if (waiter())
		return (1);
	close(pipes.a_pipefd[0]);
	close(pipes.b_pipefd[1]);
	pipes.mode = 1;
	pipes.order = 1;
	if (launch_process(pipes))
		return (1);
	if (waiter())
		return (1);
	close(pipes.b_pipefd[0]);
	return (0);
}
