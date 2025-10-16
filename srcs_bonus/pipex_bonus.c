/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:47:09 by marvin            #+#    #+#             */
/*   Updated: 2025/10/15 18:47:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	check_infile(char *infile)
{
	int	infile_fd_exist;

	infile_fd_exist = open(infile, O_RDONLY);
	if (infile_fd_exist < 0)
	{
		perror("open: infile"); // recup errno (global qui se maj toute seul)
		return (0);
	}
	close(infile_fd_exist);
	return (1);
}

int	check_args(int ac, char **av)
{
	if (ac < 6 && ac > 1)
	{
		if (ft_strcmp(av[1], "here_doc") == 0)
		{
			ft_putstr_fd("Wrong number of arguments with heredoc, Usage : ", 2);
			ft_putstr_fd("./pipex_bonus here_doc ", 2);
			ft_putstr_fd("DELIM cmd1 cmd2 ... outfile\n", 2);
			return (0);
		}
	}
	if (ac < 5)
	{
		ft_putstr_fd("Wrong number of arguments, Usage : ", 2);
		ft_putstr_fd("./pipex_bonus infile cmd1 cmd2 ... outfile\n", 2);
		return (0);
	}
	if (ft_strcmp(av[1], "here_doc") != 0)
	{
		if (!check_infile(av[1]))
			return (0);
	}
	return (1);
}

int	main(int ac, char **av, char **env)
{
	t_shared	shared;
	int			exit_code;

	if (!check_args(ac, av))
		return (0);
	if (!init_shared(&shared, ac, av, env))
		return (1);
	exec_cmds(&shared, av);
	close_all_pipes(&shared); // close car 
	exit_code = wait_for_childrend(&shared);
	free_all(&shared, NULL);
	return (exit_code); // FIX LE CMD NOT FOUND LEAK
}

// VALGRIND les ==13046== a gauche = indique le programme qui est evalue 
//
//  en haut du valgrind : ==13046== Command: ./pipex_bonus infile cat cat ls outfile
 



// < infile cat | cat > outfile

// rafficher les msg du make


// dup2 droite pointe vers gauche 
//
// CA N ECRITT PAS
// seul les syscall write etc le font
// chaque commande y fait appel (cat, ls etc)



// pipe
// tu lui donne un int il fait un fd avec 2 case



// waitpid() , return un pid
// ici // on return le dernier exit code pas celui de tous, que du dernier enfant  ==> on return STATUS, PAS le return de waitpid

// PAS OBLI DE STOCK LE RETURN DE WAITPID, return le pid si tu veux savoir quel pid est finito, sinon tu peux l use sans stock le return juste pour wait
//
// pid = a -1 si attend n importe quel enfant
// status  = NULL si tu te fout des infos   / int status si tu veux un int qui va ter dire comme ca s est terminer ===> DONE L EXIT CODE
// option = 0 si rien  = wait que si enfant a exit,    WUNTRACED pour tracer les pid stop (ctrl + z), WLES 
