/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:42:09 by marvin            #+#    #+#             */
/*   Updated: 2025/10/09 14:42:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

int	init_shared(struct s_shared *shared, char **env)
{
	if (!shared)
		return (1);
	shared->env = env;
	shared->path = get_path(env);
	if (pipe(shared->pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}
