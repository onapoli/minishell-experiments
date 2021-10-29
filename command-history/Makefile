NAME = history_test

COMP = gcc

COMP_FLAGS = -Wall -Wextra -Werror

LIBS = -ltermcap

SRCS = history.c \
		reader.c \
		printer.c \
		command_buffer.c \
		termcap_config.c \
		terminal_capabilities.c \
		ending.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(COMP) $(COMP_FLAGS) -o $(NAME) $(OBJS) $(LIBS)

%.o: %.c
	@$(COMP) $(COMP_FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
