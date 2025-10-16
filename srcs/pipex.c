/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 23:05:51 by throbert          #+#    #+#             */
/*   Updated: 2025/10/16 04:23:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_infile(char *infile)
{
	int	infile_fd_exist;

	infile_fd_exist = open(infile, O_RDONLY);
	if (infile_fd_exist < 0)
	{
		perror("open: infile");
		return (0);
	}
	close(infile_fd_exist);
	return (1);
}

int	check_args(int ac, char **av)
{
	if (ac != 5)
	{
		ft_putstr_fd("Wrong number of arguments, Usage : ", 2);
		ft_putstr_fd("./pipex infile cmd1 cmd2 outfil\n", 2);
		return (0);
	}
	if (!check_infile(av[1]))
		return (0);
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_shared	shared;
	int			exit_code;
	pid_t		pid[2];

	if (!check_args(ac, av))
		return (1);
	if (!init_shared(&shared, env))
		return (1);
	pid[0] = exec_cmds(&shared, av, 1);
	pid[1] = exec_cmds(&shared, av, 2);
	close_pipes(&shared); // close les pipe sinon pas EOF => cmd bloaquante car les pipe *
	exit_code = wait_for_children(pid);
	return (exit_code); // tester avec flags
}

// * le noyau bloque read sur fd 0
// fd 0 ferme quand fd 1 ferme avec close (recoit EOF)
// sinon le prog est en bloquant

// faut fermer quand meme fd 0 pour les fd ouverts
