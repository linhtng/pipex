/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:33:43 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/27 18:34:18 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "./ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_fds
{
	int     f1;
    int     f2;
    int     prevpipe;
}	t_fds;

void	free_arr(char **arr);

void	err_exit(char **mem1, char **mem2, int fd, char *message);
void	unlink_heredoc();

void	open_files(t_fds *fds, int arc, char **arv);
void	close_fds_exit(t_fds fds, int *pipe_end, char *message);


char	*get_exec_path(char **cmd_arr, int fd, char **envp);

void	here_doc_init(int arc, char **arv, t_fds *fds, int *prevpipe);

void	child_process(t_fds fds, int *pipe_end, char *cmd, char **envp);
void	last_child_process(int prevpipe, int f2, char *cmd, char **envp);

#endif
