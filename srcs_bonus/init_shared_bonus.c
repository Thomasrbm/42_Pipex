/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shared_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 20:14:17 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 01:18:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipes(t_shared *shared, char **av)
{
	int	i;

	i = 0;
	while (i < shared->pipecount)
	{
		shared->pipefd[i] = malloc(sizeof(int) * 2);
		if (pipe(shared->pipefd[i]) < 0)
		{
			perror("pipe");
			return (0);
		}
		i++;
	}
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		if (pipe(shared->heredoc_pipes) < 0)
		{
			perror("pipe");
			return (0);
		}
	}
	return (1);
}

void	init_shared_if_heredoc(t_shared *shared, char **av, int ac)
{
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		shared->heredoc_flag = 1;
		shared->cmd_count = ac - SKIP_PIPEX_HD_DEL_OUTFILE;
		shared->delim = av[2];
	}
}

char	*get_path(char **env)
{
	int		i;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (strncmp("PATH=", env[i], 5) == 0)
		{
			path = &env[i][5]; // adresse pour que le ptr parte de la, sinon prend la value du char
			break ;
		}
		i++;
	}
	if (!env[i])
		path = NULL;
	return (path);
}

int	init_shared(t_shared *shared, int ac, char **av, char **env)
{
	shared->env = env;
	shared->path = get_path(env);
	if (!shared->path)
	{
		ft_putstr_fd("Unset path", 2);
		return (0);
	}
	shared->heredoc_flag = 0;
	shared->cmd_count = ac - SKIP_PIPEX_INOUTFILES;
	init_shared_if_heredoc(shared, av, ac);
	shared->pipecount = shared->cmd_count - LAST_CMD;
	shared->pipefd = malloc(sizeof(int *) * shared->pipecount);
	shared->pid_tab = malloc(sizeof(pid_t) * shared->cmd_count);
	if (!shared->pipefd || !shared->pid_tab)
	{
		perror("malloc");
		return (0);
	}
	if (!init_pipes(shared, av))
		return (0);
	shared->status = 0;
	if (shared->heredoc_flag == 0)
		shared->infile = av[1];
	shared->outfile = av[ac - 1];
	return (1);
}
