# include "angel-main.h"

/*
**	echo hola >= hey
**	CREA EL ARCHIVO = CON EL CONTENIDO hola hey
*/

//busca la comilla simple fuera de las comillas dobles
int single_quote(const char *str, int *inc)
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
int double_quote(char *str, int *inc)
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


/*
**	SÓLO HE MARCADO COMO INVÁLIDOS LOS CARACTERES QUE AFECTAN A LOS COMANDOS
**	INCLUSO CUANDO SE ENCUENTRAN PEGADOS A OTROS CARACTERES.
**	POR EJEMPLO, # NO LO HE MARCADO COMO INVÁLIDO PORQUE
**	echo hola#hey DEVUELVE hola#hey, AUNQUE # CON ESPACIO DELANTE INDIQUE
**	QUE LO QUE HAY DETRÁS HASTA ENCONTRAR UN \n ES UN COMENTARIO EN bash.
**	EN CAMBIO, echo hola;hey DA ERROR PORQUE LO QUE HAY DETRÁS DE ; LO TOMA
**	COMO OTRO COMANDO.
*/

size_t validate_iput(const char *str)
{
    int count;
    int single_quote_var;
    int double_quote_var;

    count = 0;
    single_quote_var = 0;
    double_quote_var = 0;
    while (str[count])
    {
        if (str[count] == '"')
            double_quote_var += double_quote((char *)str, &count);
        else if (str[count] == '\'')
            single_quote_var += single_quote((char *)str, &count);
		else if (str[count] == ';' || str[count] == '\\'
					|| str[count] == '&' || str[count] == '`'
					|| str[count] == '(' || str[count] == ')'
					/*|| str[count] == '{' || str[count] == '}'
					|| str[count] == '[' || str[count] == ']'*/)
		{
			printf("Invalid input: %c\n", str[count]);
			return (1);
		}
		else if (str[count] == str[count + 1] && !ft_isalnum(str[count])
					&& str[count] != '\n' && str[count] != ' ')
		{
			if (str[count] == '<' || str[count] == '>')
			{
				if (str[count + 2] == str[count])
				{
					printf("Invalid input: %c.\n", str[count]);
					return (1);
				}
			}
			else
			{
				printf("Invalid input: %c.\n", str[count]);
				return (1);
			}
		}
		else if ((str[count] == '<' && str[count + 1] == '>')
		|| (str[count] == '>' && str[count + 1] == '<'))
		{
			printf("Invalid input: %c.\n", str[count]);
			return (1);
		}
        count++;
    }
    
//    return (validate);
    printf("single: %d  -  double: %d\n", single_quote_var, double_quote_var);
   printf("ft_strlen 2 %zu\n", ft_strlen(str));
    if (double_quote_var % 2 != 0 || single_quote_var % 2 != 0)
    {
        write(1, "Error quote\n", 20);
        return (1);
    }
    return (0);
}

int main(void)
{
    char	*buff;
    size_t	read_res;
	int		res;

	res = 0;
    if (!(buff = malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (1);
    while (1)
    {
        read_res = read(1, buff, BUFF_SIZE);
        if (read_res == (size_t)-1)
		{
			res = 1;
            break ;
		}
        if (read_res == 0)
            break ;
        buff[read_res] = 0;
        if (validate_iput(buff))
		{
			res = 1;
			break ;
		}
    }
    free(buff);
    return (res);
}
