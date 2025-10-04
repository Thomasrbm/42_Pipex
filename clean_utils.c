/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:13 by throbert          #+#    #+#             */
/*   Updated: 2025/10/03 20:37:15 by marvin           ###   ########.fr       */
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

void	cleanup_exit(char **cmd, struct s_shared shared, int exit_code)
{
	close_pipes(&shared);
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

void	close_pipes(struct s_shared *shared)
{
	close(shared->pipefd[READING_0]);
	close(shared->pipefd[WRITING_1]);
}
