#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

/*
**	LAS ÚNICAS VARIABLE CONSIDERADAS STANDARD DE LA ESTRUCTURA dirent
**	SON d_name Y d_ino. POR TANTO, QUE EL PROGRAMA DEPENDA DE ALGUNA
**	DE LAS OTRAS VARIABLES NO ES SEGURO.
**	
**	NO HACE FALTA ASIGNAR NI LIBERAR MEMORIA PARA DIR*, NI PARA struct dirent*.
**	
**	readdir DEVUELVE NULL TANTO SI LLEGA AL FINAL DEL STREAM, COMO SI SE
**	PRODUCE UN ERROR. POR ESO, SE RECOMIENDA ASIGNAR 0 A errno ANTES DE LLAMAR
**	A readdir, Y SI DEVUELVE NULL, CHEQUEAR SI errno SIGUE VALIENDO 0.
*/

#define TARGET_1 "bin"
#define TARGET_DIR_1 "/"

/*
**	NO SE PERMITEN BÚSQUEDAS DE NOMBRES DE ARCHIVO MULTINIVEL COMO
**	EN TARGET_2. PARA CONSEGUIR LLEGAR A ls SE PODRÍA HACER CON
**	TARGET_DIR_2 = "/bin" Y TARGET_2 = "ls".
*/

#define TARGET_2 "bin/ls"
#define TARGET_DIR_2 "/"

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

int						main(void)
{
	struct dirent	*target_data;

	if (!(target_data = get_directory_entry(TARGET_DIR_1, TARGET_1)))
		return (1);
	printf("name: %s, inode: %ld\n", target_data->d_name, target_data->d_ino);
	return (0);
}
