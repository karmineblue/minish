/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thcaquet <thcaquet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 04:18:35 by ledupont          #+#    #+#             */
/*   Updated: 2025/07/04 20:15:04 by thcaquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shell_error_token(t_data *data)
{
	if (data->pipe == AMBIGUOUS)
		ft_printf_fd(2, AMBIGUOUS_ERROR);
	else if (data->pipe == -1)
		ft_printf_fd(2, UNEXPECTED"`newline'\n");
	else if (data->pipe == -PIPE)
		ft_printf_fd(2, UNEXPECTED"`|'\n");
	else if (data->pipe == -REDINFILE)
		ft_printf_fd(2, UNEXPECTED"`<'\n");
	else if (data->pipe == -REDHEREDOC)
		ft_printf_fd(2, UNEXPECTED"`<<'\n");
	else if (data->pipe == -REDOUTFILE)
		ft_printf_fd(2, UNEXPECTED"`>'\n");
	else if (data->pipe == -REDAPPEND)
		ft_printf_fd(2, UNEXPECTED"`>>'\n");
}

void	shell_fonction_parse(t_data *data)
{
	char	**cmd;

	if (!sig_check(data) || !data->first)
		return ;
	cmd = lst_token_to_tab(data);
	if (data->first->type == FONCTION && ft_strncmp(data->first->str, "echo", 5) == 0)
		mini_echo(data, cmd);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "cd", 3) == 0)
		mini_cd(data, cmd);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "pwd", 4) == 0)
		mini_pwd(data, cmd);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "export", 7) == 0)
		mini_export(data, cmd);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "unset", 6) == 0)
		mini_unset(data, cmd);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "env", 4) == 0)
		mini_env(data);
	else if (data->first->type == FONCTION && ft_strncmp(data->first->str, "exit", 5) == 0)
		mini_exit(data, cmd);
	else if (data->first->type == FONCTION)
		mini_execve(data, cmd);
	free2dstr(cmd);
}

t_token	*tok_unset(t_data *data, t_token *node)
{
	t_token	*lst;

	data->error = 0;
	lst = data->first;
	if (!lst || !node)
		return (0);
	if (node == lst)
		lst = lst->next;
	else
	{
		while (lst->next != node)
			lst = lst->next;
		lst->next = node->next;
	}
	if (node == data->first)
		data->first = node->next;
	free(node->str);
	free(node);
	return (lst);
}

void	shell_red_parse(t_data *data)
{
	t_token	*node;

	node = data->first;
	while (node && node->type != 2)
	{
		if (!sig_check(data))
			return ;
		if (node->type == 3)
			red_read(data, node->str);
		else if (node->type == 4)
			red_heredoc(data, node->str);
		else if (node->type == 5)
			red_truncate(data, node->str);
		else if (node->type == 6)
			red_append(data, node->str);
		if (node && node->type >= 3 && node->type <= 6)
			node = tok_unset(data, node);
		else
			node = node->next;
	}
}
