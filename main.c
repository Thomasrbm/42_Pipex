/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/09/12 00:46:17 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **env)
{
	int		i;
	char	*to_find;

	i = 0;
	to_find = "PATH=";
	while (env[i] != NULL)
	{
		if (ft_strcmp(to_find, &env[i][0]) == 0)
			break ;
		i++;
	}
	ft_substr_end(&env[i][0], PATH_LENGTH);
	return (&env[i][0]);
}

char	**get_cmd(char **av, pid_t pid)
{
	char	**cmd;

	cmd = NULL;
	if (pid == 0)
	{
		if (ft_strchr(av[2], ' '))
			ft_split(av[2], ' ');
		else
		{
			cmd = malloc(sizeof(char *) * 2);
			if (!cmd)
				return (NULL);
			cmd[0] = alloc_cmd(&av[2][0]);
			cmd[1] = NULL;
		}
	}
	else
	{
		if (ft_strchr(av[3], ' '))
			ft_split(av[3], ' ');
		else
		{
			cmd = malloc(sizeof(char *) * 2);
			if (!cmd)
				return (NULL);
			cmd[0] = alloc_cmd(&av[3][0]);
			cmd[1] = NULL;
		}
	}
	return (cmd);
}

char	*get_abs_path(char *path, char *cmd)
{
	char	**to_try;
	int		i;

	i = 0;
	to_try = ft_split(path, ':');
	if (to_try == NULL)
		return (NULL);
	while (to_try[i] != NULL)
	{
		ft_strjoin(to_try[i], cmd);
		if (access(to_try[i], X_OK) == 0)
			break ;
		i++;
	}
	return (to_try[i]);
}

void	ft_exec(char *absolute_path, char **cmd, char **env, pid_t pid)
{
	int	pipefd[2];
	int	pipe_success;

	pipe_success = pipe(pipefd);
	if (pipe_success < 0)
	{
		free(cmd);
		free(absolute_path);
		exit(1);
	}
	if (pid != 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		if (execve(absolute_path, cmd, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		dup2(pipefd[1], STDOUT_FILENO);
		if (execve(absolute_path, cmd, env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	close(pipefd[1]);
}

int	main(int ac, char **av, char **env)
{
	pid_t	pid;
	char	**cmd;
	char	*path;
	char	*absolute_path;

	if (ac != 5)
		return (ft_putstr_fd_int("Wrong number of arguments", 2));
	path = get_path(env);
	pid = fork();
	cmd = get_cmd(av, pid);
	absolute_path = get_abs_path(path, cmd[0]);
	for (int i = 0; cmd[i] != NULL; i++)
		printf("cmd : %s\n", cmd[i]);
	if (pid != 0)
	{
		ft_redirect("STDIN TO INFILE", av[1]);
		ft_exec(absolute_path, cmd, env, pid);
	}
	else
	{
		ft_redirect("STDOUT TO OUTFILE", av[4]);
		ft_exec(absolute_path, cmd, env, pid);
	}
	return (0);
}
