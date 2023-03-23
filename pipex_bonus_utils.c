/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thuynguy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:13:34 by thuynguy          #+#    #+#             */
/*   Updated: 2023/03/20 14:15:47 by thuynguy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	put_usage(void)
{
	ft_printf("Usage: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n");
	ft_printf("Usage: ./pipex here_doc LIMITER cmd cmd1 file2\n");
}
