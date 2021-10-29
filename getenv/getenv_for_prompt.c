#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
**	PROBANDO getenv PARA CONSTRUIR EL COMMAND PROMPT.
*/

#define ENV_USER "USER"
#define ENV_SESSION "SESSION_MANAGER"
#define ENV_PWD "PWD"

static void	clean_exit(char **prompt, char **aux_memo, int exit_status)
{
	free(*prompt);
	if (*aux_memo)
		free(*aux_memo);
	exit(exit_status);
}

static char	*ft_strjoin(char const *s1, char const *s2, size_t *len_join)
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
	*len_join = i;
	return (result);
}

static int	update_prompt(char **prompt, char *env_value, size_t *len_prompt)
{
	char	*aux;

	if (!(aux = ft_strjoin(*prompt, env_value, len_prompt)))
		return (1);
	free(*prompt);
	*prompt = strdup(aux);
	free(aux);
	return (0);
}

static char	*get_user_device()
{
	char	*aux;
	char	*result;
	char	*env_value;
	size_t	i;

	if (!(env_value = getenv(ENV_SESSION)))
		return (0);
	aux = strchr(env_value, '/');
	aux++;
	i = 0;
	while (aux[i] != '@')
		i++;
	if (!(result = malloc(sizeof(char) * (i + 1))))
		return (0);
	i = 0;
	while (aux[i] != '@')
	{
		result[i] = aux[i];
		i++;
	}
	result[i] = 0;
	return (result);
}

static char	*build_home_path()
{
	char	*env_value;
	char	*home_path;

	if (!(env_value = getenv(ENV_USER)))
		return (0);
	if (!(home_path = malloc(sizeof(char) * (strlen("/home/") +
	strlen(env_value) + 1))))
		return (0);
	home_path[0] = 0;
	strcat(home_path, "/home/");
	strcat(home_path, env_value);
	return (home_path);
}

static char	*get_pwd()
{
	char	*env_value;
	char	*home_path;
	size_t	i;

	if (!(home_path = build_home_path()))
		return (0);
	if (!(env_value = getenv(ENV_PWD)))
	{
		free(home_path);
		return (0);
	}
	i = 0;
	while (env_value[i] == home_path[i] && home_path[i])
		i++;
	if (home_path[i])
	{
		free(home_path);
		return(strdup(env_value));
	}
	free(home_path);
	i = 0;
	while (i++ < 2)
	{
		env_value++;
		env_value = strchr(env_value, '/');
	}
	return (ft_strjoin("~", env_value, &i));
}

int			main(void)
{
	char	*env_value;
	char	*aux_memo;
	char	*prompt;
	size_t	len_prompt;

	prompt = strdup("");
	aux_memo = 0;
	if (!(env_value = getenv(ENV_USER)))
		clean_exit(&prompt, &aux_memo, 1);
	if (update_prompt(&prompt, env_value, &len_prompt))
		clean_exit(&prompt, &aux_memo, 1);
	if (update_prompt(&prompt, "@", &len_prompt))
		clean_exit(&prompt, &aux_memo, 1);
	aux_memo = get_user_device();
	if (update_prompt(&prompt, aux_memo, &len_prompt))
		clean_exit(&prompt, &aux_memo, 1);
	free(aux_memo);
	aux_memo = get_pwd();
	if (update_prompt(&prompt, aux_memo, &len_prompt))
		clean_exit(&prompt, &aux_memo, 1);
	if (update_prompt(&prompt, "$ ", &len_prompt))
		clean_exit(&prompt, &aux_memo, 1);
	write(1, prompt, len_prompt);
	write(1, "\n", 1);
	clean_exit(&prompt, &aux_memo, 0);
}
