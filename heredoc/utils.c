# include "angel-main.h"

size_t ft_strlen(const char *str)
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

size_t ft_strlen_debug(const char *str)
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


void	ft_putstr_fd(char *s, int fd)
{
    if (s != NULL)
        write(fd, s, ft_strlen_debug(s));
}

char	*ft_strchr(const char *s, int c)
{
    size_t	count;

    count = 0;
    while (*(s + count))
    {
        if (*(char *)(s + count) == (char)c)
            return ((char *)(s + count));
        count++;
    }
    if ((char)c == 0)
        return ((char *)(s + count));
    return (0);
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

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9'))
	{
		return (1);
	}
	return (0);
}
