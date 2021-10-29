#ifndef DEMO_H
# define DEMO_H

# include <stdio.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>

# define BUFF_SIZE 4096

int		ft_fgetc(FILE *stream);
char	*ft_fgets(char *s, int size, FILE *stream);


#endif
