/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_liberator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 04:18:35 by ledupont          #+#    #+#             */
/*   Updated: 2025/06/26 18:15:15 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	shell_invalid_line(char *str, char *msg)
{
	if (str && str != NULL)
		ft_free(str);
	if (msg)
		ft_printf_fd(2, "minishell : %s\n", msg);
}

void	free2dstr(char **strs)
{
	int	i;

	i = 0;
	while (strs && strs[i])
		ft_free(strs[i++]);
	ft_free(strs);
}

void	shell_free_envlist(t_envlist *start)
{
	t_envlist	*clean;
	t_envlist	*ntc;

	clean = start;
	while (clean)
	{
		ntc = clean->next;
		if (clean->str)
			ft_free(clean->str);
		ft_free(clean);
		clean = ntc;
	}
	clean = NULL;
}

void	shell_free_token(t_token *first)
{
	t_token	*clean;
	t_token	*ntc;

	clean = first;
	while (clean && clean != NULL)
	{
		ntc = clean->next;
		if (clean->str)
		{
			free(clean->str);
			clean->str = NULL;
		}
		free(clean);
		clean = NULL;
		clean = ntc;
	}
	clean = NULL;
}

void	shell_liberate_all(t_data *data, char *msg, int err)
{
	if (data->first)
		shell_free_token(data->first);
	if (data->strs && data->strs != NULL)
		free2dstr(data->strs);
	if (data->tmp && data->tmp != NULL)
		ft_free(data->tmp);
	if (data->start && data->start != NULL)
		shell_free_envlist(data->start);
	if (data->line && data->line != NULL)
		ft_free(data->line);
	if (err > 0)
		ft_printf_fd(2, "minishell : ");
	if (msg != NULL)
		ft_printf_fd(2, "%s\n", msg);
	exit (err);
}
