/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:13 by throbert          #+#    #+#             */
/*   Updated: 2025/09/17 15:42:01 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_redirect(char *redirect, char *file)
{
	int	fd_file;

	fd_file = open(file, O_RDWR);
	if (ft_strcmp(redirect, "STDIN TO INFILE") == 0)
		dup2(fd_file, STDIN_FILENO);
	else if (ft_strcmp(redirect, "STDOUT TO OUTFILE") == 0)
		dup2(fd_file, STDOUT_FILENO);
}

char	*alloc_cmd(char *cmd)
{
	int		size;
	char	*new_cmd;

	size = ft_strlen(cmd);
	new_cmd = malloc(size);
	new_cmd = cmd;
	return (new_cmd);
}
