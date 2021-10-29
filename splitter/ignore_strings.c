#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPUT "hello -m 'This is not valid ; separator' But this is; bye bye"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (s == NULL)
		return (NULL);
	if (start > strlen(s))
		return (strdup(""));
	p = calloc(len + 1, sizeof(char));
	if (p == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (j < len && s[i] != '\0')
	{
		p[j] = (char)s[i];
		i++;
		j++;
	}
	return (p);
}

void	quote_manager(char const c, int *singleq_open, int *doubleq_open)
{
	if (c == '\'' && !(*singleq_open) && !(*doubleq_open))
		*singleq_open = 1;
	else if (c == '\'' && (*singleq_open))
		*singleq_open = 0;
	else if (c == '"' && !(*doubleq_open) && !(*singleq_open))
		*doubleq_open = 1;
	else if (c == '"' && *doubleq_open)
		*doubleq_open = 0;
}

int	comm_counter(char const *input, char divider)
{
	int		commands;
	int		singleq_open;
	int		doubleq_open;

	/*if (!(*input))
		return (0);*/
	commands = 1;
	singleq_open = 0;
	doubleq_open = 0;
	while (*input)
	{
		quote_manager(*input, &singleq_open, &doubleq_open);
		if (*input == divider && !singleq_open && !doubleq_open)
			commands++;
		input++;
	}	
	return (commands);
}

int	pos_first_divider(char const *input, char const divider)
{
	int	singleq_open;
	int	doubleq_open;
	int	i;

	singleq_open = 0;
	doubleq_open = 0;
	i = 0;
	while (input[i])
	{
		quote_manager(input[i], &singleq_open, &doubleq_open);
		if (input[i] == divider && !singleq_open && !doubleq_open)
			return (i);
		i++;
	}
	return (i);
}

char	**comm_splitter(char const *input, char const divider)
{
	char	**res;
	int		commands;
	int		command_len;
	int		i;

	commands = comm_counter(input, divider);
	res = malloc(sizeof(char *) * (commands + 1));
	if (!res)
		return (0);
	res[commands] = 0;
	i = 0;
	while (i < commands)
	{
		command_len = pos_first_divider(input, divider);
		res[i] = ft_substr(input, 0, command_len);
		input += command_len + 1;
		i++;
	}
	return (res);
}

int	main(void)
{
	char	**commands;
	int		i;

	commands = comm_splitter(INPUT, ';');
	i = 0;
	while (commands[i])
	{
		printf("%s\n", commands[i]);
		free(commands[i]);
		i++;
	}
	free(commands);
	return (0);
}
