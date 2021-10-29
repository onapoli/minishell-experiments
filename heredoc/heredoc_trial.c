#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
**	ESTO FUNCIONA:
**
**	grep la << FIN > hola.txt   
**	> hola que tal estas
**	> yo bien
**	> FIN
**
**
**	ESTO TAMBIÉN FUNCIONA:
**
**	grep la > hola.txt << FIN              
**	> hola como estas
**	> bien
**	> FIN
**
**
**	ESTO TAMBIÉN FUNCIONA:
**
**	grep la << FIN | wc
**	> hola como estas?
**	> FIN
**		  1       3      17
**
**	ESTO NO FUNCIONA, PERO SÍ CREA hola.txt VACÍO:
**
**	grep la << FIN DE > hola.txt
**	> hola como estas
**	> bien
**	> FIN DE
**	> FIN
**	grep: DE: No such file or directory
**
**
**	ESTO NO FUNCIONA (aunque no da error), PERO SÍ CREA hola.txt VACÍO:
**
**	grep la << FIN << END > hola.txt
**	> hola
**	> que
**	> tal
**	> end
**	> FIN
**	> END
**
**
**	CUANDO HAY MÁS DE UN <<, bash ASUME COMO INPUT LO INTRODUCIDO ENTRE
**	LA INTRODUCCIÓN DEL PRIMER ELEMENTO DE << Y EL ÚLTIMO.
**
**	AQUÍ grep NO RECIBE NADA:
**	grep a <<FIN<<END
**	> hola
**	> FIN
**	> END
**
**	AQUÍ SOLO TOMA COMO INPUT adios Y adeu:
**	grep a <<FIN<<END
**	> hola
**	> END
**	> alfa
**	> FIN
**	> adios
**	> FIN
**	> adeu
**	> END
**	adios
**	adeu
**
**
**
**	grep a << FIN << END << FINALE
**	> hola
**	> FIN
**	> adios
**	> END
**	> hasta la vista
**	> FINALE
**	hasta la vista
**
**
**	SI HAY UN CARACTER ERRÓNEO EN ALGÚN PIPE, bash, EN ubuntu, SÓLO IMPRIME
**	ERROR SIN PROCESAR LOS PIPES VÁLIDOS. CONFIRMAR QUE ESTO SEA ASÍ EN MAC.
**	~$ echo hola > hola.txt | wc <<<<< FIN
**	bash: syntax error near unexpected token `<<'
**	NO HA CREADO hola.txt.
**
**
**	SI DESPUÉS DE LA PALABRA CLAVE DEL heredoc HAY ESPACIO
**	Y NO HAY OTRO heredoc, ESOS CARACTERES SE TOMARÁN COMO PARTE DEL COMANDO.
**	echo hey << FIN joe << END
**	> FIN
**	> END
**	hey joe
**
**
**	SI HAY UN heredoc (<<) Y UNA REDIRECCIÓN DE INPUT (<) EN UN MISMO COMANDO,
**	SOLO SE CONSIDERA LA REDIRECCIÓN DE INPUT (<).
**	grep a << FIN < hola.txt 
**	> adios
**	> FIN
**	hola
**
**
**
**	grep a << FIN > uno.txt | grep b << END
**	> hola
**	> END
**	> hola
**	> FIN
**	> buenas
**	> END
**	buenas
**	Y EL ARCHIVO uno.txt CONTIENE:
**	hola
**	hola
**
**
**
**	grep a <<END<<END
**	> hola
**	> END
**	> END
**
**	grep a <<END<<END
**	> hola
**	> END
**	> adios
**	> END
**	adios
**
**
**
**	LA SUSTITUCIÓN DE VARIABLES DE ENTORNO SE PRODUCE DESPUÉS DE HABER
**	COMPLETADO LA PARTE DE READ. ES DECIR, SI UNA VARIABLE DE ENTORNO
**	CONTUVIERA UN COMANDO CON heredoc (<<), bash NO PUEDE EJECUTAR
**	LA FUNCIONALIDAD DE heredoc, Y POR TANTO SEGÚN EL COMANDO, DEVOLVERÁ
**	ERROR O INTERPRETARÁ << COMO TEXTO. EJEMPLOS:
**
**	hey="echo hola"
**	$hey
**	hola
**
**	hey="grep a << END"
**	$hey
**	grep: <<: No such file or directory
**	grep: END: No such file or directory
**
**	hey="echo hola << END"
**	$hey
**	hola << END
**
**
**
**	AL INCLUIR $ COMO PARTE DE LA KEYWORD DE UN heredoc, bash TOMA $ COMO STRING
**	Y NO REALIZA SUSTITUCIÓN DE VARIABLES. EJEMPLOS:
**
**	hey=AL
**	grep a << $hey
**	> hola
**	> AL
**	> $hey
**	hola
**
**	hey=AL
**	grep a << FIN$hey
**	> hola
**	> FIN
**	> FINAL
**	> FIN$hey
**	hola
**
**
**
**	LA PALABRA CLAVE DEL heredoc DEBE INTRODUCIRSE
**	EN UNA LÍNEA INDEPENDIENTE Y SIN ESPACIOS PARA
**	QUE bash LA RECONOZCA.
**	grep a << END
**	> hola
**	> que tal END estas
**	> fin END
**	> END
**	hola
**	que tal END estas
**
**	EN ESTE EJEMPLO EL SEGUNDO END TIENE ESPACIO DETRÁS.
**	grep a << END
**	> hola
**	>  END
**	> END 
**	> END
**	hola
*/

/*
**	GLOBAL VAR
*/

typedef	struct	s_heredoc
{
	char	*keyword;
	char	*input;
}				t_heredoc;

typedef	struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct	s_glob_var
{
	t_list	*here_lst;
	char	*prov_input;
}				t_glob_var;

t_glob_var	glob_var;

#define NORMAL "Prompt:$ "
#define	HEREDOC "> "

/*
**	INIT LIBFT
*/

size_t ft_strlen(const char *str)
{
    size_t count;

    count = 0;
    while(*str)
    {
        str++;
        count++;
    }
    return (count);
}

char	*ft_strchr(const char *s, int c)
{
    size_t	count;

    count = 0;
    while (*(s + count))
    {
        if (*(char *)(s + count) == (char)c)
            return ((char *)(s + count));
        count++;
    }
    if ((char)c == 0)
        return ((char *)(s + count));
    return (0);
}

char	*ft_strdup(const char *c)
{
    char	*newc;
    int		count;
    int		len;

    len = ft_strlen(c);
    newc = malloc(sizeof(char) * (len + 1));
    if (!newc)
        return (NULL);
    count = 0;
    while (c[count])
    {
        newc[count] = c[count];
        count++;
    }
    newc[count] = '\0';
    return (newc);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
    size_t	count;
    char	*dst;

    if (!s)
        return (NULL);
    if (ft_strlen(s) < start)
        return (ft_strdup(""));
    if (ft_strlen(s + start) < len)
        len = ft_strlen(s + start);
    dst = malloc(sizeof(char) * (len + 1));
    if (dst == NULL)
        return (NULL);
    count = 0;
    while (count < len)
    {
        *(dst + count) = *(s + count + start);
        count++;
    }
    *(dst + count) = '\0';
    return (dst);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	char	*p;

	if (!s1)
		return (NULL);
	while (*s1 && ft_strchr(set, *s1))
	{
		s1++;
	}
	i = ft_strlen(s1);
	while (i && ft_strchr(set, s1[i]))
	{
		i--;
	}
	p = ft_substr(s1, 0, i + 1);
	return (p);
}

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	char			*p;

	i = 0;
	p = s;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*p;

	p = (void *)malloc(size * count);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, count * size);
	return (p);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*p;

	if (!s1 || !s2)
		return (NULL);
	if (!(p = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char))))
		return (NULL);
	j = 0;
	i = 0;
	while (j < ft_strlen(s1))
	{
		p[i] = (char)s1[j];
		j++;
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
	{
		p[i] = (char)s2[j];
		i++;
		j++;
	}
	p[i] = '\0';
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
//busca la comilla simple fuera de las comillas dobles
int single_quote(const char *str, size_t *inc)
{
    int count_queue;

    count_queue = 0;
    while (count_queue < 2 && str[*inc])
    {
        if (str[*inc] == '\'')
            ++count_queue;
        *inc = *inc + 1;
    }
    *inc = *inc - 1;
    return (count_queue);
}

//busca la primera comilla hasta cerrar cuenta como caracter la comilla simple
int double_quote(char *str, size_t *inc)
{
    int count_queue;

    count_queue = 0;
    while (count_queue < 2 && str[*inc])
    {
        if (str[*inc] == '"' && str[*inc - 1] != '\\')
            ++count_queue;
        *inc = *inc + 1;
    }
    *inc = *inc - 1;
    return (count_queue);
}

char	*mod_strchr(const char *s, int c)
{
	size_t	count;

    count = 0;
    while (*(s + count))
    {
		if (s[count] == '"')
            double_quote((char *)s, &count);
        else if (s[count] == '\'')
            single_quote((char *)s, &count);
        else if (*(char *)(s + count) == (char)c)
            return ((char *)(s + count));
        count++;
    }
    if ((char)c == 0)
        return ((char *)(s + count));
    return (0);
}

int	ft_join(char **dst, char const *src)
{
	char	*aux;

	if (!(*dst))
	{
		*dst = ft_strdup(src);
		return (0);
	}
	aux = ft_strdup(*dst);
	free(*dst);
	*dst = ft_strjoin(aux, src);
	if (!(*dst))
	{
		free(aux);
		return (1);
	}
	free(aux);
	return (0);
}

int	join_here_input(char **dst, char const *input)
{
	if (ft_join(dst, input))
		return (1);
	if (ft_join(dst, "\n"))
		return (1);
	return (0);
}

void	del_heredoc(void *content)
{
	t_heredoc	*heredoc;

	heredoc = (t_heredoc *)content;
	free(heredoc->keyword);
	if (heredoc->input)
		free(heredoc->input);
	free(heredoc);
	return ;
}

int	add_heredoc(char *keyword)
{
	t_heredoc	*heredoc;

	heredoc = malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (1);
	heredoc->keyword = ft_strdup(keyword);
	heredoc->input = 0;
	if (!glob_var.here_lst)
		glob_var.here_lst = ft_lstnew(heredoc);
	else
		ft_lstadd_back(&glob_var.here_lst, ft_lstnew(heredoc));
	return (0);
}

char	*get_keyword(char const *start)
{
	char	*keyword;
	size_t	len;

	len = 0;
	if (start[len] == ' ')
	{
		while (start[len] && start[len] == ' ')
			++len;
	}
	if (!start[len])
		return (0); //NO HAY HEREDOC KEYWORD, ASÍ QUE HAY QUE DEVOLVER ERROR.
	while (start[len] && start[len] != ' ' && start[len] != '<'
			&& start[len] != '>' && start[len] != '|' && start[len] != '\n')
		++len;
	if (start[len] == ' ')
	{
		while (start[len] && start[len] == ' ')
			++len;
	}
	keyword = ft_substr(start, 0, len);
	return (keyword);
}

char	*get_next_heredoc(char const *input)
{
	char	*target;
	int		skip;

	target = (char *)input;
	skip = 0;
	while (1)
	{
		target = mod_strchr(target + skip, '<');
		if (!target)
			break ;
		if (target[1] == '<')
			return (target + 2);
		if (!skip)
			skip = 1;
	}
	return (0);
}

int		here_processor(char const *input)
{
	char	*start;
	char	*keyword;
	char	*clean_keyword;

	start = (char *)input;
	while (1)
	{
		start = get_next_heredoc(start);
		if (!start)
			return (0);
		keyword = get_keyword(start);
		if (!keyword)
			return (1);
		clean_keyword = ft_strtrim(keyword, " ");
		if (!clean_keyword)
		{
			free(keyword);
			return (1);
		}
		free(keyword);
		//ADD TO HEREDOC ENDING LINKED LIST STORED IN THE GLOBAL VARIABLE.
		if (add_heredoc(clean_keyword))
			return (1);
		printf("keyword: %s\n", clean_keyword);
		free(clean_keyword);
	}
}

/*
**	LA FUNCIÓN DE REVISIÓN DE SINTAXIS YA HA REVISADO
**	SI HAY MÁS DE DOS < Ó > SEGUIDOS.
*/

int		is_heredoc(char const *input)
{
	char	*target;

	target = mod_strchr(input, '<');
	if (!target)
		return (0);
	if (target[1] == '<')
		return (1);
	while (1)
	{
		target = mod_strchr(target + 1, '<');
		if (!target)
			break ;
		if (target[1] == '<')
			return (1);
	}
	return (0);
}

/*
**	CUANDO EL USUARIO LE DA A ENTER SIN HABER ESCRITO NADA, readline
**	AUTOMÁTICAMENTE IMPRIME \n Y ESCUCHA EL SIGUIENTE COMANDO. NO HACE
**	FALTA QUE IMPLEMENTEMOS NOSOTROS ESA FUNCIONALIDAD.
*/

int		reader()
{
	char	*input;
	char	*first_input;
	char	*prompt;
	t_list	*here_track;

	prompt = NORMAL;
	while (1)
	{
		input = readline(prompt);
		printf("input: %s\n", input);
		if (!input)
		{
			//CTRL+D
			printf("No input.\n");
			/*
			**	HAY QUE ELIMINAR LA VARIABLE GLOBAL
			**	CON TODA SU MEMORIA ASIGNADA, INCLUIDA LA LISTA DEL HEREDOC
			**	Y EL first_input EN CASO DE RECIBIR CTRL+D EN MITAD DE UN
			**	PROCESAMIENTO DE HEREDOC.
			*/
			if (glob_var.here_lst)
				ft_lstclear(&glob_var.here_lst, del_heredoc);
			if (glob_var.prov_input)
				free(glob_var.prov_input);
			if (first_input)
				free(first_input);
			free(input);
			return (0);
		}
		if (*input)
		{
			//REVISIÓN SINTAXIS input
			//ERROR SI NO ES VÁLIDO.
			if (!glob_var.here_lst)
			{
				add_history(input);
				if (is_heredoc(input))
				{
					//INIT HEREDOC PROCESS
					printf("IS HEREDOC\n");
					if (here_processor(input))
					{
						printf("Error en here_processor.\n");
						free(input);
						return (1);
					}
					here_track = glob_var.here_lst;
					prompt = HEREDOC;
					first_input = ft_strdup(input);
					free(input);
					continue ;
				}
			}
			else
			{
				if (!strcmp(input, ((t_heredoc *)(here_track->content))->keyword))
				{
					if (glob_var.prov_input)
					{
						((t_heredoc *)(here_track->content))->input = ft_strdup(glob_var.prov_input);
						free(glob_var.prov_input);
						glob_var.prov_input = 0;
					}
					here_track = here_track->next;
				}
				else
				{
					if (join_here_input(&glob_var.prov_input, input))
					{
						printf("Error en join.\n");
						free(input);
						return (1);
					}
				}
				free(input);
				if (!here_track)
				{
					prompt = NORMAL;
					input = first_input;
				}
				else
					continue ;
			}
			//PROCESS FINAL USER INPUT
			//if (commander(input)) {return (1);}
			if (glob_var.here_lst)
			{
				here_track = glob_var.here_lst;
				while (here_track)
				{
					printf("herelist elem: %s\nAnd input: %s\n", ((t_heredoc *)(here_track->content))->keyword, ((t_heredoc *)(here_track->content))->input);
					here_track = here_track->next;
				}
				ft_lstclear(&glob_var.here_lst, del_heredoc);
			}
		}
		free(input);
		first_input = 0;
	}
	return (0);
}

int		main(void)
{
	glob_var.here_lst = 0;
	glob_var.prov_input = 0;
	if (reader())
	{
		printf("Bad input.\n");
		return (1);
	}
	return (0);
}
