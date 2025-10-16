/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:13 by throbert          #+#    #+#             */
/*   Updated: 2025/10/16 04:11:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	wait_for_children(pid_t *pid)
{
	int	status;

	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
	return (WEXITSTATUS(status));
}

void	clean_exit(t_shared *shared, char **cmd, int exit_code)
{
	close_pipes(shared);
	if (cmd)
		free_split(cmd);
	exit(exit_code);
}

void	close_pipes(t_shared *shared)
{
	close(shared->pipefd[READ_0]);
	close(shared->pipefd[WRITE_1]);
}

void	cmd_not_found(char **cmd, t_shared *shared)
{
	close_pipes(shared);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
}
