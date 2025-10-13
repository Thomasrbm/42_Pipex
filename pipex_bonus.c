/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:18:30 by marvin            #+#    #+#             */
/*   Updated: 2025/10/09 14:18:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int init_pidtab(struct s_shared *shared)
{
    int     pid_nb;
    
    pid_nb = shared->pipecount + LASTCMD;
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
		return (ft_putstr_fd_int(" infile cmd1 cmd2 ... cmdN outfile\n", 2));
	}
	if (ft_strncmp(av[1], "here_doc", 8) == 0 && ac < 6)
	{
		ft_putstr_fd("Wrong number of arguments, Usage: ./pipex", 2);
		return (ft_putstr_fd_int(" infile cmd1 cmd2 ... cmdN outfile\n", 2));
	}
	return (0);
}

void    free_all(t_shared *shared)
{
    int i;
    
    close_all_pipes(shared);
    if (shared->here_doc)
    {
        close(shared->here_doc_pipe[0]);
        close(shared->here_doc_pipe[1]);
    }
    wait_for_children(shared->pid, shared->cmd_count);
    i = 0;
    while (i < shared->pipecount)
        free(shared->pipefd[i++]);
    free(shared->pipefd);
    free(shared->pid);
}

void    run_all_commands(t_shared *shared, char **av)
{
    int i;
    
    i = 0;
    while (i < shared->cmd_count)
    {
        shared->pid[i] = execute_commands(shared, av, i);
        i++;
    }
}

int main(int ac, char **av, char **env)
{
    t_shared    shared;
    
    if (check_arguments(ac, av))
        return (1);
    if (init_shared(&shared, env, ac, av))
        return (1);
    if (init_pidtab(&shared))
        return (1);
    if (ft_strncmp(av[1], "here_doc", 8) == 0)
    {
        if (handle_here_doc(&shared, av[2]))
            return (1);
        close(shared.here_doc_pipe[1]);
    }
    run_all_commands(&shared, av);
    free_all(&shared);
    return (0);
}

// mettre message d erreur si infile exite pas :   -bash: d: No such file or directory

// pk faut un pipe a part pour le heredoc 

// comment se stack les pid et pipe 