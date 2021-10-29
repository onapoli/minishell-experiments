#include "history.h"

static int	terminal_mod()
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

static void	interruptor(int signal)
{
	if (signal == SIGINT)
		printf("Emergency exit.\n");
	clean_exit(EXIT_SUCCESS);
}

int			main(void)
{
	t_key_code		key_code;

	if (terminal_mod())
		return (1);
	if (signal(SIGINT, interruptor) == SIG_ERR)
	{
		printf("Emergency exit did not work...\n");
		return (1);
	}
	if (init_termcap() || setup_key_read(&key_code))
		clean_exit(EXIT_FAILURE);
	if (print_prompt())
		clean_exit(EXIT_FAILURE);
	reader(key_code);
}
