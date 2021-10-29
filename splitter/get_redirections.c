#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INPUT "echo 'hola < amigo'>adios.txt"

/*
**	CREO QUE GESTIONAR LAS REDIRECCIONES DE OUTPUT (>, >>) CON UNA queue (COLA)
**	PUEDE SER IDEAL A LA HORA DE RESOLVER SITUACIONES COMO ESTAS:
**
**	//CREA EL ARCHIVO INTERMEDIO, PERO EL CONTENIDO ACABA EN EL ÚLTIMO ARCHIVO
**	echo hola > hey.txt > hola.txt
**
**	//DA IGUAL EL ORDEN
**	grep hola < hola.txt > result.txt
**	grep hola > result.txt < hola.txt
**
**	//AQUÍ SÓLO TOMA COMO INPUT EL ÚLTIMO ARCHIVO
**	grep hola > res.txt < adios.txt < hola.txt
*/

/*
**	START LIBFT
*/

typedef	struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (s == NULL)
		return (NULL);
	if (start > strlen(s))
		return (strdup(""));
	p = calloc(len + 1, sizeof(char));
	if (p == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (j < len && s[i] != '\0')
	{
		p[j] = (char)s[i];
		i++;
		j++;
	}
	return (p);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*head;

	if (!(head = (t_list *)malloc(sizeof(t_list))))
		return (0);
	head->content = content;
	head->next = 0;
	return (head);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*target;

	target = *lst;
	if (!target)
		*lst = new;
	else
	{
		while (target->next)
			target = target->next;
		target->next = new;
	}
}

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	(*del)(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*head;
	t_list	*prov;

	head = *lst;
	while (head)
	{
		prov = head;
		head = head->next;
		ft_lstdelone(prov, del);
	}
	*lst = 0;
}

/*
**	END LIBFT
*/

typedef struct s_redirection
{
	char	*input_filepath;
	t_list	*output_queue;
}				t_redirection;

/*
**	append = 0 = >;
**	append = 1 = >>;
*/

typedef struct s_output_data
{
	char	*filepath;
	int		append;
}				t_output_data;

//FOR ft_lstclear()
void	free_redir_output_content(void *content)
{
	free(((t_output_data *)content)->filepath);
	free((t_output_data *)content);
}

void	free_redir(t_redirection *redir)
{
	if ((*redir).input_filepath)
		free(redir->input_filepath);
	if (redir->output_queue)
	{
		ft_lstclear(&(redir->output_queue), free_redir_output_content);
	}
	return ;
}

int	add_output_redir(int const type, char const *filepath,
t_redirection *redir)
{
	t_list			*output_redir;
	t_output_data	*data;

	data = malloc(sizeof(t_output_data));
	if (!data)
		return (1);
	data->append = 0;
	if (type == 3)
		data->append = 1;
	data->filepath = strdup(filepath);
	if (!data->filepath)
		return (1);
	output_redir = ft_lstnew(data);
	ft_lstadd_back(&(redir->output_queue), output_redir);
	return (0);
}

int	add_redir_filepath(char const *input, int const type,
size_t const sub_start, size_t const sub_end, t_redirection *redir)
{
	char	*filepath;

	filepath = ft_substr(input, sub_start, sub_end);
	if (!filepath)
		return (1);
	if (type == 1)
	{
		redir->input_filepath = strdup(filepath);
		if (!redir->input_filepath)
			return (1);
	}
	else
	{
		if (add_output_redir(type, filepath, redir))
			return (1);
	}
	free(filepath);
	return (0);
}

void	update_redir_positions(char const *input, size_t *i, int *redir_type,
size_t	*sub_start)
{
	if (input[*i] == '<')
		*redir_type = 1;
	else if (input[*i + 1] == '>')
	{
		*redir_type = 3;
		*sub_start = *(++i);
	}
	else
		*redir_type = 2;
	*sub_start = *i + 1;
	return ;
}

int		redirector(char const *input, t_redirection *redir)
{
	size_t	i;
	size_t	sub_start;
	int		redir_type; //0 = none; 1 = <; 2 = >; 3 = >>;

	i = 0;
	sub_start = 0;
	redir_type = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			if (redir_type)
			{
				if (add_redir_filepath(input, redir_type, sub_start, i, redir))
					return (1);
				update_redir_positions(input, &i, &redir_type, &sub_start);
			}
			else
				update_redir_positions(input, &i, &redir_type, &sub_start);
		}
		i++;
	}
	if (redir_type)
	{
		if (add_redir_filepath(input, redir_type, sub_start, i, redir))
			return (1);
	}
	return (0);
}

void	quote_manager(char const c, int *singleq_open, int *doubleq_open)
{
	if (c == '\'' && !(*singleq_open) && !(*doubleq_open))
		*singleq_open = 1;
	else if (c == '\'' && (*singleq_open))
		*singleq_open = 0;
	else if (c == '"' && !(*doubleq_open) && !(*singleq_open))
		*doubleq_open = 1;
	else if (c == '"' && *doubleq_open)
		*doubleq_open = 0;
	return ;
}

int	main(void)
{
	size_t	i;
	int				squote_open;
	int				dquote_open;
	char			*command;
	t_redirection	redir;

	i = 0;
	squote_open = 0;
	dquote_open = 0;
	redir.input_filepath = 0;
	redir.output_queue = 0;
	while (INPUT[i])
	{
		quote_manager(INPUT[i], &squote_open, &dquote_open);
		if ((INPUT[i] == '<' || INPUT[i] == '>') && !squote_open
		&& !dquote_open)
		{
			command = ft_substr(INPUT, 0, i);
			if (!command)
				return (1);
			redirector(&INPUT[i], &redir);
		}
		i++;
	}
	printf("%s\n", command);
	if (command)
		free(command);
	free_redir(&redir);
	return (0);
}
