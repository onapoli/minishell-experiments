#include <unistd.h>
#include <stdlib.h>

typedef	struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct	s_cond_content
{
	int		head;
	int		tail;
	char	*str;
}				t_cond_content;

/*
**	START LIBFT
*/

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

void	del_cond_content(void *content)
{
	t_cond_content	*cond_content;

	cond_content = (t_cond_content *)content;
	if (cond_content->str)
		free(cond_content->str);
	free(cond_content);
	return ;
}

int	get_coincidences(char **coincidences, t_list *cond_lst)
{
	//SEARCH COINCIDENCES
	//ORDER COINCIDENCES ALPHABETICALLY
	//APPEND TO coincidences
	return (0);
}

int	add_condition(t_list **lst, char const *condition)
{
	t_cond_content	*content;

	content = malloc(sizeof(t_cond_content));
	if (!content)
		return (1);
	content->head = 0;
	content->tail = 0;
	content->str = ft_strdup(condition);
	if (!content->str)
		return (1);
	if (*lst)
		ft_lstadd_back(lst, ft_lstnew(content));
	else
		*lst = ft_lstnew(content);
	return (0);
}

int	get_condition(char const *src, char **condition, size_t *i)
{
	char	*end;
	size_t	len;

	end = mod_strchr(src, '*');
	len = end - src;
	if (!len)
		return (0);
	*condition = ft_substr(src, 0, len);
	if (!(*condition))
		return (1);
	*i += len;
	return (0);
}

int	get_cond_lst(t_list **cond_lst, char const *wildcard)
{
	char	*condition;
	size_t	i;

	condition = 0;
	i = 0;
	while (wildcard[i])
	{
		if (get_condition(&(wildcard[i]), &condition, &i))
			return (1);
		if (add_condition(cond_lst, condition))
			return (1);
		free(condition);
		condition = 0;
	}
	if (wildcard[0] != '*')
		((t_cond_content *)((*cond_lst)->content))->head = 1;
	if (wildcard[ft_strlen(wildcard) - 1] != '*')
		((t_cond_content *)(ft_lstlast(*cond_lst)))->tail = 1;
	return (0);
}

int	process_wildcard(char **wildcard)
{
	t_list	*cond_lst;
	char	*coincidences;

	cond_lst = 0;
	coincidences = 0;
	if (get_cond_lst(&cond_lst, *wildcard))
		return (1);
	free(*wildcard);
	if (get_coincidences(&coincidences, cond_lst))
		return (1);
	*wildcard = ft_strdup(coincidences);
	if (!wildcard)
		return (1);
	free(coincidences);
	ft_lstclear(&cond_lst, del_cond_content);
	return (0);
}

int	wildcard(char const *comm, char **new_comm)
{
	char	**words;
	size_t	i;

	words = ft_splitter(comm, ' ');
	if (!words)
		return (1);
	i = 0;
	while (words[i])
	{
		if (mod_strchr(words[i], '*'))
		{
			if (process_wildcard(&words[i]))
				return (1);
		}
		++i;
	}
	free(words);
	return (0);
}

int	main(void)
{
	char	*res;

	res = 0;
	if (wildcard("echo a*o*.c", &res))
		return (1);
	return (0);
}