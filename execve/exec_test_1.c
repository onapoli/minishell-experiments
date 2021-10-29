#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
**	PRUEBA FLUJO execve, COMPROBANDO SI EL PROGRAMA A EJECUTAR ES INTERNO
**	O EXTERNO. SI ES INTERNO NO OCURRE NADA EN ESTE TEST, SOLO SE IMPRIME
**	EN PANTALLA QUE ES INTERNO.
**	
**	SI ES EXTERNO, SE COMPRUEBA SI EL USUARIO HA INDICADO EL PATH DEL PROGRAMA
**	YA SEA ABSOLUTO O RELATIVO, Y SI NO, SE BUSCA EN LOS DIRECTORIOS QUE
**	CONTIENE LA VARIABLE DE ENTORNO $PATH EL NOMBRE DEL PROGRAMA ESPECIFICADO
**	POR EL USUARIO.
**	
**	UNA VEZ OBTENIDO EL PATH DEL PROGRAMA, SE EJECUTA CON execve.
**	
**	AVISO!!!
**	ES SOLO UNA PRUEBA, HAY MUCHOS FALLOS...
**
**	HE PROBADO CON echo, ls y cat.
**	CON echo FUNCIONA, CON ls FUNCIONA, CON cat NO FUNCIONA.
**	TODAVÍA NO SÉ PORQUÉ FALLA CON cat.
**
**	PARA PROBARLO EJECUTAR ESTE PROGRAMA DE ESTA FORMA:
**	./nombre_de_este_programa "echo parametro1 parametro2"
**	./nombre_de_este_programa "ls parametro1 parametro2"
**	./nombre_de_este_programa "cat parametro1 parametro2"
*/

static int				launch_exec_process(char *program_path, char **param)
{
	pid_t	child_process;

	if ((child_process = fork()) == -1)
		return (1);
	if (child_process == 0)
	{
		execve(program_path, param, NULL);
		exit(EXIT_FAILURE);
	}
	else
		return (0);
}

static char				*ft_strjoin(char *s1, char const *s2)
{
	int		i;
	char	*result;

	if (!(result = malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1))))
		return (0);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		result[i++] = *(s2)++;
	}
	result[i] = 0;
	free(s1);
	return (result);
}

static struct dirent	*get_directory_entry(char const *target_dir,
char const *target)
{
	DIR				*dir;
	struct dirent	*dir_entry;

	dir = 0;
	dir_entry = 0;
	if (!(dir = opendir(target_dir)))
		return (dir_entry);
	errno = 0;
	while ((dir_entry = readdir(dir)) != NULL)
	{
		if (strcmp(dir_entry->d_name, target) == 0)
			break ;
	}
	if (errno)
		printf("readdir error.\n");
	else if (!dir_entry)
		printf("Not found.\n");
	if (closedir(dir) == -1)
		return (0);
	return (dir_entry);
}

static char				*get_next_path(char **path)
{
	char	*next_path;
	size_t	i;

	next_path = 0;
	i = 0;
	while ((*path)[i] && (*path)[i] != ':')
		i++;
	if (!(next_path = malloc(sizeof(char) * (i + 1))))
		return (next_path);
	next_path[i] = 0;
	i = 0;
	while ((*path)[i] && (*path)[i] != ':')
	{
		next_path[i] = (*path)[i];
		i++;
	}
	if ((*path = strchr(*path, ':')))
		(*path)++;
	return (next_path);
}

static char				*get_program_path(char const* program)
{
	char	*sys_path;
	char	*program_path;

	sys_path = 0;
	program_path = 0;
	if (*program == '/' || (*program == '.' && program[1] == '/')
	|| strchr(program, '/'))
		return (strdup(program));
	sys_path = getenv("PATH");
	program_path = get_next_path(&sys_path);
	while (!get_directory_entry(program_path, program))
	{
		free(program_path);
		if (!(program_path = get_next_path(&sys_path)))
			break ;
	}
	return (program_path);
}

