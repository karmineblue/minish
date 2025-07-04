/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_token_expand.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledupont <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 20:44:25 by ledupont          #+#    #+#             */
/*   Updated: 2025/07/02 20:44:33 by ledupont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_token_assign_bis(t_data *data)
{
	t_token	*cur;
	int		pt;

	cur = data->first;
	pt = PIPE;
	while (cur)
	{
		if (cur->str && cur->type == ARG && pt == PIPE)
		{
			cur->type = FONCTION;
			pt = FONCTION;
		}
		if (cur->type == PIPE)
			pt = PIPE;
		cur = cur->next;
	}
}

int	shell_last_strlen(char **strs)
{
	int	i;

	i = -1;
	while (strs[++i])
	{
		if (!strs[i + 1])
			return (ft_strlen(strs[i]));
	}
	return (0);
}

t_token	*shell_token_inject(t_data *data, t_token *cur,\
	t_token *prev, char **strs)
{
	int		i;

	data->cnext = cur->next;
	free(cur->str);
	cur->str = NULL;
	free(cur);
	cur = NULL;
	if (prev)
	{
		prev->next = shell_token_new(data, strs, 0, 0);
		cur = prev->next;
	}
	else
	{
		data->first = shell_token_new(data, strs, 0, 0);
		cur = data->first;
	}
	i = 0;
	while (strs[++i])
	{
		cur->next = shell_token_new(data, strs, i, 0);
		cur = cur->next;
	}
	cur->next = data->cnext;
	return (cur);
}

int	shell_check_space(char*str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (shell_space(str[i]) == 1)
			return (1);
	return (0);
}

t_token	*shell_token_sep(t_data *data, t_token *cur, t_token *prev, t_index *in)
{
	if (cur->type > 2 && in->m == 0
		&& (ft_strlen(cur->str) == 0 || shell_check_space(cur->str) == 1))
		data->pipe = AMBIGUOUS;
	else if (in->m == 0 && shell_check_space(cur->str) == 1)
	{
		//data->strs = ft_split(cur->str, ' ');
		data->strs = shell_split_space(cur->str);
		if (!data->strs)
			shell_liberate_all(data, NULL, 1);
		in->i = shell_last_strlen(data->strs);
		cur = shell_token_inject(data, cur, prev, data->strs);
		free2dstr(data->strs);
	}
	return (cur);
}
