/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:27:18 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/27 18:27:32 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_files(t_fds *fds, int arc, char **arv)
{
	fds->f1 = open(arv[1], O_RDONLY);
	fds->f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (fds->f1 < 0)
	{
		ft_putstr_fd("zsh: no such file or directory: ", 2);
		err_exit(NULL, NULL, fds->f2, arv[1]);
	}
	if (fds->f2 < 0)
		err_exit(NULL, NULL, fds->f1, "Open f2 error.");
}

char	*valid_path(char **cmd_arr, int fd, char **paths_arr)
{
	char	*path_end;
	char	*path;
	int		i;

	i = 0;
	path_end = ft_strjoin("/", cmd_arr[0]);
	if (!path_end)
		err_exit(cmd_arr, paths_arr, fd, "Malloc Error");
	while (paths_arr[i])
	{
		path = ft_strjoin(paths_arr[i], path_end);
		if (!path)
		{
			free(path_end);
			err_exit(cmd_arr, paths_arr, fd, "Malloc Error");
		}
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free(path_end);
	ft_putstr_fd("zsh: command not found: ", 2);
	err_exit(cmd_arr, paths_arr, fd, cmd_arr[0]);
	return (NULL);
}

char	*get_exec_path(char **cmd_arr, int fd, char **envp)
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
		return (NULL);
	env_paths_arr = ft_split(env_paths_str, ':');
	free (env_paths_str);
	if (!env_paths_arr)
		return (NULL);
	return (valid_path(cmd_arr, fd, env_paths_arr));
}

void	child_process(t_fds fds, int *pipe_end, char *cmd, char **envp)
{
	char	*path;
	char	**cmd_arr;

	close(pipe_end[0]);
	if (dup2(pipe_end[1], STDOUT_FILENO) < 0)
		close_fds_exit(fds, pipe_end, "Dup error: ");
	close(pipe_end[1]);
	if (dup2(fds.prevpipe, STDIN_FILENO) < 0)
		close_fds_exit(fds, pipe_end, "Dup error: ");
	close(fds.prevpipe);
	cmd_arr = pipex_split(cmd, ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, fds.f2, "Malloc Error");
	path = get_exec_path(cmd_arr, fds.f2, envp);
	if (!path)
		err_exit(cmd_arr, NULL, fds.f2, "zsh: command not found");
	execve(path, cmd_arr, envp);
	free(path);
	err_exit(NULL, cmd_arr, fds.f2, "Execve error");
}

void	last_child_process(int prevpipe, int f2, char *cmd, char **envp)
{
	char	*path;
	char	**cmd_arr;

	if (dup2(prevpipe, STDIN_FILENO) < 0)
	{
		close(prevpipe);
		err_exit(NULL, NULL, f2, "Pipe error");
	}
	close(prevpipe);
	if (dup2(f2, STDOUT_FILENO) < 0)
		err_exit(NULL, NULL, f2, "Pipe error");
	close(f2);
	cmd_arr = pipex_split(cmd, ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, -1, "Malloc Error");
	path = get_exec_path(cmd_arr, -1, envp);
	if (!path)
		err_exit(cmd_arr, NULL, -1, "zsh: command not found");
	execve(path, cmd_arr, envp);
	free(path);
	err_exit(NULL, cmd_arr, -1, "Execve error");
}
