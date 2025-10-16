/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 01:11:53 by marvin            #+#    #+#             */
/*   Updated: 2025/10/15 19:00:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	setup_infile(struct s_shared *shared, char **av)
{
	int	infile;

	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		dup2(shared->here_doc_pipe[READ_0], STDIN_FILENO);
		dup2(shared->pipefd[0][WRITE_1], STDOUT_FILENO);
	}
	else
	{
		infile = open(av[1], O_RDONLY);
		if (infile < 0)
		{
			close_all_pipes(shared);
			free_pipes_tab(shared);
			perror("open: infile");
			exit(ERROR);
		}
		dup2(infile, STDIN_FILENO);
		dup2(shared->pipefd[0][WRITE_1], STDOUT_FILENO);
		close(infile);
	}
	close_all_pipes(shared);
}

void	setup_outfile(struct s_shared *shared, char **av, int cmd_count)
{
	int	outfile;
	int	last_pipe;

	last_pipe = shared->pipecount - 1;
	if (ft_strcmp(av[1], "here_doc") == 0)
		outfile = open(av[SKIP_PIPEX_HEREDOC_AND_DELIMITER + cmd_count],
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		outfile = open(av[SKIP_PIPEX_AND_INFILE + cmd_count],
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close_all_pipes(shared);
		exit(ERROR);
	}
	dup2(shared->pipefd[last_pipe][READ_0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close_all_pipes(shared);
}

void	prepare_and_exec_command(struct s_shared *shared, char **av,
			int actual_cmd_idx)
{
	char	*absolute_path;
	char	**cmd;

	if (ft_strcmp(av[1], "here_doc") == 0)
		cmd = ft_split(av[SKIP_PIPEX_HEREDOC_AND_DELIMITER + actual_cmd_idx],
				' ');
	else
		cmd = ft_split(av[SKIP_PIPEX_AND_INFILE + actual_cmd_idx],
				' ');
	absolute_path = get_abs_path(shared->path, cmd[0]);
	free_pipes_tab(shared);
	if (!absolute_path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		clean_and_exit_child(cmd, NULL, CMD_NOT_FOUND);
	}
	if (execve(absolute_path, cmd, shared->env) == -1)
	{
		perror("execve");
		clean_and_exit_child(cmd, absolute_path, CMD_NOT_EXECUTABLE);
	}
}

int	execute_commands(struct s_shared *shared, char **av, int actual_cmd_idx)
{
	pid_t	child_pid;
	int		last_cmd;
	int		prev_pipe_toread;

	last_cmd = shared->cmd_count - 1;
	prev_pipe_toread = actual_cmd_idx - 1;
	child_pid = fork();
	if (child_pid == 0)
	{
		free(shared->pid);
		if (actual_cmd_idx == 0)
			setup_infile(shared, av);
		else if (actual_cmd_idx == last_cmd)
			setup_outfile(shared, av, shared->cmd_count);
		else
		{
			dup2(shared->pipefd[prev_pipe_toread][READ_0], STDIN_FILENO);
			dup2(shared->pipefd[actual_cmd_idx][WRITE_1], STDOUT_FILENO);
			close_all_pipes(shared);
		}
		prepare_and_exec_command(shared, av, actual_cmd_idx);
	}
	return (child_pid); // pour pouvoir le wait. le pid dit tel process est en cours.
}

// pipe[0] = lecture, 1 ecriture. 

// dup2 = droite devient gauche

// dup2 (old, new);  .
// ferme le fd de new
// new pointe mtn sur le meme fd que old.

// un fd =   un int qui pointer vers un objet valide pour etre un fd (socket, fichier, pipe etc.)
// alias + objet.
// dup2 = permet de faire pointer droite vers gauche.

// alias POUR UN FD = pas comme en cpp. tu peux rien faire. un pointeur  tu peux l assginer c est un adresse mais tu peux manip a l adresse voulu.
// ici tu peux rien faire, tu peux juste use execve avec et il fera les syscall (write etc) pour faire le truc.

// le shell va toujour exec avec stdin et stdout mais comme stdin pointe vers infile et l autre vers outfile ca va faire la redirection.
// c est pas le shelle qui va changer de fd . shell = toujours use ces 2 la.