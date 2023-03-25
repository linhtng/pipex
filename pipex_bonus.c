/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:21:40 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/25 17:33:50 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_pipe(int *prevpipe, char *cmd, char **envp)
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
	}
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
	int		i;
	t_fds	fds;
	int		prevpipe;

	i = 2;
	open_files(&fds, arc, arv);
	prevpipe = dup(fds.f1);
	close(fds.f1);
	while (pipe_num >= 0)
	{
		if (pipe_num)
			ft_pipe(&prevpipe, arv[i], envp);
		else
			pipe_last(prevpipe, fds.f2, arv[i], envp);
		pipe_num--;
		i++;
	}
}

void	here_doc(int pipe_num, int arc, char **arv, char **envp)
{
	int	i;
	int	f2;
	int	prevpipe;
	int	here_doc;

	i = 3;
	here_doc_init(arc, arv, &f2, &prevpipe);
	here_doc = 1;
	while (pipe_num >= 0)
	{
		if (pipe_num)
		{
			ft_pipe(&prevpipe, arv[i], envp);
			if (here_doc)
			{
				unlink(".heredoc.tmp");
				here_doc = 0;
			}
		}
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
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n");
		ft_printf("Usage: ./pipex here_doc LIMITER cmd cmd1 file2\n");
	}
	return (0);
}
