#include <stdio.h>
#include <stdlib.h>

#define INPUT "echo hola '   > hola.txt   'que tal estas << HEY yo bien <input.txt y tu>> append.txt que tal?"

/*
**	IMPORTANTE!!!
**
**	echa jsgd > hola.txt
**	Y
**	> hola.txt echa hdlhld
**	CREAN hola.txt VACÍO E IMPRIMEN ERROR DE COMANDO.
**
**	echo hola >
**	Y
**	echo hola > | echo adios
**	DEVUELVE ERROR DE SINTAXIS.
**
**	grep a << FIN
**	> hola        amigo como    estas
**	> FIN
**	hola        amigo como    estas
**	EN LOS HEREDOC NO SE SUPRIMEN LOS ESPACIOS EXTRA
**	QUE NO ESTÁN ENTRE COMILLAS.
**
**	grep a << FIN
**	> hola "amigo como"   estas
**	> "esto     es una prueba'
**	> FIN
**	hola "amigo como"   estas
**	"esto     es una prueba'
**	EN LOS HEREDOC NO HAY QUE QUITAR LAS COMILLAS, POR TANTO
**	EL INPUT DE LOS HEREDOC NO HAY QUE MODIFICARLO PARA NADA.
*/

/*
**	HAY QUE DECIDIR DÓNDE QUITAR LOS ESPACIOS EXTRA QUE ESTÁN FUERA DE "" Y ''.
**	PIENSO QUE SERÍA MEJOR HACERLO AL REVISAR LA SINTAXIS, YA QUE AHÍ ESTAMOS
**	UTILIZANDO LA FUNCIÓN PARA IGNORAR COMILLAS, Y SI LO HICIÉRAMOS AQUÍ SE
**	TENDRÍA QUE VOLVER A USAR DE NUEVO. LO QUE PODRÍA SER MENOS EFICIENTE.
**	EN CUALQUIER CASO, PARA SUPRIMIR LOS ESPACIOS EXTRA PIENSO QUE HABRÍA QUE
**	EXTRAER PALABRA POR PALABRA DEL INPUT QUE NO ESTÉ ENTRECOMILLADO Y AÑADIR UN
**	ESPACIO ANTES DE CONCATENAR CON LO ANTERIOR. O HACER SUBSTR AL ENCONTRAR UN
**	ESPACIO EN EL INPUT QUE NO ESTÁ ENTRECOMILLADO, E INCLUYENDO SÓLO UN ESPACIO
**	EN EL SUBSTR. 
*/

typedef	struct	s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

typedef	struct	s_output
{
	int		append;
	char	*filename;
}				t_output;

/*
**	heredoc YA HA SIDO RELLENADO EN LA FASE DEL reader, POR TANTO
**	LO ÚNICO QUE HAY QUE HACER ES SALTAR SU APARICIÓN EN EL INPUT
**	DEL USUARIO.
*/

typedef	struct	s_input
{
	char	*ext_file;
	//t_list	*heredoc;
}				t_input;

typedef	struct	s_redir
{
	t_input		in;
	t_list		*out;
}				t_redir;

typedef	struct	s_global
{
	t_redir	redir;
}				t_global;

t_global	global;

/*
**  INIT LIBFT
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

void			ft_bzero(void *s, size_t n)
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
**  END LIBFT
*/

/*
**  FUNCIÓN PARA SEPARAR LAS REDIRECCIONES DEL COMANDO,
**  QUE YA HA SIDO DIVIDIDO POR PIPES.
**  ADEMÁS, LLEGADOS A ESTE PUNTO, YA SE HAN SUSTITUIDO
**  LAS VARIABLES DE ENTORNO QUE ESTUVIERAN PRESENTES EN
**  EL COMANDO, Y YA SE HA COMPROBADO QUE LA PRIMERA PALABRA
**  CORRESPONDE A UN PROGRAMA INTERNO O EXTERNO EXISTENTE Ó
**  A UNA REDIRECCIÓN. 
*/

void	skip_spaces(char const *input, size_t *len)
{
	while (input[*len] == ' ')
		*len += 1;
}

char	*get_filename(char const *input)
{
	char	*filename;
	char	*aux;
	size_t	len;

	filename = 0;
	aux = 0;
	len = 0;
	skip_spaces(input, &len);
	if (!input[len])
		return (0); //NO HAY FILENAME
	while (input[len] && input[len] != ' ' && input[len] != '<'
			&& input[len] != '>' && input[len] != '|' && input[len] != '\n')
		++len;
	skip_spaces(&input[len], &len);
	aux = ft_substr(input, 0, len);
	filename = ft_strtrim(aux, " ");
	free(aux);
	return (filename);
}

size_t	get_redir_len(char const *input)
{
	size_t	len;

	len = 0;
	if (input[len] == ' ')
	{
		while (input[len] && input[len] == ' ')
			++len;
	}
	while (input[len] && input[len] != ' ' && input[len] != '<'
			&& input[len] != '>')
		++len;
	if (input[len] == ' ')
	{
		while (input[len] && input[len] == ' ')
			++len;
	}
	return (len);
}

