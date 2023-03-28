/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 19:18:36 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/28 15:44:50 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

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

void	unlink_heredoc(void)
{
	if (access(".heredoc.tmp", F_OK) != -1)
	{
		if (unlink(".heredoc.tmp") != 0)
		{
			perror("Failed to delete temporary file");
			exit(EXIT_FAILURE);
		}
	}
}

void	err_exit(char **mem1, char **mem2, int fd, char *message)
{
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	if (mem1)
		free_arr(mem1);
	if (mem2)
		free_arr(mem2);
	if (fd != -1)
		close(fd);
	unlink_heredoc();
	exit(EXIT_FAILURE);
}

void	close_fds(t_fds fds, int *pipe_end)
{
	if (fds.f1 != -1)
		close(fds.f1);
	if (fds.f2 != -1)
		close(fds.f2);
	if (fds.prevpipe != -1)
		close(fds.prevpipe);
	if (pipe_end[0] != -1)
		close(pipe_end[0]);
	if (pipe_end[1] != -1)
		close(pipe_end[1]);
}

void	close_fds_exit(t_fds fds, int *pipe_end, char *message)
{
	close_fds(fds, pipe_end);
	unlink_heredoc();
	perror(message);
	exit(EXIT_FAILURE);
}
