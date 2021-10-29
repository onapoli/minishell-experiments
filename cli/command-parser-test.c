#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define COMMAND_1 "ls -la /bin;"
#define COMMAND_2 "ls -la /bin |"
#define COMMAND_3 "ls -la /bin \n"
#define COMMAND_4 "ls    a    ;"
#define COMMAND_5 "  ls    a b   c   ;"

static void		free_param_until(char ***param, size_t const i_limit)
{
	size_t	i;

	i = 0;
	while (i < i_limit)
	{
		free((*param)[i]);
		i++;
	}
	free(*param);
	return ;
}

static int		fill_param(char const *command, char **param)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (*command == ' ')
		command++;
	while (*command)
	{
		i = 0;
		while (command[i] != ' ')
			i++;
		if (!(param[j] = malloc(sizeof(char) * (i + 1))))
		{
			free_param_until(&param, j);
			return (1);
		}
		param[j][i] = 0;
		i = 0;
		while (command[i] != ' ')
		{
			param[j][i] = command[i];
			i++;
		}
		command = strchr(command, ' ');
		while (*command && *command == ' ' && command[1] == ' ')
		{
			command = strchr(command, ' ');
			command++;
		}
		command++;
		j++;
	}
	return (0);
}

static size_t	get_total_param(char const *command)
{
	size_t	i;
	size_t	j;

	i = 1;
	while ((command = strchr(command, ' ')))
	{
		j = 1;
		if (command[j] == ' ')
		{
			while (command[j] == ' ')
				j++;
			if (!command[j])
				break ;
		}
		command += j;
		i++;
	}
	return (i);
}

int				main(void)
{
	char	*command;
	char	**param;
	size_t	len;
	size_t	i;

	len = 0;
	while (COMMAND_5[len] != '\n' &&
	COMMAND_5[len] != '|' && COMMAND_5[len] != ';')
		len++;
	printf("Type: %c\n", COMMAND_5[len]);
	if (!(command = malloc(sizeof(char) * (len + 1))))
		return (1);
	i = 0;
	while (i < len)
	{
		command[i] = COMMAND_5[i];
		i++;
	}
	command[i] = 0;
	printf("Command: %s\n", command);
	len = get_total_param(command);
	if (!(param = malloc(sizeof(char *) * (len + 1))))
	{
		free(command);
		return (1);
	}
	param[len] = 0;
	if (fill_param(command, param))
	{
		free(command);
		return (1);
	}
	i = 0;
	while (param[i])
	{
		printf("param %ld: %s\n", i, param[i]);
		free(param[i]);
		i++;
	}
	free(command);
	free(param);
	return (0);
}
