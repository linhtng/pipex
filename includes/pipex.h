/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 17:43:30 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/08 17:45:22 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
/* # include <string.h>
# include <errno.h> */

typedef struct s_fds
{
	int     f1;
    int     f2;
}	t_fds;

void	free_arr(char **arr);
void	err_exit(char **mem1, char **mem2, t_fds fds, char *message);
char	*valid_path(char **cmd_arr, t_fds fds, char **paths_arr);
void	open_files(t_fds *fds, int arc, char **arv);
void	close_fds(t_fds fds, int *pipe_fd);
void	close_fds_exit(t_fds fds, int *pipe_end, char *message);
char	*get_exec_path(char **cmd_arr, t_fds fds, char **envp);

#endif
