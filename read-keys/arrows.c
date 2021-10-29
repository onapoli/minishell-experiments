#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <termcap.h>

/*
**	PRUEBA DE LECTURA DE CÓDIGO TECLA FLECHA ARRIBA.
**	CUANDO EL USUARIO PULSE LA TECLA DE LA FLECHA ARRIBA Y LUEGO PULSE
**	ENTER, LA FUNCIÓN READ RECIBIRÁ EL STRING QUE CONTIENE LA VARIABLE
**	arrow_up.
**	CUANDO LA VARIABLE input CONTENGA EL STRING QUE CONTIENE arrow_up
**	EL PROGRAMA IMPRIMIRÁ "bingo".
**
**	SE NECESITA TENER INSTALADO EL PAQUETE libncurses-dev
**	
**	COMPILAR CON -ltermcap Ó -ltermlib
*/

#define BUFF_SIZE 10

static char	*ft_strjoin(char *s1, char const *s2)
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
	free(s1);
    return (result);
}

static int	start_termcap(char **arrow_up, char **arrow_down)
{
	char	*term_description;
	int		read_result;
	int		width;
	char	*start;

	term_description = 0;
	read_result = tgetent(term_description, getenv("TERM"));
	if (!read_result || read_result == -1)
		return (1);
	if ((width = tgetnum ("co")) == -1)
		return (1);
	printf("terminal width: %d\n", width);
	if (!(start = tgetstr("ks", NULL)))
		return (1);
	// ESTO ES MUY IMPORTANTE. QUITARLO FUE EL ORIGEN DE TODOS LOS MALES!!!
	printf("%s\n", start);
	if (!(*arrow_up = tgetstr("ku", NULL)))
		return (1);
	if (!(*arrow_down = tgetstr("kd", NULL)))
		return (1);
	return (0);
}

static int	end_termcap()
{
	char	*end;

	if (!(end = tgetstr("ke", NULL)))
		return (1);
	// ESTO ES MUY IMPORTANTE. QUITARLO FUE EL ORIGEN DE TODOS LOS MALES!!!
	printf("%s\n", end);
	return (0);
}

static void	interruptor(int signal_type)
{
	/*
	**	PUEDE SER QUE PARA PODER SALIR SIN LEAKS AL MANDAR EL USUARIO
	**	UNA SIGINT SE NECESITE ESA FAMOSA VARIABLE GLOBAL?
	**	ESTA FUNCIÓN NO ACEPTA OTROS ARGUMENTOS A PARTE DEL int.
	*/

	if (signal_type == SIGINT)
		printf("Emergency exit.\n");
	if (end_termcap())
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

int	main(void)
{
	char	*buff;
	char	*input;
	char	*arrow_up;
	char	*arrow_down;
	int		read_res;

	read_res = 0;
	input = strdup("");
	if (!(buff = malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (1);
	if (start_termcap(&arrow_up, &arrow_down))
		return(1);
	if (signal(SIGINT, interruptor) == SIG_ERR)
	{
		printf("Emergency exit did not work...\n");
		return (1);
	}
	write(1, "my_prompt:$ ", 12);
	while ((read_res = read(1, buff, BUFF_SIZE)) != -1)
	{
		buff[read_res] = 0;
		if (read_res == 0)
		{
			free(buff);
			free(input);
			end_termcap();
			return (0);
		}
		if (!(input = ft_strjoin(input, buff)))
			return (1);
		if (strstr(input, arrow_up) || strstr(input, arrow_down))
			write(1, "bingo\n", 6);
		if (strchr(input, '\n'))
		{
			write(1, input, strlen(input));
			free(input);
			input = strdup("");
			write(1, "my_prompt:$ ", 12);
		}
	}
	free(buff);
	free(input);
	end_termcap();
	return (1);
}
