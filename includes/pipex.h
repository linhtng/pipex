/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 17:43:30 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/29 18:37:21 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>

typedef struct s_fds
{
	int	f1;
	int	f2;
}	t_fds;

typedef struct s_spldata
{
	size_t	i;
	int		j;
	int		start;
	int		in_quotes;
	size_t	slen;
}	t_spldata;

/* pipex_split_quote */
int		count_with_quotes(char const *s, char const c, int quotes);
char	**make_arr_quotes(char const*s, char c, int quotes);
void	do_split_with_quotes(char **arr, char const *s, char c);
char	**pipex_split(char const *s, char c);
int		count_occurences(const char *str, char c);
int		free_arr_spl(char **arr, size_t n);

void	free_arr(char **arr);
void	err_exit(char **mem1, char **mem2, t_fds fds, char *message);
char	*valid_path(char **cmd_arr, t_fds fds, char **paths_arr);
void	open_files(t_fds *fds, int arc, char **arv);
void	close_fds(t_fds fds, int *pipe_fd);
void	close_fds_exit(t_fds fds, int *pipe_end, char *message);
char	*get_exec_path(char **cmd_arr, t_fds fds, char **envp);

#endif
