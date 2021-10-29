#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

extern int	*glob;

int	*glob;

void	child_interrupt_signal(int signal_type)
{
	if (signal_type == SIGINT)
		printf("child interrupt signal received!\n");
	free(glob);
	exit(2);
}

void	main_interrupt_signal(int signal_type)
{
	if (signal_type == SIGINT)
		printf("main interrupt signal received!\n");
}

int	child_waiter(pid_t const child)
{
	int	status;

	if (waitpid(child, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
	{
		printf("exited execve\n");
		if (WEXITSTATUS(status) != 0)
			return (1);
	}
	if (WIFSIGNALED(status))
		printf("execve interrupted!\n");
	return (0);
}

void	executor(char **envp)
{
	char	*args[2] = {"/usr/bin/top"};

	args[1] = 0;
	execve(args[0], args, envp);
}

int	start_processing(pid_t *child, char **envp)
{
	*child = fork();
	if (*child == -1)
		return (1);
	if (*child == 0)
	{
		executor(envp);
		exit(EXIT_FAILURE);
	}
	else
		return (0);
}

int	reader(char **envp)
{
	pid_t	child;
	char	*input;

	input = 0;
	while (1)
	{
		input = readline("Prompt:~$ ");
		if (!input)
			break ;
		if (*input)
		{
			if (start_processing(&child, envp))
				return (1);
			if (child_waiter(child))
				return (1);
		}
		free(input);
	}	
	return (0);
}

int	start_reading(pid_t *child, char **envp)
{
	*child = fork();
	if (*child == -1)
		return (1);
	if (*child == 0)
	{
		signal(SIGINT, child_interrupt_signal);
		glob = malloc(sizeof(int));
		if (!glob)
			exit(EXIT_FAILURE);
		*glob = 0;
		if (reader(envp))
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, main_interrupt_signal);
		return (0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	if (argc != 1)
		return (1);
	(void) argv;
	while (1)
	{
		child = 0;
		status = 0;
		if (start_reading(&child, envp))
			return (1);
		if (waitpid(child, &status, 0) == -1)
			return (1);
		if (WIFEXITED(status))
		{
			printf("exited reader\n");
			if (WEXITSTATUS(status) != 2)
				break ;
		}
		else if (WIFSIGNALED(status))
		{
			printf("interrupted reader\n");
			return (0);
		}
	}
	return (WEXITSTATUS(status));
}