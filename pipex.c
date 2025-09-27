/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:06:13 by throbert          #+#    #+#             */
/*   Updated: 2025/09/27 03:57:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*alloc_cmd(char *cmd)
{
	int		size;
	char	*new_cmd;

	size = ft_strlen(cmd);
	new_cmd = malloc(size + 1);
	if (!new_cmd)
		return (NULL);
	ft_strlcpy(new_cmd, cmd, size + 1);
	return (new_cmd);
}

int	ft_putstr_fd_int(char *s, int fd)
{
	while (s && *s)
	{
		write(fd, s, 1);
		s++;
	}
	write(fd, "\n", 1);
	return (1);
}

int	wait_for_children(pid_t *pid)
{
	int	status;

	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
	return (WEXITSTATUS(status));
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
