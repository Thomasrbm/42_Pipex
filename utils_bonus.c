/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:42:09 by marvin            #+#    #+#             */
/*   Updated: 2025/10/13 05:57:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*get_abs_path(char *path, char *cmd)
{
	char	**to_try;
	char	*merged_try;
	int		i;

	i = 0;
	to_try = ft_split(path, ':');
	if (!to_try)
		return (NULL);
	while (to_try[i])
	{
		merged_try = ft_strjoin3(to_try[i], "/", cmd);
		if (access(merged_try, X_OK) == 0)
		{
			free_split(to_try);
			return (merged_try);
		}
		free(merged_try);
		i++;
	}
	free_split(to_try);
	return (NULL);
}

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

int	init_pipes(struct s_shared *shared)
{
	int	i;

	shared->pipefd = malloc(sizeof(int *) * shared->pipecount);
	if (!shared->pipefd)
		return (1);
	i = 0;
	while (i < shared->pipecount)
	{
		shared->pipefd[i] = malloc(sizeof(int) * 2);
		if (!shared->pipefd[i] || pipe(shared->pipefd[i]) < 0)
		{
			perror("pipe");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_shared(struct s_shared *shared, char **env, int ac, char **av)
{
	if (!shared)
		return (1);
	shared->env = env;
	shared->path = get_path(env);
	shared->here_doc = 0;
	shared->pid = NULL;
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		shared->here_doc = 1;
		shared->pipecount = ac - EXEC_FILE_HD_LIM_AND_LASTCMD;
		if (pipe(shared->here_doc_pipe) < 0)
		{
			perror("pipe");
			return (1);
		}
	}
	else
		shared->pipecount = ac - EXEC_FILES_AND_LASTCMD;
	shared->cmd_count = shared->pipecount + 1;
	if (init_pipes(shared))
		return (1);
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