/*
**	SE LE AÑADE UN ESPACIO ATRÁS A *dst PARA EVITAR QUE 2
**	PALABRAS SE JUNTEN AL PROCESAR UN INPUT COMO ÉSTE:
**	echo hola> hola.txt que tal
**
**	EN UN SIGUIENTE PASO HABRÁ QUE SEPARAR CADA PALABRA
**	DEL INPUT QUE NO ESTÉ ENTRECOMILLADO PARA SUPRIMIR ESPACIOS
**	EXTRA Y SÓLO DEJAR UNO ENTRE CADA PALABRA. POR ESO DA IGUAL
**	SI AÑADIENDO UN ESPACIO A aux AÑADIMOS, EN LA MAYORÍA DE CASOS,
**	UN ESPACIO EXTRA. LO MÁS IMPORTANTE ES EVITAR QUE NO SE JUNTEN
**	PALABRAS QUE NO LO ESTÁN, PORQUE SE CAMBIARÍA EL SIGNIFICADO
**	DEL INPUT DEL USUARIO.
*/

int		cut_and_append(size_t const start, size_t const end,
char const *input, char **dst)
{
	size_t	len;
	char	*chunk;
	char	*aux;

	len = end - start;
	chunk = ft_substr(input, start, len);
	if (!chunk)
		return (1);
	if (*dst)
	{
		aux = ft_strjoin(*dst, " ");
		free(*dst);
		*dst = ft_strjoin(aux, chunk);
		if (!(*dst))
		{
			free(aux);
			free(chunk);
			return (1);
		}
	}
	else
		*dst = ft_strdup(chunk);
	free(chunk);
	return (0);
}

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

int separator(char const *command, char **clean_command)
{
    size_t  chunk_start;
	size_t	redir_len;
    size_t  i;
	t_list		*out;
	t_output	*output;
	size_t	inc;

    chunk_start = 0;
	redir_len = 0;
    i = 0;
    while (1)
    {
		if (command[i] == '"')
            double_quote((char *)command, &i);
        else if (command[i] == '\'')
            single_quote((char *)command, &i);
        else if (command[i] == '<' || command[i] == '>')
        {
            if (command[i] == '<')
            {
				//CHECK IF HEREDOC
				if (command[i + 1] == '<')
				{
					inc = 2;
					i += inc;
					redir_len = get_redir_len(&command[i]);
					/*
					**	EN CASO DE NO SER EL PRIMER HEREDOC DEL PIPE,
					**	AUMENTAR UNA POSICIÓN EL HEREDOC TRACKER, YA QUE
					**	SÓLO CUENTA EL INPUT DEL ÚLTIMO HEREDOC DEL PIPE. 
					*/
				}
				else
				{
					inc = 1;
					i += inc;
					redir_len = get_redir_len(&command[i]);
					if (global.redir.in.ext_file)
					{
						free(global.redir.in.ext_file);
						global.redir.in.ext_file = 0;
					}
					global.redir.in.ext_file = get_filename(&command[i]);
					if (!global.redir.in.ext_file)
						return (1);
				}
			}
            else if (command[i] == '>')
            {
				output = malloc(sizeof(t_output));
				if (!output)
					return (1);
				//CHECK IF APPEND
				if (command[i + 1] == '>')
				{
					inc = 2;
					i += inc;
					redir_len = get_redir_len(&command[i]);
					output->append = 1;
				}
				else
				{
					inc = 1;
					i += inc;
					redir_len = get_redir_len(&command[i]);
					output->append = 0;
				}
				output->filename = get_filename(&command[i]);
				if (!output->filename)
				{
					free(output);
					return (1);
				}
				out = ft_lstnew(output);
				if (!global.redir.out)
					global.redir.out = out;
				else
					ft_lstadd_back(&global.redir.out, out);
			}
			if (command[chunk_start] != '<' && command[chunk_start] != '>')
			{
				//JOIN PREVIOUS CHUNK WITH clean_command
				if (cut_and_append(chunk_start, i - inc, command, clean_command))
					return (1);
			}
            chunk_start = i + redir_len;
			i += redir_len;
        }
		else if (!command[i])
		{
			if (command[chunk_start] != '<' && command[chunk_start] != '>')
			{
				//JOIN PREVIOUS CHUNK WITH clean_command
				if (!(*clean_command))
					*clean_command = (char *)command;
				else
				{
					if (cut_and_append(chunk_start, i, command, clean_command))
						return (1);
				}
			}
			return (0);
		}        
        ++i;
    }
}

int main(void)
{
    char    *clean_command;

	global.redir.in.ext_file = 0;
	global.redir.out = 0;
    clean_command = 0;
    if (separator(INPUT, &clean_command))
		return (1);
	printf("clean command: %s\n", clean_command);
	if (clean_command)
		free(clean_command);
    return (0);
}
