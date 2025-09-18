/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/09/18 20:11:51 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **env)
{
	int		i;
	char	*to_find;
	char	*result;

	i = 0;
	to_find = "PATH=";
	while (env[i] != NULL)
	{
		if (ft_strncmp(to_find, &env[i][0], 5) == 0)
			break ;
		i++;
	}
	result = ft_substr_end(&env[i][0], PATH_LENGTH);
	return (result);
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
	cmd = ft_strjoin("/", &cmd[0]);
	while (to_try[i] != NULL)
	{
		merged_try = ft_strjoin(to_try[i], cmd);
		if (access(merged_try, X_OK) == 0)
			break ;
		i++;
	}
	return (merged_try);
}

void	ft_exec_cmd1(char *absolute_path, char **cmd, char **env, char *file)
{
	int	fd_file;
	int	pipefd[2];
	int	pipe_success;

	pipe_success = pipe(pipefd);
	if (pipe_success < 0)
		exit(1);
	fd_file = open(file, O_RDONLY);
	dup2(fd_file, STDIN_FILENO);
	close(fd_file);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	close(pipefd[0]);
	if (execve(absolute_path, cmd, env) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	ft_exec_cmd2(char *absolute_path, char **cmd, char **env, char *file)
{
	int	fd_file;
	int	pipefd[2];
	int	pipe_success;

	pipe_success = pipe(pipefd);
	if (pipe_success < 0)
		exit(1);
	fd_file = open(file, O_CREAT | O_RDWR | O_TRUNC, 0755);
	dup2(fd_file, STDOUT_FILENO);
	close(fd_file);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	if (execve(absolute_path, cmd, env) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

// pb si commande avec flag
int	main(int ac, char **av, char **env)
{
	pid_t	pid[2];
	int		status;

	if (ac != 5)
		return (ft_putstr_fd_int("Wrong number of arguments", 2));
	pid[0] = fork();
	if (!pid[0])
	{
		ft_exec_cmd1(get_abs_path(get_path(env), ft_strjoin("/", &av[2][0])),
			ft_split(av[2], ' '), env, av[1]);
	}
	pid[1] = fork();
	if (!pid[1])
	{
		ft_exec_cmd2(get_abs_path(get_path(env), ft_strjoin("/", &av[3][0])),
			ft_split(av[3], ' '), env, av[4]);
	}
	while (waitpid(-1, &status, 0) == -1)
		;
	status = WEXITSTATUS(status);
	return (status);
}
