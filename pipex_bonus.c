/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:21:40 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/21 15:35:20 by thuynguy         ###   ########.fr       */
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

void	ft_pipe(int *prevpipe, int *heredoc, char *cmd, char **envp)
{
	int		pipe_end[2];
	pid_t	child;

	if (pipe(pipe_end) == -1)
		return (perror("Pipe: "));
	child = fork();
	if (child < 0)
		return (perror("Fork: "));
	if (child == 0)
		child_process(*prevpipe, pipe_end, cmd, envp);
	else
	{
		close(pipe_end[1]);
		close(*prevpipe);
		*prevpipe = pipe_end[0];
		if (*heredoc)
		{
			unlink(".heredoc.tmp");
			*heredoc = 0;
		}
	}
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

void	pipe_last(int prevpipe, int f2, char *cmd, char **envp)
{
	int		status;
	pid_t	child;

	child = fork();
	if (child < 0)
		return (perror("Fork: "));
	if (child == 0)
		last_child_process(prevpipe, f2, cmd, envp);
	else
	{
		close(prevpipe);
		while (wait(&status) > 0)
			;
	}
}

void	pipex_b(int pipe_num, int arc, char **arv, char **envp)
{
	int	i;
	int	f1;
	int	f2;
	int	prevpipe;

	i = 2;
	open_files(&f1, &f2, arc, arv);
	prevpipe = dup(f1);
	while (pipe_num >= 0)
	{
		if (pipe_num)
			ft_pipe(&prevpipe, 0, arv[i], envp);
		else
			pipe_last(prevpipe, f2, arv[i], envp);
		pipe_num--;
		i++;
	}
}

void here_doc_input(char *limiter)
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
		if (ft_strlen(buf) == (ft_strlen(limiter) + 1) && ft_strncmp(buf, limiter, ft_strlen(limiter)) == 0)
		{
			close(stdin_fd);
			break;
		}
		ft_putstr_fd(buf, tmp_fd);
		free(buf);
	}
	close(tmp_fd);
}

void	here_doc(int pipe_num, int arc, char **arv, char **envp)
{
	int	i;
	int	f2;
	int	prevpipe;
	int	heredoc;

	i = 3;
	f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_APPEND, 0777);
	if (f2 < 0)
		return (perror("Open file: "));
	here_doc_input(arv[2]);
	prevpipe = open(".heredoc.tmp", O_CREAT | O_RDWR | O_APPEND, 0777);;
	if (prevpipe < 0)
		return (perror("Heredoc: "));
	heredoc = 1;
	while (pipe_num >= 0)
	{
		if (pipe_num)
			ft_pipe(&prevpipe, &heredoc, arv[i], envp);
		else
			pipe_last(prevpipe, f2, arv[i], envp);
		pipe_num--;
		i++;
	}
}

int	main(int arc, char **arv, char **envp)
{
	int	pipe_num;

	if (arc >= 5)
	{
		if (ft_strncmp(arv[1], "here_doc", ft_strlen(arv[1])))
		{
			pipe_num = arc - 4;
			pipex_b(pipe_num, arc, arv, envp);
		}
		else
		{
			pipe_num = arc - 5;
			here_doc(pipe_num, arc, arv, envp);
		}
	}
	else
		put_usage();
	return (0);
}
