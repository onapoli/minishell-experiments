#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_node
{
	char			*value;
	struct s_node	*next;
}	t_node;

typedef struct s_list
{
	size_t	size;
	t_node	*firstItem;
	t_node	*lastItem;
}	t_list;

t_node	*getNode(char *value)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	node->value = value;
	node->next = NULL;
	return (node);
}

/*
**	enlazar nueva lista ligada
*/

t_list	*getNewList(void)
{
	t_list	*ll;

	ll = malloc(sizeof(t_list));
	ll->size = 0;
	ll->firstItem = NULL;
	ll->lastItem = NULL;
	return (ll);
}

/*
**	operaciones CRUD
*/

void	add(t_list *ll, t_node *node)
{
	if (ll->firstItem == NULL)
		ll->firstItem = node;
	else
		ll->lastItem->next = node;
	ll->lastItem = node;
	ll->size++;
}

t_node	*read(t_list *ll, unsigned int index)
{
	size_t	i;
	t_node	*list_explorer;

	if (ll->size && index < ll->size)
	{
		list_explorer = ll->firstItem;
		i = 0;
		while (i++ < index)
			list_explorer = list_explorer->next;
	}	
	return (list_explorer);
}

void	update(t_list *ll, unsigned int index, char *newValue)
{
	size_t	i;
	t_node	*list_explorer;

	if (ll->size && index < ll->size)
	{
		list_explorer = ll->firstItem;
		i = 0;
		while (i++ < index)
			list_explorer = list_explorer->next;
		free(list_explorer->value);
		list_explorer->value = newValue;
	}
}

void	removed(t_list *ll, unsigned int index)
{
	t_node	*aux;
	t_node	*list_explorer;
	size_t	i;

	if (ll->size && index < ll->size)
	{
		if (index == 0)
		{
			aux = ll->firstItem;
			ll->firstItem = ll->firstItem->next;
		}
		else
		{
			list_explorer = ll->firstItem;
			i = 0;
			while (i++ < index - 1)
				list_explorer = list_explorer->next;
			aux = list_explorer->next;
			if (index == ll->size - 1)
				list_explorer->next = list_explorer;
			else
				list_explorer->next = list_explorer->next->next;
		}
	}
	free(aux->value);
	free(aux);
	ll->size--;
}

void	free_list(t_list **lst)
{
	t_node	*iter;
	t_node	*aux;

	iter = (*lst)->firstItem;
	while (iter)
	{
		printf("iter: %s\n", iter->value);
		aux = iter->next;
		free(iter->value);
		free(iter);
		iter = aux;
	}
	free(*lst);
}

int	main()
{
	t_list	*lista = getNewList();
	t_node	*a = getNode(strdup("hola"));
	t_node	*b = getNode(strdup("alo"));
	t_node	*c = getNode(strdup("ciao"));
	t_node	*d = getNode(strdup("hello"));
	
	add(lista, a);
	add(lista, b);
	add(lista, c);
	add(lista, d);
	
	printf("%s\n", read(lista, 3)->value);
	update(lista, 3, strdup("salut"));
	printf("%s\n", read(lista, 3)->value);
	printf("%s\n", read(lista, 0)->value);
	removed(lista, 0);
	printf("%s\n", read(lista, 0)->value);

	free_list(&lista);
	return (0);
}
