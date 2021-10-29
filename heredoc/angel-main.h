#ifndef MAIN_H

# define MAIN_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

#define BUFF_SIZE 4096

# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define RES "\e[0m"

size_t  ft_strlen(const char *str);
size_t  ft_strlen_debug(const char *str);
char    *ft_strchr(const char *s, int c);
void	ft_putstr_fd(char *s, int fd);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *c);
int		ft_isalnum(int c);

#endif
