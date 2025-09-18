/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:13 by throbert          #+#    #+#             */
/*   Updated: 2025/09/18 19:25:49 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"



char	*alloc_cmd(char *cmd)
{
	int		size;
	char	*new_cmd;

	size = ft_strlen(cmd);
	new_cmd = malloc(size);
	new_cmd = cmd;
	return (new_cmd);
}
