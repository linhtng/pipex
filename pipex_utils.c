/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:03:54 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/16 16:04:22 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	open_files(int *f1, int *f2, int arc, char **arv)
{
	*f1 = open(arv[1], O_RDONLY);
	if (*f1 < 0)
	{
		ft_putstr_fd("zsh: no such file or directory: ", 2);
		err_exit(NULL, NULL, arv[1]);
	}
	*f2 = open(arv[arc - 1], O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (*f2 < 0)
	{
		if (*f1 != -1)
			close(*f1);
		return (perror("Open f2 error: "));
	}
}

void	err_exit(char **mem1, char **mem2, char *message)
{
	ft_putstr_fd(message, 2);
	ft_putchar_fd('\n', 2);
	if (mem1)
		free_arr(mem1);
	if (mem2)
		free_arr(mem2);
	exit(EXIT_FAILURE);
}

char	*valid_path(char **cmd_arr, char **paths_arr)
{
	char	*path_end;
	char	*path;
	int		i;

	i = 0;
	path_end = ft_strjoin("/", cmd_arr[0]);
	if (!path_end)
		err_exit(cmd_arr, paths_arr, "Malloc Error");
	while (paths_arr[i])
	{
		path = ft_strjoin(paths_arr[i], path_end);
		if (!path)
		{
			free(path_end);
			err_exit(cmd_arr, paths_arr, "Malloc Error");
		}
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	free(path_end);
	ft_putstr_fd("zsh: command not found: ", 2);
	err_exit(cmd_arr, paths_arr, cmd_arr[0]);
	return (NULL);
}
