/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thcaquet <thcaquet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:45:53 by thcaquet          #+#    #+#             */
/*   Updated: 2025/07/03 18:07:52 by thcaquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*env_get_search(t_data *data, char *re)
{
	char		*str;
	t_envlist	*node;

	node = data->start;
	if (!node)
		return (0);
	while (node && !check_research(node->str, re))
		node = node->next;
	if (node)
	{
		str = node->str;
		while (*str != '=' && str)
			++str;
		return (++str);
	}
	return (0);
}

char	*env_dup_search(t_data *data, char *re)
{
	char		*str;
	char		*new;
	t_envlist	*node;

	node = data->start;
	if (!node)
		return (0);
	while (node && !check_research(node->str, re))
		node = node->next;
	if (node)
	{
		str = node->str;
		while (*str != '=' && str)
			str++;
		new = ft_strdup(++str);
		if (!new)
			mini_liberate_all(data, MALLOC_FAILURE, 1);
		return (new);
	}
	return (0);
}

char	**env_w_search(t_data *data, char *re)
{
	t_envlist	*node;

	node = data->start;
	if (!node)
		return (0);
	while (node && !check_research(node->str, re))
		node = node->next;
	if (node)
	{
		free(node->str);
		return (&(node->str));
	}
	return (0);
}

void	env_unset(t_data *data, char *str)
{
	t_envlist	*node;
	t_envlist	*lst;

	data->error = 0;
	lst = data->start;
	if (!lst)
		return ;
	node = env_search(data, str);
	if (!node)
		return ;
	if (node == data->start)
		data->start = data->start->next;
	else
	{
		while (lst->next != node)
			lst = lst->next;
		lst->next = node->next;
	}
	free(node->str);
	free(node);
}

t_envlist	*env_search(t_data *data, char *search)
{
	t_envlist	*lst;

	lst = data->start;
	while (lst && !check_research(lst->str, search))
		lst = lst->next;
	return (lst);
}
