#include "history.h"

int	history_up(t_command_buff *command_buff)
{
	static size_t	last_command = -1;
	size_t			i;
	/*
	**	HAY QUE PREPARAR FALLBACKS EN CASO DE QUE HAYA CAPABILITIES
	**	QUE NO ESTÉN PRESENTES EN EL TERMINAL UTILIZADO.
	**	POR EJEMPLO, NO TODOS TIENEN "cr", EN ESE CASO HABRÍA QUE
	**	UTILIZAR "cm".
	*/
	/*
	**	SI NO HAY UN COMANDO ANTERIOR AL ACTUAL NO HACER NADA.
	*/
	last_command++;
	i = 0;
	while (i++ < last_command)
		command_buff = command_buff->next;
	if (!command_buff)
	{
		last_command--;
		return (0);
	}
	if ((tputs(tgetstr("cr", NULL), 1, tputs_write)) == ERR)
		return (1);
	if ((tputs(tgetstr("dl", NULL), 1, tputs_write)) == ERR)
		return (1);
	if (print_prompt() || print_command(command_buff->command))
		return (1);
	return (0);
}

int	history_down()
{
	/*
	**	SI NO HAY UN COMANDO POSTERIOR AL ACTUAL NO HACER NADA.
	*/
	/*if ((tputs(tgetstr("cr", NULL), 1, tputs_write)) == ERR)
		return (1);
	if ((tputs(tgetstr("dl", NULL), 1, tputs_write)) == ERR)
		return (1);*/
	/*
	**	IMPRIMIR PROMPT Y LUEGO IMPRIMIR EL COMANDO POSTERIOR.
	*/
	return (0);
}

int move_left()
{
	/*
	**	PRIMERO HAY QUE COMPROBAR QUE PUEDA MOVERSE UNA POSICIÓN
	**	A LA IZQUIERDA.
	*/
	if ((tputs(tgetstr("le", NULL), 1, tputs_write)) == ERR)
		return (1);
	return (0);
}

int move_right()
{
	/*
	**	SI ESTUVIERA EN LA ÚLTIMA COLUMNA DE LA DERECHA,
	**	HABRÍA QUE PASAR AL EXTREMO IZQUIERDO DE LA
	**	LÍNEA INFERIOR MANUALMENTE, SIN LLAMAR A "nd",
	**	PORQUE DARÍA RESULTADO INDEFINIDO.
	*/
	if ((tputs(tgetstr("nd", NULL), 1, tputs_write)) == ERR)
		return (1);
	return (0);
}

int	delete_char()
{
	if ((tputs(tgetstr("dm", NULL), 1, tputs_write)) == ERR)
		return (1);
	if ((tputs(tgetstr("dc", NULL), 1, tputs_write)) == ERR)
		return (1);
	if ((tputs(tgetstr("ed", NULL), 1, tputs_write)) == ERR)
		return (1);
	/*
	**	ELIMINAR CARACTER TAMBIÉN DEL BUFFER DE COMANDOS.
	*/
	return (0);
}

int	delete_back()
{
	/*
	**	PRIMERO HAY QUE COMPROBAR QUE PUEDA MOVERSE UNA POSICIÓN
	**	A LA IZQUIERDA
	*/
	move_left();
	delete_char();
	/*
	**	ELIMINAR CARACTER TAMBIÉN DEL BUFFER DE COMANDOS.
	*/
	return (0);
}

int	execute_capability(char *target_key, t_key_code key_code,
t_command_buff *command_buff)
{
	int	result;

	result = 0;
	if (!strcmp(target_key, key_code.del))
		delete_char();
	else if (!strcmp(target_key, key_code.backspace))
		delete_back();
	else if (!strcmp(target_key, key_code.arrow_up))
		history_up(command_buff);
	else if (!strcmp(target_key, key_code.arrow_down))
		history_down();
	else if (!strcmp(target_key, key_code.arrow_left))
		move_left();
	else if (!strcmp(target_key, key_code.arrow_right))
		move_right();
	else
		result = 1;
	return (result);
}
