/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:42:05 by marvin            #+#    #+#             */
/*   Updated: 2025/10/15 03:32:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"


// wait sinon processus zombie. 
// tu perd l exit code
// il prend un place dans la table de pid (plus de 4M de pid mais sur gros prog h24 ca va saturer a terme)
// 
// fait du leak 
//
// rend valgrind impossible car il attends la fin d un prog zombie

int	wait_for_children(pid_t *pid, int cmd_count) // plus dans le return du main car ligne too long 
{
	int	status;
	int	i;

	i = 0;
	while (i < cmd_count)
		waitpid(pid[i++], &status, 0);
	return (WEXITSTATUS(status)); // donne le code exit du fils SI il s est arreter normal (sinon ca fait value inusable, en general c est juste l exit code)
}

// WEXITSTATUS(status) → code de sortie (si WIFEXITED est vrai).

// WIFSIGNALED(status) → vrai si le fils est mort d’un signal.

// WTERMSIG(status) → donne le signal qui l’a tué.

// WUNTRACED = option de waitpid (et wait) voir les fils temp stop (ctrl + z) 

void	clean_and_exit_child(char **cmd, char *abs_path, int exit_code)
{
	if (cmd)
		free_split(cmd);
	if (abs_path)
		free(abs_path);
	exit(exit_code);
}

void	close_all_pipes(t_shared *shared)
{
	int	i;

	i = 0;
	if (shared->here_doc_flag)
	{
		close(shared->here_doc_pipe[READ_0]);
		close(shared->here_doc_pipe[WRITE_1]);
	}
	while (i < shared->pipecount)
	{
		close(shared->pipefd[i][READ_0]);
		close(shared->pipefd[i][WRITE_1]);
		i++;
	}
}

void	free_pipes_tab(t_shared *sh)
{
	int	i;

	if (!sh->pipefd)
		return ;
	i = 0;
	while (i < sh->pipecount)
	{
		free(sh->pipefd[i]);
		i++;
	}
	free(sh->pipefd);
	sh->pipefd = NULL;
}
