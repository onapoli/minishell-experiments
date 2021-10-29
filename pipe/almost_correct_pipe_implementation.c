#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int	waiter()
{
	int		status;
	size_t	i;

	i = 0;
	while (i < 3)
	{
		if (wait(&status) == -1)
		{
			printf("Wait failed.\n");
			return (1);
		}
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status))
				printf("Child process encountered an error.\n");
		}
		if (WIFSTOPPED(status))
			printf("Child stopped.");
		if (WIFSIGNALED(status))
			printf("Child interrrupted by signal %d\n", WTERMSIG(status));
		++i;
	}
	return (0);
}

void	child_func(int rd, int wt)
{
	char	*ls_args[2] = {"ls", 0};
	//char	*grep_args[3] = {"grep", "a", 0};
	//char	*wc_args[2] = {"wc", 0};
	char	*cat_args[2] = {"cat", 0};

	if (rd != -1)
	{
		dup2(rd, STDIN_FILENO);
		close(rd);
		if (wt != -1)
		{
			dup2(wt, STDOUT_FILENO);
			close(wt);
			//execve("/bin/grep", grep_args, NULL);
			execve("/bin/cat", cat_args, NULL);
			perror("grep");
		}
		else
		{
			//execve("/usr/bin/wc", wc_args, NULL);
			execve("/bin/ls", ls_args, NULL);
			perror("wc");
		}
		exit(EXIT_FAILURE);
	}
	if (wt != -1)
	{
		dup2(wt, STDOUT_FILENO);
		close(wt);
		//execve("/bin/ls", ls_args, NULL);
		execve("/bin/cat", cat_args, NULL);
		perror("ls");
		exit(EXIT_FAILURE);
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
	close(a_pipefd[1]);
	if (pipe(b_pipefd) == -1)
		return (1);
	if (launch_process(a_pipefd[0], b_pipefd[1]))
		return (1);
	close(a_pipefd[0]);
	close(b_pipefd[1]);
	if (launch_process(b_pipefd[0], -1))
		return (1);
	close(b_pipefd[0]);
	if (waiter())
		return (1);
	return (0);
}
