#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define INPUT "echo '' hola'que   tal' estas 'yo bien'y    tu''que'tal'estas.  "

/*
**	START LIBFT
*/

size_t	ft_strlen(const char *str)
{
    size_t count;

    count = 0;
    while(*str)
    {
        str++;
        count++;
    }
    return (count);
}

char	*ft_strdup(const char *c)
{
    char	*newc;
    int		count;
    int		len;

    len = ft_strlen(c);
    newc = malloc(sizeof(char) * (len + 1));
    if (!newc)
        return (NULL);
    count = 0;
    while (c[count])
    {
        newc[count] = c[count];
        count++;
    }
    newc[count] = '\0';
    return (newc);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t	count;
    char	*dst;

    if (!s)
        return (NULL);
    if (ft_strlen(s) < start)
        return (ft_strdup(""));
    if (ft_strlen(s + start) < len)
        len = ft_strlen(s + start);
    dst = malloc(sizeof(char) * (len + 1));
    if (dst == NULL)
        return (NULL);
    count = 0;
    while (count < len)
    {
        *(dst + count) = *(s + count + start);
        count++;
    }
    *(dst + count) = '\0';
    return (dst);
}

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	char			*p;

	i = 0;
	p = s;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	p = (void *)malloc(size * count);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, count * size);
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	if (!(p = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char))))
		return (NULL);
	j = 0;
	i = 0;
	while (j < ft_strlen(s1))
	{
		p[i] = (char)s1[j];
		j++;
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
	{
		p[i] = (char)s2[j];
		i++;
		j++;
	}
	p[i] = '\0';
	return (p);
}

/*
**	END LIBFT
*/

int		append_word(char **dst, char const *word,
char const *input, size_t const counter)
{
	int		add_space;
	char	*aux;

	add_space = 1;
	if (((input[counter] == '"' || input[counter] == '\'')
			&& input[counter - 1] != ' ')
			|| input[counter - 1] == '"' || input[counter - 1] == '\'')
		add_space = 0;
	if (*dst)
	{
		if (add_space)
			aux = ft_strjoin(*dst, " ");
		else
			aux = ft_strdup(*dst);
		if (!aux)
			return (1);
		free(*dst);
		if (!word)
			*dst = ft_strdup(aux);
		else
			*dst = ft_strjoin(aux, word);
		if (!(*dst))
		{
			free(aux);
			return (1);
		}
		free(aux);
	}
	else
		*dst = ft_strdup(word);
	return (0);
}

size_t	get_len(char const *input)
{
	size_t	len;

	len = 0;
	if (input[len] == '"')
	{
		++len;
		while (input[len] != '"')
			++len;
		++len;
	}
	else if (input[len] == '\'')
	{
		++len;
		while (input[len] != '\'')
			++len;
		++len;
	}
	else
	{
		while (input[len] && input[len] != ' '
		&& input[len] != '"' && input[len] != '\'')
			++len;
	}	
	return (len);
}

char	*xtra_space_rem(char const *input)
{
	char	*result;
	char	*word;
	size_t	len;
	size_t	i;

	result = 0;
	word = 0;
	len = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] != ' ')
		{
			len = get_len(&input[i]);
			if ((input[i] == '"' || input[i] == '\'') && len > 2)
			{
				word = ft_substr(&input[i + 1], 0, len - 2);
				if (!word)
				{
					if (result)
						free(result);
					return (0);
				}
			}
			else if (input[i] != '"' && input[i] != '\'')
			{
				word = ft_substr(&input[i], 0, len);
				if (!word)
				{
					if (result)
						free(result);
					return (0);
				}
			}
			/*if ((len && input[i] != '"' && input[i] != '\'')
					|| (len > 2 && (input[i] == '"'
					|| input[i] == '\'')))
			{*/
				//JOIN TO RESULT
				if (append_word(&result, word, input, i))
				{
					if (result)
							free(result);
					free(word);
					return (0);
				}
			//}
			free(word);
			word = 0;
			i += len - 1;
		}
		++i;
	}
	return (result);
}

int	main(void)
{
	char	*final_input;

	final_input = xtra_space_rem(INPUT);
	if (!final_input)
	{
		printf("Error\n");
		return (1);
	}
	printf("final_input: %s\n", final_input);
	free(final_input);
	return (0);
}
