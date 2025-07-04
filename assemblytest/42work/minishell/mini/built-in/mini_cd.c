/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thcaquet <thcaquet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 16:44:06 by thcaquet          #+#    #+#             */
/*   Updated: 2025/07/02 19:59:48 by thcaquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void cd_home(t_data *data)
{
	char *home;
	char **w_oldpwd;

	home = env_get_search(data, "HOME");
	last_cmd(data, "cd", NULL);
	if (!home)
		ft_putstr_fd(CD_HOME_NO_SET, 2);
	else if (access(home, F_OK) == -1)
		ft_putstr_fd(CD_NO_DIRECTORY, 2);
	else if (access(home, X_OK) == -1)
		ft_putstr_fd(CD_NO_PERM, 2);
	else
	{
		w_oldpwd = env_w_search(data, "OLDPWD");
		if (w_oldpwd)
		{
			*w_oldpwd = ft_clean_strjoin("OLDPWD=", getcwd(0, 0), 1);
			if (!*w_oldpwd)
				mini_liberate_all(data, MALLOC_FAILURE, 1);
		}
		chdir(home);
		data->error = 0;
		last_cmd(data, home, NULL);
	}
}

void cd_oldpwd(t_data *data)
{
	char *oldpwd[2];
	char **w_oldpwd;

	oldpwd[0] = env_get_search(data, "OLDPWD");
	last_cmd(data, "", NULL);
	if (!oldpwd[0])
		ft_putstr_fd(CD_OLDPWD_NO_SET, 2);
	else if (access(oldpwd[0], F_OK) == -1)
		ft_putstr_fd(CD_NO_DIRECTORY, 2);
	else if (access(oldpwd[0], X_OK) == -1)
		ft_putstr_fd(CD_NO_PERM, 2);
	else
	{
		oldpwd[1] = getcwd(0, 0);
		chdir(oldpwd[0]);
		last_cmd(data, oldpwd[0], NULL);
		w_oldpwd = env_w_search(data, "OLDPWD");
		if (w_oldpwd)
		{
			*w_oldpwd = ft_clean_strjoin("OLDPWD=", oldpwd[1], 1);
			if (!*w_oldpwd)
				mini_liberate_all(data, MALLOC_FAILURE, 1);
		}
		data->error = 0;
	}
}

void cd_parente(t_data *data)
{
	int i;
	char *path;
	char **w_oldpwd;

	path = getcwd(0, 0);
	if (!path)
		mini_liberate_all(data, MALLOC_FAILURE, 1);
	i = ft_strlen(path);
	while (i > 1 && i >= 0 && path[i] != '/')
		i--;
	path = ft_n_realloc(path, i);
	w_oldpwd = env_w_search(data, "OLDPWD");
	if (w_oldpwd)
	{
		*w_oldpwd = ft_clean_strjoin("OLDPWD=", getcwd(0, 0), 1);
		if (!*w_oldpwd)
			mini_liberate_all(data, MALLOC_FAILURE, 1);
	}
	last_cmd(data, "..", NULL);
	chdir(path);
	data->error = 0;
	free(path);
}

void cd_chdir(t_data *data, char *path)
{
	char **w_oldpwd;

	last_cmd(data, path, NULL);
	if (access(path, F_OK) == -1)
		ft_putstr_fd(CD_NO_DIRECTORY, 2);
	else if (access(path, X_OK) == -1)
		ft_putstr_fd(CD_NO_PERM, 2);
	else
	{
		w_oldpwd = env_w_search(data, "OLDPWD");
		if (w_oldpwd)
		{
			*w_oldpwd = ft_clean_strjoin("OLDPWD=", getcwd(0, 0), 1);
			if (!*w_oldpwd)
				mini_liberate_all(data, MALLOC_FAILURE, 1);
		}
		chdir(path);
		data->error = 0;
	}
}

void mini_cd(t_data *data, char **cmd)
{
	data->error = 1;
	if (ft_tab2len(cmd) > 2)
	{
		last_cmd(data, NULL, cmd);
		ft_putstr_fd(CD_TOO_MANY_ARG, 2);
		return ;
	}
	else if (ft_tab2len(cmd) == 1 || (cmd[1][0] == '~' && cmd[1][1] == 0))
		cd_home(data);
	else if (cmd[1][0] == '-' && cmd[1][1] == 0)
		cd_oldpwd(data);
	else if (cmd[1][0] == '.' && cmd[1][1] == '.' && cmd[1][2] == 0)
		cd_parente(data);
	else
		cd_chdir(data, cmd[1]);
}
