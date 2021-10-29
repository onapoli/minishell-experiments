#include "demo.h"

/*
**	SEGÚN EL man, fgetc DEVUELVE unsigned char CASTEADO A int.
**	Y SI HAY ERROR (read DEVUELVE -1) O SE HA LLEGADO AL FINAL
**	DEL STREAM (read DEVUELVE 0), DEVUELVE EOF.
*/

int		ft_fgetc(FILE *stream)
{
	int				rval;
	unsigned char	content;

	if (!stream)
		return (EOF);
	if ((rval = read(stream->_fileno, &content, 1)) <= 0)
		return(EOF);
	return ((int)content);
}
