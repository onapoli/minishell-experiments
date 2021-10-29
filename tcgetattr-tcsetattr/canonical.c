#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <termios.h>

#define BUFF_SIZE 10

/*
**	ESTRUCTURA QUE DEVUELVE tcgetarr(), DECLARADA COMO VARIABLE
**	GLOBAL PARA PODER UTILIZAR EN LA FUNCIÓN interruptor CUANDO
**	SE SALE DEL PROGRAMA CON Ctrl+C (SIGINT).
*/

struct termios	init_term_attr;

/*
**	ESTA FUNCIÓN SE ENCARGA DE REESTABLECER LA CONFIGURACIÓN DEL TERMINAL
**	ANTES DE SALIR DEL PROGRAMA.
*/

void	clean_exit(int exit_code)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &init_term_attr))
		printf("Could not reset terminal config.\n");
	exit(exit_code);
}

/*
**	ESTA FUNCIÓN ES LA QUE signal() LLAMA CUANDO EL PROGRAMA RECIBE
**	LA SEÑAL SIGINT (EL USUARIO PULSA Ctrl+C).
*/

void	interruptor(int signal)
{
	if (signal == SIGINT)
		printf("Emergency exit.\n");
	clean_exit(EXIT_SUCCESS);
}

/*
**	PRIMERO SE OBTIENE LA CONFIGURACIÓN DEL TERMINAL:
**		1.	PARA GUARDARLA EN init_term_attr Y PODER VOLVER A REESTABLECER
**			ESA MISMA CONFIGURACIÓN AL SALIR DEL PROGRAMA.
**		2.	PARA GUARDARLA EN mod_term_attr Y MODIFICAR LOS VALORES
**			EN ESA ESTRUCTURA PARA PASARLA A tcsetattr(), QUE MODIFICARÁ
**			LA CONFIGURACIÓN CON LOS VALORES CONTENIDOS EN mod_term_attr.
**
**	LUEGO, UTILIZANDO BITWISE OPERATIONS SOBRE LOS VALORES DE LA ESTRUCTURA
**	mod_term_attr, SE MODIFICA EL MODO DE FUNCIONAMIENTO DEL TERMINAL
**	QUITANDO EL MODO CANÓNICO QUE TIENE POR DEFECTO Y QUITANDO LA FUNCIONALIDAD
**	DE IMPRIMIR LOS CARACTERES QUE EL USUARIO INTRODUCE MIENTRAS SE LEE EN
**	TIEMPO REAL.
**
**	LUEGO, SE MODIFICAN LOS VALORES DE NÚMERO MÍNIMO DE CARACTERES INTRODUCIDOS,
**	Y EL TIEMPO MÍNIMO DE LECTURA PARA QUE READ DEVUELVA LOS CARACTERES
**	QUE INTRODUCE EL USUARIO.
**
**	POR ÚLTIMO SE APLICAN LOS CAMBIOS CON tcsetattr();
*/

int	terminal_mod()
{
	struct termios	mod_term_attr;

	if (tcgetattr(STDIN_FILENO, &init_term_attr) ||
	tcgetattr(STDIN_FILENO, &mod_term_attr))
		return (1);
	mod_term_attr.c_lflag &= ~(ICANON|ECHO);
	mod_term_attr.c_cc[VMIN] = 1;
	mod_term_attr.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &mod_term_attr))
		return (1);
	return (0);
}

int	main(void)
{
	int				read_res;
	char			real_time_buff;
	char			buff[BUFF_SIZE + 1];
	int				buff_len;

	buff_len = 0;
	if (terminal_mod())
		return (1);
	if (signal(SIGINT, interruptor) == SIG_ERR)
	{
		printf("Emergency exit did not work...\n");
		return (1);
	}
	write(1, "my_prompt:$ ", 12);
	while ((read_res = read(1, &real_time_buff, 1)) != -1)
	{
		if (read_res == 0)
		{
			printf("buff: %s\n", buff);
			clean_exit(EXIT_SUCCESS);
		}
		if (real_time_buff == '\004')
		{
			printf("No ha recibido 0 la función read.\n");
			printf("buff: %s\n", buff);
			clean_exit(EXIT_SUCCESS);
		}
		if (buff_len < BUFF_SIZE)
		{
			buff[buff_len] = real_time_buff;
			buff[buff_len + 1] = '\0';
			buff_len = strlen(buff);
		}
	}
	clean_exit(EXIT_FAILURE);
}
