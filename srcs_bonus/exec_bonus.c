/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 23:07:02 by marvin            #+#    #+#             */
/*   Updated: 2025/10/16 05:03:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*get_absolute_path(char *path, char *cmd, t_shared *shared)
{
	char	**path_to_try;
	char	*tmp;
	int		i;

	i = 0;
	shared->abs_path = NULL;
	path_to_try = ft_split(path, ':');
	while (path_to_try[i])
	{
		tmp = ft_strjoin3(path_to_try[i], "/", cmd);
		if (access(tmp, X_OK) == 0)
		{
			shared->abs_path = tmp;
			break ;
		}
		free(tmp);
		i++;
	}
	free_split(path_to_try);
	if (!shared->abs_path)
	{
		cmd_not_found(shared->cmd, shared);
		clean_exit(shared, shared->cmd, CMD_NOT_FOUND);
	}
	return (shared->abs_path);
}

void	setup_infile(t_shared *shared)
{
	int	infile_fd;

	infile_fd = 0;
	if (shared->heredoc_flag)
	{
		handle_heredoc(shared);
		dup2(shared->heredoc_pipes[READ_0], STDIN_FILENO); 
	}
	else
	{
		infile_fd = open(shared->infile, O_RDONLY);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
	dup2(shared->pipefd[0][WRITE_1], STDOUT_FILENO);
	close_all_pipes(shared);
}

void	setup_outfile(t_shared *shared)
{
	int	outfile_fd;
	int	last_pipe;

	outfile_fd = 0;
	last_pipe = shared->pipecount - 1;
	if (shared->heredoc_flag)
		outfile_fd = open(shared->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644); // met chiffre avec o creat obli
	else
		outfile_fd = open(shared->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(shared->pipefd[last_pipe][READ_0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	close_all_pipes(shared);
}

void	ft_redirect(t_shared *shared, int i)
{
	int last_cmd;

	last_cmd = shared->cmd_count - 1;
	if (i == 0)
		setup_infile(shared);
	else if (i == last_cmd)
		setup_outfile(shared);
	else 
	{
		dup2(shared->pipefd[i - 1][READ_0] , STDIN_FILENO);
		dup2(shared->pipefd[i][WRITE_1] , STDOUT_FILENO);
		close_all_pipes(shared);  // ON PEUT CLOSE AVANT L EXEC car on a dup2 et c est que STDIN OUT  qui font les exec en shell
	}
}

// toutes tes fonctions vont rediriger vers in out out. ls, cat, printf etc. elles ont ete code ainsi en c. avec write etc.

void	exec_cmds(t_shared *shared, char **av)
{
	int		i;
	char	*abs_path;

	i = 0;
	while (i < shared->cmd_count)
	{
		shared->pid_tab[i] = fork();
		if (shared->pid_tab[i] == 0)
		{
			ft_redirect(shared, i);
			if (shared->heredoc_flag)
				shared->cmd = ft_split(av[SKIP_PIPEX_HD_DEL + i], ' ');
			else
				shared->cmd = ft_split(av[SKIP_PIPEX_INFILE + i], ' ');
			abs_path = get_absolute_path(shared->path, shared->cmd[0], shared);
			if (execve(abs_path, shared->cmd, shared->env) == -1)
			{
				perror("execve");
				free(abs_path);
				clean_exit(shared, shared->cmd, CMD_NOT_EXECUTABLE);
			}
		}
		i++;
	}
}
