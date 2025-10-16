/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 23:08:33 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 05:02:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	wait_for_childrend(t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->cmd_count)
	{
		waitpid(shared->pid_tab[i], &shared->status, 0);
		i++;
	}
	return (WEXITSTATUS(shared->status));
}

void	close_all_pipes(t_shared *shared)
{
	int	i;

	i = 0;
	if (shared->heredoc_flag)
	{
		close(shared->heredoc_pipes[READ_0]);
		close(shared->heredoc_pipes[WRITE_1]);
	}
	while (i < shared->pipecount)
	{
		close(shared->pipefd[i][WRITE_1]);
		close(shared->pipefd[i][READ_0]);
		i++;
	}
}

void	free_all(t_shared *shared, char **cmd)
{
	int	i;

	i = 0;
	free(shared->pid_tab);
	while (i < shared->pipecount)
	{
		free(shared->pipefd[i]);
		i++;
	}
	free(shared->pipefd);
	if (cmd)
		free_split(cmd);
}

void	clean_exit(t_shared *shared, char **cmd, int exit_code)
{
	close_all_pipes(shared);
	free_all(shared, cmd);
	exit(exit_code);
}

void	cmd_not_found(char **cmd, t_shared *shared)
{
	close_all_pipes(shared);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
}
