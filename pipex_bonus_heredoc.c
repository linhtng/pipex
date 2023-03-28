/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 18:41:12 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/27 18:41:23 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	here_doc_input(char *limiter, size_t lim_len, t_fds fds)
{
	char	*buf;
	int		tmp_fd;

	tmp_fd = open(".heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (tmp_fd < 0)
		err_exit(NULL, NULL, fds.f2, "Open .heredoc.tmp error");
	buf = NULL;
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 1);
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			err_exit(NULL, NULL, fds.f2, "Get next line error.");
		if (ft_strlen(buf) == (lim_len + 1)
			&& ft_strncmp(buf, limiter, lim_len) == 0)
		{
			free(buf);
			break ;
		}
		ft_putstr_fd(buf, tmp_fd);
		free(buf);
	}
	close(tmp_fd);
}

void	here_doc_init(int arc, char **arv, t_fds *fds, int *prevpipe)
{
	fds->f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_APPEND, 0777);
	if (fds->f2 < 0)
		err_exit(NULL, NULL, -1, "Open f2 error.");
	here_doc_input(arv[2], ft_strlen(arv[2]), *fds);
	*prevpipe = open(".heredoc.tmp", O_RDONLY);
	if (*prevpipe < 0)
		err_exit(NULL, NULL, fds->f2, "Open .heredoc.tmp error.");
}
