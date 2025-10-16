/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:18:30 by marvin            #+#    #+#             */
/*   Updated: 2025/10/15 03:21:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pidtab(struct s_shared *shared)
{
	int	pid_nb;

	pid_nb = shared->cmd_count;
	shared->pid = malloc(sizeof(pid_t) * pid_nb);
	if (!shared->pid)
		return (1);
	return (0);
}

int	check_arguments(int ac, char **av)
{
	if (ac < 5)
	{
		ft_putstr_fd("Wrong number of arguments, Usage: ./pipex", 2);
		return (ft_putstr_fd_int(" infile cmd1 cmd2 ... cmdN outfile", 2));
	}
	if (ft_strncmp(av[1], "here_doc", 8) == 0 && ac < 6)
	{
		ft_putstr_fd("Wrong number of arguments, Usage: ./pipex", 2);
		return (ft_putstr_fd_int(" infile cmd1 cmd2 ... cmdN outfile", 2));
	}
	return (0);
}

int	free_all(t_shared *shared)
{
	int	i;
	int	exit_code;

	close_all_pipes(shared);
	if (shared->here_doc_flag)
	{
		close(shared->here_doc_pipe[READ_0]);
		close(shared->here_doc_pipe[WRITE_1]);
	}
	exit_code = wait_for_children(shared->pid, shared->cmd_count);
	i = 0;
	while (i < shared->pipecount)
		free(shared->pipefd[i++]);
	free(shared->pipefd);
	free(shared->pid);
	return (exit_code);
}

void	run_all_commands(t_shared *shared, char **av)
{
	int	actual_cmd_idx;

	actual_cmd_idx = 0;
	while (actual_cmd_idx < shared->cmd_count)
	{
		shared->pid[actual_cmd_idx] = execute_commands(shared, av,
				actual_cmd_idx); // on stock tous les pid en cours pour les waits a la fin
		actual_cmd_idx++;
	}
}

int	main(int ac, char **av, char **env)
{
	t_shared	shared;
	int			exit_code;

	if (check_arguments(ac, av))
		return (1);
	if (init_shared(&shared, env, ac, av))
		return (1);
	if (init_pidtab(&shared))
		return (1);
	if (ft_strcmp(av[1], "here_doc") == 0)
	{
		if (handle_here_doc(&shared, av[2]))
			return (1);
		close(shared.here_doc_pipe[1]);
	}
	run_all_commands(&shared, av);
	exit_code = free_all(&shared);
	get_next_line(-1);
	return (exit_code); // < infile cat | grep foo | grep bar => montre que ca marche bien
}

// mettre message d erreur si infile exite pas :  
// -bash: d: No such file or directory

// pk faut un pipe a part pour le heredoc  ===> j avais pas envie de modif mes multi pipe alors j ai juste add par dessus en independant le heredoc

// comment se stack les pid et pipe


// pipe pour partage d un proco a l autre. les pipe sont des fd

// pid = les proco (0 pour les enfants) et le proco de l enfant dans le parent.

// les fork pour separer et donner la valeur au pi
 