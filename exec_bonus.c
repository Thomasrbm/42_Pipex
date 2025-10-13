/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 01:11:53 by marvin            #+#    #+#             */
/*   Updated: 2025/10/13 06:02:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_unused_fds(struct s_shared *shared)
{
	close_all_pipes(shared);
	if (shared->here_doc)
	{
		close(shared->here_doc_pipe[0]);
		close(shared->here_doc_pipe[1]);
	}
}

void	setup_infile(struct s_shared *shared, char **av, int i)
{
	int	infile;

	if (i == 0 && ft_strcmp(av[1], "here_doc") == 0)
	{
		dup2(shared->here_doc_pipe[0], STDIN_FILENO);
		dup2(shared->pipefd[0][1], STDOUT_FILENO);
	}
	else
	{
		infile = open(av[1], O_RDONLY);
		if (infile < 0)
		{
			close_unused_fds(shared);
			free_pipes(shared);
			perror("open infile");
			exit(ERROR);
		}
		dup2(infile, STDIN_FILENO);
		dup2(shared->pipefd[0][1], STDOUT_FILENO);
		close(infile);
	}
	close_unused_fds(shared);
}

void	setup_outfile(struct s_shared *shared, char **av, int i, int cmd_count)
{
	int	outfile;

	if (ft_strcmp(av[1], "here_doc") == 0)
		outfile = open(av[SKIP_PIPEX_HEREDOC_AND_DELIMITER + cmd_count],
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		outfile = open(av[SKIP_PIPEX_AND_INFILE + cmd_count],
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close_unused_fds(shared);
		exit(ERROR);
	}
	dup2(shared->pipefd[i - 1][0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close_unused_fds(shared);
}

void	prepare_and_exec_command(struct s_shared *shared, char **av, int i)
{
	char	*absolute_path;
	char	**cmd;

	if (ft_strcmp(av[1], "here_doc") == 0)
		cmd = ft_split(av[i + SKIP_PIPEX_HEREDOC_AND_DELIMITER], ' ');
	else
		cmd = ft_split(av[i + SKIP_PIPEX_AND_INFILE], ' ');
	absolute_path = get_abs_path(shared->path, cmd[0]);
	free_pipes(shared);
	if (!absolute_path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		clean_and_exit(cmd, NULL, 127);
	}
	if (execve(absolute_path, cmd, shared->env) == -1)
	{
		perror("execve");
		clean_and_exit(cmd, absolute_path, CMD_NOT_EXECUTABLE);
	}
}

int	execute_commands(struct s_shared *shared, char **av, int i)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == 0)
	{
		free(shared->pid);
		if (i == 0)
			setup_infile(shared, av, i);
		else if (i == shared->cmd_count - 1)
			setup_outfile(shared, av, i, shared->cmd_count);
		else
		{
			dup2(shared->pipefd[i - 1][0], STDIN_FILENO);
			dup2(shared->pipefd[i][1], STDOUT_FILENO);
			close_unused_fds(shared);
		}
		prepare_and_exec_command(shared, av, i);
	}
	return (child_pid);
}
