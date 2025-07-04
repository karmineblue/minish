/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_red.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thcaquet <thcaquet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:33:33 by thcaquet          #+#    #+#             */
/*   Updated: 2025/07/04 12:30:03 by thcaquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	red_truncate(t_data *data, char *file)
{
	data->fd.out = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (data->fd.out < 0)
	{
		put_error(data, OPEN_FAILURE, 1);
		data->error = 128;
		return ;
	}
	dup2(data->fd.out, STDOUT_FILENO);
	close(data->fd.out);
	data->error = 0;
}

void	red_append(t_data *data, char *file)
{
	if (access(file, F_OK) == 0 && access(file, W_OK) == -1)
	{
		put_error(data, ERROR_FILE_PERM, 1);
		data->error = 126;
		return ;
	}
	data->fd.out = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (data->fd.out < 0)
	{
		put_error(data, OPEN_FAILURE, 1);
		data->error = 128;
		return ;
	}
	dup2(data->fd.out, STDOUT_FILENO);
	close(data->fd.out);
	data->error = 0;
}

void	red_read(t_data *data, char *file)
{
	if (access(file, F_OK))
	{
		put_error(data, ERROR_FILE_PATH, 1);
		data->error = 126;
		return ;
	}
	else if (access(file, R_OK))
	{
		put_error(data, ERROR_FILE_PERM, 1);
		data->error = 127;
		return ;
	}
	data->fd.in = open(file, O_RDONLY);
	if (data->fd.in < 0)
	{
		put_error(data, OPEN_FAILURE, 1);
		data->error = 128;
		return ;
	}
	dup2(data->fd.in, STDIN_FILENO);
	close(data->fd.in);
	data->error = 0;
}

void	hook_heredoc(t_data *data, char *end)
{
	char	*line;
	char	*l_expand;

	line = 0;
	while (sig_check(data))
	{
		line = mini_readline(data, "> ");
		if (!line)
			break ;
		if (ft_strcmp(line, end) == 0)
		{
			free(line);
			break ;
		}
		l_expand = mini_expand(data, line);
		write(data->fd.out, l_expand, strlen(l_expand));
		write(data->fd.out, "\n", 1);
		free(l_expand);
	}
}

void	red_heredoc(t_data *data, char *end)
{
	int		i;

	i = -2147483647;
	data->fd.file = 0;
	data->error = 0;
	while (!data->fd.file || access(data->fd.file, F_OK) != -1)
	{
		if (data->fd.file)
			free(data->fd.file);
		data->fd.file = ft_clean_strjoin("heredoc_tmp_", ft_itoa(i++), 1);
		if (i == -2147483647 || !data->fd.file)
		{
			printf("%s", ERROR_HEREDOC);
			free(data->fd.file);
			return ;
		}
	}
	data->fd.out = open(data->fd.file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	hook_heredoc(data, end);
	close(data->fd.out);
	data->fd.in = open(data->fd.file, O_RDONLY);
	dup2(data->fd.in, STDIN_FILENO);
	unlink(data->fd.file);
	free(data->fd.file);
	close(data->fd.in);
}
