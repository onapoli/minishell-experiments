#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
**	INIT UTILS
*/

//busca la comilla simple fuera de las comillas dobles
size_t single_quote(const char *str, size_t *inc)
{
    size_t count_queue;

    count_queue = 0;
    while (count_queue < 2 && str[*inc])
    {
        if (str[*inc] == '\'')
            ++count_queue;
        *inc = *inc + 1;
    }
    *inc = *inc - 1;
    return (count_queue);
}
//busca la primera comilla hasta cerrar cuenta como caracter la comilla simple
size_t double_quote(char *str, size_t *inc)
{
    size_t count_queue;

    count_queue = 0;
    while (count_queue < 2 && str[*inc])
    {
        if (str[*inc] == '"' && str[*inc - 1] != '\\')
            ++count_queue;
        *inc = *inc + 1;
    }
    *inc = *inc - 1;
    return (count_queue);
}

char	*mod_strchr(const char *s, int c)
{
	size_t	count;

    count = 0;
    while (*(s + count))
    {
		if (s[count] == '"')
            double_quote((char *)s, &count);
        else if (s[count] == '\'')
            single_quote((char *)s, &count);
        else if (*(char *)(s + count) == (char)c)
            return ((char *)(s + count));
        count++;
    }
    if ((char)c == 0)
        return ((char *)(s + count));
    return (0);
}

/*
**	END UTILS
*/

static void	skip_spaces(char const *input, size_t *len)
{
	while (input[*len] == ' ')
		*len += 1;
}

/*
**	DEVUELVE 0 SI LA REDIRECCIÓN TIENE UN FILENAME, 1 SI NO LO TIENE.
*/

static int	check_empty(char const *input)
{
	size_t	i;

	i = 0;
	skip_spaces(input, &i);
	if (!input[i])
		return (1);
	if (input[i] && input[i] != ' ' && input[i] != '|' && input[i] != '\n')
		return (0);
	return (1);
}

static int	pipe_validator(char const *input)
{
	char	*target;

	target = (char *)input;
	if (*target == '|')
		return (1);
	while (1)
	{
		target = mod_strchr(target, '|');
		if (!target)
			return (0);
		++target;
		if (check_empty(target))
			return (1);
	}
}

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("Prompt:$ ");
		//CTRL+D
		if (!input)
		{
			free(input);
			break ;
		}
		if (*input)
		{
			if (pipe_validator(input))
				printf("INVALID PIPE/S\n");
			else
			{
				printf("VALID PIPE/S\n");
				add_history(input);
			}
		}
		free(input);
	}
	return (0);
}
