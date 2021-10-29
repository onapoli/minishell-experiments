#include "demo.h"

char *ft_fgets(char *s, int size, FILE *stream)
{
	int	content;
	int i;

	content = 0;
	i = 0;
	/*HAY QUE INSERTAR '\0' DESPUÉS DE '\n'*/
	while (i < size)
	{
		if ((content = ft_fgetc(stream)) == EOF)
		{
			/*
			**	POR ESTE if LE ASIGNO '\0' AL PRIMER BYTE DE buff
			**	ANTES DE LLAMAR A ft_fgets().
			**	SI NO SE HA LEÍDO NADA EN LA LLAMADA A fgets(), ÉSTE
			**	DEVUELVE NULL.
			**	MIRAR man PARA VER CUÁNDO DEVUELVE NULL Y LO QUE PASA
			**	CON EL BUFFER.
			*/
			if (s[0])
				break ;
			//SI HA HABIDO ERRORES, O NO SE HA LEÍDO NADA, RETORNA NULL.
			return (NULL);
		}
		s[i] = content;
		i++;
		// SI fgets() ENCUENTRA UN '\n' DEJA DE LEER.
		if (content == '\n')
			break ;
	}
	//SIEMPRE HAY QUE INSERTAR '\0' DESPUÉS DEL ÚLTIMO BYTE LEÍDO.
	s[i] = '\0';
	//SI NO HA HABIDO ERRORES, Y SE HA LEÍDO ALGO RETORNA EL BUFFER.
	return (s);
}
