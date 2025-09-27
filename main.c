/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/09/27 03:55:03 by marvin           ###   ########.fr       */
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
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
		i++;
	}
	return (NULL);
}

char	*get_abs_path(char *path, char *cmd)
{
	char	**to_try;
	char	*merged_try;
	int		i;

	i = 0;
	to_try = ft_split(path, ':');
	if (to_try == NULL)
		return (NULL);
	while (to_try[i] != NULL)
	{
		merged_try = ft_strjoin(to_try[i], cmd);
		if (access(merged_try, X_OK) == 0)
			break ;
		i++;
	}
	return (merged_try);
}

void	ft_exec_cmd1(char *absolute_path, char **cmd,
	char *file, struct s_shared shared)
{
	int	fd_file;

	fd_file = open(file, O_RDONLY);
	if (fd_file < 0)
		exit(1);
	dup2(fd_file, STDIN_FILENO);
	close(fd_file);
	dup2(shared.pipefd[1], STDOUT_FILENO);
	close(shared.pipefd[0]);
	close(shared.pipefd[1]);
	if (execve(absolute_path, cmd, shared.env) == -1)
	{
		perror("execve");
		exit(126);
	}
}

void	ft_exec_cmd2(char *absolute_path, char **cmd,
	char *file, struct s_shared shared)
{
	int	fd_file;

	fd_file = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_file < 0)
		exit(1);
	dup2(shared.pipefd[0], STDIN_FILENO);
	dup2(fd_file, STDOUT_FILENO);
	close(fd_file);
	close(shared.pipefd[0]);
	close(shared.pipefd[1]);
	if (execve(absolute_path, cmd, shared.env) == -1)
	{
		perror("execve");
		exit(126);
	}
}

int	main(int ac, char **av, char **env)
{
	pid_t			pid[2];
	char			**cmd;
	struct s_shared	shared;

	if (ac != 5)
		return (ft_putstr_fd_int("Wrong number of arguments", 2));
	if (init_shared(&shared, env))
		return (1);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		cmd = ft_split(av[2], ' ');
		ft_exec_cmd1(get_abs_path(shared.path, ft_strjoin("/", cmd[0])), cmd,
			av[1], shared);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		cmd = ft_split(av[3], ' ');
		ft_exec_cmd2(get_abs_path(shared.path, ft_strjoin("/", cmd[0])), cmd,
			av[4], shared);
	}
	close(shared.pipefd[0]);
	close(shared.pipefd[1]);
	return (wait_for_children(pid));
}
