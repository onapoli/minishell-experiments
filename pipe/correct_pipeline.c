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
		if (WEXITSTATUS(status))
		{
			printf("Child process encountered an error.\n");
			return (1);
		}
		++i;
	}
	return (0);
}

void	child_func(int rd[2], int wt[2])
{
	char	*ls_args[2] = {"ls", 0};
	//char	*grep_args[3] = {"grep", "a", 0};
	//char	*wc_args[2] = {"wc", 0};
	char	*cat_args[2] = {"cat", 0};

	if (rd[0] != -1)
	{
		//close(rd[1]);
		dup2(rd[0], STDIN_FILENO);
		close(rd[0]);
		if (wt[1] != -1)
		{
			close(wt[0]);
			dup2(wt[1], STDOUT_FILENO);
			close(wt[1]);
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
	if (wt[1] != -1)
	{
		close(wt[0]);
		dup2(wt[1], STDOUT_FILENO);
		close(wt[1]);
		//execve("/bin/ls", ls_args, NULL);
		execve("/bin/cat", cat_args, NULL);
		perror("ls");
		exit(EXIT_FAILURE);
	}
}

int	launch_process(int rd[2], int wt[2])
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

	b_pipefd[0] = -1;
	b_pipefd[1] = -1;
	if (pipe(a_pipefd) == -1)
		return (1);
	if (launch_process(b_pipefd, a_pipefd))
		return (1);
	close(a_pipefd[1]);
	if (pipe(b_pipefd) == -1)
		return (1);
	if (launch_process(a_pipefd, b_pipefd))
		return (1);
	close(a_pipefd[0]);
	close(b_pipefd[1]);
	a_pipefd[0] = -1;
	a_pipefd[1] = -1;
	if (launch_process(b_pipefd, a_pipefd))
		return (1);
	close(b_pipefd[0]);
	if (waiter())
		return (1);
	return (0);
}
