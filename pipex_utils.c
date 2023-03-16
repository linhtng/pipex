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

void	err_exit(char **mem1, char **mem2, char *message)
{
	if (mem1)
		free_arr(mem1);
	if (mem2)
		free_arr(mem2);
	perror(message);
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
		i++;
	}
	free(path_end);
	err_exit(cmd_arr, paths_arr, "Command not found");
	return (NULL);
}
