/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/10/16 04:58:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_absolute_path(char *path, char *cmd, t_shared *shared)
{
	char	**path_to_try;
	char    *tmp;
	char	*abs_path;
	int		i;

	i = -1;
	abs_path = NULL;
	path_to_try = ft_split(path, ':');
	while (path_to_try[++i])
	{
		tmp = ft_strjoin3(path_to_try[i], "/", cmd);
		if (access(tmp, X_OK) == 0)
		{
			abs_path = tmp;
			break ;
		}
		free(tmp);
	}
	free_split(path_to_try);
	if (!abs_path)
	{
		cmd_not_found(shared->cmd, shared);
		clean_exit(shared, shared->cmd, CMD_NOT_FOUND);
	}
	return (abs_path);
}

void	infile_cmd1(t_shared *shared, char *file)
{
	int	infile;

	infile = open(file, O_RDONLY);
	if (infile < 0)
	{
		close_pipes(shared);
		perror("open: infile");
		exit(0);
	}
	dup2(infile, STDIN_FILENO);
	dup2(shared->pipefd[WRITE_1], STDOUT_FILENO);
	close(infile);
	close_pipes(shared);
}

void	cmd2_outfile(t_shared *shared, char *file)
{
	int	outfile;

	outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close_pipes(shared);
		perror("open: outfile");
		exit(0);
	}
	dup2(shared->pipefd[READ_0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close_pipes(shared);
}

void	ft_redirect(t_shared *shared, char ** av, int cmd_type)
{
	if (cmd_type == 1)
		infile_cmd1(shared, av[1]);
	else
		cmd2_outfile(shared, av[4]);
}

int	exec_cmds(t_shared *shared, char **av, int cmd_type)
{
	pid_t	pid;
	char	*abs_path;

	pid = fork();
	if (pid == 0)
	{
		if (cmd_type == 1)
			shared->cmd = ft_split(av[2], ' ');
		else
			shared->cmd = ft_split(av[3], ' ');
		abs_path = get_absolute_path(shared->path, shared->cmd[0], shared);
		ft_redirect(shared, av, cmd_type);
		if (execve(abs_path, shared->cmd, shared->env) == -1)
		{
			perror("execve");
			free(abs_path);
			clean_exit(shared, shared->cmd, CMD_NOT_EXECUTABLE);
		}
	}
	return (pid);
}
