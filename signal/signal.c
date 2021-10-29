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

void	main_interrupt_signal(int signal_type)
{
	if (signal_type == SIGINT)
		printf("main interrupt signal received!");
	printf("%d\n", *glob);
	return ;
}

void	child_interrupt_signal(int signal_type)
{
	if (signal_type == SIGINT)
		printf("child interrupt signal received!");
	printf("%d\n", *glob);
	free(glob);
	exit(2);
}

int	reader(void)
{
	char	*input;

	input = 0;
	while (1)
	{
		input = readline("Prompt:~$ ");
		if (!input)
			break ;
		if (*input)
		{}
		free(input);
	}	
	return (0);
}

int	start_reading(pid_t *child)
{
	*child = fork();
	if (*child == -1)
		return (1);
	if (*child == 0)
	{
		signal(SIGINT, child_interrupt_signal);
		if (reader())
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else
	{
		*glob = 1;
		write(STDOUT_FILENO, "hi\n", 3);
		return (0);
	}
	return (0);
}

int	main(void)
{
	pid_t	child;
	int		status;

	while (1)
	{
		glob = malloc(sizeof(int));
		*glob = 0;
		child = 0;
		status = 0;
		if (start_reading(&child))
			return (1);
		signal(SIGINT, main_interrupt_signal);
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
			printf("interrupted reader");
		}
		free(glob);
	}
	free(glob);
	return (WEXITSTATUS(status));
}