#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int	get_script_fd(char *filename)
{
	int	fd;

	fd = -1;
	if (access(filename, X_OK))
	{
		perror("exec permission");
		return (1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror("open");
	return (fd);
}

int	script_reader(char *filename)
{
	int		script_fd;
	//char	*input;

	script_fd = get_script_fd(filename);
	if (script_fd == -1)
		return (1);
	/*while (1)
	{
		input = get_next_line();
		if (input == -1)
			return (1);
		if (!input)
			return (0);
		if (divider(input))
			return (1);
		free(input);
	}*/
	close(script_fd);
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 2)
	{
		if (script_reader(argv[1]))
			return (1);
	}
	else
		return (1);
	return (0);
}