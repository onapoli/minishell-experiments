#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

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
		/*
		**	SI *input ES NULL, SIGNIFICA QUE EL USUARIO PULSÓ ENTER SIN
		**	INTRODUCIR NADA. Y readline RECIBIÓ SIMPLEMENTE \n, CARACTER QUE
		**	readline ELIMINA AUTOMÁTICAMENTE DEL input, Y POR TANTO,
		**	RECIBIMOS NULL EN *input. ESTO ES DIFERENTE A RECIBIR
		**	NULL EN input, YA QUE ESTO SIGNIFICA QUE READLINE NO HA RECIBIDO
		**	NI SIQUIERA \n, PORQUE EL USUARIO PULSÓ CTRL+D, PROVOCANDO QUE
		**	readline RECIBA EOF.
		*/
		if (*input)
		{
			printf("received: %s\n", input);
			add_history(input);
		}
		free(input);
	}
	return (0);
}