static int				check_internal_program(char const *program)
{
	if (!strcmp(program, "echo") || !strcmp(program, "cd")
	|| !strcmp(program, "pwd") || !strcmp(program, "export")
	|| !strcmp(program, "export") || !strcmp(program, "unset")
	|| !strcmp(program, "env") || !strcmp(program, "exit"))
		return (0);
	return (1);
}

static size_t			get_total_param(char const *command)
{
	size_t	i;

	i = 0;
	while (*command && *command != ';' && *command != '\n')
	{
		while (*command == ' ')
			command++;
		if (!(*command) || *command == ';' || *command == '\n')
			return (i);
		while (*command && *command != ' ' && *command != ';' && *command != '\n')
			command++;
		if (*command != ' ')
			return (++i);
		i++;
	}
	return (i);
}

static int				get_param(char const *command, char ***param)
{
	size_t	len;
	size_t	i;
	size_t	j;

	*param = 0;
	if ((len = get_total_param(command)) > 1)
		len--;
	else
		return (0);
	if (!(*param = malloc(sizeof(char *) * (len + 1))))
		return (1);
	(*param)[len] = 0;
	while (*command == ' ')
		command++;
	while (*command && *command != ' ' && *command != ';' && *command != '\n')
		command++;
	if (*command != ' ')
		return (1);
	i = 0;
	while (*command == ' ')
	{
		len = 0;
		while (*command == ' ')
			command++;
		if (!(*command) || *command == ';' || *command == '\n')
			return (0);
		while (command[len] && command[len] != '\n' &&
		command[len] != ' ' && command[len] != ';')
			len++;
		if (!((*param)[i] = malloc(sizeof(char) * (len + 1))))
			return (0);
		(*param)[i][len] = 0;
		j = 0;
		while (j < len)
		{
			(*param)[i][j] = *command;
			command++;
			j++;
		}
		i++;
	}
	return (0);
}

static char				*get_program(char const *command)
{
	char	*program;
	size_t	len;
	size_t	i;

	program = 0;
	len = 0;
	i = 0;
	while (*command == ' ')
		command++;
	while (command[len] && command[len] != '\n' &&
	command[len] != ' ' && command[len] != ';')
		len++;
	if (!(program = malloc(sizeof(char) * (len + 1))))
		return (program);
	program[len] = 0;
	while (i < len)
	{
		program[i] = command[i];
		i++;
	}
	return (program);
}

int						main(int argc, char **argv)
{
	char	*program;
	char	*program_path;
	char	**param;
	int		status;
	size_t	i;

	program = 0;
	program_path = 0;
	param = 0;
	if (argc == 1 || argc > 2)
		return (0);
	if (!(program = get_program(argv[1])))
		return (1);
	printf("program: %s\n", program);
	if (get_param(argv[1], &param))
		return (1);
	i = 0;
	while (param[i])
	{
		printf("param %ld: %s\n", i, param[i]);
		i++;
	}
	if (!check_internal_program(program))
	{
		printf("Internal program execution.\n");
		free(program);
		i = 0;
		while (param[i])
		{
			free(param[i]);
			i++;
		}
		free(param);
		return (0);
	}
	if (!(program_path = get_program_path(program)))
	{
		printf("Program could not be found.\n");
		return (1);
	}
	printf("program_path: %s\n", program_path);
	program_path = ft_strjoin(program_path, "/");
	program_path = ft_strjoin(program_path, program);
	if (launch_exec_process(program_path, param))
	{
		printf("program execution failed\n");
		return (1);
	}
	if (wait(&status) == -1)
	{
		printf("Wait failed.\n");
		//clean_exit(&wdir, EXIT_FAILURE);
	}
	if (WEXITSTATUS(status))
	{
		printf("Child process encountered an error.\n");
		//clean_exit(&wdir, EXIT_FAILURE);
	}
	free(program);
	free(program_path);
	i = 0;
	while (param[i])
	{
		free(param[i]);
		i++;
	}
	free(param);
	return (0);
}
