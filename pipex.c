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

char	*get_exec_path(char **cmd_arr, char **envp)
{
	int		i;
	char	*env_paths_str;
	char	**env_paths_arr;

	i = 0;
	if (access(cmd_arr[0], X_OK) == 0)
		return (cmd_arr[0]);
	while (envp[i++])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
			break ;
	}
	env_paths_str = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	if (!env_paths_str)
		err_exit(cmd_arr, NULL, "Malloc Error");
	env_paths_arr = ft_split(env_paths_str, ':');
	free (env_paths_str);
	if (!env_paths_arr)
		err_exit(cmd_arr, NULL, "Malloc Error");
	return (valid_path(cmd_arr, env_paths_arr));
}

void	child1_process(int f1, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;

	dup2(f1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(f1);
	cmd_arr = ft_split(arv[2], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "zsh: command not found");
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}

void	child2_process(int f2, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;

	dup2(f2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(f2);
	cmd_arr = ft_split(arv[3], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "zsh: command not found");
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}

void	pipex(int f1, int f2, char **arv, char **envp)
{
	int		end[2];
	int		status;
	pid_t	child1;
	pid_t	child2;

	pipe(end);
	child1 = fork();
	if (child1 < 0)
		return (perror("Fork: "));
	if (child1 == 0)
		child1_process(f1, end, arv, envp);
	child2 = fork();
	if (child2 < 0)
		return (perror("Fork: "));
	if (child2 == 0)
		child2_process(f2, end, arv, envp);
	close(end[0]);
	close(end[1]);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
}

int	main(int arc, char **arv, char **envp)
{
	int	f1;
	int	f2;

	if (arc == 5)
	{
		f1 = open(arv[1], O_RDONLY);
		f2 = open(arv[4], O_CREAT | O_RDWR | O_TRUNC, 0777);
		if (f1 < 0 || f2 < 0)
		{
			ft_putstr_fd("zsh: no such file or directory: ", 2);
			err_exit(NULL, NULL, arv[1]);
			return (EXIT_FAILURE);
		}
		pipex(f1, f2, arv, envp);
	}
	else
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
	return (0);
}
