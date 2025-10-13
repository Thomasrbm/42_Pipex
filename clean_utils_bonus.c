/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:42:05 by marvin            #+#    #+#             */
/*   Updated: 2025/10/13 05:53:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	wait_for_children(pid_t *pid, int cmd_count)
{
	int	status;
	int	i;

	i = 0;
	while (i < cmd_count)
		waitpid(pid[i++], &status, 0);
	return (WEXITSTATUS(status));
}

void	cleanup_exit(char **cmd, struct s_shared shared, int exit_code)
{
	close_all_pipes(&shared);
	free_split(cmd);
	exit(exit_code);
}

void	clean_and_exit(char **cmd, char *abs_path, int exit_code)
{
	if (cmd)
		free_split(cmd);
	if (abs_path)
		free(abs_path);
	exit(exit_code);
}

void	close_all_pipes(t_shared *sh)
{
	int	i;

	i = 0;
	while (i < sh->pipecount)
	{
		if (sh->pipefd[i][0] != -1)
			close(sh->pipefd[i][0]);
		if (sh->pipefd[i][1] != -1)
			close(sh->pipefd[i][1]);
		i++;
	}
}

void	free_pipes(t_shared *sh)
{
	int	i;

	if (!sh->pipefd)
		return ;
	i = 0;
	while (i < sh->pipecount)
	{
		free(sh->pipefd[i]);
		i++;
	}
	free(sh->pipefd);
	sh->pipefd = NULL;
}
