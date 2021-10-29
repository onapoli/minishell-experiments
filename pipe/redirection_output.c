#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
**	AL USAR execve, SI EL PROGRAMA NO TIENE ARGUMENTOS, HAY QUE PASAR
**	COMO ÚNICO ARGUMENTO DE args, EL NOMBRE DEL PROGRAMA. SI NO, DARÁ ERROR.
**	AUNQUE EL PROGRAMA SÍ TENGA ARGUMENTOS, SE PUEDE DEJAR COMO PRIMER ARGUMENTO
**	EL NOMBRE DEL PROGRAMA (SIN PATH) Y LA EJECUCIÓN FUNCIONARÁ IGUAL QUE SI
**	NO LO INCLUERA.
**	ES DECIR SI ls TUVIERA COMO ARGUMENTO -la, args PODRÍA ESTAR COMPUESTO POR:
**	args[0] = "ls";
**	args[1] = "-la"
**	args[2] = 0;
*/

/*
**	EN exec_program() UTILIZO dup2() PARA ASIGNAR EL VALOR
**	DEL fd(FILE DESCRIPTOR) dest AL fd DEL STANDARD OUTPUT (stdout)
**	DEL SUBPROCESO child. UNA VEZ QUE dup2() SE HA EJECUTADO, EL stdout
**	DE ESE PROCESO APUNTA AL ARCHIVO dest, ASÍ QUE YA PODEMOS CERRAR
**	EL fd QUE CONTIENE dest, YA QUE NO NOS SIRVE TENER 2 fd APUNTANDO
**	AL MISMO ARCHIVO.
**	AL EJECUTAR LA FUNCIÓN execve(), SE INICIA UN NUEVO SUBPROCESO DENTRO
**	DEL SUBPROCESO child, QUE HEREDA LOS fd (ABIERTOS?) DE SU PROCESO PADRE,
**	ES DECIR, stdin, stdout Y stderr EN ESTE PROGRAMA. COMO EL stdout DE child
**	APUNTA AL ARCHIVO AL QUE APUNTABA dest, ESE ARCHIVO RECIBIRÁ TODO LO QUE
**	EL SUBPROCESO INICIADO POR execve() ESCRIBA A stdout.
*/

void	exec_program()
{
	char	*args[2];
	int		dest;

	args[0] = "ls";
	args[1] = 0;
	//EN CASO DE QUE dest NO EXISTA, LE DOY LOS MISMOS PERMISOS QUE bash.
	dest = open("ls_output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR
	| S_IRGRP | S_IWGRP | S_IROTH);
	if (dest == -1)
		exit(EXIT_FAILURE);
	if (dup2(dest, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(dest);
	execve("/bin/ls", args, NULL);
	exit(EXIT_FAILURE);
}

int	launch_process()
{
	pid_t	child;

	child = fork();
	if (child == -1)
		return (1);
	if (!child)
	{
		exec_program();
		exit(EXIT_SUCCESS);
	}
	else
		return (0);
}

int	main(void)
{
	int	status;

	if (launch_process())
		return (1);
	if (wait(&status) == -1)
		return (1);
	if (WEXITSTATUS(status))
		return (1);
	return (0);
}
