/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main_parsing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 04:18:35 by ledupont          #+#    #+#             */
/*   Updated: 2025/07/01 12:58:11 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	shell_fonction_parse(t_token *cur)
{
	if (cur->type == FONCTION && ft_strncmp(cur->str, "echo", 5) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "cd", 3) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "pwd", 4) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "export", 7) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "unset", 6) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "env", 4) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION && ft_strncmp(cur->str, "exit", 5) == 0)
		printf("buil-tin: ");
	else if (cur->type == FONCTION)
		printf("execve: ");
}

void	shell_print_envp(t_data *data)
{
	t_envlist	*cur;

	if (data->start && data->start->str != NULL)
	{
		cur = data->start;
		while (cur != NULL)
		{
			printf("%s\n", cur->str);
			cur = cur->next;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	t_token	*cur;

	(void)argv;
	if (argc > 1)
	{
		write(2, "Error : no argument please\n", 27);
		return (0);
	}
	ft_bzero(&data, sizeof(t_data));
	shell_get_envp(&data, envp);
	while (1)
	{
		data.pipe = 0;
		data.line = readline("ask > ");
		if (data.line && data.line != NULL)
			shell_line_set(&data);
		printf("piping %d\n", data.pipe);
		if (data.end && data.end->str != NULL)
			printf("und = %s\n", data.end->str);
		if (data.first && data.first != NULL)
		{
			cur = data.first;
			while (cur)
			{
				shell_fonction_parse(cur);
				printf("%s (%d)\n", cur->str, cur->type);
				cur = cur->next;
			}
			if (ft_strncmp(data.first->str, "exit", 5) == 0)
			{
				shell_free_envlist(data.start);
				shell_free_token(data.first);
				exit (0);
			}
			shell_error_token(&data);
			shell_free_token(data.first);
			data.first = NULL;
		}
	}
	return (0);
}
