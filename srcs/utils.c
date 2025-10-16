/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 20:37:01 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 04:26:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **env)
{
	int		i;
	char	*to_find;

	i = 0;
	to_find = "PATH=";
	while (env[i])
	{
		if (ft_strncmp(to_find, env[i], 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

int	init_shared(t_shared *shared, char **env)
{
	if (!shared)
		return (1);
	shared->env = env;
	shared->path = get_path(env);
	if (pipe(shared->pipefd) < 0)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}
