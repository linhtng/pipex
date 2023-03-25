/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:21:40 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/19 18:24:18 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	child1_process(t_fds fds, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;

	if (dup2(fds.f1, STDIN_FILENO) < 0)
		close_fds_exit(fds, end, "Dup error: ");
	if (dup2(end[1], STDOUT_FILENO) < 0)
		close_fds_exit(fds, end, "Dup error: ");
	close_fds(fds, end);
	cmd_arr = ft_split(arv[2], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, fds, "Malloc Error");
	path = get_exec_path(cmd_arr, fds, envp);
	if (!path)
		err_exit(cmd_arr, NULL, fds, "zsh: command not found");
	execve(path, cmd_arr, envp);
	free(path);
	err_exit(NULL, cmd_arr, fds, "Execve error");
}

void	child2_process(t_fds fds, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;

	if (dup2(fds.f2, STDOUT_FILENO) < 0)
		close_fds_exit(fds, end, "Dup error: ");
	if (dup2(end[0], STDIN_FILENO) < 0)
		close_fds_exit(fds, end, "Dup error: ");
	close_fds(fds, end);
	cmd_arr = ft_split(arv[3], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, fds, "Malloc Error");
	path = get_exec_path(cmd_arr, fds, envp);
	if (!path)
		err_exit(cmd_arr, NULL, fds, "zsh: command not found");
	execve(path, cmd_arr, envp);
	free(path);
	err_exit(NULL, cmd_arr, fds, "Execve error");
}

void	pipex(t_fds fds, char **arv, char **envp)
{
	int		end[2];
	int		status;
	pid_t	child1;
	pid_t	child2;

	if (pipe(end) < 0)
		err_exit(NULL, NULL, fds, "Pipe: ");
	child1 = fork();
	if (child1 < 0)
		close_fds_exit(fds, end, "Fork: ");
	if (child1 == 0)
		child1_process(fds, end, arv, envp);
	child2 = fork();
	if (child2 < 0)
		close_fds_exit(fds, end, "Fork: ");
	if (child2 == 0)
		child2_process(fds, end, arv, envp);
	close_fds(fds, end);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
}

int	main(int arc, char **arv, char **envp)
{
	t_fds	fds;

	if (arc == 5)
	{
		open_files(&fds, arc, arv);
		pipex(fds, arv, envp);
	}
	else
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
	return (0);
}
