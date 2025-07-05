/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_envlen.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledupont <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:17:31 by ledupont          #+#    #+#             */
/*   Updated: 2025/07/05 19:17:36 by ledupont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_strrlen_nospace(char *str)
{
	int	i;
	int	j;
	int	k;

	i = ft_strlen(str) - 1;
	j = 0;
	k = 0;
	if (shell_space(str[i]) == 1)
		return (0);
	while (--i >= 0)
	{
		if (k == 1 && shell_space(str[i]) == 0)
			j++;
		if (k == 0 && shell_space(str[i]) == 0)
		{
			k = 1;
			j++;
		}
		if (k == 1 && shell_space(str[i]) == 1)
			return (j);
	}
	return (j);
}

char	*shell_strjumpdup(t_data *data, char *str, int start)
{
	int		l;
	char	*new;

	l = ft_strlen(str) - start;
	new = malloc(sizeof(char) * (l + 1));
	if (!new)
		shell_liberate_all(data, "malloc failure", 1);
	l = 0;
	while (str[start])
		new[l++] = str[start++];
	return (new);
}

int	shell_env_lastlen(t_data *data)
{
	t_envlist	*cur;
	char		*str;
	int			l;

	cur = data->start;
	str = "";
	l = ft_strlen(data->ex);
	while (cur)
	{
		if (shell_exp_strcmp(cur->str, data->ex, l) == 0)
		{
			str = shell_strjumpdup(data, cur->str, l + 1);
			break ;
		}
		cur = cur->next;
	}
	l = shell_strrlen_nospace(str);
	free(str);
	str = NULL;
	return (l);
}
