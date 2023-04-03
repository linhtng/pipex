/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:03:54 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/28 15:45:09 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	open_files(t_fds *fds, int arc, char **arv)
{
	fds->f1 = open(arv[1], O_RDONLY);
	fds->f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fds->f1 < 0)
	{
		ft_putstr_fd("zsh: no such file or directory: ", 2);
		err_exit(NULL, NULL, *fds, arv[1]);
	}
	if (fds->f2 < 0)
		err_exit(NULL, NULL, *fds, "Open f2 error.");
}

char	*valid_path(char **cmd_arr, t_fds fds, char **paths_arr)
{
	char	*path_end;
	char	*path;
	int		i;

	i = 0;
	path_end = ft_strjoin("/", cmd_arr[0]);
	if (!path_end)
		err_exit(cmd_arr, paths_arr, fds, "Malloc Error");
	while (paths_arr[i])
	{
		path = ft_strjoin(paths_arr[i], path_end);
		if (!path)
		{
			free(path_end);
			err_exit(cmd_arr, paths_arr, fds, "Malloc Error");
		}
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free(path_end);
	ft_putstr_fd("zsh: command not found: ", 2);
	err_exit(cmd_arr, paths_arr, fds, cmd_arr[0]);
	return (NULL);
}

char	*get_exec_path(char **cmd_arr, t_fds fds, char **envp)
{
	int		i;
	char	*env_paths_str;
	char	**env_paths_arr;

	i = 0;
	if (access(cmd_arr[0], X_OK) == 0)
		return (cmd_arr[0]);
	while (envp[i++])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			break ;
	}
	env_paths_str = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	if (!env_paths_str)
		err_exit(cmd_arr, NULL, fds, "Malloc Error");
	env_paths_arr = ft_split(env_paths_str, ':');
	free (env_paths_str);
	if (!env_paths_arr)
		err_exit(cmd_arr, NULL, fds, "Malloc Error");
	return (valid_path(cmd_arr, fds, env_paths_arr));
}
