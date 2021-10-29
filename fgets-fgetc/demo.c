#include "demo.h"

int main(void)
{
	FILE	*stream;
	char	buff[BUFF_SIZE];
	char	*res;

	//ASIGNAMOS LA ESTRUCTURA DE stdin QUE ES DE TIPO "FILE" A stream.
	stream = stdin;
	buff[0] = '\0';
	write(1, "user@PC $: ", 11);
	while(1)
	{
		/* 	
		**	SI res DEVUELVE NULL, QUIERE DECIR QUE HA HABIDO UN ERROR, O QUE
		**	HA LLEGADO AL FINAL DEL ARCHIVO, QUE EN ESTE CASO QUIERE DECIR
		**	QUE LA FUNCIÓN read DE ft_fgetc() HA DEVUELTO -1 Ó 0 RESPECTIVAMENTE.
		*/
		if (!(res = ft_fgets(buff, BUFF_SIZE, stream)))
		{
			/*
			**	SI buff NO CONTIENE '\n', QUIERE DECIR QUE HA HABIDO UN ERROR,
			**	YA QUE CADA COMANDO INTRODUCIDO POR EL USUARIO termina con '\n'
			**	AL CONFIRMARLO PULSANDO EL BOTÓN ENTER(CR).
			*/
			if (!strchr(buff, '\n'))
			{
				write(1, "\n", 1);
				return (1);
			}
			break ;
		}
		//NO INCLUYO '\n' PORQUE buff CONTIENE LO CONTIENE AL FINAL.
		printf("%s", buff);
		/*
		**	INCLUYO '\0' EN EL PRIMER BYTE PARA QUE DENTRO DE fgets LO INTERPRETE
		**	COMO VACÍO.
		*/
		buff[0] = '\0';
		write(1, "user@PC $:", 11);
	}
	return (0);
} 
