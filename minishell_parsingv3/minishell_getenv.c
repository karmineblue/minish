/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_getenv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 04:18:35 by ledupont          #+#    #+#             */
/*   Updated: 2025/06/26 13:39:58 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envlist	*shell_envlist_new(t_data *data, char **envp, int i)
{
	t_envlist	*new;

	new = malloc(sizeof(t_envlist));
	if (!new)
		shell_liberate_all(data, "malloc failure", 1);
	new->str = ft_strdup(envp[i]);
	if (!new->str)
		shell_liberate_all(data, "malloc failure", 1);
	new->next = NULL;
	return (new);
}

int	shell_get_envp_check(char **envp, int i)
{
	int	j;

	while (envp[i])
	{
		j = -1;
		while (envp[i][++j])
			if (envp[i][j] == '=')
				return (i);
		i++;
	}
	return (-1);
}

void	shell_get_envp(t_data *data, char **envp)
{
	int			i;
	t_envlist	*cur;

	cur = NULL;
	data->start = NULL;
	i = shell_get_envp_check(envp, 0);
	if (i == -1)
		return ;
	if (envp[i])
	{
		data->start = shell_envlist_new(data, envp, i);
		cur = data->start;
		if (ft_strncmp(cur->str, "_=", 2) == 0)
			data->end = cur;
	}
	i = shell_get_envp_check(envp, i + 1);
	while (i != -1 && envp[i])
	{
		cur->next = shell_envlist_new(data, envp, i);
		cur = cur->next;
		if (ft_strncmp(cur->str, "_=", 2) == 0)
			data->end = cur;
		i = shell_get_envp_check(envp, i + 1);
	}
	return ;
}
