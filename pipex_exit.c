/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 19:22:35 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/25 19:22:56 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	err_exit(char **mem1, char **mem2, t_fds fds, char *message)
{
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	if (mem1)
		free_arr(mem1);
	if (mem2)
		free_arr(mem2);
	if (fds.f1 != -1)
		close(fds.f1);
	if (fds.f2 != -1)
		close(fds.f2);
	exit(EXIT_FAILURE);
}

void	close_fds(t_fds fds, int *pipe_end)
{
	if (fds.f1 != -1)
		close(fds.f1);
	if (fds.f2 != -1)
		close(fds.f2);
	close(pipe_end[0]);
	close(pipe_end[1]);
}

void	close_fds_exit(t_fds fds, int *pipe_end, char *message)
{
	close_fds(fds, pipe_end);
	perror(message);
	exit(EXIT_FAILURE);
}
