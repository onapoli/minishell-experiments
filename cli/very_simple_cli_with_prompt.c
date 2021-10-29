#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
**	PRUEBA PEQUEÑA CLI CON PROMPT FIJA. LOS DATOS DEL PROMPT SE PODRÍAN
**	CONSEGUIR CON LA INFO OBTENIDA DE LA FUNCIÓN getenv.
*/

#define BUFF_SIZE 10
#define PROMPT "my_prompt:$ "

static void	clean_exit(char **command, char **buffer, int exit_status)
{
	free(*buffer);
	free(*command);
	exit(exit_status);
}

static void	init_session(char **command, size_t *read_result,
size_t *len_command)
{
	*command = strdup("");
	*read_result = 0;
	*len_command = 0;
	write(1, PROMPT, 12);
	return ;
}

static void	output(char **command)
{
	printf("reply: %s", *command);
	free(*command);
	*command = strdup("");
	write(1, PROMPT, 12);
}

static char	*ft_strjoin(char const *s1, char const *s2, size_t *len_join)
{
    int     i;
    char    *result;

    if (!(result = malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1))))
        return (0);
    i = 0;
    while (s1[i])
	{
        result[i] = s1[i];
		i++;
	}
    while (*s2)
	{
        result[i++] = *(s2)++;
	}
	result[i] = 0;
	*len_join = i;
    return (result);
}

static int	update_command(char **command, char *buffer, size_t *len_command)
{
	char	*aux;
	
	if (!(aux = ft_strjoin(*command, buffer, len_command)))
		return (1);
	free(*command);
	*command = strdup(aux);
	free(aux);
	return (0);
}

int			main(void)
{
	char	*buffer;
	char	*command;
	size_t	read_result;
	size_t	len_command;

	if (!(buffer = malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (1);
	init_session(&command, &read_result, &len_command);
	while ((read_result = read(0, buffer, BUFF_SIZE)) != (size_t)-1)
	{
		buffer[read_result] = 0;
		if (read_result == 0)
		{
			write(1, "END\n", 4);
			clean_exit(&command, &buffer, 0);
		}
		if (update_command(&command, buffer, &len_command))
			clean_exit(&command, &buffer, 1);
		if (/*read_result < BUFF_SIZE || */command[len_command - 1] == '\n')
			output(&command);
	}
	clean_exit(&command, &buffer, 1);
}
