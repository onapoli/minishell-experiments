#include "history.h"

int	tputs_write(int c)
{
	if (!write(1, &c, 1))
		return (1);
	return (0);
}

int	init_termcap()
{
	char	*term_description;
	int		read_result;

	term_description = 0;
	read_result = tgetent(term_description, getenv("TERM"));
	if (!read_result || read_result == -1)
		return (1);
	return (0);
}

int	setup_key_read(t_key_code *key_code)
{
	if ((tputs(tgetstr("ks", NULL), 1, tputs_write)) == ERR)
		return (1);
	if (!(key_code->arrow_up = tgetstr("ku", NULL)) ||
	!(key_code->arrow_down = tgetstr("kd", NULL)) ||
	!(key_code->arrow_left = tgetstr("kl", NULL)) ||
	!(key_code->arrow_right = tgetstr("kr", NULL)) ||
	!(key_code->backspace = tgetstr("kb", NULL)) ||
	!(key_code->del = tgetstr("kD", NULL)))
		return (1);
	return (0);
}

int	end_termcap()
{
	if ((tputs(tgetstr("ke", NULL), 1, tputs_write)) == ERR)
		return (1);
	return (0);
}
