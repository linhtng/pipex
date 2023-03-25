/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:13:34 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/25 17:30:43 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
		if (!ft_strncmp(envp[i], "PATH=", 5))
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

void	here_doc_input(char *limiter, size_t lim_len)
{
	char	*buf;
	int		stdin_fd;
	int		tmp_fd;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (tmp_fd < 0)
		return (perror("Open file: "));
	stdin_fd = dup(STDIN_FILENO);
	buf = NULL;
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 1);
		buf = get_next_line(stdin_fd);
		if (!buf)
			return (perror("Get next line error: "));
		if (ft_strlen(buf) == (lim_len + 1)
			&& ft_strncmp(buf, limiter, lim_len) == 0)
		{
			close(stdin_fd);
			break ;
		}
		ft_putstr_fd(buf, tmp_fd);
		free(buf);
	}
	close(tmp_fd);
}

void	here_doc_init(int arc, char **arv, int *f2, int *prevpipe)
{
	*f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_APPEND, 0777);
	if (*f2 < 0)
		return (perror("Open file: "));
	here_doc_input(arv[2], ft_strlen(arv[2]));
	*prevpipe = open(".heredoc.tmp", O_CREAT | O_RDWR | O_APPEND, 0777);
	if (*prevpipe < 0)
		return (perror("Heredoc: "));
}

void	child_process(int prevpipe, int *pipe_end, char *cmd, char **envp)
{
	char	*path;
	char	**cmd_arr;

	close(pipe_end[0]);
	dup2(pipe_end[1], STDOUT_FILENO);
	close(pipe_end[1]);
	dup2(prevpipe, STDIN_FILENO);
	close(prevpipe);
	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "zsh: command not found");
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}

void	last_child_process(int prevpipe, int f2, char *cmd, char **envp)
{
	char	*path;
	char	**cmd_arr;

	dup2(f2, STDOUT_FILENO);
	dup2(prevpipe, STDIN_FILENO);
	close(prevpipe);
	close(f2);
	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr)
		err_exit(NULL, NULL, "Malloc Error");
	path = get_exec_path(cmd_arr, envp);
	if (!path)
		err_exit(cmd_arr, NULL, "zsh: command not found");
	execve(path, cmd_arr, envp);
	exit(EXIT_FAILURE);
}
