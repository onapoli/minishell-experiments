#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int		swap_s(char **a, char **b)
{
	char	*aux;

	if (!*a || !*b)
		return (1);
	aux = strdup(*a);
	free(*a);
	*a = strdup(*b);
	free(*b);
	*b = strdup(aux);
	free(aux);
	return (0);
}

static int		ordered_fill(char const **src, char **dst)
{
	size_t	i;
	size_t	j;

	if (!src || !dst || !src[0])
		return (0);
	i = 0;
	j = 0;
	while (src[i])
	{
		dst[i] = strdup(src[i]);
		j = i;
		while (j > 0
		&& strncmp(dst[j], dst[j - 1], max_len(dst[j], dst[j - 1])) < 0)
		{
			if (swap_s(&dst[j], &dst[j - 1]))
				return (1);
			--j;
		}
		++i;
	}
	return (0);
}

static int		create_array(char ***res, size_t len)
{
	*res = malloc(sizeof(char *) * len + 1);
	if (!(*res))
		return (1);
	(*res)[len] = 0;
	return (0);
}

char			**insertion_sort(char const **a, size_t len)
{
	char	**res;

	if (create_array(&res, len))
		return (0);
	if (ordered_fill(a, res))
		return (0);
	return (res);
}

int	main()
{
	char const	*a[] = {"hola", "ciao", "hello", "alo", "salut", '\0'};
	char		**sorted_a;
	size_t		i;

	sorted_a = insertion_sort(a, LEN);
	if (!sorted_a)
		return (1);
	i = 0;
	while (sorted_a[i])
	{
		printf("%s\n", sorted_a[i]);
		free(sorted_a[i]);
		++i;
	}
	free(sorted_a);
	return (0);
}
