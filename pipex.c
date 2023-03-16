/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:21:40 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/16 16:11:05 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_exec_path(char **cmd_arr, char **envp)
{
	int		i;
	char	*env_paths_str;
	char	**env_paths_arr;

	i = 0;
	while (envp[i++])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
			break ;
	}
	env_paths_str = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	if (!env_paths_str)
		return (NULL);
	env_paths_arr = ft_split(env_paths_str, ':');
	free (env_paths_str);
	if (!env_paths_arr)
		return (NULL);
	return (valid_path(cmd_arr, env_paths_arr));
}

void	child_process(int f1, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;

	cmd_arr = ft_split(arv[2], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "Malloc Error");
	dup2(f1, STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	close(end[0]);
	close(f1);
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}

void	parent_process(int f2, int *end, char **arv, char **envp)
{
	char	*path;
	char	**cmd_arr;
	int		status;

	waitpid(-1, &status, 0);
	cmd_arr = ft_split(arv[3], ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "Malloc Error");
	dup2(f2, STDOUT_FILENO);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	close(f2);
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}

void	pipex(int f1, int f2, char **arv, char **envp)
{
	int		end[2];
	pid_t	parent;

	pipe(end);
	parent = fork();
	if (parent < 0)
		return (perror("Fork: "));
	if (!parent)
		child_process(f1, end, arv, envp);
	else
		parent_process(f2, end, arv, envp);
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
			perror("Open file error");
			return (EXIT_FAILURE);
		}
		pipex(f1, f2, arv, envp);
	}
	else
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
	return (0);
}
