#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define REMOVE "ciao"
#define LEN 5

static size_t	max_len(char const *a, char const *b)
{
	size_t	len_a;
	size_t	len_b;

	if (a)
		len_a = strlen(a);
	else
		len_a = 0;
	if (b)	
		len_b = strlen(b);
	else
		len_b = 0;
	if (len_a > len_b)
		return (len_a);
	return (len_b);
}

static	void	reorder_arr(char **a)
{
	size_t	i;

	i = 1;
	while (a[i])
	{
		a[i - 1] = strdup(a[i]);
		free(a[i]);
		a[i] = 0;
		++i;
	}
	return ;
}

int	remove_str_arr_elem(char **a, char *rem)
{
	size_t	i;

	if (!a || !rem)
		return (1);
	i = 0;
	while (a[i])
	{
		if (strncmp(rem, a[i], max_len(rem, a[i])) == 0)
		{
			free(a[i]);
			a[i] = 0;
			reorder_arr(&a[i]);
			break ;
		}
		++i;
	}	
	return (0);
}

int	main()
{
	char	*ref[] = {"hola", "hello", "ciao", "alo", "salut", '\0'};
	char	**a;
	size_t	i;
	
	a = malloc(sizeof(char *) * LEN + 1);
	if (!a)
		return (1);
	a[LEN] = 0;
	i = 0;
	while (ref[i])
	{
		a[i] = strdup(ref[i]);
		++i;
	}
	if (remove_str_arr_elem(a, REMOVE))
		return (1);
	i = 0;
	while (a[i])
	{
		printf("%s\n", a[i]);
		free(a[i]);
		++i;
	}
	free(a);
	return (0);
}
