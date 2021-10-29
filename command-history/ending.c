#include "history.h"

void	clean_exit(int exit_code)
{
	write(1, "exiting\n", 8);
	if (end_termcap())
		printf("Could not end termcap.\n");
	if (tcsetattr(STDIN_FILENO, TCSANOW, &init_term_attr))
		printf("Could not reset terminal config.\n");
	exit(exit_code);
}
